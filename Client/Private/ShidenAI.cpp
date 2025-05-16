#include "ClientPCH.h"
#include "ShidenAI.h"
#include "Shiden_Teammate.h"
#include "Monster.h"

#include "GameInstance.h"
#include "Effect_Manager.h"

#include "Player.h"
#include "Camera_Player.h"
#include "PlayerState.h"

HRESULT CShidenAI::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> pOwner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(E_FAIL, "CArashiAI::Initialize", "Failed to CTeammateAI::Initialize");
	}

	shared_ptr<CShidenAI> pInstance = static_pointer_cast<CShidenAI>(shared_from_this());

	m_pState_Battle_Idle = make_shared<CStateTemplate<CShidenAI>>();
	m_pState_Battle_Idle->Set(pInstance, &CShidenAI::BeginState_Battle_Idle, &CShidenAI::ExecuteState_Battle_Idle, &CShidenAI::EndState_Battle_Idle);
	m_pState_Battle_Chase = make_shared<CStateTemplate<CShidenAI>>();
	m_pState_Battle_Chase->Set(pInstance, &CShidenAI::BeginState_Battle_Chase, &CShidenAI::ExecuteState_Battle_Chase, &CShidenAI::EndState_Battle_Chase);
	m_pState_Attack_Dodge = make_shared<CStateTemplate<CShidenAI>>();
	m_pState_Attack_Dodge->Set(pInstance, &CShidenAI::BeginState_Attack_Dodge, &CShidenAI::ExecuteState_Attack_Dodge, &CShidenAI::EndState_Attack_Dodge);
	m_pState_Attack = make_shared<CStateTemplate<CShidenAI>>();
	m_pState_Attack->Set(pInstance, &CShidenAI::BeginState_Attack, &CShidenAI::ExecuteState_Attack, &CShidenAI::EndState_Attack);
	m_pState_Attack_Escape = make_shared<CStateTemplate<CShidenAI>>();
	m_pState_Attack_Escape->Set(pInstance, &CShidenAI::BeginState_Attack_Escape, &CShidenAI::ExecuteState_Attack_Escape, &CShidenAI::EndState_Attack_Escape);

	m_wstrJumpSound = TEXT("Voice_Siden_Jump.wav");
	m_wstrLandingSound = TEXT("");

	return S_OK;
}

#pragma region Default State

void CShidenAI::BeginState_Idle()
{
	m_iWaitAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_WAIT01_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_WAIT01);
	__super::BeginState_Idle();
}

void CShidenAI::ExecuteState_Idle(_float fTimeDelta)
{
	__super::ExecuteState_Idle(fTimeDelta);
}

void CShidenAI::EndState_Idle()
{
	__super::EndState_Idle();
}

void CShidenAI::BeginState_Chase()
{
	m_iWalkAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_WALK_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_WALK);
	__super::BeginState_Chase();
}

void CShidenAI::ExecuteState_Chase(_float fTimeDelta)
{
	m_iRunAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_RUN_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_RUN);
	m_iDashAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DASH_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DASH);

	__super::ExecuteState_Chase(fTimeDelta);
}

void CShidenAI::EndState_Chase()
{
	__super::EndState_Chase();
}

void CShidenAI::BeginState_Jump()
{
	m_iJumpStartAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_START_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_START);
	m_iJumpLandingAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_LANDING_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_LANDING);
	__super::BeginState_Jump();
}

void CShidenAI::ExecuteState_Jump(_float fTimeDelta)
{
	m_iJumpStartAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_START_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_START);
	m_iJumpRiseAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_RISE_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_RISE);
	m_iJumpFallAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_FALL_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_FALL);
	m_iJumpLandingAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_LANDING_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_LANDING);
	__super::ExecuteState_Jump(fTimeDelta);
}

void CShidenAI::EndState_Jump()
{
	m_iJumpStartAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_START_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_START);
	m_iJumpRiseAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_RISE_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_RISE);
	m_iJumpFallAnimIndex = m_bBattleMode ? IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_FALL_BATTLE) : IDX(ANIMATION::SHIDEN_TEAMMATE::AL_JUMP_FALL);
	__super::EndState_Jump();
}

