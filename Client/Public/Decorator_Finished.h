#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CDecorator_Finished final : public CDecorator
{
private :
	explicit CDecorator_Finished(shared_ptr<CNode> pNode);
	virtual ~CDecorator_Finished() DEFAULT;

public :
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CTransform>			m_pTransform;
	shared_ptr<CTransform>			m_pTargetTransform;

public :
	static shared_ptr<CDecorator_Finished> Create(shared_ptr<CBlackBoard> pBlackBoard, shared_ptr<CNode> _pNode);
};

END