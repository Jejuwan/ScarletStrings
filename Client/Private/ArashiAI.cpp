#include "ClientPCH.h"

#include "ArashiAI.h"
#include "Arashi_Teammate.h"
#include "Monster.h"

#include "GameInstance.h"
#include "Effect_Manager.h"

#include "Player.h"
#include "Camera_Player.h"
#include "PlayerState.h"

HRESULT CArashiAI::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> pOwner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(E_FAIL, "CArashiAI::Initialize", "Failed to CTeammateAI::Initialize");
	}

	shared_ptr<CArashiAI> pInstance = static_pointer_cast<CArashiAI>(shared_from_this());

	m_pState_Battle_Idle = make_shared<CStateTemplate<CArashiAI>>();
	m_pState_Battle_Idle->Set(pInstance, &CArashiAI::BeginState_Battle_Idle, &CArashiAI::ExecuteState_Battle_Idle, &CArashiAI::EndState_Battle_Idle);
	m_pState_Battle_Chase = make_shared<CStateTemplate<CArashiAI>>();
	m_pState_Battle_Chase->Set(pInstance, &CArashiAI::BeginState_Battle_Chase, &CArashiAI::ExecuteState_Battle_Chase, &CArashiAI::EndState_Battle_Chase);
	m_pState_Attack_Dodge = make_shared<CStateTemplate<CArashiAI>>();
	m_pState_Attack_Dodge->Set(pInstance, &CArashiAI::BeginState_Attack_Dodge, &CArashiAI::ExecuteState_Attack_Dodge, &CArashiAI::EndState_Attack_Dodge);
	m_pState_Attack = make_shared<CStateTemplate<CArashiAI>>();
	m_pState_Attack->Set(pInstance, &CArashiAI::BeginState_Attack, &CArashiAI::ExecuteState_Attack, &CArashiAI::EndState_Attack);
	m_pState_Attack_Escape = make_shared<CStateTemplate<CArashiAI>>();
	m_pState_Attack_Escape->Set(pInstance, &CArashiAI::BeginState_Attack_Escape, &CArashiAI::ExecuteState_Attack_Escape, &CArashiAI::EndState_Attack_Escape);

	m_wstrJumpSound = TEXT("Voice_Arasi_Jump.wav");
	m_wstrLandingSound = TEXT("Voice_Arasi_Landing.wav");

	return S_OK;
}

#pragma region Default State

void CArashiAI::BeginState_Idle()
{
	m_iWaitAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_WAIT01_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_WAIT01);
	__super::BeginState_Idle();
}

void CArashiAI::ExecuteState_Idle(_float fTimeDelta)
{
	__super::ExecuteState_Idle(fTimeDelta);
}

void CArashiAI::EndState_Idle()
{
	__super::EndState_Idle();
}

void CArashiAI::BeginState_Chase()
{
	m_iWalkAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_WALK_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_WALK);
	__super::BeginState_Chase();
}

void CArashiAI::ExecuteState_Chase(_float fTimeDelta)
{
	m_iRunAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_RUN_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_RUN);
	m_iDashAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_DASH_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_DASH);

	__super::ExecuteState_Chase(fTimeDelta);
}

void CArashiAI::EndState_Chase()
{
	__super::EndState_Chase();
}

void CArashiAI::BeginState_Jump()
{
	m_iJumpStartAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_START_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_START);
	m_iJumpLandingAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_LANDING_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_LANDING);
	__super::BeginState_Jump();
}

void CArashiAI::ExecuteState_Jump(_float fTimeDelta)
{
	m_iJumpStartAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_START_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_START);
	m_iJumpRiseAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_RISE_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_RISE);
	m_iJumpFallAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_FALL_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_FALL);
	m_iJumpLandingAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_LANDING_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_LANDING);
	__super::ExecuteState_Jump(fTimeDelta);
}

void CArashiAI::EndState_Jump()
{
	m_iJumpStartAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_START_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_START);
	m_iJumpRiseAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_RISE_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_RISE);
	m_iJumpFallAnimIndex = m_bBattleMode ? IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_FALL_BATTLE) : IDX(ANIMATION::ARASHI_TEAMMATE::AL_JUMP_FALL);
	__super::EndState_Jump();
}

