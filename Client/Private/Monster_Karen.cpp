#include "ClientPCH.h"
#include "Monster_Karen.h"
#include "Monster_Karen_Cloned.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Karen_FSM.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "UI_Indicator_Boss_Hp.h"
#include "Camera_Player.h"
#pragma endregion

#include "Effect_Manager.h"
#include "Bone.h"
#include "Monster_Karen_Ice.h"
#include "PlayerTentacle.h"

#include "KarenControllerFilterCallBack.h"
#include "KarenCCTFilterCallBack.h"

CMonster_Karen::CMonster_Karen(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_Karen::CMonster_Karen(const CMonster_Karen& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_Karen::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN, g_aNull);

	return S_OK;
}

HRESULT CMonster_Karen::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen::Initialize", "Failed to CMonster::Initialize");
	}
	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen::Initialize", "Failed to CMonster::Ready_Behaviors");
	}
	m_eMonsterType = Karen;
	m_tEntityDesc.fHP =	m_tEntityDesc.fMaxHP = m_tEntityDesc.fKnuckBackGauge = 1500;
	
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,1000.f,0.f};
	cDesc.height = 2.f;
	cDesc.radius = 1.f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = NULL;

	if(pGameInstance->Current_Scene() == SCENE::REALTEST)
	{
		_float4 InitPos = any_cast<_float4>(Any);
		InitPos.y += 1.f;

		m_pTransform->Set_State(TRANSFORM::POSITION, InitPos);
		m_pTransform->Rotate(XMVectorSet(0.f,  1.f, 0.f, 0.f), XMConvertToRadians(InitPos.w));
		InitPos.w = 1.f;
		cDesc.position = { InitPos.x,InitPos.y,InitPos.z };
	}

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::MONSTER;
	m_pController->getActor()->userData = &m_ActorDesc;
	
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster::SetWeaponeActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CMonster_Karen::VFX_Kick1, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION4, std::bind(&CMonster_Karen::VFX_Kick2, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION5, std::bind(&CMonster_Karen::VFX_Kick3, this));
	//m_pModel->SetRootBoneAddIndex(m_pModel->Get_BoneIndex("Hips"));

	m_vControllerPivot = PxExtendedVec3(0.f, 0.f, 0.f);

	m_pFSM = CKaren_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 2.f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	m_pAttackCollider->Initialize(desc,actorDesc);
	
	m_pElectricCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 5.f;

	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	actorDesc.stAttackDesc.eHurtType = PLAYER_HURTTYPE::STRONG;
	m_pElectricCollider->Initialize(desc,actorDesc);

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
	m_pBodyCollider->Initialize(desc,bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;
	m_pBigAttackActorDesc = (ACTORDESC*)m_pBigAttackCollider->GetActor()->userData;
	m_pElectricActorDesc = (ACTORDESC*)m_pElectricCollider->GetActor()->userData;

	m_pAttackActorDesc->bActive = false;

	m_strAttackBoneName = "Reference";
	m_mAttackColliderPivot = XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 2.f, 3.f);

	Fetch(_float4(0.f, 0.5f, -30.f, 0.f));
