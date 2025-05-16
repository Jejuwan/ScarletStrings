#pragma once
#include "Engine_Define.h"
#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CDecorator abstract : public CNode
{
protected:
	explicit CDecorator(shared_ptr<CNode>);
	virtual ~CDecorator() DEFAULT;

public:
	virtual void					Activate() override;
	virtual STATUS					Invoke(_float fTimeDelta) override	PURE;
	virtual void					Terminate() override;

public:
#ifdef _DEBUG
	virtual void					Refresh_Node(shared_ptr<CNode>, shared_ptr<CNode> = nullptr) override;
#endif

protected:
	shared_ptr<CNode>				m_pNode;
};

END
