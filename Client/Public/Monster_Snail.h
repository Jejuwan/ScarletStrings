#pragma once
#include "Client_Define.h"
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMonster_Snail final : public CMonster
{
private:
	explicit CMonster_Snail(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Snail(const CMonster_Snail&);
	virtual ~CMonster_Snail() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual void						RootMotion(_float fTimeDelta, _float fAmount, _bool OneFrame = true);
	virtual wstring						Get_TypeName() { return L"레벨 05 바일 풀"; };

	virtual HRESULT						Render_OutLine() override;
	virtual void						ShootBulletToPlayer() override;

private:
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

public:
	static shared_ptr<CMonster_Snail>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	
};

END
