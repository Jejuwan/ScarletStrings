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

class CUI_Indicator_ToolTip_Tutorial_Success final : public CGameObject
{
private:
	explicit CUI_Indicator_ToolTip_Tutorial_Success(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_ToolTip_Tutorial_Success() DEFAULT;

public:
	virtual HRESULT												Initialize_Prototype() override;
	virtual HRESULT												Initialize(any _aDesc = g_aNull)	override;
	virtual void												Tick(_float fTimeDelta) override;
	virtual void												Late_Tick(_float fTimeDelta) override;
	virtual HRESULT												Render() override;

private:
	virtual HRESULT												Ready_Components() override;

private:
	void														Initialize_UI();
	void														Tick_UI(_float fTimeDelta);
	void														Blink_System(_float fTimeDelta);
	void														Blink_Background(_float fTimeDelta);
	void														Reset_UI();
	void														Sound(_float fTimeDelta);


public:
	void														Set_Success_PMCC();
	void														Set_Success_LockOn();
	void														Set_Success_Kasane_BattleStyle();
	void														Set_Success_Kasane_SpecialAttack();

private:
	shared_ptr<CTransform>										m_pBackground;
	shared_ptr<CTransform>										m_pSuccess;

private:
	shared_ptr<CTexture>										m_pTexture_Background;
	shared_ptr<CTexture>										m_pTexture_Success;

private:
	shared_ptr<CShader>											m_pShader;
	shared_ptr<CVIBuffer_Rect>									m_pVIBuffer_Rect;

private:
	_uint														m_iSoundIndex;

#pragma region Situation
	_bool														m_bPMCC;
	_bool														m_bLockOn;
	_bool														m_bKasane_BattleStyle;
	_bool														m_bKasane_SpecialAttack;
#pragma endregion
#pragma region Control-Setting
	_bool														m_bScalingX;
	_bool														m_bScalingY;
	_bool														m_bScalingDone;
	_bool														m_bRewind;
	_bool														m_bRewindScalingXY;
#pragma endregion



private:
	_float														m_fAccTime;
	_float														m_fBlinkDoneTime;
	_float														m_fBackgroundAlpha;
	_float														m_fBackgroundColor;
	_float														m_fSuccessColor;

private:
	_float3														m_vScale_Background;
	_float3														m_vScale_Success;

private:
	_float4														m_vPosition_Background;
	_float4														m_vPosition_Success;

public:
	static shared_ptr<CUI_Indicator_ToolTip_Tutorial_Success>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>								Clone(any = g_aNull) override;

};

END