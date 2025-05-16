 #include "ClientPCH.h"
#include "FlowerChest_FSM.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"

HRESULT CFlowerChest_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner,  ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CFlowerChest_FSM> pInstance = static_pointer_cast<CFlowerChest_FSM>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_Idle->Set(pInstance, &CFlowerChest_FSM::BeginState_Idle, &CFlowerChest_FSM::ExecuteState_Idle, &CFlowerChest_FSM::EndState_Idle);
	m_pState_Walk = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_Walk->Set(pInstance, &CFlowerChest_FSM::BeginState_Walk, &CFlowerChest_FSM::ExecuteState_Walk, &CFlowerChest_FSM::EndState_Walk);
	m_pState_Run = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_Run->Set(pInstance, &CFlowerChest_FSM::BeginState_Run, &CFlowerChest_FSM::ExecuteState_Run, &CFlowerChest_FSM::EndState_Run);
	m_pState_Attack = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_Attack->Set(pInstance, &CFlowerChest_FSM::BeginState_Attack, &CFlowerChest_FSM::ExecuteState_Attack, &CFlowerChest_FSM::EndState_Attack);
	m_pState_OnHit_Front = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_OnHit_Front->Set(pInstance, &CFlowerChest_FSM::BeginState_OnHit_Front, &CFlowerChest_FSM::ExecuteState_OnHit_Front, &CFlowerChest_FSM::EndState_OnHit_Front);
	m_pState_OnHit_Back = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_OnHit_Back->Set(pInstance, &CFlowerChest_FSM::BeginState_OnHit_Back, &CFlowerChest_FSM::ExecuteState_OnHit_Back, &CFlowerChest_FSM::EndState_OnHit_Back);
	m_pState_OnHit_Psy_Front = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_OnHit_Psy_Front->Set(pInstance, &CFlowerChest_FSM::BeginState_OnHit_Psy_Front, &CFlowerChest_FSM::ExecuteState_OnHit_Psy_Front, &CFlowerChest_FSM::EndState_OnHit_Psy_Front);
	m_pState_OnHit_Psy_Back = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_OnHit_Psy_Back->Set(pInstance, &CFlowerChest_FSM::BeginState_OnHit_Psy_Back, &CFlowerChest_FSM::ExecuteState_OnHit_Psy_Back, &CFlowerChest_FSM::EndState_OnHit_Psy_Back);
	m_pState_OnHit_Psy_Left = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_OnHit_Psy_Left->Set(pInstance, &CFlowerChest_FSM::BeginState_OnHit_Psy_Left, &CFlowerChest_FSM::ExecuteState_OnHit_Psy_Left, &CFlowerChest_FSM::EndState_OnHit_Psy_Left);
	m_pState_OnHit_Psy_Right = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_OnHit_Psy_Right->Set(pInstance, &CFlowerChest_FSM::BeginState_OnHit_Psy_Right, &CFlowerChest_FSM::ExecuteState_OnHit_Psy_Right, &CFlowerChest_FSM::EndState_OnHit_Psy_Right);
	m_pState_AirBone = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_AirBone->Set(pInstance, &CFlowerChest_FSM::BeginState_AirBone, &CFlowerChest_FSM::ExecuteState_AirBone, &CFlowerChest_FSM::EndState_AirBone);
	m_pState_AirBone_Falling = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_AirBone_Falling->Set(pInstance, &CFlowerChest_FSM::BeginState_AirBone_Falling, &CFlowerChest_FSM::ExecuteState_AirBone_Falling, &CFlowerChest_FSM::EndState_AirBone_Falling);
	m_pState_Dead = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_Dead->Set(pInstance, &CFlowerChest_FSM::BeginState_Dead, &CFlowerChest_FSM::ExecuteState_Dead, &CFlowerChest_FSM::EndState_Dead);
	m_pState_BCChance = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_BCChance->Set(pInstance, &CFlowerChest_FSM::BeginState_BCChance, &CFlowerChest_FSM::ExecuteState_BCChance, &CFlowerChest_FSM::EndState_BCChance);
	m_pState_FinishMotion = make_shared<CStateTemplate<CFlowerChest_FSM>>();
	m_pState_FinishMotion->Set(pInstance, &CFlowerChest_FSM::BeginState_FinishMotion, &CFlowerChest_FSM::ExecuteState_FinishMotion, &CFlowerChest_FSM::EndState_FinishMotion);

	//auto pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_FlowerChest>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_FSM::Initialize", "Failed to Get: Owner");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_pOwner.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_FSM::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pTargetTransform = dynamic_pointer_cast<CTransform>(Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pModel = dynamic_pointer_cast<CModel>(Function::Find_Player()->Get_Component(COMPONENT::MODEL));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pAnimator = dynamic_pointer_cast<CAnimator>(m_pOwner.lock()->Get_Behavior(BEHAVIOR::ANIMATOR));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_FSM::Initialize", "Failed to Get: Player:Transform");
	}
	
	if (nullptr == pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;
	
	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_FSM::Initialize", " pMonsterDesc is NULL");
	}
	m_pActorDesc = pActorDesc;

	//cout << m_pMonsterDesc << endl;

