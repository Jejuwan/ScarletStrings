#include "ClientPCH.h"
#include "Monster_Snail.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "PlayerIdleState.h"
#include "Effect_Manager.h"
#include "Bone.h"
#include "Snail_FSM.h"
#include "Part.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#pragma endregion

CMonster_Snail::CMonster_Snail(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_Snail::CMonster_Snail(const CMonster_Snail& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_Snail::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_SNAIL, g_aNull);

	m_tEntityDesc.fAtk = 0.f;

	m_tEntityDesc.fForwardSpeed = 60.f;
	m_tEntityDesc.fBackwardSpeed = 60.f;
	m_tEntityDesc.fRightSpeed = 60.f;
	m_tEntityDesc.fSpritPower = 1.f;
	m_tEntityDesc.fJumpPower = 60.f;
	m_tEntityDesc.fLeftSpeed = 60.f;

	m_tEntityDesc.vMaxSpeed = _float3(200.f, 200.f, 200.f);
	m_tEntityDesc.vResist = _float3(0.02f, 0.2f, 0.02f);

	return S_OK;
}

HRESULT CMonster_Snail::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Snail::Initialize", "Failed to CGameObject::Initialize");
	}
	m_eMonsterType = SNAIL;

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();


	if (pGameInstance->Current_Scene() == SCENE::TEST)
	{
		m_tEntityDesc.fMaxHP = m_tEntityDesc.fHP = 9;
		m_tEntityDesc.fKnuckBackGauge = 999999;
	}
	else
		m_tEntityDesc.fKnuckBackGauge = m_tEntityDesc.fHP = m_tEntityDesc.fMaxHP = 20;


	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,1000.f,0.f };
	cDesc.height = 2.f;
	cDesc.radius = 1.f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = m_pMonsterControllerHitReport.get();

	if (pGameInstance->Current_Scene() == SCENE::REALTEST)
	{
		_float4 InitPos = any_cast<_float4>(Any);
		InitPos.y += 1.f;

		m_pTransform->Set_State(TRANSFORM::POSITION, InitPos);
		m_pTransform->Rotate(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(InitPos.w));
		InitPos.w = 1.f;
		cDesc.position = { InitPos.x,InitPos.y,InitPos.z };
	}

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::MONSTER;
	m_pController->getActor()->userData = &m_ActorDesc;

	//m_eCurState = make_shared<CPlayerIdleState>();
	//m_eCurState->Entry(*this);

	/*m_pCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 2.f;
	m_pCollider->Initialize(desc);

	pGameInstance->Set_Player(this);*/

	//Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS)->Set_Gravity(false);
	m_pTexDissolve = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_043.dds"), 1, true);


	CPart::PARTDESC BoxDesc;
	BoxDesc.PivotMatrix = m_pModel->Get_Pivot() * XMMatrixTranslation(0.f, 0.f, 0.f);
	BoxDesc.pParentMatrix = m_pTransform->Get_WorldMatrixPtr();
	BoxDesc.pWeakBoneMatrix = m_pModel->Get_Bone("Weak01")->Get_CombinedTransformationPointer();
	BoxDesc.bNeon = true;

	m_pWeak = CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_PART_WEAK, BoxDesc);

	m_pProjectile = CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_PROJECTILE_WATERDROP);
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER_PROJECTILE);
	pLayer->Add(m_pProjectile);

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Weak01"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster::SetWeaponeActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CMonster_Snail::ShootBulletToPlayer, this));

	m_pFSM = CSnail_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 3.f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	actorDesc.stAttackDesc.iType = (_uint)CMonster::SNAIL;
	m_pAttackCollider->Initialize(desc, actorDesc);

	m_pBigAttackCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 4.f;

	actorDesc.strTag = ACTORTAG::MONSTER_BIGATTACK;
	actorDesc.stAttackDesc.iDamage = 0.f;
	m_pBigAttackCollider->Initialize(desc, actorDesc);

	m_pBodyCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 1.2f;
	desc.fCapsuleHeight = 2.4f;

	ACTORDESC bodyActorDesc;
	bodyActorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	m_pBodyCollider->Initialize(desc, bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;

	m_pAttackActorDesc->bActive = false;

	m_strAttackBoneName = "Reference";
	m_strCenterBoneName = "Chest";

#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion

	return S_OK;
}

