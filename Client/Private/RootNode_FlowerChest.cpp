#include "ClientPCH.h"
#include "RootNode_FlowerChest.h"
#include "FlowerChestNodeSelector_Phase.h"

HRESULT CRootNode_FlowerChest::Initialize(shared_ptr<class CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CRootNode_FlowerChest::Initialize", "Failed to __super::Initialize");
	}

	if (FAILED(Ready_Skills()))
	{
		MSG_RETURN(E_FAIL, "CRootNode_FlowerChest::Initialize", "Failed to Ready_Skills");
	}
	
	m_pPhaseSelector = CFlowerChestNodeSelector_Phase::Create(m_pBlackBoard);
	if (nullptr == m_pPhaseSelector)
	{
		MSG_RETURN(E_FAIL, "CRootNode_FlowerChest::Initialize", "Failed to CBrotherNodeSelector_Phase::Create");
	}

	m_pPhase = m_pBlackBoard->Get_Anything<FC_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pPhase)
	{
		MSG_RETURN(E_FAIL, "CRootNode_FlowerChest::Initialize", "Failed to Get: Owner:Phase");
	}

	return S_OK;
}

void CRootNode_FlowerChest::Activate()
{
	__super::Activate();
}

STATUS CRootNode_FlowerChest::Invoke(_float fTimeDelta)
{
	Begin_Invoke(fTimeDelta);
	m_eStatus = m_pPhaseSelector->Invoke(fTimeDelta);

	return Return_Invoke();
}

void CRootNode_FlowerChest::Terminate()
{
	__super::Terminate();
}

HRESULT CRootNode_FlowerChest::Ready_Skills()
{
	//SKILLDESC
	return S_OK;
}

shared_ptr<CRootNode_FlowerChest> CRootNode_FlowerChest::Create(shared_ptr<class CBlackBoard> _pBlackBoard)
{
	shared_ptr<CRootNode_FlowerChest> pInstance = make_private_shared(CRootNode_FlowerChest);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CRootNode_FlowerChest::Create", "Failed to Initialize");
	}

	return pInstance;
}