#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
	m_pUI_BossHp = dynamic_pointer_cast<CUI_Indicator_Boss_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP));
	m_pUI_BossHp->Set_BossHp_Initialize(this->m_tEntityDesc.fHP, this->m_tEntityDesc.fMaxHP);
	m_pUI_BossHp->Render_Start();
	//m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion
	for (size_t i = 0; i < 2; i++)
	{
		m_pCloned[i] = CMonster_Karen_Cloned::Create(m_pDevice, m_pContext);
		m_pCloned[i]->Initialize();
	}

	m_vecMeshPassIndexPair.push_back({ 0, 17 }); // AMB
	//m_vecMeshPassIndexPair.push_back({ 2, 17 }); // AMB
	m_vecMeshPassIndexPair.push_back({ 3, 17 }); // AMB
	m_vecMeshPassIndexPair.push_back({ 4, 17 }); // AMB

	m_vecMeshPassIndexPair.push_back({ 1, 0 });
	//m_vecMeshPassIndexPair.push_back({ 5, 0 });

	m_pIceCorn = pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_KAREN_ICE);


	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_PLAYER);

	_float3 movePos[5] = {
		_float3(-0.2f,0.2f,0.f),
		_float3(0.2f,0.3f,0.f),
		_float3(0.f,0.f,0.f),
		_float3(-0.2f,-0.2f,0.f),
		_float3(0.2f,-0.3f,0.f)
	};

	string boneName[16] = { "LeftShoulder","RightShoulder","Spine1","LeftUpLeg","RightUpLeg",
							"Neck", "LeftWear1","LeftWear5", "RightWear1", "RightWear5",
							"LeftWear9","RightWear9","LeftWear2","RightWear2","LeftWear10","RightWear10"};
	for (int i = 0; i < 16; i++)
	{
		m_Tentacles.push_back(nullptr);

		TENTACLEDESC desc;
		desc.szBoneName = boneName[i];
		desc.index = (i)%5 + 1;

		m_Tentacles[i] = CGameInstance::Get_Instance()->Clone_GameObject<CPlayerTentacle>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE, desc);
		if (FAILED(pLayer->Add(m_Tentacles[i])))
		{
			MSG_RETURN(E_FAIL, "CScene_Mindroom::Ready_Karen_Tentacle", "Failed to CObjectLayer::Add");
		}
		m_Tentacles[i]->Set_Karen(true);
	}

	m_strWeakBoneName = "Spine";

	m_ControllerFilterCallBack = make_shared<CKarenControllerFilterCallBack>();
	m_ControllerFilters.mFilterCallback = m_ControllerFilterCallBack.get();
	m_CCTFilterCallBack = make_shared<CKarenCCTFilterCallback>();
	m_ControllerFilters.mCCTFilterCallback = m_CCTFilterCallBack.get();
	m_ControllerFilters.mFilterFlags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;
	m_ControllerFilters.mFilterData = NULL;
	
	
	PxSetGroup(*m_pController->getActor(), (_uint)ACTORTAG::MONSTER);
	PxSetGroupCollisionFlag((_uint)ACTORTAG::MONSTER, (_uint)ACTORTAG::FALLEN_OBJECT, false);
	return S_OK;
}

void CMonster_Karen::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_bRealCloned)
	{
		for (size_t i = 0; i < 2; i++)
		{
			m_pCloned[i]->Tick(fTimeDelta);
		}
	}

	
	m_pIceCorn->Tick(fTimeDelta);
#pragma region UI
	m_pUI_Hp->Tick(fTimeDelta);
	m_pUI_BossHp->Tick(fTimeDelta);

	m_pElectricActorDesc->mOwnerMat = m_pTransform->Get_Matrix();
#pragma endregion
}

void CMonster_Karen::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	if (m_bRealCloned)
	{
		for (size_t i = 0; i < 2; i++)
		{
			m_pCloned[i]->Late_Tick(fTimeDelta);
		}
	}
	m_pIceCorn->Late_Tick(fTimeDelta);

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
	m_pUI_BossHp->Set_BossHp(m_tEntityDesc.fHP, m_tEntityDesc.fMaxHP);
	m_pUI_BossHp->Set_BossName(Get_TypeName());
	m_pUI_BossHp->Late_Tick(fTimeDelta);
#pragma endregion
}

