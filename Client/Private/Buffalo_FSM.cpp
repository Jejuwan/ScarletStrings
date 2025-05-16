
#include "ClientPCH.h"
#include "Buffalo_FSM.h"
#include "GameInstance.h"
#include "Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "Camera_Player.h"

#include "UI_Manager.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"

HRESULT CBuffalo_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CBuffalo_FSM> pInstance = static_pointer_cast<CBuffalo_FSM>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_Idle->Set(pInstance, &CBuffalo_FSM::BeginState_Idle, &CBuffalo_FSM::ExecuteState_Idle, &CBuffalo_FSM::EndState_Idle);
	m_pState_Walk = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_Walk->Set(pInstance, &CBuffalo_FSM::BeginState_Walk, &CBuffalo_FSM::ExecuteState_Walk, &CBuffalo_FSM::EndState_Walk);
	m_pState_Run = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_Run->Set(pInstance, &CBuffalo_FSM::BeginState_Run, &CBuffalo_FSM::ExecuteState_Run, &CBuffalo_FSM::EndState_Run);
	m_pState_Swing = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_Swing->Set(pInstance, &CBuffalo_FSM::BeginState_Swing, &CBuffalo_FSM::ExecuteState_Swing, &CBuffalo_FSM::EndState_Swing);
	m_pState_Rush = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_Rush->Set(pInstance, &CBuffalo_FSM::BeginState_Rush, &CBuffalo_FSM::ExecuteState_Rush, &CBuffalo_FSM::EndState_Rush);
	m_pState_BackKick = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_BackKick->Set(pInstance, &CBuffalo_FSM::BeginState_BackKick, &CBuffalo_FSM::ExecuteState_BackKick, &CBuffalo_FSM::EndState_BackKick);
	m_pState_OnHit_Front = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_OnHit_Front->Set(pInstance, &CBuffalo_FSM::BeginState_OnHit_Front, &CBuffalo_FSM::ExecuteState_OnHit_Front, &CBuffalo_FSM::EndState_OnHit_Front);
	m_pState_OnHit_Back = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_OnHit_Back->Set(pInstance, &CBuffalo_FSM::BeginState_OnHit_Back, &CBuffalo_FSM::ExecuteState_OnHit_Back, &CBuffalo_FSM::EndState_OnHit_Back);
	m_pState_OnHit_Psy_Front = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_OnHit_Psy_Front->Set(pInstance, &CBuffalo_FSM::BeginState_OnHit_Psy_Front, &CBuffalo_FSM::ExecuteState_OnHit_Psy_Front, &CBuffalo_FSM::EndState_OnHit_Psy_Front);
	m_pState_OnHit_Psy_Back = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_OnHit_Psy_Back->Set(pInstance, &CBuffalo_FSM::BeginState_OnHit_Psy_Back, &CBuffalo_FSM::ExecuteState_OnHit_Psy_Back, &CBuffalo_FSM::EndState_OnHit_Psy_Back);
	m_pState_OnHit_Psy_Left = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_OnHit_Psy_Left->Set(pInstance, &CBuffalo_FSM::BeginState_OnHit_Psy_Left, &CBuffalo_FSM::ExecuteState_OnHit_Psy_Left, &CBuffalo_FSM::EndState_OnHit_Psy_Left);
	m_pState_OnHit_Psy_Right = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_OnHit_Psy_Right->Set(pInstance, &CBuffalo_FSM::BeginState_OnHit_Psy_Right, &CBuffalo_FSM::ExecuteState_OnHit_Psy_Right, &CBuffalo_FSM::EndState_OnHit_Psy_Right);
	m_pState_Dead = make_shared<CStateTemplate<CBuffalo_FSM>>();
	m_pState_Dead->Set(pInstance, &CBuffalo_FSM::BeginState_Dead, &CBuffalo_FSM::ExecuteState_Dead, &CBuffalo_FSM::EndState_Dead);

	//auto pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_Buffalo>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CBuffalo_FSM::Initialize", "Failed to Get: Owner");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_pOwner.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBuffalo_FSM::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pTargetTransform = dynamic_pointer_cast<CTransform>(Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBuffalo_FSM::Initialize", "Failed to Get: Player:Transform");
	}
	
	m_pModel = dynamic_pointer_cast<CModel>(m_pOwner.lock()->Get_Component(COMPONENT::MODEL));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBuffalo_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pAnimator = dynamic_pointer_cast<CAnimator>(m_pOwner.lock()->Get_Behavior(BEHAVIOR::ANIMATOR));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBuffalo_FSM::Initialize", "Failed to Get: Player:Transform");
	}
	
	if (nullptr == pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CBuffalo_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;
	
	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CBuffalo_FSM::Initialize", " pMonsterDesc is NULL");
	}
	m_pActorDesc = pActorDesc;
	//GotoState(m_pState_Idle);

