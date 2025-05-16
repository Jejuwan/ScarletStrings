#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugSelector.h"
#else
#include "Selector.h"
#endif

BEGIN(Client)

class CDispenPerrySelector_LeapingBodySlam final
#ifdef _DEBUG
	: public CDebugSelector
#else
	: public CSelector
#endif
{
private:
	explicit CDispenPerrySelector_LeapingBodySlam() DEFAULT;
	virtual ~CDispenPerrySelector_LeapingBodySlam() DEFAULT;

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
	weak_ptr<class CMonster_DispenPerry>					m_pOwner;

	shared_ptr<CTransform>									m_pTransform;
	shared_ptr<CPhysics>									m_pPhysics;
	shared_ptr<CAnimator>									m_pAnimator;

public:
	static shared_ptr<CDispenPerrySelector_LeapingBodySlam>	Create(shared_ptr<class CBlackBoard>);
};

END
