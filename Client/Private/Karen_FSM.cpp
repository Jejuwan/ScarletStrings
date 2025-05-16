#include "ClientPCH.h"
#include "Karen_FSM.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "Effect_Manager.h"
#include "Bone.h"
#include "PlayerFinalState.h"
#include "PlayerTentacle.h"
#include "Teammate.h"

HRESULT CKaren_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner,  ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CKaren_FSM> pInstance = static_pointer_cast<CKaren_FSM>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Idle->Set(pInstance, &CKaren_FSM::BeginState_Idle, &CKaren_FSM::ExecuteState_Idle, &CKaren_FSM::EndState_Idle);
	m_pState_Walk = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Walk->Set(pInstance, &CKaren_FSM::BeginState_Walk, &CKaren_FSM::ExecuteState_Walk, &CKaren_FSM::EndState_Walk);
	m_pState_CutScene = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_CutScene->Set(pInstance, &CKaren_FSM::BeginState_CutScene, &CKaren_FSM::ExecuteState_CutScene, &CKaren_FSM::EndState_CutScene);
	m_pState_Run = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Run->Set(pInstance, &CKaren_FSM::BeginState_Run, &CKaren_FSM::ExecuteState_Run, &CKaren_FSM::EndState_Run);
	m_pState_KickCombo = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_KickCombo->Set(pInstance, &CKaren_FSM::BeginState_KickCombo, &CKaren_FSM::ExecuteState_KickCombo, &CKaren_FSM::EndState_KickCombo);
	m_pState_Kick1 = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Kick1->Set(pInstance, &CKaren_FSM::BeginState_Kick1, &CKaren_FSM::ExecuteState_Kick1, &CKaren_FSM::EndState_Kick1);
	m_pState_Kick2 = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Kick2->Set(pInstance, &CKaren_FSM::BeginState_Kick2, &CKaren_FSM::ExecuteState_Kick2, &CKaren_FSM::EndState_Kick2);
	m_pState_Kick3 = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Kick3->Set(pInstance, &CKaren_FSM::BeginState_Kick3, &CKaren_FSM::ExecuteState_Kick3, &CKaren_FSM::EndState_Kick3);
	m_pState_Kick4 = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Kick4->Set(pInstance, &CKaren_FSM::BeginState_Kick4, &CKaren_FSM::ExecuteState_Kick4, &CKaren_FSM::EndState_Kick4);
	m_pState_Electric = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Electric->Set(pInstance, &CKaren_FSM::BeginState_Electric, &CKaren_FSM::ExecuteState_Electric, &CKaren_FSM::EndState_Electric);
	m_pState_Teleport = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Teleport->Set(pInstance, &CKaren_FSM::BeginState_Teleport, &CKaren_FSM::ExecuteState_Teleport, &CKaren_FSM::EndState_Teleport);
	m_pState_Teleport_Dodge = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Teleport_Dodge->Set(pInstance, &CKaren_FSM::BeginState_Teleport_Dodge, &CKaren_FSM::ExecuteState_Teleport_Dodge, &CKaren_FSM::EndState_Teleport_Dodge);
	m_pState_IceCorn = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_IceCorn->Set(pInstance, &CKaren_FSM::BeginState_IceCorn, &CKaren_FSM::ExecuteState_IceCorn, &CKaren_FSM::EndState_IceCorn);
	m_pState_CloneDash = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_CloneDash->Set(pInstance, &CKaren_FSM::BeginState_CloneDash, &CKaren_FSM::ExecuteState_CloneDash, &CKaren_FSM::EndState_CloneDash);
	m_pState_DodgeCloned = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_DodgeCloned->Set(pInstance, &CKaren_FSM::BeginState_DodgeCloned, &CKaren_FSM::ExecuteState_DodgeCloned, &CKaren_FSM::EndState_DodgeCloned);
	m_pState_Dead = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_Dead->Set(pInstance, &CKaren_FSM::BeginState_Dead, &CKaren_FSM::ExecuteState_Dead, &CKaren_FSM::EndState_Dead);
	m_pState_FinishMotion = make_shared<CStateTemplate<CKaren_FSM>>();
	m_pState_FinishMotion->Set(pInstance, &CKaren_FSM::BeginState_FinishMotion, &CKaren_FSM::ExecuteState_FinishMotion, &CKaren_FSM::EndState_FinishMotion);

	//auto pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_Karen>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CKaren_FSM::Initialize", "Failed to Get: Owner");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_pOwner.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CKaren_FSM::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pPlayerTransform = dynamic_pointer_cast<CTransform>(Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pPlayerTransform)
	{
		MSG_RETURN(E_FAIL, "CTwince_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pModel = dynamic_pointer_cast<CModel>(m_pOwner.lock()->Get_Component(COMPONENT::MODEL));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CKaren_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pAnimator = dynamic_pointer_cast<CAnimator>(m_pOwner.lock()->Get_Behavior(BEHAVIOR::ANIMATOR));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CKaren_FSM::Initialize", "Failed to Get: Player:Transform");
	}
	
	if (nullptr == pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CKaren_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;
	
	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CKaren_FSM::Initialize", " pMonsterDesc is NULL");
	}
	m_pActorDesc = pActorDesc;

	SCENE Scene = CGameInstance::Get_Instance()->Current_Scene();
	if (Scene != SCENE::TEST)
	{
		shared_ptr<CObjectLayer> Layer = CGameInstance::Get_Instance()->Find_Layer(Scene, LAYER_TEAMMATE);
		if (nullptr != Layer)
		{
			shared_ptr<CGameObject> MaybeArashi = Layer->Get_GameObject(0);
			shared_ptr<CGameObject> MaybeShiDeng = Layer->Get_GameObject(1);
			m_pArashTransform = dynamic_pointer_cast<CTransform>(MaybeArashi->Get_Component(COMPONENT::TRANSFORM));
			m_pShidengTransform = dynamic_pointer_cast<CTransform>(MaybeShiDeng->Get_Component(COMPONENT::TRANSFORM));
		}
	}
	m_pTargetTransform = m_pPlayerTransform;
	//cout << m_pMonsterDesc << endl;

