#include "ClientPCH.h"
#include "Snail_FSM.h"
#include "GameInstance.h"
#include "Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"

#include "UI_Manager.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "UI_Indicator_InstKill.h"

HRESULT CSnail_FSM::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CSnail_FSM> pInstance = static_pointer_cast<CSnail_FSM>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_Idle->Set(pInstance, &CSnail_FSM::BeginState_Idle, &CSnail_FSM::ExecuteState_Idle, &CSnail_FSM::EndState_Idle);
	m_pState_Walk = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_Walk->Set(pInstance, &CSnail_FSM::BeginState_Walk, &CSnail_FSM::ExecuteState_Walk, &CSnail_FSM::EndState_Walk);
	m_pState_Walk_Left = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_Walk_Left->Set(pInstance, &CSnail_FSM::BeginState_Walk_Left, &CSnail_FSM::ExecuteState_Walk_Left, &CSnail_FSM::EndState_Walk_Left);
	m_pState_Walk_Right = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_Walk_Right->Set(pInstance, &CSnail_FSM::BeginState_Walk_Right, &CSnail_FSM::ExecuteState_Walk_Right, &CSnail_FSM::EndState_Walk_Right);
	m_pState_Walk_Back = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_Walk_Back->Set(pInstance, &CSnail_FSM::BeginState_Walk_Back, &CSnail_FSM::ExecuteState_Walk_Back, &CSnail_FSM::EndState_Walk_Back);
	//m_pState_Run = make_shared<CStateTemplate<CSnail_FSM>>();
	//m_pState_Run->Set(pInstance, &CSnail_FSM::BeginState_Run, &CSnail_FSM::ExecuteState_Run, &CSnail_FSM::EndState_Run);
	m_pState_OilWalk = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_OilWalk->Set(pInstance, &CSnail_FSM::BeginState_OilWalk, &CSnail_FSM::ExecuteState_OilWalk, &CSnail_FSM::EndState_OilWalk);
	m_pState_Shoot = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_Shoot->Set(pInstance, &CSnail_FSM::BeginState_Shoot, &CSnail_FSM::ExecuteState_Shoot, &CSnail_FSM::EndState_Shoot);
	m_pState_OnHit_Front = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_OnHit_Front->Set(pInstance, &CSnail_FSM::BeginState_OnHit_Front, &CSnail_FSM::ExecuteState_OnHit_Front, &CSnail_FSM::EndState_OnHit_Front);
	m_pState_OnHit_Back = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_OnHit_Back->Set(pInstance, &CSnail_FSM::BeginState_OnHit_Back, &CSnail_FSM::ExecuteState_OnHit_Back, &CSnail_FSM::EndState_OnHit_Back);
	m_pState_OnHit_Psy_Front = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_OnHit_Psy_Front->Set(pInstance, &CSnail_FSM::BeginState_OnHit_Psy_Front, &CSnail_FSM::ExecuteState_OnHit_Psy_Front, &CSnail_FSM::EndState_OnHit_Psy_Front);
	m_pState_OnHit_Psy_Back = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_OnHit_Psy_Back->Set(pInstance, &CSnail_FSM::BeginState_OnHit_Psy_Back, &CSnail_FSM::ExecuteState_OnHit_Psy_Back, &CSnail_FSM::EndState_OnHit_Psy_Back);
	m_pState_OnHit_Psy_Left = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_OnHit_Psy_Left->Set(pInstance, &CSnail_FSM::BeginState_OnHit_Psy_Left, &CSnail_FSM::ExecuteState_OnHit_Psy_Left, &CSnail_FSM::EndState_OnHit_Psy_Left);
	m_pState_OnHit_Psy_Right = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_OnHit_Psy_Right->Set(pInstance, &CSnail_FSM::BeginState_OnHit_Psy_Right, &CSnail_FSM::ExecuteState_OnHit_Psy_Right, &CSnail_FSM::EndState_OnHit_Psy_Right);
	m_pState_AirBone = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_AirBone->Set(pInstance, &CSnail_FSM::BeginState_AirBone, &CSnail_FSM::ExecuteState_AirBone, &CSnail_FSM::EndState_AirBone);
	m_pState_AirBone_Falling = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_AirBone_Falling->Set(pInstance, &CSnail_FSM::BeginState_AirBone_Falling, &CSnail_FSM::ExecuteState_AirBone_Falling, &CSnail_FSM::EndState_AirBone_Falling); m_pState_Dead = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_Dead->Set(pInstance, &CSnail_FSM::BeginState_Dead, &CSnail_FSM::ExecuteState_Dead, &CSnail_FSM::EndState_Dead);
	m_pState_BCChance = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_BCChance->Set(pInstance, &CSnail_FSM::BeginState_BCChance, &CSnail_FSM::ExecuteState_BCChance, &CSnail_FSM::EndState_BCChance);
	m_pState_FinishMotion = make_shared<CStateTemplate<CSnail_FSM>>();
	m_pState_FinishMotion->Set(pInstance, &CSnail_FSM::BeginState_FinishMotion, &CSnail_FSM::ExecuteState_FinishMotion, &CSnail_FSM::EndState_FinishMotion);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	//m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	m_pOwner = dynamic_pointer_cast<CMonster_Snail>(Owner);
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CSnail_FSM::Initialize", "Failed to Get: Owner");
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
		MSG_RETURN(E_FAIL, "CSnail_FSM::Initialize", " pEntityDesc is NULL");
	}
	m_pEntityDesc = pEntityDesc;
	
	if (nullptr == pActorDesc)
	{
		MSG_RETURN(E_FAIL, "CSnail_FSM::Initialize", " pMonsterDesc is NULL");
	}
	m_pActorDesc= pActorDesc;
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
	m_pUI_NoticeMark = dynamic_pointer_cast<CUI_Indicator_Monster_NoticeMark>(pGameInstance->Clone_GameObject(Scene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK));
	m_pUI_NoticeMark->Initialize();
	m_pUI_InstKIll = dynamic_pointer_cast<CUI_Indicator_InstKill>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL));
	m_pUI_InstKIll->Initialize();