void CArashiAI::BeginState_Hit()
{
	auto pOwner = m_pOwner.lock();
	auto pModel = m_pModel.lock();

	pModel->Reset_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_DAMAGE_L_FL));
	pModel->Set_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_DAMAGE_L_FL), 2.f, false, 0.2f, false);

	pOwner->Init_RootMotion();

	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Arasi_Damaged0.wav"), TEXT("Voice_Arasi_Damaged1.wav"), TEXT("Voice_Arasi_Damaged2.wav") }), 5.f);
}

void CArashiAI::ExecuteState_Hit(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	pOwner->Update_Motion(fTimeDelta);
	pOwner->RootMotion(fTimeDelta);

	auto pModel = m_pModel.lock();
	if (pModel->Is_AnimationFinished(IDX(ANIMATION::ARASHI_TEAMMATE::AL_DAMAGE_L_FL)))
	{
		GotoState(m_pState_Attack_Escape);
	}
}

void CArashiAI::EndState_Hit()
{
	auto pModel = m_pModel.lock();
	pModel->Reset_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_DAMAGE_L_FL));

	m_pOwner.lock()->Reset_Motion();
}

void CArashiAI::BeginState_Trans_Battle()
{
	m_iUpTransBattleAnimIndex = IDX(ANIMATION::ARASHI_TEAMMATE::UP_TRANS_BATTLE);
	__super::BeginState_Trans_Battle();
}

void CArashiAI::ExecuteState_Trans_Battle(_float fTimeDelta)
{
	m_iUpTransBattleAnimIndex = IDX(ANIMATION::ARASHI_TEAMMATE::UP_TRANS_BATTLE);
	__super::ExecuteState_Trans_Battle(fTimeDelta);
}

void CArashiAI::EndState_Trans_Battle()
{
	__super::EndState_Trans_Battle();
}

void CArashiAI::ExecutePersistent(_float fTimeDelta)
{
	__super::ExecutePersistent(fTimeDelta);

	if (m_bBattleMode && false == m_bOnBattle)
	{
		auto pTransform = m_pTransform.lock();
		auto pGameInstance = CGameInstance::Get_Instance();
		auto pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);

		auto Iter = pLayer->Begin();
		while (Iter != pLayer->End())
		{
			auto pMonster = dynamic_pointer_cast<CMonster>(*Iter);
			if (pGameInstance->Current_Scene() == SCENE::MINDROOM)
			{
				if (pMonster->Get_Type() == CMonster::MONSTERTYPE::Karen)
				{
					m_bOnBattle = true;
					m_pTargetMonster = pMonster;
					GotoState(m_pState_Battle_Idle);

					break;
				}
			}
			else
			{
				auto pMonsterTrasnform = dynamic_pointer_cast<CTransform>(pMonster->Get_Component(COMPONENT::TRANSFORM));

				_float3 vMonsterPosition = pMonsterTrasnform->Get_State(TRANSFORM::POSITION);
				_float3 vPosition = pTransform->Get_State(TRANSFORM::POSITION);

				if (XMVectorGetX(XMVector3Length(vMonsterPosition - vPosition)) <= m_fExecuteBattleDistanceThreshold)
				{
					m_bOnBattle = true;
					m_pTargetMonster = pMonster;
					GotoState(m_pState_Battle_Idle);

					break;
				}
			}

			++Iter;
		}
	}

	//if (m_bOnBattle && m_pTargetMonster)
	//{

	//	//Get_Player -> State -> Current State == PLAYER_STATE::EXECUTE

	//}

	if (m_pActorDesc->bHit && *m_pActorDesc->stHitDesc.AttackColliderActive)
	{
		m_pActorDesc->bHit = false;
		GotoState(m_pState_Hit);
	}
}

#pragma endregion

#pragma region Battle State
/*==================================== Battle Idle ====================================*/
void CArashiAI::BeginState_Battle_Idle()
{
	auto pOwner = m_pOwner.lock();
	m_pModel.lock()->Set_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_WAIT01_BATTLE), 2.f, false, 0.2f, true);

	pOwner->Init_RootMotion();
}

