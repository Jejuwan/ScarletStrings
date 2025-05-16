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

class CUI_Indicator_SpcObjectAttack final : public CGameObject
{
private:
	explicit CUI_Indicator_SpcObjectAttack(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_SpcObjectAttack() DEFAULT;

public:
	virtual HRESULT										Initialize_Prototype() override;
	virtual HRESULT										Initialize(any _aDesc = g_aNull)	override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;
	 
private:
	virtual HRESULT										Ready_Components() override;

public:
	void												Ready_PsycoAttack(shared_ptr<CInteractiveObject>, _float fTimeDelta);
	void												Set_IndicatorPosition(_vectorf vWorldPosition, _matrixf _WorldMatrix);
	void												Set_RenderOff();

private:
	shared_ptr<CTransform>								m_pIndicator_SpecialObjectAttack;
	shared_ptr<CTexture>								m_pTexture;
	shared_ptr<CShader>									m_pShader;
	shared_ptr<CVIBuffer_Rect>							m_pVIBuffer_Rect;

private:
	_bool												m_bRenderSwitch;
	_bool												m_bRenderDegree;
private:
	_float4												m_vPosition;

public:
	static shared_ptr<CUI_Indicator_SpcObjectAttack>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;
};

END