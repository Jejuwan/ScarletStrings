#include "ClientPCH.h"
#include "MenuCharacter.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "RenderTarget_Manager.h"

CMenuCharacter::CMenuCharacter(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMenuCharacter::CMenuCharacter(const CMenuCharacter& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CMenuCharacter::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);

	return S_OK;
}

HRESULT CMenuCharacter::Initialize(any tMenuCharacterDesc)
{
	m_tMenuCharacterDesc = any_cast<MENUCHARACTERDESC>(tMenuCharacterDesc);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(m_tMenuCharacterDesc.wszModelTag, g_aNull);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Matrix(m_tMenuCharacterDesc.matTransform);

	_uint iNumMesh = m_pModel->Get_NumMeshes();
	for (_uint i = 0; i < iNumMesh; ++i)
	{
		SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
		tShaderDesc.tMaterialDesc.vDiffuse = m_tMenuCharacterDesc.vMtrlDiffuse;

		m_pModel->Set_ShaderDesc(i, tShaderDesc);
	}

	m_pModel->Set_Animation(m_tMenuCharacterDesc.iAnimIndex, 2.f, false, 0.f, true);

	m_pRenderer->Subscribe_Render_Finish_Callback(std::bind(&CMenuCharacter::On_RenderFinishedCallback, this));

	m_pUITransform->Set_Scale({ g_iWinCX, g_iWinCY, 1.f });
	m_pUITransform->Set_State(TRANSFORM::POSITION, { 0.f, 0.f, 0.5f, 1.f });

	m_matProjection = XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_bEnable = false;

	return S_OK;
}

void CMenuCharacter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModel->Tick_Animation(fTimeDelta);

	if (m_bFading)
	{
		if (m_bFadeIn)
		{
			m_fFadeFactor += fTimeDelta;
			if (m_fFadeFactor > 1.f)
			{
				m_fFadeFactor = 1.f;
				m_bFading = false;
			}
		}
		else
		{
			m_fFadeFactor -= fTimeDelta;
			if (m_fFadeFactor < 0.f)
			{
				m_fFadeFactor = 0.f;
				m_bFading = false;
			}
		}
	}
}

void CMenuCharacter::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(UI_LAYER::LAYER_7);
}

