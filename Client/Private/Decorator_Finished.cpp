#include "ClientPCH.h"
#include "Decorator_Finished.h"
#include "GameInstance.h"

CDecorator_Finished::CDecorator_Finished(shared_ptr<CNode> pNode)
	: CDecorator(pNode)
{
}

HRESULT CDecorator_Finished::Initialize(shared_ptr<class CBlackBoard> pBlackBoard)
{
	if (FAILED(__super::Initialize(pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDecorator_Finished::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDecorator_Finished::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDecorator_Finished::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CDecorator_Finished::Activate()
{
	__super::Activate();
}

STATUS CDecorator_Finished::Invoke(_float fTimeDelta)
{
	Begin_Invoke(fTimeDelta);

	return Return_Invoke();
}

void CDecorator_Finished::Terminate()
{
	m_pNode->Terminate();
	__super::Terminate();
}

shared_ptr<CDecorator_Finished> CDecorator_Finished::Create(shared_ptr<CBlackBoard> pBlackBoard, shared_ptr<CNode> _pNode)
{
	shared_ptr<CDecorator_Finished> pInstance = make_private_shared(CDecorator_Finished, _pNode);

	if (FAILED(pInstance->Initialize(pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeDecorator_Range::Create", "Failed to Initialize");
	}

	return pInstance;
}
