#pragma once

#include "Client_Define.h"
#include "Teammate_Weapon.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CArashi_Weapon : public CTeammate_Weapon
{
private:
	explicit CArashi_Weapon(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CArashi_Weapon(const CArashi_Weapon&);
	virtual ~CArashi_Weapon() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

private:
	shared_ptr<class CPhysXCollider>	m_pCollider;

public:
	static shared_ptr<CArashi_Weapon>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END