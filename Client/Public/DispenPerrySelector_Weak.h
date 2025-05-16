#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugSelector.h"
#else
#include "Selector.h"
#endif

BEGIN(Client)

class CDispenPerrySelector_Weak final
#ifdef _DEBUG
	: public CDebugSelector
#else
	: public CSelector
#endif
{
private:
	explicit CDispenPerrySelector_Weak() DEFAULT;
	virtual ~CDispenPerrySelector_Weak() DEFAULT;

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
	weak_ptr<class CMonster_DispenPerry>			m_pOwner;

	_bool											m_bSuccess	= false;

public:
	static shared_ptr<CDispenPerrySelector_Weak>	Create(shared_ptr<class CBlackBoard>);
};

END
