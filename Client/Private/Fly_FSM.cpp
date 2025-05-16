#include "ClientPCH.h"
#include "Fly_FSM.h"
#include "Bone.h"
#include "GameInstance.h"
#include "Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "Camera_Player.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Effect_Manager.h"

#include "UI_Manager.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "UI_Indicator_InstKill.h"

HRESULT CFly_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CFly_FSM> pInstance = static_pointer_cast<CFly_FSM>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_Idle->Set(pInstance, &CFly_FSM::BeginState_Idle, &CFly_FSM::ExecuteState_Idle, &CFly_FSM::EndState_Idle);
	m_pState_Walk = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_Walk->Set(pInstance, &CFly_FSM::BeginState_Walk, &CFly_FSM::ExecuteState_Walk, &CFly_FSM::EndState_Walk);
	m_pState_Escape = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_Escape->Set(pInstance, &CFly_FSM::BeginState_Escape, &CFly_FSM::ExecuteState_Escape, &CFly_FSM::EndState_Escape);
	//m_pState_Run = make_shared<CStateTemplate<CFly_FSM>>();
	//m_pState_Run->Set(pInstance, &CFly_FSM::BeginState_Run, &CFly_FSM::ExecuteState_Run, &CFly_FSM::EndState_Run);
	m_pState_BodySlam = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_BodySlam->Set(pInstance, &CFly_FSM::BeginState_BodySlam, &CFly_FSM::ExecuteState_BodySlam, &CFly_FSM::EndState_BodySlam);
	m_pState_Shoot = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_Shoot->Set(pInstance, &CFly_FSM::BeginState_Shoot, &CFly_FSM::ExecuteState_Shoot, &CFly_FSM::EndState_Shoot);
	m_pState_OnHit_Front = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_OnHit_Front->Set(pInstance, &CFly_FSM::BeginState_OnHit_Front, &CFly_FSM::ExecuteState_OnHit_Front, &CFly_FSM::EndState_OnHit_Front);
	m_pState_OnHit_Back = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_OnHit_Back->Set(pInstance, &CFly_FSM::BeginState_OnHit_Back, &CFly_FSM::ExecuteState_OnHit_Back, &CFly_FSM::EndState_OnHit_Back);
	m_pState_OnHit_Psy_Front = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_OnHit_Psy_Front->Set(pInstance, &CFly_FSM::BeginState_OnHit_Psy_Front, &CFly_FSM::ExecuteState_OnHit_Psy_Front, &CFly_FSM::EndState_OnHit_Psy_Front);
	m_pState_OnHit_Psy_Back = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_OnHit_Psy_Back->Set(pInstance, &CFly_FSM::BeginState_OnHit_Psy_Back, &CFly_FSM::ExecuteState_OnHit_Psy_Back, &CFly_FSM::EndState_OnHit_Psy_Back);
	m_pState_OnHit_Psy_Left = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_OnHit_Psy_Left->Set(pInstance, &CFly_FSM::BeginState_OnHit_Psy_Left, &CFly_FSM::ExecuteState_OnHit_Psy_Left, &CFly_FSM::EndState_OnHit_Psy_Left);
	m_pState_OnHit_Psy_Right = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_OnHit_Psy_Right->Set(pInstance, &CFly_FSM::BeginState_OnHit_Psy_Right, &CFly_FSM::ExecuteState_OnHit_Psy_Right, &CFly_FSM::EndState_OnHit_Psy_Right);
	m_pState_AirBone = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_AirBone->Set(pInstance, &CFly_FSM::BeginState_AirBone, &CFly_FSM::ExecuteState_AirBone, &CFly_FSM::EndState_AirBone);
	m_pState_AirBone_Falling = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_AirBone_Falling->Set(pInstance, &CFly_FSM::BeginState_AirBone_Falling, &CFly_FSM::ExecuteState_AirBone_Falling, &CFly_FSM::EndState_AirBone_Falling);
	m_pState_Dead = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_Dead->Set(pInstance, &CFly_FSM::BeginState_Dead, &CFly_FSM::ExecuteState_Dead, &CFly_FSM::EndState_Dead);
	m_pState_BCChance = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_BCChance->Set(pInstance, &CFly_FSM::BeginState_BCChance, &CFly_FSM::ExecuteState_BCChance, &CFly_FSM::EndState_BCChance);
	m_pState_FinishMotion = make_shared<CStateTemplate<CFly_FSM>>();
	m_pState_FinishMotion->Set(pInstance, &CFly_FSM::BeginState_FinishMotion, &CFly_FSM::ExecuteState_FinishMotion, &CFly_FSM::EndState_FinishMotion);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_Fly>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CFly_FSM::Initialize", "Failed to Get: Owner");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_pOwner.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CFly_FSM::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pPlayerTransform = dynamic_pointer_cast<CTransform>(Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pPlayerTransform)
	{
		MSG_RETURN(E_FAIL, "CFly_FSM::Initialize", "Failed to Get: Player:Transform");
	}
	m_pTargetTransform = m_pPlayerTransform;

	m_pModel = dynamic_pointer_cast<CModel>(m_pOwner.lock()->Get_Component(COMPONENT::MODEL));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CFly_FSM::Initialize", "Failed to Get: Player:MODEL");
	}

	m_pAnimator = dynamic_pointer_cast<CAnimator>(m_pOwner.lock()->Get_Behavior(BEHAVIOR::ANIMATOR));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CFly_FSM::Initialize", "Failed to Get: Player:ANIMATOR");
	}
	
	if (nullptr == pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CFly_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;
	
	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CFly_FSM::Initialize", " pMonsterDesc is NULL");
	}
	m_pActorDesc = pActorDesc;
	//GotoState(m_pState_Idle);
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
	FindNewTarget();
