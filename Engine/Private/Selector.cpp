#include "EnginePCH.h"
#include "Selector.h"

CSelector::CSelector()
	: CComposite(NODE::SELECTOR)
{
}

void CSelector::Activate()
{
	__super::Activate();

	m_itCurrentChild	= m_lstChildren.begin();
	m_iChildIndex		= 0u;
}

STATUS CSelector::Invoke(_float _fTimeDelta)
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
		m_eStatus = STATUS::SUCCESS;
	}
	break;
	case STATUS::FAILURE:
	{
		++m_itCurrentChild;
		++m_iChildIndex;
		if (m_lstChildren.end() != m_itCurrentChild)
		{
			m_eStatus = STATUS::RUNNING;
		}
		else
		{
			m_eStatus = STATUS::FAILURE;
		}
	}
	break;
	}

	return m_eStatus;
}

void CSelector::Terminate()
{
	__super::Terminate();
}
