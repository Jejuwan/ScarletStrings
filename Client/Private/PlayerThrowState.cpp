#include "ClientPCH.h"
#include "PlayerThrowState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerJumpState.h"
#include "GameInstance.h"
#include "Player.h"
#include "Model.h"
#include "Animator.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "Effect_Manager.h"
#include "ScreenEffectManager.h"

CPlayerThrowState::CPlayerThrowState()
{
}

CPlayerThrowState::CPlayerThrowState(ANIMATION::PLAYER _anim)
{
	m_eAnim = _anim;
}

shared_ptr<CPlayerState> CPlayerThrowState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eEndAnim)
		&& m_pModel->Get_Proportion() > 0.4f)
	{
		if (pGameInstance->Key_Hold('W'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::F, true);
		}
		else if (pGameInstance->Key_Hold('A'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::L, true);
		}
		else if (pGameInstance->Key_Hold('S'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::B, true);
		}
		else if (pGameInstance->Key_Hold('D'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::R, true);
		}
		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			if (!_player.Get_Kinesis()->Get_ThrowHit())
				return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::NORMAL);
			else
				return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::FOLLOW);
		}
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::THROW_END) ||
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_END) ||
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::THROW_FORWARD) || 
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::JUMP_LANDING))
	{
		return make_shared<CPlayerIdleState>();
	}
	return nullptr;
}

void CPlayerThrowState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	m_eCurrentState = PLAYER_STATE::THROW; 

	if (m_eAnim == ANIMATION::PLAYER::CAPTURE_AIR_START)
	{
		_float3 vDir = static_pointer_cast<CTransform>(_player.Get_Kinesis()->Get_CapturedObject()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);
		vDir.y = 0.f;
		m_pTransform->LookTo_Interpolation(XMVector3Normalize(vDir));
		m_bGravity = false;
		CEffect_Manager::Get_Instance()->Fetch(VFX::AIR_KINESEIS_DIFFUSE0, m_pTransform->Get_Matrix());
		CEffect_Manager::Get_Instance()->Fetch(VFX::AIR_KINESEIS_DIFFUSE1, m_pTransform->Get_Matrix());
		CEffect_Manager::Get_Instance()->Fetch(VFX::AIR_KINESEIS_DISTORTION, m_pTransform->Get_Matrix());
	}
	else
	{
		CEffect_Manager::Get_Instance()->Fetch(VFX::NEW_KINESEIS_DIFFUSE0, m_pTransform->Get_Matrix());
		CEffect_Manager::Get_Instance()->Fetch(VFX::NEW_KINESEIS_DIFFUSE1, m_pTransform->Get_Matrix());
		CEffect_Manager::Get_Instance()->Fetch(VFX::NEW_KINESEIS_DISTORTION, m_pTransform->Get_Matrix());
		CEffect_Manager::Get_Instance()->Fetch(VFX::KINESIS_PARTICLE, m_pTransform->Get_Matrix());
	}
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eAnim, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
	m_eEndAnim = ANIMATION::PLAYER::THROW_END;
	if (_player.Get_Kinesis()->Get_Combo() == 2)
		m_eThrowAnim = ANIMATION::PLAYER::THROW_STRONG_SECOND;
	else
		m_eThrowAnim = ANIMATION::PLAYER::THROW_FORWARD;
	//cout << _player.Get_Kinesis()->Get_Combo() << endl;
	_player.Enable_OutLine(true);

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Use_PyocoKinesis01.wav"), g_fPlayerSound);

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Psycokinesis.wav"), g_fPlayerSound);
}

void CPlayerThrowState::Update(CPlayer & _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);
	_player.Enable_OutLine(true, fTimeDelta);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	
	switch(m_eAnim)
	{
	case ANIMATION::PLAYER::CAPUTRE_START:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPUTRE_START) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_LOOP, _player.Get_PlayerDesc().fAnimSpeed, false, 0.0f, true);
		}

		if (m_bThrow && !m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_START, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Use_PyocoKinesis00.wav"), g_fPlayerSound);
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::THROW_START) && !m_bExecuted[2])
		{
			m_bExecuted[2] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_END, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			_player.Get_PlayerDesc().fKinesisGauge -= _player.Get_PlayerDesc().fKinesisConsume;
		}

		if (!pGameInstance->Key_Hold(VK_RBUTTON) && !m_bExecuted[1] && !m_bExecuted[3])
		{
			m_bExecuted[3] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_END, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);

		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case ANIMATION::PLAYER::CAPTURE_AIR_START:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_AIR_START) && !m_bExecuted[0] &&!m_bThrow)
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_AIR_LOOP, _player.Get_PlayerDesc().fAnimSpeed, false, 0.0f, true);
		}

		if (m_bThrow && !m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_AIR_START, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			_player.Get_PlayerDesc().fKinesisGauge -= _player.Get_PlayerDesc().fKinesisConsume;
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Use_PyocoKinesis00.wav"), g_fPlayerSound);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::THROW_AIR_START) && !m_bExecuted[2])
		{
			m_bExecuted[2] = true;
			m_bGravity = true;
			m_fGravityTime = 0.f;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_AIR_CANCEL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}

		if (!pGameInstance->Key_Hold(VK_RBUTTON) && !m_bExecuted[3])
		{
			m_bExecuted[3] = true;
			m_bGravity = true;
			m_fGravityTime = 0.f;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_AIR_CANCEL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}

		if (!m_bInAir && _player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::THROW_AIR_CANCEL) &&
			!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_LANDING))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case ANIMATION::PLAYER::CAPTURE_BACKSTEP_START:
		_player.Get_Kinesis()->Set_Strong(true);
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START))
		{
			__super::Late_Update_Root(_player, fTimeDelta,2.f);
		}
		else
			__super::Late_Update(_player, fTimeDelta);

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START) && !m_bExecuted[1] && !m_bThrow)
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation((_uint)ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, true);
		}
		if ( m_bThrow &&!m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			_float fSpeed = _player.Get_PlayerDesc().fAnimSpeed;
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Use_PyocoKinesis00.wav"), g_fPlayerSound);
			if (m_eThrowAnim == ANIMATION::PLAYER::THROW_STRONG_SECOND)
				fSpeed += 0.75f;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eThrowAnim, fSpeed, false, g_fDefaultInterpolationDuration, false);
			_player.Get_PlayerDesc().fKinesisGauge -= _player.Get_PlayerDesc().fKinesisConsume;

			CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::PURPLE_FILTER); 
		}
		//if (m_pModel->Get_InterpolateEnd() && m_bThrow)
		//{
		//	m_bExecuted[2] = true;
		//	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Lock(false);
		//	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_FORWARD, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		//}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::THROW_STRONG_SECOND))
		{
			m_eEndAnim = ANIMATION::PLAYER::THROW_STRONG_SECOND;
			if (m_pModel->Get_Proportion() > 0.3f && !m_bExecuted[4])
			{
				m_bExecuted[4] = true;
				_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
				pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
				pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.1f);
			}
			if (m_pModel->Get_Proportion() > 0.35f && !m_bExecuted[5])
			{
				m_bExecuted[5] = true;
				_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
				pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 1.f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 1.f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 1.f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 1.f);
				pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 1.f);
			}
		}
		else if (!pGameInstance->Key_Hold(VK_RBUTTON) && !m_bExecuted[3])
		{
			m_bExecuted[3] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_END, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		break;
	}

}

void CPlayerThrowState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.Get_Kinesis()->Reset();
	_player.Enable_OutLine(false);
}
