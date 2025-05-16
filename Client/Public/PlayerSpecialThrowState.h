#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CPlayerSpecialThrowState :
	public CPlayerState
{
public:
	CPlayerSpecialThrowState();
	CPlayerSpecialThrowState(SPECIAL_THROW type);
	virtual ~CPlayerSpecialThrowState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

private:
	SPECIAL_THROW m_eThrowType = SPECIAL_THROW::MAX;
	_bool m_bThrow = false;
	_float m_fThrowTime = 0.f;
	_float m_fBusSpeed = 30.f;
	_float m_fBusRotateSpeed = 2.f;
	_float m_fRideTime = 0.f;
	_float3 m_vStartPos;
	_float3 m_vThrowDir;
	_float m_fTime = 0.f;
	_float3 m_vRanMindPos[30];
	_bool m_bPileHit = { false };
	shared_ptr<class CGameObject> m_pThrowObject;
	_bool m_bSlowStart = false;
	_float m_fSlowTime = 0.1f;
	shared_ptr<class CMonster_Karen> m_pKaren;
private:
	void HeavyEquipment(class CPlayer& _player, _float fTimeDelta);
	void Beam(class CPlayer& _player, _float fTimeDelta);
	void Rock(class CPlayer& _player, _float fTimeDelta);
	void Bulldozer(class CPlayer& _player, _float fTimeDelta);
	void Bus(class CPlayer& _player, _float fTimeDelta);
	void Final_Rock(class CPlayer& _Player, _float fTimeDelta);
	void Long(class CPlayer& _player, _float fTimeDelta);
	void Mindroom(class CPlayer& _player, _float fTimeDelta);

	void Drive_Bus(class CPlayer& _player, _float fTimeDelta);
public:
	void Set_Throw(_bool b) { m_bThrow = b; }
	_bool Get_Throw() { return m_bThrow; }

};
END

