#pragma once
#include "PlayerState.h"
#include "Client_Define.h"
class CPlayerHurtState :
	public CPlayerState
{
public:
	CPlayerHurtState();
	CPlayerHurtState(PLAYER_HURTTYPE _type);
	virtual ~CPlayerHurtState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

private:
	PLAYER_HURTTYPE m_eType = PLAYER_HURTTYPE::NONE;
	ANIMATION::PLAYER m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_FL;
	_bool m_bBreakFall = false;

private:
	void Default_Entry(class CPlayer& _player);
	void Normal_Entry(class CPlayer& _player);
	void Strong_Entry(class CPlayer& _player);
	void Air_Entry(class CPlayer& _player);

};