HRESULT CMenuCharacter::Render()
{
	if (false == m_bRenderUI)
	{
		if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
		{
			MSG_RETURN(E_FAIL, "CMenuCharacter::Bind_OnShader", "Failed to CShader::Bind_Matrix");
		}
		if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, XMMatrixIdentity())))
		{
			MSG_RETURN(E_FAIL, "CMenuCharacter::Bind_OnShader", "Failed to CShader::Bind_Matrix");
		}
		if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
		{
			MSG_RETURN(E_FAIL, "CMenuCharacter::Bind_OnShader", "Failed to CShader::Bind_Matrix");
		}

		for (_uint i = 0; i < m_tMenuCharacterDesc.vecMeshPassIndexPair.size(); ++i)
		{
			_uint iMeshIndex = m_tMenuCharacterDesc.vecMeshPassIndexPair[i].first;
			_uint iPassIndex = m_tMenuCharacterDesc.vecMeshPassIndexPair[i].second;
			if (FAILED(m_pModel->Bind_ShaderResourceViews(iMeshIndex, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CMenuCharacter::Render", "Failed to Bind_ShaderResourceViews");
			}
			if (FAILED(m_pModel->Bind_ShaderOptions(iMeshIndex, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CMenuCharacter::Render", "Failed to Bind_ShaderOptions");
			}
			if (FAILED(m_pModel->Bind_BoneMatrices(iMeshIndex, m_pShader, SHADER_BONE)))
			{
				MSG_RETURN(E_FAIL, "CMenuCharacter::Render", "Failed to Bind_BoneMatrices");
			}

			if (FAILED(m_pModel->Render(iMeshIndex, m_pShader, iPassIndex)))
			{
				MSG_RETURN(E_FAIL, "CMenuCharacter::Render", "Failed to Render Model");
			}
		}

		m_bRenderUI = true;
	}
	else
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		if (FAILED(m_pUIShader->Bind_ShaderResourceView(SHADER_TEXDIFFUSE, pGameInstance->Get_RenderTarget_ShaderResourceView(RENDERTARGET_UI_3D_CHARACTER))))
		{
			MSG_RETURN(E_FAIL, "CMenuCharacter::Render", "Failed to Bind_ShaderResourceViews");
		}
		if (FAILED(m_pUIShader->Bind_ShaderResourceView(SHADER_TEXMASK, pGameInstance->Get_RenderTarget_ShaderResourceView(RENDERTARGET_DEPTH))))
		{
			MSG_RETURN(E_FAIL, "CMenuCharacter::Render", "Failed to Bind_ShaderResourceViews");
		}

		if (FAILED(m_pUIShader->Bind_Matrix(SHADER_MATWORLD, m_pUITransform->Get_Matrix())))
		{
			MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
		}
		if (FAILED(m_pUIShader->Bind_Matrix(SHADER_MATVIEW, XMMatrixIdentity())))
		{
			MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
		}
		if (FAILED(m_pUIShader->Bind_Matrix(SHADER_MATPROJ, m_matProjection)))
		{
			MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
		}

		if (FAILED(m_pUIShader->Bind_Float("g_fFadeFactor", m_fFadeFactor)))
		{
			MSG_RETURN(E_FAIL, "CMenuCharacter::Render", "Failed to Bind FadeFactor");
		}
		
		if (FAILED(m_pVIBuffer_Rect->Render(m_pUIShader, 8)))
		{
			MSG_RETURN(E_FAIL, "CMenuCharacter::Render", "Failed to CVIBuffer::Render");
		}

		m_bRenderUI = false;
	}

	return S_OK;
}

void CMenuCharacter::Fade_In()
{
	m_bFading = true;
	m_bFadeIn = true;

	m_fFadeFactor = 0.f;
}

void CMenuCharacter::Fade_Out()
{
	m_bFading = true;
	m_bFadeIn = false;

	m_fFadeFactor = 1.f;
}

HRESULT CMenuCharacter::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMenuCharacter::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CMenuCharacter::Ready_Components", "Failed to Get_Component: RENDERER");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMenuCharacter::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}
	m_pTransform->Set_Matrix(g_mUnit);

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMenuCharacter::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMenuCharacter::Ready_Components", "Failed to Get_Component: Model");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pUITransform = CTransform::Create(m_pDevice, m_pContext);
	m_pUITransform->Set_Matrix(g_mUnit);

	m_pUIShader = pGameInstance->Clone_Component<CShader>(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE);
	if (nullptr == m_pUIShader)
	{
		MSG_RETURN(E_FAIL, "CMenuCharacter::Ready_Components", "Failed to Clone_Component: Shader");
	}

	m_pVIBuffer_Rect = pGameInstance->Clone_Component<CVIBuffer_Rect>(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CMenuCharacter::Ready_Components", "Failed to Clone_Component: VIBuffer");
	}

	return S_OK;
}

void CMenuCharacter::On_RenderFinishedCallback()
{
	CGameInstance::Get_Instance()->Copy_From_BackBuffer(RENDERTARGET_UI_3D_CHARACTER);
	//CRenderTarget_Manager::Get_Instance()->Copy_From_BackBuffer();
}

shared_ptr<CMenuCharacter> CMenuCharacter::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, any)
{
	shared_ptr<CMenuCharacter> pInstance = make_private_shared(CMenuCharacter, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMenuCharacter::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMenuCharacter::Clone(any tMenuCharacterDesc)
{
	shared_ptr<CMenuCharacter> pInstance = make_private_shared_copy(CMenuCharacter, *this);

	if (FAILED(pInstance->Initialize(tMenuCharacterDesc)))
	{
		MSG_RETURN(nullptr, "CMenuCharacter::Clone", "Failed to Initialize");
	}

	return pInstance;
}