#pragma region UI
	m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	m_pUI_NoticeMark->Initialize();
	m_pUI_InstKIll = dynamic_pointer_cast<CUI_Indicator_InstKill>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL));
	m_pUI_InstKIll->Initialize();

#pragma endregion

	return S_OK;
}

void CFly_FSM::StateInitial(_float fTimeDelta)
{
	GotoState(m_pState_Idle);
}

void CFly_FSM::ExecutePersistent(_float fTimeDelta)
{
	if (m_pCurrentState == m_pState_Dead)
		return;

	m_pUI_NoticeMark->Tick(fTimeDelta);
	m_pUI_NoticeMark->Set_IndicatorPosition(this->m_pTransform->Get_State(TRANSFORM::POSITION), this->m_pTransform->Get_WorldMatrix());
	m_pUI_NoticeMark->Late_Tick(fTimeDelta);

	m_pUI_InstKIll->Tick(fTimeDelta);
	m_pUI_InstKIll->Set_IndicatorPosition(this->m_pTransform->Get_WorldMatrix());
	m_pUI_InstKIll->Late_Tick(fTimeDelta);

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

	_float Radian = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
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
	/*if (m_pEntityDesc->fJumpPower > 0.f)
	{
		GotoState(m_pState_Escape);
		return;
	}*/
	if (m_pActorDesc->bCapture && CGameInstance::Get_Instance()->Current_Scene() == SCENE::MIZUHA &&
		!(dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::ACCELERATION || dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH)
		&& m_pCurrentState != m_pState_FinishMotion
		)
	{
		//if (m_pActorDesc->bHit)
		//	return;
		auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
		auto Interactive = Kynesis->Get_RecentThrowdObject();
		if (Interactive == nullptr)
			m_pTransform->LookAt(m_pTargetTransform);
		else
		{
			shared_ptr<CTransform> InterActiveTransform = dynamic_pointer_cast<CTransform>(Interactive->Get_Component(COMPONENT::TRANSFORM));
			m_pTransform->LookAt(InterActiveTransform);
		}
		GotoState(m_pState_Escape);
		//m_pActorDesc->bCapture = false;
		return;
	}
	if (m_pActorDesc->bHit/* && *m_pActorDesc->stHitDesc.AttackColliderActive*/)
	{
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
		m_pEntityDesc->fKnuckBackGauge -= m_pActorDesc->stHitDesc.iDamage * ((rand() % 30) / 10.f);

		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}
		if (m_pAnimator->Is_Playing(ANIMATION::FLY::RISE_START) || m_pAnimator->Is_Playing(ANIMATION::FLY::FALL_START))
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
				GotoState(m_pState_OnHit_Back);
		}
		return;
	}
	if (m_pActorDesc->bThrowHit)
	{
		/*
		if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::MIZUHA &&
			!(dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::ACCELERATION || dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH)
			)
		{
			m_pTransform->LookAt(m_pTargetTransform);
			GotoState(m_pState_Escape);
			return;
		}
		*/
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
		m_pEntityDesc->fKnuckBackGauge -= m_pActorDesc->stHitDesc.iDamage * ((rand() % 30) / 10.f);

		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}
		shared_ptr<CTransform> InterActiveTransform = dynamic_pointer_cast<CTransform>(Interactive->Get_Component(COMPONENT::TRANSFORM));

		_vector Direction = InterActiveTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);

		_float Radian = (_float)acos(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
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

		return;
	}

	if (m_pActorDesc->bAirBone)
	{
		GotoState(m_pState_AirBone);
		return;
	}
}

