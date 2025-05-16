#pragma once
#include "Client_Define.h"

BEGIN(Engine)
class CShader;
END


BEGIN(Client)

class CUI_Manager : public CSingleton<CUI_Manager>
{
private:
	explicit CUI_Manager();
	virtual ~CUI_Manager() DEFAULT;

#ifdef _DEBUG
public:
	void Debug_Render();
#endif

public:
	HRESULT		Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext);
	void		Tick(_float fTimeDelta);
	void		Late_Tick(_float fTimeDelta);
	HRESULT		Render();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function
public:		/* Getter */
	_bool		Get_UI_Bound()												{ return m_bUI_Bound; }
	_bool		Get_UI_CanFade()											{ return m_bCanFade; }
	_bool		Get_OnOffSwitch_RenderUI()									{ return m_bOnOffSwitch_RenderUI; }
	_bool		Get_BrainTalk()												{ return m_bBrainTalk; }
	_bool		Get_MindRoomCutScene()										{ return m_bMindRoom_CutScene_End; }
	_bool		Get_MouseClick()											{ return m_bMouseClick; }
	_bool		Get_BrainTalk_RenderNow()									{ return m_bBrainTalk_RenderNow; }
	_bool		Get_NoDamage()												{ return m_bNoDamage; }


	_int		Get_UI_All_RenderStop()										{ return m_bUI_All_RenderStop; }
	_int		Get_MenuIndex()												{ return m_iMenuIndex; }
	_int		Get_MenuSwitch()											{ return m_iMenuSwitch; }
	_int		Get_SelectMemberIndex()										{ return m_iSelect_MemberIndex; }
	_int		Get_ItemIndex()												{ return m_iItemIndex; }
	_int		Get_ItemCount()												{ return m_iItemCount; }
	_int		Get_FadeInOut()												{ return m_iFadeInOut; }
	_int		Get_DamageType()											{ return m_iDamageType; }
	_int		Get_Possession()											{ return m_iPossession; }
	_int		Get_GiftPossession()										{ return m_iGift_Possession; }
	_uint		Get_K()														{ return m_iK; }
	_uint		Get_BP()													{ return m_iBP; }
	_float3		Get_IndicatorScale()										{ return m_vIndicatorScale; }
	_matrix		Get_DamagePosition()										{ return m_vPosition_Damage; }

	shared_ptr<class CUI_Psyco_Wave>										Get_Psyco_Wave();
	shared_ptr<class CUI_Indicator_ObjectAttack>							Get_Indicator();
	shared_ptr<class CUI_Indicator_SpcObjectAttack>							Get_IndicatorSpc();
	shared_ptr<class CUI_Indicator_Tooltip_Tutorial_PMCC>					Get_Indicator_ToolTip_Tutorial_PMCC();
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_LockOn>					Get_Indicator_ToolTip_Tutorial_LockOn();
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle>		Get_Indicator_ToolTip_Tutorial_Kasane_BattleStyle();
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack>	Get_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack();
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Success>				Get_Indicator_ToolTip_Tutorial_Success();
	shared_ptr<class CUI_Indicator_BrainTalk_Tutorial>						Get_Indicator_BrainTalk_Tutorial();
	shared_ptr<class CUI_Indicator_BrainTalk_Mizuha>						Get_Indicator_BrainTalk_Mizuha();
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Tips>					Get_Indicator_ToolTip_Tutorial_Tips();
	shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Electric>				Get_Indicator_ToolTip_Mizuha_SAS_Electric();
	shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration>			Get_Indicator_ToolTip_Mizuha_SAS_Acceleration();
	shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Replication>			Get_Indicator_ToolTip_Mizuha_SAS_Replication();
	shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Stealth>				Get_Indicator_ToolTip_Mizuha_SAS_Stealth();
	shared_ptr<class CUI_Indicator_Script>									Get_Script();
	shared_ptr<class CUI_Indicator_Communication>							Get_Indicator_Communication();
