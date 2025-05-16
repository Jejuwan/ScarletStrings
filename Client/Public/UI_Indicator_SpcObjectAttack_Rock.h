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

class CUI_Indicator_SpcObjectAttack_Rock final : public CGameObject
{
private:
	explicit CUI_Indicator_SpcObjectAttack_Rock(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual  ~CUI_Indicator_SpcObjectAttack_Rock() DEFAULT;


public:
	virtual HRESULT													Initialize_Prototype() override;
	virtual HRESULT													Initialize(any _aDesc = g_aNull)	override;
	virtual void													Tick(_float fTimeDelta) override;
	virtual void													Late_Tick(_float fTimeDelta) override;
	virtual HRESULT													Render() override;

private:
	virtual HRESULT													Ready_Components() override;

private:
	void															Initialize_UI();
	void															Tick_UI(_float fTimeDelta);

private:
	void															System(_float fTimeDelta);

public:
	void															Set_RenderStart();
	void															Set_KeyInput();
	void															Reset();

private:
#pragma region CTransform
	shared_ptr<CTransform>											m_pFillter1;
	shared_ptr<CTransform>											m_pFillter2;
	shared_ptr<CTransform>											m_pDownArrow1;
	shared_ptr<CTransform>											m_pDownArrow2;
	shared_ptr<CTransform>											m_pKey_A;
	shared_ptr<CTransform>											m_pKey_D;
	shared_ptr<CTransform>											m_pHighLightImpulse1;
	shared_ptr<CTransform>											m_pHighLightImpulse2;

#pragma endregion
#pragma region CTexture
	shared_ptr<CTexture>											m_pTexture_Fillter1;
	shared_ptr<CTexture>											m_pTexture_Fillter2;
	shared_ptr<CTexture>											m_pTexture_DownArrow1;
	shared_ptr<CTexture>											m_pTexture_DownArrow2;
	shared_ptr<CTexture>											m_pTexture_Key_A;
	shared_ptr<CTexture>											m_pTexture_Key_D;
	shared_ptr<CTexture>											m_pTexture_HighLightImpulse1;
	shared_ptr<CTexture>											m_pTexture_HighLightImpulse2;

#pragma endregion
#pragma region Others
	shared_ptr<CShader>												m_pShader;
	shared_ptr<CVIBuffer_Rect>										m_pVIBuffer_Rect;
#pragma endregion

private:
#pragma region _bool
	_bool															m_bRenderSwitch;
	_bool															m_bFinish;

	_bool															m_bScaleFinish;
	_bool															m_bScaleFinish_Fillter;
	_bool															m_bScaleFinish_DownArrow;
	_bool															m_bScaleFinish_Key;
	_bool															m_bScaleFinish_HighLightImpulse;

	_bool															m_bStart_WaitingKeyInput;
	_bool															m_bInputKey;
#pragma endregion
#pragma region _int
	_int															m_iPositionChanger;
#pragma endregion
#pragma region _float
	_float															m_fTime;
	_float															m_fHighLightImpulseAlpha;

#pragma endregion
#pragma region _float3
	_float3															m_vScale_Fillter;
	_float3															m_vScale_DownArrow;
	_float3															m_vScale_Key;
	_float3															m_vScale_HighLightImpulse;

#pragma endregion
#pragma region _float4
	_float4															m_vPosition_Fillter1;
	_float4															m_vPosition_Fillter2;
	_float4															m_vPosition_DownArrow1;
	_float4															m_vPosition_DownArrow2;
	_float4															m_vPosition_Key_A;
	_float4															m_vPosition_Key_D;
	_float4															m_vPosition_HighLightImpulse1;
	_float4															m_vPosition_HighLightImpulse2;

#pragma endregion
#pragma region _vector
	_vector															m_Distance = XMVectorSet(0.f, 20.f, 0.f, 0.f);
#pragma endregion

public:
	static shared_ptr<CUI_Indicator_SpcObjectAttack_Rock>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>									Clone(any = g_aNull) override;
};

END