#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_RelativeAngle.h"

CBehaviorTreeDecorator_RelativeAngle::CBehaviorTreeDecorator_RelativeAngle(shared_ptr<CNode> _pNode, shared_ptr<CTransform> _pOwner, shared_ptr<CTransform> _pTarget, _float _fThreshold)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_pTransform		(_pOwner)
	, m_pTargetTransform(_pTarget)
	, m_fThreshold		(_fThreshold)
{
}

HRESULT CBehaviorTreeDecorator_RelativeAngle::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "RelativeAngle";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_RelativeAngle::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_RelativeAngle::Activate()
{
	__super::Activate();
}

STATUS CBehaviorTreeDecorator_RelativeAngle::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pNode->Is_Running())
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else if (fabsf(Function::RelativeAngle(m_pTransform, m_pTargetTransform)) >= m_fThreshold)
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CBehaviorTreeDecorator_RelativeAngle::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_RelativeAngle::DebugLog()
{
}
#endif

shared_ptr<CBehaviorTreeDecorator_RelativeAngle> CBehaviorTreeDecorator_RelativeAngle::Create(shared_ptr<CBlackBoard> _pBlackBoard, shared_ptr<CTransform> _pOwner, shared_ptr<CTransform> _pTarget, _float _fThreshold, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_RelativeAngle> pInstance = make_private_shared(CBehaviorTreeDecorator_RelativeAngle, _pNode, _pOwner, _pTarget, _fThreshold);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_RelativeAngle::Create", "Failed to Initialize");
	}

	return pInstance;
}