public:	/* Setter */
	void		Set_UI_KPlus()												{ ++m_iK; }
	void		Set_UI_KMinus()
	{
		--m_iK;
		if (0 >= m_iK)
			m_iK = 0;
	}
	void		Set_UI_BPPlus()												{ ++m_iBP; }
	void		Set_UI_BPMinus()
	{
		--m_iBP; 
		if (0 >= m_iBP)
			m_iBP = 0;
	}

	void		Set_UI_K(_int ik)											{ m_iK = ik; };
	void		Set_UI_Possession_Plus(_int iPossession)					{ m_iPossession += iPossession; };
	void		Set_UI_Gift_Possession_Plus(_int iPossession)				{ m_iGift_Possession += iPossession; };
	void		Set_UI_Gift_Possession_Minus(_int iPossession)				{ m_iGift_Possession -= iPossession; };
	void		Set_UI_CanFadeTrue()										{ m_bCanFade = true; }
	void		Set_UI_CanFadeFalse()										{ m_bCanFade = false; }
	void		Set_BrainTalk_True()										{ m_bBrainTalk = true; }
	void		Set_BrainTalk_False()										{ m_bBrainTalk = false; }
	void		Set_MindRoomCutScene(_bool bMindRoomCutScene)				{ m_bMindRoom_CutScene_End = bMindRoomCutScene; }
	void		Set_UI_FadeInOut()											{ m_iFadeInOut *= -1; }
	void		Set_UI_FadeInOutDefault()									{ m_iFadeInOut = -1; }
	void		Set_UI_Bound(_bool bBound)									{ m_bUI_Bound = bBound; }
	void		Set_UI_All_RenderStopTrue()									{ m_bUI_All_RenderStop = true; }
	void		Set_UI_All_RenderStopFalse()								{ m_bUI_All_RenderStop = false; }
	void		Set_DamageType(_int iDamage)								{ m_iDamageType = iDamage; }
	void		Set_MouseClick(_bool bClick)								{ m_bMouseClick = bClick; }
	void		Set_BrainTalk_RenderNow(_bool bBrainTalkRender)				{ m_bBrainTalk_RenderNow = bBrainTalkRender; }
	void		Set_MenuIndexZero()											{ m_iMenuIndex = 0;}
	void		Set_MenuIndexPlus()
	{
		m_iMenuIndex += 1; 

		if (m_iMenuIndex > 7)
			m_iMenuIndex = 0;
	}
	void		Set_MenuIndexMlnus()
	{
		m_iMenuIndex -= 1;

		if (m_iMenuIndex < 0)
			m_iMenuIndex = 7;

	}
	void		Set_Select_MemberIndexZero()							{ m_iSelect_MemberIndex = 0; }
	void		Set_Select_MemberIndexPlus()
	{
		m_iSelect_MemberIndex += 1;

		if (m_iSelect_MemberIndex > 4)
			m_iSelect_MemberIndex = 4;
	}
	void		Set_Select_MemberIndexMlnus()
	{
		m_iSelect_MemberIndex -= 1;

		if (m_iSelect_MemberIndex < 0)
			m_iSelect_MemberIndex = 0;

	}
	void		Set_ItemCountMinus()
	{
		m_iItemCount -= 1;

		if (0 >= m_iItemCount)
			m_iItemCount = 0;
	}
	void		Set_ItemCountPlus()
	{
		if (10 <= m_iItemCount)
		{
			m_iItemCount = 10;
		}
		else
		{
			m_iItemCount += 1;
		}
	}
	void		Set_ItemIndexMinus()
	{
		m_iItemIndex -= 1;

		if (0 >= m_iItemIndex)
			m_iItemIndex = 0;
	}
	void		Set_ItemIndexPlus()
	{
		if (10 <= m_iItemIndex)
		{
			m_iItemIndex = 10;
		}
		else
		{
			m_iItemIndex += 1;
		}
	}
	void		Set_MenuSwitchOn()										{ m_iMenuSwitch = 1; }
	void		Set_MenuSwitchOff()										{ m_iMenuSwitch = -1; }
	void		Set_OnOffSwitch_RenderUI(_bool _bOnOffSwitch)			{ m_bOnOffSwitch_RenderUI = _bOnOffSwitch; }
	void		Set_IndicatorScale(_float3 _vIndicatorScale)			{ m_vIndicatorScale = _vIndicatorScale; }
	void		Set_DamagePosition(_matrix _vDamagePosition)			{ m_vPosition_Damage = _vDamagePosition; }
	void		Set_NoDamage(_bool _bNoDamage)							{ m_bNoDamage = _bNoDamage; }

	void		Set_LovePlus()											{ m_iLove += 1; }
	_uint		Get_Love()												{ return m_iLove; }
	void		Set_ArasiJoinParty()									{ m_bArasiJoinParty += 1; }
	_uint		Get_ArasiJoinParty()									{ return m_bArasiJoinParty; }
