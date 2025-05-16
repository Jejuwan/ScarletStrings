#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CState
{
public:
	CState() DEFAULT;
	CState(const CState& other) DEFAULT;
	virtual ~CState() DEFAULT;

	virtual void ExecuteBeginState() PURE;
	virtual void ExecuteState(_float fTimeDelta)PURE;
	virtual void ExecuteEndState()PURE;
};

END