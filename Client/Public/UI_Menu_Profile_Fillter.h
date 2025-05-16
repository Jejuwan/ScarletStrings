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

class CUI_Menu_Profile_Fillter final : public CGameObject
{
private:
	explicit CUI_Menu_Profile_Fillter(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Profile_Fillter() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any _aDesc = g_aNull)	override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;
	 
private:
	virtual HRESULT								Ready_Components() override;

public:
	void										KeyWork();

private:
	shared_ptr<CTransform>						m_pFillter;
	shared_ptr<CTransform>						m_pFillter_HighLight;

	shared_ptr<CTexture>						m_pTexture;
	shared_ptr<CTexture>						m_pTexture_HighLight;

	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;

private:
	_float3										m_vScale;
	_float3										m_vHighLightScale;
	
private:
	_float4										m_vPosition;
	_float4										m_vHighLightPosition;

public:
	static shared_ptr<CUI_Menu_Profile_Fillter>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END