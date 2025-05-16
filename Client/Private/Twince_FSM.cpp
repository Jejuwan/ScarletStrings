#include "ClientPCH.h"
#include "Twince_FSM.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_MissPlayer.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "Effect_Manager.h"

#include "UI_Manager.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "UI_Indicator_InstKill.h"

#include "Bone.h"

HRESULT CTwince_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner,  ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CTwince_FSM> pInstance = static_pointer_cast<CTwince_FSM>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_Idle->Set(pInstance, &CTwince_FSM::BeginState_Idle, &CTwince_FSM::ExecuteState_Idle, &CTwince_FSM::EndState_Idle);
	m_pState_Wait = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_Wait->Set(pInstance, &CTwince_FSM::BeginState_Wait, &CTwince_FSM::ExecuteState_Wait, &CTwince_FSM::EndState_Wait);
	m_pState_Hide = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_Hide->Set(pInstance, &CTwince_FSM::BeginState_Hide, &CTwince_FSM::ExecuteState_Hide, &CTwince_FSM::EndState_Hide);
	m_pState_Down = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_Down->Set(pInstance, &CTwince_FSM::BeginState_Down, &CTwince_FSM::ExecuteState_Down, &CTwince_FSM::EndState_Down);
	m_pState_Beam = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_Beam->Set(pInstance, &CTwince_FSM::BeginState_Beam, &CTwince_FSM::ExecuteState_Beam, &CTwince_FSM::EndState_Beam);
	m_pState_Beam_OneWay = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_Beam_OneWay->Set(pInstance, &CTwince_FSM::BeginState_Beam_OneWay, &CTwince_FSM::ExecuteState_Beam_OneWay, &CTwince_FSM::EndState_Beam_OneWay);
	m_pState_OnHit_Front = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_OnHit_Front->Set(pInstance, &CTwince_FSM::BeginState_OnHit_Front, &CTwince_FSM::ExecuteState_OnHit_Front, &CTwince_FSM::EndState_OnHit_Front);
	m_pState_OnHit_Back = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_OnHit_Back->Set(pInstance, &CTwince_FSM::BeginState_OnHit_Back, &CTwince_FSM::ExecuteState_OnHit_Back, &CTwince_FSM::EndState_OnHit_Back);
	m_pState_OnHit_Psy_Front = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_OnHit_Psy_Front->Set(pInstance, &CTwince_FSM::BeginState_OnHit_Psy_Front, &CTwince_FSM::ExecuteState_OnHit_Psy_Front, &CTwince_FSM::EndState_OnHit_Psy_Front);
	m_pState_OnHit_Psy_Back = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_OnHit_Psy_Back->Set(pInstance, &CTwince_FSM::BeginState_OnHit_Psy_Back, &CTwince_FSM::ExecuteState_OnHit_Psy_Back, &CTwince_FSM::EndState_OnHit_Psy_Back);
	m_pState_OnHit_Psy_Left = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_OnHit_Psy_Left->Set(pInstance, &CTwince_FSM::BeginState_OnHit_Psy_Left, &CTwince_FSM::ExecuteState_OnHit_Psy_Left, &CTwince_FSM::EndState_OnHit_Psy_Left);
	m_pState_OnHit_Psy_Right = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_OnHit_Psy_Right->Set(pInstance, &CTwince_FSM::BeginState_OnHit_Psy_Right, &CTwince_FSM::ExecuteState_OnHit_Psy_Right, &CTwince_FSM::EndState_OnHit_Psy_Right);
	m_pState_AirBone = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_AirBone->Set(pInstance, &CTwince_FSM::BeginState_AirBone, &CTwince_FSM::ExecuteState_AirBone, &CTwince_FSM::EndState_AirBone);
	m_pState_AirBone_Falling = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_AirBone_Falling->Set(pInstance, &CTwince_FSM::BeginState_AirBone_Falling, &CTwince_FSM::ExecuteState_AirBone_Falling, &CTwince_FSM::EndState_AirBone_Falling);
	m_pState_Dead = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_Dead->Set(pInstance, &CTwince_FSM::BeginState_Dead, &CTwince_FSM::ExecuteState_Dead, &CTwince_FSM::EndState_Dead);
	m_pState_BCChance = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_BCChance->Set(pInstance, &CTwince_FSM::BeginState_BCChance, &CTwince_FSM::ExecuteState_BCChance, &CTwince_FSM::EndState_BCChance);
	m_pState_FinishMotion = make_shared<CStateTemplate<CTwince_FSM>>();
	m_pState_FinishMotion->Set(pInstance, &CTwince_FSM::BeginState_FinishMotion, &CTwince_FSM::ExecuteState_FinishMotion, &CTwince_FSM::EndState_FinishMotion);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_Twince>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CTwince_FSM::Initialize", "Failed to Get: Owner");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_pOwner.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTwince_FSM::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pPlayerTransform = dynamic_pointer_cast<CTransform>(Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pPlayerTransform)
	{
		MSG_RETURN(E_FAIL, "CTwince_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pModel = dynamic_pointer_cast<CModel>(m_pOwner.lock()->Get_Component(COMPONENT::MODEL));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CTwince_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pAnimator = dynamic_pointer_cast<CAnimator>(m_pOwner.lock()->Get_Behavior(BEHAVIOR::ANIMATOR));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CTwince_FSM::Initialize", "Failed to Get: Player:Transform");
	}
	
	if (nullptr == pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CTwince_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;
	
	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CTwince_FSM::Initialize", " pMonsterDesc is NULL");
	}
	m_pActorDesc = pActorDesc;
	
	SCENE Scene = pGameInstance->Current_Scene();
	if (Scene != SCENE::TEST)
	{
		shared_ptr<CObjectLayer> Layer = pGameInstance->Find_Layer(Scene, LAYER_TEAMMATE);
		if (nullptr != Layer)
		{
			shared_ptr<CGameObject> MaybeArashi = Layer->Get_GameObject(0);
			shared_ptr<CGameObject> MaybeShiDeng = Layer->Get_GameObject(1);
			m_pArashTransform = dynamic_pointer_cast<CTransform>(MaybeArashi->Get_Component(COMPONENT::TRANSFORM));
			m_pShidengTransform = dynamic_pointer_cast<CTransform>(MaybeShiDeng->Get_Component(COMPONENT::TRANSFORM));
		}
	}
	m_pTargetTransform = m_pPlayerTransform;

	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION5, std::bind(&CTwince_FSM::TentacleBeam, this));

