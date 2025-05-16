#include "EnginePCH.h"
#include "Renderer.h"
#include "GameInstance.h"
#include "GraphicDevice.h"
#include "RenderTarget_Manager.h"
#include "Light_Manager.h"
#include "Component_Manager.h"
#include "Scene_Manager.h"
#include "PipeLine.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "Light.h"
#include "GameObject.h"
#include "Transform.h"
#include "System.h"
#include "ImageProcess_Manager.h"
#include "Camera.h"
#include "Dialogue_Manager.h"
#include "Transform.h"

CRenderer::CRenderer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::RENDERER)
	, m_pRenderTarget_Manager(CRenderTarget_Manager::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
{
}

HRESULT CRenderer::Initialize(any)
{
	_uint			iNumViewPorts(1);

	m_pContext->RSGetViewports(&iNumViewPorts, &m_tOriginViewport);
	m_tShadowViewport = m_tOriginViewport;
	m_tShadowViewport.Width *= g_iShadowScale;
	m_tShadowViewport.Height *= g_iShadowScale;

	m_mQuadWorld = XMMatrixScaling(m_tOriginViewport.Width, m_tOriginViewport.Height, 1.f);
	m_mQuadView = XMMatrixIdentity();
	m_mQuadProjection = XMMatrixOrthographicLH(m_tOriginViewport.Width, m_tOriginViewport.Height, 0.f, 1.f);

	_uint2	vResolution			= _uint2(static_cast<_uint>(m_tOriginViewport.Width), static_cast<_uint>(m_tOriginViewport.Height));
	_uint2	vShadowResolution	= _uint2(vResolution.x * g_iShadowScale, vResolution.y * g_iShadowScale);

#pragma region Renger Targets
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_BLUR_BUFFER_A,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_BLUR_BUFFER");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_BLUR_BUFFER_B,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_BLUR_BUFFER");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_SHADOWDEPTH,
		vShadowResolution, DXGI_FORMAT_R32G32B32A32_FLOAT, _color(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_SHADOWDEPTH");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MATERIAL_DIFFUSE,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MATERIAL_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MATERIAL_AMBIENT,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MATERIAL_AMBIENT");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MATERIAL_SPECULAR,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MATERIAL_SPECULAR");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MATERIAL_EMISSIVE,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MATERIAL_EMISSIVE");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_NORMAL,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_NORMAL");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_DEPTH,
		vResolution, DXGI_FORMAT_R32G32B32A32_FLOAT, _color(0.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_DEPTH");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_PREMASK,
		vResolution, DXGI_FORMAT_R16G16B16A16_UNORM, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_PREMASK");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_OUTLINE,
		vResolution, DXGI_FORMAT_R32G32B32A32_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_OUTLINE");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_LIGHT_SSAO,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_LIGHT_SSAO");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_LIGHT_DIFFUSE,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_LIGHT_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_LIGHT_AMBIENT,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_LIGHT_AMBIENT");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_LIGHT_SPECULAR,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_LIGHT_SPECULAR");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_BLOOM,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_BLOOM");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_BLOOMSTRENGTH,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_BLOOMSTRENGTH");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_NEON,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_NEON");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_NEON_COMBINE,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_NEON_COMBINE");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_SHADOWPROCESS,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_SHADOWPROCESS");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_PREPROCESS,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_PREPROCESS");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_PROCESSDATA,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_PROCESSDATA");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MASK,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MASK");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_PREPROCESS_COMBINE,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_PREPROCESS_COMBINE");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_POSTPROCESS,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_POSTPROCESS");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_UI_3D,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_UI_3D");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_UI_3D_CHARACTER,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_UI_3D_CHARACTER");
	}
	
#pragma region For Cutscene
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_CUTSCENE_MAIN,
		vResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, _color(0.01f, 0.01f, 0.01f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_CUTSCENE_MAIN");
	}
#pragma endregion

#pragma endregion
#pragma region Depth Stencils

	if (FAILED(m_pRenderTarget_Manager->Add_DepthStencil(DEPTHSTENCIL_SHADOWDEPTH, vShadowResolution)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_DepthStencil: DEPTHSTENCIL_SHADOWDEPTH");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_DepthStencil(DEPTHSTENCIL_CUTSCENE, vResolution)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_DepthStencil: DEPTHSTENCIL_CUTSCENE");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_DepthStencil(DEPTHSTENCIL_UI_3D, vResolution)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_DepthStencil: DEPTHSTENCIL_UI_3D");
	}

