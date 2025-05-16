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

class CUI_Indicator_SpcObjectAttack_Rotation_Beam final : public CGameObject
{
private:
	explicit CUI_Indicator_SpcObjectAttack_Rotation_Beam(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_SpcObjectAttack_Rotation_Beam() DEFAULT;

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
	void															Set_RotateMouse();
	void															Set_Finish();
	void															Reset();

private:
#pragma region CTransform
	shared_ptr<CTransform>											m_pFillter;
	shared_ptr<CTransform>											m_pRotationArrow;
	shared_ptr<CTransform>											m_pBackground;
	shared_ptr<CTransform>											m_pMouse;
	shared_ptr<CTransform>											m_pImpulse;
	shared_ptr<CTransform>											m_pHighLightFillter;
	shared_ptr<CTransform>											m_pHighLightImpulse;
#pragma endregion
#pragma region CTexture
	shared_ptr<CTexture>											m_pTexture_Fillter;
	shared_ptr<CTexture>											m_pTexture_RotationArrow;
	shared_ptr<CTexture>											m_pTexture_Background;
	shared_ptr<CTexture>											m_pTexture_Mouse;
	shared_ptr<CTexture>											m_pTexture_Impulse;
	shared_ptr<CTexture>											m_pTexture_HighLightFillter;
	shared_ptr<CTexture>											m_pTexture_HighLightImpulse;
#pragma endregion
#pragma region Others
	shared_ptr<CShader>												m_pShader;
	shared_ptr<CVIBuffer_Rect>										m_pVIBuffer_Rect;
#pragma endregion

private:
#pragma region _bool
	_bool															m_bRenderSwitch;
	_bool															m_bRotateMouse;
	_bool															m_bFinish_HighLightFillter;
	_bool															m_bFinish_HighLightImpulse;
	_bool															m_bFinish;
#pragma endregion
#pragma region _int

#pragma endregion

#pragma region _float
	_float															m_fTime;
	_float															m_fImpulseAlpha;
	_float															m_fHighLightFillterAlpha;
	_float															m_fHighLightImpulseAlpha;

#pragma endregion
#pragma region _float3
	_float3															m_vScale_Fillter;
	_float3															m_vScale_RotationArrow;
	_float3															m_vScale_Background;
	_float3															m_vScale_Mouse;
	_float3															m_vScale_Impulse;
	_float3															m_vScale_HighLightFillter;
	_float3															m_vScale_HighLightImpulse;
#pragma endregion
#pragma region _float4
	_float4															m_vPosition_Fillter;
	_float4															m_vPosition_RotationArrow;
	_float4															m_vPosition_Background;
	_float4															m_vPosition_Mouse;
	_float4															m_vPosition_Impulse;
	_float4															m_vPosition_HighLightFillter;
	_float4															m_vPosition_HighLightImpulse;
#pragma endregion
#pragma region _vector
	_vector															m_Distance = XMVectorSet(0.f, 20.f, 0.f, 0.f);
#pragma endregion


public:
	static shared_ptr<CUI_Indicator_SpcObjectAttack_Rotation_Beam>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>									Clone(any = g_aNull) override;
};

END