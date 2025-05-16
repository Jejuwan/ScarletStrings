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

class CUI_Menu_Profile_ID_Card final : public CGameObject
{
private:
	explicit CUI_Menu_Profile_ID_Card(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Profile_ID_Card() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any _aDesc = g_aNull)	override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;
	 
private:
	virtual HRESULT									Ready_Components() override;

public:
	void											KeyWorks();

private:
	void											Tick_UI();
	void											Set_HPPosition();

private:
	shared_ptr<CTransform>							m_pBackground;
	shared_ptr<CTransform>							m_pFrameUp;
	shared_ptr<CTransform>							m_pFrameDw;
	shared_ptr<CTransform>							m_pID_Frame;
	shared_ptr<CTransform>							m_pProfile1;
	shared_ptr<CTransform>							m_pProfile2;
	shared_ptr<CTransform>							m_pProfile3;
	shared_ptr<CTransform>							m_pProfile4;
	shared_ptr<CTransform>							m_pProfile5;
	shared_ptr<CTransform>							m_pSkillIcon1;
	shared_ptr<CTransform>							m_pSkillIcon2;
	shared_ptr<CTransform>							m_pSkillIcon3;
	shared_ptr<CTransform>							m_pSkillIcon4;
	shared_ptr<CTransform>							m_pSkillIcon5;
	shared_ptr<CTransform>							m_pSkillTag;
	shared_ptr<CTransform>							m_pLoveIcon;
	shared_ptr<CTransform>							m_pLoveIndex1;
	shared_ptr<CTransform>							m_pLoveIndex2;
	shared_ptr<CTransform>							m_pLoveIndex3;
	shared_ptr<CTransform>							m_pLoveIndex4;
	shared_ptr<CTransform>							m_pLoveIndex5;
	shared_ptr<CTransform>							m_pProfileHp1;
	shared_ptr<CTransform>							m_pProfileHp2;
	shared_ptr<CTransform>							m_pProfileHp3;
	shared_ptr<CTransform>							m_pProfileHp4;
	shared_ptr<CTransform>							m_pProfileHp5;
	shared_ptr<CTransform>							m_pProfileHpBackground;
	shared_ptr<CTransform>							m_pEXP;

	
	shared_ptr<CTexture>							m_pTexture_Background;
	shared_ptr<CTexture>							m_pTexture_FrameUp;
	shared_ptr<CTexture>							m_pTexture_FrameDw;
	shared_ptr<CTexture>							m_pTexture_ID_Frame;
	shared_ptr<CTexture>							m_pTexture_Profile1;
	shared_ptr<CTexture>							m_pTexture_Profile2;
	shared_ptr<CTexture>							m_pTexture_Profile3;
	shared_ptr<CTexture>							m_pTexture_Profile4;
	shared_ptr<CTexture>							m_pTexture_Profile5;
	shared_ptr<CTexture>							m_pTexture_SkillIcon1;
	shared_ptr<CTexture>							m_pTexture_SkillIcon2;
	shared_ptr<CTexture>							m_pTexture_SkillIcon3;
	shared_ptr<CTexture>							m_pTexture_SkillIcon4;
	shared_ptr<CTexture>							m_pTexture_SkillIcon5;
	shared_ptr<CTexture>							m_pTexture_SkillTag;
	shared_ptr<CTexture>							m_pTexture_LoveIcon;
	shared_ptr<CTexture>							m_pTexture_LoveIndex1;
	shared_ptr<CTexture>							m_pTexture_LoveIndex2;
	shared_ptr<CTexture>							m_pTexture_LoveIndex3;
	shared_ptr<CTexture>							m_pTexture_LoveIndex4;
	shared_ptr<CTexture>							m_pTexture_LoveIndex5;
	shared_ptr<CTexture>							m_pTexture_ProfileHp1;
	shared_ptr<CTexture>							m_pTexture_ProfileHp2;
	shared_ptr<CTexture>							m_pTexture_ProfileHp3;
	shared_ptr<CTexture>							m_pTexture_ProfileHp4;
	shared_ptr<CTexture>							m_pTexture_ProfileHp5;
	shared_ptr<CTexture>							m_pTexture_ProfileHpBackground;
	shared_ptr<CTexture>							m_pTexture_EXP;


	shared_ptr<CShader>								m_pShader;
	shared_ptr<CVIBuffer_Rect>						m_pVIBuffer_Rect;

private:
	// Background
	_float3											m_vScale_Background;
	_float4											m_vPosition_Background;

	// Frame
	_float3											m_vScale_Frame_Up;
	_float3											m_vScale_Frame_Dw;
	_float4											m_vPosition_Frame_Up;
	_float4											m_vPosition_Frame_Dw;

