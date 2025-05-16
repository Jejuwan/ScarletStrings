#include "ClientPCH.h"
#include "PlayerJumpState.h"
#include "PlayerAttackState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerThrowState.h"
#include "GameInstance.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Model.h"
#include "Animator.h"
#include "Psychokinesis.h"

CPlayerJumpState::CPlayerJumpState(JUMP_TYPE eType)
{
	m_eJumpType = eType;
}

shared_ptr<CPlayerState> CPlayerJumpState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	switch (m_eJumpType)
	{
	case JUMP_TYPE::INPLACE:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_FALL)
			&&pGameInstance->Key_Hold(VK_RBUTTON) && nullptr!=_player.Get_Kinesis()->Get_CapturedObject())
		{
			if(_player.Get_PlayerDesc().fKinesisGauge > 20.f)
				return make_shared<CPlayerThrowState>(ANIMATION::PLAYER::CAPTURE_AIR_START);
			else
			{
				CGameInstance::Get_Instance()->PlaySoundeffect(TEXT("UI_SFX_Kasane_No_PsycoGauge.wav"), CSound_Manager::SOUND_CHARGE, g_fPlayerSound);
				_player.Get_Kinesis()->Set_CapturedObject(nullptr);
			}
	
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::JUMP_LANDING))
		{
			return make_shared<CPlayerIdleState>();
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_LANDING))
		{
			if (pGameInstance->Key_Hold('W'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::F, true);
			else if (pGameInstance->Key_Hold('A'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::L, true);
			else if (pGameInstance->Key_Hold('S'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::B, true);
			else if (pGameInstance->Key_Hold('D'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::R, true);
		}
		break;
	case JUMP_TYPE::RUN:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN_JUMP_FALL)
			&& pGameInstance->Key_Hold(VK_RBUTTON) && nullptr != _player.Get_Kinesis()->Get_CapturedObject())
		{
			if (_player.Get_PlayerDesc().fKinesisGauge > 20.f)
				return make_shared<CPlayerThrowState>(ANIMATION::PLAYER::CAPTURE_AIR_START);
			else
				CGameInstance::Get_Instance()->PlaySoundeffect(TEXT("UI_SFX_Kasane_No_PsycoGauge.wav"), CSound_Manager::SOUND_CHARGE, g_fPlayerSound);

		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::RUN_JUMP_LANDING))
		{
			return make_shared<CPlayerIdleState>();
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN_JUMP_LANDING))
		{
			if (pGameInstance->Key_Hold('W'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::F, true);
			else if (pGameInstance->Key_Hold('A'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::L, true);
			else if (pGameInstance->Key_Hold('S'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::B, true);
			else if (pGameInstance->Key_Hold('D'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::R, true);
		}
		break;
	case JUMP_TYPE::DASH:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_FALL)
			&& pGameInstance->Key_Hold(VK_RBUTTON) && nullptr != _player.Get_Kinesis()->Get_CapturedObject())
		{
			if (_player.Get_PlayerDesc().fKinesisGauge > 20.f)
				return make_shared<CPlayerThrowState>(ANIMATION::PLAYER::CAPTURE_AIR_START);
			else
				CGameInstance::Get_Instance()->PlaySoundeffect(TEXT("UI_SFX_Kasane_No_PsycoGauge.wav"), CSound_Manager::SOUND_CHARGE, g_fPlayerSound);

		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DASH_JUMP_LANDING))
		{
			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_FALL))
				return make_shared<CPlayerThrowState>(ANIMATION::PLAYER::CAPTURE_AIR_START);
			return make_shared<CPlayerIdleState>();
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_LANDING))
		{
			if (pGameInstance->Key_Hold('W'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::F, true,true);
			else if (pGameInstance->Key_Hold('A'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::L, true, true);
			else if (pGameInstance->Key_Hold('S'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::B, true, true);
			else if (pGameInstance->Key_Hold('D'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::R, true, true);
		}
		break;
	case JUMP_TYPE::DOUBLE:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_FALL)
			&& pGameInstance->Key_Hold(VK_RBUTTON) && nullptr != _player.Get_Kinesis()->Get_CapturedObject())
		{
			return make_shared<CPlayerThrowState>(ANIMATION::PLAYER::CAPTURE_AIR_START);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DASH_JUMP_LANDING))
		{
			//if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_FALL))
			//	return make_shared<CPlayerThrowState>(ANIMATION::PLAYER::CAPTURE_AIR_START);
			return make_shared<CPlayerIdleState>();
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_LANDING))
		{
			if (pGameInstance->Key_Hold('W'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::F, true, true);
			else if (pGameInstance->Key_Hold('A'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::L, true, true);
			else if (pGameInstance->Key_Hold('S'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::B, true, true);
			else if (pGameInstance->Key_Hold('D'))
				return make_shared<CPlayerWalkState>(PLAYER_DIR::R, true, true);
		}
		break;
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eJumpDashAnim))
	{
		if (_player.Get_PlayerDesc().AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::JUMPDASHATK]
			&& m_pModel->Get_Proportion() > 0.5f && pGameInstance->Key_Down(VK_LBUTTON))
		{
			if (_player.Get_Camera()->Get_LockOn())
				m_pTransform->LookAt_Interpolation(_player.Get_Camera()->Get_LockOnTransform());
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::AIRDASH);
		}
	}

	if (pGameInstance->Key_Down(VK_LBUTTON))
	{
		return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::JUMP);
	}

	if (pGameInstance->Key_Down('C'))
	{
		return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::AIRDOWN);
	}

	if (pGameInstance->Key_Down(VK_SPACE))
	{
		//if(m_eJumpType != JUMP_TYPE::DOUBLE)
		//	return make_shared<CPlayerJumpState>(CPlayerJumpState::JUMP_TYPE::DOUBLE);
	}

	return nullptr;
}

