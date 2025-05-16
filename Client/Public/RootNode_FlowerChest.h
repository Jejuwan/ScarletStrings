#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Client)

class CRootNode_FlowerChest :
    public CNode
{
private :
    explicit CRootNode_FlowerChest() DEFAULT;
    virtual ~CRootNode_FlowerChest() DEFAULT;


public:
	virtual HRESULT							Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void							Activate() override;
	virtual STATUS							Invoke(_float fTimeDelta) override;
	virtual void							Terminate() override;

private:
	HRESULT									Ready_Skills();

private:
	const FC_PHASE* m_pPhase = nullptr;
	shared_ptr<CNode>						m_pPhaseSelector;

	map<wstring, SKILLDESC>					m_mapSkills;

public:
	static shared_ptr<CRootNode_FlowerChest>	Create(shared_ptr<class CBlackBoard>);
};

END


