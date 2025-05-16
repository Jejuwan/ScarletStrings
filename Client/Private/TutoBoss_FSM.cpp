#include "ClientPCH.h"
#include "TutoBoss_FSM.h"
#include "GameInstance.h"
#include "Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"

HRESULT CTutoBoss_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CTutoBoss_FSM> pInstance = static_pointer_cast<CTutoBoss_FSM>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Idle->Set(pInstance, &CTutoBoss_FSM::BeginState_Idle, &CTutoBoss_FSM::ExecuteState_Idle, &CTutoBoss_FSM::EndState_Idle);
	m_pState_Walk = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Walk->Set(pInstance, &CTutoBoss_FSM::BeginState_Walk, &CTutoBoss_FSM::ExecuteState_Walk, &CTutoBoss_FSM::EndState_Walk);
	m_pState_Walk_Left = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Walk_Left->Set(pInstance, &CTutoBoss_FSM::BeginState_Walk_Left, &CTutoBoss_FSM::ExecuteState_Walk_Left, &CTutoBoss_FSM::EndState_Walk_Left);
	m_pState_Walk_Right = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Walk_Right->Set(pInstance, &CTutoBoss_FSM::BeginState_Walk_Right, &CTutoBoss_FSM::ExecuteState_Walk_Right, &CTutoBoss_FSM::EndState_Walk_Right);
	m_pState_Walk_Back = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Walk_Back->Set(pInstance, &CTutoBoss_FSM::BeginState_Walk_Back, &CTutoBoss_FSM::ExecuteState_Walk_Back, &CTutoBoss_FSM::EndState_Walk_Back);
	m_pState_Run = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Run->Set(pInstance, &CTutoBoss_FSM::BeginState_Run, &CTutoBoss_FSM::ExecuteState_Run, &CTutoBoss_FSM::EndState_Run);
	m_pState_Bite = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Bite->Set(pInstance, &CTutoBoss_FSM::BeginState_Bite, &CTutoBoss_FSM::ExecuteState_Bite, &CTutoBoss_FSM::EndState_Bite);
	m_pState_Laser = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Laser->Set(pInstance, &CTutoBoss_FSM::BeginState_Laser, &CTutoBoss_FSM::ExecuteState_Laser, &CTutoBoss_FSM::EndState_Laser);
	m_pState_OnHit_Front = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_OnHit_Front->Set(pInstance, &CTutoBoss_FSM::BeginState_OnHit_Front, &CTutoBoss_FSM::ExecuteState_OnHit_Front, &CTutoBoss_FSM::EndState_OnHit_Front);
	m_pState_OnHit_Back = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_OnHit_Back->Set(pInstance, &CTutoBoss_FSM::BeginState_OnHit_Back, &CTutoBoss_FSM::ExecuteState_OnHit_Back, &CTutoBoss_FSM::EndState_OnHit_Back);
	m_pState_OnHit_Psy_Front = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_OnHit_Psy_Front->Set(pInstance, &CTutoBoss_FSM::BeginState_OnHit_Psy_Front, &CTutoBoss_FSM::ExecuteState_OnHit_Psy_Front, &CTutoBoss_FSM::EndState_OnHit_Psy_Front);
	m_pState_OnHit_Psy_Back = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_OnHit_Psy_Back->Set(pInstance, &CTutoBoss_FSM::BeginState_OnHit_Psy_Back, &CTutoBoss_FSM::ExecuteState_OnHit_Psy_Back, &CTutoBoss_FSM::EndState_OnHit_Psy_Back);
	m_pState_OnHit_Psy_Left = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_OnHit_Psy_Left->Set(pInstance, &CTutoBoss_FSM::BeginState_OnHit_Psy_Left, &CTutoBoss_FSM::ExecuteState_OnHit_Psy_Left, &CTutoBoss_FSM::EndState_OnHit_Psy_Left);
	m_pState_OnHit_Psy_Right = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_OnHit_Psy_Right->Set(pInstance, &CTutoBoss_FSM::BeginState_OnHit_Psy_Right, &CTutoBoss_FSM::ExecuteState_OnHit_Psy_Right, &CTutoBoss_FSM::EndState_OnHit_Psy_Right);
	m_pState_Dead = make_shared<CStateTemplate<CTutoBoss_FSM>>();
	m_pState_Dead->Set(pInstance, &CTutoBoss_FSM::BeginState_Dead, &CTutoBoss_FSM::ExecuteState_Dead, &CTutoBoss_FSM::EndState_Dead);

	//auto pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_TutoBoss>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CTutoBoss_FSM::Initialize", "Failed to Get: Owner");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_pOwner.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTutoBoss_FSM::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pTargetTransform = dynamic_pointer_cast<CTransform>(Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CTutoBoss_FSM::Initialize", "Failed to Get: Player:Transform");
	}
	
	m_pModel = dynamic_pointer_cast<CModel>(m_pOwner.lock()->Get_Component(COMPONENT::MODEL));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CTutoBoss_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pAnimator = dynamic_pointer_cast<CAnimator>(m_pOwner.lock()->Get_Behavior(BEHAVIOR::ANIMATOR));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CTutoBoss_FSM::Initialize", "Failed to Get: Player:Transform");
	}
	
	if (nullptr == pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CTutoBoss_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;
	
	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CTutoBoss_FSM::Initialize", " pMonsterDesc is NULL");
	}
	m_pActorDesc = pActorDesc;
	//GotoState(m_pState_Idle);
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION5, std::bind(&CTutoBoss_FSM::AgariBeamEff, this));
#pragma region UI
	//m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	//m_pUI_NoticeMark->Initialize();
