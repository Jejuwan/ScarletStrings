#pragma once
#include "Client_Define.h"
#include "Decorator.h"
#include "Monster.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CDecorator_CoolTime final : public CDecorator
{
private :
	explicit CDecorator_CoolTime(shared_ptr<CNode> pNode);
	virtual ~CDecorator_CoolTime() DEFAULT;

public :
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	//CMonster::MonsterDesc* m_pMonsterDesc = nullptr;

public :
	static shared_ptr<CDecorator_CoolTime> Create(shared_ptr<CBlackBoard> pBlackBoard, shared_ptr<CNode> _pNode);
};

END