#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUI_Menu_Models final : public CGameObject
{
private:
	explicit CUI_Menu_Models(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Models() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pTransform;
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CModel>						m_pModel;

private:
	class CPlayer*							m_pPlayer;
	shared_ptr<class CCamera_Player>		m_pCamera;

private:
	_float3									m_vCameraPosition;
	_float3									m_vSofaPosition;

public:
	static shared_ptr<CUI_Menu_Models>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END