	// ID_Frame
	_float3											m_vScale_ID_Frame;
	_float4											m_vPosition_ID_Frame;

	// Profile Picture
	_float3											m_vScale_Profile1;
	_float3											m_vScale_Profile2;
	_float3											m_vScale_Profile3;
	_float3											m_vScale_Profile4;
	_float3											m_vScale_Profile5;
	_float4											m_vPosition_Profile1;
	_float4											m_vPosition_Profile2;
	_float4											m_vPosition_Profile3;
	_float4											m_vPosition_Profile4;
	_float4											m_vPosition_Profile5;

	// SkillIcon
	_float3											m_vScale_SkillIcon1;
	_float3											m_vScale_SkillIcon2;
	_float3											m_vScale_SkillIcon3;
	_float3											m_vScale_SkillIcon4;
	_float3											m_vScale_SkillIcon5;
	_float4											m_vPosition_SkillIcon1;
	_float4											m_vPosition_SkillIcon2;
	_float4											m_vPosition_SkillIcon3;
	_float4											m_vPosition_SkillIcon4;
	_float4											m_vPosition_SkillIcon5;

	// SkillTag
	_float3											m_vScale_SkillTag;
	_float4											m_vPosition_SkillTag;

	// LoveIcon
	_float3											m_vScale_LoveIcon;
	_float4											m_vPosition_LoveIcon;

	// LoveIndex
	_float3											m_vScale_LoveIndex1;
	_float3											m_vScale_LoveIndex2;
	_float3											m_vScale_LoveIndex3;
	_float3											m_vScale_LoveIndex4;
	_float3											m_vScale_LoveIndex5;
	_float4											m_vPosition_LoveIndex1;
	_float4											m_vPosition_LoveIndex2;
	_float4											m_vPosition_LoveIndex3;
	_float4											m_vPosition_LoveIndex4;
	_float4											m_vPosition_LoveIndex5;

	// ProfileHp
	_float3											m_vScale_ProfileHp1;
	_float3											m_vScale_ProfileHp2;
	_float3											m_vScale_ProfileHp3;
	_float3											m_vScale_ProfileHp4;
	_float3											m_vScale_ProfileHp5;

	_float3											m_vScale_ProfileHpBackground;

	_float4											m_vPosition_ProfileHp1;
	_float4											m_vPosition_ProfileHp2;
	_float4											m_vPosition_ProfileHp3;
	_float4											m_vPosition_ProfileHp4;
	_float4											m_vPosition_ProfileHp5;
	
	_float4											m_vPosition_ProfileHpBackground;

	// EXP
	_float3											m_vScale_EXP;
	_float4											m_vPosition_EXP;

private:
	_float											m_fHp1;
	_float											m_fHpMax1;
	_float											m_fHp2;
	_float											m_fHpMax2;
	_float											m_fHp3;
	_float											m_fHpMax3;
	_float											m_fHp4;
	_float											m_fHpMax4;
	_float											m_fHp5;
	_float											m_fHpMax5;

	_float											m_fRatio1;
	_float											m_fRatio2;
	_float											m_fRatio3;
	_float											m_fRatio4;
	_float											m_fRatio5;

	_float											m_fPercentage1;
	_float											m_fPercentage2;
	_float											m_fPercentage3;
	_float											m_fPercentage4;
	_float											m_fPercentage5;

	_uint											m_iLevel;
	_uint											m_iEXP;
	_uint											m_iFromNowOn;
	_uint											m_iAp;
	_uint											m_iAtk;
	_uint											m_iDef;

private:
	wstring											m_strName			= TEXT("");
	wstring											m_strLevelTag		= TEXT("");
	wstring											m_strLevelIndex		= TEXT("");
	wstring											m_strFromNowOnTag	= TEXT("");
	wstring											m_strFromNowOnIndex	= TEXT("");
	wstring											m_strEXP			= TEXT("");
	wstring											m_strHpTag			= TEXT("");
	wstring											m_strHp				= TEXT("");
	wstring											m_strHpSeperate		= TEXT("");
	wstring											m_strHpMax			= TEXT("");
	wstring											m_strAPTag			= TEXT("");
	wstring											m_strAp				= TEXT("");
	wstring											m_strAtkTag			= TEXT("");
	wstring											m_strAtk			= TEXT("");
	wstring											m_strDefTag			= TEXT("");
	wstring											m_strDef			= TEXT("");
	wstring											m_strPsycoTag		= TEXT("");
	wstring											m_strOperation		= TEXT("");
	wstring											m_strOperations		= TEXT("");
	wstring											m_strIDNumber		= TEXT("");


public:
	static shared_ptr<CUI_Menu_Profile_ID_Card>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END