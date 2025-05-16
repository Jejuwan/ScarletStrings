#pragma once
#include "PlayerState.h"
class CPlayerWalkState :
	public CPlayerState
{
public:
	CPlayerWalkState();
	CPlayerWalkState(PLAYER_DIR eDir,_bool bRunniung = false,_bool bDash = false);
	virtual ~CPlayerWalkState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

private:
	PLAYER_DIR	m_eDir;
	ANIMATION::PLAYER m_eAnim =ANIMATION::PLAYER::MAX;
	_bool bGoDiagonalDuringRun = { false };
	_bool m_bTurnAnimPassed = { false };
	_bool m_bRunning = { false };
	_bool m_bDash = { false };

	_float3 m_vRealDir;

	_float	m_fRunStartSkipProportion = 1.1f;
	_float m_fSkipProportion = 0.7f;
	_float m_fAnimSpeed = 2.5f;

	_bool m_bSoundReset1 = true;
	_bool m_bSoundReset2 = true;
private:
	void RayCastFloor(CPlayer& _player);
};