void CShidenAI::BeginState_Hit()
{
	auto pOwner = m_pOwner.lock();
	auto pModel = m_pModel.lock();

	pModel->Reset_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DAMAGE_L_FL));
	pModel->Set_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DAMAGE_L_FL), 2.f, false, 0.2f, false);

	pOwner->Init_RootMotion();

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Siden_Damaged.wav"), 5.f);
}

void CShidenAI::ExecuteState_Hit(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	pOwner->Update_Motion(fTimeDelta);
	pOwner->RootMotion(fTimeDelta);

	auto pModel = m_pModel.lock();
	if (pModel->Is_AnimationFinished(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DAMAGE_L_FL)))
	{
		GotoState(m_pState_Attack_Escape);
	}
}

void CShidenAI::EndState_Hit()
{
	auto pModel = m_pModel.lock();
	pModel->Reset_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DAMAGE_L_FL));

	m_pOwner.lock()->Reset_Motion();
}

void CShidenAI::BeginState_Trans_Battle()
{
	m_iUpTransBattleAnimIndex = IDX(ANIMATION::SHIDEN_TEAMMATE::UP_TRANS_BATTLE);
	__super::BeginState_Trans_Battle();
}

void CShidenAI::ExecuteState_Trans_Battle(_float fTimeDelta)
{
	m_iUpTransBattleAnimIndex = IDX(ANIMATION::SHIDEN_TEAMMATE::UP_TRANS_BATTLE);
	__super::ExecuteState_Trans_Battle(fTimeDelta);
}

void CShidenAI::EndState_Trans_Battle()
{
	__super::EndState_Trans_Battle();
}

void CShidenAI::ExecutePersistent(_float fTimeDelta)
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
			auto pMonsterTrasnform = dynamic_pointer_cast<CTransform>((*Iter)->Get_Component(COMPONENT::TRANSFORM));

			_float3 vMonsterPosition = pMonsterTrasnform->Get_State(TRANSFORM::POSITION);
			_float3 vPosition = pTransform->Get_State(TRANSFORM::POSITION);

			if (XMVectorGetX(XMVector3Length(vMonsterPosition - vPosition)) <= m_fExecuteBattleDistanceThreshold)
			{
				m_bOnBattle = true;
				m_pTargetMonster = dynamic_pointer_cast<CMonster>(*Iter);
				GotoState(m_pState_Battle_Idle);

				break;
			}

			++Iter;
		}
	}

	if (m_pActorDesc->bHit && *m_pActorDesc->stHitDesc.AttackColliderActive)
	{
		m_pActorDesc->bHit = false;
		GotoState(m_pState_Hit);
	}
}

#pragma endregion

void CShidenAI::BeginState_Battle_Idle()
{
	m_pModel.lock()->Set_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_WAIT01_BATTLE), 2.f, false, 0.2f, true);
	m_pOwner.lock()->Init_RootMotion();

	m_fAttackDelay = 0.f;
}

void CShidenAI::ExecuteState_Battle_Idle(_float fTimeDelta)
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
		}
		else
		{
			m_pTargetMonster = nullptr;
			m_bOnBattle = false;

			GotoState(m_pState_Idle);
		}
	}
}

void CShidenAI::EndState_Battle_Idle()
{
	m_pOwner.lock()->Reset_Motion();
}

void CShidenAI::BeginState_Battle_Chase()
{
	auto pOwner = m_pOwner.lock();
	m_pModel.lock()->Set_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_RUN_BATTLE), 2.f, false, 0.2f, true);

	pOwner->Init_RootMotion();

	m_fAngle = Function::RandomFloat(0.f, XM_2PI);
	m_fAttackInterval = Function::RandomFloat(6.f, 15.f);

	m_bArrived = true;
	m_bWaiting = false;
	m_fChaseDelay = 0.f;
}

