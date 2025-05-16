#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Client)

class CNoiseCloud final : public CGameObject
{
private:
	explicit							CNoiseCloud(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit							CNoiseCloud(const CNoiseCloud&);
	virtual								~CNoiseCloud() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	HRESULT								Render() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	_float								m_fNoiseAccTime = { 0.f };

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CTexture>				m_pTexture;

public:
	static shared_ptr<CNoiseCloud>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END