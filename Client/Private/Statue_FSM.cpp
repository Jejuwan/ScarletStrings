#include "ClientPCH.h"
#include "Statue_FSM.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "Monster_FallenObject.h"
#include "Part.h"
#include "Bone.h"
#include "Teammate.h"
#include "PhysXEngine.h"
#include "Camera_Main.h"
#include "PhysXCollider.h"
#include "Scene.h"
#include "Scene_MindRoom.h"

HRESULT CStatue_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CStatue_FSM> pInstance = static_pointer_cast<CStatue_FSM>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_Idle->Set(pInstance, &CStatue_FSM::BeginState_Idle, &CStatue_FSM::ExecuteState_Idle, &CStatue_FSM::EndState_Idle);
	m_pState_Attack = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_Attack->Set(pInstance, &CStatue_FSM::BeginState_Attack, &CStatue_FSM::ExecuteState_Attack, &CStatue_FSM::EndState_Attack);
	m_pState_OnHit = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_OnHit->Set(pInstance, &CStatue_FSM::BeginState_OnHit_Front, &CStatue_FSM::ExecuteState_OnHit_Front, &CStatue_FSM::EndState_OnHit_Front);
	m_pState_Dead = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_Dead->Set(pInstance, &CStatue_FSM::BeginState_Dead, &CStatue_FSM::ExecuteState_Dead, &CStatue_FSM::EndState_Dead);
	m_pState_BCChance = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_BCChance->Set(pInstance, &CStatue_FSM::BeginState_BCChance, &CStatue_FSM::ExecuteState_BCChance, &CStatue_FSM::EndState_BCChance);
	m_pState_FinishMotion = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_FinishMotion->Set(pInstance, &CStatue_FSM::BeginState_FinishMotion, &CStatue_FSM::ExecuteState_FinishMotion, &CStatue_FSM::EndState_FinishMotion);

	m_pState_Cutscene_Init = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_Cutscene_Init->Set(pInstance, &CStatue_FSM::BeginState_Cutscene_Initialize, &CStatue_FSM::ExecuteState_Cutscene_Initialize, &CStatue_FSM::EndState_Cutscene_Initialize);
	m_pState_Cutscene_Positioning = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_Cutscene_Positioning->Set(pInstance, &CStatue_FSM::BeginState_Cutscene_Positioning, &CStatue_FSM::ExecuteState_Cutscene_Positioning, &CStatue_FSM::EndState_Cutscene_Positioning);
	m_pState_Cutscene_KarenTravers = make_shared<CStateTemplate<CStatue_FSM>>();
	m_pState_Cutscene_KarenTravers->Set(pInstance, &CStatue_FSM::BeginState_Cutscene_KarenTravers, &CStatue_FSM::ExecuteState_Cutscene_KarenTravers, &CStatue_FSM::EndState_Cutscene_KarenTravers);

	//auto pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_Statue>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CStatue_FSM::Initialize", "Failed to Get: Owner");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_pOwner.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CStatue_FSM::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pTargetTransform = dynamic_pointer_cast<CTransform>(Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CStatue_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pModel = dynamic_pointer_cast<CModel>(m_pOwner.lock()->Get_Component(COMPONENT::MODEL));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CStatue_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pAnimator = dynamic_pointer_cast<CAnimator>(m_pOwner.lock()->Get_Behavior(BEHAVIOR::ANIMATOR));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CStatue_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	if (nullptr == pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CStatue_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;

	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CStatue_FSM::Initialize", " pMonsterDesc is NULL");
	}
	m_pActorDesc = pActorDesc;

	m_MaskType = (_uint)m_pOwner.lock()->Get_MaskType();
	//cout << m_pMonsterDesc << endl;

	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, []() {
		CGameInstance::Get_Instance()->PlaySoundW(L"00.Karen_SE_Statue_Arm.wav", 5.f);
		});


#pragma region UI
	m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	m_pUI_NoticeMark->Initialize();
