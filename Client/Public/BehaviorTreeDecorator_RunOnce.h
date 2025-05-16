#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class CBehaviorTreeDecorator_RunOnce final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_RunOnce(shared_ptr<CNode>);
	virtual ~CBehaviorTreeDecorator_RunOnce() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

public:
#if ACTIVATE_IMGUI
	virtual void										DebugLog() override;
#endif

private:
	_bool												m_bExecuted	= false;

public:
	static shared_ptr<CBehaviorTreeDecorator_RunOnce>	Create(shared_ptr<class CBlackBoard>, shared_ptr<CNode>);
};

END