void CArashiAI::ExecuteState_Battle_Idle(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	pOwner->Update_Motion(fTimeDelta);
	pOwner->Motion(fTimeDelta);

	if (nullptr == m_pTargetMonster)
	{
		m_pTargetMonster = nullptr;
		m_bOnBattle = false;

		GotoState(m_pState_Idle);
	}
	else
	{
		_bool bAlive = !m_pTargetMonster->GetDead();
		if (bAlive)
		{
			GotoState(m_pState_Battle_Chase);
			//auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));

			//_float3 vOwnerPosition = XMVectorSetY(m_pTransform.lock()->Get_State(TRANSFORM::POSITION), 0.f);
			//_float3 vMonsterPosition = XMVectorSetY(pMonsterTransform->Get_State(TRANSFORM::POSITION), 0.f);

			//if (XMVectorGetX(XMVector3Length(vMonsterPosition - vOwnerPosition)) >= m_fBattleChaseDistanceThreshold)
			//{
			//	GotoState(m_pState_Battle_Chase);
			//}
		}
		else
		{
			m_pTargetMonster = nullptr;
			m_bOnBattle = false;

			GotoState(m_pState_Idle);
		}
	}
}

void CArashiAI::EndState_Battle_Idle()
{
	m_pOwner.lock()->Reset_Motion();
}

/*==================================== Battle Chase ====================================*/
void CArashiAI::BeginState_Battle_Chase()
{
	auto pOwner = m_pOwner.lock();
	m_pModel.lock()->Set_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_RUN_BATTLE), 2.f, false, 0.2f, true);

	pOwner->Init_RootMotion();

	m_fAngle = Function::RandomFloat(0.f, XM_2PI);
	m_fAttackInterval = Function::RandomFloat(2.f, 6.f);
	m_bRightChase = Function::Random({ true, false });
}

void CArashiAI::ExecuteState_Battle_Chase(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	pOwner->Update_Motion(fTimeDelta);

	_bool bAlive = !m_pTargetMonster->GetDead();
	if (bAlive)
	{
		auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));

		auto pTransform = m_pTransform.lock();
		_float3 vOwnerOriginPosition = pTransform->Get_State(TRANSFORM::POSITION);
		_float3 vOwnerPosition = XMVectorSetY(vOwnerOriginPosition, 0.f);
		_float3 vMonsterPosition = XMVectorSetY(pMonsterTransform->Get_State(TRANSFORM::POSITION), 0.f);

		_float fDistance = XMVectorGetX(XMVector3Length(vMonsterPosition - vOwnerPosition));

		_float4 vNextPosition;
		vNextPosition.x = vMonsterPosition.x + m_fChaseRadius * cos(m_fAngle);
		vNextPosition.y = vOwnerOriginPosition.y;
		vNextPosition.z = vMonsterPosition.z + m_fChaseRadius * sin(m_fAngle);
		vNextPosition.w = 1.f;

		_vector vDirection = vNextPosition - pTransform->Get_State(TRANSFORM::POSITION);
		if (fDistance >= m_fBattleChaseDistanceThreshold)
		{
			pTransform->LookAt_Interpolation(vNextPosition, true);
			pTransform->Translate(XMVector3Normalize(vNextPosition - pTransform->Get_State(TRANSFORM::POSITION)) * m_fRunSpeed * fTimeDelta);
		}
		else
		{
			if (XMVectorGetX(XMVector3Length(vDirection)) <= 0.3f)
				m_fAngle += m_bRightChase ? fTimeDelta * 5.f : -fTimeDelta * 5.f;

			pTransform->LookAt_Interpolation(vNextPosition, true);
			pTransform->Translate(XMVector3Normalize(vDirection) * m_fRunSpeed * fTimeDelta);
		}

		if (false == pOwner->Get_Transparency())
			m_fAttackDelay += fTimeDelta;

		if (m_fAttackDelay >= m_fAttackInterval)
		{
			auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
			if (m_pTargetMonster->Get_BcChance() || pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::EXECUTE)
			{
				GotoState(m_pState_Battle_Chase);
			}
			else
			{
				GotoState(m_pState_Attack_Dodge);
			}
		}
		else if (false == m_pTargetMonster->Get_BcChance() && false == m_bAfterImageEffect && m_fAttackDelay >= (m_fAttackInterval - 0.35f))
		{
			m_bAfterImageEffect = true;

			auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
			if (pPlayer->Get_PlayerDesc().eSAS != PLAYER_SAS::ACCELERATION && m_pTargetMonster->Get_Type() != CMonster::MONSTERTYPE::FLY)
				CEffect_Manager::Get_Instance()->Fetch(VFX::AFTERIMAGE, dynamic_pointer_cast<CGameObject>(pOwner));
		}
	}
	else
	{
		m_pTargetMonster = nullptr;
		m_bOnBattle = false;

		GotoState(m_pState_Battle_Idle);
	}

	pOwner->Motion(fTimeDelta);
}