#pragma region UI
	m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	m_pUI_NoticeMark->Initialize();

	m_pUI_MissPlayer = dynamic_pointer_cast<CUI_Indicator_Monster_MissPlayer>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_MISSPLAYER));
	m_pUI_MissPlayer->Initialize();

	m_pUI_InstKIll = dynamic_pointer_cast<CUI_Indicator_InstKill>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL));
	m_pUI_InstKIll->Initialize();
#pragma endregion

	return S_OK;
}

void CTwince_FSM::StateInitial(_float fTimeDelta)
{
	GotoState(m_pState_Idle);
}

void CTwince_FSM::ExecutePersistent(_float fTimeDelta)
{
	if (m_pCurrentState == m_pState_Dead)
		return;

	m_pUI_NoticeMark->Tick(fTimeDelta);
	m_pUI_NoticeMark->Set_IndicatorPosition(this->m_pTransform->Get_State(TRANSFORM::POSITION), this->m_pTransform->Get_WorldMatrix());
	m_pUI_NoticeMark->Late_Tick(fTimeDelta);

	m_pUI_MissPlayer->Tick(fTimeDelta);
	m_pUI_MissPlayer->Set_IndicatorPosition(this->m_pTransform->Get_WorldMatrix());
	m_pUI_MissPlayer->Late_Tick(fTimeDelta);

	m_pUI_InstKIll->Tick(fTimeDelta);
	m_pUI_InstKIll->Set_IndicatorPosition(this->m_pTransform->Get_WorldMatrix());
	m_pUI_InstKIll->Late_Tick(fTimeDelta);

	/*if (dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH && m_pCurrentState != m_pState_Idle)
	{
		m_pUI_MissPlayer->Call_Indicator();

		GotoState(m_pState_Idle);

		return;
	}*/
	

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

	if (m_pEntityDesc->fKnuckBackGauge <= 0.f && m_pCurrentState != m_pState_BCChance && m_pCurrentState != m_pState_AirBone && m_pCurrentState != m_pState_AirBone_Falling && m_pCurrentState != m_pState_FinishMotion)
	{
		m_pUI_InstKIll->Set_InstKillRenderStart();
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("UI_SFX_Kasane_InstantKill_Indicator.wav"), g_fPlayerSound);
		GotoState(m_pState_BCChance);
		return;
	}
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

		if (m_pCurrentState == m_pState_Hide)
		{
			CUI_Manager::Get_Instance()->Set_NoDamage(true);
			pGameInstance->PlaySoundW(Function::Random({ L"Monster_Mimic_Defence0.wav",L"Monster_Mimic_Defence1.wav",L"Monster_Mimic_Defence2.wav",L"Monster_Mimic_Defence3.wav" }), 1.f);
			return;
		}

		m_pEntityDesc->fHP -= m_pActorDesc->stHitDesc.iDamage;
		m_pEntityDesc->fKnuckBackGauge -= m_pActorDesc->stHitDesc.iDamage * ((rand() % 30) / 10.f);
		if (m_pCurrentState == m_pState_Down)
			return;
		pGameInstance->PlaySoundW(Function::Random({ L"Monster_Mimic_Damaged.wav",L"Monster_Mimic_Damaged1.wav",L"Monster_Mimic_Damaged2.wav",L"Monster_Mimic_Damaged3.wav" }), 3.f);

		if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::BLOW_START) || m_pAnimator->Is_Playing(ANIMATION::TWINCE::BLOW_LANDING))
		{
			m_pOwner.lock()->SetAirBoneTimeZero();
			GotoState(m_pState_AirBone);
			return;
		}

		if (!m_pActorDesc->bThrowHit)
		{
			if (90.f >= m_fDegree && -90.f <= m_fDegree)
				GotoState(m_pState_OnHit_Front);
			else
				GotoState(m_pState_Down);
		}
		return;
	} 
	auto pGameInstance = CGameInstance::Get_Instance();
	if (m_pActorDesc->bThrowHit)
	{
		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}
		if (m_pCurrentState == m_pState_Hide)
		{
			pGameInstance->PlaySoundW(Function::Random({ L"Monster_Mimic_Defence0.wav",L"Monster_Mimic_Defence1.wav",L"Monster_Mimic_Defence2.wav",L"Monster_Mimic_Defence3.wav" }), 1.f);
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
		m_pEntityDesc->fKnuckBackGauge -= m_pActorDesc->stHitDesc.iDamage * ((rand() % 20) / 10.f + 1.f);
		if (m_pCurrentState == m_pState_Down)
			return;
		pGameInstance->PlaySoundW(Function::Random({ L"Monster_Mimic_Damaged.wav",L"Monster_Mimic_Damaged1.wav",L"Monster_Mimic_Damaged2.wav",L"Monster_Mimic_Damaged3.wav" }), 3.f);
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
			GotoState(m_pState_Down);
		else if (135.f >= InteractiveDegree && 45 <= InteractiveDegree)
			GotoState(m_pState_OnHit_Psy_Right);
		else if (-45.f >= InteractiveDegree && -135.f <= InteractiveDegree)
			GotoState(m_pState_OnHit_Psy_Left);
		Kynesis->Set_RecentThrowObject(nullptr);
		return;
	}
	if (m_pActorDesc->bAirBone)
	{
		GotoState(m_pState_AirBone);
		return;
	}
}