void CPlayerJumpState::Entry(CPlayer& _player)
{
		__super::Entry(_player);

		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_Jump00.wav"),TEXT("Voice_Kasane_Jump01.wav"),TEXT("Voice_Kasane_Jump02.wav") }), g_fPlayerSound);

	m_eCurrentState = PLAYER_STATE::JUMP;
	switch (m_eJumpType)
	{
	case JUMP_TYPE::INPLACE:
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_START, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		break;
	case JUMP_TYPE::RUN:
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::RUN_JUMP_START, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		break;
	case JUMP_TYPE::DASH:
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH_JUMP_START, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		break;
	case JUMP_TYPE::DOUBLE:
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DOUBLE_JUMP_RISE, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		m_fGravityTime = 0.f;
		break;
	}

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_JumpUp(R).wav"), g_fPlayerSound);

}

void CPlayerJumpState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();


	switch (m_eJumpType)
	{
	case JUMP_TYPE::INPLACE:
		if (m_bExecuted[1] && !m_bInAir && !_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_LANDING))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Land.wav"), g_fPlayerSound);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::JUMP_START) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			m_bJumpRise = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_RISE, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::JUMP_RISE) && !m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			m_bJumpRise = false;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_FALL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, true);
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_RISE))
		{
			if (!m_bJumpDash)
			{
				m_pTransform->Translate(_float4(0, fTimeDelta * _player.Get_PlayerDesc().fJumpHeight, 0, 0));
				//m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * _player.Get_PlayerDesc().fJumpRunForwardSpeed * fTimeDelta);
				if (pGameInstance->Key_Hold('A'))
					m_pTransform->Translate(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::RIGHT) * -2.f * fTimeDelta);
				if (pGameInstance->Key_Hold('D'))
					m_pTransform->Translate(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::RIGHT) * 2.f * fTimeDelta);
			}
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN_JUMP_FALL))
		{
			if (!m_bJumpDash)
			{
				m_pTransform->Translate(_float4(0, fTimeDelta * _player.Get_PlayerDesc().fJumpHeight, 0, 0));
			}
		}
		__super::Late_Update(_player, fTimeDelta);
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_FALL) && !m_bInAir)
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Land.wav"), g_fPlayerSound);
		}
		break;
	case JUMP_TYPE::RUN:
		if (m_bExecuted[1] && !m_bInAir && !_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN_JUMP_LANDING))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::RUN_JUMP_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Land.wav"), g_fPlayerSound);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::RUN_JUMP_START) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			m_bJumpRise = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::RUN_JUMP_RISE, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::RUN_JUMP_RISE) && !m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			m_bJumpRise = false;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::RUN_JUMP_FALL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, true);
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN_JUMP_RISE))
		{
			if (!m_bJumpDash)
			{
				m_pTransform->Translate(_float4(0, fTimeDelta * _player.Get_PlayerDesc().fJumpHeight, 0, 0));
				m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * _player.Get_PlayerDesc().fJumpRunForwardSpeed * fTimeDelta);
				if (pGameInstance->Key_Hold('A'))
					m_pTransform->Translate(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::RIGHT) * -2.f * fTimeDelta);
				if (pGameInstance->Key_Hold('D'))
					m_pTransform->Translate(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::RIGHT) * 2.f * fTimeDelta);
			}
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::RUN_JUMP_FALL))
		{
			if (!m_bJumpDash)
			{
				m_pTransform->Translate(_float4(0, fTimeDelta * _player.Get_PlayerDesc().fJumpHeight, 0, 0));
			}
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case JUMP_TYPE::DASH:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DASH_JUMP_START) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			m_bJumpRise = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH_JUMP_RISE, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DASH_JUMP_RISE) && !m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			m_bJumpRise = false;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH_JUMP_FALL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, true);
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_RISE) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_FALL))
		{
			if (!m_bJumpDash)
			{
				m_pTransform->Translate(_float4(0, fTimeDelta * _player.Get_PlayerDesc().fJumpHeight, 0, 0));
				m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * _player.Get_PlayerDesc().fJumpDashForwardSpeed * fTimeDelta);
				if (pGameInstance->Key_Hold('A'))
					m_pTransform->Translate(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::RIGHT) * -2.f * fTimeDelta);
				if (pGameInstance->Key_Hold('D'))
					m_pTransform->Translate(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::RIGHT) * 2.f * fTimeDelta);
			}
		}
		if (m_bExecuted[1] && !m_bInAir && !_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_LANDING))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH_JUMP_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Land.wav"), g_fPlayerSound);
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case JUMP_TYPE::DOUBLE:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DOUBLE_JUMP_RISE) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH_JUMP_FALL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DASH_JUMP_FALL) && !m_bExecuted[1] && !m_bInAir)
		{
			m_bExecuted[1] = true;
			m_bJumpRise = false;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DASH_JUMP_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, true);
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Land.wav"), g_fPlayerSound);
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DOUBLE_JUMP_RISE) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DASH_JUMP_FALL))
		{
			if (!m_bJumpDash)
			{
				m_pTransform->Translate(_float4(0, fTimeDelta * _player.Get_PlayerDesc().fJumpHeight, 0, 0));
				m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * _player.Get_PlayerDesc().fJumpDashForwardSpeed * fTimeDelta);
				if (pGameInstance->Key_Hold('A'))
					m_pTransform->Translate(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::RIGHT) * -2.f * fTimeDelta);
				if (pGameInstance->Key_Hold('D'))
					m_pTransform->Translate(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::RIGHT) * 2.f * fTimeDelta);
			}
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	}

