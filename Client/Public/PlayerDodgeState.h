#pragma once
#include "PlayerState.h"
class CPlayerDodgeState :
	public CPlayerState
{
public:
	CPlayerDodgeState();
	CPlayerDodgeState(PLAYER_DIR _dirs);
	virtual ~CPlayerDodgeState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

private:
	PLAYER_DIR m_eDir;
	ANIMATION::PLAYER m_eAnim = ANIMATION::PLAYER::DODGE_F_START;
	ANIMATION::PLAYER m_eEndAnim = ANIMATION::PLAYER::DODGE_F_STOP;
};

