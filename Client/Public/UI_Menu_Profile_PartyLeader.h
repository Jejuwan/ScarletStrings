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

class CUI_Menu_Profile_PartyLeader final : public CGameObject
{
private:
	explicit CUI_Menu_Profile_PartyLeader(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Menu_Profile_PartyLeader() DEFAULT;

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
	void											Set_HPPosition();

private:
	shared_ptr<CTransform>							m_pBackground;
	shared_ptr<CTransform>							m_pProfileFrame;
	shared_ptr<CTransform>							m_pProfileIndex;
	shared_ptr<CTransform>							m_pLEADER;
	shared_ptr<CTransform>							m_pProfile;
	shared_ptr<CTransform>							m_pSkillIconFrame;
	shared_ptr<CTransform>							m_pSkillIcon;
	shared_ptr<CTransform>							m_pSkillTag;
	shared_ptr<CTransform>							m_pLoveIcon;
	shared_ptr<CTransform>							m_pLoveIndex;
	shared_ptr<CTransform>							m_pProfileHp;
	shared_ptr<CTransform>							m_pProfileHpBackground;
	

	shared_ptr<CTexture>							m_pTexture_On;
	shared_ptr<CTexture>							m_pTexture_Off;
	shared_ptr<CTexture>							m_pTexture_ProfileFrame;
	shared_ptr<CTexture>							m_pTexture_ProfileIndex;
	shared_ptr<CTexture>							m_pTexture_LEADER;
	shared_ptr<CTexture>							m_pTexture_Profile;
	shared_ptr<CTexture>							m_pTexture_SkillIconFrame;
	shared_ptr<CTexture>							m_pTexture_SkillIcon;
	shared_ptr<CTexture>							m_pTexture_SkillTag;
	shared_ptr<CTexture>							m_pTexture_LoveIcon;
	shared_ptr<CTexture>							m_pTexture_LoveIndex;
	shared_ptr<CTexture>							m_pTexture_ProfileHp;
	shared_ptr<CTexture>							m_pTexture_ProfileHpBackground;

	shared_ptr<CShader>								m_pShader;
	shared_ptr<CVIBuffer_Rect>						m_pVIBuffer_Rect;

private:
	// Frame On / Off
	_float3											m_vScaleOn;
	_float3											m_vScaleOff;
	_float4											m_vPositionOn;
	_float4											m_vPositionOff;

	// Profile Frame On / Off
	_float3											m_vScale_ProfileFrameOn;
	_float3											m_vScale_ProfileFrameOff;
	_float4											m_vPosition_ProfileFrameOn;
	_float4											m_vPosition_ProfileFrameOff;

	// Profile Index On / Off
	_float3											m_vScale_ProfileIndexOn;
	_float3											m_vScale_ProfileIndexOff;	
	_float4											m_vPosition_ProfileIndexOn;
	_float4											m_vPosition_ProfileIndexOff;
	 
	// LEADER On / Off
	_float3											m_vScale_LEADEROn;
	_float3											m_vScale_LEADEROff;
	_float4											m_vPosition_LEADEROn;
	_float4											m_vPosition_LEADEROff;

	// Profile
	_float3											m_vScale_ProfileOn;
	_float3											m_vScale_ProfileOff;
	_float4											m_vPosition_ProfileOn;
	_float4											m_vPosition_ProfileOff;

	// SkillICon
	_float3											m_vScale_SkillIconFrameOn;
	_float3											m_vScale_SkillIconFrameOff;
	_float4											m_vPosition_SkillIconFrameOn;
	_float4											m_vPosition_SkillIconFrameOff;

	// SkillICon
	_float3											m_vScale_SkillIconOn;
	_float3											m_vScale_SkillIconOff;
	_float4											m_vPosition_SkillIconOn;
	_float4											m_vPosition_SkillIconOff;

	// SkillTag
	_float3											m_vScale_SkillTagOn;
	_float3											m_vScale_SkillTagOff;
	_float4											m_vPosition_SkillTagOn;
	_float4											m_vPosition_SkillTagOff;

	// LoveIcon
	_float3											m_vScale_LoveIconOn;
	_float3											m_vScale_LoveIconOff;
	_float4											m_vPosition_LoveIconOn;
	_float4											m_vPosition_LoveIconOff;

	// LoveIndex
	_float3											m_vScale_LoveIndexOn;
	_float3											m_vScale_LoveIndexOff;
	_float4											m_vPosition_LoveIndexOn;
	_float4											m_vPosition_LoveIndexOff;

	// ProfileHp
	_float3											m_vScale_ProfileHpOn;
	_float3											m_vScale_ProfileHpOff;
	_float4											m_vPosition_ProfileHpOn;
	_float4											m_vPosition_ProfileHpOff;
	_float3											m_vScale_ProflieHpBackground;
	_float4											m_vPosition_ProfileHpBackground;

private:
	_int											m_iHp;
	_float											m_fHp;
	_int											m_iHpMax;
	_float											m_fHpMax;
	_uint											m_iLevel;

	_float											m_fRatio;
	_float											m_fPercentage;

private:
	shared_ptr<class CMenuCharacter>				m_pMenuCharacter;

private:
	wstring											m_strName			= TEXT("");
	wstring											m_strLevelTag		= TEXT("");
	wstring											m_strLevelIndex		= TEXT("");
	wstring											m_strHpTag			= TEXT("");
	wstring											m_strHp				= TEXT("");
	wstring											m_strHpSeperate		= TEXT("");
	wstring											m_strHpMax			= TEXT("");
	wstring											m_strPsyco			= TEXT("");


public:
	static shared_ptr<CUI_Menu_Profile_PartyLeader>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END