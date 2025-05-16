#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class CBehaviorTreeDecorator_AnimationPrevent final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_AnimationPrevent(shared_ptr<CNode>, initializer_list<_uint>);
	virtual ~CBehaviorTreeDecorator_AnimationPrevent() DEFAULT;

public:
	virtual HRESULT												Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void												Activate() override;
	virtual STATUS												Invoke(_float fTimeDelta) override;
	virtual void												Terminate() override;

public:
#if ACTIVATE_IMGUI
	virtual void												DebugLog() override;
#endif

private:
	const _bool													Is_PlayingPreventAnimation() const;

private:
	shared_ptr<CAnimator>										m_pAnimator;

	list<_uint>													m_lstPrevent;

public:
	static shared_ptr<CBehaviorTreeDecorator_AnimationPrevent>	Create(shared_ptr<class CBlackBoard>, initializer_list<_uint> ilPrevent, shared_ptr<CNode>);
};

END
