#pragma once
#include "Engine_Define.h"

// ´ëÃæ ¸¸µé¾îÁü

BEGIN(Engine)

class CLight_Manager final : public CSingleton<CLight_Manager>
{
private:
	typedef vector<shared_ptr<class CLight>>												Lights;
	typedef map<shared_ptr<class CLight>, shared_ptr<class CGameObject>, std::owner_less<>>	ShadowObjects;
	typedef map<shared_ptr<class CGameObject>, shared_ptr<class CLight>, std::owner_less<>>	Shadows;

private:
	explicit CLight_Manager() DEFAULT;
	virtual ~CLight_Manager() DEFAULT;

public:
	HRESULT																				Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const GRAPHICDESC, const SCENE);
	void																				Tick(_float fTimeDelta);

public:
#ifdef _DEBUG
	ComPtr<ID3D11ShaderResourceView>													Get_Shadow(shared_ptr<class CLight> pLight)	{ return m_mapShadowShaderResourceViews[pLight]; }
#endif

public:
	shared_ptr<class CLight>															Add_Light(const SCENE, LIGHTDESC, shared_ptr<class CTransform>, shared_ptr<class CGameObject> = nullptr);
	HRESULT																				Add_Light(const SCENE, shared_ptr<class CLight>);
	HRESULT																				Erase_Light(shared_ptr<class CLight>);
	HRESULT																				Clear_Lights(const SCENE);

	HRESULT																				Bind_Lights(shared_ptr<class CShader>, shared_ptr<class CVIBuffer_Rect>);
	HRESULT																				Bind_ShadowMatrices(shared_ptr<class CShader>);

	HRESULT																				Bind_LightDirection(SCENE, shared_ptr<class CShader>);

	void																				Iterate_Shadows(function<void(shared_ptr<class CLight>)>);
	void																				Copy_RenderedShadows(shared_ptr<class CLight>, ComPtr<ID3D11ShaderResourceView>);

public:
	void																				Enable_SSAO(_bool bEnable) { m_bEnableSSAO = bEnable; }
	_bool																				Enable_SSAO() { return m_bEnableSSAO; }

public:
	void																				Enable_Lensflare(_bool bEnable) { m_bEnableLensflare = bEnable; }
	_bool																				Enable_Lensflare() { return m_bEnableLensflare; }

	_float3																				Get_SunPosition() { return m_vSunPosition; }
	void																				Set_SunPosition(_float3 vPosition) { m_vSunPosition = vPosition; }

	_float3																				Get_SunColor() { return m_vSunColor; }
	void																				Set_SunColor(_float3 vColor) { m_vSunColor = vColor; }

	_float																				Get_LensflareStrength() { return m_fLensflareStrength; }
	void																				Set_LensflareStrength(_float fStrength) { m_fLensflareStrength = fStrength; }

	HRESULT																				Render_Lensflare(shared_ptr<class CShader> pShader, shared_ptr<class CVIBuffer_Rect> pVIBuffer, ComPtr<ID3D11ShaderResourceView> pDepthSRV);

public:
	void																				Set_Reflection(shared_ptr<class CTexture> pReflection);
	HRESULT																				Bind_Reflection(shared_ptr<class CShader> pShader);

private:
	unique_ptr<Lights[]>																m_arrLights; 
	_uint 																				m_iLightCount	= 0;

	D3D11_TEXTURE2D_DESC																m_tTexture2DDesc{};

	map<shared_ptr<class CLight>, ComPtr<ID3D11ShaderResourceView>, std::owner_less<>>	m_mapShadowShaderResourceViews;
	vector<_float4x4>																	m_vecShadowViewMatrices;
	vector<_float4x4>																	m_vecShadowProjectionMatrices;

	vector<ID3D11ShaderResourceView*>													m_vecShadowRawShaderResourceViews;

	list<shared_ptr<class CLight>>														m_lstErase;

	SCENE																				m_eSceneMax		= static_cast<SCENE>(0);

private:	// SSAO
	_bool																				m_bEnableSSAO = { false };

private:	// Lensflare
	_bool																				m_bEnableLensflare = { false };
	shared_ptr<class CTexture>															m_pLensNoiseTexture;
	_float																				m_fLensflareAccTime = { 0.f };

	_float3																				m_vSunPosition = { 2000.f, 10.f, 3000.f };
	_float3																				m_vSunColor = { 1.4f, 1.2f, 1.0f };
	_float																				m_fLensflareStrength = { 1.f };

private:
	shared_ptr<CTexture>																m_pReflection;

private:
	ComPtr<ID3D11Device>																m_pDevice;
	ComPtr<ID3D11DeviceContext>															m_pContext;

	friend CSingleton<CLight_Manager>;
};

END
