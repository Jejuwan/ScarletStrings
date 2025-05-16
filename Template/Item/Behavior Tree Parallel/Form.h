#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugParallel.h"
#else
#include "Parallel.h"
#endif

BEGIN(Client)

class C$safeitemname$ final
#ifdef _DEBUG
	: public CDebugParallel
#else
	: public CParallel
#endif
{
private:
	explicit C$safeitemname$();
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
	static shared_ptr<C$safeitemname$>	Create(shared_ptr<class CBlackBoard>);
};

END