public:
	HRESULT		Bind_Matixies(shared_ptr<CShader>);

// SAS Cutscene
public:
	void		Bind_Character(CHARACTER, shared_ptr<CGameObject>);
	HRESULT		Play_SAS_Cutscene(CHARACTER, const std::function<void()>& fpFinished = nullptr);
	HRESULT		Open_Drive_Mode(CHARACTER, const std::function<void()>& fpFinished = nullptr);

private:
	unordered_map<CHARACTER, shared_ptr<class CCharacter>> m_mapCharacter;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	_bool		Get_SkillIconBecomeActivated(_uint iSkillNumber)						{ return m_bSkillIconBecomeActivated[iSkillNumber]; }
	void		Set_SkillIconBecomeActivated(_uint iSkillNumber, _bool bSkillNumber)	{ m_bSkillIconBecomeActivated[iSkillNumber] = bSkillNumber; }
	_bool		Get_TutorialRender()						{ return m_bTutorialRender; }
	void		Set_TutorialRender(_bool bTutorialRender)	{ m_bTutorialRender = bTutorialRender; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void		Shop_UI_Control();
public:
	_bool		Get_IsShop() { return m_IsShop; };
	_bool		Get_Enter_ShopUI() { return m_bEnter_ShopUI; };
	_bool		Get_Buy_CheckBox() { return m_bBuy_CheckBox; };
	_bool		Get_Buy() { return m_bBuy; };

	_int		Get_ShopMenu_Index() { return m_iShopMenu_Index; };
	_int		Get_ShopSubMenu_Index() { return m_iShopSubMenu_Index; };
	_int		Get_Shop_Catalog_Index() { return m_iShop_Catalog_index; };
	_int		Get_Shop_Number_Index() { return m_iShop_Number_index; };
	_int		Get_Buy_Check() { return m_iBuy_Check; };

	void		Set_IsShop(_bool bShop) { m_IsShop = bShop; };
	void		Set_Enter_ShopUI(_bool bFade) { m_bEnter_ShopUI = bFade; };
	void		Set_Buy(_bool bBuy) { m_bBuy = bBuy; };

	void		Set_ShopMenu_Index(_int iIndex) { m_iShopMenu_Index = iIndex; };
	void		Set_ShopSubMenu_Index(_int iIndex) { m_iShopSubMenu_Index = iIndex; };
	void		Set_Shop_Catalog_Index(_int iIndex) { m_iShop_Catalog_index = iIndex; };

	_bool		Get_InstKillCutScene()					{ return m_bInstKillCutScene; }
	void		Set_InstKillCutScene(_bool bInstKill)	{ m_bInstKillCutScene = bInstKill; }

	_bool		Get_RenderJW()							{ return m_bRenderJW; }
	void		Set_RenderJW(_bool bJW)					{ m_bRenderJW = bJW; }

private:
	_bool		m_IsShop = { false };
	_bool		m_bEnter_ShopUI = { false };
	_bool		m_bBuy_CheckBox = { false };
	_bool		m_bBuy_Check_Done = { false };
	_bool		m_bBuy = { false };
	_bool		m_bMenu_UI = { false };

	_int		m_iShopMenu_Index = { 0 };
	_int		m_iShopSubMenu_Index = { 0 };
	_int		m_iShop_Catalog_index = { 0 };
	_int		m_iShop_Number_index = { 0 };
	_int		m_iBuy_Check = { false };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variable
private:
	_bool		m_bUI_All_RenderStop;
	_bool		m_bOnOffSwitch_RenderUI;
	_bool		m_bMindRoom_CutScene_End;
	_bool		m_bBrainTalk_RenderNow;

	_bool		m_bUI_Bound;
	_bool		m_bPhysxRender = false;
	_bool		m_bCanFade;
	_bool		m_bBrainTalk;
	_bool		m_bMouseClick;
	_bool		m_bSkillIconBecomeActivated[61];
	_bool		m_bTutorialRender;

	_bool		m_bNoDamage;
	_bool		m_bInstKillCutScene;
	_bool		m_bRenderJW;
private:
	_int		m_iFadeInOut;
	_int		m_iMenuIndex;
	_int		m_iMenuSwitch;
	_int		m_iSelect_MemberIndex;
	_int		m_iItemIndex;
	_int		m_iItemCount;
	_int		m_iK;
	_int		m_iBP;
	_int		m_iDamageType;
	_int		m_iPossession;
	_int		m_iGift_Possession = { 0 };
	_uint		m_iLove = { 1 };
	_uint		m_bArasiJoinParty = { 0 };
private:
	_float3		m_vIndicatorScale;

private:
	_matrix		m_vPosition_Damage;

private:
	_float4x4	m_mView, m_mProj;

private:
	shared_ptr<CUI_Indicator_ObjectAttack>									m_pIndicator_ObjectAttack;
	shared_ptr<CUI_Indicator_SpcObjectAttack>								m_pIndicator_SpecialObjectAttack;

#pragma region PsycoWave
	shared_ptr<class CUI_Psyco_Wave>										m_pPsycoWave;
#pragma endregion
#pragma region Script
	shared_ptr<CUI_Indicator_Script>										m_pScript;
#pragma endregion
#pragma region ToolTip
	// SCENE::TEST
	shared_ptr<class CUI_Indicator_Tooltip_Tutorial_PMCC>					m_pIndicator_ToolTip_Tutorial_PMCC;
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_LockOn>					m_pIndicator_ToolTip_Tutorial_LockOn;
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle>		m_pIndicator_ToolTip_Tutorial_Kasane_BattleStyle;
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack>	m_pIndicator_ToolTip_Tutorial_Kasane_SpecialAttack;
	// SCENE::MIZUHA
	shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration>			m_pIndicator_ToolTip_Mizuha_SAS_Acceleration;
	shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Electric>				m_pIndicator_ToolTip_Mizuha_SAS_Electric;
	shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Replication>			m_pIndicator_ToolTip_Mizuha_SAS_Replication;
	shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Stealth>				m_pIndicator_ToolTip_Mizuha_SAS_Stealth;
#pragma endregion
#pragma region Success
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Success>				m_pIndicator_ToolTip_Tutorial_Success;
#pragma endregion
#pragma region BrainTalk
	shared_ptr<class CUI_Indicator_BrainTalk_Tutorial>						m_pIndicator_BrainTalk_Tutorial;
	shared_ptr<class CUI_Indicator_BrainTalk_Mizuha>						m_pIndicator_BrainTalk_Mizuha;
	shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Tips>					m_pIndicator_ToolTip_Tutorial_Tips;
#pragma endregion
#pragma region Communication
	shared_ptr<class CUI_Indicator_Communication>							m_pIndicator_Communication;
#pragma endregion




private:
	ComPtr<ID3D11Device>													m_pDevice;
	ComPtr<ID3D11DeviceContext>												m_pContext;

	friend		CSingleton<CUI_Manager>;
};

END