void CArashiAI::EndState_Battle_Chase()
{
	m_pOwner.lock()->Reset_Motion();
	m_fAttackDelay = 0.f;
	m_bAfterImageEffect = false;
}

/*==================================== Attack ====================================*/
void CArashiAI::BeginState_Attack_Dodge()
{
	auto pOwner = m_pOwner.lock();
	auto pTransform = m_pTransform.lock();

	pOwner->Init_RootMotion();

	_bool bAlive = !m_pTargetMonster->GetDead();
	if (bAlive)
	{
		auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));
		_float3 vMonsterPosition = XMVectorSetY(pMonsterTransform->Get_State(TRANSFORM::POSITION), 0.f);

		pTransform->LookAt_Interpolation(vMonsterPosition, true);
		m_pModel.lock()->Set_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_F_START), 2.f, false, 0.2f, false);

		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Arasi_Dodge.wav"), TEXT("Voice_Arasi_Dodge1.wav") }), 1.f);
	}
	else
	{
		m_pTargetMonster = nullptr;
		GotoState(m_pState_Battle_Idle);
	}
}

void CArashiAI::ExecuteState_Attack_Dodge(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	auto pOwnerTransform = m_pTransform.lock();
	auto pModel = m_pModel.lock();

	if (m_pTargetMonster)
	{
		_bool bAlive = !m_pTargetMonster->GetDead();
		if (bAlive)
		{
			pOwner->Update_Motion(fTimeDelta);

			auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));
			_float3 vMonsterPosition = XMVectorSetY(pMonsterTransform->Get_State(TRANSFORM::POSITION), 0.f);
			pOwnerTransform->LookAt_Interpolation(vMonsterPosition, true);

			if (pModel->Is_AnimationFinished(IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_F_START)))
			{
				GotoState(m_pState_Attack);
			}

			pOwner->RootMotion(fTimeDelta);
		}
		else
		{
			GotoState(m_pState_Battle_Idle);
		}
	}
	else
	{
		GotoState(m_pState_Battle_Idle);
	}
}

void CArashiAI::EndState_Attack_Dodge()
{
	m_pOwner.lock()->Reset_Motion();

	auto pModel = m_pModel.lock();
	pModel->Reset_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_F_START));
	//pModel->Reset_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_A1));
}

/*==================================== Attack ====================================*/
void CArashiAI::BeginState_Attack()
{
	auto pOwner = m_pOwner.lock();
	auto pTransform = m_pTransform.lock();

	m_iAttackAnimIndex = Function::Random({ IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_A2), IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_B2),
		/*IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_A3_F_DOWN_L), */IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_A3_R_UP) });

	m_pModel.lock()->Reset_Animation(m_iAttackAnimIndex);
	m_pModel.lock()->Set_Animation(m_iAttackAnimIndex, 2.f, false, 0.2f, false);

	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Arasi_Attack.wav"), TEXT("Voice_Arasi_Attack0.wav"),
		TEXT("Voice_Arasi_Attack01.wav"), TEXT("Voice_Arasi_Attack2.wav"), TEXT("Voice_Arasi_Attack3.wav"), TEXT("Voice_Arasi_Attack4.wav"),
		TEXT("Voice_Arasi_Attack5.wav"), TEXT("Voice_Arasi_Attack6.wav"), TEXT("Voice_Arasi_Attack7.wav") }), 1.f);

	pOwner->Init_RootMotion();
	pOwner->Active_Weapon(true);
}

