#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class CBehaviorTreeDecorator_Random final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_Random(initializer_list<shared_ptr<CNode>>, initializer_list<_float>);
	virtual ~CBehaviorTreeDecorator_Random() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

public:
#ifdef _DEBUG
	virtual void										Refresh_Node(shared_ptr<CNode>, shared_ptr<CNode>) override;
	virtual void										Debug(any = g_aNull) override;
#if ACTIVATE_IMGUI
	virtual void										DebugLog() override;
#endif
#endif

private:
	vector<_float>										m_vecProbabilities;
	vector<shared_ptr<CNode>>							m_vecNodes;

	unordered_set<shared_ptr<CNode>>					m_usetSearchedNodes;

public:
	static shared_ptr<CBehaviorTreeDecorator_Random>	Create(shared_ptr<class CBlackBoard>, initializer_list<_float>, initializer_list<shared_ptr<CNode>>);
};

END
