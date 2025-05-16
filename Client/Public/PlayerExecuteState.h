#pragma once
#include "PlayerState.h"
BEGIN(Client)
class CPlayerExecuteState :
    public CPlayerState
{
public:
	CPlayerExecuteState();
	virtual ~CPlayerExecuteState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

private:
	shared_ptr<class CTransform> m_pInitLockedOnTransform = nullptr;
	_float m_fInterpolateTime[100] = { 0.f };
	_float m_fEndTime = 0.f;
	_float3 vInitCamPos;
	_float3 m_vInitPlayerPos;
};
END
