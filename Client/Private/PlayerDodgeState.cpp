#include "ClientPCH.h"
#include "PlayerDodgeState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerThrowState.h"
#include "PlayerJumpState.h"
#include "PlayerExecuteState.h"
#include "GameInstance.h"
#include "Player.h"
#include "Model.h"
#include "Animator.h"
#include "Psychokinesis.h"
#include "Camera_Player.h"

CPlayerDodgeState::CPlayerDodgeState()
{
}

CPlayerDodgeState::CPlayerDodgeState(PLAYER_DIR _dirs)
{
	m_eDir = _dirs;
}

shared_ptr<CPlayerState> CPlayerDodgeState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	
	if (_player.Get_PlayerDesc().bJustDodge)
	{
		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::JUSTDODGE);
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eEndAnim)&&
		m_pModel->Get_Proportion() > 0.3f)
	{
		if (pGameInstance->Key_Hold('W'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::F);
		}
		else if (pGameInstance->Key_Hold('A'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::L);
		}
		else if (pGameInstance->Key_Hold('S'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::B);
		}
		else if (pGameInstance->Key_Hold('D'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::R);
		}
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eEndAnim))
	{
		if (pGameInstance->Key_Down(VK_UP) ||
			pGameInstance->Key_Down(VK_LEFT) ||
			pGameInstance->Key_Down(VK_RIGHT) ||
			pGameInstance->Key_Down(VK_DOWN))
			return make_shared<CPlayerWalkState>();

		return make_shared<CPlayerIdleState>();
	}
	
	return nullptr;
}

void CPlayerDodgeState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	m_eCurrentState = PLAYER_STATE::DODGE;

	switch (m_eDir)
	{
	case PLAYER_DIR::F:
		m_eAnim = ANIMATION::PLAYER::DODGE_F_START;
		break;
	case PLAYER_DIR::L:
		m_eAnim = ANIMATION::PLAYER::DODGE_L_START;
		break;
	case PLAYER_DIR::R:
		m_eAnim = ANIMATION::PLAYER::DODGE_R_START;
		break;
	case PLAYER_DIR::B:
		m_eAnim = ANIMATION::PLAYER::DODGE_B_START;
		break;
	}
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eAnim, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, false);
	m_pTransform->LookTo(m_pCameraTransform->Get_HorizontalDir(TRANSFORM::LOOK));
	CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Dodge.wav"), g_fPlayerSound);
}

void CPlayerDodgeState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eAnim) && !m_bExecuted[0])
	{
		m_bExecuted[0] = true;
		switch (m_eDir)
		{
		case PLAYER_DIR::F:
			m_eEndAnim = ANIMATION::PLAYER::DODGE_F_STOP;
			break;
		case PLAYER_DIR::L:
			m_eEndAnim = ANIMATION::PLAYER::DODGE_L_STOP;
			break;
		case PLAYER_DIR::R:
			m_eEndAnim = ANIMATION::PLAYER::DODGE_R_STOP;
			break;
		case PLAYER_DIR::B:
			m_eEndAnim = ANIMATION::PLAYER::DODGE_B_STOP;
			break;
		}
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eEndAnim, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, false);
	}

	__super::Late_Update_Root(_player, fTimeDelta,1.f);

	if (_player.Get_ActorDesc().bBigHit && *_player.Get_ActorDesc().stHitDesc.AttackColliderActive && !m_bExecuted[1])
	{
		m_bExecuted[1] = true;
		_player.Get_ActorDesc().bBigHit = false;
		_player.Get_PlayerDesc().bJustDodge = true;
		_player.Get_Camera()->Set_LerpTime(0.01f);
		_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
		pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.2f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.2f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.2f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.2f);
		pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.2f);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_JustDodge.wav"), g_fPlayerSound);
	}
}

void CPlayerDodgeState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.Get_Camera()->Set_LerpTime(0.075f);

}