#pragma region UI
	m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	m_pUI_NoticeMark->Initialize();
#pragma endregion

	return S_OK;
}

void CKaren_FSM::StateInitial(_float fTimeDelta)
{
	m_pOwner.lock()->SetElectricAttackFalse();
	GotoState(m_pState_CutScene);
}

void CKaren_FSM::ExecutePersistent(_float fTimeDelta)
{
	if (m_pCurrentState == m_pState_Dead)
		return;

	m_pUI_NoticeMark->Tick(fTimeDelta);
	m_pUI_NoticeMark->Set_IndicatorPosition(this->m_pTransform->Get_State(TRANSFORM::POSITION), this->m_pTransform->Get_WorldMatrix());
	m_pUI_NoticeMark->Late_Tick(fTimeDelta);

	if (dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH && m_pCurrentState != m_pState_Idle)
	{
		GotoState(m_pState_Idle);
		return;
	}

	m_fDistance = Function::Distance(m_pTransform, m_pTargetTransform);
	//cout << m_fDistance << endl;
	if (m_fCoolTime >= 0)
		m_fCoolTime -= fTimeDelta;

	_vector myLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector Direction = m_pTargetTransform->Get_State(TRANSFORM::POSITION) - XMVectorSetY(m_pTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pTargetTransform->Get_State(TRANSFORM::POSITION)));

	_float Radian = (_float)acos(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
	_vector Cross = XMVector3Cross(myLook, Direction);

	if (XMVectorGetY(Cross) <= 0.f)
		Radian *= -1;

	m_fDegree = XMConvertToDegrees(Radian);

	if (m_pActorDesc->bHit)
	{
		if (m_pEntityDesc->fHP <= 0)
		{
			if (m_pCurrentState != m_pState_FinishMotion || m_pCurrentState != m_pState_FinishMotion)
				GotoState(m_pState_FinishMotion);
			return;
		}	

		auto pGameInstance = CGameInstance::Get_Instance();
		CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
		if(nullptr==pPlayer->Get_Camera()->Get_LockOnTransform() &&
			m_pActorDesc->stHitDesc.strAttackTag != ACTORTAG::TEAMMATE_ATTACK)
		{
			pPlayer->Get_Camera()->Set_LockOn(true);
			pPlayer->Get_Camera()->Set_LockedMonster(m_pOwner.lock());
			pPlayer->Get_Camera()->Set_LockOnTransform(m_pTransform);
			pPlayer->Set_FinishMonster(m_pOwner.lock());
		}

		if (m_pCurrentState != m_pState_DodgeCloned && m_pCurrentState != m_pState_CloneDash && m_fCoolTime <= 0.f)
		{
			GotoState(m_pState_DodgeCloned);
			return;
		}
		if (m_pCurrentState != m_pState_DodgeCloned)
		{
			m_pEntityDesc->fHP -= m_pActorDesc->stHitDesc.iDamage;
			m_pEntityDesc->fKnuckBackGauge -= m_pActorDesc->stHitDesc.iDamage * ((rand() % 30) / 10.f);
		}

		return;
	} 
	if (m_pActorDesc->bThrowHit)
	{
		_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
		Pos.y += 1.5f;
		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_ROCK0,
			initializer_list<any>{Pos, Pos, Pos, Pos});
		if (m_pEntityDesc->fHP <= 0)
		{
			if (m_pCurrentState != m_pState_FinishMotion || m_pCurrentState != m_pState_FinishMotion)
				GotoState(m_pState_FinishMotion);
			return;
		}

		auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
		//auto Interactive = Kynesis->Get_RecentThrowdObject();
		//if (Interactive == nullptr)
		//	return;

		auto pGameInstance = CGameInstance::Get_Instance();
		CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
		if (nullptr == pPlayer->Get_Camera()->Get_LockOnTransform())
		{
			pPlayer->Get_Camera()->Set_LockOn(true);
			pPlayer->Get_Camera()->Set_LockedMonster(m_pOwner.lock());
			pPlayer->Get_Camera()->Set_LockOnTransform(m_pTransform);
			pPlayer->Set_FinishMonster(m_pOwner.lock());
		}

		if (m_pCurrentState != m_pState_DodgeCloned && m_pCurrentState != m_pState_CloneDash && m_fCoolTime <= 0.f)
		{
			GotoState(m_pState_DodgeCloned);
			return;
		}
		if (m_pCurrentState != m_pState_DodgeCloned)
		{
			m_pEntityDesc->fHP -= m_pActorDesc->stHitDesc.iDamage;
			m_pEntityDesc->fKnuckBackGauge -= m_pActorDesc->stHitDesc.iDamage * ((rand() % 20) / 10.f + 1.f);
		}
		//shared_ptr<CTransform> InterActiveTransform = dynamic_pointer_cast<CTransform>(Interactive->Get_Component(COMPONENT::TRANSFORM));

		//_vector Direction = InterActiveTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);

		//_float Radian = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
		//_vector Cross = XMVector3Cross(myLook, Direction);

		//if (XMVectorGetY(Cross) <= 0.f)
		//	Radian *= -1;
		//_float InteractiveDegree = XMConvertToDegrees(Radian);

		Kynesis->Set_RecentThrowObject(nullptr);
		//if (m_pCurrentState != m_pState_DodgeCloned)
		//	GotoState(m_pState_DodgeCloned);
		return;
	}
}

