#pragma once
#include "PlayerState.h"
class CPlayerJumpState :
	public CPlayerState
{
public:
	enum class JUMP_TYPE{INPLACE,RUN,DASH,DOUBLE};
public:
	CPlayerJumpState(JUMP_TYPE _eType);
	virtual ~CPlayerJumpState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

private:
	JUMP_TYPE m_eJumpType;
	_bool	m_bTurning = { false };
	_bool  m_bInitJump = { false };
	_bool  m_bJumpDash = { false };
	ANIMATION::PLAYER m_eJumpDashAnim = ANIMATION::PLAYER::AIR_DODGE_F;
	ANIMATION::PLAYER m_eJumpDashFallAnim = ANIMATION::PLAYER::JUMP_FALL;
	ANIMATION::PLAYER m_eJumpDashLandingAnim = ANIMATION::PLAYER::JUMP_LANDING;

public:
	virtual void					Set_ForceInAir(_bool b);
	
};
