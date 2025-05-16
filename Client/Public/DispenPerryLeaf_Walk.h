#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugNode.h"
#else
#include "Node.h"
#endif

BEGIN(Engine)
END

BEGIN(Client)

class CDispenPerryLeaf_Walk final
#ifdef _DEBUG
	: public CDebugNode
#else
	: public CNode
#endif
{
private:
	explicit CDispenPerryLeaf_Walk(DIRECTION);
	virtual ~CDispenPerryLeaf_Walk() DEFAULT;

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
	const ENTITYDESC*							m_pEntityDesc	= nullptr;

	shared_ptr<CTransform>						m_pTransform;
	shared_ptr<CModel>							m_pModel;
//	shared_ptr<CPhysics>						m_pPhysics;
	shared_ptr<CAnimator>						m_pAnimator;

	shared_ptr<CTransform>						m_pTargetTransform;

	const DIRECTION								m_eDirection	= DIRECTION::DIR_END;
	ANIMATION::DISPEN_PERRY						m_eAnimation	= ANIMATION::DISPEN_PERRY::MAX;
//	ANIMATION::DISPEN_PERRY						m_eStart		= ANIMATION::DISPEN_PERRY::MAX;
//	ANIMATION::DISPEN_PERRY						m_eRoop			= ANIMATION::DISPEN_PERRY::MAX;

public:
	static shared_ptr<CDispenPerryLeaf_Walk>	Create(shared_ptr<class CBlackBoard>, DIRECTION);
};

END