#pragma endregion
#pragma region Multi Render Targets
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_BLUR_BUFFER_A, RENDERTARGET_BLUR_BUFFER_A)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_BLUR_BUFFER");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_BLUR_BUFFER_B, RENDERTARGET_BLUR_BUFFER_B)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_BLUR_BUFFER");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_SHADOWDEPTH, RENDERTARGET_SHADOWDEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_DepthStencil(MULTIRENDERTARGET_SHADOWDEPTH, DEPTHSTENCIL_SHADOWDEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_DepthStencil: MULTIRENDERTARGET_NONBLEND");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_MATERIAL_DIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_MATERIAL_AMBIENT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_MATERIAL_SPECULAR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_MATERIAL_EMISSIVE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_NORMAL)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_DEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_PREMASK)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_OUTLINE, RENDERTARGET_OUTLINE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_OUTLINE");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_BLOOM, RENDERTARGET_BLOOM)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_BLOOM");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_BLOOM, RENDERTARGET_BLOOMSTRENGTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_BLOOM");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NEON, RENDERTARGET_NEON)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NEON");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_SSAO, RENDERTARGET_LIGHT_SSAO)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_SSAO");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_LIGHT, RENDERTARGET_LIGHT_DIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_LIGHT");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_LIGHT, RENDERTARGET_LIGHT_AMBIENT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_LIGHT");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_LIGHT, RENDERTARGET_LIGHT_SPECULAR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_LIGHT");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_SHADOWPROCESS, RENDERTARGET_SHADOWPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_SHADOWPROCESS");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_PREPROCESS, RENDERTARGET_PREPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_PREPROCESS");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_PREPROCESS, RENDERTARGET_PROCESSDATA)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_PREPROCESS");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_PREPROCESS, RENDERTARGET_MASK)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_PREPROCESS");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_PREPROCESS_COMBINE, RENDERTARGET_PREPROCESS_COMBINE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_PREPROCESS_COMBINE");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_POSTPROCESS, RENDERTARGET_POSTPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_POSTPROCESS");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_UI_3D, RENDERTARGET_UI_3D)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_UI_3D");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_DepthStencil(MULTIRENDERTARGET_UI_3D, DEPTHSTENCIL_UI_3D)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_DepthStencil: MULTIRENDERTARGET_UI_3D");
	}
#pragma region For Cutscene
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_CUTSCENE_MAIN, RENDERTARGET_CUTSCENE_MAIN)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_CUTSCENE_MAIN");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_DepthStencil(MULTIRENDERTARGET_CUTSCENE_MAIN, DEPTHSTENCIL_CUTSCENE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_DepthStencil: MULTIRENDERTARGET_CUTSCENE_MAIN");
	}
#pragma endregion

#pragma endregion

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex_Deferred.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Compile Shader");
	}

	CComponent_Manager::Get_Instance()->Add_Prototype(CScene_Manager::Get_Instance()->Static_Scene(), PROTOTYPE_COMPONENT_SHADER_DEFERRED, m_pShader);

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Create VIBuffer");
	}

	m_pBlurShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex_Blur.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement);
	if (nullptr == m_pBlurShader)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Compile Blur Shader");
	}

	m_pLensflareShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex_Lensflare.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement);
	if (nullptr == m_pLensflareShader)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Compile Lensflare Shader");
	}

