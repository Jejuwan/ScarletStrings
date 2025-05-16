#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_Health.h"

CBehaviorTreeDecorator_Health::CBehaviorTreeDecorator_Health(shared_ptr<CNode> _pNode, const ENTITYDESC* _pEntityDesc, _float _fMinHP, _float _fMaxHP)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_pEntityDesc(_pEntityDesc)
{
}

HRESULT CBehaviorTreeDecorator_Health::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Health";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_Health::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_Health::Activate()
{
	__super::Activate();
}

STATUS CBehaviorTreeDecorator_Health::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pNode->Is_Running())
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		if (Function::InRange(m_pEntityDesc->fHP, m_fMinHP, m_fMaxHP))
		{
			m_eStatus = m_pNode->Invoke(_fTimeDelta);
		}
		else
		{
			m_eStatus = STATUS::FAILURE;
		}
	}

	return Return_Invoke();
}

void CBehaviorTreeDecorator_Health::Terminate()
{
	if (m_pNode->Is_Running())
	{
		m_pNode->Terminate();
	}

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_Health::DebugLog()
{
}
#endif

shared_ptr<CBehaviorTreeDecorator_Health> CBehaviorTreeDecorator_Health::Create(shared_ptr<CBlackBoard> _pBlackBoard, const ENTITYDESC* _pEntityDesc, _float _fMinHP, _float _fMaxHP, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_Health> pInstance = make_private_shared(CBehaviorTreeDecorator_Health, _pNode, _pEntityDesc, _fMinHP, _fMaxHP);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_Health::Create", "Failed to Initialize");
	}

	return pInstance;
}
