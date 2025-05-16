#include "ClientPCH.h"
#include "SunnyFliterEffect.h"

#include "GameInstance.h"

CSunnyFliterEffect::CSunnyFliterEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScreenEffect(_pDevice, _pContext)
{
}

CSunnyFliterEffect::CSunnyFliterEffect(const CSunnyFliterEffect& _rhs)
	: CScreenEffect(_rhs)
{
}

HRESULT CSunnyFliterEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSunnyFliterEffect::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CColorReverseEffect::Initialize", "Failed to CGameObject::Initialize");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	m_fOriginExposure = pGameInstance->Get_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING);
	m_fOriginGamma = pGameInstance->Get_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING);

	return S_OK;
}

void CSunnyFliterEffect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CSunnyFliterEffect::Late_Tick(_float _fTimeDelta)
{
	Add_RenderObject(RENDER_GROUP::POSTPROCESS);

	_float fScaledTimeDelta = _fTimeDelta * 2.f;

	m_fShaderTime += fScaledTimeDelta;
	if (m_fShaderTime > 1.f)
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, m_fOriginExposure);
		pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, m_fOriginGamma);

		m_fLUTFactor -= fScaledTimeDelta;
		if (m_fLUTFactor < 0.f)
			Set_Enable(false);
	}
	else
	{
		m_fLUTFactor += fScaledTimeDelta;
		if (m_fLUTFactor >= 0.3f)
		{
			auto pGameInstance = CGameInstance::Get_Instance();
			pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 3.f);
			pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 3.f);
		}
	}
}

HRESULT CSunnyFliterEffect::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CSunnyFliterEffect::Render", "Failed to CScreenEffect::Render");
	}

	if (FAILED(m_pLUTTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texLUT")))
	{
		MSG_RETURN(E_FAIL, "CSunnyFliterEffect::Render", "Failed to Bind_ShaderResourceView: LUT Texture");
	}

	if (FAILED(m_pShader->Bind_Float("g_fLUTFactor", m_fLUTFactor)))
	{
		MSG_RETURN(E_FAIL, "CSunnyFliterEffect::Render", "Failed to Bind_Float: LUT Factor");
	}

	if (FAILED(m_pVIBuffer_Rect->Render(m_pShader, 13)))
	{
		MSG_RETURN(E_FAIL, "CSunnyFliterEffect::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CSunnyFliterEffect::Set_Enable(_bool bEnable)
{
	__super::Set_Enable(bEnable);
	m_fLUTFactor = 0.f;
	m_fShaderTime = 0.f;

	if (bEnable)
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		m_fOriginExposure = pGameInstance->Get_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING);
		m_fOriginGamma = pGameInstance->Get_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING);
	}
	else
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, m_fOriginExposure);
		pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, m_fOriginGamma);
	}
}

HRESULT CSunnyFliterEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CSunnyFliterEffect::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pLUTTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/LUT/T_SunnyNoisePowerCurve.png"));
	if (nullptr == m_pLUTTexture)
	{
		MSG_RETURN(E_FAIL, "CSunnyFliterEffect::Ready_Components", "Failed to Create Texture");
	}

	return S_OK;
}

shared_ptr<CSunnyFliterEffect> CSunnyFliterEffect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CSunnyFliterEffect> pInstance = make_private_shared(CSunnyFliterEffect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CSunnyFliterEffect::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CSunnyFliterEffect::Clone(any)
{
	shared_ptr<CSunnyFliterEffect> pInstance = make_private_shared_copy(CSunnyFliterEffect, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CSunnyFliterEffect::Clone", "Failed to Initialize");
	}

	return pInstance;
}