void CKaren_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::IDLE1, 2.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Karen_status_Idle" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	GotoState(m_pState_Walk);
}

void CKaren_FSM::EndState_Idle()
{
	m_pUI_NoticeMark->NoticePlayer();
	m_pOwner.lock()->MotionReset();
}

void CKaren_FSM::BeginState_CutScene()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::WALK, 2.f, false, 0.2f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Karen_status_Idle" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_CutScene(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if(m_pAnimator->Is_Playing(ANIMATION::KAREN::WALK))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_fDistance <= 10.f)
			m_pAnimator->Play_Animation(ANIMATION::KAREN::IDLE2, 2.f, false, 0.2f, false);
	}
	else if(m_pAnimator->Is_Playing(ANIMATION::KAREN::IDLE2))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Get_Duration(ANIMATION::KAREN::IDLE2) >= 400.f)
			GotoState(m_pState_Walk);
	}

}

void CKaren_FSM::EndState_CutScene()
{
	m_pUI_NoticeMark->NoticePlayer();
	m_pOwner.lock()->MotionReset();
}

void CKaren_FSM::BeginState_Walk()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::WALK, 2.f, false, 0.2f);
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->InitRootMotion();
	FindNewTarget();
#ifdef _DEBUG
	cout << "Karen_status_Walk" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Walk(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if(m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta);
	
	m_pOwner.lock()->RootMotion(fTimeDelta);
	{
		//GotoState(m_pState_IceCorn);
		GotoState(Function::Random({ m_pState_CloneDash,m_pState_KickCombo,m_pState_Kick1,m_pState_Kick2,m_pState_Kick3,m_pState_Kick4,m_pState_Electric,m_pState_IceCorn}));
	}
}

