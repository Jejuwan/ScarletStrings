#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_Range.h"

CBehaviorTreeDecorator_Range::CBehaviorTreeDecorator_Range(shared_ptr<CNode> _pNode, _float _fMinRange, _float _fMaxRange)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_fMinRange(_fMinRange)
	, m_fMaxRange(_fMaxRange)
{
}

HRESULT CBehaviorTreeDecorator_Range::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Range";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_Range::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_Range::Initialize", "Failed to Get Owner:Transform");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_Range::Initialize", "Failed to Get Target:Transform");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_Range::Activate()
{
	__super::Activate();
}

STATUS CBehaviorTreeDecorator_Range::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pNode->Is_Running())
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		if (Function::InRange(Function::Distance(m_pTransform, m_pTargetTransform), m_fMinRange, m_fMaxRange))
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

void CBehaviorTreeDecorator_Range::Terminate()
{
	if (m_pNode->Is_Running())
	{
		m_pNode->Terminate();
	}

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_Range::DebugLog()
{
}
#endif

shared_ptr<CBehaviorTreeDecorator_Range> CBehaviorTreeDecorator_Range::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fMinRange, _float _fMaxRange, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_Range> pInstance = make_private_shared(CBehaviorTreeDecorator_Range, _pNode, _fMinRange, _fMaxRange);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_Range::Create", "Failed to Initialize");
	}

	return pInstance;
}