void CTwince_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::IDLE, 2.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Idle" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	FindNewTarget();
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	if (m_fDistance <= 10.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	{
		GotoState(m_pState_Wait);
	}
}

void CTwince_FSM::EndState_Idle()
{
	m_pUI_NoticeMark->NoticePlayer();
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_Wait()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::IDLE, 2.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Wait" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_Wait(_float fTimeDelta)
{
	FindNewTarget();

	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	if (m_fDistance <= 10.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	{
		GotoState(m_pState_Hide);
	}
	else if (m_fCoolTime <= 0.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	{
		GotoState(Function::Random({ m_pState_Beam,m_pState_Beam_OneWay }));
	}
}

void CTwince_FSM::EndState_Wait()
{
	//m_pUI_NoticeMark->NoticePlayer();
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_Hide()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::BCCHANCE_START, 2.f, false, 0.2f, false);

	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Hide" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_Hide(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::BCCHANCE_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::BCCHANCE_START))
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::IDLE2, 2.f);
	}
	if (!m_pAnimator->Is_Playing(ANIMATION::TWINCE::WARP_END))
	{
		if (dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH)
		{
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::WARP_END, 2.f, false, 0.3f, false);
		}
		if (m_fDistance > 10.f)
		{
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::WARP_END, 2.f, false, 0.3f, false);
		}
	}
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::WARP_END))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::WARP_END))
		{
			CUI_Manager::Get_Instance()->Set_NoDamage(false);
			GotoState(m_pState_Wait);
		}
	}


}

