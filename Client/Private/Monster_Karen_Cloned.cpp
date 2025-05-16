#include "ClientPCH.h"
#include "Monster_Karen_Cloned.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Karen_FSM.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#pragma endregion

#include "Bone.h"
#include "KarenControllerFilterCallBack.h"

CMonster_Karen_Cloned::CMonster_Karen_Cloned(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_Karen_Cloned::CMonster_Karen_Cloned(const CMonster_Karen_Cloned& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_Karen_Cloned::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN, g_aNull);

	return S_OK;
}

HRESULT CMonster_Karen_Cloned::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Initialize", "Failed to CMonster::Initialize");
	}
	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Initialize", "Failed to CMonster::Ready_Behaviors");
	}
	m_eMonsterType = Karen;
	m_tEntityDesc.fHP = m_tEntityDesc.fMaxHP = m_tEntityDesc.fKnuckBackGauge = 20;

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

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::MONSTER;
	m_pController->getActor()->userData = &m_ActorDesc;

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster::SetWeaponeActiveOff, this));
	//m_pModel->SetRootBoneAddIndex(m_pModel->Get_BoneIndex("Hips"));

	m_vControllerPivot = PxExtendedVec3(0.f, 0.f, 0.f);

	//m_pFSM = CKaren_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 2.f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	m_pAttackCollider->Initialize(desc, actorDesc);


	m_pBigAttackCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 2.5;

	actorDesc.strTag = ACTORTAG::MONSTER_BIGATTACK;
	actorDesc.stAttackDesc.iDamage = 0.f;
	m_pBigAttackCollider->Initialize(desc, actorDesc);


	m_pBodyCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleHeight = 3.4f;
	desc.fCapsuleRadius = 2.2f;

	ACTORDESC bodyActorDesc;
	bodyActorDesc.strTag = ACTORTAG::KAREN_BIGBODY;
	m_pBodyCollider->Initialize(desc, bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;
	m_pBigAttackActorDesc = (ACTORDESC*)m_pBigAttackCollider->GetActor()->userData;

	m_pAttackActorDesc->bActive = false;

	m_strAttackBoneName = "Reference";
	m_mAttackColliderPivot = XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 2.f, 3.f);
#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion

	m_vecMeshPassIndexPair.push_back({ 0, 17 }); // AMB
	//m_vecMeshPassIndexPair.push_back({ 2, 17 }); // AMB
	m_vecMeshPassIndexPair.push_back({ 3, 17 }); // AMB
	m_vecMeshPassIndexPair.push_back({ 4, 17 }); // AMB

	m_vecMeshPassIndexPair.push_back({ 1, 0 });
	//m_vecMeshPassIndexPair.push_back({ 5, 0 });
	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, -1000.f, 0.f, 1.f));

	m_ControllerFilterCallBack = make_shared<CKarenControllerFilterCallBack>();
	m_ControllerFilters.mFilterCallback = m_ControllerFilterCallBack.get();
	//m_CCTFilterCallBack = make_shared<CKarenCCTFilterCallback>();
	m_ControllerFilters.mCCTFilterCallback = NULL/*m_CCTFilterCallBack.get()*/;
	m_ControllerFilters.mFilterFlags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;
	m_ControllerFilters.mFilterData = NULL;


	PxSetGroup(*m_pController->getActor(), (_uint)ACTORTAG::MONSTER);
	return S_OK;
}

