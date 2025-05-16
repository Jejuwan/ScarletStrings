#pragma once

#include "State.h"
#include "ClientPCH.h"

template <class T>
class CStateTemplate : public CState
{
public:
	CStateTemplate() DEFAULT;
	CStateTemplate(const CStateTemplate& other) DEFAULT;
	virtual ~CStateTemplate() DEFAULT;

	CStateTemplate& operator=(const CStateTemplate& other) DEFAULT;

protected:
	typedef void (T::* PFNSTATE)(void);
	typedef void (T::* PFNEXCUTESTATE)(_float);
	weak_ptr<T> m_pInstance;
	PFNSTATE m_pfnBeginState = { nullptr };
	PFNEXCUTESTATE m_pfnState = { nullptr };
	PFNSTATE m_pfnEndState = { nullptr };

public:
	void Set(weak_ptr<T> pInstance, PFNSTATE pfnBeginState, PFNEXCUTESTATE pfnState, PFNSTATE pfnEndState)
	{
		assert(pInstance.lock());
		m_pInstance = pInstance;
		assert(pfnBeginState);
		m_pfnBeginState = pfnBeginState;
		assert(pfnState);
		m_pfnState = pfnState;
		assert(pfnEndState);
		m_pfnEndState = pfnEndState;
	}

	virtual void ExecuteBeginState()
	{
		assert(m_pInstance.lock() && m_pfnBeginState);
		(m_pInstance.lock().get()->*m_pfnBeginState)();
	}
	virtual void ExecuteState(_float fTimeDelta)
	{
		assert(m_pInstance.lock() && m_pfnState);
		(m_pInstance.lock().get()->*m_pfnState)(fTimeDelta);
	}
	virtual void ExecuteEndState()
	{
		assert(m_pInstance.lock() && m_pfnEndState);
		(m_pInstance.lock().get()->*m_pfnEndState)();
	}
};

