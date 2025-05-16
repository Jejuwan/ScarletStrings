#include "ClientPCH.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerAttackState.h"
#include "PlayerThrowState.h"
#include "PlayerExecuteState.h"
#include "PlayerDriveModeState.h"
#include "PlayerHealState.h"
#include "PlayerDodgeState.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Bone.h"
#include "Model.h"
#include "Psychokinesis.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "Map_Tutorial_Dynamic.h"
#include "Map_Special_Dynamic.h"
#include "PlayerSpecialThrowState.h"
#include "Monster.h"
#include "Effect_Manager.h"
#include "PlayerBrainFieldStartState.h"

CPlayerWalkState::CPlayerWalkState()
{
}

CPlayerWalkState::CPlayerWalkState(PLAYER_DIR _eDir,_bool bRunning,_bool bDash)
{
	m_eDir = _eDir;
	m_bRunning = bRunning;
	m_bDash = bDash;
}

shared_ptr<CPlayerState> CPlayerWalkState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (m_fTime < 0.1f && 
		pGameInstance->Key_Hold(VK_SHIFT))
	{
		if (pGameInstance->Key_Hold('W'))
			return make_shared<CPlayerDodgeState>(PLAYER_DIR::F);
		else if (pGameInstance->Key_Hold('A'))
			return make_shared<CPlayerDodgeState>(PLAYER_DIR::L);
		else if (pGameInstance->Key_Hold('S'))
			return make_shared<CPlayerDodgeState>(PLAYER_DIR::B);
		else if (pGameInstance->Key_Hold('D'))
			return make_shared<CPlayerDodgeState>(PLAYER_DIR::R);
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_Dash00.wav"),TEXT("Voice_Kasane_Dash01.wav") ,TEXT("Voice_Kasane_Dash02.wav") }), g_fPlayerSound);
	}

	if (pGameInstance->Key_Down(VK_SPACE))
	{
		if(_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN_START_F))
			return make_shared<CPlayerJumpState>(CPlayerJumpState::JUMP_TYPE::RUN);
		else if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH))
			return make_shared<CPlayerJumpState>(CPlayerJumpState::JUMP_TYPE::DASH);
		else 
			return make_shared<CPlayerJumpState>(CPlayerJumpState::JUMP_TYPE::INPLACE);
	}

	if (pGameInstance->Key_Down(VK_LBUTTON))
	{
		if (_player.Get_PlayerDesc().bBrainField)
		{
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::BRAINFIELD);
		}
		else
		{
			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH))
				return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::DASH);
			if (!_player.Get_Kinesis()->Get_ThrowHit())
				return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::NORMAL);
			else
				return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::FOLLOW);
		}
	}

	if (pGameInstance->Key_Down('C'))
		return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::BACKSTEP);

	if (pGameInstance->Key_Down('F'))
		return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::UPPER);

	weak_ptr<CMonster> PickedMonster = _player.Get_Camera()->Get_LockedMonster();
	if (PickedMonster.lock() != nullptr)
	{
		if (pGameInstance->Key_Down('G') && PickedMonster.lock()->Get_BcChance() && _player.Get_Camera()->Get_LockOnTransform() != nullptr)
			return make_shared<CPlayerExecuteState>();
	}

	if (nullptr != _player.Get_Kinesis()->Get_CapturedObject())
	{
		if (shared_ptr<CMap_Special_Dynamic> pSpecial = dynamic_pointer_cast<CMap_Special_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
		{
			if (pGameInstance->Key_Hold('G'))
			{
				return make_shared<CPlayerSpecialThrowState>(pSpecial->Get_Type());
				_player.Get_Kinesis()->Set_CapturedObject(nullptr);
			}
		}
		else if (nullptr != dynamic_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
		{
			if (_player.Get_PlayerDesc().fKinesisGauge > 20.f)
			{
				if (pGameInstance->Key_Hold(VK_RBUTTON))
					return make_shared<CPlayerThrowState>();
			}
			else if (pGameInstance->Key_Hold(VK_RBUTTON))
				CGameInstance::Get_Instance()->PlaySoundeffect(TEXT("UI_SFX_Kasane_No_PsycoGauge.wav"), CSound_Manager::SOUND_CHARGE, g_fPlayerSound);
		}
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_STOP))
	{
		if (pGameInstance->Key_Down('W'))
		{
			if (pGameInstance->Key_Down('A'))
			{
				return make_shared<CPlayerWalkState>(PLAYER_DIR::FL);
			}
			if (pGameInstance->Key_Down('D'))
			{
				return make_shared<CPlayerWalkState>(PLAYER_DIR::FR);
			}
			return make_shared<CPlayerWalkState>(PLAYER_DIR::F);
		}
		if (pGameInstance->Key_Down('S'))
		{
			if (pGameInstance->Key_Down('A'))
			{
				return make_shared<CPlayerWalkState>(PLAYER_DIR::BL);
			}
			if (pGameInstance->Key_Down('D'))
			{
				return make_shared<CPlayerWalkState>(PLAYER_DIR::BR);
			}
			return make_shared<CPlayerWalkState>(PLAYER_DIR::B);
		}
		if (pGameInstance->Key_Down('A'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::L);
		}
		if (pGameInstance->Key_Down('D'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::R);
		}
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DASH_STOP))
	{
		return make_shared<CPlayerIdleState>();
	}

	if (pGameInstance->Key_Down('Z'))
	{
		if (_player.Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH)
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::CHARGE);
		else if(true == _player.Get_PlayerDesc().AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::ONECLICKCHARGE])
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::ONECLICKCHARGE);
	}

	if (pGameInstance->Key_Down('R') && !_player.Get_PlayerDesc().bDriveMode)
		return make_shared<CPlayerDriveModeState>();

	if (pGameInstance->Key_Down('T'))
	{
		shared_ptr<CTransform> pPlayerTramsform =
			CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

		_float3 PlayerPos = pPlayerTramsform->Get_State(TRANSFORM::POSITION);
		_float3 PlayerUp = pPlayerTramsform->Get_State(TRANSFORM::UP);

		_float3 Pos = PlayerPos;
		_float3 Pos2 = PlayerPos;
		Pos.y += 2.5f;
		Pos2.y += 1.25f;

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::ITEM_USE,
			initializer_list<any>{Pos, Pos, Pos2, Pos2, Pos2 });

		return make_shared<CPlayerHealState>();
	}

	if (pGameInstance->Key_Down('7') && _player.Get_PlayerDesc().bBrainField)
	{
		return make_shared<CPlayerSpecialThrowState>(SPECIAL_THROW::MINDROOM);
	}

	if (pGameInstance->Current_Scene() == SCENE::MINDROOM)
	{
		if (!_player.Get_PlayerDesc().bBrainField && pGameInstance->Key_Down('B'))
			return make_shared<CPlayerBrainFieldStartState>();
		else if (_player.Get_PlayerDesc().bBrainField && pGameInstance->Key_Down('B'))
			_player.Exit_BrainField();
	}

    return nullptr;
}

