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

class CUI_Menu_Background final : public CGameObject
{
private:
	explicit CUI_Menu_Background(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Background() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pBackground;
	shared_ptr<CTransform>					m_pBackground_Frame;
	shared_ptr<CTransform>					m_pSeperate_Line;
	shared_ptr<CTransform>					m_pInfo_L;
	shared_ptr<CTransform>					m_pInfo_M;
	shared_ptr<CTransform>					m_pInfo_S1;
	shared_ptr<CTransform>					m_pInfo_S2;
	shared_ptr<CTransform>					m_pBP;
	shared_ptr<CTransform>					m_pK;

	shared_ptr<CTexture>					m_pTexture_Background;
	shared_ptr<CTexture>					m_pTexture_Frame;
	shared_ptr<CTexture>					m_pTexture_Sperate_Line;
	shared_ptr<CTexture>					m_pTexture_Info_L;
	shared_ptr<CTexture>					m_pTexture_Info_M;
	shared_ptr<CTexture>					m_pTexture_Info_S1;
	shared_ptr<CTexture>					m_pTexture_Info_S2;
	shared_ptr<CTexture>					m_pTexture_BP;
	shared_ptr<CTexture>					m_pTexture_K_Info;

	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

public:
	void									KeyControl();
	void									Calculate_Time(_float fTimeDelta);
	void									Get_BP();
	void									Use_BP();
	void									Get_K();
	void									Use_K();

public:
	void									TestKey();

private:
	_uint									m_iTimeAccHour	= { 0 };
	_uint									m_iTimeAccMin	= { 0 };

private:
	_float									m_fTimeAccSec	= { 0.f };

private:
	wstring									m_strK			= TEXT("");
	wstring									m_strBP			= TEXT("");
	wstring									m_strInfo		= TEXT("");
	wstring									m_strZero		= TEXT("");
	wstring									m_strTimeSec	= TEXT("");
	wstring									m_strTimeMin	= TEXT("");
	wstring									m_strTimeHour	= TEXT("");
	wstring									m_strSepertate	= TEXT("");

public:
	static shared_ptr<CUI_Menu_Background>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END