#pragma endregion

	// Cutscene Camera
	auto pGameInstance = CGameInstance::Get_Instance();
	//auto pPlayerCamera = static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_Camera();
	//pPlayerCamera->Append_Cinematic(TEXT("Bin/Resources/Animation/CameraKeyframe/Cutscene_MindRoom_Init.cin"), TEXT("Cutscene_MindRoom_Init"));
	auto pCameraLayer = pGameInstance->Find_Layer(SCENE::MINDROOM, LAYER_CAMERA_CUTSCENE);
	auto pCutsceneCamera = dynamic_pointer_cast<CCamera_Main>(pCameraLayer->Get_GameObject(0));
	pCutsceneCamera->Append_Cinematic(TEXT("Bin/Resources/Animation/CameraKeyframe/Cutscene_MindRoom_Init.cin"), TEXT("Cutscene_MindRoom_Init"));
	pCutsceneCamera->Append_Cinematic(TEXT("Bin/Resources/Animation/CameraKeyframe/Cutscene_MindRoom_Positioning.cin"), TEXT("Cutscene_MindRoom_Positioning"));
	pCutsceneCamera->Append_Cinematic(TEXT("Bin/Resources/Animation/CameraKeyframe/Cutscene_MindRoom_KarenTravers.cin"), TEXT("Cutscene_MindRoom_KarenTravers"));
	CPipeLine::Get_Instance()->Set_Camera(pCutsceneCamera);
   
	// Create Fallen Object
	
		m_FallenPool[0] = pGameInstance->Add_Pool(pGameInstance->Current_Scene(), L"FallingObject_Pool1" + std::to_wstring(m_MaskType), CMonster_FallenObject::Create(m_pDevice, m_pContext, 3), 1);
		m_FallenPool[1] = pGameInstance->Add_Pool(pGameInstance->Current_Scene(), L"FallingObject_Pool2" + std::to_wstring(m_MaskType), CMonster_FallenObject::Create(m_pDevice, m_pContext, 5), 1);
		m_FallenPool[2] = pGameInstance->Add_Pool(pGameInstance->Current_Scene(), L"FallingObject_Pool3" + std::to_wstring(m_MaskType), CMonster_FallenObject::Create(m_pDevice, m_pContext, 7), 1);

		/*m_BFObjectPool[0] = pGameInstance->Add_Pool(pGameInstance->Current_Scene(), L"BFObject_Pool1" + std::to_wstring(m_MaskType), CMonster_FallenObject::Create(m_pDevice, m_pContext, 0),6);
		m_BFObjectPool[1] = pGameInstance->Add_Pool(pGameInstance->Current_Scene(), L"BFObject_Pool2" + std::to_wstring(m_MaskType), CMonster_FallenObject::Create(m_pDevice, m_pContext, 1),6);
		m_BFObjectPool[2] = pGameInstance->Add_Pool(pGameInstance->Current_Scene(), L"BFObject_Pool3" + std::to_wstring(m_MaskType), CMonster_FallenObject::Create(m_pDevice, m_pContext, 2),6);*/

		//CMonster_FallenObject::FALLENDESC FallenDesc;

		//for (int i = 0; i < 3; i++)
		//{
		//	for (int j = 0; j < 1; j++)
		//	{
		//		FallenDesc.InitPos = { /*cosf(XMConvertToRadians(60.f * j))*/i * 10.f,10.f,/*sinf(XMConvertToRadians(60.f * j))*/0, 1.f };
		//		FallenDesc.Randomize = true;
		//		m_BFObjectPool[i].lock()->Pop(FallenDesc);
		//	}
		//}

		pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 2.5f);

	return S_OK;
}

void CStatue_FSM::StateInitial(_float fTimeDelta)
{
	GotoState(m_pState_Cutscene_Init);
	m_bCutscenePlaying = true;
}