void CKaren_FSM::EndState_Walk()
{
	m_pOwner.lock()->MotionReset();
}

void CKaren_FSM::BeginState_Run()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::RUN, 2.f, false, 0.2f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Karen_status_Run" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Run(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);
	
	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 2.f);

	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_fDistance <= 5.f)
	{
		GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_Run()
{
	m_pOwner.lock()->MotionReset();
}

void CKaren_FSM::BeginState_KickCombo()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_KICK_A1, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pModel->Set_TrackPosition(40.f);
	CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_OneTwo.wav", 5.f);
	Teleport(2.f);
#ifdef _DEBUG
	cout << "Karen_status_kickCombo" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_KickCombo(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_KICK_A1))
	{
		if (m_pAnimator->Get_TrackPosition(ANIMATION::KAREN::ATTACK_KICK_A1) >= 80.f)
		{
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_KICK_A2, 3.f, false, 0.2f, false);
			m_pOwner.lock()->InitRootMotion();
			m_pModel->Set_TrackPosition(15.f);
		}
	}
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_KICK_A2))
	{
		if (m_pAnimator->Get_TrackPosition(ANIMATION::KAREN::ATTACK_KICK_A2) >= 100.f)
		{
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_KICK_A3, 2.f, false, 0.2f, false);
			CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 5.f);
			m_pOwner.lock()->InitRootMotion();
		}
	}
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_KICK_A3))
	{
		if (m_pAnimator->Get_TrackPosition(ANIMATION::KAREN::ATTACK_KICK_A3) >= 75.f)
			GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_KickCombo()
{
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->SetWeaponeActiveOff();
}

void CKaren_FSM::BeginState_Kick1()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_KICK_A1, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();

	CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 5.f);
	Teleport(2.f);

#ifdef _DEBUG
	cout << "Karen_status_kick1" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Kick1(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_KICK_A1))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_KICK_A1))
			GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_Kick1()
{
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->SetWeaponeActiveOff();
}

void CKaren_FSM::BeginState_Kick2()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_KICK_A2, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	Teleport(2.f);
	CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 5.f);

#ifdef _DEBUG
	cout << "Karen_status_kick2" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Kick2(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_KICK_A2))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_KICK_A2))
			GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_Kick2()
{
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->SetWeaponeActiveOff();
}

void CKaren_FSM::BeginState_Kick3()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_KICK_A3, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	Teleport(2.f);
	CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 5.f);

#ifdef _DEBUG
	cout << "Karen_status_kick3" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Kick3(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_KICK_A3))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_KICK_A3))
			GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_Kick3()
{
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->SetWeaponeActiveOff();
}

