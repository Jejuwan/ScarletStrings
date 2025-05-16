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

class CUI_Indicator_ToolTip_Tutorial_Tips final : public CGameObject
{
private:
	explicit CUI_Indicator_ToolTip_Tutorial_Tips(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_ToolTip_Tutorial_Tips() DEFAULT;

public:
	virtual HRESULT											Initialize_Prototype() override;
	virtual HRESULT											Initialize(any _aDesc = g_aNull)	override;
	virtual void											Tick(_float fTimeDelta) override;
	virtual void											Late_Tick(_float fTimeDelta) override;
	virtual HRESULT											Render() override;

private:
	virtual HRESULT											Ready_Components() override;

private:
	void													Initialize_UI();
	void													Tick_UI(_float fTimeDelta);
	void													TestKey();
	
private:
	void													Render_Tips_System(_float fTimeDelta);

	void													Tip_Jump();
	void													Tip_Psycokinesis();
	void													Tip_BasicAttack();
	void													Tip_SomerSaultKick();
	void													Tip_SpecialAttack();

	void													Tip_SystemLog();
	void													Tip_SAS_Electric();
	void													Tip_SAS_Acceleration();
	void													Tip_SAS_Clone();
	void													Tip_SAS_Stealth();
	void													Tip_Gimmick_DumpTruck();

	void													Reset();

public:
	void													Start_Tip_Jump();
	void													Start_Tip_Psycokinesis();
	void													Start_Tip_BasicAttack();
	void													Start_Tip_SomerSaultKick();
	void													Start_Tip_Tip_SpecialAttack();

	void													Start_Tip_SystemLog();
	void													Start_Tip_SAS_Electric();
	void													Start_Tip_SAS_Clone();
	void													Start_Tip_SAS_Stealth();
	void													Start_Tip_Gimmick_DumpTruck();

private:
	shared_ptr<CTransform>									m_pBackground;
	shared_ptr<CTransform>									m_pTips_Tag;
	shared_ptr<CTransform>									m_pTips;

	shared_ptr<CTransform>									m_pKey_SpaceBar;
	shared_ptr<CTransform>									m_pKey_RButton;
	shared_ptr<CTransform>									m_pKey_LButton;
	shared_ptr<CTransform>									m_pKey_C;
	shared_ptr<CTransform>									m_pKey_F;
	shared_ptr<CTransform>									m_pKey_G;
	shared_ptr<CTransform>									m_pKey_Z;
	shared_ptr<CTransform>									m_pKey_1;
	shared_ptr<CTransform>									m_pKey_2;
	shared_ptr<CTransform>									m_pKey_3;
	shared_ptr<CTransform>									m_pKey_4;

private:
	shared_ptr<CTexture>									m_pTexture_Background;
	shared_ptr<CTexture>									m_pTexture_Tips_Tag;
	shared_ptr<CTexture>									m_pTexture_Tips;

	shared_ptr<CTexture>									m_pTexture_Key_SpaceBar;
	shared_ptr<CTexture>									m_pTexture_Key_RButton;
	shared_ptr<CTexture>									m_pTexture_Key_LButton;
	shared_ptr<CTexture>									m_pTexture_Key_C;
	shared_ptr<CTexture>									m_pTexture_Key_F;
	shared_ptr<CTexture>									m_pTexture_Key_G;
	shared_ptr<CTexture>									m_pTexture_Key_Z;
	shared_ptr<CTexture>									m_pTexture_Key_1;
	shared_ptr<CTexture>									m_pTexture_Key_2;
	shared_ptr<CTexture>									m_pTexture_Key_3;
	shared_ptr<CTexture>									m_pTexture_Key_4;

private:
	shared_ptr<CShader>										m_pShader;
	shared_ptr<CVIBuffer_Rect>								m_pVIBuffer_Rect;

private:
#pragma region Common _bool
	_bool													m_bTipRenderStart;
	_bool													m_bTipRenderEnd;
	_bool													m_bScaleUp;
	_bool													m_bScale_Background;
	_bool													m_bScale_Tips_Tag;
	_bool													m_bScale_Tips;

#pragma endregion
#pragma region Tips _bool
	_bool													m_bTip_PMCC;
	_bool													m_bTip_Jump;
	_bool													m_bTip_PsycoKinesis;
	_bool													m_bTip_BasicAttack;
	_bool													m_bTip_SomerSaultKick;
	_bool													m_bTip_SpecialAttack;
	_bool													m_bTip_SystemLog;

	_bool													m_bTip_SAS_Electric;
	_bool													m_bTip_SAS_Acceleration;
	_bool													m_bTip_SAS_Clone;
	_bool													m_bTip_SAS_Stealth;
	_bool													m_bTip_Gimmick_DumpTruck;
	_bool													m_bSoundStart;
	_bool													m_bSoundEnd;
#pragma endregion

private:
	_uint													m_iRenderPhase;

private:
	_float													m_fCurrentTipsIndex;

	_float													m_fTipsBackgroundTime;
	_float													m_fTipsShaderTime;
	_float													m_fSeperateLineTime;
	_float													m_fScaleX_ToolTip;

	_float													m_fScale_Additinal;
	_float													m_fRunningTime;

private:
	_float3													m_vScale_Background;
	_float3													m_vScale_Tips_Tag;
	_float3													m_vScale_Tips;

	_float3													m_vScale_Key_SpaceBar;
	_float3													m_vScale_Key_RButton;
	_float3													m_vScale_Key_LButton;
	_float3													m_vScale_Key_C;
	_float3													m_vScale_Key_F;
	_float3													m_vScale_Key_G;
	_float3													m_vScale_Key_Z;
	_float3													m_vScale_Key_1;
	_float3													m_vScale_Key_2;
	_float3													m_vScale_Key_3;
	_float3													m_vScale_Key_4;

private:
	_float4													m_vPosition_Background;
	_float4													m_vPosition_Tips_Tag;
	_float4													m_vPosition_Tips;

	_float4													m_vPosition_Key_SpaceBar;
	_float4													m_vPosition_Key_RButton;
	_float4													m_vPosition_Key_LButton;
	_float4													m_vPosition_Key_C;
	_float4													m_vPosition_Key_F;
	_float4													m_vPosition_Key_G;
	_float4													m_vPosition_Key_Z;
	_float4													m_vPosition_Key_1;
	_float4													m_vPosition_Key_2;
	_float4													m_vPosition_Key_3;
	_float4													m_vPosition_Key_4;

private:
	wstring													m_strExplanation1;
	wstring													m_strExplanation1_1;
	wstring													m_strExplanation2;
	wstring													m_strExplanation3;


public:
	static shared_ptr<CUI_Indicator_ToolTip_Tutorial_Tips>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>							Clone(any = g_aNull) override;
};

END