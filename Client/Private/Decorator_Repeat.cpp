#include "ClientPCH.h"
#include "GameInstance.h"
#include "Decorator_Repeat.h"

CDecorator_Repeat::CDecorator_Repeat(shared_ptr<CNode> _pNode, _float _fRepeat)
	: CDecorator(_pNode)
	, m_fRepeat	(_fRepeat)
{
}

HRESULT CDecorator_Repeat::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDecorator_Repeat::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CDecorator_Repeat::Activate()
{
	__super::Activate();
}

STATUS CDecorator_Repeat::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_fTimeAcc < m_fRepeat)
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CDecorator_Repeat::Terminate()
{
	m_pNode->Terminate();

	__super::Terminate();
}


shared_ptr<CDecorator_Repeat> CDecorator_Repeat::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fRepeat, shared_ptr<CNode> _pNode)
{
	shared_ptr<CDecorator_Repeat> pInstance = make_private_shared(CDecorator_Repeat, _pNode, _fRepeat);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDecorator_Repeat::Create", "Failed to Initialize");
	}

	return pInstance;
}
