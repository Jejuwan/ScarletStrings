#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugSequence.h"
#else
#include "Sequence.h"
#endif

BEGIN(Client)

class CDispenPerrySequence_LeapingBodySlam final
#ifdef _DEBUG
	: public CDebugSequence
#else
	: public CSequence
#endif
{
private:
	explicit CDispenPerrySequence_LeapingBodySlam() DEFAULT;
	virtual ~CDispenPerrySequence_LeapingBodySlam() DEFAULT;

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

	shared_ptr<CTransform>									m_pTargetTransform;

	_bool													m_bIsJumping		= false;
	_float													m_fEffect_Timer = { 1.f };
	_float													m_fEffect_Timer2 = { 0.f };

private:
	_float													m_fLockOnDelay		= 1.3f;
	_float													m_fLockOnAccTime	= 0.f;

public:
	static shared_ptr<CDispenPerrySequence_LeapingBodySlam>	Create(shared_ptr<class CBlackBoard>);
};

END
