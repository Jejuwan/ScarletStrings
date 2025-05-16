#include "EnginePCH.h"
#include "Sequence.h"

CSequence::CSequence()
	: CComposite(NODE::SEQUENCE)
{
}

void CSequence::Activate()
{
	__super::Activate();

	m_itCurrentChild	= m_lstChildren.begin();
	m_iChildIndex		= 0u;
}

STATUS CSequence::Invoke(_float _fTimeDelta)
{
	if (m_lstChildren.end() == m_itCurrentChild)
	{
		return STATUS::FAILURE;
	}

	switch ((*m_itCurrentChild)->Invoke(_fTimeDelta))
	{
	case STATUS::RUNNING:
	{
		m_eStatus = STATUS::RUNNING;
	}
	break;
	case STATUS::SUCCESS:
	{
		++m_itCurrentChild;
		++m_iChildIndex;
		if (m_lstChildren.end() != m_itCurrentChild)
		{
			m_eStatus = STATUS::RUNNING;
		}
		else
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}
	break;
	case STATUS::FAILURE:
	{
		m_eStatus = STATUS::FAILURE;
	}
	break;
	}

	return m_eStatus;
}

void CSequence::Terminate()
{
	__super::Terminate();
}
