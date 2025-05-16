#include "ClientPCH.h"
#include "TestSphere.h"
#include "GameInstance.h"

CTestSphere::CTestSphere(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CInteractiveObject(_pDevice, _pContext)
{
}

CTestSphere::CTestSphere(const CTestSphere& _rhs)
	: CInteractiveObject(_rhs)
{
}

HRESULT CTestSphere::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_SPHERE);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_SPHERE] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_SPHERE, g_aNull);

	return S_OK;
}

HRESULT CTestSphere::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_State(TRANSFORM::POSITION, { 2,2,2,1 });

	return S_OK;
}

void CTestSphere::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CTestSphere::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::OUTLINE);
}

// Super Class Uses 2D Texture
// It Forces Users to Bind Texture Manually
// Usually must be set from prototype
HRESULT CTestSphere::Render()
{
	if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}

	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTestSphere::Render_OutLine()
{
	if (m_bFocused)
	{
		if (FAILED(__super::Render(2)))
		{
			MSG_RETURN(E_FAIL, "CTestCube::Render_OutLine", "Failed to CGameObject::Render_OutLine");
		}

		m_bFocused = false;
	}

	return S_OK;
}

HRESULT CTestSphere::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pTexture = CGameInstance::Get_Instance()->Clone_Component<CTexture>(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_BLACK);
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Ready_Components", "Failed to Clone_Component: PROTOTYPE_COMPONENT_TEXTURE_BLACK");
	}

	m_pVIBuffer_Sphere = Get_Component<CVIBuffer_Sphere>(COMPONENT::VIBUFFER_SPHERE);
	if (nullptr == m_pVIBuffer_Sphere)
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

shared_ptr<CTestSphere> CTestSphere::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTestSphere> pInstance = make_private_shared(CTestSphere, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTestSphere::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTestSphere::Clone(any)
{
	shared_ptr<CTestSphere> pInstance = make_private_shared_copy(CTestSphere, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTestSphere::Clone", "Failed to Initialize");
	}

	return pInstance;
}
