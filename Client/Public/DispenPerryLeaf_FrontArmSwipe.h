#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugNode.h"
#else
#include "Node.h"
#endif

BEGIN(Client)

class CDispenPerryLeaf_FrontArmSwipe final
#ifdef _DEBUG
	: public CDebugNode
#else
	: public CNode
#endif
{
private:
	explicit CDispenPerryLeaf_FrontArmSwipe() DEFAULT;
	virtual ~CDispenPerryLeaf_FrontArmSwipe() DEFAULT;

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
	weak_ptr<class CMonster_DispenPerry>				m_pOwner;
	shared_ptr<CModel>									m_pModel;
	shared_ptr<CAnimator>								m_pAnimator;

public:
	static shared_ptr<CDispenPerryLeaf_FrontArmSwipe>	Create(shared_ptr<class CBlackBoard>);
};

END
