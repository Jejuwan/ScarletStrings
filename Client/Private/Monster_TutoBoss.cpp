#include "ClientPCH.h"
#include "Monster_TutoBoss.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "PlayerIdleState.h"
#include "TutoBoss_FSM.h"
#include "Bone.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#pragma endregion

CMonster_TutoBoss::CMonster_TutoBoss(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_TutoBoss::CMonster_TutoBoss(const CMonster_TutoBoss& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_TutoBoss::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS, g_aNull);

	return S_OK;
}

HRESULT CMonster_TutoBoss::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Buffalo::Initialize", "Failed to CGameObject::Initialize");
	}
	m_eMonsterType = TUTOBOSS;


	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,1000.f,0.f };
	cDesc.height = 2.f;
	cDesc.radius = 1.f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = NULL;

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

	m_tEntityDesc.fMaxHP = 100.f;
	m_tEntityDesc.fHP = 100.f;

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster_TutoBoss::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster_TutoBoss::SetWeaponeActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CMonster_TutoBoss:: SetAttackBoneAndSwap1, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION4, std::bind(&CMonster_TutoBoss:: SetAttackBoneAndSwap2, this));
	
	m_pFSM = CTutoBoss_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pAttack1Collider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 1.f;
	desc.fCapsuleHeight = 1.5f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	m_pAttack1Collider->Initialize(desc, actorDesc);

	m_pAttack2Collider = make_shared<CPhysXCollider>();
	
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 1.f;
	desc.fCapsuleHeight = 15.f;

	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.eHurtType = PLAYER_HURTTYPE::AIR;
	m_pAttack2Collider->Initialize(desc, actorDesc);


	m_pBigAttackCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 2.f;
	desc.fCapsuleHeight = 16.f;

	actorDesc.strTag = ACTORTAG::MONSTER_BIGATTACK;
	actorDesc.stAttackDesc.iDamage = 0.f;
	m_pBigAttackCollider->Initialize(desc, actorDesc);



	m_pBodyCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 2.f;
	desc.fCapsuleHeight = 2.f;

	ACTORDESC bodyActorDesc;
	bodyActorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	m_pBodyCollider->Initialize(desc, bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;

	m_pAttack1ActorDesc = (ACTORDESC*)m_pAttack1Collider->GetActor()->userData;
	m_pAttack2ActorDesc = (ACTORDESC*)m_pAttack2Collider->GetActor()->userData;

	m_pAttackCollider = m_pAttack1Collider;
	m_pAttackActorDesc = m_pAttack1ActorDesc;

	m_pNormalActorDesc->bActive = false;
	m_pAttack1ActorDesc->bActive = false;
	m_pAttack2ActorDesc->bActive = false;

	m_strAttackBoneName = "Head";
	m_strWeakBoneName = "LeftForeArm";

#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion

	return S_OK;
}

void CMonster_TutoBoss::Tick(_float fTimeDelta)
{
	_matrix BoneMatrix = m_pModel->Get_Bone(m_strAttackBoneName.c_str())->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_vector Position = BoneMatrix.r[3];
	BoneMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	BoneMatrix *= XMMatrixRotationY(XMConvertToRadians(90.f));
	BoneMatrix.r[3] = Position;
	_matrix PivotMatrix = m_pModel->Get_Pivot();

	_float4x4 WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* PivotMatrix
		* m_pTransform->Get_Matrix();

	_matrix Laser = XMLoadFloat4x4(&WeaponMat);
	Laser.r[3] += m_pTransform->Get_State(TRANSFORM::LOOK) * 15;
	XMStoreFloat4x4(&WeaponMat, Laser);

	PxTransform p;
	p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
	_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
	p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	m_pAttack2Collider->GetActor()->setGlobalPose(p);
	__super::Tick(fTimeDelta);
	m_pBigAttackCollider->GetActor()->setGlobalPose(p);

	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("TutoBoss Material"))
	//{
	//	_uint iNumMesh = m_pModel->Get_NumMeshes();
	//	for (_uint i = 0; i < iNumMesh; ++i)
	//	{
	//		string MeshName = m_pModel->Get_MeshName(i);
	//		if (ImGui::TreeNode(MeshName.c_str()))
	//		{
	//			SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//			_bool bDirtyFlag = false;
	//			if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
	//				bDirtyFlag = true;

	//			if (bDirtyFlag)
	//			{
	//				m_pModel->Set_ShaderDesc(i, tShaderDesc);
	//			}

	//			ImGui::TreePop();
	//		}
	//	}

	//	if (ImGui::Button("Save"))
	//	{
	//		m_pModel->Export_ShaderMaterial();
	//	}
	//}

	//ImGui::End();