#pragma region

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATQUADWORLD, m_mQuadWorld)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADWORLD");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATQUADVIEW, m_mQuadView)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADVIEW");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATQUADPROJ, m_mQuadProjection)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADPROJ");
	}

	if (FAILED(m_pBlurShader->Bind_Matrix(SHADER_MATQUADWORLD, m_mQuadWorld)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADWORLD");
	}
	if (FAILED(m_pBlurShader->Bind_Matrix(SHADER_MATQUADVIEW, m_mQuadView)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADVIEW");
	}
	if (FAILED(m_pBlurShader->Bind_Matrix(SHADER_MATQUADPROJ, m_mQuadProjection)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADPROJ");
	}

	if (FAILED(m_pLensflareShader->Bind_Matrix(SHADER_MATQUADWORLD, m_mQuadWorld)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADWORLD");
	}
	if (FAILED(m_pLensflareShader->Bind_Matrix(SHADER_MATQUADVIEW, m_mQuadView)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADVIEW");
	}
	if (FAILED(m_pLensflareShader->Bind_Matrix(SHADER_MATQUADPROJ, m_mQuadProjection)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADPROJ");
	}

#pragma endregion

	auto pImageProcessor = CImageProcess_Manager::Get_Instance();

	auto pBloomTexture2D = m_pRenderTarget_Manager->Get_Texture2D(RENDERTARGET_BLOOM);
	auto pBloomRenderTargetView = m_pRenderTarget_Manager->Get_RenderTargetView(RENDERTARGET_BLOOM);
	pImageProcessor->Initialize(IMAGE_PROCESS::PROCESS_BLOOM, m_pDevice, m_pContext, pBloomTexture2D, pBloomRenderTargetView
		, vResolution.x, vResolution.y, 4, 0.5f, 1.f, 1.f);

	auto pBloomStrengthTexture2D = m_pRenderTarget_Manager->Get_Texture2D(RENDERTARGET_BLOOMSTRENGTH);
	auto pBloomStrengthRenderTargetView = m_pRenderTarget_Manager->Get_RenderTargetView(RENDERTARGET_BLOOMSTRENGTH);
	pImageProcessor->Initialize(IMAGE_PROCESS::PROCESS_BLOOM_STRENGTH, m_pDevice, m_pContext, pBloomStrengthTexture2D, pBloomStrengthRenderTargetView
		, vResolution.x, vResolution.y, 4, 0.5f, 1.f, 1.f);

	auto pNeonTexture2D = m_pRenderTarget_Manager->Get_Texture2D(RENDERTARGET_NEON);
	auto pNeonRenderTargetView = m_pRenderTarget_Manager->Get_RenderTargetView(RENDERTARGET_NEON);
	pImageProcessor->Initialize(IMAGE_PROCESS::PROCESS_NEON, m_pDevice, m_pContext, pNeonTexture2D, pNeonRenderTargetView
		, vResolution.x, vResolution.y, 4, 0.1f, 1.7f, 1.1f);

	m_vecCutsceneRenderGroup.resize(IDX(CUTSCENE_CHANNEL::MAX));

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(const RENDER_GROUP _eRenderGroup, shared_ptr<ISystem> _pGameObject)
{
	if (nullptr == _pGameObject)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Add_RenderObject", "Nullptr Exception: _pGameObject");
	}

	m_lstRenderGroup[IDX(_eRenderGroup)].emplace_back(_pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(const UI_LAYER _eUILayer, shared_ptr<class ISystem> _pGameObject)
{
	if (nullptr == _pGameObject)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Add_RenderObject(UI)", "Nullptr Exception: _pGameObject");
	}

	m_lstUIRenderGroup[IDX(_eUILayer)].emplace_back(_pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(const CUTSCENE_CHANNEL eCutsceneChannel, shared_ptr<class ISystem> _pGameObject)
{
	if (nullptr == _pGameObject)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Add_RenderObject(CutsceneChannel)", "Nullptr Exception: _pGameObject");
	}

	m_vecCutsceneRenderGroup[IDX(eCutsceneChannel)] = _pGameObject;

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pShader->Bind_Float(SHADER_TIME, CGameInstance::Get_Instance()->Get_ActivatedTime())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_RenderGroup", "Failed to Bind_Float: SHADER_TIME");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_RenderGroup", "Failed to Bind_Matrix: SHADER_MATVIEW");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_RenderGroup", "Failed to Bind_Matrix: SHADER_MATPROJ");
	}

	//if (FAILED(CLight_Manager::Get_Instance()->Bind_Reflection(m_pShader)))
	//{
	//	MSG_RETURN(E_FAIL, "CRenderer::Draw_RenderGroup", "Failed to Bind_Reflection");
	//}

	if (FAILED(Ready_Camera()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Ready_Camera");
	}
	if (FAILED(Render_Priority()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Priority");
	}

	if (FAILED(Render_ShadowDepth()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_ShadowDepth");
	}
	if (FAILED(Render_NonBlend()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_NonBlend");
	}

	if (FAILED(Render_SSAO()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_SSAO");
	}

	if (FAILED(Draw_Light()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Draw_Light");
	}

	if (FAILED(Draw_Shadows()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Draw_Shadows");
	}
	if (FAILED(Draw_PreProcess()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Draw_PreProcess");
	}
	
	if (FAILED(Render_Lensflare()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Lensflare");
	}
	
	if (FAILED(Render_NonLight()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_NonLight");
	}
	if (FAILED(Render_Blend()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Blend");
	}
	if (FAILED(Render_Decal()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Decal");
	}

	if (FAILED(Render_Bloom()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Bloom");
	}

	if (FAILED(Render_Neon()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Neon");
	}

	if (FAILED(Render_Screen_Effect()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Screen_Effect");
	}

	if (FAILED(Draw_PostProcess()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Draw_PostProcess");
	}

	if (FAILED(Render_OutLine()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_OutLine");
	}

	if (FAILED(Render_PostProcess()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_PostProcess");
	}

	if (m_bCutsceneMode)
	{
		if (FAILED(Render_Cutscene()))
		{
			hr = E_FAIL;
			MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Cutscene");
		}
	}

	if (m_bRadialBlur)
	{
		if (FAILED(Execute_RadialBlur()))
		{
			hr = E_FAIL;
			MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Execute_RadialBlur");
		}
	}

	if (FAILED(Render_UI_3D()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_UI_3D");
	}

	if (FAILED(Render_UI()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_UI");
	}

	if (FAILED(CDialogue_Manager::Get_Instance()->Render()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Dialogue_Render");
	}

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Debug");
	}
#endif

	return hr;
}

void CRenderer::Bind_ModelRenderer(shared_ptr<class CModelRenderer> _pModelRenderer)
{
	m_pModelRenderer = _pModelRenderer;
}

void CRenderer::Shadow_Settings(_float _fShadowShade, _float _fBias, _int2 _vGridSize, _float _fMinShadowAvg, _float _fTexelScale)
{
	if (FAILED(m_pShader->Bind_Float("fShadowShade", _fShadowShade)))
	{
		MSG_RETURN(, "CRenderer::Shadow_Settings", "Failed to Bind_Float: Shadow Shade");
	}
	if (FAILED(m_pShader->Bind_Float("fBias", _fBias)))
	{
		MSG_RETURN(, "CRenderer::Shadow_Settings", "Failed to Bind_Float: Bias");
	}
	if (FAILED(m_pShader->Bind_RawValue("vGridSize", &_vGridSize, sizeof(_int2))))
	{
		MSG_RETURN(, "CRenderer::Shadow_Settings", "Failed to Bind_Float: Grid Size");
	}
	if (FAILED(m_pShader->Bind_Float("fMinShadowAvg", _fMinShadowAvg)))
	{
		MSG_RETURN(, "CRenderer::Shadow_Settings", "Failed to Bind_Float: Min Shadow Avg");
	}
	if (FAILED(m_pShader->Bind_Float("fTexelScale", _fTexelScale)))
	{
		MSG_RETURN(, "CRenderer::Shadow_Settings", "Failed to Bind_Float: Texel Scale");
	}
}

void CRenderer::Subscribe_Render_Finish_Callback(const std::function<void()>& fpCaptureSuccess)
{
	m_fpOnRenderFinish = fpCaptureSuccess;
}

#pragma region RenderGroup
HRESULT CRenderer::Ready_Camera()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::CAMERA)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::CAMERA)].clear();

	return hr;
}

HRESULT CRenderer::Render_Priority()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::PRIORITY)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::PRIORITY)].clear();

	return hr;
}

HRESULT CRenderer::Render_ShadowDepth()
{
	HRESULT hr = S_OK;

	m_pContext->RSSetViewports(1, &m_tOriginViewport);

	m_pLight_Manager->Iterate_Shadows(
		[&](shared_ptr<CLight> _pShadow)
		{
			if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_SHADOWDEPTH)))
			{
				MSG_RETURN(E_FAIL, "CRenderer::Render_ShadowDepth", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_SHADOWDEPTH");
			}

		//	if (FAILED(_pShadow->Bind_ShadowMatrix(m_pShader)))
		//	{
		//		MSG_RETURN(E_FAIL, "CRenderer::Render_ShadowDepth", "Failed to CLight: Bind_ShadowMatrix");
		//	}

		//	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::SHADOW)])
		//	{
		//		if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(pSystem))
		//		{
		//	//		if (FAILED(pObject->Render_ShadowDepth(_pShadow)))
		//	//	//	if (FAILED(pObject->Render()))
		//	//		{
		//	//			MSG_CONTINUE("CRenderer::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
		//	//		}
		//
		//			if (FAILED(pObject->Render_ShadowDepth(_pShadow)))
		//			{
		//				MSG_CONTINUE("CRenderer::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
		//			}
		//		}
		//	}

			if (m_pModelRenderer)
			{
				m_pModelRenderer->Render(RENDER_GROUP::SHADOW, _pShadow);
			}

			_pShadow->Render_ShadowDepth(static_pointer_cast<CRenderer>(shared_from_this()));

			if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
			{
				MSG_RETURN(E_FAIL, "CRenderer::Render_ShadowDepth", "Failed to End_MultiRenderTarget");
			}

			m_pLight_Manager->Copy_RenderedShadows(_pShadow, m_pRenderTarget_Manager->Get_ShaderResourceView(RENDERTARGET_SHADOWDEPTH));
		}
	);

	m_pContext->RSSetViewports(1, &m_tOriginViewport);

	m_lstRenderGroup[IDX(RENDER_GROUP::SHADOW)].clear();

	return hr;
}

HRESULT CRenderer::Render_NonBlend()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_NONBLEND)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_NonBlend", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_NONBLEND");
	}

	if (m_pModelRenderer)
	{
		m_pModelRenderer->Render(RENDER_GROUP::NONBLEND);
	}

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::NONBLEND)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::NONBLEND)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_NonBlend", "Failed to End_MultiRenderTarget");
	}

	return hr;
}

HRESULT CRenderer::Render_OutLine()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_OUTLINE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_OUTLINE");
	}

	if (m_pModelRenderer)
	{
		m_pModelRenderer->Render(RENDER_GROUP::OUTLINE);
	}

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::OUTLINE)])
	{
		if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(pSystem))
		{
			if (FAILED(pObject->Render_OutLine()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::OUTLINE)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to End_MultiRenderTarget");
	}

	//if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_OUTLINE_SCALING)))
	//{
	//	MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_OUTLINE");
	//}
	//m_pContext->RSSetViewports(1, &m_tOutLineViewport);

	//if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_OUTLINE, m_pShader, SHADER_TEXTARGET_OUTLINE)))
	//{
	//	MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_OUTLINE");
	//}

	// Upscaling
	//if (FAILED(m_pShader->BeginPass(5)))
	//{
	//	MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to BeginPass");
	//}
	//if (FAILED(m_pVIBuffer->Render()))
	//{
	//	MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to Render");
	//}

	//if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	//{
	//	MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to End_MultiRenderTarget");
	//}
	//m_pContext->RSSetViewports(1, &m_tOriginViewport);


	// Combine OutLine
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_OUTLINE, m_pShader, SHADER_TEXTARGET_OUTLINE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_OUTLINE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_DEPTH, m_pShader, SHADER_TEXTARGET_DEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_OUTLINE");
	}

	if (FAILED(m_pShader->BeginPass(4)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to Render");
	}

	return hr;
}

HRESULT CRenderer::Render_PostProcess()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::POSTPROCESS)])
	{
		if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(pSystem))
		{
			if (FAILED(pObject->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::POSTPROCESS)].clear();

	return hr;
}

HRESULT CRenderer::Draw_Light()
{
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_LIGHT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_LIGHT");
	}

	if (CPipeLine::Get_Instance()->Get_Camera())
	{
		if (FAILED(m_pShader->Bind_Vector(SHADER_CAMPOS, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION))))
		{
			MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Vector: SHADER_CAMPOS");
		}
		if (FAILED(m_pShader->Bind_Float(SHADER_CAMFAR, CPipeLine::Get_Instance()->Get_Camera()->Get_Desc().fFar)))
		{
			MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Float: SHADER_CAMFAR");
		}
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEWINV, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).inverse())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Matrix: SHADER_MATVIEWINV");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJINV, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION).inverse())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Matrix: SHADER_MATPROJINV");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_NORMAL, m_pShader, SHADER_TEXTARGET_NORMAL)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_NORMAL");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_DEPTH, m_pShader, SHADER_TEXTARGET_DEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_DEPTH");
	}

	if (FAILED(m_pLight_Manager->Bind_Lights(m_pShader, m_pVIBuffer)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Lights");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to End_MultiRenderTarget");
	}

	return S_OK;
}

