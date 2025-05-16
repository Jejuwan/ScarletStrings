#include "EnginePCH.h"
#include "Light_Manager.h"
#include "Light.h"
#include "Shader.h"
#include "VIBuffer.h"
#include "Object_Manager.h"
#include "RenderTarget_Manager.h"

#include "Texture.h"
#include "PipeLine.h"
#include "VIBuffer_Rect.h"

#include "Frustom.h"

#define SHADOW_RESOLUTION	_uint2(m_tTexture2DDesc.Width, m_tTexture2DDesc.Height)

HRESULT CLight_Manager::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const GRAPHICDESC _tGraphicDesc, const SCENE _eSceneMax)
{
	m_pDevice = _pDevice;
	m_pContext = _pContext;

	m_eSceneMax = _eSceneMax;
	
	m_arrLights = Function::CreateDynamicArray<Lights>(IDX(_eSceneMax), false);

	m_tTexture2DDesc.Width = _tGraphicDesc.iWinCX * g_iShadowScale;
	m_tTexture2DDesc.Height = _tGraphicDesc.iWinCY * g_iShadowScale;
	m_tTexture2DDesc.MipLevels = 1;
	m_tTexture2DDesc.ArraySize = 1;
	m_tTexture2DDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_tTexture2DDesc.SampleDesc.Count = 1;
	m_tTexture2DDesc.SampleDesc.Quality = 0;

	m_tTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_tTexture2DDesc.CPUAccessFlags = 0;
	m_tTexture2DDesc.MiscFlags = 0;

	// Lensflare
	m_pLensNoiseTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Common/RGBANoise.png"));
	if (nullptr == m_pLensNoiseTexture)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Initialize", "Can not Load LensNoiseTexture");
	}

	// Default Reflection
	m_pReflection = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Cubemap/T_Cubemap.dds"), 1, true);
	if (nullptr == m_pReflection)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Initialize", "Can not Load Default Reflection");
	}

	return S_OK;
}

void CLight_Manager::Tick(_float _fTimeDelta)
{
	for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
	{
		for (auto iter = m_arrLights[i].begin(); iter != m_arrLights[i].end();)
		{
			if ((*iter)->Is_Expired())
			{
				iter = m_arrLights[i].erase(iter);
				if ((*iter) && LIGHTTYPE::SHADOW == (*iter)->Get_LightDesc().eLightType)
				{
					m_mapShadowShaderResourceViews.erase((*iter));
				}
			}
			else
			{
				(*iter)->Tick(_fTimeDelta);
				++iter;
			}
		}
	}

	for (auto& pLight : m_lstErase)
	{
		for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
		{
			for (auto iter = m_arrLights[i].begin(); iter != m_arrLights[i].end();)
			{
				if (*iter == pLight)
				{
					iter = m_arrLights[i].erase(iter);
					--m_iLightCount;
				}
				else
				{
					++iter;
				}
			}
		}
	}

	m_lstErase.clear();
	if (m_bEnableLensflare)
	{
		m_fLensflareAccTime += 0.016f;
	}
}

shared_ptr<CLight> CLight_Manager::Add_Light(const SCENE _eScene, LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform, shared_ptr<CGameObject> _pObject)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CLight_Manager::Add_Light", "Invalid Range: SCENE");
	}

	if (m_iLightCount >= g_iMaxLights - 2)
	{
		MSG_RETURN(nullptr, "CLight_Manager::Add_Light", "Out of Range: m_iLightCount");
	}
	//if (_tLightDesc.eLightType == LIGHTTYPE::POINT)
	//	return nullptr;

	shared_ptr<CLight> pLight = CLight::Create(_tLightDesc, _pTransform);
	if (nullptr == pLight)
	{
		MSG_RETURN(nullptr, "CLight_Manager::Add_Light", "Failed to Create: CLight");
	}

	m_arrLights[IDX(_eScene)].emplace_back(pLight);

	++m_iLightCount;

	if (LIGHTTYPE::SHADOW == _tLightDesc.eLightType)
	{
		ComPtr<ID3D11Texture2D>				pTexture2D;
		ComPtr<ID3D11ShaderResourceView>	pShaderResourceView;

		if (FAILED(m_pDevice->CreateTexture2D(&m_tTexture2DDesc, nullptr, pTexture2D.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CLight_Manager::Add_Light", "Failed to CreateTexture2D");
		}
		if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D.Get(), nullptr, pShaderResourceView.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CLight_Manager::Add_Light", "Failed to CreateShaderResourceView");
		}

		m_mapShadowShaderResourceViews.emplace(pLight, pShaderResourceView);
	}

	return pLight;
}

