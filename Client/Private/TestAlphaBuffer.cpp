#include "ClientPCH.h"
#include "TestAlphaBuffer.h"
#include "GameInstance.h"

CTestAlphaBuffer::CTestAlphaBuffer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTestAlphaBuffer::CTestAlphaBuffer(const CTestAlphaBuffer& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTestAlphaBuffer::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	return S_OK;
}

HRESULT CTestAlphaBuffer::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CTestAlphaBuffer::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CTestAlphaBuffer::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CTestAlphaBuffer::Render()
{
	if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}

	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTestAlphaBuffer::Render_OutLine()
{
	return S_OK;
}

HRESULT CTestAlphaBuffer::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pTexture = CGameInstance::Get_Instance()->Clone_Component<CTexture>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_COMPONENT_TEXTURE_TEST_ALPHA);
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Clone_Component: PROTOTYPE_COMPONENT_TEXTURE_BLACK");
	}

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: VIBUFFER_RECT");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

HRESULT CTestAlphaBuffer::Set_Texture(shared_ptr<CTexture> _pTexture)
{
	m_pTexture = _pTexture;

	return S_OK;
}

shared_ptr<CTestAlphaBuffer> CTestAlphaBuffer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext>_pContext)
{
	shared_ptr<CTestAlphaBuffer> pInstance = make_private_shared(CTestAlphaBuffer, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTestAlphaBuffer::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTestAlphaBuffer::Clone(any pArg)
{
	shared_ptr<CTestAlphaBuffer> pInstance = make_private_shared_copy(CTestAlphaBuffer, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CTestAlphaBuffer::Clone", "Failed to Initialize");
	}

	return pInstance;
}