#pragma endregion

	return S_OK;
}

void CTutoBoss_FSM::StateInitial(_float fTimeDelta)
{
	GotoState(m_pState_Idle);
}

void CTutoBoss_FSM::ExecutePersistent(_float fTimeDelta)
{
	if (m_pCurrentState == m_pState_Dead)
		return;

	//m_pUI_NoticeMark->Tick(fTimeDelta);
	//m_pUI_NoticeMark->Set_IndicatorPosition(this->m_pTransform->Get_State(TRANSFORM::POSITION), this->m_pTransform->Get_WorldMatrix());
	//m_pUI_NoticeMark->Late_Tick(fTimeDelta);

	if (dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH && m_pCurrentState != m_pState_Idle)
	{
		GotoState(m_pState_Idle);
		return;
	}

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

	auto pOwner = m_pOwner.lock();
	if (m_pActorDesc->bHit)
	{
		pOwner->On_HitBlink();

		if (m_pCurrentState == m_pState_Dead)
			return;
		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}

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
		return;

		if (m_pCurrentState == m_pState_Bite)
			return;
		if (m_pCurrentState == m_pState_Laser)
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
		pOwner->On_HitBlink();

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

		if (m_pCurrentState == m_pState_Laser)
			return;

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

void CTutoBoss_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::IDLE1, 2.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "TutoBoss_status_Idle" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	GotoState(Function::Random({ m_pState_Walk }));
	return;
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_fDistance <= 5.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	{
		GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
	}
}

void CTutoBoss_FSM::EndState_Idle()
{
	//m_pUI_NoticeMark->NoticePlayer();

	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_Walk()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::WALK_FRONT, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "TutoBoss_status_Walk_Forward" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_Walk(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);
	//m_pTransform->LookAt_Interpolation(m_pTargetTransform);

	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 2.f);

	m_pOwner.lock()->RootMotion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::WALK_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::WALK_FRONT))
		{
			m_pOwner.lock()->MotionReset();
			
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::WALK_FRONT_LOOP, 2.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::WALK_FRONT_LOOP))
	{
		if (m_fCoolTime <= 0)
		{
			if (abs(m_fDegree) <= 5.f)
				GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right }));
			/*if (m_fDistance >= 10.f)
			{
				GotoState(m_pState_Laser);
			}
			else if (m_fDistance <= 5.f)
			{
				GotoState(m_pState_Bite);
			}*/
		}
		else
		{
			/*if (m_fDistance <= 5.f)
				GotoState(m_pState_Walk_Back);
			else if (m_fDistance <= 10.f)
				GotoState(Function::Random({ m_pState_Walk_Left,m_pState_Walk_Right }));*/
		}
	}
}

