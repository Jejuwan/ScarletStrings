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

class CUI_Menu_Fade_Fillter final : public CGameObject
{
private:
	explicit CUI_Menu_Fade_Fillter(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Fade_Fillter() DEFAULT;


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
	void										Tick_UI(_float fTimeDelta);
	void										Fade_InOut(_float fTimeDelta);

private:
	shared_ptr<CTransform>						m_pFade;
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CTexture>						m_pTexture_Fade;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;

private:
	_bool										m_bRender;

private:
	_uint										m_iSoundIndex;

private:
	_float										m_fFadeAlpha;
	_float										m_fSoundDelay;

private:
	_float3										m_vScale_Fade;

private:
	_float4										m_vPosition_Fade;

public:
	static shared_ptr<CUI_Menu_Fade_Fillter>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END 