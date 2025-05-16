#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugSelector.h"
#else
#include "Selector.h"
#endif

BEGIN(Engine)
END

BEGIN(Client)

class CDispenPerrySelector_Hit final
#ifdef _DEBUG
	: public CDebugSelector
#else
	: public CSelector
#endif
{
private:
	explicit CDispenPerrySelector_Hit() DEFAULT;
	virtual ~CDispenPerrySelector_Hit() DEFAULT;

public:
	virtual HRESULT								Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void								Activate() override;
	virtual STATUS								Invoke(_float fTimeDelta) override;
	virtual void								Terminate() override;

public:
#if ACTIVATE_IMGUI
	virtual void								DebugLog() override;
#endif

private:
	weak_ptr<class CMonster_DispenPerry>		m_pOwner;

	DP_STATE*									m_pState		= nullptr;
	_bool*										m_pResetTree	= nullptr;

public:
	static shared_ptr<CDispenPerrySelector_Hit>	Create(shared_ptr<class CBlackBoard>);
};

END
