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

class CUI_Shop_Fade final : public CGameObject
{
private:
	explicit CUI_Shop_Fade(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Shop_Fade() DEFAULT;


public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any _aDesc = g_aNull)	override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

private:
	virtual HRESULT								Ready_Components() override;

public:
	void										Initialize_UI();
	void										Tick_UI(_float fTimeDelta);
	void										Fade_InOut(_float fTimeDelta);

private:
	shared_ptr<CTransform>						m_pFade;
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CTexture>						m_pTexture_Fade;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;

private:
	_bool			m_bEnter_ShopUI = { false };
	_uint										m_iSoundIndex;

	_float										m_fFadeAlpha;
	_float										m_fSoundDelay;

	_float3										m_vScale_Fade;

	_float4										m_vPosition_Fade;

public:
	static shared_ptr<CUI_Shop_Fade>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END 