void CKaren_FSM::BeginState_Kick4()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_KICK_A4, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 5.f);
	Teleport(2.f);

#ifdef _DEBUG
	cout << "Karen_status_kick4" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Kick4(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_KICK_A4))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_KICK_A4))
			GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_Kick4()
{
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->SetWeaponeActiveOff();
}

void CKaren_FSM::BeginState_Electric()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_AIR_CHARGE_START, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	
	Teleport(5.f,XMVectorSet(0.f,5.f,0.f,0.f));
	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ L"01.Karen_Voice_Attack_Electric0.wav",L"01.Karen_Voice_Attack_Electric1.wav" }), 5.f);

	/*_vector Dir = XMVector3Normalize(m_pTargetTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION));
	_float Dist = Function::Distance(m_pTransform, m_pTargetTransform);
	m_pTransform->Translate(Dir * (Function::Distance(m_pTransform, m_pTargetTransform) - 10.f));
	m_pTransform->Translate(XMVectorSet(0.f, 5.f, 0.f, 0.f));
	m_pTransform->LookAt(m_pTargetTransform);
	
	m_pOwner.lock()->No_RootMotion(0.016f);*/
#ifdef _DEBUG
	cout << "Karen_status_Electric" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Electric(_float fTimeDelta)
{
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_AIR_CHARGE_START))
	{
		m_pOwner.lock()->Update_Motion(0.f);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_AIR_CHARGE_START))
		{
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_AIR_START, 2.f, false, 0.2f, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_AIR_START))
	{
		m_pOwner.lock()->Update_Motion(0.f);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_AIR_START))
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Electric.wav", 5.f);
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_AIR_LOOP, 3.f, false, 0.2f, false);
			_float fHeight = _float4x4(m_pModel->Get_Bone("LeftWeapon")->Get_CombinedTransformation() * m_pModel->Get_Pivot() * m_pTransform->Get_Matrix()).row(3).y - MINDROOM_TERRAIN_HEIGHT;
			VFXMGR->Fetch(VFX::KAREN_AIR_0,				make_tuple(static_pointer_cast<CGameObject>(m_pOwner.lock()), "Reference", fHeight));
			VFXMGR->Fetch(VFX::KAREN_AIR_1,				make_tuple(static_pointer_cast<CGameObject>(m_pOwner.lock()), "Reference", fHeight));
			VFXMGR->Fetch(VFX::KAREN_AIR_2,				make_tuple(static_pointer_cast<CGameObject>(m_pOwner.lock()), "Reference", fHeight));
			VFXMGR->Fetch(VFX::KAREN_AIR_HORIZONTAL,	make_tuple(static_pointer_cast<CGameObject>(m_pOwner.lock()), "Reference", fHeight));

			static _float f(0.f);
			CGameInstance::Get_Instance()->Register_OnTickListener(shared_from_this(),
				[&](_float fTimeDelta)->_bool
				{
					f += fTimeDelta;
					if (f > 0.1f)
					{
						_float4x4 mOriginal = m_pTransform->Get_Matrix();
						m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * 6.f);
						_float3 vPosition = m_pTransform->Get_State(TRANSFORM::POSITION);
						m_pOwner.lock()->SetElectricAttackTrue(vPosition);
						m_pTransform->Set_State(TRANSFORM::POSITION, _float3(vPosition.x, MINDROOM_TERRAIN_HEIGHT, vPosition.z));
						VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_0,	m_pTransform->Get_State(TRANSFORM::POSITION));
						VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_1,	m_pTransform->Get_State(TRANSFORM::POSITION));
						VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_2,	m_pTransform->Get_State(TRANSFORM::POSITION));
						VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_3,	m_pTransform->Get_State(TRANSFORM::POSITION));
						VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_4,	m_pTransform->Get_State(TRANSFORM::POSITION));
						VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_5,	m_pTransform->Get_State(TRANSFORM::POSITION));
						m_pTransform->Set_Matrix(mOriginal); 
						f = 0.f;
						return false;
					}
					else
					{
						return true;
					}
				}
			);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_AIR_LOOP))
	{
		m_pOwner.lock()->Update_Motion(0.f);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_AIR_LOOP))
		{
			m_pOwner.lock()->SetElectricAttackFalse();
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_AIR_FALL, 2.f, false, 0.2f, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_AIR_FALL))
	{
		m_pOwner.lock()->Update_Motion(fTimeDelta);
		if (m_pOwner.lock()->Is_PxColDown())
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_AIR_LANDING, 2.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_AIR_LANDING))
	{
		m_pOwner.lock()->Update_Motion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_AIR_LANDING))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_AIR_LANDING_END, 2.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_AIR_LANDING_END))
	{
		m_pOwner.lock()->Update_Motion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_AIR_LANDING_END))
			GotoState(m_pState_Walk);
	}
	m_pOwner.lock()->RootMotion(fTimeDelta);
}

