#include "ClientPCH.h"
#include "SkyCube.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Map_ToolManager.h"

CSkyCube::CSkyCube(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CSkyCube::CSkyCube(const CSkyCube& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CSkyCube::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_SPHERE);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXSKYBOXPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_SPHERE]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_SPHERE, g_aNull);

	return S_OK;
}

HRESULT CSkyCube::Initialize(any _wstrTexturePath)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTexture = dynamic_pointer_cast<CTexture>(CTexture::Create(m_pDevice, m_pContext, any_cast<wstring>(_wstrTexturePath), 1, true)->Clone());
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Initialize", "Null Exception: m_pTexture");
	}

	return S_OK;
}

void CSkyCube::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	/*if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::MINDROOM)
	{
		if (CMap_ToolManager::Get_Instance()->Is_MindRoom())
		{
			shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_BACKGROUND);

		}
		else
		{

		}
	}*/
}

void CSkyCube::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::PRIORITY);

	if (m_bSingleColorMode)
	{
		m_fAccTime += _fTimeDelta;
		if (m_fAccTime >= m_fDuration)
		{
			m_fAccTime = m_fDuration;
		}
	}

	if (m_bGradationClearMode)
	{
		m_fAccTime += _fTimeDelta;
		if (m_fAccTime >= 1.f)
		{
			m_fAccTime = 1.f;
		}
	}

//#ifdef _DEBUG
//	if (CImGui_Manager::Get_Instance()->Is_Enable())
//	{
//		ImGui::Begin("LerpFactor");
//		ImGui::SliderFloat("LerpFactor", &m_fAccTime, -5.f, 5.f);
//		ImGui::End();
//	}
//#endif
}

HRESULT CSkyCube::Render()
{
	if (m_bSingleColorMode)
	{
		if (FAILED(m_pShader->Bind_RawValue("g_vColorA", &m_vColorA, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CSkyCube::Bind_RawValue");
		}
		if (FAILED(m_pShader->Bind_RawValue("g_vColorB", &m_vColorB, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CSkyCube::Bind_RawValue");
		}

		if (FAILED(m_pShader->Bind_Float("g_fLerpFactor", m_fAccTime / m_fDuration)))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CSkyCube::Bind_Float");
		}

		if (FAILED(__super::Render(1)))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CGameObject::Render");
		}
	}
	else if (m_bGradationMode)
	{
		if (FAILED(m_pShader->Bind_RawValue("g_vColorA", &m_vColorA, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CSkyCube::Bind_RawValue");
		}
		if (FAILED(m_pShader->Bind_RawValue("g_vColorB", &m_vColorB, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CSkyCube::Bind_RawValue");
		}
		
		m_pTransform->Set_Matrix(CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD));
		
		if (FAILED(__super::Render(2)))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CGameObject::Render");
		}
	}
	else if (m_bGradationClearMode)
	{
		if (FAILED(m_pShader->Bind_RawValue("g_vColorA", &m_vColorA, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CSkyCube::Bind_RawValue");
		}

		if (FAILED(m_pShader->Bind_Float("g_fLerpFactor", m_fAccTime)))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CSkyCube::Bind_Float");
		}

		m_pTransform->Set_Matrix(CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD));

		if (FAILED(__super::Render(3)))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CGameObject::Render");
		}
	}
	else
	{
		if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texCubeDiffuse")))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CTexture::Bind_ShaderResourceView");
		}

		if (FAILED(__super::Render(0)))
		{
			MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CGameObject::Render");
		}
	}

	return S_OK;
}

void CSkyCube::Set_LerpColor(_float3 vColorA, _float3 vColorB, _float fDuration)
{
	m_bSingleColorMode = true;
	m_bGradationMode = false;
	m_bGradationClearMode = false;

	m_vColorA = vColorA;
	m_vColorB = vColorB;

	m_fAccTime = 0.f;
	m_fDuration = fDuration;
}

void CSkyCube::Set_GradationBackGround(_float3 vColorA, _float3 vColorB)
{
	m_bGradationMode = true;
	m_bSingleColorMode = false;
	m_bGradationClearMode = false;

	m_vColorA = vColorA;
	m_vColorB = vColorB;
}

void CSkyCube::GradientClear(_float3 vColorA)
{
	m_bGradationClearMode = true;
	m_bGradationMode = false;
	m_bSingleColorMode = false;

	m_fAccTime = -0.5f;

	m_vColorA = vColorA;
}

void CSkyCube::Reset()
{
	m_bSingleColorMode = false;
	m_bGradationMode = false;
	m_bGradationClearMode = false;
}

HRESULT CSkyCube::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}
	
	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Ready_Components", "Failed to Get_Component: SHADER");
	}

	return S_OK;
}

shared_ptr<CSkyCube> CSkyCube::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CSkyCube> pInstance = make_private_shared(CSkyCube, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CSkyCube::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CSkyCube::Clone(any _wstrTexturePath)
{
	shared_ptr<CSkyCube> pInstance = make_private_shared_copy(CSkyCube, *this);

	if (FAILED(pInstance->Initialize(_wstrTexturePath)))
	{
		MSG_RETURN(nullptr, "CSkyCube::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}
