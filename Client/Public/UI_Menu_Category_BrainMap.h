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

class CUI_Menu_Category_BrainMap final : public CGameObject
{
private:
	explicit CUI_Menu_Category_BrainMap(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Category_BrainMap() DEFAULT;

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

private:
	void											Menu_Shift();
	void											Set_BrainShader(_float fTimeDelta);
	void											Set_MousePos();
	void											Set_MouseIndicator_Shader(_float fTimeDelta);
	void											Set_MouseIndicator_Position(_float fTimeDelta);
	void											Set_ConnectorBlink(_float fTimeDelta);
	void											Set_BlackFillterClose();


private:
#pragma region CTransform
	shared_ptr<CTransform>							m_pBackground;
	shared_ptr<CTransform>							m_pSkillTags[5];
	shared_ptr<CTransform>							m_pSkillTagLines[5];
	shared_ptr<CTransform>							m_pSkillIcons[61];
	shared_ptr<CTransform>							m_pSkillColor[61];
	shared_ptr<CTransform>							m_pBM_Connector[59];
	shared_ptr<CTransform>							m_pBrain[6];
	shared_ptr<CTransform>							m_pMouseIndicator;
	shared_ptr<CTransform>							m_pSkillOpen_Gauge;

	shared_ptr<CTransform>							m_pBlackFillter;
	shared_ptr<CTransform>							m_pBlackFillterWindow;
	shared_ptr<CTransform>							m_pGetSkillTag;
	shared_ptr<CTransform>							m_pGetSkillFrameT;
	shared_ptr<CTransform>							m_pGetSkillFrameB;
	shared_ptr<CTransform>							m_pGetSkillLine;
	shared_ptr<CTransform>							m_pGetSkillReturn;

	shared_ptr<CTransform>							m_pExplainFrameT;
	shared_ptr<CTransform>							m_pExplainFrameB;
	shared_ptr<CTransform>							m_pExplainFrameLine1;
	shared_ptr<CTransform>							m_pExplainFrameLine2;
	shared_ptr<CTransform>							m_pExplainFrameLine3;
	shared_ptr<CTransform>							m_pExplainVideo;
	shared_ptr<CTransform>							m_pExplainVideoFrame;
	shared_ptr<CTransform>							m_pExplainProfile;
	shared_ptr<CTransform>							m_pExplainProfileFrame;
	shared_ptr<CTransform>							m_pExplainProfileFrameLine1;
	shared_ptr<CTransform>							m_pExplainProfileFrameLine2;
	shared_ptr<CTransform>							m_pExplainProfileFrameLine3;

#pragma endregion
#pragma region CTexture
	shared_ptr<CTexture>							m_pTexture_On;
	shared_ptr<CTexture>							m_pTexture_Off;
	shared_ptr<CTexture>							m_pTexture_SkillTags;
	shared_ptr<CTexture>							m_pTexture_SkillTagLines;
	shared_ptr<CTexture>							m_pTexture_SkillIconsOn;
	shared_ptr<CTexture>							m_pTexture_SkillIconsOff;
	shared_ptr<CTexture>							m_pTexture_SkillIconColor;
	shared_ptr<CTexture>							m_pTexture_BM_Connector;
	shared_ptr<CTexture>							m_pTexture_Brain;
	shared_ptr<CTexture>							m_pTexture_SkillOpen_Gague;

	shared_ptr<CTexture>							m_pTexture_BlackFillter;
	shared_ptr<CTexture>							m_pTexture_BlackFillterWindow;
	shared_ptr<CTexture>							m_pTexture_GetSkillTag;
	shared_ptr<CTexture>							m_pTexture_GetSkillFrame;
	shared_ptr<CTexture>							m_pTexture_GetSkillLine;
	shared_ptr<CTexture>							m_pTexture_GetSkillReturn;

	shared_ptr<CTexture>							m_pTexture_ExplainFrame;
	shared_ptr<CTexture>							m_pTexture_ExplainFrameLine;
	shared_ptr<CTexture>							m_pTexture_ExplainVideo;
	shared_ptr<CTexture>							m_pTexture_ExplainVideoFrame;
	shared_ptr<CTexture>							m_pTexture_ExplainProfile;
	shared_ptr<CTexture>							m_pTexture_ExplainProfileFrame;
	shared_ptr<CTexture>							m_pTexture_ExplainProfileFrameLine;

#pragma endregion
#pragma region Others
	shared_ptr<CShader>								m_pShader;
	shared_ptr<CVIBuffer_Rect>						m_pVIBuffer_Rect;
#pragma endregion

private:
#pragma region _bool
	_bool											m_bSkillOnOff[61];
	_bool											m_bSkillOpen_Gauge;
	_bool											m_bBlackFillter;

#pragma endregion
#pragma region _int
	_uint											m_iBrainPhase;
	_uint											m_iConnectorBlinkIndex;
	_int											m_iMouseIndicatorShaderBlink;

	_uint											m_iCursorSoundOnce[61];
	_uint											m_iClickSoundOnce[61];
	_uint											m_iActivateSoundOnce[61];
#pragma endregion
#pragma region _float
	_float											m_fTime;
	_float											m_fConnectorBlink;

	_float											m_fMaster_Position_X;
	_float											m_fMaster_Position_Y;

	_float											m_fBrainShader;
	_float											m_fBrainShader1;
	_float											m_fBrainShader2;
	_float											m_fBrainShader3;

	_float											m_fMouseIndicatorShader;
	_float											m_fSkillOpen_Gauge;
#pragma endregion
#pragma region _float3
	_float3											m_vScale_Background_On;
	_float3											m_vScale_Background_Off;

	_float3											m_vScale_SkillTags;
	_float3											m_vScale_SkillTagLines;
	_float3											m_vScale_SkillIcons;
	_float3											m_vScale_SkillColor;
	_float3											m_vScale_BM_Connector[7];
	_float3											m_vScale_Brain[6];
	_float3											m_vScale_MouseIndicator;
	_float3											m_vScale_SkillOpen_Gauge;
	_float3											m_vScale_BlackFillter;
	_float3											m_vScale_BlackFillterWindow;
	_float3											m_vScale_GetSkillTag;
	_float3											m_vScale_GetSkillFrame;
	_float3											m_vScale_GetSkillLine;
	_float3											m_vScale_GetSkillReturn;

	_float3											m_vScale_ExplainFrame;
	_float3											m_vScale_ExplainLine;
	_float3											m_vScale_Video;
	_float3											m_vScale_VideoFrame;
	_float3											m_vScale_Profile;
	_float3											m_vScale_ProfileFrame;
	_float3											m_vScale_ProfileFrameLine;
#pragma endregion
#pragma region _float4
	_float4											m_vPosition_Background_On;
	_float4											m_vPosition_Background_Off;

	_float4											m_vPosition_SkillTags[5];
	_float4											m_vPosition_SkillTagLines[5];
	_float4											m_vPosition_SkillIcons[61];
	_float4											m_vPosition_SkillColor[61];
	_float4											m_vPosition_BM_Connector[60];
	_float4											m_vPosition_Brain[6];
	_float4											m_vPosition_MouseIndicator;
	_float4											m_vPosition_SkillOpen_Gauge;
	_float4											m_vPosition_BlackFillter;
	_float4											m_vPosition_BlackFillterWindow;
	_float4											m_vPosition_GetSkillTag;
	_float4											m_vPosition_GetSkillFrame[2];
	_float4											m_vPosition_GetSkillLine;
	_float4											m_vPosition_GetSkillReturn;

	_float4											m_vPosition_ExplainFrameT;
	_float4											m_vPosition_ExplainFrameB;
	_float4											m_vPosition_ExplainFrameLine[3];
	_float4											m_vPosition_Video;
	_float4											m_vPosition_VideoFrame;
	_float4											m_vPosition_Profile;
	_float4											m_vPosition_ProfileFrame;
	_float4											m_vPosition_ProfileFrameLine[3];


#pragma endregion

private:
	POINT											m_ptMousePos;

private:
	wstring											m_strFont = TEXT("");
	wstring											m_strSkillTags[5];
	wstring											m_strGetSkillTag;
	wstring											m_strGetSkillName;
	wstring											m_strGetSkillEnd;
	wstring											m_strGetSkillClose;

	wstring											m_strExplainTitle;
	wstring											m_strHP;
	wstring											m_strAblilty;
	wstring											m_strAttack;
	wstring											m_strDefence;
	wstring											m_strExplain1;
	wstring											m_strExplain2;
	wstring											m_strExplain3;
	wstring											m_strExplain4;
	wstring											m_strGetorNot;

	wstring											m_strPlayerHp;
	wstring											m_strPlayerAblity;
	wstring											m_strPlayerAttack;
	wstring											m_strPlayerDefence;

public:
	static shared_ptr<CUI_Menu_Category_BrainMap>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END