void CStatue_FSM::ExecutePersistent(_float fTimeDelta)
{
	if (m_pCurrentState == m_pState_Dead)
		return;

	if (m_bCutscenePlaying)
		return;


	m_pUI_NoticeMark->Tick(fTimeDelta);
	m_pUI_NoticeMark->Set_IndicatorPosition(m_pTransform->Get_State(TRANSFORM::POSITION), m_pTransform->Get_WorldMatrix());
	m_pUI_NoticeMark->Late_Tick(fTimeDelta);

	m_fDistance = Function::Distance(m_pTransform, m_pTargetTransform);

	if (m_fCoolTime >= 0)
		m_fCoolTime -= fTimeDelta;

	_vector myLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector Direction = m_pTargetTransform->Get_State(TRANSFORM::POSITION) - XMVectorSetY(m_pTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pTargetTransform->Get_State(TRANSFORM::POSITION)));

	_float Radian = (_float)acos(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
	_vector Cross = XMVector3Cross(myLook, Direction);	

	if (XMVectorGetY(Cross) <= 0.f)
		Radian *= -1;

	m_fDegree = XMConvertToDegrees(Radian);

	if (m_pOwner.lock()->Get_MyTurn() && m_pCurrentState != m_pState_Attack && m_pCurrentState != m_pState_OnHit)
		GotoState(m_pState_Attack);

	if (m_pActorDesc->bThrowHit&& *m_pActorDesc->stHitDesc.AttackColliderActive)
	{
		m_pActorDesc->bThrowHit = false;
		m_pOwner.lock()->GetBodyCollider()->GetActorDesc().bThrowHit = false;
		if (!m_pOwner.lock()->Get_MyTurn())
		{
			return;	
		}
		//if (m_pEntityDesc->fHP <= 0.f)
		//{
		//	GotoState(m_pState_Dead);
		//	return;
		//}
		auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
		auto Interactive = Kynesis->Get_RecentThrowdObject();
		if (Interactive == nullptr)
		{ 
			return;
		}

		m_pEntityDesc->fHP -= m_pActorDesc->stHitDesc.iDamage;
		//cout << m_pActorDesc->stHitDesc.iDamage << '\t' << m_pEntityDesc->fHP << endl;
		if (m_pEntityDesc->fHP <= 0.f)
		{
			//	StatueFalling();
			GotoState(m_pState_Dead);
			return;
		}
		GotoState(m_pState_OnHit);

		return;
	}
}

