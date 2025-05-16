#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugSelector.h"
#else
#include "Selector.h"
#endif

BEGIN(Client)

class CDispenPerrySelector_SetField final
#ifdef _DEBUG
	: public CDebugSelector
#else
	: public CSelector
#endif
{
private:
	explicit CDispenPerrySelector_SetField() DEFAULT;
	virtual ~CDispenPerrySelector_SetField() DEFAULT;

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
	shared_ptr<CTransform>								m_pTransform;
	shared_ptr<CAnimator>								m_pAnimator;

	shared_ptr<CTransform>								m_pTargetTransform;

	_bool												m_bEnter	= false;

public:
	static shared_ptr<CDispenPerrySelector_SetField>	Create(shared_ptr<class CBlackBoard>);
};

END
