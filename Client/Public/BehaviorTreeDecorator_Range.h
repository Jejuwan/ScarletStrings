#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class CBehaviorTreeDecorator_Range final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_Range(shared_ptr<CNode>, _float, _float);
	virtual ~CBehaviorTreeDecorator_Range() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

public:
#if ACTIVATE_IMGUI
	virtual void									DebugLog() override;
#endif

private:
	shared_ptr<CTransform>							m_pTransform;

	shared_ptr<CTransform>							m_pTargetTransform;

	const _float									m_fMinRange	= 0.f;
	const _float									m_fMaxRange	= 0.f;

public:
	static shared_ptr<CBehaviorTreeDecorator_Range>	Create(shared_ptr<class CBlackBoard>, _float fMinRange, _float fMaxRange, shared_ptr<CNode>);
};

END
