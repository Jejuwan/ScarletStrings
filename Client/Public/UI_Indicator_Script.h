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

class CUI_Indicator_Script final : public CGameObject
{
private:
	explicit CUI_Indicator_Script(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Script() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any _aDesc = g_aNull) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

private:
	virtual HRESULT								Ready_Components() override;

public:
	void										Set_ScriptRenderStart();

private:
#pragma region CTransform
	shared_ptr<CTransform>						m_pBackground;
	shared_ptr<CTransform>						m_pNameTag;

#pragma endregion
#pragma region CTexture
	shared_ptr<CTexture>						m_pTexture_Background;
	shared_ptr<CTexture>						m_pTexture_NameTag;
#pragma endregion
#pragma region Others
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;
#pragma endregion


private:
	void										Initialize_UI();
	void										Tick_UI(_float fTimeDelta);

private:
#pragma region _bool
#pragma endregion
#pragma region _int
#pragma endregion
#pragma region _float
	_float										m_fRenderTime;
#pragma endregion
#pragma region _float3
	_float3										m_vScale_Background;
	_float3										m_vScale_NameTag;
#pragma endregion
#pragma region _float4
	_float4										m_vPosition_Background;
	_float4										m_vPosition_NameTag;
#pragma endregion
#pragma region wstring
	wstring										m_strNameTag;
	wstring										m_strText;
#pragma endregion


public:
	static shared_ptr<CUI_Indicator_Script>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END