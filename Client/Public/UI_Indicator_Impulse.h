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

class CUI_Indicator_Impulse final : public CGameObject
{
private:
	explicit CUI_Indicator_Impulse(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Impulse() DEFAULT;

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
	void										Set_IndicatorPosition(_float4 _Position);
private:
	shared_ptr<CTransform>						m_pItemBox;
	shared_ptr<CTexture>						m_pTexture;
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;

private:
	_float										m_fGauge;

private:
	_float3										m_vScale;
	_float4										m_vPosition;

private:
	SPRITEUIDESC								m_tUIDesc;


public:
	static shared_ptr<CUI_Indicator_Impulse>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END