void CArashiAI::ExecuteState_Attack(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	auto pTransform = m_pTransform.lock();
	auto pModel = m_pModel.lock();

	pOwner->Update_Motion(fTimeDelta);

	_bool bAlive = !m_pTargetMonster->GetDead();
	if (bAlive)
	{
		if (false == m_bExecuteAttack)
		{
			m_bExecuteAttack = true;

			auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));
			_float3 vMonsterPosition = pMonsterTransform->Get_State(TRANSFORM::POSITION);
			_float3 vOwnerPosition = pTransform->Get_State(TRANSFORM::POSITION);

			_vector vDirection = XMVector3Normalize(vOwnerPosition - vMonsterPosition) * 0.15f;
			pTransform->LookAt(vMonsterPosition, true);
			pTransform->Set_State(TRANSFORM::POSITION, vMonsterPosition + vDirection);
		}

		if (pModel->Is_AnimationFinished(m_iAttackAnimIndex))
		{
			_uint iPrevAttackAnimIndex = m_iAttackAnimIndex;
			switch ((ANIMATION::ARASHI_TEAMMATE)m_iAttackAnimIndex)
			{
			case ANIMATION::ARASHI_TEAMMATE::AL_ATK_A3_F_DOWN_L:
				pOwner->Reset_Motion();

				m_iAttackAnimIndex = IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_A3_L_UP);
				pModel->Reset_Animation(m_iAttackAnimIndex);
				pModel->Set_Animation(m_iAttackAnimIndex, 2.f, false, 0.2f, false);

				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Arasi_Attack.wav"), TEXT("Voice_Arasi_Attack0.wav"),
					TEXT("Voice_Arasi_Attack01.wav"), TEXT("Voice_Arasi_Attack2.wav"), TEXT("Voice_Arasi_Attack3.wav"), TEXT("Voice_Arasi_Attack4.wav"),
					TEXT("Voice_Arasi_Attack5.wav"), TEXT("Voice_Arasi_Attack6.wav"), TEXT("Voice_Arasi_Attack7.wav") }), 1.f);
				break;
			case ANIMATION::ARASHI_TEAMMATE::AL_ATK_A3_R_UP:
				pOwner->Reset_Motion();

				m_iAttackAnimIndex = IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_A3_L_DOWN);
				pModel->Reset_Animation(m_iAttackAnimIndex);
				pModel->Set_Animation(m_iAttackAnimIndex, 2.f, false, 0.2f, false);

				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Arasi_Attack.wav"), TEXT("Voice_Arasi_Attack0.wav"),
					TEXT("Voice_Arasi_Attack01.wav"), TEXT("Voice_Arasi_Attack2.wav"), TEXT("Voice_Arasi_Attack3.wav"), TEXT("Voice_Arasi_Attack4.wav"),
					TEXT("Voice_Arasi_Attack5.wav"), TEXT("Voice_Arasi_Attack6.wav"), TEXT("Voice_Arasi_Attack7.wav") }), 1.f);
				break;
			case ANIMATION::ARASHI_TEAMMATE::AL_ATK_A3_L_UP:
				pOwner->Reset_Motion();

				m_iAttackAnimIndex = IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_A3_R_SIDE);
				pModel->Reset_Animation(m_iAttackAnimIndex);
				pModel->Set_Animation(m_iAttackAnimIndex, 2.f, false, 0.2f, false);

				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Arasi_Attack.wav"), TEXT("Voice_Arasi_Attack0.wav"),
					TEXT("Voice_Arasi_Attack01.wav"), TEXT("Voice_Arasi_Attack2.wav"), TEXT("Voice_Arasi_Attack3.wav"), TEXT("Voice_Arasi_Attack4.wav"),
					TEXT("Voice_Arasi_Attack5.wav"), TEXT("Voice_Arasi_Attack6.wav"), TEXT("Voice_Arasi_Attack7.wav") }), 1.f);
				break;
			default:
				GotoState(m_pState_Attack_Escape);
				break;
			}

			pModel->Reset_Animation(iPrevAttackAnimIndex);
		}
	}
	else
	{
		m_pTargetMonster = nullptr;
		GotoState(m_pState_Battle_Idle);
	}
	
	pOwner->Motion(fTimeDelta);
}

void CArashiAI::EndState_Attack()
{
	m_bExecuteAttack = false;

	m_pOwner.lock()->Reset_Motion();
	m_pOwner.lock()->Active_Weapon(false);
}

