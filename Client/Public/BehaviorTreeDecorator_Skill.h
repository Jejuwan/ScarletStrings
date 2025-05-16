 #pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class CBehaviorTreeDecorator_Skill final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_Skill(shared_ptr<CNode>, shared_ptr<class CSkill>, _bool);
	virtual ~CBehaviorTreeDecorator_Skill() DEFAULT;

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
	shared_ptr<class CSkill>						m_pSkill;

	const _bool										m_bManageAfterExit	= false;

public:
	static shared_ptr<CBehaviorTreeDecorator_Skill>	Create(shared_ptr<class CBlackBoard>, shared_ptr<class CSkill>, _bool bManualStockManage, shared_ptr<CNode>);
};

END