void CTwince_FSM::EndState_Hide()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_Down()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DOWN_START, 2.f, false, 0.2f, false);

	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Twince_State_Down" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_Down(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DOWN_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DOWN_START))
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::DOWN, 2.f, false, 0.2f, false);
	}
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DOWN))
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::GETUP, 2.f, false, 0.2f, false);
	}
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::GETUP))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::GETUP))
			GotoState(m_pState_Wait);
	}
}

void CTwince_FSM::EndState_Down()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_Beam()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::ATTACK_A10_START_LEFT2, 2.f, false, 0.2f, false);
	m_pOwner.lock()->InitRootMotion();
	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({L"Monster_Mimic_WaterCannon.wav",L"Monster_Mimic_WaterCannon1.wav"}), 3.f);
	//이게 언제끝날지를 못하겠다.

#ifdef _DEBUG
	cout << "Twince_status_Beam" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_Beam(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);

	if (nullptr != m_pEffect)
		m_pEffect->Set_State(TRANSFORM::UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	//if (m_fDistance <= 10.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	//{
	//	GotoState(m_pState_Hide);
	//
	//	return;
	//}
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::ATTACK_A10_START_LEFT2))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::ATTACK_A10_START_LEFT2))
		{
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::ATTACK_A10_SWING_LR, 0.5f, false, 0.2f, false);
		}
	}
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::ATTACK_A10_SWING_LR))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::ATTACK_A10_SWING_LR))
		{
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::ATTACK_A10_SWING_RL, 0.5f, false, 0.2f, false);
		}
	}
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::ATTACK_A10_SWING_RL))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::ATTACK_A10_SWING_RL))
		{
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::ATTACK_A10_END_L, 2.f, false, 0.2f, false);
		}
	}
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::ATTACK_A10_END_L))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::ATTACK_A10_END_L))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Wait);
		}
	}
}

void CTwince_FSM::EndState_Beam()
{
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_Beam_OneWay()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::ATTACK_A4_END2, 2.f, false, 0.2f, false);

	//CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ L"Monster_Mimic_WaterCannon.wav",L"Monster_Mimic_WaterCannon1.wav" }), 3.f);
	m_pOwner.lock()->InitRootMotion();
	