void CPlayerWalkState::Entry(CPlayer & _player)
{
	__super::Entry(_player);

	if (!m_bRunning)
	{
		m_eAnim = ANIMATION::PLAYER::RUN_START_F;
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eAnim, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
	}
	else
	{
		if (!m_bDash)
			m_pModel->Set_NextAnimIndex((_uint)ANIMATION::PLAYER::RUN);
		else
			m_pModel->Set_NextAnimIndex((_uint)ANIMATION::PLAYER::DASH);
		m_eAnim = ANIMATION::PLAYER::MAX;

	}
	
}


void CPlayerWalkState::Update(CPlayer& _player, _float fTimeDelta)
{
	//_float3 vLook = m_pCameraTransform->Get_State(TRANSFORM::LOOK);
	//cout << vLook.x << '\t' << vLook.y << '\t' << vLook.z << endl;
	if (m_fTime > 0.1f && !m_bExecuted[99])
	{
		m_bExecuted[99] = true;
		switch (m_eDir)
		{
		case PLAYER_DIR::F:
			m_pTransform->LookTo(m_pCameraTransform->Get_State(TRANSFORM::LOOK));
			break;
		case PLAYER_DIR::L:
			m_pTransform->LookTo(-m_pCameraTransform->Get_State(TRANSFORM::RIGHT));
			break;
		case PLAYER_DIR::R:
			m_pTransform->LookTo(m_pCameraTransform->Get_State(TRANSFORM::RIGHT));
			break;
		case PLAYER_DIR::B:
			m_pTransform->LookTo(-m_pCameraTransform->Get_State(TRANSFORM::LOOK));
			break;
		case PLAYER_DIR::FL:
			m_pTransform->LookTo(m_pCameraTransform->Get_State(TRANSFORM::LOOK) - m_pCameraTransform->Get_State(TRANSFORM::RIGHT));
			break;
		case PLAYER_DIR::FR:
			m_pTransform->LookTo(m_pCameraTransform->Get_State(TRANSFORM::LOOK) + m_pCameraTransform->Get_State(TRANSFORM::RIGHT));
			break;
		case PLAYER_DIR::BL:
			m_pTransform->LookTo(-m_pCameraTransform->Get_State(TRANSFORM::LOOK) - m_pCameraTransform->Get_State(TRANSFORM::RIGHT));
			break;
		case PLAYER_DIR::BR:
			m_pTransform->LookTo(-m_pCameraTransform->Get_State(TRANSFORM::LOOK) + m_pCameraTransform->Get_State(TRANSFORM::RIGHT));
		}
	}

	__super::Update(_player, fTimeDelta);
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

#pragma region INTERPOLATE
	if(m_bRunning)
	{
		if (m_pModel->Get_InterpolateEnd() && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Lock(false);
			if (!m_bDash)
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::RUN, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, true);
			else
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, true);
		}
			else if(!m_bExecuted[0])
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Interpolate_Animation(fTimeDelta);
	}
