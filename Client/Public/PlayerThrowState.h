#pragma once
#include "PlayerState.h"
class CPlayerThrowState :
	public CPlayerState
{
public:
	CPlayerThrowState();
	CPlayerThrowState(ANIMATION::PLAYER _anim);
	virtual ~CPlayerThrowState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

	void SetThrow(_bool b) { m_bThrow = b; }
private:
	_bool m_bThrow = { false };
	_bool m_bSlow = { false };
	ANIMATION::PLAYER m_eAnim = ANIMATION::PLAYER::CAPUTRE_START;
	ANIMATION::PLAYER m_eThrowAnim;
};

