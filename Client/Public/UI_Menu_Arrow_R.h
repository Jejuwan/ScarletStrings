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

class CUI_Menu_Arrow_R final : public CGameObject
{
private:
	explicit CUI_Menu_Arrow_R(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Arrow_R() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pArrow_Up;
	shared_ptr<CTransform>					m_pArrow_Down;

	shared_ptr<CTexture>					m_pTexture_Up;
	shared_ptr<CTexture>					m_pTexture_Down;

	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	//const PLAYERENTITYDESC*					m_pEntityDesc	= nullptr;


public:
	static shared_ptr<CUI_Menu_Arrow_R>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END