void CShidenAI::ExecuteState_Battle_Chase(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	auto pModel = m_pModel.lock();
	pOwner->Update_Motion(fTimeDelta);

	_bool bAlive = !m_pTargetMonster->GetDead();
	if (bAlive)
	{
		auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));

		auto pTransform = m_pTransform.lock();
		_float3 vOwnerOriginPosition = pTransform->Get_State(TRANSFORM::POSITION);
		_float3 vOwnerPosition = XMVectorSetY(vOwnerOriginPosition, 0.f);
		_float3 vMonsterPosition = XMVectorSetY(pMonsterTransform->Get_State(TRANSFORM::POSITION), 0.f);

		if (m_bArrived)
		{
			m_bArrived = false;

			m_vNextPosition.x = vMonsterPosition.x + m_fChaseRadius * cos(m_fAngle);
			m_vNextPosition.y = vOwnerOriginPosition.y;
			m_vNextPosition.z = vMonsterPosition.z + m_fChaseRadius * sin(m_fAngle);
			m_vNextPosition.w = 1.f;
		}

		/*_vector vDirection = m_vNextPosition - pTransform->Get_State(TRANSFORM::POSITION);*/
		_float fDistance = XMVectorGetX(XMVector3Length(m_vNextPosition - vOwnerOriginPosition));
		if (fDistance >= m_fBattleChaseDistanceThreshold)
		{
			pTransform->LookAt_Interpolation(m_vNextPosition, true);
			pTransform->Translate(XMVector3Normalize(m_vNextPosition - pTransform->Get_State(TRANSFORM::POSITION)) * m_fRunSpeed * fTimeDelta);
		}
		else
		{
			pTransform->LookAt_Interpolation(vMonsterPosition, true);

			if (false == m_bWaiting)
			{
				m_bWaiting = true;

				pOwner->Reset_Motion();
				pModel->Set_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_WAIT01_BATTLE), 2.f, false, 0.2f, true);
			}

			m_fChaseDelay += fTimeDelta;
			if (m_fChaseDelay >= m_fChaseInterval)
			{
				m_bArrived = true;
				m_bWaiting = false;
				m_fChaseDelay = 0.f;

				m_fAngle = Function::RandomFloat(0.f, XM_2PI);

				pOwner->Reset_Motion();
				pModel->Set_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_RUN_BATTLE), 2.f, false, 0.2f, true);
			}
		}

		if (false == pOwner->Get_Transparency())
			m_fAttackDelay += fTimeDelta;

		if (m_fAttackDelay >= m_fAttackInterval)
		{
			m_fAttackDelay = 0.f;
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
	}
	else
	{
		m_pTargetMonster = nullptr;
		m_bOnBattle = false;

		GotoState(m_pState_Battle_Idle);
	}

	pOwner->Motion(fTimeDelta);
}

void CShidenAI::EndState_Battle_Chase()
{
	m_pOwner.lock()->Reset_Motion();
}

void CShidenAI::BeginState_Attack_Dodge()
{
	auto pOwner = m_pOwner.lock();
	auto pTransform = m_pTransform.lock();

	_bool bAlive = !m_pTargetMonster->GetDead();
	if (bAlive)
	{
		auto pMonsterTransform = dynamic_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));
		_float3 vMonsterPosition = XMVectorSetY(pMonsterTransform->Get_State(TRANSFORM::POSITION), 0.f);

		pTransform->LookAt_Interpolation(vMonsterPosition, true);
		m_pModel.lock()->Set_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_F_START_BATTLE), 2.f, false, 0.2f, false);
		
		pOwner->Init_RootMotion();

		CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Siden_Dodge.wav"), 1.f);

		m_bDodgeStartEnded = false;
	}
	else
	{
		m_pTargetMonster = nullptr;
		GotoState(m_pState_Battle_Idle);
	}
}

