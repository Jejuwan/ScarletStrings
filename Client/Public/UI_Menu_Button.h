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

class CUI_Menu_Button final : public CGameObject
{
private:
	explicit CUI_Menu_Button(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Button() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pButton_1;
	shared_ptr<CTransform>					m_pButton_3;
	shared_ptr<CTransform>					m_pButton_Q;
	shared_ptr<CTransform>					m_pButton_E;

	shared_ptr<CTexture>					m_pTexture_1;
	shared_ptr<CTexture>					m_pTexture_3;
	shared_ptr<CTexture>					m_pTexture_Q;
	shared_ptr<CTexture>					m_pTexture_E;

	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	//const PLAYERENTITYDESC*					m_pEntityDesc	= nullptr;


public:
	static shared_ptr<CUI_Menu_Button>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END