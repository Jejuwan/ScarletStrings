#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CObjectLayer;
class CPlayer;
END

BEGIN(Client)

class CUI_Indicator_Monster_MissPlayer final : public CGameObject
{
private:
	explicit CUI_Indicator_Monster_MissPlayer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Monster_MissPlayer() DEFAULT;

public:
	virtual HRESULT										Initialize_Prototype() override;
	virtual HRESULT										Initialize(any _aDesc = g_aNull)	override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;

private:
	virtual HRESULT										Ready_Components() override;

private:
	void												Initialize_UI();
	void												Tick_UI(_float fTimeDelta);
	void												Set_ScalingSystem(_float fTimeDelta);

public:
	void												Call_Indicator();

public:
	void												NoticePlayer() { m_bRenderSwitch = true; }
	void												Set_IndicatorPosition(_matrixf _WorldMatrix);
	void												Reset_Initialize();

private:
	shared_ptr<CTexture>								m_pTexture_Indicator_M1;
	shared_ptr<CTexture>								m_pTexture_Indicator_M2;
	shared_ptr<CTexture>								m_pTexture_Indicator_M3;
	shared_ptr<CTexture>								m_pTexture_Indicator_L1;
	shared_ptr<CTexture>								m_pTexture_Indicator_L2;
	shared_ptr<CTexture>								m_pTexture_Indicator_R1;
	shared_ptr<CTexture>								m_pTexture_Indicator_R2;

private:
	shared_ptr<CTransform>								m_pIndicator_M1;
	shared_ptr<CTransform>								m_pIndicator_M2;
	shared_ptr<CTransform>								m_pIndicator_M3;
	shared_ptr<CTransform>								m_pIndicator_L1;
	shared_ptr<CTransform>								m_pIndicator_L2;
	shared_ptr<CTransform>								m_pIndicator_R1;
	shared_ptr<CTransform>								m_pIndicator_R2;

private:
	shared_ptr<CShader>									m_pShader;
	shared_ptr<CVIBuffer_Rect>							m_pVIBuffer_Rect;

private:
	_bool												m_bRenderSwitch;
	_bool												m_bScaleDownStart;
	_bool												m_bScaleDisappear;
	_bool												m_bRenderDegree;

private:
	_uint												m_iRenderPhase;

	_int												m_iTextureShift;

private:
	_float												m_fTime;
	_float												m_fAccTime;
	_float												m_fShaderTime;
	_float												m_fShaderIndex;
	_float												m_fTexture3Shader;
	_float												m_fRenderEndTime;

private:
	_float3												m_vScale_Indicator_M1;
	_float3												m_vScale_Indicator_M2;
	_float3												m_vScale_Indicator_M3;
	_float3												m_vScale_Indicator_L1;
	_float3												m_vScale_Indicator_L2;
	_float3												m_vScale_Indicator_R1;
	_float3												m_vScale_Indicator_R2;

private:
	_float4												m_vPosition_Indicator_M1;
	_float4												m_vPosition_Indicator_M2;
	_float4												m_vPosition_Indicator_M3;
	_float4												m_vPosition_Indicator_L1;
	_float4												m_vPosition_Indicator_L2;
	_float4												m_vPosition_Indicator_R1;
	_float4												m_vPosition_Indicator_R2;

public:
	static shared_ptr<CUI_Indicator_Monster_MissPlayer>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;
};

END