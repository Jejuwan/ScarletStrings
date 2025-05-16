#include "ClientPCH.h"
#include "FlowerHead_FSM.h"
#include "GameInstance.h"
#include "Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "Camera_Player.h"
#include "Map_Dynamic.h"

HRESULT CFlowerHead_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CFlowerHead_FSM> pInstance = static_pointer_cast<CFlowerHead_FSM>(shared_from_this());
	
	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_Idle->Set(pInstance, &CFlowerHead_FSM::BeginState_Idle, &CFlowerHead_FSM::ExecuteState_Idle, &CFlowerHead_FSM::EndState_Idle);
	m_pState_Walk = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_Walk->Set(pInstance, &CFlowerHead_FSM::BeginState_Walk, &CFlowerHead_FSM::ExecuteState_Walk, &CFlowerHead_FSM::EndState_Walk);
	m_pState_Run = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_Run->Set(pInstance, &CFlowerHead_FSM::BeginState_Run, &CFlowerHead_FSM::ExecuteState_Run, &CFlowerHead_FSM::EndState_Run);
	m_pState_SpinAttack = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_SpinAttack->Set(pInstance, &CFlowerHead_FSM::BeginState_SpinAttack, &CFlowerHead_FSM::ExecuteState_SpinAttack, &CFlowerHead_FSM::EndState_SpinAttack);
	m_pState_Shoot = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_Shoot->Set(pInstance, &CFlowerHead_FSM::BeginState_Shoot, &CFlowerHead_FSM::ExecuteState_Shoot, &CFlowerHead_FSM::EndState_Shoot);
	m_pState_OnHit_Front = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_OnHit_Front->Set(pInstance, &CFlowerHead_FSM::BeginState_OnHit_Front, &CFlowerHead_FSM::ExecuteState_OnHit_Front, &CFlowerHead_FSM::EndState_OnHit_Front);
	m_pState_OnHit_Back = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_OnHit_Back->Set(pInstance, &CFlowerHead_FSM::BeginState_OnHit_Back, &CFlowerHead_FSM::ExecuteState_OnHit_Back, &CFlowerHead_FSM::EndState_OnHit_Back);
	m_pState_OnHit_Psy_Front = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_OnHit_Psy_Front->Set(pInstance, &CFlowerHead_FSM::BeginState_OnHit_Psy_Front, &CFlowerHead_FSM::ExecuteState_OnHit_Psy_Front, &CFlowerHead_FSM::EndState_OnHit_Psy_Front);
	m_pState_OnHit_Psy_Back = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_OnHit_Psy_Back->Set(pInstance, &CFlowerHead_FSM::BeginState_OnHit_Psy_Back, &CFlowerHead_FSM::ExecuteState_OnHit_Psy_Back, &CFlowerHead_FSM::EndState_OnHit_Psy_Back);
	m_pState_OnHit_Psy_Left = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_OnHit_Psy_Left->Set(pInstance, &CFlowerHead_FSM::BeginState_OnHit_Psy_Left, &CFlowerHead_FSM::ExecuteState_OnHit_Psy_Left, &CFlowerHead_FSM::EndState_OnHit_Psy_Left);
	m_pState_OnHit_Psy_Right = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_OnHit_Psy_Right->Set(pInstance, &CFlowerHead_FSM::BeginState_OnHit_Psy_Right, &CFlowerHead_FSM::ExecuteState_OnHit_Psy_Right, &CFlowerHead_FSM::EndState_OnHit_Psy_Right);
	m_pState_AirBone = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_AirBone->Set(pInstance, &CFlowerHead_FSM::BeginState_AirBone, &CFlowerHead_FSM::ExecuteState_AirBone, &CFlowerHead_FSM::EndState_AirBone);
	m_pState_AirBone_Falling = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_AirBone_Falling->Set(pInstance, &CFlowerHead_FSM::BeginState_AirBone_Falling, &CFlowerHead_FSM::ExecuteState_AirBone_Falling, &CFlowerHead_FSM::EndState_AirBone_Falling);
	m_pState_Dead = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_Dead->Set(pInstance, &CFlowerHead_FSM::BeginState_Dead, &CFlowerHead_FSM::ExecuteState_Dead, &CFlowerHead_FSM::EndState_Dead);
	m_pState_BCChance = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_BCChance->Set(pInstance, &CFlowerHead_FSM::BeginState_BCChance, &CFlowerHead_FSM::ExecuteState_BCChance, &CFlowerHead_FSM::EndState_BCChance);
	m_pState_FinishMotion = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_FinishMotion->Set(pInstance, &CFlowerHead_FSM::BeginState_FinishMotion, &CFlowerHead_FSM::ExecuteState_FinishMotion, &CFlowerHead_FSM::EndState_FinishMotion);
	m_pState_FinishMotion_Common = make_shared<CStateTemplate<CFlowerHead_FSM>>();
	m_pState_FinishMotion_Common->Set(pInstance, &CFlowerHead_FSM::BeginState_FinishMotion_Common, &CFlowerHead_FSM::ExecuteState_FinishMotion_Common, &CFlowerHead_FSM::EndState_FinishMotion_Common);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_FlowerHead>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CFlowerHead_FSM::Initialize", "Failed to Get: Owner");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_pOwner.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CSnail_FSM::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pPlayerTransform = dynamic_pointer_cast<CTransform>(Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));
	if (nullptr == m_pPlayerTransform)
	{
		MSG_RETURN(E_FAIL, "CSnail_FSM::Initialize", "Failed to Get: Player:Transform");
	}

	m_pModel = dynamic_pointer_cast<CModel>(m_pOwner.lock()->Get_Component(COMPONENT::MODEL));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CSnail_FSM::Initialize", "Failed to Get: Player:MODEL");
	}

	m_pAnimator = dynamic_pointer_cast<CAnimator>(m_pOwner.lock()->Get_Behavior(BEHAVIOR::ANIMATOR));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CSnail_FSM::Initialize", "Failed to Get: Player:ANIMATOR");
	}

	if (nullptr == pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CFlowerHead_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;
	
	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CFlowerHead_FSM::Initialize", " pMonsterDesc is NULL");
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

#pragma region UI
	m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	m_pUI_NoticeMark->Initialize();
#pragma endregion

	return S_OK;
}