#pragma region Cutscene
void CStatue_FSM::BeginState_Cutscene_Initialize()
{
	CUI_Manager::Get_Instance()->Set_MindRoomCutScene(true);

	auto pOwner = m_pOwner.lock();
	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	switch ((CMonster_Statue::TYPE)m_MaskType)
	{
	case CMonster_Statue::MASK_WHITE:
	{
		auto pMask = pOwner->Get_Part(1); // Mask
		auto pFan = pOwner->Get_Part(2); // Fan

		_float4x4 matAdditivePivot = XMMatrixRotationY(XMConvertToRadians(-90.f)) * XMMatrixTranslation(-100.f, -100.f, 0.f);
		pMask->Set_WeakBoneMatrix(m_pModel->Get_Bone("Hood")->Get_CombinedTransformationPointer());
		pMask->Set_PlusMatrix(matAdditivePivot);

		matAdditivePivot = XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(180.f));
		pFan->Set_WeakBoneMatrix(m_pModel->Get_Bone("LeftWeapon")->Get_CombinedTransformationPointer());
		pFan->Set_PlusMatrix(matAdditivePivot);

		m_iCutsceneIndex = (_uint)(ANIMATION::STATUE::CUTSCENE_INITIAL_0);
	}
	break;
	case CMonster_Statue::MASK_BLACK:
	{
		auto pFlower = pOwner->Get_Part(0); // Flower
		auto pMask = pOwner->Get_Part(1); // Mask

		_float4x4 matAdditivePivot = XMMatrixRotationX(XMConvertToRadians(115.f)) * XMMatrixTranslation(-50.f, -112.f, 350.f);
		pFlower->Set_WeakBoneMatrix(m_pModel->Get_Bone("RightWeapon")->Get_CombinedTransformationPointer());
		pFlower->Set_PlusMatrix(matAdditivePivot);

		matAdditivePivot = XMMatrixRotationX(XMConvertToRadians(-150.f)) * XMMatrixRotationY(XMConvertToRadians(-160.f)) * XMMatrixRotationZ(XMConvertToRadians(-35.f)) *
			XMMatrixTranslation(-162.f, -277.f, 60.f);
		pMask->Set_WeakBoneMatrix(m_pModel->Get_Bone("LeftWeapon")->Get_CombinedTransformationPointer());
		pMask->Set_PlusMatrix(matAdditivePivot);

		m_iCutsceneIndex = (_uint)(ANIMATION::STATUE::CUTSCENE_INITIAL_1);
	}
	break;
	case CMonster_Statue::MASK_RED:
	{
		auto pFlower = pOwner->Get_Part(0); // Flower
		auto pMask = pOwner->Get_Part(1); // Mask

		_float4x4 matAdditivePivot = XMMatrixRotationX(XMConvertToRadians(115.f)) * XMMatrixTranslation(-50.f, -112.f, 350.f);
		pFlower->Set_WeakBoneMatrix(m_pModel->Get_Bone("RightWeapon")->Get_CombinedTransformationPointer());
		pFlower->Set_PlusMatrix(matAdditivePivot);

		matAdditivePivot = XMMatrixRotationX(XMConvertToRadians(30.f)) * XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixRotationZ(XMConvertToRadians(130.f)) *
			XMMatrixTranslation(-300.f, -230.f, 2.f);
		pMask->Set_WeakBoneMatrix(m_pModel->Get_Bone("LeftWeapon")->Get_CombinedTransformationPointer());
		pMask->Set_PlusMatrix(matAdditivePivot);

		m_iCutsceneIndex = (_uint)(ANIMATION::STATUE::CUTSCENE_INITIAL_2);
	}
	break;
	}

	m_pAnimator->Play_Animation(m_iCutsceneIndex, 2.f, false, 0.2f, false);

	auto pGameInstance = CGameInstance::Get_Instance();

	_float3 vPivotPosition = { 0.f, 0.5f, -8.f };

	auto pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Player());
	pPlayer->Get_Controller()->setFootPosition(PxExtendedVec3(vPivotPosition.x, vPivotPosition.y, vPivotPosition.z));
	pPlayer->Get_Camera()->Lock(true);

	auto pCameraLayer = pGameInstance->Find_Layer(SCENE::MINDROOM, LAYER_CAMERA_CUTSCENE);
	auto pCutsceneCamera = dynamic_pointer_cast<CCamera_Main>(pCameraLayer->Get_GameObject(0));
	pCutsceneCamera->Play_CinematicCamera(TEXT("Cutscene_MindRoom_Init"));

	auto pPlayerTransform = pPlayer->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	pPlayerTransform->Set_State(TRANSFORM::POSITION, vPivotPosition);
	pPlayerTransform->LookTo(XMVectorSet(0.f, 0.f, -1.f, 0.f));
	
	vPivotPosition = { -1.5f, 0.5f, -7.f };
	auto pTeammateLayer = pGameInstance->Find_Layer(SCENE::MINDROOM, LAYER_TEAMMATE);
	if (nullptr == pTeammateLayer)
		return;

	pTeammateLayer->Set_TimeScale(0.f);
	pTeammateLayer->Iterate_Objects([&](shared_ptr<CGameObject> pObject)
		{
			auto pTeammate = dynamic_pointer_cast<CTeammate>(pObject);
			pTeammate->Enable_Weapon(true);
			pTeammate->Enable_FSM(false);
			pTeammate->Set_Animation(0, 2.f);

			pTeammate->Get_Controller()->setFootPosition(PxExtendedVec3(vPivotPosition.x, vPivotPosition.y, vPivotPosition.z));

			auto pTeammateTransform = pTeammate->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			pTeammateTransform->Set_State(TRANSFORM::POSITION, vPivotPosition);
			pTeammateTransform->LookTo(XMVectorSet(0.f, 0.f, -1.f, 0.f));

			vPivotPosition.x += 3.f;
			
			return true;
		});

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("00.Karen_SE_Statue_Appear2.wav"), 1.f);
}

