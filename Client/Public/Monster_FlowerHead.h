 #pragma once
#include "Client_Define.h"
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMonster_FlowerHead final : public CMonster
{
private:
	explicit CMonster_FlowerHead(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_FlowerHead(const CMonster_FlowerHead&);
	virtual ~CMonster_FlowerHead() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual void						RootMotion(_float fTimeDelta, _bool OneFrame = true) override;
	virtual wstring						Get_TypeName() { return L"레벨 06 바스 포즈"; };

	virtual HRESULT						Render_OutLine() override;
	void SetWeaphoneBoneNameLeft()		{ m_strAttackBoneName = "LeftFoot"; m_pAttackActorDesc->bActive = true; };
	void SetWeaphoneBoneNameRight()		{ m_strAttackBoneName = "RightFoot"; m_pAttackActorDesc->bActive = true; };
	void SetAttackColliderOn()			{ m_pAttackActorDesc->bActive = false; };

private:
	virtual HRESULT						Ready_Components() override;

	virtual HRESULT						Ready_Behaviors() override;

public:
	static shared_ptr<CMonster_FlowerHead>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	
};

END
