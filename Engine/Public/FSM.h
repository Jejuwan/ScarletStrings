#pragma once
#include "Engine_Define.h"
#include "State.h"
#include "System.h"

BEGIN(Engine)

class ENGINE_DLL CFSM abstract : public ISystem
{
protected:
	shared_ptr<CState> m_pCurrentState;
	shared_ptr<CState> m_pNewState;

protected:
	explicit CFSM() DEFAULT;
	virtual ~CFSM() DEFAULT;

public:
	virtual void Tick(_float fTimeDelta) override;
	
public:
	bool IsState(const shared_ptr<CState>& pState);
	bool GotoState(const shared_ptr<CState> pNewState);

	virtual void BeginStateInitial() {}
	virtual void StateInitial(_float fTimeDelta) {}
	virtual void EndStateInitial() {}

	virtual void ExecutePersistent(_float fTimeDelta) {}

protected:
	_float m_fFSMAccTime = { 0.f };

public:
	virtual void GotoFinish() {};
	
};

END