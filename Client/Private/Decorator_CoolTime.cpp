#include "ClientPCH.h"
#include "Decorator_CoolTime.h"
#include "GameInstance.h"

CDecorator_CoolTime::CDecorator_CoolTime(shared_ptr<CNode> pNode)
	: CDecorator(pNode)
{
}

HRESULT CDecorator_CoolTime::Initialize(shared_ptr<class CBlackBoard> pBlackBoard)
{
	if (FAILED(__super::Initialize(pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDecorator_CoolTime::Initialize", "Failed to __super::Initialize");
	}

	/*m_pMonsterDesc = m_pBlackBoard->Get_Anything<CMonster::MonsterDesc*>(TEXT("Owner:MonsterDesc")).value_or(nullptr);
	if (nullptr == m_pMonsterDesc)
	{
		MSG_RETURN(E_FAIL, "CFlowerChest_LeafNode_Idle::Initialize", "Failed to Get: Owner:MonsterDesc");
	}*/

	return S_OK;
}

void CDecorator_CoolTime::Activate()
{
	__super::Activate();
}

STATUS CDecorator_CoolTime::Invoke(_float fTimeDelta)
{
	Begin_Invoke(fTimeDelta);
	/*if (m_pMonsterDesc->CoolTime >= 0.f)
	{
		m_eStatus = STATUS::FAILURE;
		return Return_Invoke();
	}
	m_eStatus = m_pNode->Invoke(fTimeDelta);*/
	return Return_Invoke();
}

void CDecorator_CoolTime::Terminate()
{
	//m_pNode->Terminate();
	__super::Terminate();
}

shared_ptr<CDecorator_CoolTime> CDecorator_CoolTime::Create(shared_ptr<CBlackBoard> pBlackBoard, shared_ptr<CNode> _pNode)
{
	shared_ptr<CDecorator_CoolTime> pInstance = make_private_shared(CDecorator_CoolTime, _pNode);

	if (FAILED(pInstance->Initialize(pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDecorator_Distance::Create", "Failed to Initialize");
	}

	return pInstance;
}
