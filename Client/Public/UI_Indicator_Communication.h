#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CSound_Manager;
END

BEGIN(Client)

class CUI_Indicator_Communication final : public CGameObject
{
private:
	explicit CUI_Indicator_Communication(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Communication() DEFAULT;

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

private:
	void													TestKey();

public:
	void	Set_Render_Icon_OnOff(_bool bOnOff)				{ m_bRenderIconSwitch = bOnOff; }
	void	Set_Render_Menu_OnOff(_bool bOnOff)				{ m_bRenderMenuSwitch = bOnOff; }
	void	Set_Switch_Icon_Near(_bool bNearOrNot)			{ m_bNearIcon = bNearOrNot; }

public:
	void													Communication_System(_float fTimeDelta);
	void													KeyInput();
	void													Open_Menu(_float fTimeDelta);
	void													Quit_Menu();
	void													Set_IndicatorPosition(_matrixf _WorldMatrix);
	void													Open_ReAnswer(_float fTimeDelta);
	void													Close_ReAnswer(_float fTimeDelta);
	void													Arash_JoinParty(_float fTimeDelta);

private:
#pragma region CTransform
#pragma region 1. Menu
	shared_ptr<CTransform>									m_pIcon_Background;
	shared_ptr<CTransform>									m_pIcon_On;
	shared_ptr<CTransform>									m_pIcon_Off;
	shared_ptr<CTransform>									m_pMenu;
	shared_ptr<CTransform>									m_pSelector[4];
	shared_ptr<CTransform>									m_pLine[3];
#pragma endregion
#pragma region 2. Gift Window
	shared_ptr<CTransform>									m_pWindow_Backgrouund;
	shared_ptr<CTransform>									m_pWindow_Title;
	shared_ptr<CTransform>									m_pProfileFrames[2];
	shared_ptr<CTransform>									m_pGiftFrames[3];
	shared_ptr<CTransform>									m_pProfileFrame;
	shared_ptr<CTransform>									m_pProfile;
	shared_ptr<CTransform>									m_pGiftIcon[2];
	shared_ptr<CTransform>									m_pActivateProfile[5];
	shared_ptr<CTransform>									m_pActivateCharacter[5];
	shared_ptr<CTransform>									m_pMiniFillter;
	shared_ptr<CTransform>									m_pGift;
	shared_ptr<CTransform>									m_pButton[2];
	shared_ptr<CTransform>									m_pReAnswer;
	shared_ptr<CTransform>									m_pReAnswerSelector[4];
#pragma endregion
#pragma region Arasi_JoinParty
	shared_ptr<CTransform>									m_pJoinBackground;
	shared_ptr<CTransform>									m_pEnterAgain;
#pragma endregion

#pragma endregion
#pragma region CTexture
#pragma region 1. Menu
	shared_ptr<CTexture>									m_pTexture_Icon;
	shared_ptr<CTexture>									m_pTexture_Menu;
	shared_ptr<CTexture>									m_pTexture_Selector;
	shared_ptr<CTexture>									m_pTexture_Line;
#pragma endregion
#pragma region 2. Gift Window
	shared_ptr<CTexture>									m_pTexture_WindowBackground;
	shared_ptr<CTexture>									m_pTexture_WindowTitle;
	shared_ptr<CTexture>									m_pTexture_ProfileFrame;
	shared_ptr<CTexture>									m_pTexture_GiftFrame;
	shared_ptr<CTexture>									m_pTexture_Profile;
	shared_ptr<CTexture>									m_pTexture_GiftIcon;
	shared_ptr<CTexture>									m_pTexture_ActivateProfile;
	shared_ptr<CTexture>									m_pTexture_ActivateCharacter;
	shared_ptr<CTexture>									m_pTexture_Gift;
	shared_ptr<CTexture>									m_pTexture_Button;
#pragma endregion

#pragma endregion
#pragma region Others
	shared_ptr<CShader>										m_pShader;
	shared_ptr<CVIBuffer_Rect>								m_pVIBuffer_Rect;
#pragma endregion
#pragma region _bool
	_bool													m_bRenderIconSwitch;
	_bool													m_bRenderMenuSwitch;
	_bool													m_bRenderGiftWindow;

	_bool													m_bRenderDegree;
	_bool													m_bNearIcon;
	_bool													m_bCanKeyInput;
	_bool													m_bScaleDoneWindow;
	_bool													m_bScaleDoneTitle;
	_bool													m_bOpenReAnswer;
	_bool													m_bCloseReAnswer;
	_bool													m_bJoin;
#pragma endregion
#pragma region _int
	_int													m_iBlinkIndex;
	_int													m_iReAnswerBlinkIndex;
	_int													m_iOpenSystem_Phase;
	_int													m_iOpenMenu_Squence;
	_int													m_iMenu_SelectorIndex;
	_int													m_iGiftWindowOpen_Phase;
	_int													m_iReAnswerOpen_Phase;
	_int													m_iMenu_ReAnswerSelectorIndex;
	_uint													m_iLove;
	_uint													m_iPossesion;
	_uint													m_iJoin_Phase;
#pragma endregion
#pragma region _float
	_float													m_fSelectorAlpha;
	_float													m_fReAnswerSelectorAlpha;
	_float													m_fBlinkTime;
	_float													m_fReAnswerBlinkTime;
	_float													m_fLineAlpha;
	_float													m_fMenuAlpha;
	_float													m_fWindowBackgroundAlpha;
	_float													m_fWindowTitleAlpha;
	_float													m_fTextAlpha;
	_float													m_fReAnswerAlpha;
	_float													m_fJoinAlpha;
	_float													m_fAccTime;
#pragma endregion
#pragma region _float3
	_float3													m_vScale_Icon_Background;
	_float3													m_vScale_Icon_On;
	_float3													m_vScale_Icon_Off;
	_float3													m_vScale_Menu;
	_float3													m_vScale_Selector[4];
	_float3													m_vScale_Line[3];
	_float3													m_vScale_WindowBackground;
	_float3													m_vScale_WindowTitle;
	_float3													m_vScale_ProfileFrameUpDw;
	_float3													m_vScale_ProfileFrame;
	_float3													m_vScale_Profile;
	_float3													m_vScale_GiftFrameUp;
	_float3													m_vScale_GiftFrameDw;
	_float3													m_vScale_GiftListFrame;
	_float3													m_vScale_GiftIcon[2];
	_float3													m_vScale_ActivateProfile;
	_float3													m_vScale_ActivateCharacter;
	_float3													m_vScale_MiniFillter;
	_float3													m_vScale_Gift;
	_float3													m_vScale_Button;
	_float3													m_vScale_ReAnswer;
	_float3													m_vScale_ReAnswerSelector[4];
	_float3													m_vScale_JoinParty;

#pragma endregion
#pragma region _float4
	_float4													m_vPosition_Icon;
	_float4													m_vPosition_Menu;
	_float4													m_vPosition_Selector[4];
	_float4													m_vPosition_WindowBackground;
	_float4													m_vPosition_WindowTitle;
	_float4													m_vPosition_ProfileFrameUpDw[2];
	_float4													m_vPosition_ProfileFrame;
	_float4													m_vPosition_Profile;
	_float4													m_vPosition_Line[2];
	_float4													m_vPosition_GiftFrameUpDw[2];
	_float4													m_vPosition_GiftListFrame;
	_float4													m_vPosition_GiftIcon[2];
	_float4													m_vPosition_ActivateProfile[5];
	_float4													m_vPosition_ActivateCharacter[5];
	_float4													m_vPosition_MiniFillter;
	_float4													m_vPosition_Gift;
	_float4													m_vPosition_Button[2];
	_float4													m_vPosition_ReAnswer;
	_float4													m_vPosition_ReAnswerSelector[4];
	_float4													m_vPosition_JoinParty;
	_float4													m_vPosition_EnterAgain;
#pragma endregion
#pragma region wstring
	wstring													m_strMenuTag[2];
	wstring													m_strGiftWindowTitle;
	wstring													m_strArasiSpring;
	wstring													m_strLove;
	wstring													m_strGift;
	wstring													m_strGiftPossesion;
	wstring													m_strHowManyPossese;
	wstring													m_strMaxPossese;
	wstring													m_strExplain[2];
	wstring													m_strButton[2];
	wstring													m_strReAnswer[3];
	wstring													m_strJoinParty;
	wstring													m_strJoinParty_Check;
#pragma endregion

public:
	static shared_ptr<CUI_Indicator_Communication>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>							Clone(any = g_aNull) override;

};

END