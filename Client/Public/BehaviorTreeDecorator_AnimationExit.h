#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugDecorator.h"
#else
#include "Decorator.h"
#endif

BEGIN(Client)

class CBehaviorTreeDecorator_AnimationExit final
#ifdef _DEBUG
	: public CDebugDecorator
#else
	: public CDecorator
#endif
{
private:
	explicit CBehaviorTreeDecorator_AnimationExit(shared_ptr<CNode>, const _uint, _bool, const string&, const string&, _float, _bool, _float, _bool);
	virtual ~CBehaviorTreeDecorator_AnimationExit() DEFAULT;

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
	weak_ptr<class CMonster>								m_pOwner;

	shared_ptr<CAnimator>									m_pAnimator;
	shared_ptr<CModel>										m_pModel;

	const _uint												m_iAnimationIndex;
	const _float											m_fPlaySpeed;
	const _bool												m_bReverse;
	const _float											m_fInterpolationDuration;
	const _bool												m_bLoop;

	const _bool												m_bRootMotion;
	const string											m_strReferenceBone;
	const string											m_strRootBone;

	_bool													m_bComplete	= false;

public:
	static shared_ptr<CBehaviorTreeDecorator_AnimationExit>	Create(shared_ptr<class CBlackBoard>,
		const string& strReferenceBone, const string& strRootBone, _bool bRootMotion,
		_uint iAnimationIndex, _float fPlaySpeed, _bool bReverse, _float fInterpolationDuration, _bool bLoop, shared_ptr<CNode>);
};

END