void CFly_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::FLY_IDLE, 3.f);
	m_pOwner.lock()->InitRootMotion();
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));

#ifdef _DEBUG
	cout << "Fly_status_Idle" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	FindNewTarget();
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_fDistance <= 5.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	{
		GotoState(m_pState_Walk);
	}
}

void CFly_FSM::EndState_Idle()
{
	m_pUI_NoticeMark->NoticePlayer();

	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_Walk()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::FLY::WALK_FRONT, 2.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Fly_status_Walk_Forward" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_Walk(_float fTimeDelta)
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
		GotoState(Function::Random({ m_pState_BodySlam,m_pState_Shoot }));
	}
	//else if (m_fDistance <= 5.f)
	//	GotoState(m_pState_Escape);
}

void CFly_FSM::EndState_Walk()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_Escape()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::FLY::FLY_ESCAPE, 3.f,false,g_fDefaultInterpolationDuration,false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Set_LockOnAble(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
	//m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK)  * -1.f);

	auto pPlayer = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
	if (pPlayer->Get_PlayerDesc().eSAS != PLAYER_SAS::ACCELERATION)
		CEffect_Manager::Get_Instance()->Fetch(VFX::AFTERIMAGE, dynamic_pointer_cast<CGameObject>(m_pOwner.lock()));

#ifdef _DEBUG
	cout << "Fly_status_Escape" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_Escape(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * -3.f);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	//CEffect_Manager::Get_Instance()->Fetch(VFX::AFTERIMAGE, m_pOwner.lock());
	
	//CGameInstance::Get_Instance()->Set_LUTIndex(2);
	//CGameInstance::Get_Instance()->MaskingLUT(true, m_pCircularMask);
	
	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);
	
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::FLY_ESCAPE))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::FLY_ESCAPE))
			GotoState(m_pState_Walk);
	}
}

void CFly_FSM::EndState_Escape()
{
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->Set_LockOnAble(true);
}

void CFly_FSM::BeginState_BodySlam()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::FLY::ATTACK_BODYSLAM_START, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Fly_status_BodySlam" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_BodySlam(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::ATTACK_BODYSLAM_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::ATTACK_BODYSLAM_START))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::FLY::ATTACK_BODYSLAM_END, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLY::ATTACK_BODYSLAM_END))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::ATTACK_BODYSLAM_END))
		{
			m_pOwner.lock()->MotionReset();
			m_pTransform->LookAt(m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK));
			m_pAnimator->Play_Animation(ANIMATION::FLY::TAKEOFF, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLY::TAKEOFF))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::TAKEOFF))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}
}

void CFly_FSM::EndState_BodySlam()
{
	FindNewTarget();
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_Shoot()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Spine"));
	m_pAnimator->Play_Animation(ANIMATION::FLY::ATTACK2, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Fly_status_Walk_Forward" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_Shoot(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	/*if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);*/

	//m_pTransform->LookAt(m_pTargetTransform);
	
	m_pOwner.lock()->RootMotion(fTimeDelta, true);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::ATTACK2))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::ATTACK2))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}

}