HRESULT CRenderer::Draw_Shadows()
{
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_SHADOWPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Shadows", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_SHADOWPROCESS");
	}

	if (FAILED(m_pLight_Manager->Bind_ShadowMatrices(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Shadows", "Failed to Bind_ShadowMatrices");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_DEPTH, m_pShader, SHADER_TEXTARGET_DEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_DEPTH");
	}
//	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_SHADOWDEPTH, m_pShader, SHADER_TEXTARGET_SHADOWDEPTH)))
//	{
//		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_SHADOWDEPTH");
//	}

	if (FAILED(m_pShader->BeginPass(7)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Shadows", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Shadows", "Failed to Render");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Shadows", "Failed to End_MultiRenderTarget");
	}

	return S_OK;
}

HRESULT CRenderer::Draw_PreProcess()
{
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_PREPROCESS, 0)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_PREPROCESS");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEWINV, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).inverse())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_Matrix: SHADER_MATVIEWINV");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJINV, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION).inverse())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_Matrix: SHADER_MATPROJINV");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_PREMASK, m_pShader, SHADER_TEXTARGET_PREMASK)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_PREMASK");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_SHADOWPROCESS, m_pShader, SHADER_TEXTARGET_SHADOWPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_SHADOWPROCESS");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MATERIAL_DIFFUSE, m_pShader, SHADER_TEXTARGET_MTRL_DIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MATERIAL_AMBIENT, m_pShader, SHADER_TEXTARGET_MTRL_AMBIENT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_AMBIENT");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MATERIAL_SPECULAR, m_pShader, SHADER_TEXTARGET_MTRL_SPECULAR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_SPECULAR");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MATERIAL_EMISSIVE, m_pShader, SHADER_TEXTARGET_MTRL_EMISSIVE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_EMISSIVE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_LIGHT_DIFFUSE, m_pShader, SHADER_TEXTARGET_LIGHTDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_LIGHT_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_LIGHT_AMBIENT, m_pShader, SHADER_TEXTARGET_LIGHTAMBIENT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_LIGHT_AMBIENT");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_LIGHT_SPECULAR, m_pShader, SHADER_TEXTARGET_LIGHTSPECULAR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_LIGHT_SPECULAR");
	}

	if (FAILED(m_pShader->BeginPass(2)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Render");
	}

	return S_OK;
}

