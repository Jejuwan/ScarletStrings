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

class CUI_Indicator_Name_Town final : public CGameObject
{
private:
	explicit CUI_Indicator_Name_Town(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Name_Town() DEFAULT;

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

public:
	void										Start_Title_Animation_Trigger();
	void										Start_Animation(_float fTimeDelta);
	void										Start_Line_M_Become_Longer(_float fTimeDelta);
	void										Start_Line_Movement(_float fTimeDelta);
	void										End_Animation(_float fTimeDelta);

private:
	shared_ptr<CTransform>						m_pBackground_1;
	shared_ptr<CTransform>						m_pBackground_2;
	shared_ptr<CTransform>						m_pLine_L1;
	shared_ptr<CTransform>						m_pLine_L2;
	shared_ptr<CTransform>						m_pLine_M;
	shared_ptr<CTransform>						m_pLine_R1;
	shared_ptr<CTransform>						m_pLine_R2;

private:
	shared_ptr<CTexture>						m_pTexture_Background_1;
	shared_ptr<CTexture>						m_pTexture_Background_2;
	shared_ptr<CTexture>						m_pTexture_Line_L1;
	shared_ptr<CTexture>						m_pTexture_Line_L2;
	shared_ptr<CTexture>						m_pTexture_Line_M;
	shared_ptr<CTexture>						m_pTexture_Line_R1;
	shared_ptr<CTexture>						m_pTexture_Line_R2;

private:
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;

private:
	_bool										m_bBackground1_Start;
	_bool										m_bBackground2_Start;
	_bool										m_bRenderFont_Start;
	_bool										m_bDisappear_Start;

private:
	_uint										m_iBlinkIndex; 
	_uint										m_iAnimCount;

private:
	_float										m_fTime_Background_1;
	_float										m_fTime_Background_2;
	_float										m_fTime_Line_L1;
	_float										m_fTime_Line_L2;
	_float										m_fTime_Line_M;
	_float										m_fTime_Line_R1;
	_float										m_fTime_Line_R2;
	_float										m_fTime_RenderFontAlpha_OutLine;
	_float										m_fAccTime;
	_float										m_fAppearTime;

private:
	_float3										m_vScale_Background_1;
	_float3										m_vScale_Background_2;
	_float3										m_vScale_Line_L1;
	_float3										m_vScale_Line_L2;
	_float3										m_vScale_Line_M;
	_float3										m_vScale_Line_R1;
	_float3										m_vScale_Line_R2;

private:
	_float4										m_vPosition_Background_1;
	_float4										m_vPosition_Background_2;
	_float4										m_vPosition_Line_L1;
	_float4										m_vPosition_Line_L2;
	_float4										m_vPosition_Line_M;
	_float4										m_vPosition_Line_R1;
	_float4										m_vPosition_Line_R2;

private:
	wstring										m_strNameTitle_OutLine; // goo ro digital
	wstring										m_strNameSubTitle;
	
	wstring										m_strNameTitle_OutLine_Suoh; // Suoh


public:
	static shared_ptr<CUI_Indicator_Name_Town>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END