void CFly_FSM::EndState_Shoot()
{
	FindNewTarget();
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_OnHit_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::DAMAGE_FRONT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Front" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_OnHit_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::DAMAGE_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DAMAGE_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CFly_FSM::EndState_OnHit_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_OnHit_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::DAMAGE_BACK, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Back" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_OnHit_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::DAMAGE_BACK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DAMAGE_BACK))
			GotoState(m_pState_Walk);
	}
}

void CFly_FSM::EndState_OnHit_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_OnHit_Psy_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_PSY_FRONT" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_OnHit_Psy_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CFly_FSM::EndState_OnHit_Psy_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_OnHit_Psy_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Back" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_OnHit_Psy_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CFly_FSM::EndState_OnHit_Psy_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_OnHit_Psy_Left()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::DAMAGE_PSY_LEFT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Left" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_OnHit_Psy_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::DAMAGE_PSY_LEFT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DAMAGE_PSY_LEFT))
			GotoState(m_pState_Walk);
	}
}

void CFly_FSM::EndState_OnHit_Psy_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_OnHit_Psy_Right()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::DAMAGE_PSY_RIGHT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Right" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_OnHit_Psy_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::DAMAGE_PSY_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DAMAGE_PSY_RIGHT))
			GotoState(m_pState_Walk);
	}

}

void CFly_FSM::EndState_OnHit_Psy_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_AirBone()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::RISE_START, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Fly_status_AirBone" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_AirBone(_float fTimeDelta)
{
	m_pOwner.lock()->CMonster::Update_Motion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::FLY::RISE_START))
	{
		//m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSetY(m_pTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pTargetTransform->Get_State(TRANSFORM::POSITION)) + 1.f));

		if (m_pOwner.lock()->Height() <= 4.f)
			m_pTransform->Translate((-m_pTransform->Get_State(TRANSFORM::LOOK) + _float4(0, 7, 0, 0)) * fTimeDelta * 2.f);

		m_pOwner.lock()->AirBone(fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Get_TrackPosition(ANIMATION::FLY::RISE_START) >= 20.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::FLY::FALL_START, 2.f, false, 0.5f, false);
		}
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLY::FALL_START))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
}

void CFly_FSM::EndState_AirBone()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_AirBone_Falling()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::DOWN_SHOCK, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Fly_status_AirBone_Falling" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_AirBone_Falling(_float fTimeDelta)
{
	m_pOwner.lock()->CMonster::Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::DOWN_SHOCK))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DOWN_SHOCK))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::FLY::GETUP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLY::GETUP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::GETUP))
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_Walk);
		}
	}
}

void CFly_FSM::EndState_AirBone_Falling()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_Dead()
{
	m_pAnimator->Play_Animation(ANIMATION::FLY::DEAD_DOWN, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->Dissolve();
	m_pOwner.lock()->DeadSlow();
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->ReleaseController();
	m_pOwner.lock()->Set_LockOnAble(false);
	CGameInstance::Get_Instance()->PlaySoundW(L"Monster_Fly_Dead.wav", 1.f);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
#ifdef _DEBUG
	cout << "Fly_status_Death" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	//m_pOwner.lock()->Update_Motion(fTimeDelta);
	//m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::DEAD_DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DEAD_DOWN))
		{
			m_pOwner.lock()->SetDead();
			m_pAnimator->Set_Default(ANIMATION::FLY::DOWN);
			m_pAnimator->Play_Animation(ANIMATION::FLY::DOWN);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLY::DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::DOWN))
		 {
			//nothing
		}
	}
}

void CFly_FSM::EndState_Dead()
{
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_BCChance()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::BC_CHANCE_START, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Set_BcChance(true);
	//m_pOwner.lock()->SetWeakRender(true);
#ifdef _DEBUG
	cout << "FLY_status_BCChance" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_BCChance(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::BC_CHANCE_START))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::BC_CHANCE_START))
		{
			m_pAnimator->Play_Animation(ANIMATION::SNAIL::BC_CHANCE_LOOP, 2.f);
		}
	}
	else
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
}

void CFly_FSM::EndState_BCChance()
{
	m_pOwner.lock()->Set_BcChance(false);
	m_pOwner.lock()->MotionReset();
}