#ifdef _DEBUG
	cout << "Twince_status_Beam" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_Beam_OneWay(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (nullptr != m_pEffect)
		m_pEffect->Set_State(TRANSFORM::UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	//if (m_fDistance <= 10.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	//{
	//	GotoState(m_pState_Hide);
	//	return;
	//}
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::ATTACK_A4_END2))
	{
		

		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::ATTACK_A4_END2))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Wait);
		}
	}
}

void CTwince_FSM::EndState_Beam_OneWay()
{
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_OnHit_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DAMAGE_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Demaged_Normal_Front" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_OnHit_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DAMAGE_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DAMAGE_FRONT))
			GotoState(m_pState_Hide);
	}
}

void CTwince_FSM::EndState_OnHit_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_OnHit_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DAMAGE_BACK, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Demaged_Normal_Back" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_OnHit_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DAMAGE_BACK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DAMAGE_BACK))
			GotoState(m_pState_Hide);
	}
}

void CTwince_FSM::EndState_OnHit_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_OnHit_Psy_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Demaged_Psyco_PSY_FRONT" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_OnHit_Psy_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Hide);
	}
}

void CTwince_FSM::EndState_OnHit_Psy_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_OnHit_Psy_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Demaged_Psyco_Back" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_OnHit_Psy_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Hide);
	}
}

void CTwince_FSM::EndState_OnHit_Psy_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_OnHit_Psy_Left()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DAMAGE_PSY_LEFT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Demaged_Psyco_Left" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_OnHit_Psy_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DAMAGE_PSY_LEFT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DAMAGE_PSY_LEFT))
			GotoState(m_pState_Hide);
	}
}

void CTwince_FSM::EndState_OnHit_Psy_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_OnHit_Psy_Right()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DAMAGE_PSY_RIGHT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Twince_status_Demaged_Psyco_Right" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_OnHit_Psy_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DAMAGE_PSY_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DAMAGE_PSY_RIGHT))
			GotoState(m_pState_Hide);
	}
}

void CTwince_FSM::EndState_OnHit_Psy_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_AirBone()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::BLOW_START, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_fTime = 0;
#ifdef _DEBUG
	cout << "Twince_status_AirBone" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_AirBone(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::BLOW_START))
	{
		//m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSetY(m_pTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pTargetTransform->Get_State(TRANSFORM::POSITION)) + 0.5f));

		if (m_pOwner.lock()->Height() <= 6.f) //이게나을지 다른게 나을지 확신이 업따
		{
			m_fTime += fTimeDelta;
			m_fHeight = (m_fTime - 1.f) * 5 + 6;
			m_pTransform->Translate((-m_pTransform->Get_State(TRANSFORM::LOOK) + _float4(0, m_fHeight, 0, 0)) * fTimeDelta * 2.f);
		}

		m_pOwner.lock()->AirBone(fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Get_TrackPosition(ANIMATION::TWINCE::BLOW_START) >= 20.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::BLOW_LANDING, 2.f, false, 0.5f, false);
			return;
		}
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::BLOW_LANDING))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
}

void CTwince_FSM::EndState_AirBone()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_AirBone_Falling()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DOWN_SHOCK, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Twince_status_AirBone_Falling" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_AirBone_Falling(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DOWN_SHOCK))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DOWN_SHOCK))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::GETUP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::GETUP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::GETUP))
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_Hide);
		}
	}
}

void CTwince_FSM::EndState_AirBone_Falling()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_Dead()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::DEAD_DOWN, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Dissolve();
	m_pOwner.lock()->Set_LockOnAble(false);
	m_pOwner.lock()->SetWeakRender(false);
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->ReleaseController();
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
#ifdef _DEBUG
	cout << "Twince_status_Death" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	//m_pOwner.lock()->Update_Motion(fTimeDelta);
	//m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::DEAD_DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::DEAD_DOWN))
		{
			//m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
			//m_pOwner.lock()->No_RootMotion(fTimeDelta);
			m_pOwner.lock()->SetDead();
			m_pAnimator->Set_Default(ANIMATION::TWINCE::DOWN);
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::DOWN);
		}
	}
}