#pragma region UI
	m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	m_pUI_NoticeMark->Initialize();
#pragma endregion

	return S_OK;
}

void CFlowerChest_FSM::StateInitial(_float fTimeDelta)
{
	GotoState(m_pState_Idle);
}

void CFlowerChest_FSM::ExecutePersistent(_float fTimeDelta)
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
	_vector Direction = m_pTargetTransform->Get_State(TRANSFORM::POSITION) - XMVectorSetY(m_pTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pTargetTransform->Get_State(TRANSFORM::POSITION)));

	_float Radian = (_float)acos(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
	_vector Cross = XMVector3Cross(myLook, Direction);

	if (XMVectorGetY(Cross) <= 0.f)
		Radian *= -1;

	m_fDegree = XMConvertToDegrees(Radian);

	if (m_pEntityDesc->fKnuckBackGauge <= 0.f && m_pCurrentState != m_pState_BCChance && m_pCurrentState != m_pState_AirBone && m_pCurrentState != m_pState_AirBone_Falling && m_pCurrentState != m_pState_FinishMotion)
	{
		GotoState(m_pState_BCChance);
		return;
	}
	if (m_pActorDesc->bHit)
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
		if(nullptr==pPlayer->Get_Camera()->Get_LockOnTransform())
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
		pGameInstance->PlaySoundW(Function::Random({ L"Monster_FlowerChest_Damaged00.wav",L"Monster_FlowerChest_Damaged01.wav" }), 1.f);
		if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::AIRBONE) || m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::FALLING))
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
		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}
		pGameInstance->PlaySoundW(Function::Random({ L"Monster_FlowerChest_Damaged00.wav",L"Monster_FlowerChest_Damaged01.wav" }), 1.f);

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
		return;
	}
	if (m_pActorDesc->bAirBone)
	{
		GotoState(m_pState_AirBone);
		return;
	}
}

void CFlowerChest_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::IDLE, 2.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Idle" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->No_RootMotion(fTimeDelta);
	if (m_fDistance <= 5.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	{
		GotoState(m_pState_Walk);
	}
}

void CFlowerChest_FSM::EndState_Idle()
{
	m_pUI_NoticeMark->NoticePlayer();

	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_Walk()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::WALK, 2.f, false, 0.3f);

	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "FC_status_Walk" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_Walk(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if(m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

	//m_pTransform->LookAt(m_pTargetTransform);
	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta);
	
	m_pOwner.lock()->RootMotion(fTimeDelta);
	//m_pOwner.lock()->RootMotion(fTimeDelta, false);
	if (m_fCoolTime <= 0 && (m_fDegree <= 5.f && m_fDegree >= -5.f))
	{
		if (m_fDistance >= 10.f)
		{
			GotoState(m_pState_Run);
		}
		else if (m_fDistance <= 3.f)
		{
			GotoState(m_pState_Attack);
		}
	}
}

void CFlowerChest_FSM::EndState_Walk()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_Run()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::RUN, 2.f, false, 1.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Run" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_Run(_float fTimeDelta)
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

void CFlowerChest_FSM::EndState_Run()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_Attack()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::ATTACK1, 2.f, false, 1.f, false);

	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Attack" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_Attack(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	_float TrackPosition = m_pAnimator->Get_TrackPosition(ANIMATION::FLOWER_CHEST::ATTACK1);
	if (TrackPosition <= 170.f)
	{
		if (m_fDegree >= 5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
		else if (m_fDegree <= -5.f)
			m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);
	}
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::ATTACK1))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::ATTACK1))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}
}