void CKaren_FSM::EndState_Electric()
{
	m_pOwner.lock()->SetElectricAttackFalse();
	m_pOwner.lock()->MotionReset();
}

void CKaren_FSM::BeginState_Teleport()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::DODGE_F_START, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Karen_status_Teleport" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Teleport(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::DODGE_F_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::DODGE_F_START))
		{
			_float Distance = Function::Distance(m_pTransform, m_pTargetTransform);

			_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
			Pos.y += 2.f;

			CEffect_Manager::Get_Instance()->Fetch(VFX::KAREN_TELEPORT, Pos);
			CEffect_Manager::Get_Instance()->Fetch(VFX::KAREN_TELEPORT_DISTORTION, Pos);

			/*_float4x4 Mat = m_pTransform->Get_Matrix();
			Mat._42 += 2.f;
			CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::KAREN_TELPORT_ATT, initializer_list<any>{Mat, Mat});*/

			if (Distance >= 10.f)
				Teleport(10.f);
			else
				Teleport(Distance * 0.5f);
			m_pAnimator->Play_Animation(ANIMATION::KAREN::DODGE_F_STOP, 2.f, false, 0.2f, false);
		}
		else
			m_pOwner.lock()->RootMotion(fTimeDelta);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::DODGE_F_STOP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::DODGE_F_STOP))
			GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_Teleport()
{
	m_pOwner.lock()->MotionReset();
}

void CKaren_FSM::BeginState_Teleport_Dodge()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_COPY_START, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Karen_status_Teleport_Dodge" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Teleport_Dodge(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_COPY_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_COPY_START))
		{
			Teleport(5.f, TRANSFORM::MAX);
			GotoState(m_pState_Walk);
		}
	}
}

void CKaren_FSM::EndState_Teleport_Dodge()
{
	m_pOwner.lock()->MotionReset();
}

void CKaren_FSM::Teleport(_float Distance, _vectorf Pivot)
{
	_vector Dir = XMVector3Normalize(m_pTargetTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION));
	_float Dist = Function::Distance(m_pTransform, m_pTargetTransform);
	m_pTransform->Translate(Dir * (Function::Distance(m_pTransform, m_pTargetTransform) - Distance));
	m_pTransform->Translate(Pivot);
	m_pTransform->LookAt(m_pTargetTransform);
	m_pOwner.lock()->No_RootMotion(0.0001f);
	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ L"01.Karen_SE_MakeReplication0.wav",L"01.Karen_SE_MakeReplication2.wav" }), 5.f);
	//_float Distance = Function::Distance(m_pTransform, m_pTargetTransform);

	_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
	Pos.y += 2.f;

	CEffect_Manager::Get_Instance()->Fetch(VFX::KAREN_TELEPORT, Pos);
	CEffect_Manager::Get_Instance()->Fetch(VFX::KAREN_TELEPORT_DISTORTION, Pos);

	/*_float4x4 Mat = m_pTransform->Get_Matrix();
			Mat._42 += 2.f;
			CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::KAREN_TELPORT_ATT, initializer_list<any>{Mat, Mat});*/
}

