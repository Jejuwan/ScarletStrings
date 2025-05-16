#include "ClientPCH.h"
#include "Decorator_OnHit.h"
#include "GameInstance.h"

CDecorator_OnHit::CDecorator_OnHit(shared_ptr<CNode> pNode)
	: CDecorator(pNode)
{
}

HRESULT CDecorator_OnHit::Initialize(shared_ptr<class CBlackBoard> pBlackBoard)
{
	if (FAILED(__super::Initialize(pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDecorator_OnHit::Initialize", "Failed to __super::Initialize");
	}

	m_pMonsterDesc = m_pBlackBoard->Get_Anything<CMonster::MonsterDesc*>(TEXT("Owner:MonsterDesc")).value_or(nullptr);
	if (nullptr == m_pMonsterDesc)
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_LeafNode_Idle::Initialize", "Failed to Get: Owner:MonsterDesc");
	}

	return S_OK;
}

void CDecorator_OnHit::Activate()
{
	__super::Activate();
}

STATUS CDecorator_OnHit::Invoke(_float fTimeDelta)
{
	Begin_Invoke(fTimeDelta);
	if (m_pMonsterDesc->HitStatus)
	{
		m_eStatus = STATUS::SUCCESS;
		return Return_Invoke();
	}
	m_eStatus = m_pNode->Invoke(fTimeDelta);
	return Return_Invoke();
}

void CDecorator_OnHit::Terminate()
{
	m_pNode->Terminate();
	__super::Terminate();
}

shared_ptr<CDecorator_OnHit> CDecorator_OnHit::Create(shared_ptr<CBlackBoard> pBlackBoard, shared_ptr<CNode> _pNode)
{
	shared_ptr<CDecorator_OnHit> pInstance = make_private_shared(CDecorator_OnHit, _pNode);

	if (FAILED(pInstance->Initialize(pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDecorator_Distance::Create", "Failed to Initialize");
	}

	return pInstance;
}