#pragma endregion

	return S_OK;
}

void CSnail_FSM::StateInitial(_float fTimeDelta)
{
	GotoState(m_pState_Idle);
}

void CSnail_FSM::ExecutePersistent(_float fTimeDelta)
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

	_float Radian = (_float)acos(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
	_vector Cross = XMVector3Cross(myLook, Direction);

	if (XMVectorGetY(Cross) <= 0.f)
		Radian *= -1;

	m_fDegree = XMConvertToDegrees(Radian);

	if (m_pEntityDesc->fKnuckBackGauge <= 0.f && m_pCurrentState != m_pState_BCChance && m_pCurrentState != m_pState_FinishMotion && m_pCurrentState != m_pState_AirBone && m_pCurrentState != m_pState_AirBone_Falling)
	{
		m_pUI_InstKIll->Set_InstKillRenderStart();
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("UI_SFX_Kasane_InstantKill_Indicator.wav"), g_fPlayerSound);
		GotoState(m_pState_BCChance);
		return;
	}

	if (m_pActorDesc->bHit)
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

		if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::BLOW_ATTACK_FRONT) || m_pAnimator->Is_Playing(ANIMATION::SNAIL::BLOW_LANDING_FRONT))
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
		auto pGameInstance = CGameInstance::Get_Instance();
		auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
		auto Interactive = Kynesis->Get_RecentThrowdObject();
		_float3 vPos;
		if (m_pActorDesc->stHitDesc.strAttackTag != ACTORTAG::INTERACTIVE_BEAM)
		{
			if (Interactive == nullptr)
				return;
		}
		else
		{
			m_fRootAmount = 0.f;
			//vPos = m_pActorDesc->stHitDesc.vAttackPose;
			//vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			//shared_ptr<CCamera_Player> pCam = static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_Camera();
			//_float4x4 originMat = m_pTransform->Get_Matrix();

			//m_pTransform->Translate({ 0,1.f,0.f,0.f });
			//m_pTransform->Translate(XMVector3Normalize(pCam->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION)));
			//_float4x4 mat = m_pTransform->Get_Matrix();

			//m_pTransform->Set_Matrix(originMat);

			//CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_REGULAR1,
			//	initializer_list<any>{_float3(0, 0, 0), _float3(0, 0, 0), mat, mat, mat});
		}

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

		_vector Direction = vPos - m_pTransform->Get_State(TRANSFORM::POSITION);

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

