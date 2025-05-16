#include "ClientPCH.h"
#include "TitleBackground.h"

#include "GameInstance.h"

CTitleBackground::CTitleBackground(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTitleBackground::CTitleBackground(const CTitleBackground& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTitleBackground::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::TEXTURE);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_LOADING, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE] = make_pair(PROTOTYPE_COMPONENT_TEXTURE_TITLE_BACKGROUND, g_aNull);

	return S_OK;
}

HRESULT CTitleBackground::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Scale({ g_iWinCX, g_iWinCY, 1.f });
	m_pTransform->Set_State(TRANSFORM::POSITION, { 0.f, 0.f, 0.9f, 1.f });

	return S_OK;
}

void CTitleBackground::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CTitleBackground::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::PRIORITY);
}

HRESULT CTitleBackground::Render()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CTexture::Bind_OnShader");
	}

	if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}
	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTitleBackground::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}
	m_pTransform->Set_Matrix(g_mUnit);

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: CVIBuffer_Rect");
	}

	m_pTexture = Get_Component<CTexture>(COMPONENT::TEXTURE);
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	return S_OK;
}

shared_ptr<CTitleBackground> CTitleBackground::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTitleBackground> pInstance = make_private_shared(CTitleBackground, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTitleBackground::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTitleBackground::Clone(any)
{
	shared_ptr<CTitleBackground> pInstance = make_private_shared_copy(CTitleBackground, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTitleBackground::Clone", "Failed to Initialize");
	}

	return pInstance;
}