void CFlowerChest_FSM::EndState_Attack()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_OnHit_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DAMAGE_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Front" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_OnHit_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::DAMAGE_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::DAMAGE_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CFlowerChest_FSM::EndState_OnHit_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_OnHit_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DAMAGE_BACK, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Back" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_OnHit_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::DAMAGE_BACK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::DAMAGE_BACK))
			GotoState(m_pState_Walk);
	}
}

void CFlowerChest_FSM::EndState_OnHit_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_OnHit_Psy_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_PSY_FRONT" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_OnHit_Psy_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CFlowerChest_FSM::EndState_OnHit_Psy_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_OnHit_Psy_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Back" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_OnHit_Psy_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CFlowerChest_FSM::EndState_OnHit_Psy_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_OnHit_Psy_Left()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_LEFT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Left" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_OnHit_Psy_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_LEFT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_LEFT))
			GotoState(m_pState_Walk);
	}
}

void CFlowerChest_FSM::EndState_OnHit_Psy_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_OnHit_Psy_Right()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_RIGHT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Right" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_OnHit_Psy_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::DAMAGE_PSY_RIGHT))
			GotoState(m_pState_Walk);
	}
}

void CFlowerChest_FSM::EndState_OnHit_Psy_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_AirBone()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::AIRBONE, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_fTime = 0;
#ifdef _DEBUG
	cout << "FC_status_AirBone" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_AirBone(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::AIRBONE))
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
		if (m_pAnimator->Get_TrackPosition(ANIMATION::FLOWER_CHEST::AIRBONE) >= 20.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::FALLING, 2.f, false, 0.5f, false);
			return;
		}
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::FALLING))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
}

void CFlowerChest_FSM::EndState_AirBone()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_AirBone_Falling()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DOWN_SHOCK, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "FC_status_AirBone_Falling" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_AirBone_Falling(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::DOWN_SHOCK))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::DOWN_SHOCK))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::GET_UP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::GET_UP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::GET_UP))
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_Walk);
		}
	}
}

void CFlowerChest_FSM::EndState_AirBone_Falling()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_Dead()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DEAD_DOWN, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Dissolve();
	m_pOwner.lock()->DeadSlow();
	m_pOwner.lock()->Set_LockOnAble(false);
	m_pOwner.lock()->ReleaseController(); 
	m_pOwner.lock()->SetWeaponeActiveOff();
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
#ifdef _DEBUG
	cout << "FC_status_Death" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	//m_pOwner.lock()->Update_Motion(fTimeDelta);
	//m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::DEAD_DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::DEAD_DOWN))
		{
			//m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
			//m_pOwner.lock()->No_RootMotion(fTimeDelta);
			m_pOwner.lock()->SetDead();
			m_pAnimator->Set_Default(ANIMATION::FLOWER_CHEST::DOWN);
			m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::DOWN);
		}
	}
}

void CFlowerChest_FSM::EndState_Dead()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_BCChance()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::BC_CHANCE_START, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Set_BcChance(true);
	//m_pOwner.lock()->SetWeakRender(true);
#ifdef _DEBUG
	cout << "FC_status_BCChance" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_BCChance(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_CHEST::BC_CHANCE_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_CHEST::BC_CHANCE_START))
		{
			m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::BC_CHANCE_LOOP, 2.f);
		}
	}
}

void CFlowerChest_FSM::EndState_BCChance()
{
	//m_pOwner.lock()->SetWeakRender(false);
	m_pOwner.lock()->MotionReset();
}

void CFlowerChest_FSM::BeginState_FinishMotion()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_CHEST::AIRBONE, 2.f, false, 0.5f);
	m_pOwner.lock()->InitRootMotion();
	//m_pOwner.lock()->SetWeakRender(true);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
	m_pTransform->LookAt(m_pTargetTransform);
	m_pOwner.lock()->Set_LockOnAble(false);
#ifdef _DEBUG
	cout << "FC_status_BCChance" << endl;
#endif // _DEBUG
}

void CFlowerChest_FSM::ExecuteState_FinishMotion(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(0);
	_float Height = m_pOwner.lock()->Height();
	if (Height < 4.5f)
	{
		m_pTransform->Translate(XMVectorSet(0.f, 1.f, 0.f, 0.f) * fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
	}
	else
		GotoState(m_pState_Dead);
}

void CFlowerChest_FSM::EndState_FinishMotion()
{
	m_pOwner.lock()->MotionReset();
}

shared_ptr<CFlowerChest_FSM> CFlowerChest_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	shared_ptr<CFlowerChest_FSM> pInstance = make_private_shared(CFlowerChest_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CFlowerChest_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
