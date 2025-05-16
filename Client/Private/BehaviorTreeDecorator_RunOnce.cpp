#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_RunOnce.h"

CBehaviorTreeDecorator_RunOnce::CBehaviorTreeDecorator_RunOnce(shared_ptr<CNode> _pNode)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_bExecuted(false)
{
}

HRESULT CBehaviorTreeDecorator_RunOnce::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "RunOnce";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_RunOnce::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_RunOnce::Activate()
{
	__super::Activate();
}

STATUS CBehaviorTreeDecorator_RunOnce::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (!m_bExecuted)
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CBehaviorTreeDecorator_RunOnce::Terminate()
{
	__super::Terminate();

	m_bExecuted = true;
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_RunOnce::DebugLog()
{
}
#endif

shared_ptr<CBehaviorTreeDecorator_RunOnce> CBehaviorTreeDecorator_RunOnce::Create(shared_ptr<CBlackBoard> _pBlackBoard, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_RunOnce> pInstance = make_private_shared(CBehaviorTreeDecorator_RunOnce, _pNode);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_RunOnce::Create", "Failed to Initialize");
	}

	return pInstance;
}
