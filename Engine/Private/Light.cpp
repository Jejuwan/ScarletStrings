#include "EnginePCH.h"
#include "Light.h"
#include "Light_Manager.h"
#include "Transform.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Object_Manager.h"

#define SHADOW_NEAR			0.1f
#define SHADOW_FAR			10000.f

#define SHADOW_ORTHO_SIZE	20

CLight::CLight(const LIGHTDESC _tLightDesc)
	: m_tInitialData(_tLightDesc)
{
}

HRESULT CLight::Initialize(const LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	m_tLightDesc.vDirection = _float3(_tLightDesc.vDirection).normalize();

	switch (_tLightDesc.eLightType)
	{
	case LIGHTTYPE::DIRECTIONAL:
		break;

	case LIGHTTYPE::POINT:
		if (nullptr == _pTransform)
		{
			MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: CTransform");
		}
		break;

	case LIGHTTYPE::SHADOW:
	{
		switch (_tLightDesc.eShadowType)
		{
		case SHADOWTYPE::DIRECTIONAL:
		{
			m_mShadowView		= XMMatrixLookToLH(_float4(_tLightDesc.vPosition, 1.f), _float4(_tLightDesc.vDirection, 0.f).normalize(), _float4(0.f, 1.f, 0.f, 0.f));
			m_mShadowProjection	= XMMatrixOrthographicLH(_tLightDesc.vShadowProjection.x, _tLightDesc.vShadowProjection.y, SHADOW_NEAR, SHADOW_FAR);
		}
		break;
		case SHADOWTYPE::DYNAMIC:
		{
			if (nullptr == _pTransform)
			{
				MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: CTransform");
			}
			m_mShadowView		= XMMatrixLookToLH(_float4(_tLightDesc.vPosition, 1.f), _float4(_tLightDesc.vDirection, 0.f).normalize(), _float4(0.f, 1.f, 0.f, 0.f));
			m_mShadowProjection	= XMMatrixOrthographicLH(_tLightDesc.vShadowProjection.x, _tLightDesc.vShadowProjection.y, SHADOW_NEAR, SHADOW_FAR);
		}
		break;
		case SHADOWTYPE::POINT:
		{
			if (nullptr == _pTransform)
			{
				MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: CTransform");
			}
			m_mShadowProjection	= XMMatrixPerspectiveFovLH(XM_2PI, 1.f, SHADOW_NEAR, SHADOW_FAR);
		}
		break;
		default:
			MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: SHADOWTYPE");
		}
	}
	break;
	default:
		MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: LIGHTTYPE");
	}

	m_tLightDesc = _tLightDesc;
	m_pTransform = _pTransform;

	return S_OK;
}

void CLight::Tick(_float _fTimeDelta)
{
	if (m_pTransform.expired())
	{
		return;
	}

	shared_ptr<CTransform> pTransform = m_pTransform.lock();

	switch (m_tLightDesc.eLightType)
	{
	case LIGHTTYPE::POINT:
		m_tLightDesc.vPosition = pTransform->Get_State(TRANSFORM::POSITION);
		if (m_tLightDesc.fLifeTime)
		{
			m_tLightDesc.fRange -= m_tInitialData.fRange / m_tInitialData.fLifeTime * _fTimeDelta;
			m_tLightDesc.fLifeTime -= _fTimeDelta;
			if (m_tLightDesc.fRange <= 0.f)
			{
				CLight_Manager::Get_Instance()->Erase_Light(static_pointer_cast<CLight>(shared_from_this()));
			}
		}

		break;

	case LIGHTTYPE::SHADOW:
		switch (m_tLightDesc.eShadowType)
		{
		case SHADOWTYPE::DYNAMIC:
		{
			m_mShadowView = XMMatrixLookToLH(
				_float4(pTransform->Get_State(TRANSFORM::POSITION) - _float3(m_tLightDesc.vDirection) * m_tLightDesc.fRange, 1.f),
				_float4(m_tLightDesc.vDirection, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f));
		}
		break;
		case SHADOWTYPE::POINT:
		{
			m_mShadowView = XMMatrixLookToLH(pTransform->Get_State(TRANSFORM::POSITION), pTransform->Get_State(TRANSFORM::LOOK), XMVectorSet(0.f, 1.f, 0.f, 0.f));
		}
		break;
		}
		break;
	}
}

_bool CLight::Is_Expired() const
{
	switch (m_tLightDesc.eLightType)
	{
	case LIGHTTYPE::DIRECTIONAL:
		return false;

	case LIGHTTYPE::POINT:
		return m_pTransform.expired();

	case LIGHTTYPE::SHADOW:
		switch (m_tLightDesc.eShadowType)
		{
		case SHADOWTYPE::DIRECTIONAL:
			return false;
		case SHADOWTYPE::DYNAMIC:
		case SHADOWTYPE::POINT:
			return m_pTransform.expired();
		}

	default:
		MSG_RETURN(true, "CLight::Is_Expired", "Invalid LightType");
	}
}