void CTutoBoss_FSM::EndState_Walk()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_Walk_Left()
{
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::WALK_LEFT, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "TutoBoss_status_Walk_Left" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_Walk_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta); 
	//m_pTransform->LookAt_Interpolation(m_pTargetTransform);


	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::WALK_LEFT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::WALK_LEFT))
		{
			m_pOwner.lock()->MotionReset();
			
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::WALK_LEFT_LOOP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::WALK_LEFT_LOOP))
	{
		m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::RIGHT) * fTimeDelta * 3.f);
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::WALK_LEFT_LOOP))
		{
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
			/*if (m_fDistance >= 10.f)
			{
				GotoState(m_pState_Laser);
			}
			else if (m_fDistance <= 10.f)
			{
				GotoState(m_pState_Bite);
			}*/
		}
	}
	m_pOwner.lock()->RootMotion(fTimeDelta, true);
}

void CTutoBoss_FSM::EndState_Walk_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_Walk_Right()
{
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::WALK_RIGHT, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "TutoBoss_status_Walk_Right" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_Walk_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);
	//m_pTransform->LookAt_Interpolation(m_pTargetTransform);


	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::WALK_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::WALK_RIGHT))
		{
			m_pOwner.lock()->MotionReset();
			
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::WALK_RIGHT_LOOP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::WALK_RIGHT_LOOP))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::RIGHT) * fTimeDelta * 3.f);
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::WALK_RIGHT_LOOP))
		{
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
			/*if (m_fDistance >= 10.f)
			{
				GotoState(m_pState_Laser);
			}
			else if (m_fDistance <= 10.f)
			{
				GotoState(m_pState_Bite);
			}*/
		}
	}
	m_pOwner.lock()->RootMotion(fTimeDelta, true);
}

void CTutoBoss_FSM::EndState_Walk_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_Walk_Back()
{
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::WALK_BACK, 2.f, false, 0.1f, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "TutoBoss_status_Walk_Back" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_Walk_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);
	//m_pTransform->LookAt_Interpolation(m_pTargetTransform);

	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::WALK_BACK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::WALK_BACK))
		{
			m_pOwner.lock()->MotionReset();
			
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::WALK_BACK_LOOP, 2.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::WALK_BACK_LOOP))
	{
		m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta);
		if (m_fCoolTime <= 0 && (m_fDegree <= 5.f && m_fDegree >= -5.f))
		{
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
			/*if (m_fDistance >= 10.f)
			{
				GotoState(m_pState_Laser);
			}
			else if (m_fDistance <= 10.f)
			{
				GotoState(m_pState_Bite);
			}*/
		}
	}
	m_pOwner.lock()->RootMotion(fTimeDelta, true);
}

void CTutoBoss_FSM::EndState_Walk_Back()
{
	m_pOwner.lock()->MotionReset();
}


void CTutoBoss_FSM::BeginState_Run()
{
}

void CTutoBoss_FSM::ExecuteState_Run(_float fTimeDelta)
{
}

void CTutoBoss_FSM::EndState_Run()
{
}

void CTutoBoss_FSM::BeginState_Bite()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::RUN_START, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "TutoBoss Bite State" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_Bite(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	//m_pTransform->LookAt_Interpolation(m_pTargetTransform);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::RUN_START))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::RUN_START))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::RUN_LOOP, 2.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::RUN_LOOP))
	{
		if (m_fDegree >= 5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta * 2.f);
		else if (m_fDegree <= -5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta * 2.f);
		_vector Look = m_pTransform->Get_State(TRANSFORM::LOOK);
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * 15.f * fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);

		if (m_fDistance <= 5.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::ATTACK_BITE, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::ATTACK_BITE))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::ATTACK_BITE))
		{
			m_pOwner.lock()->MotionReset();
			//m_fCoolTime = 15.f;
			GotoState(m_pState_Walk);
		}
	}
}

