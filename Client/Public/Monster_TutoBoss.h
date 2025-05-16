#pragma once
#include "Client_Define.h"
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMonster_TutoBoss final : public CMonster
{
private:
	explicit CMonster_TutoBoss(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_TutoBoss(const CMonster_TutoBoss&);
	virtual ~CMonster_TutoBoss() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_Bloom() override;
	virtual void						RootMotion(_float fTimeDelta, _bool OneFrame = true) override;
	virtual wstring						Get_TypeName() { return L"∑π∫ß 10 πŸ¿Œ øÊ"; };

	virtual HRESULT						Render_OutLine() override;

	virtual void						SetWeaponeActiveOn() override;
	virtual void						SetWeaponeActiveOff() override;
	void SetAttackBoneAndSwap1();
	void SetAttackBoneAndSwap2();

public:
	void								On_HitBlink();

private:
	virtual HRESULT						Ready_Components() override;

	shared_ptr<class CPhysXCollider>	m_pAttack1Collider;
	shared_ptr<class CPhysXCollider>	m_pAttack2Collider;

	ACTORDESC* m_pAttack1ActorDesc = { nullptr };
	ACTORDESC* m_pAttack2ActorDesc = { nullptr };

private:
	_float								m_fBloomStrength = 30.f;
	_float								m_fBloomStrengthThreshold = { 5.f };

public:
	static shared_ptr<CMonster_TutoBoss>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	
};

END
