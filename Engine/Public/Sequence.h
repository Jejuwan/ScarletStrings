#pragma once
#include "Engine_Define.h"
#include "Composite.h"

BEGIN(Engine)

class ENGINE_DLL CSequence abstract : public CComposite
{
protected:
	explicit CSequence();
	virtual ~CSequence() DEFAULT;

public:
	virtual void						Activate() override;
	virtual STATUS						Invoke(_float fTimeDelta) override;
	virtual void						Terminate() override;

protected:
	list<shared_ptr<CNode>>::iterator	m_itCurrentChild;
	_uint								m_iChildIndex	= 0u;
};

END