void CTutoBoss_FSM::EndState_Bite()
{
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_Laser()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::ATTACK_LASER_START, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "TutoBoss Laser State" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_Laser(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::ATTACK_LASER_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::ATTACK_LASER_START))
		{
		//	_float4x4 mOriginal = m_pTransform->Get_Matrix();
		//	m_pTransform->Rotate(_float3(0.f, 1.f, 0.f), -XM_PIDIV2);
		//	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::RIGHT) * 0.1f);
		//	_float4x4 mBeam = m_pTransform->Get_Matrix();
		//	m_pTransform->Set_Matrix(mOriginal);
		//	pair<shared_ptr<CGameObject>, const char*> arg = make_pair(m_pOwner.lock(), "Eff02");
		//	CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::EM800_BEAM, initializer_list<any>
		//	{mOriginal, mOriginal, mOriginal, mOriginal, mBeam, mBeam, mBeam, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal});
		//	{mOriginal, mOriginal, mBeam, mBeam, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal});
		//	{arg, arg, arg, arg, arg, arg, arg, arg, arg, arg, arg, arg});
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::ATTACK_LASER_LOOP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::ATTACK_LASER_LOOP))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::ATTACK_LASER_LOOP))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::ATTACK_LASER_END, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::ATTACK_LASER_END))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::ATTACK_LASER_END))
		{
			m_pOwner.lock()->MotionReset();
			//m_fCoolTime = 15.f;
			GotoState(m_pState_Walk);
		}
	}
}

void CTutoBoss_FSM::EndState_Laser()
{
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_OnHit_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::DAMAGE_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Front" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_OnHit_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::DAMAGE_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::DAMAGE_FRONT))
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
	}
}

void CTutoBoss_FSM::EndState_OnHit_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_OnHit_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::DAMAGE_BACK, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Back" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_OnHit_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::DAMAGE_BACK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::DAMAGE_BACK))
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
	}
}

void CTutoBoss_FSM::EndState_OnHit_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_OnHit_Psy_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_PSY_FRONT" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_OnHit_Psy_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::DAMAGE_PSY_FRONT))
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
	}
}

void CTutoBoss_FSM::EndState_OnHit_Psy_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_OnHit_Psy_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Back" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_OnHit_Psy_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::DAMAGE_PSY_FRONT))
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
	}
}

void CTutoBoss_FSM::EndState_OnHit_Psy_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_OnHit_Psy_Left()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::DAMAGE_PSY_LEFT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Left" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_OnHit_Psy_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::DAMAGE_PSY_LEFT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::DAMAGE_PSY_LEFT))
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
	}
}

void CTutoBoss_FSM::EndState_OnHit_Psy_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_OnHit_Psy_Right()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::DAMAGE_PSY_RIGHT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Right" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_OnHit_Psy_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::DAMAGE_PSY_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::DAMAGE_PSY_RIGHT))
			GotoState(Function::Random({ m_pState_Laser ,m_pState_Bite, m_pState_Walk_Left,m_pState_Walk_Right,m_pState_Walk }));
	}

}

void CTutoBoss_FSM::EndState_OnHit_Psy_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CTutoBoss_FSM::BeginState_Dead()
{
	m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::DEAD_DOWN, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->Dissolve();
	m_pOwner.lock()->DeadSlow();
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Set_LockOnAble(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
#ifdef _DEBUG
	cout << "TutoBoss_status_Death" << endl;
#endif // _DEBUG
}

void CTutoBoss_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::DEAD_DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::DEAD_DOWN))
		{
			m_pOwner.lock()->SetDead();
			m_pAnimator->Set_Default(ANIMATION::TUTOBOSS::DOWN);
			m_pAnimator->Play_Animation(ANIMATION::TUTOBOSS::DOWN , 2.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TUTOBOSS::DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TUTOBOSS::DOWN))
		{
			//nothing
		}
	}
	
}

void CTutoBoss_FSM::EndState_Dead()
{
	m_pOwner.lock()->MotionReset();
}


void CTutoBoss_FSM::AgariBeamEff()
{
	pair<shared_ptr<CGameObject>, const char*> arg = make_pair(m_pOwner.lock(), "Eff02");
	CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::EM800_BEAM, initializer_list<any>
	{arg, arg, arg, arg, arg, arg, arg, arg, arg, arg, arg, arg});
}

shared_ptr<CTutoBoss_FSM> CTutoBoss_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	shared_ptr<CTutoBoss_FSM> pInstance = make_private_shared(CTutoBoss_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CTutoBoss_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