HRESULT CRenderer::Render_SSAO()
{
	if (false == m_pLight_Manager->Enable_SSAO())
	{
		return S_OK;
	}

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_SSAO)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_LIGHT_SSAO");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_NORMAL, m_pBlurShader, SHADER_TEXTARGET_NORMAL)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_NORMAL");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_DEPTH, m_pBlurShader, SHADER_TEXTARGET_DEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_DEPTH");
	}
	
	if (FAILED(m_pBlurShader->BeginPass(2)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Render");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Shadows", "Failed to End_MultiRenderTarget");
	}

	// Blur Y
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_BLUR_BUFFER_A)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_LIGHT_SSAO");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_LIGHT_SSAO, m_pBlurShader, SHADER_TEXTARGET_BLUR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_LIGHT_SSAO");
	}

	if (FAILED(m_pBlurShader->BeginPass(1)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Render");
	}
	
	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Shadows", "Failed to End_MultiRenderTarget");
	}

	// Blur X
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_SSAO)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_LIGHT_SSAO");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_BLUR_BUFFER_A, m_pBlurShader, SHADER_TEXTARGET_BLUR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_LIGHT_SSAO");
	}

	if (FAILED(m_pBlurShader->BeginPass(0)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Render");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Shadows", "Failed to End_MultiRenderTarget");
	}

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::NONLIGHT)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::NONLIGHT)].clear();

	return hr;
}

