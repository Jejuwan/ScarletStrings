#include "ClientPCH.h"
#include "PlayerDriveModeState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerThrowState.h"
#include "PlayerJumpState.h"
#include "PlayerDodgeState.h"
#include "PlayerExecuteState.h"
#include "PlayerSpecialThrowState.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "PhysXCollider.h"
#include "Model.h"
#include "Animator.h"
#include "Psychokinesis.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "Monster.h"
#include "UI_Manager.h"

CPlayerDriveModeState::CPlayerDriveModeState()
{
}

shared_ptr<CPlayerState> CPlayerDriveModeState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DRIVE_WAVE))
	{
		return make_shared<CPlayerIdleState>();
	}

	return nullptr;

}

void CPlayerDriveModeState::Entry(CPlayer& _player)
{
	__super::Entry(_player);
	m_eCurrentState = PLAYER_STATE::BRAIN_DRIVE;
	auto pUIManager = CUI_Manager::Get_Instance();
	pUIManager->Open_Drive_Mode(CHARACTER::KASANE, std::bind(&CPlayer::Drive_Entry, &_player));
	_player.Get_PlayerDesc().bInvincibility = true;
	//_player.Drive_Entry();

		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Drive_Start.wav"), g_fPlayerSound);
}

void CPlayerDriveModeState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);

	if (_player.Get_PlayerDesc().bDriveMode && !m_bExecuted[0])
	{
		m_bExecuted[0] = true;
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DRIVE_WAVE, 1.5f, false, g_fDefaultInterpolationDuration, false);
	}

	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerDriveModeState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.Get_PlayerDesc().bInvincibility = false;
}
