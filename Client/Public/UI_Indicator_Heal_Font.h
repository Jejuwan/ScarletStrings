#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Indicator_Heal_Font : public CGameObject
{
private:
	explicit CUI_Indicator_Heal_Font(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Heal_Font() DEFAULT;

public:
	virtual HRESULT										Initialize_Prototype() override;
	virtual HRESULT										Initialize(any _aDesc = g_aNull)	override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;

private:
	virtual HRESULT										Ready_Components() override;
	virtual _bool										Return() override;
	virtual HRESULT										Fetch(any = g_aNull) override;

private:
	void												Initialize_UI();
	void												Tick_UI(_float fTimeDelta);

private:
	void												Setting_SplitHeal();
	void												Setting_HealPosition(_matrixf _WorldMatrix);
	void												HealFont_Rendering(_float fTimeDelta);

private:
	shared_ptr<CTransform>								m_pOnes;
	shared_ptr<CTransform>								m_pTens;
	shared_ptr<CTransform>								m_pHundreds;

private:
	shared_ptr<CTexture>								m_pTexture_Zero;
	shared_ptr<CTexture>								m_pTexture_One;
	shared_ptr<CTexture>								m_pTexture_Two;
	shared_ptr<CTexture>								m_pTexture_Three;
	shared_ptr<CTexture>								m_pTexture_Four;
	shared_ptr<CTexture>								m_pTexture_Five;
	shared_ptr<CTexture>								m_pTexture_Six;
	shared_ptr<CTexture>								m_pTexture_Seven;
	shared_ptr<CTexture>								m_pTexture_Eight;
	shared_ptr<CTexture>								m_pTexture_Nine;

private:
	shared_ptr<CShader>									m_pShader;
	shared_ptr<CVIBuffer_Rect>							m_pVIBuffer_Rect;

private:
	_bool												m_bHeal_RenderEnd;
	_bool												m_bRenderDegree;

private:
	_uint												m_iHeal_Ones;
	_uint												m_iHeal_Tens;
	_uint												m_iHeal_Hundreds;

	_uint												m_iHeal_Split;
	_uint												m_iRenderFontPhase;

private:
	_float												m_fRenderFontDelayTime;
	_float												m_fAccTime;
	_float												m_fFontAlpha;
	_float												m_fHealFont_Rise;

private:
	_float3												m_vScale_Font;

	_float4												m_vPosition_Font_H;
	_float4												m_vPosition_Font_T;
	_float4												m_vPosition_Font_O;
	_float4x4											m_matPlayerPosition;
public:
	static shared_ptr<CUI_Indicator_Heal_Font>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;
};

END