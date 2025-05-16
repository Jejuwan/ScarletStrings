#pragma once
#include "PlayerState.h"
BEGIN(Client)
class CPlayerFinalState :
    public CPlayerState
{
public:
	CPlayerFinalState();
	virtual ~CPlayerFinalState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);


private:
	weak_ptr<class CMonster_Karen> m_pKaren;
	_float						   m_fEndTime = 0.f;
	_float						   m_fLerpTime = 0.f;
};
END
