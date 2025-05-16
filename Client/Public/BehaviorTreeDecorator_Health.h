#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Engine)
END

BEGIN(Client)

class CBehaviorTreeDecorator_Health final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_Health(shared_ptr<CNode>, const ENTITYDESC*, _float, _float);
	virtual ~CBehaviorTreeDecorator_Health() DEFAULT;

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
	const ENTITYDESC*									m_pEntityDesc	= nullptr;
	const _float										m_fMinHP		= 0.f;
	const _float										m_fMaxHP		= 0.f;

public:
	static shared_ptr<CBehaviorTreeDecorator_Health>	Create(shared_ptr<class CBlackBoard>, const ENTITYDESC*, _float fMinHP, _float fMaxHP, shared_ptr<CNode>);
};

END