#pragma region AIRDASH
	if (m_bExecuted[4] && !m_bInAir && !_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eJumpDashLandingAnim))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eJumpDashLandingAnim, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Land.wav"), g_fPlayerSound);
	}
	if (pGameInstance->Key_Down(VK_SHIFT) && !m_bExecuted[3])
	{
		m_bExecuted[3] = true;
		m_bJumpDash = true;
		if (pGameInstance->Key_Hold('W'))
		{
			m_eJumpDashAnim = ANIMATION::PLAYER::AIR_DODGE_F;
		}
		if (pGameInstance->Key_Hold('A'))
		{
			m_eJumpDashAnim = ANIMATION::PLAYER::AIR_DODGE_L;
		}
		if (pGameInstance->Key_Hold('S'))
		{
			m_eJumpDashAnim = ANIMATION::PLAYER::AIR_DODGE_B;
		}
		if (pGameInstance->Key_Hold('D'))
		{
			m_eJumpDashAnim = ANIMATION::PLAYER::AIR_DODGE_R;
		}
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eJumpDashAnim, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		m_bGravity = false;
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_JumpDodge.wav"), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_JumpDodge.wav"), g_fPlayerSound);
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eJumpDashAnim))
	{
		if (_player.Get_ActorDesc().bBigHit && *_player.Get_ActorDesc().stHitDesc.AttackColliderActive && !m_bExecuted[11])
		{
			m_bExecuted[11] = true;
			_player.Get_ActorDesc().bBigHit = false;
			_player.Get_PlayerDesc().bJustDodge = true;
			_player.Get_Camera()->Set_LerpTime(0.01f);
			_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
			pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.2f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.2f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.2f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.2f);
			pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.2f);
		}
		__super::Late_Update_Root(_player, fTimeDelta, 1.5f);
	}


	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eJumpDashAnim) && !m_bExecuted[4])
	{
		m_bExecuted[4] = true;
		m_bGravity = true;
		m_fGravityTime = 0.f;
		switch (m_eJumpType)
		{
		case JUMP_TYPE::INPLACE:
			m_eJumpDashFallAnim = ANIMATION::PLAYER::JUMP_FALL;
			m_eJumpDashLandingAnim = ANIMATION::PLAYER::JUMP_LANDING;
			break;
		case JUMP_TYPE::RUN:
			m_eJumpDashFallAnim = ANIMATION::PLAYER::RUN_JUMP_FALL;
			m_eJumpDashLandingAnim = ANIMATION::PLAYER::RUN_JUMP_LANDING;
			break;
		case JUMP_TYPE::DASH:
			m_eJumpDashFallAnim = ANIMATION::PLAYER::DASH_JUMP_FALL;
			m_eJumpDashLandingAnim = ANIMATION::PLAYER::DASH_JUMP_LANDING;
			break;
		}
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eJumpDashFallAnim, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
	}
#pragma endregion


}

void CPlayerJumpState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.Get_Camera()->Set_LerpTime(0.075f);
}

void CPlayerJumpState::Set_ForceInAir(_bool b)
{
	if (m_bExecuted[1])
	{
		m_bForceInAir = b; 
		m_bInAir = false;
	}
}