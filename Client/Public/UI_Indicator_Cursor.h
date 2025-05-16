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

class CUI_Indicator_Cursor final : public CGameObject
{
private:
	explicit CUI_Indicator_Cursor(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Cursor() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any _aDesc = g_aNull)	override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

private:
	virtual HRESULT								Ready_Components() override;

private:
	void										Initialize_UI();
	void										Set_CursorPos();

private:
	shared_ptr<CTransform>						m_pCursor_Off;
	shared_ptr<CTransform>						m_pCursor_On;

private:
	shared_ptr<CTexture>						m_pTexture_Cursor_Off;
	shared_ptr<CTexture>						m_pTexture_Cursor_On;

private:
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;

private:
	_bool										m_bIsClick;

private:
	_int										m_iShow_Cursor;

private:
	_float3										m_vScale_Cursor_Off;
	_float3										m_vScale_Cursor_On;

private:
	_float4										m_vPosition_Cursor_Off;
	_float4										m_vPosition_Cursor_On;

private:
	POINT										m_ptCursorPos;




public:
	static shared_ptr<CUI_Indicator_Cursor>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;

};

END