void CStatue_FSM::ExecuteState_Cutscene_Initialize(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
	{
		Stop_Cutscene();
		GotoState(m_pState_Idle);
	}

	if (m_pAnimator->Is_Finished(m_iCutsceneIndex))
	{
		GotoState(m_pState_Cutscene_Positioning);
	}
}

void CStatue_FSM::EndState_Cutscene_Initialize()
{
}

void CStatue_FSM::BeginState_Cutscene_Positioning()
{
	m_pAnimator->Play_Animation((_uint)(ANIMATION::STATUE::CUTSCENE_POSITIONING_0)+m_MaskType, 2.f, false, 0.f, false);
	
	auto pGameInstance = CGameInstance::Get_Instance();
	auto pCameraLayer = pGameInstance->Find_Layer(SCENE::MINDROOM, LAYER_CAMERA_CUTSCENE);
	auto pCutsceneCamera = dynamic_pointer_cast<CCamera_Main>(pCameraLayer->Get_GameObject(0));
	pCutsceneCamera->Play_CinematicCamera(TEXT("Cutscene_MindRoom_Positioning"));

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("00.Karen_SE_Statue_Appear0.wav"), 1.f);
}

void CStatue_FSM::ExecuteState_Cutscene_Positioning(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
	{
		Stop_Cutscene();
		GotoState(m_pState_Idle);
	}

	if (m_pAnimator->Is_Finished((_uint)(ANIMATION::STATUE::CUTSCENE_POSITIONING_0)+m_MaskType))
	{
		GotoState(m_pState_Cutscene_KarenTravers);
	}
}

void CStatue_FSM::EndState_Cutscene_Positioning()
{
}

void CStatue_FSM::BeginState_Cutscene_KarenTravers()
{
	if (m_MaskType == CMonster_Statue::MASK_WHITE)
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 30.f, 1.f));
	}
	else
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 100.f, 1.f));
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pCameraLayer = pGameInstance->Find_Layer(SCENE::MINDROOM, LAYER_CAMERA_CUTSCENE);
	auto pCutsceneCamera = dynamic_pointer_cast<CCamera_Main>(pCameraLayer->Get_GameObject(0));
	pCutsceneCamera->Play_CinematicCamera(TEXT("Cutscene_MindRoom_KarenTravers"));
	
	m_pAnimator->Play_Animation((_uint)(ANIMATION::STATUE::CUTSCENE_IDLE), 1.f, false, 0.f, false);

	if (m_pOwner.lock()->Get_MyTurn())
	{
		shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
		shared_ptr<CObjectLayer> pMonsterLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
		shared_ptr<CGameObject> pKaren = pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_KAREN);
		pKaren->Fetch(_float4(0.f, 0.5f, -30.f, 0.f));
		pMonsterLayer->Add(pKaren);
	}

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("00.Karen_SE_Statue_Appear2.wav"), 1.f);
}

void CStatue_FSM::ExecuteState_Cutscene_KarenTravers(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
		GotoState(m_pState_Idle);

	if (m_pAnimator->Is_Finished((_uint)(ANIMATION::STATUE::CUTSCENE_IDLE)))
	{
		GotoState(m_pState_Idle);
	}
}

void CStatue_FSM::EndState_Cutscene_KarenTravers()
{
	Stop_Cutscene();
}
#pragma endregion