#pragma region UI
	m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	m_pUI_NoticeMark->Initialize();
#pragma endregion

	return S_OK;
}

void CBuffalo_FSM::StateInitial(_float fTimeDelta)
{
	GotoState(m_pState_Idle);
}

void CBuffalo_FSM::ExecutePersistent(_float fTimeDelta)
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
	
	if (m_fCoolTime >= 0)
		m_fCoolTime -= fTimeDelta;

	_vector myLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector Direction = m_pTargetTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);
	
	_float Radian = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
	_vector Cross = XMVector3Cross(myLook, Direction);
	
	if (XMVectorGetY(Cross) <= 0.f)
		Radian *= -1;

	m_fDegree = XMConvertToDegrees(Radian);

	if (m_pActorDesc->bHit)
	{
		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}

		auto pGameInstance = CGameInstance::Get_Instance();
		CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
		if (nullptr == pPlayer->Get_Camera()->Get_LockOnTransform() &&
			m_pActorDesc->stHitDesc.strAttackTag != ACTORTAG::TEAMMATE_ATTACK)
		{
			pPlayer->Get_Camera()->Set_LockOn(true);
			pPlayer->Get_Camera()->Set_LockedMonster(m_pOwner.lock());
			pPlayer->Get_Camera()->Set_LockOnTransform(m_pTransform);
			pPlayer->Set_FinishMonster(m_pOwner.lock());
		}

		m_pEntityDesc->fHP -= m_pActorDesc->stHitDesc.iDamage;
		//if (m_pCurrentState == m_pState_Idle) 
		//	GotoState(m_pState_Walk);
		return;

		if (m_pCurrentState == m_pState_Rush)
			return;
		if (m_pCurrentState == m_pState_BackKick)
			return;
		if (m_pCurrentState == m_pState_Swing)
			return;

		if (!m_pActorDesc->bThrowHit)
		{
			if (90.f >= m_fDegree && -90.f <= m_fDegree)
				GotoState(m_pState_OnHit_Front);
			else
				GotoState(m_pState_OnHit_Back);
		}
	}
	if (m_pActorDesc->bThrowHit)
	{
		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}

		auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
		auto Interactive = Kynesis->Get_RecentThrowdObject();
		if (Interactive == nullptr)
			return;

		auto pGameInstance = CGameInstance::Get_Instance();
		CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
		if (nullptr == pPlayer->Get_Camera()->Get_LockOnTransform())
		{
			pPlayer->Get_Camera()->Set_LockOn(true);
			pPlayer->Get_Camera()->Set_LockedMonster(m_pOwner.lock());
			pPlayer->Get_Camera()->Set_LockOnTransform(m_pTransform);
			pPlayer->Set_FinishMonster(m_pOwner.lock());
		}

		m_pEntityDesc->fHP -= m_pActorDesc->stHitDesc.iDamage;
		shared_ptr<CTransform> InterActiveTransform = dynamic_pointer_cast<CTransform>(Interactive->Get_Component(COMPONENT::TRANSFORM));

		_vector Direction = InterActiveTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);

		_float Radian = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
		_vector Cross = XMVector3Cross(myLook, Direction);

		if (XMVectorGetY(Cross) <= 0.f)
			Radian *= -1;
		_float InteractiveDegree = XMConvertToDegrees(Radian);

		if (45.f >= InteractiveDegree && -45.f <= InteractiveDegree)
			GotoState(m_pState_OnHit_Psy_Front);
		else if (-135.f >= InteractiveDegree || 135.f <= InteractiveDegree)
			GotoState(m_pState_OnHit_Psy_Back);
		else if (135.f >= InteractiveDegree && 45 <= InteractiveDegree)
			GotoState(m_pState_OnHit_Psy_Right);
		else if (-45.f >= InteractiveDegree && -135.f <= InteractiveDegree)
			GotoState(m_pState_OnHit_Psy_Left);
		Kynesis->Set_RecentThrowObject(nullptr);
	}
}