HRESULT CRenderer::Render_Blend()
{
	HRESULT hr = S_OK;

	// Blend Instacing Object
	if (m_pModelRenderer)
	{
		m_pModelRenderer->Render(RENDER_GROUP::BLEND);
	}

	// Blend Object
	{
		_float3 vPosition = _float3(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION));

		m_lstRenderGroup[IDX(RENDER_GROUP::BLEND)].sort(
			[vPosition](shared_ptr<ISystem> _pSystemA, shared_ptr<ISystem> _pSystemB)->_bool
			{
				_float3 vPositionA = static_pointer_cast<CTransform>(static_pointer_cast<CGameObject>(_pSystemA)->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				_float3 vPositionB = static_pointer_cast<CTransform>(static_pointer_cast<CGameObject>(_pSystemB)->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);

				return (vPosition - vPositionA).length() > (vPosition - vPositionB).length();
			}
		);

		for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::BLEND)])
		{
			if (nullptr != pSystem)
			{
				if (FAILED(pSystem->Render()))
				{
					hr = E_FAIL;
				}
			}
		}
		m_lstRenderGroup[IDX(RENDER_GROUP::BLEND)].clear();
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Blend", "Failed to End_MultiRenderTarget");
	}

	return hr;
}

HRESULT CRenderer::Render_Decal()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_PREPROCESS, 128U, { 0, 1, 2 })))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Decal", "Failed to Begin_MultiRenderTaget");
	}

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::DECAL)])
	{
		if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(pSystem))
		{
			if (pObject)
			{
				if (FAILED(pObject->Render()))
				{
					hr = E_FAIL;
				}
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::DECAL)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Decal", "Failed to End_MultiRenderTarget");
	}

	return hr;
}

