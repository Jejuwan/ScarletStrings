#include "ClientPCH.h"
#include "PurpleFilterEffect.h"

#include "GameInstance.h"

CPurpleFilterEffect::CPurpleFilterEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScreenEffect(_pDevice, _pContext)
{
}

CPurpleFilterEffect::CPurpleFilterEffect(const CPurpleFilterEffect& _rhs)
	: CScreenEffect(_rhs)
{
}

HRESULT CPurpleFilterEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPurpleFilterEffect::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CColorReverseEffect::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CPurpleFilterEffect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CPurpleFilterEffect::Late_Tick(_float _fTimeDelta)
{
	Add_RenderObject(RENDER_GROUP::POSTPROCESS);

	_float fScaledTimeDelta = _fTimeDelta;

	m_fShaderTime += fScaledTimeDelta;
	if (m_fShaderTime > 1.f)
	{
		m_fLUTFactor -= fScaledTimeDelta;
		if (m_fLUTFactor < 0.f)
			Set_Enable(false);
	}
	else
	{
		m_fLUTFactor += fScaledTimeDelta;
		if (m_fLUTFactor >= 0.5f)
		{
			m_fLUTFactor = 0.5f;
		}
	}
}

HRESULT CPurpleFilterEffect::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CPurpleFilterEffect::Render", "Failed to CScreenEffect::Render");
	}

	if (FAILED(m_pLUTTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texLUT")))
	{
		MSG_RETURN(E_FAIL, "CPurpleFilterEffect::Render", "Failed to Bind_ShaderResourceView: LUT Texture");
	}

	if (FAILED(m_pShader->Bind_Float("g_fLUTFactor", m_fLUTFactor)))
	{
		MSG_RETURN(E_FAIL, "CPurpleFilterEffect::Render", "Failed to Bind_Float: LUT Factor");
	}

	if (FAILED(m_pVIBuffer_Rect->Render(m_pShader, 13)))
	{
		MSG_RETURN(E_FAIL, "CPurpleFilterEffect::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CPurpleFilterEffect::Set_Enable(_bool bEnable)
{
	__super::Set_Enable(bEnable);
	m_fLUTFactor = 0.f;
	m_fShaderTime = 0.f;
}

HRESULT CPurpleFilterEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CSunnyFliterEffect::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pLUTTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/LUT/LUT_01.png"));
	if (nullptr == m_pLUTTexture)
	{
		MSG_RETURN(E_FAIL, "CSunnyFliterEffect::Ready_Components", "Failed to Create Texture");
	}

	return S_OK;
}

shared_ptr<CPurpleFilterEffect> CPurpleFilterEffect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CPurpleFilterEffect> pInstance = make_private_shared(CPurpleFilterEffect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CPurpleFilterEffect::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CPurpleFilterEffect::Clone(any)
{
	shared_ptr<CPurpleFilterEffect> pInstance = make_private_shared_copy(CPurpleFilterEffect, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CPurpleFilterEffect::Clone", "Failed to Initialize");
	}

	return pInstance;
}