void CKaren_FSM::Teleport(_float Distance, TRANSFORM Transform)
{
	if (Transform == TRANSFORM::MAX)
	{
		Transform = Function::Random({ TRANSFORM::LOOK, TRANSFORM::RIGHT });
		Distance = Function::Random({ -5.f, 5.f });
	}
	_vector Dir = m_pTransform->Get_State(Transform);
	m_pTransform->Translate(Dir * Distance);
	m_pTransform->LookAt(m_pTargetTransform);
	m_pOwner.lock()->No_RootMotion(0.0001f);


	//_float Distance = Function::Distance(m_pTransform, m_pTargetTransform);

	_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
	Pos.y += 2.f;

	CEffect_Manager::Get_Instance()->Fetch(VFX::KAREN_TELEPORT, Pos);
	CEffect_Manager::Get_Instance()->Fetch(VFX::KAREN_TELEPORT_DISTORTION, Pos);

	/*_float4x4 Mat = m_pTransform->Get_Matrix();
			Mat._42 += 2.f;
			CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::KAREN_TELPORT_ATT, initializer_list<any>{Mat, Mat});*/
}

void CKaren_FSM::BeginState_IceCorn()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_FIRE_CHARGE_START, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ L"01.Karen_Voice_Attack_IceNeedle0.wav",L"01.Karen_Voice_Attack_IceNeedle1.wav",L"01.Karen_Voice_Attack_IceNeedle2.wav" }), 5.f);

#ifdef _DEBUG
	cout << "Karen_status_Teleport" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_IceCorn(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_FIRE_CHARGE_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_FIRE_CHARGE_START))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_FIRE_CHARGE_LOOP, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_FIRE_CHARGE_LOOP))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_FIRE_CHARGE_LOOP))
		{
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_FIRE_START, 2.f, false, 0.2f, false);
			CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_IceNeedle.wav", 5.f);
			m_pOwner.lock()->IceCornDecal(m_pPlayerTransform);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_FIRE_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_FIRE_START))
		{
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_FIRE_LOOP, 3.f, false, 0.2f, false);
			m_pOwner.lock()->IceCorn(m_pPlayerTransform);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_FIRE_LOOP))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_FIRE_LOOP))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_FIRE_END, 2.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_FIRE_END))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_FIRE_END))
			GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_IceCorn()
{
	m_pOwner.lock()->MotionReset();
}

void CKaren_FSM::BeginState_CloneDash()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_COPY_START, 3.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	Teleport(10.f);
	m_pOwner.lock()->SetRealCloned(m_pTargetTransform);
#ifdef _DEBUG
	cout << "Karen_status_Teleport_Dodge" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_CloneDash(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_COPY_START))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_COPY_START))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_COPY_LOOP, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_COPY_LOOP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_COPY_LOOP))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_CROUCH_START, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_CROUCH_START))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_CROUCH_START))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_CROUCH_LOOP, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_CROUCH_LOOP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_CROUCH_LOOP))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_RUSH_START, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_RUSH_START))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 30.f);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_RUSH_START))
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"01.Karen_SE_Attack_Basic_Attack_Three.wav", 3.f);
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_RUSH_LOOP, 3.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_RUSH_LOOP))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 30.f);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if ((m_fDegree >= 90.f || m_fDegree <= -90.f) && m_fDistance >= 10.f)
		//if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_RUSH_LOOP))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::ATTACK_RUSH_END, 3.f, false, 0.2f, false);
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::KAREN::ATTACK_RUSH_END))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::ATTACK_RUSH_END))
		{
			GotoState(m_pState_Walk);
		}
	}
}

void CKaren_FSM::EndState_CloneDash()
{
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->SetRealCloned();
}

