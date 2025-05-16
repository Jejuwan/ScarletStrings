#include "EnginePCH.h"
#include "Node.h"

CNode::CNode(NODE _eNodeType)
	: m_eNodeType(_eNodeType)
{
}

HRESULT CNode::Initialize(shared_ptr<CBlackBoard> pBlackBoard)
{
	m_pBlackBoard = pBlackBoard;

	return S_OK;
}

void CNode::Activate()
{
	m_eStatus	= STATUS::RUNNING;

	m_fTimeAcc	= 0.f;
}

void CNode::Terminate()
{
	m_eStatus = STATUS::MAX;
}

#ifdef _DEBUG
void CNode::Register_DebugNode(shared_ptr<CNode> _pNode)
{
	if (m_bRootNode)
	{
		m_lstNodeDebug.emplace_back(_pNode);
	}
}

void CNode::Refresh_Node(shared_ptr<CNode> _pRootNode, shared_ptr<CNode> _pParentNode)
{
	if (!_pParentNode)
	{
		return;
	}

	m_iNodeDepth = _pParentNode->m_iNodeDepth + 1u;
	_pRootNode->Register_DebugNode(shared_from_node());
}
#endif

void CNode::Begin_Invoke(_float _fTimeDelta)
{
#ifdef _DEBUG
	if (m_bRootNode)
	{
		Debug();
	}
#endif

	if (STATUS::RUNNING != m_eStatus)
	{
		Activate();
	}

	m_fTimeAcc += _fTimeDelta;
}

STATUS CNode::Return_Invoke()
{
	STATUS eReturn = m_eStatus;

	if (STATUS::RUNNING != m_eStatus)
	{
		Terminate();
	}

	return eReturn;
}

#ifdef _DEBUG
void CNode::Debug(any)
{
//	if (m_bRootNode)
//	{
//		for (auto& pNode : m_lstNodeDebug)
//		{
//			pNode->Debug();
//		}
//	}
}
#endif