void CStatue_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation((_uint)(ANIMATION::STATUE::IDLE_FLOWER0)+m_MaskType, 1.f);
	m_pOwner.lock()->InitRootMotion();
}

void CStatue_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->No_RootMotion(fTimeDelta);
}

void CStatue_FSM::EndState_Idle()
{
}

void CStatue_FSM::BeginState_Attack()
{
	m_pAnimator->Play_Animation((_uint)(ANIMATION::STATUE::ATTACK0)+m_MaskType, 1.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	if (m_MaskType == 1)
		m_pOwner.lock()->BlackMaskPivotChance();
}

void CStatue_FSM::ExecuteState_Attack(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->No_RootMotion(fTimeDelta);

	if (m_pAnimator->Is_Playing((_uint)(ANIMATION::STATUE::ATTACK0)+m_MaskType))
	{
		if (m_pAnimator->Is_Finished((_uint)(ANIMATION::STATUE::ATTACK0)+m_MaskType))
		{
			m_pAnimator->Play_Animation((_uint)(ANIMATION::STATUE::IDLE0)+m_MaskType, 1.f, false, 0.2f, true);

			if (m_MaskType == 1)
				m_pOwner.lock()->BlackMaskPivotChance();
		}
	}
}

void CStatue_FSM::EndState_Attack()
{
}

void CStatue_FSM::BeginState_OnHit_Front()
{
	m_pAnimator->Play_Animation((_uint)(ANIMATION::STATUE::DOWN_START0)+m_MaskType, 1.f, false, 0.2f, false);
	CGameInstance::Get_Instance()->PlaySoundW(L"00.Karen_SE_Statue_Damaged3.wav", 5.f);
}

void CStatue_FSM::ExecuteState_OnHit_Front(_float fTimeDelta)
{
	if (m_pAnimator->Is_Playing((_uint)(ANIMATION::STATUE::DOWN_START0)+m_MaskType))
	{
		if (m_pAnimator->Is_Finished((_uint)(ANIMATION::STATUE::DOWN_START0)+m_MaskType))
		{
			m_pAnimator->Play_Animation((_uint)(ANIMATION::STATUE::IDLE0)+m_MaskType, 1.f, false, 0.2f, true);
			GotoState(m_pState_Idle);
		}
	}
}

void CStatue_FSM::EndState_OnHit_Front()
{
	//m_pOwner.lock()->ChangeNextState();
}

void CStatue_FSM::BeginState_Dead()
{
	m_pAnimator->Play_Animation((_uint)(ANIMATION::STATUE::DOWN_START0)+m_MaskType, 1.f, false, 0.2f, false);
	CGameInstance::Get_Instance()->PlaySoundW(L"00.Karen_SE_Statue_Damaged3.wav", 5.f);
	m_pOwner.lock()->ChangeNextState();
}

void CStatue_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	if (m_pAnimator->Is_Playing((_uint)(ANIMATION::STATUE::DOWN0)+m_MaskType))
	{
		if (m_pAnimator->Is_Finished((_uint)(ANIMATION::STATUE::DOWN0)+m_MaskType))
		{
			if(m_pOwner.lock()->Get_MyTurn())
			{
				m_pOwner.lock()->ChangeNextState();
				m_pOwner.lock()->SetDead();
			}
		}
	}
}

void CStatue_FSM::EndState_Dead()
{
}

void CStatue_FSM::BeginState_BCChance()
{
}

void CStatue_FSM::ExecuteState_BCChance(_float fTimeDelta)
{
}

void CStatue_FSM::EndState_BCChance()
{
}

void CStatue_FSM::BeginState_FinishMotion()
{
}

void CStatue_FSM::ExecuteState_FinishMotion(_float fTimeDelta)
{
}

void CStatue_FSM::EndState_FinishMotion()
{
}