void CMonster_Snail::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	// Material Settings
	/*ImGui::Begin("Material Settings");
	if (ImGui::CollapsingHeader("Snail Material"))
	{
		_uint iNumMesh = m_pModel->Get_NumMeshes();
		for (_uint i = 0; i < iNumMesh; ++i)
		{
			string MeshName = m_pModel->Get_MeshName(i);
			if (ImGui::TreeNode(MeshName.c_str()))
			{
				SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
				_bool bDirtyFlag = false;
				if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
					bDirtyFlag = true;

				if (bDirtyFlag)
				{
					m_pModel->Set_ShaderDesc(i, tShaderDesc);
				}

				ImGui::TreePop();
			}
		}

		if (ImGui::Button("Save"))
		{
			m_pModel->Export_ShaderMaterial();
		}
	}

	ImGui::End();*/

#pragma region UI
	m_pUI_Hp->Tick(fTimeDelta);
#pragma endregion
}

void CMonster_Snail::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//	Add_RenderObject(RENDER_GROUP::OUTLINE);

#pragma region UI
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_CAMERA);
	m_pCamera_Player = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));

	m_pUI_Hp->Set_MonsterHp(m_tEntityDesc.fHP, m_tEntityDesc.fMaxHP);
	m_pUI_Hp->Set_MonsterNameTag(Get_TypeName());
	m_pUI_Hp->Late_Tick(fTimeDelta);
	if (m_tEntityDesc.fHP < m_tEntityDesc.fMaxHP)
	{
		m_pUI_Hp->Set_IndicatorPosition(m_pCamera_Player->Get_State(TRANSFORM::POSITION), this->m_pTransform->Get_WorldMatrix());
	}
#pragma endregion
}

HRESULT CMonster_Snail::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render", "Failed to Bind_LightDirection");
	}

	if (FAILED(__super::Render(18)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CMonster_Snail::RootMotion(_float fTimeDelta, _float fAmount, _bool OneFrame)
{
	if (m_bBulldozerHit)
		return;

	_vector vRot = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector v = XMQuaternionRotationMatrix(XMMatrixRotationX(XMConvertToRadians(90.f)) * m_pTransform->Get_WorldMatrix());
	_matrix m = XMMatrixRotationQuaternion(v);
	_float4 vUpdatePos;
	vUpdatePos = m_pTransform->Get_State(TRANSFORM::POSITION);
	//cout << vUpdatePos.x<< '\t' << vUpdatePos.y << '\t' << vUpdatePos.z << endl;
	_vector MoveDir = XMVector3TransformNormal((m_vPreRootPos - m_pModel->Get_RootPos()), m);
	//cout << XMVectorGetX(m_pModel->Get_RootPos()) << '\t' << XMVectorGetY(m_pModel->Get_RootPos()) << '\t' << XMVectorGetZ(m_pModel->Get_RootPos()) << endl;
	//cout << XMVectorGetX(MoveDir) << '\t' << XMVectorGetY(MoveDir) << '\t' << XMVectorGetZ(MoveDir) << endl;

	vUpdatePos += MoveDir;
	MoveDir *= fAmount;

	vUpdatePos.w = 1.f;

	_float4 vPosition = { vUpdatePos.x,vUpdatePos.y,vUpdatePos.z,1.f };
	m_pTransform->Set_State(TRANSFORM::POSITION, XMLoadFloat4(&vPosition));

	_vector vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	_float3 vDisp;
	XMStoreFloat3(&vDisp, vPos - m_vPrePos);
	//cout << vDisp.x<< '\t' << vDisp.y << '\t' << vDisp.z << endl;
	m_CollisionFlag = m_pController->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, m_ControllerFilters);
	if (m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_fTime = 0.f;
	}

	PxExtendedVec3 vControllerPos = m_pController->getFootPosition() + m_vControllerPivot;

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((_float)vControllerPos.x, (_float)vControllerPos.y, (_float)vControllerPos.z, 1.f));

	m_vPreRootPos = m_pModel->Get_RootPos();
}

HRESULT CMonster_Snail::Render_OutLine()
{
	if (FAILED(m_pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutLineColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(CGameObject::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Snail::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

void CMonster_Snail::ShootBulletToPlayer()
{
	__super::ShootBulletToPlayer();
	//effect
	_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
	_float3 Look = m_pTransform->Get_State(TRANSFORM::LOOK);
	Pos.y += 1.f;

	CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_SHOOTING0, Pos + Look);
}

HRESULT CMonster_Snail::Ready_Components()
{
	if (FAILED(CGameObject::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Snail::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Snail::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Snail::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Snail::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

HRESULT CMonster_Snail::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Snail::Ready_Behaviors", "Failed to CMonster::Ready_Behaviors");
	}

	return S_OK;
}

shared_ptr<CMonster_Snail> CMonster_Snail::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Snail> pInstance = make_private_shared(CMonster_Snail, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Snail::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Snail::Clone(any pArg)
{
	shared_ptr<CMonster_Snail> pInstance = make_private_shared_copy(CMonster_Snail, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Snail::Clone", "Failed to Initialize");
	}

	return pInstance;
}