void CFly_FSM::BeginState_FinishMotion()
{
	m_pOwner.lock()->Get_UI_HP()->Set_MonsterNameTagRemove_Execution();

	m_pAnimator->Play_Animation(ANIMATION::FLY::FINISH, 2.f, false, 0.f, false);
	m_pModel->Set_Pivot(m_pModel->Get_Pivot() * XMMatrixRotationY(XMConvertToRadians(180.f)));
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	//m_pOwner.lock()->SetWeakRender(true);
	m_pOwner.lock()->Set_LockOnAble(false);
	m_pOwner.lock()->MoveRootPos();
	//m_pOwner.lock()->ReleaseController();
	m_fTime = 0.f;
#ifdef _DEBUG
	cout << "Fly_status_Finish" << endl;
#endif // _DEBUG
}

void CFly_FSM::ExecuteState_FinishMotion(_float fTimeDelta)
{

	CPlayer* pPlayer = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
	shared_ptr<CTransform> pPlayerTransform = static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Component(COMPONENT::TRANSFORM));
	shared_ptr<CTransform> pPlayerCameraTransform = static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera()->Get_Component(COMPONENT::TRANSFORM));

	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_fTime += fTimeDelta;
	//if(!m_bExecuted[0] && m_bExecuted[1])
	//{
	//	m_bExecuted[0] = true;


	//	m_pTransform->LookAt(pPlayerTransform);
	//	pPlayerTransform->LookAt(m_pTransform);
	//	
	//	
	//	if (pPlayer->Get_Camera()->Get_LockOnTransform() != nullptr)
	//	{
	//		pPlayerCameraTransform->LookAtDir(pPlayerTransform->Get_State(TRANSFORM::LOOK));
	//		pPlayerCameraTransform->Set_State(TRANSFORM::POSITION, pPlayerTransform->Get_State(TRANSFORM::POSITION)
	//			- pPlayerTransform->Get_State(TRANSFORM::LOOK) * 1.5 +
	//			+pPlayerTransform->Get_State(TRANSFORM::RIGHT) * 0.8f + _float4(0, 1.7f, 0, 0));

	//		pPlayerCameraTransform->Rotate(TRANSFORM::UP, XMConvertToRadians(-30.f));
	//	}
	//}
	//else
		m_pOwner.lock()->RootMotionRotate(fTimeDelta);

	if (m_fTime < 1.f)
		m_pTransform->LookAt(static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Component(COMPONENT::TRANSFORM)));
	

	//m_bExecuted[1] = true;
	if (m_pAnimator->Is_Playing(ANIMATION::FLY::FINISH))
	{
		if (_int(m_pAnimator->Get_TrackPosition(ANIMATION::FLY::FINISH)) == 275 || _int(m_pAnimator->Get_TrackPosition(ANIMATION::FLY::FINISH)) == 276)
			m_pOwner.lock()->Dissolve();
		if (m_pAnimator->Is_Finished(ANIMATION::FLY::FINISH))
		{
			m_pOwner.lock()->SetDead();

			m_pOwner.lock()->InitRootMotion();
			m_pOwner.lock()->ReleaseController();
			m_pOwner.lock()->Set_LockOnAble(false);
			m_pOwner.lock()->SetWeaponeActiveOff();
			static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
			static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
		}
	}
}

void CFly_FSM::EndState_FinishMotion()
{
	m_pModel->Set_Pivot(m_pModel->Get_Pivot() * XMMatrixRotationY(XMConvertToRadians(180.f)));
	m_pOwner.lock()->MotionReset();
	m_bExecuted[0] = false;
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
}

void CFly_FSM::FindNewTarget()
{
	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::TEST)
	{
		m_pTargetTransform = m_pPlayerTransform;
		return;
	}
	if (!m_pArashTransform || !m_pShidengTransform)
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

shared_ptr<CFly_FSM> CFly_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	shared_ptr<CFly_FSM> pInstance = make_private_shared(CFly_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CFly_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}

void CFly_FSM::GotoFinish()
{
	m_pUI_InstKIll->Set_InstKillRenderEnd();
	GotoState(m_pState_FinishMotion);
}