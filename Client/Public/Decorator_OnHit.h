#pragma once
#include "Client_Define.h"
#include "Decorator.h"
#include "Monster.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CDecorator_OnHit final : public CDecorator
{
private :
	explicit CDecorator_OnHit(shared_ptr<CNode> pNode);
	virtual ~CDecorator_OnHit() DEFAULT;

public :
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:

public :
	static shared_ptr<CDecorator_OnHit> Create(shared_ptr<CBlackBoard> pBlackBoard, shared_ptr<CNode> _pNode);
};

END