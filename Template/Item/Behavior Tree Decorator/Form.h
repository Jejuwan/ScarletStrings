#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class C$safeitemname$ final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit C$safeitemname$(shared_ptr<CNode>);
	virtual ~C$safeitemname$() DEFAULT;

public:
	virtual HRESULT						Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void						Activate() override;
	virtual STATUS						Invoke(_float fTimeDelta) override;
	virtual void						Terminate() override;

public:
#if ACTIVATE_IMGUI
	virtual void						DebugLog() override;
#endif

public:
	static shared_ptr<C$safeitemname$>	Create(shared_ptr<class CBlackBoard>, shared_ptr<CNode>);
};

END