void CFlowerHead_FSM::StateInitial(_float fTimeDelta)
{
	GotoState(m_pState_Idle);
}

void CFlowerHead_FSM::ExecutePersistent(_float fTimeDelta)
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
	//if (m_pEntityDesc->fNuckBackGauge <= 0.f && m_pCurrentState != m_pState_BCChance && m_pCurrentState != m_pState_AirBone && m_pCurrentState != m_pState_AirBone_Falling)
	//{
	//	GotoState(m_pState_BCChance);
	//	return;
	//}

	if (m_pActorDesc->bHit)
	{
		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}

		auto pGameInstance = CGameInstance::Get_Instance();
		CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
		if (nullptr == pPlayer->Get_Camera()->Get_LockOnTransform())
		{
			//pPlayer->Get_Camera()->Set_LockOn(true);
			//pPlayer->Get_Camera()->Set_LockedMonster(m_pOwner.lock());
			//pPlayer->Get_Camera()->Set_LockOnTransform(m_pTransform);
			//pPlayer->Set_FinishMonster(m_pOwner.lock());
		}

		m_pEntityDesc->fHP -= m_pActorDesc->stHitDesc.iDamage;

		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}

		if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::BLOW_START_FRONT) || m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::BLOW_LANDING_FRONT))
			//m_pCurrentState == m_pState_AirBone /*|| m_pCurrentState == m_pState_AirBone_Falling*/)
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

	}
	if (m_pActorDesc->bThrowHit)
	{
		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("Environment_SFX_CarCrushed.wav"), g_fPlayerSound);
		auto pGameInstance = CGameInstance::Get_Instance();
		CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
		if (nullptr == pPlayer->Get_Camera()->Get_LockOnTransform())
		{
		/*	pPlayer->Get_Camera()->Set_LockOn(true);
			pPlayer->Get_Camera()->Set_LockedMonster(m_pOwner.lock());
			pPlayer->Get_Camera()->Set_LockOnTransform(m_pTransform);
			pPlayer->Set_FinishMonster(m_pOwner.lock());*/
		}

		auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
		auto Interactive = Kynesis->Get_RecentThrowdObject();
		if (Interactive == nullptr)
			return;

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
		///Kynesis->Set_RecentThrowObject(nullptr);
		if (m_pEntityDesc->fHP <= 0)
		{
			GotoState(m_pState_Dead);
			return;
		}
	}

	if (m_pActorDesc->bAirBone)
	{
		GotoState(m_pState_AirBone);
		return;
	}
}