HRESULT CLight_Manager::Add_Light(const SCENE _eScene, shared_ptr<class CLight> pLight)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Light", "Invalid Range: SCENE");
	}

	if (m_iLightCount >= g_iMaxLights - 2)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Light", "Out of Range: m_iLightCount");
	}

	m_arrLights[IDX(_eScene)].emplace_back(pLight);
	++m_iLightCount;

	return S_OK;
}

HRESULT CLight_Manager::Erase_Light(shared_ptr<CLight> _pLight)
{
	m_lstErase.emplace_back(_pLight);
	return S_OK;
}

HRESULT CLight_Manager::Clear_Lights(const SCENE _eScene)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Clear_Lights", "Invalid Range: SCENE");
	}

	m_iLightCount -= static_cast<_uint>(m_arrLights[IDX(_eScene)].size());

	m_arrLights[IDX(_eScene)].clear();
	m_mapShadowShaderResourceViews.clear();

	return S_OK;
}

HRESULT CLight_Manager::Bind_Lights(shared_ptr<CShader> _pShader, shared_ptr<CVIBuffer_Rect> _pVIBuffer)
{
	HRESULT hr = S_OK;

	for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
	{
		for (auto& pLight : m_arrLights[i])
		{
			if (m_bEnableSSAO)
			{
				const LIGHTDESC& LightDesc = pLight->Get_LightDesc();
				if (LightDesc.eLightType == LIGHTTYPE::DIRECTIONAL)
				{
					auto pRenderTargetManager = CRenderTarget_Manager::Get_Instance();
					if (FAILED(pRenderTargetManager->Bind_RenderTarget(RENDERTARGET_LIGHT_SSAO, _pShader, SHADER_TEXTARGET_SSAO)))
					{
						MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_RenderTarget: RENDERTARGET_DEPTH");
					}
				}

				if (FAILED(pLight->Bind_Light(_pShader, _pVIBuffer, true)))
				{
					hr = E_FAIL;
					MSG_BOX("CLight_Manager::Bind_Lights", "Failed to Bind_Light");
				}
			}
			else
			{
				if (FAILED(pLight->Bind_Light(_pShader, _pVIBuffer)))
				{
					hr = E_FAIL;
					MSG_BOX("CLight_Manager::Bind_Lights", "Failed to Bind_Light");
				}
			}
		}
	}

	return hr;
}

HRESULT CLight_Manager::Bind_ShadowMatrices(shared_ptr<CShader> _pShader)
{
	for (auto& iter : m_mapShadowShaderResourceViews)
	{
		m_vecShadowViewMatrices.emplace_back(iter.first->Get_ShadowView());
		m_vecShadowProjectionMatrices.emplace_back(iter.first->Get_ShadowProjection());
		m_vecShadowRawShaderResourceViews.emplace_back(iter.second.Get());
	}

	if (FAILED(_pShader->Bind_Int(SHADER_SHADOWCOUNT, static_cast<_uint>(m_mapShadowShaderResourceViews.size()))))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_ShadowMatrices", "Failed to Bind_Int: SHADER_SHADOWCOUNT");
	}
	if (FAILED(_pShader->Bind_MatrixArray(SHADER_MATSHADOWVIEWS, m_vecShadowViewMatrices.data(), static_cast<_uint>(m_vecShadowViewMatrices.size()))))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_ShadowMatrices", "Failed to Bind_MatrixArray: SHADER_MATSHADOWVIEWS");
	}
	if (FAILED(_pShader->Bind_MatrixArray(SHADER_MATSHADOWPROJS, m_vecShadowProjectionMatrices.data(), static_cast<_uint>(m_vecShadowProjectionMatrices.size()))))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_ShadowMatrices", "Failed to Bind_MatrixArray: SHADER_MATSHADOWPROJS");
	}

	if (FAILED(_pShader->Bind_ShaderResourceViews(SHADER_TEXTARGET_SHADOWDEPTH, m_vecShadowRawShaderResourceViews.data(), 0, static_cast<_uint>(m_mapShadowShaderResourceViews.size()))))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_ShadowMatrices", "Failed to Bind_ShaderResourceViews: SHADER_TEXTARGET_SHADOWDEPTH");
	}

	m_vecShadowViewMatrices.clear();
	m_vecShadowProjectionMatrices.clear();
	m_vecShadowRawShaderResourceViews.clear();

	return S_OK;
}