void CTwince_FSM::EndState_Dead()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_BCChance()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::BCCHANCE_START, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->SetWeakRender(true);
	m_pOwner.lock()->Set_BcChance(true);
#ifdef _DEBUG
	cout << "Twince_status_BCChance" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_BCChance(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::TWINCE::BCCHANCE_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::TWINCE::BCCHANCE_START))
		{
			m_pAnimator->Play_Animation(ANIMATION::TWINCE::BCCHANCE_LOOP, 2.f);
		}
	}
}

void CTwince_FSM::EndState_BCChance()
{
	//m_pOwner.lock()->SetWeakRender(false);
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::BeginState_FinishMotion()
{
	m_pAnimator->Play_Animation(ANIMATION::TWINCE::BCCHANCE_LOOP, 2.f);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->SetWeakRender(true);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
	m_pTransform->LookAt(m_pTargetTransform);
	m_pOwner.lock()->Set_LockOnAble(false);
#ifdef _DEBUG
	cout << "Twince_status_BCChance" << endl;
#endif // _DEBUG
}

void CTwince_FSM::ExecuteState_FinishMotion(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(0);
	_float Height = m_pOwner.lock()->Height();
	if (Height < 5.f)
	{
		m_pTransform->Translate(XMVectorSet(0.f, 1.f, 0.f, 0.f) * fTimeDelta * 3.5f );
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
	}
	else  
		GotoState(m_pState_Dead);
}

void CTwince_FSM::EndState_FinishMotion()
{
	m_pOwner.lock()->MotionReset();
}

void CTwince_FSM::FindNewTarget()
{
	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::TEST || CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST)
	{
		m_pTargetTransform = m_pPlayerTransform;
		return;
	}
	if(!m_pArashTransform)
	{
		m_pTargetTransform = m_pPlayerTransform;
		return;
	}
	_float DistanceTarget = Function::Distance(m_pTransform, m_pTargetTransform);
	_float DistancePlayer = Function::Distance(m_pTransform, m_pPlayerTransform);
	_float DistanceArash = Function::Distance(m_pTransform, m_pArashTransform);
	_float DistanceShideng = Function::Distance(m_pTransform, m_pShidengTransform);

	if (DistanceTarget > DistancePlayer)
	{
		m_pTargetTransform = m_pPlayerTransform;
		DistanceTarget = Function::Distance(m_pTransform, m_pTargetTransform);
	}
	if (DistanceTarget > DistanceArash)
	{
		m_pTargetTransform = m_pArashTransform;
		DistanceTarget = Function::Distance(m_pTransform, m_pTargetTransform);
	}
	if (DistanceTarget > DistanceShideng)
	{
		m_pTargetTransform = m_pShidengTransform;
		DistanceTarget = Function::Distance(m_pTransform, m_pTargetTransform);
	}
}

void CTwince_FSM::GotoFinish()
{
	m_pUI_InstKIll->Set_InstKillRenderEnd();

	GotoState(m_pState_FinishMotion);
}

void CTwince_FSM::TentacleBeam()
{
	m_pEffect = dynamic_pointer_cast<CTransform>(VFXMGR->Fetch(VFX::EM910_WATERBEAM, make_pair(static_pointer_cast<CGameObject>(m_pOwner.lock()), "HeadTenctacleB9"))->Get_Component(COMPONENT::TRANSFORM));
}

shared_ptr<CTwince_FSM> CTwince_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	shared_ptr<CTwince_FSM> pInstance = make_private_shared(CTwince_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CTwince_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
