#include "EnginePCH.h"
#include "FSM.h"

void CFSM::Tick(_float fTimeDelta)
{
	if (m_pNewState)
	{
		m_pCurrentState->ExecuteEndState();
		m_pCurrentState = m_pNewState;
		m_pNewState = nullptr;
		m_pCurrentState->ExecuteBeginState();
	}
	m_pCurrentState->ExecuteState(fTimeDelta);

	ExecutePersistent(fTimeDelta);
}

bool CFSM::IsState(const shared_ptr<CState>& pState)
{
	return (m_pCurrentState == pState);
}

bool CFSM::GotoState(const shared_ptr<CState> pNewState)
{
	m_pNewState = pNewState;
	return true;
}


