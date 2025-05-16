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

class CDispenPerrySelector_Phase01 final
#ifdef _DEBUG
	: public CDebugSelector
#else
	: public CSelector
#endif
{
private:
	explicit CDispenPerrySelector_Phase01() DEFAULT;
	virtual ~CDispenPerrySelector_Phase01() DEFAULT;

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
	const DP_PHASE*									m_pDPPhase	= nullptr;

	shared_ptr<CTransform>							m_pTransform;
	shared_ptr<CTransform>							m_pTargetTransform;
	shared_ptr<class CSkillContainer>				m_pSkillContainer;

public:
	static shared_ptr<CDispenPerrySelector_Phase01>	Create(shared_ptr<class CBlackBoard>);
};

END