void CBuffalo_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::IDLE1, 3.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Buffalo_status_Idle" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	//if (m_fDistance <= 5.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	//{
	//	GotoState(m_pState_Walk);
	//}
	GotoState(m_pState_Walk);
}

void CBuffalo_FSM::EndState_Idle()
{
	m_pUI_NoticeMark->NoticePlayer();

	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_Walk()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::WALK_FRONT, 2.f, false, g_fDefaultInterpolationDuration);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->RootMotion(0.f);
#ifdef _DEBUG
	cout << "Buffalo_status_Walk_Forward" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_Walk(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

	//m_pTransform->LookAt(m_pTargetTransform);

	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * 3 * fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);

	if (m_fCoolTime <= 0 && (m_fDegree <= 5.f && m_fDegree >= -5.f))
	{
		GotoState(Function::Random({ m_pState_Swing, m_pState_Rush }));
	}
	else if (m_fDistance <= 3.f)
		GotoState(m_pState_BackKick);

}

void CBuffalo_FSM::EndState_Walk()
{
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_Run()
{
}

void CBuffalo_FSM::ExecuteState_Run(_float fTimeDelta)
{
}

void CBuffalo_FSM::EndState_Run()
{
}

void CBuffalo_FSM::BeginState_Swing()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::TRANSFORM_UP, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Buffalo Swing State" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_Swing(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	
	//m_pTransform->LookAt(m_pTargetTransform);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::TRANSFORM_UP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_fDegree >= 5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
		else if (m_fDegree <= -5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::TRANSFORM_UP))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::BUFFALO::TWO_FOOT_RUN, 3.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::TWO_FOOT_RUN))
	{
		if (m_fDegree >= 5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
		else if (m_fDegree <= -5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);
		_vector Look = m_pTransform->Get_State(TRANSFORM::LOOK);
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * 15.f * fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_fDistance <= 5.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::BUFFALO::ATTACK1_SWING_R, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::ATTACK1_SWING_R))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_fDegree >= 5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
		else if (m_fDegree <= -5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::ATTACK1_SWING_R))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::BUFFALO::ATTACK2_SWING_L, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::ATTACK2_SWING_L))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_fDegree >= 5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
		else if (m_fDegree <= -5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::ATTACK2_SWING_L))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::BUFFALO::ATTACK3_STUMP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::ATTACK3_STUMP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::ATTACK3_STUMP))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::BUFFALO::ATTACK3_STUMP_END, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::ATTACK3_STUMP_END))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::ATTACK3_STUMP_END))
		{
			m_pOwner.lock()->MotionReset();
			//m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}
}

