#pragma once
#include "Client_Define.h"
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMonster_Buffalo final : public CMonster
{
private:
	explicit CMonster_Buffalo(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Buffalo(const CMonster_Buffalo&);
	virtual ~CMonster_Buffalo() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual void						RootMotion(_float fTimeDelta, _bool OneFrame = true) override;
	virtual HRESULT						Render_OutLine() override;
	virtual wstring						Get_TypeName() { return L"레벨 12 위더 사바트"; };

	void SetAttackBoneHead() { m_strAttackBoneName = "Head";}
	void SetAttackBoneLeftFoot() { m_strAttackBoneName = "LeftToeBase";}

private:
	virtual HRESULT						Ready_Components() override;

public:
	static shared_ptr<CMonster_Buffalo>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	
};

END
