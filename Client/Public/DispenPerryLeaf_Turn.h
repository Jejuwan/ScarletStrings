#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugNode.h"
#else
#include "Node.h"
#endif

BEGIN(Client)

class CDispenPerryLeaf_Turn final
#ifdef _DEBUG
	: public CDebugNode
#else
	: public CNode
#endif
{
private:
	explicit CDispenPerryLeaf_Turn(DIRECTION);
	virtual ~CDispenPerryLeaf_Turn() DEFAULT;

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
	shared_ptr<CTransform>						m_pTransform;
	shared_ptr<CModel>							m_pModel;
	shared_ptr<CAnimator>						m_pAnimator;

	shared_ptr<CTransform>						m_pTargetTransform;

	const ENTITYDESC*							m_pEntityDesc			= nullptr;

	const DIRECTION								m_eDirection			= DIRECTION::DIR_END;
	ANIMATION::DISPEN_PERRY						m_eAnimation			= ANIMATION::DISPEN_PERRY::MAX;
	ANIMATION::DISPEN_PERRY						m_eAnimationRoop		= ANIMATION::DISPEN_PERRY::MAX;

	_bool										m_bComplete				= false;
	_float										m_fInitialRelative		= 0.f;
	_float										m_fRelativeHorizontal	= 0.f;

	_float4x4									m_mPivot				= g_mUnit;
	const _float4x4*							m_pBone					= nullptr;
	_bool										m_bInitialAnimation		= false;

public:
	static shared_ptr<CDispenPerryLeaf_Turn>	Create(shared_ptr<class CBlackBoard>, DIRECTION);
};

END
