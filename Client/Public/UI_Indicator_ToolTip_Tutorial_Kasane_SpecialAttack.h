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

class CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack final : public CGameObject
{
private:
	explicit CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack() DEFAULT;

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
	void																	Key_Control();
	void																	LightOn_Selectors(_float fTimeDelta);
	void																	Blink_Selectors(_float fTimeDelta);
	void																	Sound(_float fTimeDelta);

public:
	void																	Movie_Start();
	void																	Movie_End(_float fTimeDelta);
	void																	Movie_Really_Close(_float fTimeDelta);

private:
	// Basic
	shared_ptr<CTransform>													m_pBackground;
	shared_ptr<CTransform>													m_pMovie;
	shared_ptr<CTransform>													m_pSeperate_Line1;
	shared_ptr<CTransform>													m_pSeperate_Line2;
	// Add
	shared_ptr<CTransform>													m_pKey_C;
	shared_ptr<CTransform>													m_pKey_ENTER;
	// Close Window
	shared_ptr<CTransform>													m_pBackground_Close;
	shared_ptr<CTransform>													m_pSeperate_Line3;
	shared_ptr<CTransform>													m_pAnswer;
	shared_ptr<CTransform>													m_pSelector_0;
	shared_ptr<CTransform>													m_pSelector_1;
	shared_ptr<CTransform>													m_pSelector_2;
private:
	// Basic
	shared_ptr<CTexture>													m_pTexture_Background;
	shared_ptr<CTexture>													m_pTexture_Movie;
	shared_ptr<CTexture>													m_pTexture_SeperateLine1;
	shared_ptr<CTexture>													m_pTexture_SeperateLine2;
	// Add
	shared_ptr<CTexture>													m_pTexture_Key_C;
	shared_ptr<CTexture>													m_pTexture_Key_ENTER;
	// Close Window
	shared_ptr<CTexture>													m_pTexture_Background_Close;
	shared_ptr<CTexture>													m_pTexture_SeperateLine3;
	shared_ptr<CTexture>													m_pTexture_Answer;
	shared_ptr<CTexture>													m_pTexture_Selector_0;
	shared_ptr<CTexture>													m_pTexture_Selector_1;
	shared_ptr<CTexture>													m_pTexture_Selector_2;

private:
	// Others
	shared_ptr<CShader>														m_pShader;
	shared_ptr<CVIBuffer_Rect>												m_pVIBuffer_Rect;

private:
	_bool																	m_bMovieStart;
	_bool																	m_bScaleUp;
	_bool																	m_bToolTip_End;
	_bool																	m_bIsRunning;

	_bool																	m_bCall_BackgroundClose;
	_bool																	m_bScaleUp_Close;
	_bool																	m_bCall_BackgroundEnd;
	_bool																	m_bIsReallyClose;
	_bool																	m_bBlinkOn;
	_bool																	m_bLightOn_Selector_1;
	_bool																	m_bLightOn_Selector_2;

private:
	_int																	m_iBlinkIndex;
	_uint																	m_iCloseIndex;
	_uint																	m_iSoundIndex;
	_uint																	m_iSoundCount;

private:
	_float																	m_fCurrentMovieIndex;

	_float																	m_fMovieBackgroundTime;
	_float																	m_fMovieShaderTime;
	_float																	m_fSeperateLineTime;
	_float																	m_fScaleX_ToolTip;

	_float																	m_fCloseBackgroundTime;
	_float																	m_fCloseSeperateLineTime;
	_float																	m_fScaleX_CloseToolTip;
	_float																	m_fCloseAnswerTime;
	_float																	m_fAllCloseDelayTime;
	_float																	m_fTimeSelector_1;
	_float																	m_fTimeSelector_2;
	_float																	m_fBlinkTime;

private:	// For. Scale
	_float3																	m_vScale_Background;
	_float3																	m_vScale_Movie;
	_float3																	m_vScale_SeperateLine1;
	_float3																	m_vScale_SeperateLine2;

	_float3																	m_vScale_Key_C;
	_float3																	m_vScale_Key_ENTER;

	_float3																	m_vScale_Background_Close;
	_float3																	m_vScale_SeperateLine3;
	_float3																	m_vScale_Answer;
	_float3																	m_vScale_Selector_0;
	_float3																	m_vScale_Selector_1;
	_float3																	m_vScale_Selector_2;

private: // For. Position
	_float4																	m_vPosition_Background;
	_float4																	m_vPosition_Movie;
	_float4																	m_vPosition_SeperateLine1;
	_float4																	m_vPosition_SeperateLine2;

	_float4																	m_vPosition_Key_C;
	_float4																	m_vPosition_Key_ENTER;

	_float4																	m_vPosition_Background_Close;
	_float4																	m_vPosition_SeperateLine3;
	_float4																	m_vPosition_Answer;
	_float4																	m_vPosition_Selector_0;
	_float4																	m_vPosition_Selector_1;
	_float4																	m_vPosition_Selector_2;

private: // For. RenderFont
	wstring																	m_strTitle;
	wstring																	m_strExplanation1;
	wstring																	m_strExplanation2;
	wstring																	m_strExplanation2_1;
	wstring																	m_strExplanation2_2;
	wstring																	m_strExplanation2_3;
	wstring																	m_strExplanation3;
	wstring																	m_strEnter;

	wstring																	m_strClose_Answer;
	wstring																	m_strClose_Yes;
	wstring																	m_strClose_No;

public:
	static shared_ptr<CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>											Clone(any = g_aNull) override;
};

END