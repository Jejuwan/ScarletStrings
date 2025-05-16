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

class CUI_Indicator_Name_Boss final : public CGameObject
{
private:
	explicit CUI_Indicator_Name_Boss(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Name_Boss() DEFAULT;

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
	void										Initialize_Variables();

public:
	void										Start_Animation_Trigger();
	void										Control_Noise(_float fTimeDelta);
	void										Control_Background1(_float fTimeDelta);
	void										Control_Background2(_float fTimeDelta);
	void										Control_Background3(_float fTimeDelta);
	void										Control_RenderFont(_float fTimeDelta);
	void										Render_RenderFont();
	void										Swap_Text();

private:
#pragma region Transform
	shared_ptr<CTransform>						m_pBackground_1;
	shared_ptr<CTransform>						m_pBackground_2;
	shared_ptr<CTransform>						m_pBackground_2_2;
	shared_ptr<CTransform>						m_pBackground_3;
	shared_ptr<CTransform>						m_pNoise1;
	shared_ptr<CTransform>						m_pNoise2;
	shared_ptr<CTransform>						m_pNoise3;
	shared_ptr<CTransform>						m_pNoise4;
	shared_ptr<CTransform>						m_pNoise5;
	shared_ptr<CTransform>						m_pNoise6;
	shared_ptr<CTransform>						m_pNoise7;
	shared_ptr<CTransform>						m_pNoise8;
	shared_ptr<CTransform>						m_pNoise9;
	shared_ptr<CTransform>						m_pNoise10;
	shared_ptr<CTransform>						m_pNoise11;
	shared_ptr<CTransform>						m_pNoise12;
	shared_ptr<CTransform>						m_pNoise13;
	shared_ptr<CTransform>						m_pNoise14;
	shared_ptr<CTransform>						m_pNoise15;
	shared_ptr<CTransform>						m_pNoise16;
	shared_ptr<CTransform>						m_pNoise17;
	shared_ptr<CTransform>						m_pNoise18;
	shared_ptr<CTransform>						m_pNoise19;
	shared_ptr<CTransform>						m_pNoise20;
	shared_ptr<CTransform>						m_pNoise21;
	shared_ptr<CTransform>						m_pNoise22;
	shared_ptr<CTransform>						m_pNoise23;
	shared_ptr<CTransform>						m_pNoise24;
	shared_ptr<CTransform>						m_pNoise25;
#pragma endregion

private:
#pragma region Texture
	shared_ptr<CTexture>						m_pTexture_Background_1;
	shared_ptr<CTexture>						m_pTexture_Background_2;
	shared_ptr<CTexture>						m_pTexture_Background_2_2;
	shared_ptr<CTexture>						m_pTexture_Background_3;
	shared_ptr<CTexture>						m_pTexture_Noise1;
	shared_ptr<CTexture>						m_pTexture_Noise2;
	shared_ptr<CTexture>						m_pTexture_Noise3;
	shared_ptr<CTexture>						m_pTexture_Noise4;
	shared_ptr<CTexture>						m_pTexture_Noise5;
	shared_ptr<CTexture>						m_pTexture_Noise6;
	shared_ptr<CTexture>						m_pTexture_Noise7;
	shared_ptr<CTexture>						m_pTexture_Noise8;
	shared_ptr<CTexture>						m_pTexture_Noise9;
	shared_ptr<CTexture>						m_pTexture_Noise10;
	shared_ptr<CTexture>						m_pTexture_Noise11;
	shared_ptr<CTexture>						m_pTexture_Noise12;
	shared_ptr<CTexture>						m_pTexture_Noise13;
	shared_ptr<CTexture>						m_pTexture_Noise14;
	shared_ptr<CTexture>						m_pTexture_Noise15;
	shared_ptr<CTexture>						m_pTexture_Noise16;
	shared_ptr<CTexture>						m_pTexture_Noise17;
	shared_ptr<CTexture>						m_pTexture_Noise18;
	shared_ptr<CTexture>						m_pTexture_Noise19;
	shared_ptr<CTexture>						m_pTexture_Noise20;
	shared_ptr<CTexture>						m_pTexture_Noise21;
	shared_ptr<CTexture>						m_pTexture_Noise22;
	shared_ptr<CTexture>						m_pTexture_Noise23;
	shared_ptr<CTexture>						m_pTexture_Noise24;
	shared_ptr<CTexture>						m_pTexture_Noise25;
#pragma endregion

private:
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer_Rect;

private:
#pragma region Common
	_bool										m_bBossAppeared;
	_bool										m_bAnimation_End;

	_uint										m_iAnimCount;
#pragma endregion
#pragma region Background1
	_bool										m_bBackground1_Start;
	_bool										m_bBackground1_RenderSwitch;
	_float										m_fBackground1_DelayingTime;
	_float										m_fBackground1_Control_Alpha;
	_float										m_fBackground1_Control_Discard;

#pragma endregion
#pragma region Background2
	_bool										m_bBackground2_Blink;
	_bool										m_bBackground2_Start;
	_bool										m_bBackground2_RenderSwitch;
	_float										m_fBackground2_DelayingTime;
	_float										m_fBackground2_Control_Alpha1;
	_float										m_fBackground2_Control_Alpha2;
	_float										m_fBackground2_Control_Discard;
	_float										m_fBackground2_Control_BlinkTime;
	_float										m_fBackground2_R1;
	_float										m_fBackground2_G1;
	_float										m_fBackground2_B1;
	_float										m_fBackground2_R2;
	_float										m_fBackground2_G2;
	_float										m_fBackground2_B2;

#pragma endregion
#pragma region Background3
	_bool										m_bBackground3_Start;
	_bool										m_bBackground3_Switch;
	_float										m_fBackground3_DelayingTime;
	_float										m_fBackground3_Control_Alpha;
	_float										m_fBackground3_Control_Discard;

#pragma endregion
#pragma region Noise
#pragma region Noise1
	_bool										m_bNoise_Start1;
	_float										m_fR1;
	_float										m_fG1;
	_float										m_fB1;
	_float										m_fA1;
	_float										m_fAccNoiseTime1;

#pragma endregion

#pragma region Noise2
	_bool										m_bNoise_Start2;
	_float										m_fR2;
	_float										m_fG2;
	_float										m_fB2;
	_float										m_fA2;
	_float										m_fAccNoiseTime2;

#pragma endregion

#pragma region Noise3
	_bool										m_bNoise_Start3;
	_float										m_fR3;
	_float										m_fG3;
	_float										m_fB3;
	_float										m_fA3;
	_float										m_fAccNoiseTime3;

#pragma endregion

#pragma region Noise4
	_bool										m_bNoise_Start4;
	_float										m_fR4;
	_float										m_fG4;
	_float										m_fB4;
	_float										m_fA4;
	_float										m_fAccNoiseTime4;

#pragma endregion

#pragma region Noise5
	_bool										m_bNoise_Start5;
	_float										m_fR5;
	_float										m_fG5;
	_float										m_fB5;
	_float										m_fA5;
	_float										m_fAccNoiseTime5;

#pragma endregion

#pragma region Noise6
	_bool										m_bNoise_Start6;
	_float										m_fR6;
	_float										m_fG6;
	_float										m_fB6;
	_float										m_fA6;
	_float										m_fAccNoiseTime6;

#pragma endregion

#pragma region Noise7
	_bool										m_bNoise_Start7;
	_float										m_fR7;
	_float										m_fG7;
	_float										m_fB7;
	_float										m_fA7;
	_float										m_fAccNoiseTime7;

#pragma endregion

#pragma region Noise8
	_bool										m_bNoise_Start8;
	_float										m_fR8;
	_float										m_fG8;
	_float										m_fB8;
	_float										m_fA8;
	_float										m_fAccNoiseTime8;

#pragma endregion

#pragma region Noise9
	_bool										m_bNoise_Start9;
	_float										m_fR9;
	_float										m_fG9;
	_float										m_fB9;
	_float										m_fA9;
	_float										m_fAccNoiseTime9;

#pragma endregion

#pragma region Noise10
	_bool										m_bNoise_Start10;
	_float										m_fR10;
	_float										m_fG10;
	_float										m_fB10;
	_float										m_fA10;
	_float										m_fAccNoiseTime10;

#pragma endregion

#pragma region Noise11
	_bool										m_bNoise_Start11;
	_float										m_fR11;
	_float										m_fG11;
	_float										m_fB11;
	_float										m_fA11;
	_float										m_fAccNoiseTime11;

#pragma endregion
#pragma region Noise12
	_bool										m_bNoise_Start12;
	_float										m_fR12;
	_float										m_fG12;
	_float										m_fB12;
	_float										m_fA12;
	_float										m_fAccNoiseTime12;

#pragma endregion
#pragma region Noise13
	_bool										m_bNoise_Start13;
	_float										m_fR13;
	_float										m_fG13;
	_float										m_fB13;
	_float										m_fA13;
	_float										m_fAccNoiseTime13;

#pragma endregion
#pragma region Noise14
	_bool										m_bNoise_Start14;
	_float										m_fR14;
	_float										m_fG14;
	_float										m_fB14;
	_float										m_fA14;
	_float										m_fAccNoiseTime14;

#pragma endregion
#pragma region Noise15
	_bool										m_bNoise_Start15;
	_float										m_fR15;
	_float										m_fG15;
	_float										m_fB15;
	_float										m_fA15;
	_float										m_fAccNoiseTime15;

#pragma endregion
#pragma region Noise16
	_bool										m_bNoise_Start16;
	_float										m_fR16;
	_float										m_fG16;
	_float										m_fB16;
	_float										m_fA16;
	_float										m_fAccNoiseTime16;

#pragma endregion
#pragma region Noise17
	_bool										m_bNoise_Start17;
	_float										m_fR17;
	_float										m_fG17;
	_float										m_fB17;
	_float										m_fA17;
	_float										m_fAccNoiseTime17;

#pragma endregion
#pragma region Noise18
	_bool										m_bNoise_Start18;
	_float										m_fR18;
	_float										m_fG18;
	_float										m_fB18;
	_float										m_fA18;
	_float										m_fAccNoiseTime18;

#pragma endregion
#pragma region Noise19
	_bool										m_bNoise_Start19;
	_float										m_fR19;
	_float										m_fG19;
	_float										m_fB19;
	_float										m_fA19;
	_float										m_fAccNoiseTime19;

#pragma endregion
#pragma region Noise20
	_bool										m_bNoise_Start20;
	_float										m_fR20;
	_float										m_fG20;
	_float										m_fB20;
	_float										m_fA20;
	_float										m_fAccNoiseTime20;

#pragma endregion
#pragma region Noise21
	_bool										m_bNoise_Start21;
	_float										m_fR21;
	_float										m_fG21;
	_float										m_fB21;
	_float										m_fA21;
	_float										m_fAccNoiseTime21;

#pragma endregion
#pragma region Noise22
	_bool										m_bNoise_Start22;
	_float										m_fR22;
	_float										m_fG22;
	_float										m_fB22;
	_float										m_fA22;
	_float										m_fAccNoiseTime22;

#pragma endregion
#pragma region Noise23
	_bool										m_bNoise_Start23;
	_float										m_fR23;
	_float										m_fG23;
	_float										m_fB23;
	_float										m_fA23;
	_float										m_fAccNoiseTime23;

#pragma endregion
#pragma region Noise24
	_bool										m_bNoise_Start24;
	_float										m_fR24;
	_float										m_fG24;
	_float										m_fB24;
	_float										m_fA24;
	_float										m_fAccNoiseTime24;

#pragma endregion
#pragma region Noise25
	_bool										m_bNoise_Start25;
	_float										m_fR25;
	_float										m_fG25;
	_float										m_fB25;
	_float										m_fA25;
	_float										m_fAccNoiseTime25;

#pragma endregion

#pragma endregion

private:
	_float3										m_vScale_Background_1;
	_float3										m_vScale_Background_2;
	_float3										m_vScale_Background_3;
	_float3										m_vScale_Noise1;
	_float3										m_vScale_Noise2;
	_float3										m_vScale_Noise3;
	_float3										m_vScale_Noise4;
	_float3										m_vScale_Noise5;
	_float3										m_vScale_Noise6;
	_float3										m_vScale_Noise7;
	_float3										m_vScale_Noise8;
	_float3										m_vScale_Noise9;
	_float3										m_vScale_Noise10;
	_float3										m_vScale_Noise11;
	_float3										m_vScale_Noise12;
	_float3										m_vScale_Noise13;
	_float3										m_vScale_Noise14;
	_float3										m_vScale_Noise15;
	_float3										m_vScale_Noise16;
	_float3										m_vScale_Noise17;
	_float3										m_vScale_Noise18;
	_float3										m_vScale_Noise19;
	_float3										m_vScale_Noise20;
	_float3										m_vScale_Noise21;
	_float3										m_vScale_Noise22;
	_float3										m_vScale_Noise23;
	_float3										m_vScale_Noise24;
	_float3										m_vScale_Noise25;

private:
	_float4										m_vPosition_Background_1;
	_float4										m_vPosition_Background_2;
	_float4										m_vPosition_Background_3;
	_float4										m_vPosition_Noise1;
	_float4										m_vPosition_Noise2;
	_float4										m_vPosition_Noise3;
	_float4										m_vPosition_Noise4;
	_float4										m_vPosition_Noise5;
	_float4										m_vPosition_Noise6;
	_float4										m_vPosition_Noise7;
	_float4										m_vPosition_Noise8;
	_float4										m_vPosition_Noise9;
	_float4										m_vPosition_Noise10;
	_float4										m_vPosition_Noise11;
	_float4										m_vPosition_Noise12;
	_float4										m_vPosition_Noise13;
	_float4										m_vPosition_Noise14;
	_float4										m_vPosition_Noise15;
	_float4										m_vPosition_Noise16;
	_float4										m_vPosition_Noise17;
	_float4										m_vPosition_Noise18;
	_float4										m_vPosition_Noise19;
	_float4										m_vPosition_Noise20;
	_float4										m_vPosition_Noise21;
	_float4										m_vPosition_Noise22;
	_float4										m_vPosition_Noise23;
	_float4										m_vPosition_Noise24;
	_float4										m_vPosition_Noise25;

private:
	wstring										m_strBossName;
	_bool										m_bRenderFont;
	_float										m_fPosition_FontX;
	_float										m_fTime_RenderFont;
	_float										m_fTime_RenderFontAlpha_E_OutLine;

public:
	static shared_ptr<CUI_Indicator_Name_Boss>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END