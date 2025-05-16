#pragma once
#include "PlayerState.h"
class CPlayerIdleState :
    public CPlayerState
{
public:
	CPlayerIdleState();
	virtual ~CPlayerIdleState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);
};

