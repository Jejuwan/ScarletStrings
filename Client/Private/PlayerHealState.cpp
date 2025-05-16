#include "ClientPCH.h"
#include "PlayerHealState.h"
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

CPlayerHealState::CPlayerHealState()
{
}

shared_ptr<CPlayerState> CPlayerHealState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::USEITEM))
	{
		return make_shared<CPlayerIdleState>();
	}

	return nullptr;

}

void CPlayerHealState::Entry(CPlayer& _player)
{
	__super::Entry(_player);
	m_eCurrentState = PLAYER_STATE::HEAL;
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::USEITEM, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_InstantKill_Fly1.wav"), g_fPlayerSound);
}

void CPlayerHealState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);

	if (m_pModel->Get_Proportion() > 0.5f && !m_bExecuted[0])
	{
		m_bExecuted[0] = true;
		_player.Get_PlayerDesc().iHp += 50;
		if (_player.Get_PlayerDesc().iHp > 750)
			_player.Get_PlayerDesc().iHp = 750;
	}

	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerHealState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
}