void CArashiAI::BeginState_Attack_Escape()
{
	auto pOwner = m_pOwner.lock();

	m_iEscapeStartAnimIndex = Function::Random({ IDX(ANIMATION::ARASHI_TEAMMATE::AL_DODGE_B_START_BATTLE), IDX(ANIMATION::ARASHI_TEAMMATE::AL_DODGE_L_START_BATTLE), IDX(ANIMATION::ARASHI_TEAMMATE::AL_DODGE_R_START_BATTLE) });
	switch ((ANIMATION::ARASHI_TEAMMATE)m_iEscapeStartAnimIndex)
	{
	case ANIMATION::ARASHI_TEAMMATE::AL_DODGE_B_START_BATTLE:
		m_iEscapeStopAnimIndex = IDX(ANIMATION::ARASHI_TEAMMATE::AL_DODGE_B_STOP_BATTLE);
		break;
	case ANIMATION::ARASHI_TEAMMATE::AL_DODGE_L_START_BATTLE:
		m_iEscapeStopAnimIndex = IDX(ANIMATION::ARASHI_TEAMMATE::AL_DODGE_L_STOP_BATTLE);
		break;
	case ANIMATION::ARASHI_TEAMMATE::AL_DODGE_R_START_BATTLE:
		m_iEscapeStopAnimIndex = IDX(ANIMATION::ARASHI_TEAMMATE::AL_DODGE_R_STOP_BATTLE);
		break;
	default:
		break;
	}

	m_pModel.lock()->Set_Animation(m_iEscapeStartAnimIndex, 2.f, false, 0.2f, false);

	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Arasi_Dodge.wav"), TEXT("Voice_Arasi_Dodge1.wav") }), 1.f);

	pOwner->Init_RootMotion();
}

void CArashiAI::ExecuteState_Attack_Escape(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	pOwner->Update_Motion(fTimeDelta);

	if (m_pTargetMonster)
	{
		auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));
		_float3 vMonsterPosition = XMVectorSetY(pMonsterTransform->Get_State(TRANSFORM::POSITION), 0.f);
		m_pTransform.lock()->LookAt_Interpolation(vMonsterPosition, true);
	}

	pOwner->RootMotion(fTimeDelta);

	auto pModel = m_pModel.lock();
	if (pModel->Is_AnimationFinished(m_iEscapeStartAnimIndex))
	{
		m_bExecuteEscape = true;
		m_pModel.lock()->Reset_Animation(m_iEscapeStartAnimIndex);

		pOwner->Reset_Motion();
		pModel->Set_Animation(m_iEscapeStopAnimIndex, 2.f, false, 0.2f, false);
	}

	if (m_bExecuteEscape && pModel->Is_AnimationFinished(m_iEscapeStopAnimIndex))
	{
		if (m_pTargetMonster)
		{
			_bool bAlive = !m_pTargetMonster->GetDead();
			if (bAlive)
			{
				GotoState(m_pState_Battle_Chase);
			}
			else
			{
				GotoState(m_pState_Battle_Idle);
			}
		}
		else
		{
			GotoState(m_pState_Battle_Idle);
		}
	}
}

void CArashiAI::EndState_Attack_Escape()
{
	m_pModel.lock()->Reset_Animation(m_iEscapeStartAnimIndex);
	m_pModel.lock()->Reset_Animation(m_iEscapeStopAnimIndex);
	m_pOwner.lock()->Reset_Motion();

	m_bExecuteEscape = false;
}

void CArashiAI::BeginState_SAS()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Player());
	auto pPlayerCamera = pPlayer->Get_Camera();

	auto pMonster = pPlayerCamera->Get_LockedMonster().lock();
	if (pMonster)
	{
		m_pTargetMonster = pMonster;

		auto pModel = m_pModel.lock();
		pModel->Set_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_SAS_START), 2.f, false, 0.f, false);

		CEffect_Manager::Get_Instance()->Fetch(VFX::AFTERIMAGE, dynamic_pointer_cast<CGameObject>(m_pOwner.lock()));
	}
	else
	{
		GotoState(m_pState_Idle);
	}
}

void CArashiAI::ExecuteState_SAS(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	auto pTransform = m_pTransform.lock();
	auto pModel = m_pModel.lock();

	pOwner->Update_Motion(fTimeDelta);

	auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));
	pTransform->Set_Matrix(pMonsterTransform->Get_Matrix());

	pOwner->Motion(fTimeDelta);

	if (pModel->Is_AnimationFinished(IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_SAS_START)))
	{
		pModel->Reset_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_ATK_SAS_START));

		GotoState(m_pState_Idle);
	}
}

void CArashiAI::EndState_SAS()
{
	m_pOwner.lock()->Reset_Motion();
}

#pragma endregion

shared_ptr<CTeammateAI> CArashiAI::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	shared_ptr<CArashiAI> pInstance = make_private_shared(CArashiAI);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CArashiAI::Create", "Failed to Initialize");
	}

	return pInstance;
}
