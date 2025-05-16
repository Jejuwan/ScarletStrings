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

class CUI_Shop_Info final : public CGameObject
{
public:
	struct CATALOGDESC {
		_int		iPossession = { 0 };
		_int		iAmount = { 0 };
	};
	
private:
	explicit CUI_Shop_Info(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Shop_Info() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	HRESULT	Bind_Shader(shared_ptr<CTexture> ptrTexture, shared_ptr<CTransform> ptrTransform, _int iPassIndex, _float fValue = 1.f);
	void	Make_Font_Catalog();
	void	Make_Font_Info();

	CATALOGDESC	Find_Catalog_Value(wstring strName, _int iChangePossession = 0);
	wstring	Int_Change_to_wstring(_int iValue);
	wstring	Catalog_Name(_int iSubMenuIndex, _int iRoop = 0);
	wstring	Catalog_Name_For_K(_int iSubMenuIndex, _int iCatalogIndex);

private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pTranform_Catalog[9];
	shared_ptr<CTransform>					m_pTranform_Texture[9];
	shared_ptr<CTransform>					m_pTranform_Icon;
	shared_ptr<CTransform>					m_pTranform_Info;
	shared_ptr<CTransform>					m_pTranform_K_Info;


	shared_ptr<CTexture>					m_pTexture_Icon[7];
	shared_ptr<CTexture>					m_pTexture_Info;
	shared_ptr<CTexture>					m_pTexture_K_Info;
	shared_ptr<CTexture>					m_pTexture_Frame_Selected[2];
	shared_ptr<CTexture>					m_pTexture_Jelly[4];
	shared_ptr<CTexture>					m_pTexture_Medium_Jelly[4];
	shared_ptr<CTexture>					m_pTexture_Max_Jelly[4];
	shared_ptr<CTexture>					m_pTexture_Gift[2];

	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	unordered_map<wstring, CATALOGDESC> m_umapCatalog;

	CATALOGDESC			m_tCatalog;

	_float2				m_NameFontPos[9] = {};
	_float2				m_PossessionFontPos[9] = {};
	_float2				m_AmountFontPos[9] = {};
	_float2				m_NumberFontPos[9] = {};

	wstring				m_strGift[3] = {};
	wstring				m_strItem[6] = {};

	wstring				m_strPossession[9] = {};
	wstring				m_strAmount[9] = {};
	wstring				m_strNumber[9] = {};

	wstring		m_strName[9] = {};
	wstring		m_strInfo[9] = {};
	wstring		m_str_K_Info[3] = {};
	wstring		m_str_K[3] = {};

public:
	static shared_ptr<CUI_Shop_Info>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END