#include "ClientPCH.h"
#include "Monster_Twince.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Twince_FSM.h"
#include "Part.h"
#include "Monster_Weak.h"

#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_MissPlayer.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#pragma endregion

#include "Effect_Manager.h"

#include "Bone.h"

CMonster_Twince::CMonster_Twince(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_Twince::CMonster_Twince(const CMonster_Twince& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_Twince::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_TWINCE, g_aNull);

	return S_OK;
}

HRESULT CMonster_Twince::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Twince::Initialize", "Failed to CMonster::Initialize");
	}
	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Twince::Initialize", "Failed to CMonster::Ready_Behaviors");
	}

	m_eMonsterType = MIMIC;
	m_tEntityDesc.fHP = 20.f;
	m_tEntityDesc.fMaxHP = 20.f;
	m_tEntityDesc.fKnuckBackGauge = m_tEntityDesc.fMaxHP;

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
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
	else
	{
		cDesc.position = { 0.f,1000.f,0.f };
		m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 1000.f, 0.f, 0.f));
	}

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::MONSTER;
	m_pController->getActor()->userData = &m_ActorDesc;

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster::SetWeaponeActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CMonster_Twince::SetRenderBodyFalse, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION4, std::bind(&CMonster_Twince::SetRenderBodyTrue, this));

	m_vControllerPivot = PxExtendedVec3(0.f, 0.f, 0.f);

	m_pFSM = CTwince_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 1.f;
	desc.fCapsuleHeight = 15.f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	actorDesc.stAttackDesc.eHurtType = PLAYER_HURTTYPE::AIR;
	actorDesc.stAttackDesc.iType = (_uint)CMonster::MONSTERTYPE::MIMIC;
	m_pAttackCollider->Initialize(desc, actorDesc);


	m_pBigAttackCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 2.f;
	desc.fCapsuleHeight = 16.f;

	actorDesc.strTag = ACTORTAG::MONSTER_BIGATTACK;
	actorDesc.stAttackDesc.iDamage = 0.f;
	m_pBigAttackCollider->Initialize(desc, actorDesc);


	m_pBodyCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleHeight = 1.f;
	desc.fCapsuleRadius = 1.2f;

	ACTORDESC bodyActorDesc;
	bodyActorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	m_pBodyCollider->Initialize(desc, bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;

	m_pAttackActorDesc->bActive = false;

	m_strAttackBoneName = "HeadTenctacleB9";

	CPart::PARTDESC BoxDesc;
	BoxDesc.PivotMatrix = m_pModel->Get_Pivot() * XMMatrixTranslation(0.f, -0.6f, 0.f);
	BoxDesc.pParentMatrix = m_pTransform->Get_WorldMatrixPtr();
	BoxDesc.pWeakBoneMatrix = m_pModel->Get_Bone("Box")->Get_CombinedTransformationPointer();

	m_pTwinceBox = CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_PART_TWINCEBOX, BoxDesc);
	
	
	BoxDesc.PivotMatrix = m_pModel->Get_Pivot() * XMMatrixTranslation(0.f, 0.f, 0.f);
	BoxDesc.pParentMatrix = m_pTransform->Get_WorldMatrixPtr();
	BoxDesc.pWeakBoneMatrix = m_pModel->Get_Bone("Weak01")->Get_CombinedTransformationPointer();
	BoxDesc.bNeon = true;

	m_pWeak = CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_PART_WEAK, BoxDesc);

#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion

	//CMonster_Weak::WEAKDESC WeakDesc;
	//WeakDesc.PivotMatrix = m_pModel->Get_Pivot();
	//WeakDesc.pParentMatrix = m_pTransform->Get_WorldMatrixPtr();
	//WeakDesc.pWeakBoneMatrix = m_pModel->Get_Bone("Weak01")->Get_CombinedTransformationPointer();
	//WeakDesc.ControllerPivot = _float4(m_vControllerPivot.x, m_vControllerPivot.y, m_vControllerPivot.z, 0.f);
	//
	//m_pWeak = CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_WEAK, WeakDesc);

	return S_OK;
}