void CSnail_FSM::BeginState_Idle()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::IDLE1, 2.f);
	m_pOwner.lock()->InitRootMotion();
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
#ifdef _DEBUG
	cout << "Snail_status_Idle" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_Idle(_float fTimeDelta)
{
	FindNewTarget();
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta,m_fRootAmount);
	if (m_fDistance <= 10.f && dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_PlayerDesc().eSAS != PLAYER_SAS::STEALTH)
	{
		GotoState(m_pState_Walk);
	}
}

void CSnail_FSM::EndState_Idle()
{
	m_pUI_NoticeMark->NoticePlayer();

	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_Walk()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::WALK_FRONT, 2.f);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Snail_status_Walk_Forward" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_Walk(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	
	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);


	if (m_fCoolTime <= 0 && (m_fDegree <= 5.f && m_fDegree >= -5.f))
	{
		if (m_fDistance >= 10.f)
		{
			GotoState(m_pState_Shoot);
		}
		else if (m_fDistance <= 3.f)
		{
			GotoState(m_pState_OilWalk);
		}
	}
	else
	{
		if (m_fDistance <= 5)
			GotoState(Function::Random({ m_pState_Walk_Left,m_pState_Walk_Right }));
		else if(m_fDistance <= 2)
			GotoState(m_pState_Walk_Back);
	}
}

void CSnail_FSM::EndState_Walk()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_Walk_Left()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::WALK_LEFT, 2.f);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Snail_status_Walk_Left" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_Walk_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);


	m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::RIGHT) * fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);

	if (m_fCoolTime <= 0)
	{
		if (m_fDistance >= 10.f)
		{
			GotoState(m_pState_Shoot);
		}
		else if (m_fDistance <= 3.f)
		{
			GotoState(m_pState_OilWalk);
		}
	}
}

void CSnail_FSM::EndState_Walk_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_Walk_Right()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::WALK_RIGHT, 2.f);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Snail_status_Walk_Right" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_Walk_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);


	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::RIGHT) *  fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);

	if (m_fCoolTime <= 0)
	{
		if (m_fDistance >= 10.f)
			GotoState(m_pState_Shoot);
		else
			GotoState(m_pState_OilWalk);
	}
}

void CSnail_FSM::EndState_Walk_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_Walk_Back()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::WALK_RIGHT, 2.f);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Snail_status_Walk_Right" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_Walk_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);


	m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);

	if (m_fCoolTime <= 0)
	{
		if (m_fDistance >= 10.f)
		{
			GotoState(m_pState_Shoot);
		}
		else if (m_fDistance <= 3.f)
		{
			GotoState(m_pState_OilWalk);
		}
	}
}

void CSnail_FSM::EndState_Walk_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_OilWalk()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::OIL_WALK_LOOP, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();

	_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
	_float3 Look = m_pTransform->Get_State(TRANSFORM::LOOK);
	Pos.y += 1.f;

	CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED1, Pos + Look);
	CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED2, Pos + Look);

	//m_pOwner.lock()->GetAttackActorDesc().stAttackDesc.eHurtType = PLAYER_HURTTYPE::AIR;
#ifdef _DEBUG
	cout << "Snail OilWalk State" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_OilWalk(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::OIL_WALK_LOOP))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::OIL_WALK_LOOP))
		{
			if(++m_iOilWalkStack <=2)
			{
				m_pAnimator->Play_Animation(ANIMATION::SNAIL::OIL_WALK_LOOP, 2.f, false, g_fDefaultInterpolationDuration, false);
				m_pOwner.lock()->MotionReset();

				_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
				_float3 Look = m_pTransform->Get_State(TRANSFORM::LOOK);
				Pos.y += 1.f;

				CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED1, Pos + Look);
				CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED2, Pos + Look);
				CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED_PARTICLE, Pos);
			}
			else {
				m_fCoolTime = 5.f;
				GotoState(m_pState_Walk);
			}
		}
	}
}

void CSnail_FSM::EndState_OilWalk()
{
	FindNewTarget();
	m_pOwner.lock()->SetWeaponeActiveOff();
	m_iOilWalkStack = 0;
	m_pOwner.lock()->MotionReset();
	m_pOwner.lock()->GetAttackActorDesc().stAttackDesc.eHurtType = PLAYER_HURTTYPE::NONE;
}

