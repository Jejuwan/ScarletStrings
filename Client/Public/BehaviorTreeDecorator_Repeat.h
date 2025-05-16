#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class CBehaviorTreeDecorator_Repeat final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_Repeat(shared_ptr<CNode>, _float fTime);
	explicit CBehaviorTreeDecorator_Repeat(shared_ptr<CNode>, _uint iCount);
	virtual ~CBehaviorTreeDecorator_Repeat() DEFAULT;

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
	const _float										m_fTime		= 0.f;
	const _uint											m_iRepeat	= 0u;

	_uint												m_iCount	= 0u;

public:
	static shared_ptr<CBehaviorTreeDecorator_Repeat>	Create(shared_ptr<class CBlackBoard>, _float fTime, shared_ptr<CNode>);
	static shared_ptr<CBehaviorTreeDecorator_Repeat>	Create(shared_ptr<class CBlackBoard>, _uint iCount, shared_ptr<CNode>);
};

END