void CMonster_Twince::Tick(_float fTimeDelta)
{
	m_pTwinceBox->Tick(fTimeDelta);

	/*
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
	Laser.r[3] += Laser.r[2] * 15 * 100;
	XMStoreFloat4x4(&WeaponMat, Laser);

	PxTransform p;
	p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
	_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
	p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	m_pAttackCollider->GetActor()->setGlobalPose(p);
	*/

	//__super::Tick(fTimeDelta);
	CGameObject::Tick(fTimeDelta);
	if (m_pNormalActorDesc->bThrowHit = m_pBigActorDesc->bThrowHit)
		m_pNormalActorDesc->stHitDesc = m_pBigActorDesc->stHitDesc;

	if (m_pFSM)
	{
		m_pFSM->Tick(fTimeDelta);
	}

	_matrix BoneMatrix = m_pModel->Get_Bone(m_strCenterBoneName.c_str())->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_matrix PivotMatrix = m_pModel->Get_Pivot();

	_float4x4 WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* PivotMatrix
		* m_pTransform->Get_Matrix();

	if (m_pController)
	{
		PxTransform p;
		p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
		_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
		p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
		m_pController->getActor()->setGlobalPose(p);
		m_pBodyCollider->GetActor()->setGlobalPose(p);

		m_pNormalActorDesc->bHit = false;
		m_pNormalActorDesc->bAirBone = false;
		m_pBigActorDesc->bThrowHit = false;

		BoneMatrix = m_pModel->Get_Bone(m_strAttackBoneName.c_str())->Get_CombinedTransformation();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
		BoneMatrix.r[3] += BoneMatrix.r[0] * 15 * 100;

		PivotMatrix = m_pModel->Get_Pivot();

		WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
			BoneMatrix
			* PivotMatrix
			* m_pTransform->Get_Matrix();

		//_matrix Laser = XMLoadFloat4x4(&WeaponMat);
		//Laser.r[3] += Laser.r[2] * 15;owner
		//XMStoreFloat4x4(&WeaponMat, Laser);

		p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
		vQuat = XMQuaternionRotationMatrix(WeaponMat);
		p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
		m_pAttackCollider->GetActor()->setGlobalPose(p);
		m_pAttackCollider->GetActorDesc().vOwnerPose = m_pTransform->Get_State(TRANSFORM::POSITION);
		m_pAttackCollider->UpdateActorDesc();
		m_pBigAttackCollider->GetActor()->setGlobalPose(p);
	}
	m_pWeak->Tick(fTimeDelta);

#ifdef _DEBUG
	//if (CGameInstance::Get_Instance()->Key_Down(VK_END)) Dissolve();
#if ACTIVATE_IMGUI
//	ImGui::Begin("Dissolve Test");
//	if (ImGui::Button("Apply")) Dissolve();
//	ImGui::End();
#endif
#endif
	if (m_bDissolve)
	{
		m_fDissolveThreshold += fTimeDelta / m_fDissolveDuration;
	}

#pragma region UI
	m_pUI_Hp->Tick(fTimeDelta);
#pragma endregion

	/*ImGui::Begin("Material Settings");
	if (ImGui::CollapsingHeader("Twince Material"))
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

	if (!m_bRenderBody)
	{
		if (!m_bEffect)
		{
			_float4x4 Mat = XMMatrixScaling(2.f, 2.f, 2.f) * m_pTransform->Get_Matrix();
			Mat._42 += 1.25f;
			CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED0, Mat);
			CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED1, Mat);
			CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED2, Mat);
			CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_APPEAR0, Mat);
			m_bEffect = !m_bEffect;
		}
	}
	else
	{
		if (m_bEffect)
		{
			_float4x4 Mat = XMMatrixScaling(2.f, 2.f, 2.f) * m_pTransform->Get_Matrix();
			Mat._42 += 1.25f;
			CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED0, Mat);
			CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED1, Mat);
			CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED2, Mat);
			CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_APPEAR0, Mat);
			m_bEffect = !m_bEffect;
		}
	}
}

void CMonster_Twince::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	m_pTwinceBox->Late_Tick(fTimeDelta);
	if(m_bRenderBody)
		Add_RenderObject(RENDER_GROUP::NONBLEND);
	//	Add_RenderObject(RENDER_GROUP::BLOOM);

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

HRESULT CMonster_Twince::Render()
{
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

HRESULT CMonster_Twince::Render_OutLine()
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
		MSG_RETURN(E_FAIL, "CMonster_Twince::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

void CMonster_Twince::SetRenderBodyTrue()
{
	CGameInstance::Get_Instance()->PlaySoundW(L"Monster_Mimic_BoxOpen.wav", 3.f);
	m_bRenderBody = true;
}

void CMonster_Twince::SetRenderBodyFalse()
{
	CGameInstance::Get_Instance()->PlaySoundW(L"Monster_Mimic_BoxOpen.wav", 3.f);
	m_bRenderBody = false;
}

void CMonster_Twince::Dissolve()
{
	__super::Dissolve();

	dynamic_pointer_cast<CPart>(m_pTwinceBox)->Dissolve();
}

HRESULT CMonster_Twince::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Twince::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Twince::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Twince::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Twince::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

HRESULT CMonster_Twince::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	return S_OK;
}

shared_ptr<CMonster_Twince> CMonster_Twince::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Twince> pInstance = make_private_shared(CMonster_Twince, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Twince::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Twince::Clone(any pArg)
{
	shared_ptr<CMonster_Twince> pInstance = make_private_shared_copy(CMonster_Twince, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Twince::Clone", "Failed to Initialize");
	}

	return pInstance;
}
