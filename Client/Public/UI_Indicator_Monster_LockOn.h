#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CObjectLayer;
class CPlayer;
END

BEGIN(Client)

class CUI_Indicator_Monster_LockOn final : public CGameObject
{
private:
	explicit CUI_Indicator_Monster_LockOn(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Monster_LockOn() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any _aDesc = g_aNull)	override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

private:
	virtual HRESULT									Ready_Components() override;

private:
	void											Initialize_UI();
	void											Tick_UI(_float fTimeDelta);
	void											LockOn(_float fTimeDelta);
	void											Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix);
	void											Set_IndicatorPosition_MIZUHA(_vectorf _vWorldPosition, _matrixf _WorldMatrix);
	void											Set_Positioning(_float fTimeDelta);
	void											Render_Background(_float fTimeDelta);
	void											Blink(_float fTimeDelta);
	void											Sound(_float fTimeDelta);

private:
	shared_ptr<CShader>								m_pShader;
	shared_ptr<CTransform>							m_pIndicator_T;
	shared_ptr<CTransform>							m_pIndicator_R;
	shared_ptr<CTransform>							m_pIndicator_B;
	shared_ptr<CTransform>							m_pIndicator_L;
	shared_ptr<CTransform>							m_pIndicator_Background;
	shared_ptr<CVIBuffer_Rect>						m_pVIBuffer_Rect;
	shared_ptr<CTexture>							m_pTexture_Indicator_T;
	shared_ptr<CTexture>							m_pTexture_Indicator_R;
	shared_ptr<CTexture>							m_pTexture_Indicator_B;
	shared_ptr<CTexture>							m_pTexture_Indicator_L;
	shared_ptr<CTexture>							m_pTexture_Indicator_Background;

	shared_ptr<CTransform>							m_pLockOn;
	shared_ptr<class CCamera_Player>				m_pCamera_Player;
	shared_ptr<class CMonster>						m_pLockOnMonster;
	
private:
	_float3											m_vScale_Indicator_T;
	_float3											m_vScale_Indicator_R;
	_float3											m_vScale_Indicator_B;
	_float3											m_vScale_Indicator_L;
	_float3											m_vScale_Indicator_Background;

private:
	_float4											m_vPosition_Indicator_T;
	_float4											m_vPosition_Indicator_R;
	_float4											m_vPosition_Indicator_B;
	_float4											m_vPosition_Indicator_L;
	_float4											m_vPosition_Indicator_Background;

private:
	_bool											m_bPositionMove;
	_bool											m_bPositionMoveEnd;
	_bool											m_bRenderBackground;
	_bool											m_bRenderBackgroundEnd;
	_bool											m_bBlink;

private:
	_float											m_fTime;
	_float											m_fTime_Mizuha;
	_float											m_fBackgroundAlpha;
	_float											m_fBlinkTime;

private:
	_int											m_iSwitch;
	_uint											m_iBlinkCount;
	_uint											m_iSoundIndex;

private:
	_vector											m_vPosition;

public:
	static shared_ptr<CUI_Indicator_Monster_LockOn>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;

};

END