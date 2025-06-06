#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTestAlphaBuffer final : public CGameObject
{
private:
	explicit CTestAlphaBuffer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTestAlphaBuffer(const CTestAlphaBuffer&);
	virtual ~CTestAlphaBuffer() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;

public:
	HRESULT								Set_Texture(shared_ptr<CTexture>);

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CTexture>				m_pTexture;
	shared_ptr<CVIBuffer_Rect>			m_pVIBuffer_Rect;

public:
	static shared_ptr<CTestAlphaBuffer>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END