void CShidenAI::ExecuteState_Attack_Dodge(_float fTimeDelta)
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

			if (false == m_bDodgeStartEnded && pModel->Is_AnimationFinished(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_F_START_BATTLE)))
			{
				m_bDodgeStartEnded = true;

				GotoState(m_pState_Attack);

				m_pOwner.lock()->Reset_Motion();
				m_pModel.lock()->Set_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_F_STOP_BATTLE), 2.f, false, 0.2f, false);
			}

			/*if (pModel->Is_AnimationFinished(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_F_STOP_BATTLE)))
			{
				GotoState(m_pState_Attack);
			}*/

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

void CShidenAI::EndState_Attack_Dodge()
{
	m_pOwner.lock()->Reset_Motion();

	auto pModel = m_pModel.lock();
	pModel->Reset_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_F_START_BATTLE));
	pModel->Reset_Animation(IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_F_STOP_BATTLE));
}

void CShidenAI::BeginState_Attack()
{
	auto pOwner = m_pOwner.lock();
	auto pTransform = m_pTransform.lock();

	m_iAttackAnimIndex = Function::Random({ IDX(ANIMATION::SHIDEN_TEAMMATE::AL_ATK_A1), IDX(ANIMATION::SHIDEN_TEAMMATE::AL_ATK_A2), IDX(ANIMATION::SHIDEN_TEAMMATE::AL_ATK_A3) });

	m_pModel.lock()->Set_Animation(m_iAttackAnimIndex, 2.f, false, 0.2f, false);
	pOwner->Init_RootMotion();

	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Siden_Attack0.wav"), TEXT("Voice_Siden_Attack1.wav"), TEXT("Voice_Siden_Attack2.wav") }), 1.f);

	pOwner->Active_Weapon(true);
}

void CShidenAI::ExecuteState_Attack(_float fTimeDelta)
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
			GotoState(m_pState_Attack_Escape);
		}
	}
	else
	{
		m_pTargetMonster = nullptr;
		GotoState(m_pState_Battle_Idle);
	}

	pOwner->Motion(fTimeDelta);
}

void CShidenAI::EndState_Attack()
{
	m_bExecuteAttack = false;
	m_pModel.lock()->Reset_Animation(m_iAttackAnimIndex);
	m_pOwner.lock()->Reset_Motion();

	m_pOwner.lock()->Active_Weapon(false);
}

void CShidenAI::BeginState_Attack_Escape()
{
	auto pOwner = m_pOwner.lock();

	m_iEscapeStartAnimIndex = Function::Random({ IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_B_START_BATTLE), IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_L_START_BATTLE), 
		IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_R_START_BATTLE) });
	switch ((ANIMATION::SHIDEN_TEAMMATE)m_iEscapeStartAnimIndex)
	{
	case ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_B_START_BATTLE:
		m_iEscapeStopAnimIndex = IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_B_STOP_BATTLE);
		break;
	case ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_L_START_BATTLE:
		m_iEscapeStopAnimIndex = IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_L_STOP_BATTLE);
		break;
	case ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_R_START_BATTLE:
		m_iEscapeStopAnimIndex = IDX(ANIMATION::SHIDEN_TEAMMATE::AL_DODGE_R_STOP_BATTLE);
		break;
	default:
		break;
	}

	m_pModel.lock()->Set_Animation(m_iEscapeStartAnimIndex, 2.f, false, 0.2f, false);

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Siden_Dodge.wav"), 1.f);

	pOwner->Init_RootMotion();
}

void CShidenAI::ExecuteState_Attack_Escape(_float fTimeDelta)
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

void CShidenAI::EndState_Attack_Escape()
{
	m_pModel.lock()->Reset_Animation(m_iEscapeStartAnimIndex);
	m_pModel.lock()->Reset_Animation(m_iEscapeStopAnimIndex);
	m_pOwner.lock()->Reset_Motion();

	m_bExecuteEscape = false;
}

void CShidenAI::BeginState_SAS()
{
}

void CShidenAI::ExecuteState_SAS(_float fTimeDelta)
{
}

void CShidenAI::EndState_SAS()
{
}

shared_ptr<CTeammateAI> CShidenAI::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	shared_ptr<CShidenAI> pInstance = make_private_shared(CShidenAI);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CShidenAI::Create", "Failed to Initialize");
	}

	return pInstance;
}
