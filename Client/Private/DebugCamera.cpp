#include "ClientPCH.h"
#include "DebugCamera.h"
#include "GameInstance.h"

#ifdef _DEBUG

CDebugCamera::CDebugCamera(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CDebugCamera::CDebugCamera(const CDebugCamera& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CDebugCamera::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_DEBUG_CINE_CAMERA, g_aNull);

	return S_OK;
}

HRESULT CDebugCamera::Initialize(any _mWorldMatrix)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CDebugCamera::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Matrix(std::any_cast<_float4x4>(_mWorldMatrix));
	//m_pTransform->Set_Scale(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

void CDebugCamera::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CDebugCamera::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::OUTLINE);
}

HRESULT CDebugCamera::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CDebugCamera::Render_OutLine()
{
	if (FAILED(__super::Render(2)))
	{
		MSG_RETURN(E_FAIL, "CDebugCamera::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

HRESULT CDebugCamera::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CDebugCamera::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDebugCamera::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CDebugCamera::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDebugCamera::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

shared_ptr<CDebugCamera> CDebugCamera::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CDebugCamera> pInstance = make_private_shared(CDebugCamera, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CDebugCamera::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CDebugCamera::Clone(any _mWorldMatrix)
{
	shared_ptr<CDebugCamera> pInstance = make_private_shared_copy(CDebugCamera, *this);

	if (FAILED(pInstance->Initialize(_mWorldMatrix)))
	{
		MSG_RETURN(nullptr, "CDebugCamera::Clone", "Failed to Initialize");
	}

	return pInstance;
}

#endif