HRESULT CRenderer::Render_Bloom()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_BLOOM)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Bloom", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_BLOOM");
	}

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::BLOOM)])
	{
		if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(pSystem))
		{
			if (pObject)
			{
				if (FAILED(pObject->Render_Bloom()))
				{
					hr = E_FAIL;
				}
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::BLOOM)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Bloom", "Failed to End_MultiRenderTarget");
	}

	CImageProcess_Manager::Get_Instance()->Excute_ImageProcess(IMAGE_PROCESS::PROCESS_BLOOM_STRENGTH, 8);
	CImageProcess_Manager::Get_Instance()->Excute_ImageProcess(IMAGE_PROCESS::PROCESS_BLOOM, 4,
		[&](shared_ptr<CShader> _pPostProcess)->HRESULT
		{
			if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_BLOOMSTRENGTH, _pPostProcess, "g_texBloomStrength")))
			{
				MSG_RETURN(E_FAIL, "CRenderer::Render_Bloom", "Failed to Bind_RenderTarget: RENDERTARGET_BLOOM");
			}

			return S_OK;
		}
	);

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_PREPROCESS_COMBINE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Bloom", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_BLOOM");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_PREPROCESS, m_pShader, SHADER_TEXTARGET_PREPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_OUTLINE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_BLOOM, m_pShader, SHADER_TEXTARGET_BLOOM)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_OUTLINE");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_BLOOMSTRENGTH, m_pShader, "g_texBloomStrengthTarget")))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Bloom", "Failed to Bind_RenderTarget: RENDERTARGET_BLOOM");
	}

	if (FAILED(m_pShader->BeginPass(5)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to Render");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Bloom", "Failed to End_MultiRenderTarget");
	}

	m_pRenderTarget_Manager->Copy_RenderTarget(RENDERTARGET_PREPROCESS, RENDERTARGET_PREPROCESS_COMBINE);

	return hr;
}

HRESULT CRenderer::Render_Neon()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_NEON)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Neon", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_NEON");
	}

	if (m_pModelRenderer)
	{
		m_pModelRenderer->Render(RENDER_GROUP::NEON);
	}

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::NEON)])
	{
		if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(pSystem))
		{
			if (pObject)
			{
				if (FAILED(pObject->Render_Neon()))
				{
					hr = E_FAIL;
				}
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::NEON)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Neon", "Failed to End_MultiRenderTarget");
	}

	CImageProcess_Manager::Get_Instance()->Excute_ImageProcess(IMAGE_PROCESS::PROCESS_NEON);

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_PREPROCESS_COMBINE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Bloom", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_BLOOM");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_PREPROCESS, m_pShader, SHADER_TEXTARGET_PREPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_OUTLINE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_NEON, m_pShader, SHADER_TEXTARGET_BLOOM)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_OUTLINE");
	}
	
	if (FAILED(m_pShader->BeginPass(9)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_OutLine", "Failed to Render");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Bloom", "Failed to End_MultiRenderTarget");
	}

	m_pRenderTarget_Manager->Copy_RenderTarget(RENDERTARGET_PREPROCESS, RENDERTARGET_PREPROCESS_COMBINE);

	return hr;
}