void CFlowerHead_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::IDLE1, 3.f);
	m_pOwner.lock()->InitRootMotion();
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));

#ifdef _DEBUG
	cout << "FlowerHead_status_Idle" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	FindNewTarget();
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_fDistance <= 5.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	{
		GotoState(m_pState_Walk);
	}
}

void CFlowerHead_FSM::EndState_Idle()
{
	m_pUI_NoticeMark->NoticePlayer();

	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_Walk()
{
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::WALK_FRONT1, 2.f);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "FlowerHead_status_Walk_Forward" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_Walk(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

	//m_pTransform->LookAt(m_pTargetTransform);

	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);

	if (m_fCoolTime <= 0)
	{
		if (m_fDistance >= 10.f)
		{
			GotoState(m_pState_Shoot);
		}
		else if (m_fDistance <= 5.f)
		{
			GotoState(m_pState_SpinAttack);
		}
	}
	else
	{
		if (m_fDistance >= 10.f)
		{
			GotoState(m_pState_Run);
		}
	}
}

void CFlowerHead_FSM::EndState_Walk()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_Run()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::RUN, 2.f, false, 1.f);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Run" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_Run(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

	//m_pTransform->LookAt(m_pTargetTransform);
	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 3.f);

	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_fDistance <= 5.f)
	{
		GotoState(m_pState_Walk);
	}
}

void CFlowerHead_FSM::EndState_Run()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_SpinAttack()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::SPIN, 1.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "FlowerHead Spin State" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_SpinAttack(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::SPIN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::SPIN))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}
}

void CFlowerHead_FSM::EndState_SpinAttack()
{
	FindNewTarget();
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_Shoot()
{
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::SHOOT1, 1.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FlowerHead Shoot State" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_Shoot(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pTransform->LookAt(m_pTargetTransform);
	//m_pOwner.lock()->Gravity(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta, true);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::SHOOT1))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::SHOOT1))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}

}

void CFlowerHead_FSM::EndState_Shoot()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_OnHit_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DAMAGE_FRONT, 2.f, false, 0.5f, false);
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Front" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_OnHit_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta, true);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DAMAGE_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DAMAGE_FRONT))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}
}

void CFlowerHead_FSM::EndState_OnHit_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_OnHit_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DAMAGE_BACK, 2.f, false, 0.5f, false);
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Normal_Back" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_OnHit_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DAMAGE_BACK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DAMAGE_BACK))
			GotoState(m_pState_Walk);
	}
}

void CFlowerHead_FSM::EndState_OnHit_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_OnHit_Psy_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_PSY_FRONT" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_OnHit_Psy_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CFlowerHead_FSM::EndState_OnHit_Psy_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_OnHit_Psy_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Back" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_OnHit_Psy_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CFlowerHead_FSM::EndState_OnHit_Psy_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_OnHit_Psy_Left()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_LEFT, 2.f, false, 0.5f, false);
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Left" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_OnHit_Psy_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_LEFT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_LEFT))
			GotoState(m_pState_Walk);
	}
}

void CFlowerHead_FSM::EndState_OnHit_Psy_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_OnHit_Psy_Right()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_RIGHT, 2.f, false, 0.5f, false);
	//m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "FC_status_Demaged_Psyco_Right" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_OnHit_Psy_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	//m_pOwner.lock()->No_RootMotion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DAMAGE_PSY_RIGHT))
			GotoState(m_pState_Walk);
	}

}

void CFlowerHead_FSM::EndState_OnHit_Psy_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_AirBone()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::BLOW_START_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "FC_status_AirBone" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_AirBone(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::BLOW_START_FRONT))
	{
		//m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSetY(m_pTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pTargetTransform->Get_State(TRANSFORM::POSITION)) + 0.5f));


		if (m_pOwner.lock()->Height() <= 6.f)
			m_pTransform->Translate((-m_pTransform->Get_State(TRANSFORM::LOOK) + _float4(0, 7, 0, 0)) * fTimeDelta * 2.f);

		m_pOwner.lock()->AirBone(fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Get_TrackPosition(ANIMATION::FLOWER_HEAD::BLOW_START_FRONT) >= 20.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::BLOW_LANDING_FRONT, 2.f, false, 0.5f, false);
			return;
		}
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::BLOW_LANDING_FRONT))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
}