void CSnail_FSM::BeginState_Shoot()
{
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::SHOOT, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
	
#ifdef _DEBUG
	cout << "Snail Shoot State" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_Shoot(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_fDegree >= 5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), fTimeDelta);
	else if (m_fDegree <= -5.f)
		m_pTransform->Turn(m_pTransform->Get_State(TRANSFORM::UP), -fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::SHOOT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::SHOOT))
		{
			m_fCoolTime = 5.f;
			GotoState(m_pState_Walk);
		}
	}

}

void CSnail_FSM::EndState_Shoot()
{
	FindNewTarget();
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_OnHit_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DAMAGE_FRONT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Snail_status_Demaged_Normal_Front" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_OnHit_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, true);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DAMAGE_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DAMAGE_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CSnail_FSM::EndState_OnHit_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_OnHit_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DAMAGE_BACK, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Snail_status_Demaged_Normal_Back" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_OnHit_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DAMAGE_BACK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DAMAGE_BACK))
			GotoState(m_pState_Walk);
	}
}

void CSnail_FSM::EndState_OnHit_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_OnHit_Psy_Front()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Snail_status_Demaged_Psyco_PSY_FRONT" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_OnHit_Psy_Front(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CSnail_FSM::EndState_OnHit_Psy_Front()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_OnHit_Psy_Back()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DAMAGE_PSY_FRONT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Snail_status_Demaged_Psyco_Back" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_OnHit_Psy_Back(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DAMAGE_PSY_FRONT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DAMAGE_PSY_FRONT))
			GotoState(m_pState_Walk);
	}
}

void CSnail_FSM::EndState_OnHit_Psy_Back()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_OnHit_Psy_Left()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DAMAGE_PSY_LEFT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Snail_status_Demaged_Psyco_Left" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_OnHit_Psy_Left(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DAMAGE_PSY_LEFT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DAMAGE_PSY_LEFT))
			GotoState(m_pState_Walk);
	}
}

void CSnail_FSM::EndState_OnHit_Psy_Left()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_OnHit_Psy_Right()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DAMAGE_PSY_RIGHT, 2.f, false, 0.5f, false);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pOwner.lock()->InitRootMotion();

#ifdef _DEBUG
	cout << "Snail_status_Demaged_Psyco_Right" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_OnHit_Psy_Right(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DAMAGE_PSY_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DAMAGE_PSY_RIGHT))
			GotoState(m_pState_Walk);
	}

}

void CSnail_FSM::EndState_OnHit_Psy_Right()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_AirBone()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::BLOW_ATTACK_FRONT, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Snail_status_AirBone" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_AirBone(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::BLOW_ATTACK_FRONT))
	{

		if (m_pOwner.lock()->Height() <= 6.f)
			m_pTransform->Translate((-m_pTransform->Get_State(TRANSFORM::LOOK) + _float4(0, 7, 0, 0)) * fTimeDelta * 2.f);

		m_pOwner.lock()->AirBone(fTimeDelta);
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Get_TrackPosition(ANIMATION::SNAIL::BLOW_ATTACK_FRONT) >= 20.f)
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::SNAIL::BLOW_LANDING_FRONT, 2.f, false, 0.5f, false);
			return;
		}
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::BLOW_LANDING_FRONT))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pOwner.lock()->GetCollisionFlags() == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_AirBone_Falling);
		}
	}
}

void CSnail_FSM::EndState_AirBone()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_AirBone_Falling()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DOWN_SHOCK, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
#ifdef _DEBUG
	cout << "Snail_status_AirBone_Falling" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_AirBone_Falling(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DOWN_SHOCK))
	{
		m_pOwner.lock()->No_RootMotion(fTimeDelta);
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DOWN_SHOCK))
		{
			m_pOwner.lock()->MotionReset();
			m_pAnimator->Play_Animation(ANIMATION::SNAIL::GETUP, 2.f, false, g_fDefaultInterpolationDuration, false);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::GETUP))
	{
		m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::GETUP))
		{
			m_pOwner.lock()->MotionReset();
			GotoState(m_pState_Walk);
		}
	}
}

