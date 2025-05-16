#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CObjectLayer;
class CInteractiveObject;
END

BEGIN(Client)

class CUI_Indicator_ObjectAttack final : public CGameObject
{
private:
	explicit CUI_Indicator_ObjectAttack(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_ObjectAttack() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any _aDesc = g_aNull)	override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;
	 
private:
	virtual HRESULT									Ready_Components() override;

public:
	void											Ready_PsycoAttack(shared_ptr<CInteractiveObject>, _float fTimeDelta);

public:
	_bool											Get_PsycoKinesisRender()								{ return m_bRenderSwitch; }

public:
	void											Set_Ratio(_float fRatio);
	void											Set_Indicator_DefaultState(_float fTimeDelta);
	void											Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix);

private:
	shared_ptr<CTransform>							m_pIndicator;
	shared_ptr<CTransform>							m_pGauge;

	shared_ptr<CTexture>							m_pTexture_Indicator;
	shared_ptr<CTexture>							m_pTexture_Gauge;

	shared_ptr<CShader>								m_pShader;
	shared_ptr<CVIBuffer_Rect>						m_pVIBuffer_Rect;

private:
	_bool											m_bRenderSwitch		= { false };
	_bool											m_bCanObjectAttack	= { false };
	_bool											m_bRenderDegree		= { false };

private:
	_float											m_fTime				= { 0.f };
	_float											m_fRatio;

private:
	_float3											m_vScale_Indicator;
	_float4											m_vPosition_Indicator;

private:
	_float3											m_vScale_Gauge;
	_float4											m_vPosition_Gauge;


public:
	static shared_ptr<CUI_Indicator_ObjectAttack>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END