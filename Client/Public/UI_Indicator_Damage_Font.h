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

class CUI_Indicator_Damage_Font : public CGameObject
{
private:
	explicit CUI_Indicator_Damage_Font(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Damage_Font() DEFAULT;

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
	

public:
	void												Setting_RandomDamage();
	void												Setting_DamagePosition(_matrixf _WorldMatrix);
	void												DamageFont_Rendering(_float fTimeDelta);

private:
	shared_ptr<CTransform>								m_pOnes;
	shared_ptr<CTransform>								m_pTens;
	shared_ptr<CTransform>								m_pHundreds;
	shared_ptr<CTransform>								m_pNoDamage;

private:
#pragma region Damage - Normal
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
#pragma endregion
#pragma region Damage - Critical
	shared_ptr<CTexture>								m_pTexture_C_Zero;
	shared_ptr<CTexture>								m_pTexture_C_One;
	shared_ptr<CTexture>								m_pTexture_C_Two;
	shared_ptr<CTexture>								m_pTexture_C_Three;
	shared_ptr<CTexture>								m_pTexture_C_Four;
	shared_ptr<CTexture>								m_pTexture_C_Five;
	shared_ptr<CTexture>								m_pTexture_C_Six;
	shared_ptr<CTexture>								m_pTexture_C_Seven;
	shared_ptr<CTexture>								m_pTexture_C_Eight;
	shared_ptr<CTexture>								m_pTexture_C_Nine;
#pragma endregion
#pragma region No Damage
	shared_ptr<CTexture>								m_pTexture_NoDamage;
#pragma endregion

private:
	shared_ptr<CShader>									m_pShader;
	shared_ptr<CVIBuffer_Rect>							m_pVIBuffer_Rect;

private:
	_bool												m_bDamage_RenderEnd;
	_bool												m_bCanRendom;
	_bool												m_bRenderDegree;

private:
	_uint												m_iDamage_Type;
	_uint												m_iDamage_RendomNumber;

	_uint												m_iDamage_Ones;
	_uint												m_iDamage_Tens;
	_uint												m_iDamage_Hundreds;

	_uint												m_iRenderFontPhase;

private:
	_float												m_fAccTime;
	_float												m_fFontAlpha;
	_float												m_fDamageFont_Rise;
	_float												m_fDamageAppearRange;

private:
	_float3												m_vScale_Font;
	_float3												m_vScale_C_Font;
	_float3												m_vScale_NoDamage;

	_float4												m_vPosition_Font_H;
	_float4												m_vPosition_Font_T;
	_float4												m_vPosition_Font_O;
	_float4												m_vPosition_Font_NoDamage;

	_float4x4											m_HitPosition;


public:
	static shared_ptr<CUI_Indicator_Damage_Font>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;

};

END