void CFlowerHead_FSM::EndState_AirBone()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_AirBone_Falling()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DOWN_SHOCK, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "FC_status_AirBone_Falling" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_AirBone_Falling(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DOWN_SHOCK))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DOWN_SHOCK))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::GETUP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::GETUP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::GETUP))
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_Walk);
		}
	}
}

void CFlowerHead_FSM::EndState_AirBone_Falling()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_Dead()
{
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->ReleaseController();
	m_pOwner.lock()->Set_LockOnAble(false);
	CGameInstance::Get_Instance()->PlaySoundW(L"Monster_Communal_Disappear.wav", 1.f);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_Shake(0.5f,7.f);
	auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
	shared_ptr<CInteractiveObject> Interactive = Kynesis->Get_RecentThrowdObject();
	auto pTransform = static_pointer_cast<CTransform>(Interactive->Get_Component(COMPONENT::TRANSFORM));
	m_vDeadDir = pTransform->Get_State(TRANSFORM::LOOK);
#ifdef _DEBUG
	cout << "FlowerHead_status_Death" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	//m_pOwner.lock()->Update_Motion(fTimeDelta);

	auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
	shared_ptr<CMap_Dynamic> Interactive = static_pointer_cast<CMap_Dynamic>(Kynesis->Get_RecentThrowdObject());
	m_fDeadTime += fTimeDelta;

	if(!Interactive->Get_ActorDesc().bThrow)
	{
		if (m_fDeadTime < 0.5f)
		{
			m_pTransform->Translate(m_vDeadDir * fTimeDelta * 20.f);
			m_pTransform->Translate({ 0,m_fDeadTime * 2.f - m_fDeadTime * m_fDeadTime * 6.f,0,0 });
		}
		else
		{
			if (!m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DEAD_DOWN))
			{
				m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DEAD_DOWN, 2.f, false, g_fDefaultInterpolationDuration, false);
				//m_pOwner.lock()->DeadSlow();
				m_pOwner.lock()->Dissolve();
			}
		}
	}
	else
	{
		if (!m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DEAD_DOWN))
		{
			m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DEAD_DOWN, 2.f, false, g_fDefaultInterpolationDuration, false);
			//m_pOwner.lock()->DeadSlow();
			m_pOwner.lock()->Dissolve();
		}
	}

	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DEAD_DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DEAD_DOWN))
		{
			m_pOwner.lock()->SetDead();
			m_pAnimator->Set_Default(ANIMATION::FLOWER_HEAD::DOWN);
			m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::DOWN);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::DOWN))
		{
			//nothing
		}
	}

}

void CFlowerHead_FSM::EndState_Dead()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_BCChance()
{
}

void CFlowerHead_FSM::ExecuteState_BCChance(_float fTimeDelta)
{
}

void CFlowerHead_FSM::EndState_BCChance()
{
}

void CFlowerHead_FSM::BeginState_FinishMotion()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::FINISH, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pTransform->LookAt(m_pTargetTransform);
	m_pOwner.lock()->SetWeakRender(true);
	m_pOwner.lock()->Set_LockOnAble(false);
#ifdef _DEBUG
	cout << "Snail_status_FinishMotion" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_FinishMotion(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::FINISH))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::FINISH))
		{
			m_pOwner.lock()->SetDead();
		}
	}
}

void CFlowerHead_FSM::EndState_FinishMotion()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::BeginState_FinishMotion_Common()
{
	m_pAnimator->Play_Animation(ANIMATION::FLOWER_HEAD::FINISH, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->SetWeakRender(true);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
	m_pTransform->LookAt(m_pTargetTransform);
	m_pOwner.lock()->Set_LockOnAble(false);
#ifdef _DEBUG
	cout << "FC_status_BCChance" << endl;
#endif // _DEBUG
}

void CFlowerHead_FSM::ExecuteState_FinishMotion_Common(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::FLOWER_HEAD::FINISH))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::FLOWER_HEAD::FINISH))
		{
			m_pOwner.lock()->SetDead();
		}
	}
}

void CFlowerHead_FSM::EndState_FinishMotion_Common()
{
	m_pOwner.lock()->MotionReset();
}

void CFlowerHead_FSM::FindNewTarget()
{
	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::TEST || CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST || CGameInstance::Get_Instance()->Current_Scene() == SCENE::SUOH_BATTLE)
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

shared_ptr<CFlowerHead_FSM> CFlowerHead_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	shared_ptr<CFlowerHead_FSM> pInstance = make_private_shared(CFlowerHead_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CFlowerHead_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
