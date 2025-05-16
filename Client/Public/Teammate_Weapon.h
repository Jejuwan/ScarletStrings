#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CTeammate_Weapon abstract : public CGameObject
{
protected:
	explicit CTeammate_Weapon(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTeammate_Weapon(const CTeammate_Weapon&);
	virtual ~CTeammate_Weapon() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Activate_Collider(_bool bActivate);

protected:
	virtual HRESULT						Ready_Components() override;

protected:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

protected:
	shared_ptr<class CPhysXCollider>	m_pCollider;
};

END