void CMonster_Karen_Cloned::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_COPY_START))
	{
		RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_COPY_START))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_COPY_LOOP, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_COPY_LOOP))
	{
		RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_COPY_LOOP))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_CROUCH_START, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_CROUCH_START))
	{
		RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_CROUCH_START))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_CROUCH_LOOP, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_CROUCH_LOOP))
	{
		RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_CROUCH_LOOP))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_RUSH_START, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_RUSH_START))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 30.f);
		No_RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_RUSH_START))
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 3.f);
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_RUSH_LOOP, 3.f, false, 0.2f, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_RUSH_LOOP))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 30.f);
		No_RootMotion(fTimeDelta);
		_vector myLook = m_pTransform->Get_State(TRANSFORM::LOOK);
		_vector Direction = m_pTargetTransform->Get_State(TRANSFORM::POSITION) - XMVectorSetY(m_pTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pTargetTransform->Get_State(TRANSFORM::POSITION)));

		_float Radian = (_float)acos(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
		_vector Cross = XMVector3Cross(myLook, Direction);

		if (XMVectorGetY(Cross) <= 0.f)
			Radian *= -1;

		_float fDegree = XMConvertToDegrees(Radian);
		if ((fDegree >= 90.f || fDegree <= -90.f) && Function::Distance(m_pTransform, m_pTargetTransform) >= 10.f)
		//if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_RUSH_LOOP))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_RUSH_END, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_RUSH_END))
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 1000.f, 0.f, 1.f));
		__super::Tick(fTimeDelta);
		/*
		PxTransform Transform = m_pAttackCollider->GetActor()->getGlobalPose();
		Transform.p = { 0.f,-1000.f,0.f };
		m_pAttackCollider->GetActor()->setGlobalPose(Transform);
		m_pController->setPosition(PxExtendedVec3(0.f, -1000.f, 0.f));*/
		RootMotion(fTimeDelta);
		//if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_RUSH_END))
		//	Release();
	}

#pragma region UI
	m_pUI_Hp->Tick(fTimeDelta);
#pragma endregion
}

void CMonster_Karen_Cloned::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

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

HRESULT CMonster_Karen_Cloned::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Render", "Failed to Bind_LightDirection");
	}


	//if (FAILED(__super::Render(16)))
	//{
	//	MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Render", "Failed to CGameObject::Render");
	//}


	if (FAILED(Bind_Transform(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to CGameObject::Bind_Transform");
	}

	//_float3 GrainColor = { 0.f, 0.f, 0.f };
	//if (FAILED(m_pShader->Bind_RawValue("g_vGrainColor", &GrainColor, sizeof(_float3))))
	//{
	//	MSG_RETURN(E_FAIL, "CMonster_Statue::Render", "Failed to Bind Grain Factor");
	//}
	for (_uint i = 0; i < m_vecMeshPassIndexPair.size(); ++i)
	{
		_uint iMeshIndex = m_vecMeshPassIndexPair[i].first;
		_uint iPassIndex = m_vecMeshPassIndexPair[i].second;
		if (FAILED(m_pModel->Bind_ShaderResourceViews(iMeshIndex, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_ShaderResourceViews");
		}
		if (FAILED(m_pModel->Bind_ShaderOptions(iMeshIndex, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_ShaderOptions");
		}
		if (FAILED(m_pModel->Bind_BoneMatrices(iMeshIndex, m_pShader, SHADER_BONE)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_BoneMatrices");
		}
	
		if (FAILED(m_pModel->Render(iMeshIndex, m_pShader, iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Render Model");
		}
	}

	return S_OK;
}

HRESULT CMonster_Karen_Cloned::Fetch(any Any)
{
	CLONEDESC CloneDesc = any_cast<CLONEDESC>(Any);
	_float4 InitPos = CloneDesc.InitPos;
	_uint Index = InitPos.w;
	
	InitPos.w = 0.f;
	__super::Fetch(InitPos);
	m_pTargetTransform = CloneDesc.TargetTransform;
	m_pTransform->LookAt(m_pTargetTransform);
	
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_COPY_START, 2.f, false, 0.2f, false);

	return S_OK;
}

HRESULT CMonster_Karen_Cloned::Render_OutLine()
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
		MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

HRESULT CMonster_Karen_Cloned::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Cloned::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

HRESULT CMonster_Karen_Cloned::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}
	//m_pTree = CKaren_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc,&//m_tMonsterDesc);
	//m_umapBehavior.emplace(BEHAVIOR::CUSTOM, m_pTree);

	return S_OK;
}

shared_ptr<CMonster_Karen_Cloned> CMonster_Karen_Cloned::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Karen_Cloned> pInstance = make_private_shared(CMonster_Karen_Cloned, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Karen_Cloned::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Karen_Cloned::Clone(any pArg)
{
	shared_ptr<CMonster_Karen_Cloned> pInstance = make_private_shared_copy(CMonster_Karen_Cloned, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Karen_Cloned::Clone", "Failed to Initialize");
	}

	return pInstance;
}
