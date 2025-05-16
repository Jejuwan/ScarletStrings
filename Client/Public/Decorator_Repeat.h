#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Engine)
END

BEGIN(Client)

class CDecorator_Repeat final : public CDecorator
{
private:
	explicit CDecorator_Repeat(shared_ptr<CNode>, _float fRepeat);
	virtual ~CDecorator_Repeat() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void							Activate() override;
	virtual STATUS							Invoke(_float fTimeDelta) override;
	virtual void							Terminate() override;

private:
	const _float							m_fRepeat;

public:
	static shared_ptr<CDecorator_Repeat>	Create(shared_ptr<class CBlackBoard>, _float fRepeat,shared_ptr<CNode>);
};

END
