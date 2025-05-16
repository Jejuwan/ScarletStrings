#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Indicator_Boss_Hp final : public CGameObject
{
private:
	explicit CUI_Indicator_Boss_Hp(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Boss_Hp() DEFAULT;

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
	void										Tick_UI(_float fTimeDelta);

	void										Render_Hp(_float fTimeDelta);
	void										Render_Shader_Background(_float fTimeDelta);
	void										Render_Shader_Back(_float fTimeDelta);
	void										Render_Shader_Front(_float fTimeDelta);
	void										Set_Position_DamagedBossHP(_float fTimeDelta);
	void										Calculate_Damage_System(_float fTimeDelta);

public:
	void										Set_BossName(wstring strBossName);
	void										Render_Start();
	void										Set_BossHp_Initialize(_float fHp, _float fHpMax);
	void										Set_BossHp(_float fHp, _float fHpMax);
	void										Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix);

private:
#pragma region Transform
	shared_ptr<CTransform>						m_pHP_Background;
	shared_ptr<CTransform>						m_pHP_Back;
	shared_ptr<CTransform>						m_pHP_Front;
#pragma endregion

#pragma region Texture
	shared_ptr<CTexture>						m_pTexture_HP_Background;
	shared_ptr<CTexture>						m_pTexture_HP_Back;
	shared_ptr<CTexture>						m_pTexture_HP_Front;
#pragma endregion

#pragma region Others
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;
#pragma endregion

#pragma region _bool
	_bool										m_bRenderStart;
	_bool										m_bRender_BackgroundEnd;
	_bool										m_bRender_BackEnd;
	_bool										m_bRender_FrontEnd;
	_bool										m_bIsFinishCalculateDamage;
	_bool										m_bHpBackDecreaseStart;
#pragma endregion

#pragma region _float(Time)
	_float										m_fAccTime_Background;
	_float										m_fAccTime_Back;
	_float										m_fAccTime_Front;
	_float										m_fShaderTime_Background;
	_float										m_fShaderTime_Back;
	_float										m_fShaderTime_Front;
	_float										m_fDamageDelayTime;
	_float										m_fTime_RenderFontAlpha_OutLine;

#pragma endregion

#pragma region _float(Hp)
	_float										m_fHp;
	_float										m_fHpMax;
	_float										m_fRatio;
	_float										m_fPercentage;

	_float										m_fHpBack;
	_float										m_fHpBackMax;
	_float										m_fRatio_Back;
	_float										m_fPercentage_Back;

	_float										m_fBackHp;
	_float										m_fBackHpMax;
	_float										m_fRatioBackHp;
	_float										m_fPercentage_BackHp;
#pragma endregion

#pragma region _float3
	_float3										m_vScale_Hp_Background;
	_float3										m_vScale_Hp_Back;
	_float3										m_vScale_Hp_Front;
#pragma endregion

#pragma region _float4
	_float4										m_vPosition_Hp_Background;
	_float4										m_vPosition_Hp_Back;
	_float4										m_vPosition_Hp_Front;

#pragma endregion

#pragma region wstring
	wstring										m_strBossName;
#pragma endregion


public:
	static shared_ptr<CUI_Indicator_Boss_Hp>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END