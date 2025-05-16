#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class CBehaviorTreeDecorator_RelativeAngle final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_RelativeAngle(shared_ptr<CNode>, shared_ptr<CTransform>, shared_ptr<CTransform>, _float);
	virtual ~CBehaviorTreeDecorator_RelativeAngle() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

public:
#if ACTIVATE_IMGUI
	virtual void											DebugLog() override;
#endif

private:
	const _float											m_fThreshold	= 0.f;

	shared_ptr<CTransform>									m_pTransform;
	shared_ptr<CTransform>									m_pTargetTransform;

public:
	static shared_ptr<CBehaviorTreeDecorator_RelativeAngle>	Create(shared_ptr<class CBlackBoard>, shared_ptr<CTransform> pOwner, shared_ptr<CTransform> pTarget, _float fThreshold, shared_ptr<CNode>);
};

END