HRESULT CLight::Render_ShadowDepth(shared_ptr<CRenderer> _pRenderer)
{
	_pRenderer->Shadow_Settings(m_tShadowDesc.fShadowShade, m_tShadowDesc.fBias, m_tShadowDesc.vGridSize, m_tShadowDesc.fMinShadowAvg, m_tShadowDesc.fTexelScale);

	for (auto& pLayer : m_lstLayer)
	{
		auto pLayerInstance = pLayer.lock();
		if (pLayerInstance)
		{
			pLayerInstance->Iterate_Objects(
				[&](shared_ptr<CGameObject> _pObject)->_bool
				{
					if (FAILED(_pObject->Render_ShadowDepth(static_pointer_cast<CLight>(shared_from_this()))))
					{
						MSG_RETURN(E_FAIL, "CLight::Render_ShadowDepth", "Failed to Render_ShadowDepth");
					}

					return true;
				}
			);
		}
	}

	return S_OK;
}

HRESULT CLight::Bind_Light(shared_ptr<CShader> _pShader, shared_ptr<CVIBuffer_Rect> _pVIBuffer, _bool bSSAO)
{
	if (LIGHTTYPE::SHADOW == m_tLightDesc.eLightType)
	{
		return S_OK;
	}

	if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTDIF, m_tLightDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTDIF");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTAMB, m_tLightDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTAMB");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTSPC,m_tLightDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTSPC");
	}
	if (FAILED(_pShader->Bind_Float(SHADER_ATT0, m_tLightDesc.fAttenuation0)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Float: SHADER_ATT0");
	}
	if (FAILED(_pShader->Bind_Float(SHADER_ATT1, m_tLightDesc.fAttenuation1)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Float: SHADER_ATT1");
	}
	if (FAILED(_pShader->Bind_Float(SHADER_ATT2, m_tLightDesc.fAttenuation2)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Float: SHADER_ATT2");
	}

	switch (m_tLightDesc.eLightType)
	{
	case LIGHTTYPE::DIRECTIONAL:
	{
		if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTDIR, _float4(m_tLightDesc.vDirection, 0.f))))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTDIR");
		}

		if (bSSAO)
		{
			if (FAILED(_pShader->BeginPass(8)))
			{
				MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to BeginPass");
			}
		}
		else
		{
			if (FAILED(_pShader->BeginPass(0)))
			{
				MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to BeginPass");
			}
		}

		if (FAILED(_pVIBuffer->Render()))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Render");
		}
	}
	break;
	case LIGHTTYPE::POINT:
	{
		if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTPOS, _float4(m_tLightDesc.vPosition, 1.f))))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTPOS");
		}
		if (FAILED(_pShader->Bind_Float(SHADER_LIGHTRNG, m_tLightDesc.fRange)))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Float: SHADER_LIGHTRNG");
		}

		if (FAILED(_pShader->BeginPass(1))) 
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to BeginPass");
		}
		if (FAILED(_pVIBuffer->Render()))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Render");
		}
	}
	break;

	default:
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Invalid LightType");
	}

	return S_OK;
}

void CLight::Set_LightDesc(LIGHTDESC _tLightDesc)
{
	m_tLightDesc = _tLightDesc;

	Initialize(_tLightDesc, m_pTransform.lock());
}

HRESULT CLight::Register_Layer(SCENE _eScene, const wstring& _wstrLayerTag)
{
	shared_ptr<CObjectLayer> pLayer = CObject_Manager::Get_Instance()->Find_Layer(_eScene, _wstrLayerTag);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CLight::Register_Layer", "Failed to Find_Layer");
	}

	m_lstLayer.emplace_back(pLayer);

	return S_OK;
}

HRESULT CLight::Bind_ShadowMatrix(shared_ptr<CShader> _pShader)
{
	if (FAILED(_pShader->Bind_Matrix(SHADER_MATVIEW, m_mShadowView)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_ShadowMatrix", "Failed to Bind_Matrix: SHADER_MATVIEW");
	}
	if (FAILED(_pShader->Bind_Matrix(SHADER_MATPROJ, m_mShadowProjection)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_ShadowMatrix", "Failed to Bind_Matrix: SHADER_MATVIEW");
	}

	return S_OK;
}

shared_ptr<CLight> CLight::Create(const LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	shared_ptr<CLight> pInstance = make_private_shared(CLight, _tLightDesc);

	if (FAILED(pInstance->Initialize(_tLightDesc, _pTransform)))
	{
		MSG_RETURN(nullptr, "CLight::Create", "Failed to Initialize");
	}

	return pInstance;
}
