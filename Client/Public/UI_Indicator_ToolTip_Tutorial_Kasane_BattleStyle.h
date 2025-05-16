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

class CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle final : public CGameObject
{
private:
	explicit CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle() DEFAULT;

public:
	virtual HRESULT															Initialize_Prototype() override;
	virtual HRESULT															Initialize(any _aDesc = g_aNull)	override;
	virtual void															Tick(_float fTimeDelta) override;
	virtual void															Late_Tick(_float fTimeDelta) override;
	virtual HRESULT															Render() override;

private:
	virtual HRESULT															Ready_Components() override;

private:
	void																	Initialize_UI();
	void																	Tick_UI(_float fTimeDelta);
	void																	Movie_System(_float fTimeDelta);
	void																	PageSwitch();
	void																	SwitchDatas(_float fTimeDelta);
	void																	Sound(_float fTimeDelta);

public:
	void																	Movie_Start();
	void																	Movie_End(_float fTimeDelta);

private:
	shared_ptr<CTransform>													m_pBackground;
	shared_ptr<CTransform>													m_pMovie;
	shared_ptr<CTransform>													m_pSeperate_Line1;
	shared_ptr<CTransform>													m_pSeperate_Line2;
	shared_ptr<CTransform>													m_pPage_1;
	shared_ptr<CTransform>													m_pPage_Slash;
	shared_ptr<CTransform>													m_pPage_2;
private:
	shared_ptr<CTransform>													m_pKey_VK_LButton;
	shared_ptr<CTransform>													m_pKey_Switch_L1;
	shared_ptr<CTransform>													m_pKey_Switch_L2;
	shared_ptr<CTransform>													m_pKey_Switch_R1;
	shared_ptr<CTransform>													m_pKey_Switch_R2;
	shared_ptr<CTransform>													m_pKey_Switch_Icon;
	shared_ptr<CTransform>													m_pKey_ENTER;

private:
	shared_ptr<CTexture>													m_pTexture_Background;
	shared_ptr<CTexture>													m_pTexture_Movie;
	shared_ptr<CTexture>													m_pTexture_SeperateLine1;
	shared_ptr<CTexture>													m_pTexture_SeperateLine2;
	shared_ptr<CTexture>													m_pTexture_Page_1;
	shared_ptr<CTexture>													m_pTexture_Page_Slash;
	shared_ptr<CTexture>													m_pTexture_Page_2;
private:
	shared_ptr<CTexture>													m_pTexture_Key_VK_LButton;
	shared_ptr<CTexture>													m_pTexture_Key_Switch_L1;
	shared_ptr<CTexture>													m_pTexture_Key_Switch_L2;
	shared_ptr<CTexture>													m_pTexture_Key_Switch_R1;
	shared_ptr<CTexture>													m_pTexture_Key_Switch_R2;
	shared_ptr<CTexture>													m_pTexture_Key_Switch_Icon;
	shared_ptr<CTexture>													m_pTexture_Key_ENTER;

private:
	shared_ptr<CShader>														m_pShader;
	shared_ptr<CVIBuffer_Rect>												m_pVIBuffer_Rect;

private:
	_bool																	m_bMovieStart;
	_bool																	m_bScaleUp;
	_bool																	m_bToolTip_End;
	_bool																	m_bIsRunning;

	_bool																	m_bSwitchStart_L;
	_bool																	m_bSwitchStart_R;
	_bool																	m_bLightOn_L;
	_bool																	m_bLightOn_R;

private:
	_int																	m_iTimePlusMinus;
	_uint																	m_iTimeIndexCount_L;
	_uint																	m_iTimeIndexCount_R;
	_uint																	m_iPageIndex;
	_uint																	m_iSoundIndex;
	_uint																	m_iSoundCount;

private:
	_float																	m_fCurrentMovieIndex;

	_float																	m_fMovieBackgroundTime;
	_float																	m_fMovieShaderTime;
	_float																	m_fSeperateLineTime;
	_float																	m_fScaleX_ToolTip;

	_float																	m_fAccTime;
	_float																	m_fShaderTime;

private:
	_float3																	m_vScale_Background;
	_float3																	m_vScale_Movie;
	_float3																	m_vScale_SeperateLine1;
	_float3																	m_vScale_SeperateLine2;
	_float3																	m_vScale_Page_1;
	_float3																	m_vScale_Page_Slash;
	_float3																	m_vScale_Page_2;
private:
	_float3																	m_vScale_Key_VK_LButton;
	_float3																	m_vScale_Key_Switch_L1;
	_float3																	m_vScale_Key_Switch_L2;
	_float3																	m_vScale_Key_Switch_R1;
	_float3																	m_vScale_Key_Switch_R2;
	_float3																	m_vScale_Key_Switch_Icon;
	_float3																	m_vScale_Key_ENTER;

private:
	_float4																	m_vPosition_Background;
	_float4																	m_vPosition_Movie;
	_float4																	m_vPosition_SeperateLine1;
	_float4																	m_vPosition_SeperateLine2;
	_float4																	m_vPosition_Page_1;
	_float4																	m_vPosition_Page_Slash;
	_float4																	m_vPosition_Page_2;
private:
	_float4																	m_vPosition_Key_VK_LButton;
	_float4																	m_vPosition_Key_Switch_L1;
	_float4																	m_vPosition_Key_Switch_L2;
	_float4																	m_vPosition_Key_Switch_R1;
	_float4																	m_vPosition_Key_Switch_R2;
	_float4																	m_vPosition_Key_Switch_Icon;
	_float4																	m_vPosition_Key_ENTER;

private:
	wstring																	m_strTitle;
	wstring																	m_strExplanation1;
	wstring																	m_strExplanation1_1;
	wstring																	m_strExplanation1_2;
	wstring																	m_strExplanation2;
	wstring																	m_strExplanation2_1;
	wstring																	m_strExplanation2_2;
	wstring																	m_strExplanation3;
	wstring																	m_strExplanation4;

	wstring																	m_strExplanation5;
	wstring																	m_strExplanation5_1;
	wstring																	m_strExplanation6;

	wstring																	m_strPageSwitch;
	wstring																	m_strEnter;

public:
	static shared_ptr<CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>											Clone(any = g_aNull) override;
};

END