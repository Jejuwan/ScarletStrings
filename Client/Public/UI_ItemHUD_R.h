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

class CUI_ItemHUD_R final : public CGameObject
{
private:
	explicit CUI_ItemHUD_R(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_ItemHUD_R() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	virtual HRESULT							Ready_Components() override;

private:
	void									Initialize_UI();

public:
	void									UIBounce_System(_float fTimeDelta);

private:
	shared_ptr<CTransform>					m_pItemHUD_R;
	shared_ptr<CTexture>					m_pTexture;
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
#pragma region Bounce
	POINT									m_ptCursorMove;
	_bool									m_bUIBounce;
	_bool									m_bLeft;
	_bool									m_bRight;
	_bool									m_bTop;
	_bool									m_bBottom;
	_float									m_fMovedPositionX_Left;
	_float									m_fMovedPositionX_Right;
	_float									m_fMovedPositionY_Top;
	_float									m_fMovedPositionY_Bottom;
	_float									m_fOriginalPosX;
	_float									m_fOriginalPosY;
#pragma endregion

private:
	_float3									m_vScale_ItemHUD_R;
	_float4									m_vPosition_ItemHUD_R;
	_float4									m_vPrevPosition_ItemHUD_R;

public:
	static shared_ptr<CUI_ItemHUD_R>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END