HRESULT CRenderer::Render_Lensflare()
{
	return CLight_Manager::Get_Instance()->Render_Lensflare(m_pLensflareShader, m_pVIBuffer,
		m_pRenderTarget_Manager->Get_ShaderResourceView(RENDERTARGET_DEPTH));
}

HRESULT CRenderer::Render_Screen_Effect()
{
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_PREPROCESS, 128, {0, 1, 2})))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_PREPROCESS");
	}

	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::SCREEN_EFFECT)])
	{
		if (nullptr != pSystem)
		{
			if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(pSystem))
			{
				if (FAILED(pObject->Render_Screen_Effect()))
				{
					hr = E_FAIL;
				}
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::SCREEN_EFFECT)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to End_MultiRenderTarget");
	}

	return hr;
}

HRESULT CRenderer::Draw_PostProcess()
{
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_POSTPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_PREPROCESS");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_PREPROCESS, m_pShader, SHADER_TEXTARGET_PREPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Bind_RenderTarget: RENDERTARGET_PREPROCESS");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_PROCESSDATA, m_pShader, SHADER_TEXTARGET_PROCESSDATA)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Bind_RenderTarget: RENDERTARGET_PROCESSDATA");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MASK, m_pShader, SHADER_TEXTARGET_MASK)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MASK");
	}

	if (FAILED(m_pShader->BeginPass(3)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Render");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget(RENDERTARGET_POSTPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Blend", "Failed to End_MultiRenderTarget");
	}
	
	// Bloom & Tonemapping
	auto pImageProcess = CImageProcess_Manager::Get_Instance();
	pImageProcess->Excute_ImageProcess(IMAGE_PROCESS::PROCESS_TONEMAPPING);

	// Backbuffer Capture
	if (m_fpOnRenderFinish)
	{
		m_fpOnRenderFinish();
	}

	return S_OK;
}

HRESULT CRenderer::Render_Cutscene()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_CUTSCENE_MAIN)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Cutscene", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_CUTSCENE_MAIN");
	}

	for (_uint i = 0; i < IDX(CUTSCENE_CHANNEL::MAX); ++i)
	{
		if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(m_vecCutsceneRenderGroup[i]))
		{
			if (false == pObject->Get_Enable())
			{
				continue;
			}

			if (FAILED(pObject->Render()))
			{
				hr = E_FAIL;
			}

			m_vecCutsceneRenderGroup[i] = nullptr;
		}
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget(RENDERTARGET_CUTSCENE_MAIN))) 
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Blend", "Failed to End_MultiRenderTarget");
	}

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	HRESULT hr = S_OK;

	for (_uint i = 0; i < _uint(UI_LAYER::MAX); ++i)
	{
		for (auto& pSystem : m_lstUIRenderGroup[i])
		{
			if (nullptr != pSystem)
			{
				if (FAILED(pSystem->Render()))
				{
					hr = E_FAIL;
				}
			}
		}
		m_lstUIRenderGroup[i].clear();
	}

	return hr;
}

HRESULT CRenderer::Render_UI_3D()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_UI_3D)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_NonBlend", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_UI_3D");
	}

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::UI_3D)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::UI_3D)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_NonBlend", "Failed to End_MultiRenderTarget");
	}

	return hr;
}

HRESULT CRenderer::Execute_RadialBlur()
{
	m_pRenderTarget_Manager->Copy_From_BackBuffer(RENDERTARGET_BLUR_BUFFER_A);

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_BLUR_BUFFER_A, m_pShader, SHADER_TEXTARGET_PREPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Bind_RenderTarget: RENDERTARGET_BLUR_BUFFER_A");
	}

	if (FAILED(m_pShader->BeginPass(10)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Render");
	}

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	HRESULT hr = S_OK;
	//CGraphicDevice::Get_Instance()->Clear_DepthStencil_View();
	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::DEBUG)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::DEBUG)].clear();

	return hr;
}
#endif
#pragma endregion

shared_ptr<CRenderer> CRenderer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRenderer> pInstance = make_private_shared(CRenderer, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CRenderer::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CRenderer::Clone(any)
{
	return shared_from_componenet();
}
