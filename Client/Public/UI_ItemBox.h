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

class CUI_ItemBox final : public CGameObject
{
private:
	explicit CUI_ItemBox(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_ItemBox() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	virtual HRESULT							Ready_Components() override;

public:
	void									Initialize_UI();
	void									ItemSystem(_float fTimeDelta);
	void									Use_Potion();
	void									Get_Potion();
	void									UIBounce_System(_float fTimeDelta);

private:
	shared_ptr<CTransform>					m_pItemBox;
	shared_ptr<CTransform>					m_pItemPotion;
	shared_ptr<CTransform>					m_pItemShader;
	shared_ptr<CTransform>					m_pItemCoolTimeGauge;

	shared_ptr<CTexture>					m_pTexture;
	shared_ptr<CTexture>					m_pTexture_ItemPotion;
	shared_ptr<CTexture>					m_pTexture_ItemShader;
	shared_ptr<CTexture>					m_pTexture_ItemCoolTimeGauge;

	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	_float4									m_vPosition_ItemBox;
	_float4									m_vPosition_ItemPotion;
	_float4									m_vPosition_ItemShader;

	_float3									m_vItemCoolTimeGauge_Scale;
	_float4									m_vItemCoolTimeGauge_Position;
	_float4									m_vPrevItemCoolTimeGauge_Position;

private:
	_bool									m_bUsePotion;
	_bool									m_bRenderSwitch;

private:
	_int									m_iPlayerHpMax;

private:
	_float									m_fRatio;
	_float									m_fGauge, m_fGaugeMax;

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


public:
	static shared_ptr<CUI_ItemBox>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END