void CBuffalo_FSM::EndState_Swing()
{
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_Rush()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::ATTACK_RUSH_START, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
	
#ifdef _DEBUG
	cout << "Buffalo Rush State" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_Rush(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::ATTACK_RUSH_START))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Get_TrackPosition(ANIMATION::BUFFALO::ATTACK_RUSH_START) >= 105.f)
		{
			if (m_fDegree >= 1.f)
				m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
			else if (m_fDegree <= -1.f)
				m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);
		}

		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::ATTACK_RUSH_START))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::BUFFALO::ATTACK_RUSH_LOOP, 2.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::ATTACK_RUSH_LOOP))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * 15.f * fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (abs(m_fDegree) >= 140.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::BUFFALO::ATTACK_RUSH_STOP_RETURN, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::ATTACK_RUSH_STOP_RETURN))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::ATTACK_RUSH_STOP_RETURN))
		{
			m_pTransform->LookAt(m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK));
			m_pOwner.lock()->MotionReset();
			//m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}
}

void CBuffalo_FSM::EndState_Rush()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::WALK_FRONT, 2.f, false, 0.f);
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_BackKick()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::ATTACK_BACKFOOT_L, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Buffalo Rush State" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_BackKick(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::ATTACK_BACKFOOT_L))
	{
		if (m_pAnimator->Get_TrackPosition(ANIMATION::BUFFALO::ATTACK_BACKFOOT_L) >= 120.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pTransform->LookAt(m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK));
			GotoState(m_pState_Walk);
		}
	}
}

void CBuffalo_FSM::EndState_BackKick()
{
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_OnHit_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::DAMAGE_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Front" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_OnHit_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta, true);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::DAMAGE_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::DAMAGE_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CBuffalo_FSM::EndState_OnHit_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_OnHit_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::DAMAGE_BACK, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Back" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_OnHit_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::DAMAGE_BACK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::DAMAGE_BACK))
			GotoState(m_pState_Walk);
	}
}

void CBuffalo_FSM::EndState_OnHit_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_OnHit_Psy_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_PSY_FRONT" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_OnHit_Psy_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CBuffalo_FSM::EndState_OnHit_Psy_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_OnHit_Psy_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Back" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_OnHit_Psy_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CBuffalo_FSM::EndState_OnHit_Psy_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_OnHit_Psy_Left()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::DAMAGE_PSY_LEFT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Left" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_OnHit_Psy_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::DAMAGE_PSY_LEFT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::DAMAGE_PSY_LEFT))
			GotoState(m_pState_Walk);
	}
}

void CBuffalo_FSM::EndState_OnHit_Psy_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_OnHit_Psy_Right()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::DAMAGE_PSY_RIGHT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Right" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_OnHit_Psy_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::DAMAGE_PSY_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::DAMAGE_PSY_RIGHT))
			GotoState(m_pState_Walk);
	}
}

void CBuffalo_FSM::EndState_OnHit_Psy_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CBuffalo_FSM::BeginState_Dead()
{
	m_pAnimator->Play_Animation(ANIMATION::BUFFALO::DEAD_DOWN, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->Dissolve();
	m_pOwner.lock()->DeadSlow();
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Set_LockOnAble(false);
	m_pOwner.lock()->SetWeaponeActiveOff();
	CGameInstance::Get_Instance()->PlaySoundW(L"Monster_Buffalo_Die.wav", 3.f);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
#ifdef _DEBUG
	cout << "Buffalo_status_Death" << endl;
#endif // _DEBUG
}

void CBuffalo_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::DEAD_DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::DEAD_DOWN))
		{
			m_pOwner.lock()->SetDead();
			m_pAnimator->Set_Default(ANIMATION::BUFFALO::DOWN);
			m_pAnimator->Play_Animation(ANIMATION::BUFFALO::DOWN, 2.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BUFFALO::DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BUFFALO::DOWN))
		{
			//nothing
		}
	}
}

void CBuffalo_FSM::EndState_Dead()
{
	m_pOwner.lock()->MotionReset();
}

shared_ptr<CBuffalo_FSM> CBuffalo_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	shared_ptr<CBuffalo_FSM> pInstance = make_private_shared(CBuffalo_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CBuffalo_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
