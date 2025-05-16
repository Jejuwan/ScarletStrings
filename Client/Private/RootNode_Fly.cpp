#include "ClientPCH.h"
#include "RootNode_Fly.h"
#include "FlyNodeSelector_Phase.h"

HRESULT CRootNode_Fly::Initialize(shared_ptr<class CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to __super::Initialize");
	}

	if (FAILED(Ready_Skills()))
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to Ready_Skills");
	}
	
	m_pPhaseSelector = CFlyNodeSelector_Phase::Create(m_pBlackBoard);
	if (nullptr == m_pPhaseSelector)
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to CBrotherNodeSelector_Phase::Create");
	}

	m_pPhase = m_pBlackBoard->Get_Anything<FC_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pPhase)
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to Get: Owner:Phase");
	}

	return S_OK;
}

void CRootNode_Fly::Activate()
{
	__super::Activate();
}

STATUS CRootNode_Fly::Invoke(_float fTimeDelta)
{
	Begin_Invoke(fTimeDelta);
	m_eStatus = m_pPhaseSelector->Invoke(fTimeDelta);

	return Return_Invoke();
}

void CRootNode_Fly::Terminate()
{
	__super::Terminate();
}

HRESULT CRootNode_Fly::Ready_Skills()
{
	//SKILLDESC
	return S_OK;
}

shared_ptr<CRootNode_Fly> CRootNode_Fly::Create(shared_ptr<class CBlackBoard> _pBlackBoard)
{
	shared_ptr<CRootNode_Fly> pInstance = make_private_shared(CRootNode_Fly);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CRootNode_Fly::Create", "Failed to Initialize");
	}

	return pInstance;
}