void CStatue_FSM::StatueFalling()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	CMonster_FallenObject::FALLENDESC FallenDesc;
	FallenDesc.InitPos = { 0.f,5.f,0.f, 1.f };
	_uint idx = static_pointer_cast<CScene_MindRoom>(pGameInstance->Get_Current_Scene_Ptr())->Get_PoolIdx();
	if(idx <=2)
	{
		m_FallenPool[idx].lock()->Pop(FallenDesc);
		static_pointer_cast<CScene_MindRoom>(pGameInstance->Get_Current_Scene_Ptr())->Set_PoolIdx(idx + 1);
	}
}

void CStatue_FSM::Stop_Cutscene()
{
	CUI_Manager::Get_Instance()->Set_MindRoomCutScene(false);
	// Positioning
	_float4 vInitPosition = _float4(0.f, -0.5f, -50.f, 0.f);
	switch ((CMonster_Statue::TYPE)m_MaskType)
	{
	case CMonster_Statue::MASK_WHITE:
		break;
	case CMonster_Statue::MASK_BLACK:
		vInitPosition = XMVector3TransformNormal(XMLoadFloat4(&vInitPosition), XMMatrixRotationY(XMConvertToRadians(120.f)));
		vInitPosition.w = 120.f;
		break;
	case CMonster_Statue::MASK_RED:
		vInitPosition = XMVector3TransformNormal(XMLoadFloat4(&vInitPosition), XMMatrixRotationY(XMConvertToRadians(240.f)));
		vInitPosition.w = 120.f * 2;
		break;
	}
	m_pOwner.lock()->Fetch(vInitPosition);

	m_bCutscenePlaying = false;

	// Part Settings
	auto pOwner = m_pOwner.lock();
	auto pFlower = pOwner->Get_Part(0); // Flower
	pFlower->Set_WeakBoneMatrix(m_pModel->Get_Bone("RightWeapon")->Get_CombinedTransformationPointer());

	_float4x4 matAdditivePivot = XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(180.f));
	pFlower->Set_PlusMatrix(matAdditivePivot);

	auto pMask = pOwner->Get_Part(1);
	pMask->Set_WeakBoneMatrix(m_pModel->Get_Bone("RightWeapon")->Get_CombinedTransformationPointer());
	pMask->Set_PlusMatrix(matAdditivePivot);

	if (m_MaskType == CMonster_Statue::MASK_WHITE)
	{
		auto pFan = pOwner->Get_Part(2);
		pFan->Set_WeakBoneMatrix(m_pModel->Get_Bone("LeftWeapon")->Get_CombinedTransformationPointer());
		pFan->Set_PlusMatrix(matAdditivePivot);
	}

	// Camera Settings
	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Player());
	pPlayer->Get_Camera()->Lock(false);

	CPipeLine::Get_Instance()->Set_Camera(pPlayer->Get_Camera());

	auto pCameraLayer = pGameInstance->Find_Layer(SCENE::MINDROOM, LAYER_CAMERA_CUTSCENE);
	auto pCutsceneCamera = dynamic_pointer_cast<CCamera_Main>(pCameraLayer->Get_GameObject(0));
	pCutsceneCamera->Set_Enable(false);

	// Teammate
	auto pTeammateLayer = pGameInstance->Find_Layer(SCENE::MINDROOM, LAYER_TEAMMATE);
	if (nullptr == pTeammateLayer)
		return;
	pTeammateLayer->Set_TimeScale(1.f);
	pTeammateLayer->Iterate_Objects([&](shared_ptr<CGameObject> pObject)
		{
			auto pTeammate = dynamic_pointer_cast<CTeammate>(pObject);
			pTeammate->Enable_FSM(true);

			return true;
		});


	// Set_MindRoom_StartBGM
	CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_The_Decisive_Battle_of_the_Ring_of_Time.mp3"), 0.3f);
	//CGameInstance::Get_Instance()->StopAll();
}

shared_ptr<CStatue_FSM> CStatue_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	shared_ptr<CStatue_FSM> pInstance = make_private_shared(CStatue_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CStatue_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