HRESULT CLight_Manager::Bind_LightDirection(SCENE eScene, shared_ptr<class CShader> pShader)
{
	for (auto& pLight : m_arrLights[IDX(eScene)])
	{
		const LIGHTDESC& tLightDesc = pLight->Get_LightDesc();
		if (tLightDesc.eLightType == LIGHTTYPE::DIRECTIONAL)
		{
			if (FAILED(pShader->Bind_RawValue(SHADER_LIGHTDIR, &tLightDesc.vDirection, sizeof(_float3))))
			{
				MSG_RETURN(E_FAIL, "CLight_Manager::Bind_LightDirection", "Failed to Bind_RawValue: SHADER_LIGHTDIR");
			}
		}
	}

	return S_OK;
}

void CLight_Manager::Iterate_Shadows(function<void(shared_ptr<CLight>)> _fpListener)
{
	for (auto& iter : m_mapShadowShaderResourceViews)
	{
		_fpListener(iter.first);
	}
}

void CLight_Manager::Copy_RenderedShadows(shared_ptr<CLight> _pLight, ComPtr<ID3D11ShaderResourceView> _pTarget)
{
	ComPtr<ID3D11Resource> pSrc, pDst;

	_pTarget->GetResource(pSrc.GetAddressOf());
	m_mapShadowShaderResourceViews[_pLight]->GetResource(pDst.GetAddressOf());
	m_pContext->CopyResource(pDst.Get(), pSrc.Get());
}

HRESULT CLight_Manager::Render_Lensflare(shared_ptr<class CShader> pShader, shared_ptr<CVIBuffer_Rect> pVIBuffer, ComPtr<ID3D11ShaderResourceView> pDepthSRV)
{
	if (false == m_bEnableLensflare)
	{
		return S_OK;
	}

	_float4x4 mCameraWorldMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).inverse();
	_vector vScale, vRotation, vTransflation;
	XMMatrixDecompose(&vScale, &vRotation, &vTransflation, mCameraWorldMatrix);
	_matrix mSunMatrix = XMMatrixRotationRollPitchYawFromVector(vRotation);

	//_vector vCamLook = XMLoadFloat3((_float3*)mCameraWorldMatrix.m[2]);
	//_vector vSunDir = m_vSunPosition - vTransflation;
	//if (XMVectorGetX(XMVector3Dot(vCamLook, vSunDir)) < 0.f)
	//{
	//	return S_OK;
	//}
	
	if (FAILED(pShader->Bind_Matrix(SHADER_MATWORLD, mSunMatrix)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_Matrix: SHADER_MATWORLD");
	}
	if (FAILED(pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_Matrix: SHADER_MATVIEW");
	}
	if (FAILED(pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_Matrix: SHADER_MATPROJ");
	}

	if (FAILED(m_pLensNoiseTexture->Bind_ShaderResourceView(pShader, aiTextureType_DIFFUSE, "g_NoiseTexture")))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(pShader->Bind_ShaderResourceView(SHADER_TEXTARGET_DEPTH, pDepthSRV)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_RenderTarget: RENDERTARGET_DEPTH");
	}

	if (FAILED(pShader->Bind_Float("fTime", m_fLensflareAccTime)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_Float");
	}

	if (FAILED(pShader->Bind_Float("fStrength", m_fLensflareStrength)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_Float");
	}

	if (FAILED(pShader->Bind_RawValue("g_vFlarePosition", &m_vSunPosition, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_RawValue: FlarePosition");
	}

	if (FAILED(pShader->Bind_RawValue("g_vSunBaseColor", &m_vSunColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Bind_RawValue: g_vSunBaseColor");
	}

	if (FAILED(pShader->BeginPass(0)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to BeginPass");
	}
	if (FAILED(pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Lensflare", "Failed to Render");
	}
	
	return S_OK;
}

void CLight_Manager::Set_Reflection(shared_ptr<CTexture> pReflection)
{
	m_pReflection = pReflection;
}

HRESULT CLight_Manager::Bind_Reflection(shared_ptr<class CShader> pShader)
{
	if (FAILED(pShader->Bind_ShaderResourceView(SHADER_REFLECTION, m_pReflection->Get_ShaderResourceView())))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Reflection", "Failed to Bind_ShaderResourceView: Reflection");
	}

	return S_OK;
}
