#pragma once
#include "Engine_Define.h"
#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CComposite abstract : public CNode
{
protected:
	explicit CComposite(NODE);
	virtual ~CComposite() DEFAULT;

public:
	virtual void					Activate() override;
	virtual STATUS					Invoke(_float fTimeDelta) override	PURE;
	virtual void					Terminate() override;

public:
	void							Add_Child(shared_ptr<CNode> pNode);

public:
#ifdef _DEBUG
	virtual void					Refresh_Node(shared_ptr<CNode>, shared_ptr<CNode> = nullptr) override;
#endif

protected:
	list<shared_ptr<CNode>>			m_lstChildren;
};

END
