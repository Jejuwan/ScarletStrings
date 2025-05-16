#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTitleBackground final : public CGameObject
{
private:
	explicit CTitleBackground(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTitleBackground(const CTitleBackground&);
	virtual ~CTitleBackground() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pTransform;
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;
	shared_ptr<CTexture>					m_pTexture;

public:
	static shared_ptr<CTitleBackground>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END