void CKaren_FSM::BeginState_DodgeCloned()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::DODGE_JUST_COPY, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->SetCloned();
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	CUI_Manager::Get_Instance()->Set_NoDamage(true);
#ifdef _DEBUG
	cout << "Karen_status_DodgeCloned" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_DodgeCloned(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::DODGE_JUST_COPY))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::DODGE_JUST_COPY))
			GotoState(m_pState_Walk);
	}
}

void CKaren_FSM::EndState_DodgeCloned()
{
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->SetCloned();
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_fCoolTime = 10.f; 
	CUI_Manager::Get_Instance()->Set_NoDamage(false);
}

void CKaren_FSM::BeginState_Dead()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::DOWN_F, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Dissolve();
	m_pOwner.lock()->DeadSlow();
	m_pOwner.lock()->Set_LockOnAble(false);
	m_pOwner.lock()->ReleaseController(); 
	for (shared_ptr<CPlayerTentacle> elem : m_pOwner.lock()->Get_Tentacle())
		elem->Set_Dead(true);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
#ifdef _DEBUG
	cout << "Karen_status_Death" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	//m_pOwner.lock()->Update_Motion(fTimeDelta);
	//m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::KAREN::DOWN_F))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::DOWN_F))
		{
			//m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
			//m_pOwner.lock()->No_RootMotion(fTimeDelta);
			m_pOwner.lock()->SetDead();
			//m_pAnimator->Set_Default(ANIMATION::KAREN::DOWN);
			//m_pAnimator->Play_Animation(ANIMATION::KAREN::DOWN);
		}
	}
}

void CKaren_FSM::EndState_Dead()
{
	m_pOwner.lock()->MotionReset();
};

void CKaren_FSM::BeginState_FinishMotion()
{
	m_pAnimator->Play_Animation(ANIMATION::KAREN::BC_START, 2.f, false, 0.5f,false);
	m_pOwner.lock()->InitRootMotion();
	//m_pOwner.lock()->SetWeakRender(true);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
	m_pTransform->LookAt(m_pTargetTransform);
	m_pOwner.lock()->Set_LockOnAble(false);

	//static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Set_FinishMonster(static_pointer_cast<CMonster>(m_pOwner.lock()));
	static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Set_ChangeState(make_shared<CPlayerFinalState>());


	auto pGameInstance = CGameInstance::Get_Instance();
	auto pTeammateLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_TEAMMATE);
	pTeammateLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			auto pTeammate = dynamic_pointer_cast<CTeammate>(pObject);
			pTeammate->Set_Enable(false);

			return true;
		});

#ifdef _DEBUG
	cout << "Karen_status_BCChance" << endl;
#endif // _DEBUG
}

void CKaren_FSM::ExecuteState_FinishMotion(_float fTimeDelta)
{
	if(m_pAnimator->Is_Playing(ANIMATION::KAREN::BC_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::BC_START))
			m_pAnimator->Play_Animation(ANIMATION::KAREN::BC_END, 2.f, false, 0.5f, false);
	}
	else if(m_pAnimator->Is_Playing(ANIMATION::KAREN::BC_END))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::KAREN::BC_END))
			GotoState(m_pState_Dead);
	}
}

void CKaren_FSM::EndState_FinishMotion()
{
	m_pOwner.lock()->MotionReset();

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pTeammateLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_TEAMMATE);
	pTeammateLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			auto pTeammate = dynamic_pointer_cast<CTeammate>(pObject);
			pTeammate->Set_Enable(true);

			return true;
		});
}

void CKaren_FSM::FindNewTarget()
{
	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::TEST || CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST)
	{
		m_pTargetTransform = m_pPlayerTransform;
		return;
	}
	if (!m_pArashTransform)
	{
		m_pTargetTransform = m_pPlayerTransform;
		return;
	}
	m_pTargetTransform = Function::Random({ m_pPlayerTransform,m_pArashTransform,m_pShidengTransform });
}

shared_ptr<CKaren_FSM> CKaren_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	shared_ptr<CKaren_FSM> pInstance = make_private_shared(CKaren_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CKaren_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
