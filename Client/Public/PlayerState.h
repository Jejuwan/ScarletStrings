#pragma once
#include "Client_Define.h"
BEGIN(Engine)
class CModel;
class CGameInstance;
END

BEGIN(Client)
class CPlayerState
{
public:
	CPlayerState();
	virtual ~CPlayerState() = default;

public:
	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Late_Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Late_Update_Root(class CPlayer& _player, _float fTimeDelta, _float fAmount);
	virtual void Exit(class CPlayer& _player);

protected:
	PLAYER_STATE						m_eCurrentState;

	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<class CModel>			m_pModel;
	shared_ptr<CTransform>				m_pCameraTransform;

	_vector								m_vPrePos;
	_vector								m_vInitPos;
	_float4								m_vPreRootPos;
	_float m_fGravityTime = 0.f;
	_float m_fTime = 0.f;

	PLAYER_SAS	m_eCurSAS;

	ANIMATION::PLAYER m_eAnim;
	ANIMATION::PLAYER m_eEndAnim;
	_float m_fProportion = 0.f;

	_float m_fTrackPos = 0.f;
	_float m_fDuration = 0.f;
	_float4 m_fPreRootPos;
	_bool m_bExecuted[100] = { false };
	_bool m_bForceInAir = { false };

	_bool m_bGravity = { true };
	_bool m_bInAir = { false };
	_bool m_bJumpRise = { false };
	_bool m_bHitLagEnable = { true };
	_float m_fCurrentLagIntervalTime = 0.f;
	_float m_fLagIntervalTime = 0.f;
	_bool m_bMonsterKickExecuted = { false };
	_bool m_bMonsterKick = { false };
	
	PxControllerCollisionFlags m_CollisionFlag;
public:
	void							Execute_Lag();
	void							Execute_Lag_Interval();
public:
	PLAYER_STATE					Get_CurrentState() { return m_eCurrentState; }
	PLAYER_SAS						Get_CurrentSAS() { return m_eCurSAS; }
	virtual void					Set_ForceInAir(_bool b) { m_bForceInAir = b; m_bInAir = false; }
	_bool							Get_InAir() { return m_bInAir; }
	_bool							Get_HitLag() { return m_bHitLagEnable; };
	void							Set_MonsterKick(_bool b) { m_bMonsterKick = b; }
	_bool							Get_MonsterKick() { return m_bMonsterKick; }
	void							Set_LagIntervalTime(_float f) { m_fLagIntervalTime = f; }
	_bool							Is_PxColUp() { return m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_UP; }
	_bool							Is_PxColSides() { return m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_SIDES; }
	_bool							Is_PxColDown() { return m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN; }
	_bool							Get_CollisionFlag() { return m_CollisionFlag; }
};
END