HRESULT CMonster_Karen::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen::Render", "Failed to Bind_LightDirection");
	}
	if (FAILED(Bind_Transform(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to CGameObject::Bind_Transform");
	}

	if(m_bCloned)
	{
		for (size_t i = 0; i < 3; i++)
		{
			_vector OriginPosition = m_pTransform->Get_State(TRANSFORM::POSITION);
			auto pBone = m_pModel->Get_Bone(("Eff0" + std::to_string(i + 1)).c_str());
			_matrix BoneMatrix = pBone->Get_CombinedTransformation();
			BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
			BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
			BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

			_matrix RealMetrix = XMMatrixScaling(100.f, 100.f, 100.f)
				* BoneMatrix
				* m_pModel->Get_Pivot()
				* m_pTransform->Get_Matrix();

			m_pTransform->Set_State(TRANSFORM::POSITION, RealMetrix.r[3]);
			_float3 GrainColor = { 0.f, 0.5f, 0.5f };
			if (FAILED(m_pShader->Bind_RawValue("g_vGrainColor", &GrainColor, sizeof(_float3))))
			{
				MSG_RETURN(E_FAIL, "CMonster_Statue::Render", "Failed to Bind Grain Factor");
			}
			if (FAILED(__super::Render(20)))
			{
				MSG_RETURN(E_FAIL, "CMonster_Karen::Render", "Failed to CGameObject::Render");
			}
			m_pTransform->Set_State(TRANSFORM::POSITION, OriginPosition);
		}

		if (FAILED(__super::Render(20)))
		{
			MSG_RETURN(E_FAIL, "CMonster_Karen::Render", "Failed to CGameObject::Render");
		}
	}
	else 
	{
		_float3 GrainColor = { 0.f, 0.f, 0.f };
		if (FAILED(m_pShader->Bind_RawValue("g_vGrainColor", &GrainColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CMonster_Statue::Render", "Failed to Bind Grain Factor");
		}
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

	}

	return S_OK;
}

HRESULT CMonster_Karen::Render_OutLine()
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
		MSG_RETURN(E_FAIL, "CMonster_Karen::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

void CMonster_Karen::VFX_Kick1()
{
	VFXMGR->Fetch(VFX::KAREN_KICK1, make_pair(shared_from_gameobject(), "LeftLeg"));
	CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 3.f);
}

void CMonster_Karen::VFX_Kick2()
{
	VFXMGR->Fetch(VFX::KAREN_KICK2, make_pair(shared_from_gameobject(), "LeftToeBase"));
	CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 3.f);
}

void CMonster_Karen::VFX_Kick3()
{
	_matrix mOriginal = m_pTransform->Get_Matrix();
	m_pTransform->Translate(- 1.5f * m_pTransform->Get_State(TRANSFORM::LOOK));
	VFXMGR->Fetch(VFX::KAREN_KICK3_0, m_pTransform->Get_Matrix());
	VFXMGR->Fetch(VFX::KAREN_KICK3_1, m_pTransform->Get_Matrix());
	VFXMGR->Fetch(VFX::KAREN_KICK3_2, m_pTransform->Get_Matrix());
	m_pTransform->Set_Matrix(mOriginal);
	CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 3.f);
}

void CMonster_Karen::SetElectricAttackTrue(_float3 Position)
{
	_matrix Transform = m_pTransform->Get_Matrix();
	Position.y = 0.f;
	PxTransform Pxtransform;
	_vector Quat = XMQuaternionRotationMatrix(Transform);
	Pxtransform.q = PxQuat(XMVectorGetX(Quat), XMVectorGetY(Quat), XMVectorGetZ(Quat), XMVectorGetW(Quat));
	Pxtransform.p = PxVec3(Position.x, Position.y, Position.z);

	m_pElectricCollider->GetActor()->setGlobalPose(Pxtransform);
}

void CMonster_Karen::SetElectricAttackFalse()
{
	PxTransform Pxtransform = m_pElectricCollider->GetActor()->getGlobalPose();
	Pxtransform.p.y = -1000.f;
	m_pElectricCollider->GetActor()->setGlobalPose(Pxtransform);
}

void CMonster_Karen::IceCorn(any Any)
{
	m_pIceCorn->Fetch(Any);
}

void CMonster_Karen::IceCornDecal(any Any)
{
	dynamic_pointer_cast<CMonster_Karen_Ice>(m_pIceCorn)->Fetch_Decal(Any);
}

HRESULT CMonster_Karen::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

HRESULT CMonster_Karen::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}
	//m_pTree = CKaren_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc,&//m_tMonsterDesc);
	//m_umapBehavior.emplace(BEHAVIOR::CUSTOM, m_pTree);

	return S_OK;
}

void CMonster_Karen::SetRealCloned(any Any)
{
	m_bRealCloned = !m_bRealCloned;
	
	if(m_bRealCloned)
	{
		for (size_t i = 0; i < 2; i++)
		{
			CMonster_Karen_Cloned::CLONEDESC CloneDesc;
			_float3 Transform = m_pTransform->Get_State(TRANSFORM::POSITION) + m_pTransform->Get_State(TRANSFORM::RIGHT) * 5.f * pow(-1, i);
			CloneDesc.InitPos = _float4(Transform);
			CloneDesc.InitPos.w = i;
			CloneDesc.TargetTransform = any_cast<shared_ptr<CTransform>>(Any);
			m_pCloned[i]->Fetch(CloneDesc);
		}
	}
	else
	{
		for (size_t i = 0; i < 2; i++)
			m_pCloned[i]->Return();
	}
}

shared_ptr<CMonster_Karen> CMonster_Karen::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Karen> pInstance = make_private_shared(CMonster_Karen, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Karen::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Karen::Clone(any pArg)
{
	shared_ptr<CMonster_Karen> pInstance = make_private_shared_copy(CMonster_Karen, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Karen::Clone", "Failed to Initialize");
	}

	return pInstance;
}

void CMonster_Karen::Tentacle_Fly()
{
	CGameInstance::Get_Instance()->PlaySoundW(TEXT("00.Karen_SE_BrainCrush.wav"), 3.f);

	for (shared_ptr<CPlayerTentacle> elem : m_Tentacles)
	{
		elem->Set_Fly(true);
		elem->Set_BrainFieldMode(true);
		elem->Set_Render(true);
		elem->Set_Karen(true);
	}
}
