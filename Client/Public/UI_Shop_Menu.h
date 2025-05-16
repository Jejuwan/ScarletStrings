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

class CUI_Shop_Menu final : public CGameObject
{
private:
	explicit CUI_Shop_Menu(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Shop_Menu() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	HRESULT	Bind_Shader(shared_ptr<CTexture> ptrTexture, shared_ptr<CTransform> ptrTransform,
		_int iPassIndex, _float fValue = 1.f, _bool	IsCheckBox = false);

	void	Make_Font();

	void	Input_Transform();

private:
	virtual HRESULT							Ready_Components() override;


private:
	shared_ptr<CTransform>					m_pTranform_Purchase;
	shared_ptr<CTransform>					m_pTranform_Sale;
	shared_ptr<CTransform>					m_pTransform_Everything;
	shared_ptr<CTransform>					m_pTransform_Item;
	shared_ptr<CTransform>					m_pTransform_Present;
	shared_ptr<CTransform>					m_pTransform_ThinLine;
	shared_ptr<CTransform>					m_pTransform_ThickLine;
	shared_ptr<CTransform>					m_pTransform_Diagonal;
	shared_ptr<CTransform>					m_pTransform_Cart;
	shared_ptr<CTransform>					m_pTransform_Arrow[6];
	shared_ptr<CTransform>					m_pTransform_Highlight[2];
	shared_ptr<CTransform>					m_pTransform_CheckBox_YellowLine;
	shared_ptr<CTransform>					m_pTransform_CheckBox;
	shared_ptr<CTransform>					m_pTransform_CheckBox_Line;
	shared_ptr<CTransform>					m_pTransform_Key_Icon[4];


	shared_ptr<CTexture>					m_pTexture_On;
	shared_ptr<CTexture>					m_pTexture_Off;
	shared_ptr<CTexture>					m_pTexture_SubMenu;
	shared_ptr<CTexture>					m_pTexture_Line;
	shared_ptr<CTexture>					m_pTexture_Diagonal;
	shared_ptr<CTexture>					m_pTexture_Cart;
	shared_ptr<CTexture>					m_pTexture_Arrow[2];
	shared_ptr<CTexture>					m_pTexture_Highlight;
	shared_ptr<CTexture>					m_pTexture_CheckBox;
	shared_ptr<CTexture>					m_pTexture_CheckBox_Line;
	shared_ptr<CTexture>					m_pTexture_Key_Icon[4];

	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

public:
	static shared_ptr<CUI_Shop_Menu>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END