void CSnail_FSM::EndState_AirBone_Falling()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_Dead()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DEAD_DOWN, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->Dissolve();
	m_pOwner.lock()->DeadSlow();
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->ReleaseController();
	m_pOwner.lock()->Set_LockOnAble(false);
	m_pOwner.lock()->SetWeaponeActiveOff();
	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ L"Monster_Snail_Dead01.wav" ,L"Monster_Snail_Dead00.wav" }), 3.f);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
#ifdef _DEBUG
	cout << "Snail_status_Death" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_Dead(_float fTimeDelta)
{
	//m_pOwner.lock()->Update_Motion(fTimeDelta);
	
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DEAD_DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DEAD_DOWN))
		{
			m_pOwner.lock()->SetDead();
			m_pAnimator->Set_Default(ANIMATION::SNAIL::DOWN);
			m_pAnimator->Play_Animation(ANIMATION::SNAIL::DOWN, 2.f);
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DOWN))
		{
			//nothing
		}
	}
	//m_pOwner.lock()->Gravity(fTimeDelta);
}

void CSnail_FSM::EndState_Dead()
{
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_BCChance()
{
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::BC_CHANCE_START, 2.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pOwner.lock()->Set_BcChance(true);
	//m_pOwner.lock()->SetWeakRender(true);
#ifdef _DEBUG
	cout << "Snail_status_BCChance" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_BCChance(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::BC_CHANCE_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::BC_CHANCE_START))
		{
			m_pAnimator->Play_Animation(ANIMATION::SNAIL::BC_CHANCE_LOOP, 2.f);
		}
	}
}

void CSnail_FSM::EndState_BCChance()
{
	m_pOwner.lock()->Set_BcChance(false);
	m_pOwner.lock()->MotionReset();
}

void CSnail_FSM::BeginState_FinishMotion()
{
	m_pOwner.lock()->Get_UI_HP()->Set_MonsterNameTagRemove_Execution();

	m_pAnimator->Play_Animation(ANIMATION::SNAIL::FINISH, 3.f, false, 0.5f, false);
	m_pOwner.lock()->InitRootMotion();
	m_pTransform->LookAt(m_pPlayerTransform);
	m_pOwner.lock()->SetWeakRender(true);
	m_pOwner.lock()->Set_LockOnAble(false);
	//m_pModel->Set_Pivot(m_pModel->Get_Pivot() * XMMatrixRotationY(XMConvertToRadians(180.f)));
	//m_pTransform->Rotate(TRANSFORM::UP, XMConvertToRadians(180.f));
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOn(false);
	static_pointer_cast<CCamera_Player>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera())->Set_LockOnTransform(nullptr);
#ifdef _DEBUG
	cout << "Snail_status_FinishMotion" << endl;
#endif // _DEBUG
}

void CSnail_FSM::ExecuteState_FinishMotion(_float fTimeDelta)
{
	m_pOwner.lock()->Update_Motion(fTimeDelta);
	m_pOwner.lock()->RootMotion(fTimeDelta, m_fRootAmount);
	if (_int(m_pAnimator->Get_TrackPosition(ANIMATION::SNAIL::FINISH)) == 270 || _int(m_pAnimator->Get_TrackPosition(ANIMATION::SNAIL::FINISH)) == 271.f)
	{
		m_pOwner.lock()->Dissolve();
		m_pOwner.lock()->Dissolve_Weak();
	}

	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::FINISH))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::FINISH))
		{
			m_pOwner.lock()->SetDead();
			m_pOwner.lock()->InitRootMotion();
			m_pOwner.lock()->ReleaseController();
			m_pOwner.lock()->Set_LockOnAble(false);
			m_pOwner.lock()->SetWeaponeActiveOff();
		}
	}
}

void CSnail_FSM::EndState_FinishMotion()
{
	m_pOwner.lock()->MotionReset();
	//m_pModel->Set_Pivot(m_pModel->Get_Pivot() * XMMatrixRotationY(XMConvertToRadians(180.f)));
}

void CSnail_FSM::FindNewTarget()
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

shared_ptr<CSnail_FSM> CSnail_FSM::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc)
{
	shared_ptr<CSnail_FSM> pInstance = make_private_shared(CSnail_FSM);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CSnail_LeafNode_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}

void CSnail_FSM::GotoFinish()
{
	m_pUI_InstKIll->Set_InstKillRenderEnd();

	GotoState(m_pState_FinishMotion);
}