#pragma endregion


#pragma region READY
	if(!m_bRunning)
	{
		/*if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eAnim))
		{
			if (m_eAnim == ANIMATION::PLAYER::RUN_START_F)
			{
				if (m_eDir == PLAYER_DIR::F)
					m_pTransform->LookTo(m_pCameraTransform->Get_State(TRANSFORM::LOOK));
				else if (m_eDir == PLAYER_DIR::L)
					m_pTransform->LookTo(-m_pCameraTransform->Get_State(TRANSFORM::RIGHT));
				else if (m_eDir == PLAYER_DIR::R)
					m_pTransform->LookTo(m_pCameraTransform->Get_State(TRANSFORM::RIGHT));
				else if (m_eDir == PLAYER_DIR::B)
					m_pTransform->LookTo(-m_pCameraTransform->Get_State(TRANSFORM::LOOK));
			}
		}

		if (m_eAnim == ANIMATION::PLAYER::RUN_START_L && !m_bExecuted[0])
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Interpolate_Animation(fTimeDelta);
			if (m_pModel->Get_InterpolateEnd())
			{
				m_bExecuted[0] = true;
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Lock(false);
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::RUN, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, true);
			}
		}*/
		/*else */if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eAnim) /*|| m_pModel->Get_Proportion() >= m_fRunStartSkipProportion) && !m_bExecuted[0]*/)
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::RUN, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, true);
		}

	}
#pragma endregion
	
#pragma region DASH_STOP
	if (!pGameInstance->Key_Hold('A') &&
		!pGameInstance->Key_Hold('D') &&
		!pGameInstance->Key_Hold('W') &&
		!pGameInstance->Key_Hold('S') &&
		m_bExecuted[65] && 
		!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_STOP))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH_STOP, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, false);
		m_vPreRootPos = _float4(0, 0, 0, 1);

		_player.Set_Dir(m_eDir);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_StopRunning.wav"), g_fPlayerSound);
	}
	m_bExecuted[65] = true;

#pragma endregion


