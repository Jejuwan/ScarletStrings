#include "EnginePCH.h"
#include "Decorator.h"

CDecorator::CDecorator(shared_ptr<CNode> _pNode)
	: CNode(NODE::DECORATOR)
	, m_pNode(_pNode)
{
}

void CDecorator::Activate()
{
	__super::Activate();
}

void CDecorator::Terminate()
{
	__super::Terminate();
}

#ifdef _DEBUG
void CDecorator::Refresh_Node(shared_ptr<CNode> _pRootNode, shared_ptr<CNode> _pParentNode)
{
	__super::Refresh_Node(_pRootNode, _pParentNode);
	m_pNode->Refresh_Node(_pRootNode, _pParentNode);
}
#endif
