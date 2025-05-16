                                                   #pragma once
#include "PlayerState.h"
BEGIN(Client)
class CPlayerAttackState :
	public CPlayerState
{
public:
	enum class ATK_TYPE{NONE,NORMAL,BACKSTEP,UPPER,DASH,JUMP,CHARGE,FOLLOW
		,AIRDASH,AIRDOWN,ONECLICKCHARGE,JUSTDODGE,BRAINFIELD};
public:
	CPlayerAttackState(ATK_TYPE type);
	virtual ~CPlayerAttackState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

private:
	ATK_TYPE m_eType = ATK_TYPE::NONE;
	ANIMATION::PLAYER m_eEndAnim = ANIMATION::PLAYER::MAX;
	_float m_fBlendTime = 0.f;
	_float m_fChargeTime = 0.f;

	_bool m_bElectric = { false };
	shared_ptr<class CMap_Dynamic> m_pBFObject = nullptr;

	_float m_fEffect_Timer = { 1.f };
//private:
//	void AirDownAttack(class CPlayer& _player);
};
END
