#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
private:
	explicit CRenderer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CRenderer() DEFAULT;

public:
	HRESULT									Initialize(any = g_aNull);

public:
	HRESULT									Add_RenderObject(const RENDER_GROUP, shared_ptr<class ISystem>);
	HRESULT									Add_RenderObject(const UI_LAYER, shared_ptr<class ISystem>);
	
	HRESULT									Add_RenderObject(const CUTSCENE_CHANNEL, shared_ptr<class ISystem>);

	HRESULT									Draw_RenderGroup();

public:
	void									Bind_ModelRenderer(shared_ptr<class CModelRenderer>);

public:
	void									Set_CutsceneMode(_bool _bCutsceneMode) { m_bCutsceneMode = _bCutsceneMode; }
	void									Enable_RadialBlur(_bool _bEnable) { m_bRadialBlur = _bEnable; }
	void									Shadow_Settings(_float fShadowShade, _float fBias, _int2 vGridSize, _float fMinShadowAvg = 0.f, _float fTexelScale = 1.f);

public:
	void									Subscribe_Render_Finish_Callback(const std::function<void()>& fpCaptureSuccess);

public:
	shared_ptr<class CShader>				Get_DeferredShader() const	{ return m_pShader; }

private:
	HRESULT									Ready_Camera();
	HRESULT									Render_Priority();

	HRESULT									Render_ShadowDepth();
	HRESULT									Render_NonBlend();
	HRESULT									Draw_Light();

	HRESULT									Render_Screen_Effect();

	HRESULT									Draw_Shadows();
	HRESULT									Draw_PreProcess();

	HRESULT									Render_SSAO();
	HRESULT									Render_NonLight();
	HRESULT									Render_Blend();
	HRESULT									Render_Decal();
	HRESULT									Render_Bloom();
	HRESULT									Render_Neon();

	HRESULT									Render_Lensflare();

	HRESULT									Draw_PostProcess();

	HRESULT									Render_Cutscene();

	HRESULT									Render_OutLine();

	HRESULT									Render_PostProcess();

	HRESULT									Render_UI();

	HRESULT									Render_UI_3D();

	// Screen Effect
private:
	HRESULT									Execute_RadialBlur();

#ifdef _DEBUG
	HRESULT									Render_Debug();
#endif

private:
	list<shared_ptr<class ISystem>>			m_lstRenderGroup[IDX(RENDER_GROUP::MAX)];
	list<shared_ptr<class ISystem>>			m_lstUIRenderGroup[IDX(UI_LAYER::MAX)];
	vector<shared_ptr<class ISystem>>		m_vecCutsceneRenderGroup;

	shared_ptr<class CRenderTarget_Manager>	m_pRenderTarget_Manager;
	shared_ptr<class CLight_Manager>		m_pLight_Manager;

	shared_ptr<class CShader>				m_pShader;
	shared_ptr<class CVIBuffer_Rect>		m_pVIBuffer;

	shared_ptr<class CShader>				m_pBlurShader;

	_float4x4								m_mQuadWorld;
	_float4x4								m_mQuadView;
	_float4x4								m_mQuadProjection;

	D3D11_VIEWPORT							m_tOriginViewport{};
	D3D11_VIEWPORT							m_tShadowViewport{};

private:
	shared_ptr<class CShader>				m_pLensflareShader;

private:
	_bool									m_bCutsceneMode = { false };
	_bool									m_bRadialBlur = { false };

private:
	std::function<void()> m_fpOnRenderFinish;

private:
	shared_ptr<class CModelRenderer>		m_pModelRenderer;

public:
	static shared_ptr<CRenderer>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>			Clone(any = g_aNull) override;
};

END
