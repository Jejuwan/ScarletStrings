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

class CDispenPerryLeaf_Damage final
#ifdef _DEBUG
	: public CDebugNode
#else
	: public CNode
#endif
{
private:
	explicit CDispenPerryLeaf_Damage() DEFAULT;
	virtual ~CDispenPerryLeaf_Damage() DEFAULT;

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

	ANIMATION::DISPEN_PERRY						m_eAnimation;

	_bool										m_bSuccess	= false;

public:
	static shared_ptr<CDispenPerryLeaf_Damage>	Create(shared_ptr<class CBlackBoard>);
};

END
