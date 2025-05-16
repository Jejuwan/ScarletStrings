#include "EnginePCH.h"
#include "Composite.h"

CComposite::CComposite(NODE _eNodeType)
	: CNode(_eNodeType)
{
}

void CComposite::Activate()
{
	__super::Activate();
}

void CComposite::Terminate()
{
	__super::Terminate();
}

void CComposite::Add_Child(shared_ptr<CNode> _pNode)
{
	if (nullptr == _pNode)
	{
		MSG_RETURN(, "CComposite::Add_Child", "Nullptr Exception: _pNode");
	}

	m_lstChildren.emplace_back(_pNode);
}

#ifdef _DEBUG
void CComposite::Refresh_Node(shared_ptr<CNode> _pRootNode, shared_ptr<CNode> _pParentNode)
{
	__super::Refresh_Node(_pRootNode, _pParentNode);

	for (auto& pNode : m_lstChildren)
	{
		pNode->Refresh_Node(_pRootNode, shared_from_node());
	}
}
#endif
