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

class CUI_Indicator_Monster_NoticeMark final : public CGameObject
{
private:
	explicit CUI_Indicator_Monster_NoticeMark(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Monster_NoticeMark() DEFAULT;

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
	void												Set_IndicatorScaleDown();
	void												Set_ScalingSystem(_float fTimeDelta);

public:												
	void												NoticePlayer()				{ m_bRenderSwitch = true; }
	void												Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix);
	void												Set_Initialize();

private:
	shared_ptr<CShader>									m_pShader;
	shared_ptr<CTexture>								m_pTexture_Indicator;
	shared_ptr<CTexture>								m_pTexture_Indicator_L;
	shared_ptr<CTexture>								m_pTexture_Indicator_R;
	shared_ptr<CTransform>								m_pIndicator;
	shared_ptr<CTransform>								m_pIndicator_L;
	shared_ptr<CTransform>								m_pIndicator_R;
	shared_ptr<CVIBuffer_Rect>							m_pVIBuffer_Rect;

private:
	_bool												m_bRenderSwitch;
	_bool												m_bScaleDownStart;
	_bool												m_bScaleDisappear;
	_bool												m_bRenderDegree;

private:
	_float												m_fTime;
	_float												m_fAccTime;
	_float												m_fShaderTime;
	_float												m_fShaderIndex;
	_float												m_fAcceleration;

private:
	_float3												m_vScale_Indicator;
	_float3												m_vScale_Indicator_L;
	_float3												m_vScale_Indicator_R;

private:
	_float4												m_vPosition_Indicator;
	_float4												m_vPosition_Indicator_L;
	_float4												m_vPosition_Indicator_R;

public:
	static shared_ptr<CUI_Indicator_Monster_NoticeMark>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;
};

END