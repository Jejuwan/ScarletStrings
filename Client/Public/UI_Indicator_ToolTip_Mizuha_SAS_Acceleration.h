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

class CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration final : public CGameObject
{
private:
	explicit CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration() DEFAULT;

public:
	virtual HRESULT														Initialize_Prototype() override;
	virtual HRESULT														Initialize(any _aDesc = g_aNull)	override;
	virtual void														Tick(_float fTimeDelta) override;
	virtual void														Late_Tick(_float fTimeDelta) override;
	virtual HRESULT														Render() override;

private:
	virtual HRESULT														Ready_Components() override;

private:
	void																Initialize_UI();
	void																Tick_UI(_float fTimeDelta);
	void																Movie_System(_float fTimeDelta);
	void																Sound(_float fTimeDelta);

public:
	void																Movie_Start();
	void																Movie_End(_float fTimeDelta);

private:
	shared_ptr<CTransform>												m_pBackground;
	shared_ptr<CTransform>												m_pMovie;
	shared_ptr<CTransform>												m_pSeperate_Line1;
	shared_ptr<CTransform>												m_pSeperate_Line2;

private:
	shared_ptr<CTransform>												m_pKey_ENTER;

private:
	shared_ptr<CTexture>												m_pTexture_Background;
	shared_ptr<CTexture>												m_pTexture_Movie;
	shared_ptr<CTexture>												m_pTexture_SeperateLine1;
	shared_ptr<CTexture>												m_pTexture_SeperateLine2;

private:
	shared_ptr<CTexture>												m_pTexture_Key_ENTER;

private:
	shared_ptr<CShader>													m_pShader;
	shared_ptr<CVIBuffer_Rect>											m_pVIBuffer_Rect;

private:
	_bool																m_bMovieStart;
	_bool																m_bScaleUp;
	_bool																m_bToolTip_End;
	_bool																m_bIsRunning;

private:
	_uint																m_iSoundIndex;

private:
	_float																m_fCurrentMovieIndex;

	_float																m_fMovieBackgroundTime;
	_float																m_fMovieShaderTime;
	_float																m_fSeperateLineTime;
	_float																m_fScaleX_ToolTip;

private:
	_float3																m_vScale_Background;
	_float3																m_vScale_Movie;
	_float3																m_vScale_SeperateLine1;
	_float3																m_vScale_SeperateLine2;

private:
	_float3																m_vScale_Key_ENTER;

private:
	_float4																m_vPosition_Background;
	_float4																m_vPosition_Movie;
	_float4																m_vPosition_SeperateLine1;
	_float4																m_vPosition_SeperateLine2;

private:
	_float4																m_vPosition_Key_ENTER;

private:
	wstring																m_strTitle;
	wstring																m_strExplanation1;
	wstring																m_strExplanation2;
	wstring																m_strExplanation3;
	wstring																m_strExplanation4;
	wstring																m_strExplanation5;
	wstring																m_strEnter;

public:
	static shared_ptr<CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>										Clone(any = g_aNull) override;
};

END