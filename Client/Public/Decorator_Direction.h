#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CDecorator_Direction final : public CDecorator
{
private :
	explicit CDecorator_Direction(shared_ptr<CNode> pNode, DIRECTION Direction);
	virtual ~CDecorator_Direction() DEFAULT;

public :
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CTransform>			m_pTransform;
	shared_ptr<CTransform>			m_pTargetTransform;
	DIRECTION						m_eDirection;

public :
	static shared_ptr<CDecorator_Direction> Create(shared_ptr<CBlackBoard> pBlackBoard, DIRECTION Direction, shared_ptr<CNode> _pNode);
};

END