#pragma region RUNNING

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN) ||
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH))
	{

		if (pGameInstance->Key_Hold(VK_SHIFT) && !m_bExecuted[2])
		{
			m_bExecuted[2] = true;
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DODGE_F_START, _player.Get_PlayerDesc().fAnimSpeed, false, 0.2f, false);
			m_eCurrentState = PLAYER_STATE::DASH;
		}
		_float3 vDir;
		if (pGameInstance->Key_Hold('W'))
		{
			if (pGameInstance->Key_Hold('A'))
			{
				vDir = m_pCameraTransform->Get_State(TRANSFORM::LOOK) - m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else if (pGameInstance->Key_Hold('D'))
			{
				vDir = m_pCameraTransform->Get_State(TRANSFORM::LOOK) + m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else if(pGameInstance->Key_Hold('S'))
			{
				vDir = -m_pCameraTransform->Get_State(TRANSFORM::LOOK);
			}
			else
			{
				vDir = m_pCameraTransform->Get_State(TRANSFORM::LOOK);
			}
		}
		else if (pGameInstance->Key_Hold('S'))
		{
			if (pGameInstance->Key_Hold('A'))
			{
				vDir = -m_pCameraTransform->Get_State(TRANSFORM::LOOK) - m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else if (pGameInstance->Key_Hold('D'))
			{
				vDir = -m_pCameraTransform->Get_State(TRANSFORM::LOOK) + m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else if (pGameInstance->Key_Hold('W'))
			{
				vDir = m_pCameraTransform->Get_State(TRANSFORM::LOOK);
			}
			else
				vDir = -m_pCameraTransform->Get_State(TRANSFORM::LOOK);
		}
		else if (pGameInstance->Key_Hold('A'))
		{
			if (pGameInstance->Key_Hold('S'))
			{
				vDir = -m_pCameraTransform->Get_State(TRANSFORM::LOOK); -m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else if (pGameInstance->Key_Hold('D'))
			{
				vDir = m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else if (pGameInstance->Key_Hold('W'))
			{
				vDir = m_pCameraTransform->Get_State(TRANSFORM::LOOK) - m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else
			{
				vDir = -m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
		}
		else if (pGameInstance->Key_Hold('D'))
		{
			if (pGameInstance->Key_Hold('S'))
			{
				vDir = -m_pCameraTransform->Get_State(TRANSFORM::LOOK) + m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else if (pGameInstance->Key_Hold('A'))
			{
				vDir = -m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else if (pGameInstance->Key_Hold('W'))
			{
				vDir = m_pCameraTransform->Get_State(TRANSFORM::LOOK) + m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
			else
			{
				vDir = m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
			}
		}
		m_pTransform->LookTo(XMVectorLerp(m_pTransform->Get_State(TRANSFORM::LOOK), vDir, 0.1f));
	}
#pragma endregion
	

//#pragma region RUNTURN
//	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN_TURN))
//	{
//		//__super::Late_Update_Root(_player, fTimeDelta, -0.5f);
//		m_bTurnAnimPassed = true;
//		
//	}
//
//	//dir 180turn
//	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::RUN_TURN) && m_bTurnAnimPassed)
//	{
//		m_bTurnAnimPassed = false;
//		if (m_eDir == PLAYER_DIR::L)
//			m_eDir = PLAYER_DIR::R;
//		else if (m_eDir == PLAYER_DIR::R)
//			m_eDir = PLAYER_DIR::L;
//		else if (m_eDir == PLAYER_DIR::F)
//			m_eDir = PLAYER_DIR::B;
//		else if (m_eDir == PLAYER_DIR::B)
//			m_eDir = PLAYER_DIR::F;
//
//		m_pTransform->Rotate(TRANSFORM::UP, XMConvertToRadians(180.f));
//		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::RUN, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, true);
//	}
//#pragma endregion


#pragma region DODGE&DASH
	//dodge and dash
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DODGE_F_START) && !m_bExecuted[3])
	{
		m_bExecuted[3] = true;
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, true);
	}

#pragma endregion
		RayCastFloor(_player);


	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eAnim))
	{
		//_float3 vDir;
		//switch (m_eDir)
		//{
		//case PLAYER_DIR::F:
		//	vDir = m_pCameraTransform->Get_State(TRANSFORM::LOOK);
		//	break;
		//case PLAYER_DIR::L:
		//	vDir = -m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
		//	break;
		//case PLAYER_DIR::R:
		//	vDir = m_pCameraTransform->Get_State(TRANSFORM::RIGHT);
		//	break;
		//case PLAYER_DIR::B:
		//	vDir = -m_pCameraTransform->Get_State(TRANSFORM::LOOK);
		//	break;
		//}
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * (_player.Get_PlayerDesc().fSpeed * 5/7.f));
		__super::Late_Update(_player, fTimeDelta);
	}
	else if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_STOP))
	{
		__super::Late_Update_Root(_player, fTimeDelta, 1.f);
	}
	else if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK)* fTimeDelta * _player.Get_PlayerDesc().fSpeed);
		__super::Late_Update(_player, fTimeDelta);
	}
	else if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DODGE_F_START))
	{
		__super::Late_Update_Root(_player, fTimeDelta, 1.f);
	}
	else if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta* _player.Get_PlayerDesc().fDashSpeed);
		__super::Late_Update(_player, fTimeDelta);
	}
	else
		__super::Late_Update(_player, fTimeDelta);
}

void CPlayerWalkState::Exit(CPlayer& _player)
{
    __super::Exit(_player);
}

void CPlayerWalkState::RayCastFloor(CPlayer& _player)
{
	const PxU32 bufferSize = 128;        // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

	
	PxExtendedVec3 vPos = _player.Get_Controller()->getPosition();

	bool hasHit = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->raycast(
		PxVec3(vPos.x, vPos.y, vPos.z),
		PxVec3(0, -1, 0),
		300.f,
		buf);

	_float fMinDist = 10.f;

	if (hasHit) {
		PxU32 nbHits = buf.nbTouches;
		for (PxU32 i = 0; i < nbHits; ++i) {
			const PxRaycastHit& touch = buf.touches[i];
			ACTORDESC* pActorDesc = (ACTORDESC*)touch.actor->userData;
			if(nullptr == pActorDesc)
			{
				PxF32 fDist = touch.distance;
					if (fMinDist > fDist)
						fMinDist = fDist;
			}
		}
		if (!_player.Get_PlayerDesc().bRideStair)
		{
			if (fMinDist < 1.5f)
			{
				m_pTransform->Translate({ 0,-(fMinDist - 1.f),0.f });
			}
		}
		else
		{
			m_pTransform->Translate({ 0,-(fMinDist - 1.f),0.f });
		}
	}
}
