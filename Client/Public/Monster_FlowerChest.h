#pragma once
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMonster_FlowerChest final : public CMonster
{
private:
	explicit CMonster_FlowerChest(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_FlowerChest(const CMonster_FlowerChest&);
	virtual ~CMonster_FlowerChest() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;
	virtual wstring						Get_TypeName() { return L"레벨 04 버디 러미"; }; 

private:
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

public:
	static shared_ptr<CMonster_FlowerChest>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	


};

END