#pragma region UI
	m_pUI_Hp->Tick(fTimeDelta);
#pragma endregion
}

void CMonster_TutoBoss::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	if (false == m_bDissolve)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
	
	//Add_RenderObject(RENDER_GROUP::OUTLINE);

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

	if (m_fBloomStrength >= m_fBloomStrengthThreshold)
	{
		m_fBloomStrength -= fTimeDelta * 30.f;
	}
}

HRESULT CMonster_TutoBoss::Render()
{
	/*if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}*/

	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render", "Failed to Bind_LightDirection");
	}

	if (FAILED(__super::Render(16)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMonster_TutoBoss::Render_Bloom()
{
	if (FAILED(m_pModel->Bind_BoneMatrices(2, m_pShader, SHADER_BONE)))
	{
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Render_Bloom", "Failed to Bind_BoneMatrices");
	}
	if (FAILED(m_pShader->Bind_Float("g_fBloomStrength", m_fBloomStrength)))
	{
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Render_Bloom", "Failed to Bind BloomStrength");
	}
	if (FAILED(Bind_Transform(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Render_Bloom", "Failed to Bind_Transform");
	}

	if (FAILED(m_pModel->Render(2, m_pShader, 6)))
	{
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Render_Bloom", "Failed to Render");
	}

	return S_OK;
}

void CMonster_TutoBoss::RootMotion(_float fTimeDelta, _bool OneFrame)
{
	//이쯤되면 이게 맞는 피벗인가?
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

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));

	m_vPreRootPos = m_pModel->Get_RootPos();
}

HRESULT CMonster_TutoBoss::Render_OutLine()
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
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

void CMonster_TutoBoss::SetWeaponeActiveOn()
{
	m_pAttackActorDesc->bActive = true;
	m_pAttack1ActorDesc->bActive = true;
	m_pAttack2ActorDesc->bActive = true;
}

void CMonster_TutoBoss::SetWeaponeActiveOff()
{
	m_pAttackActorDesc->bActive = false;
	m_pAttack1ActorDesc->bActive = false;
	m_pAttack2ActorDesc->bActive = false;
}

void CMonster_TutoBoss::SetAttackBoneAndSwap1()
{
	m_pAttackCollider = m_pAttack1Collider;
	m_pAttackActorDesc = m_pAttack1ActorDesc;
	m_pAttackActorDesc->bActive = true;
	m_strAttackBoneName = "Head";
}

void CMonster_TutoBoss::SetAttackBoneAndSwap2()
{
	m_pAttackActorDesc = m_pAttack2ActorDesc;
	m_pAttackActorDesc = m_pAttack2ActorDesc;
	m_pAttackActorDesc->bActive = true;
	m_strAttackBoneName = "Eff02";
}

void CMonster_TutoBoss::On_HitBlink()
{
	m_fBloomStrength = 30.f;
}

HRESULT CMonster_TutoBoss::Ready_Components()
{
	if (FAILED(CGameObject::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_TutoBoss::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

shared_ptr<CMonster_TutoBoss> CMonster_TutoBoss::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_TutoBoss> pInstance = make_private_shared(CMonster_TutoBoss, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_TutoBoss::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_TutoBoss::Clone(any pArg)
{
	shared_ptr<CMonster_TutoBoss> pInstance = make_private_shared_copy(CMonster_TutoBoss, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_TutoBoss::Clone", "Failed to Initialize");
	}

	return pInstance;
}
