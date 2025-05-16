#include "ClientPCH.h"
#include "PointGizmo.h"
#include "GameInstance.h"

CPointGizmo::CPointGizmo(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CPointGizmo::CPointGizmo(const CPointGizmo& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPointGizmo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPointGizmo::Initialize(any)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_SPHERE);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_SPHERE] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_SPHERE, g_aNull);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Scale(XMVectorSet(0.3f, 0.3f, 0.3f, 0.f));

	return S_OK;
}

void CPointGizmo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPointGizmo::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::OUTLINE);
}

HRESULT CPointGizmo::Render()
{
	if (FAILED(m_pShader->Bind_RawValue("vGizmoColor", &m_vGizmoColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Render", "Failed to Bind_RawValue: GizmoColor");
	}

	if (FAILED(__super::Render(5)))
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CPointGizmo::Render_OutLine()
{
	if (FAILED(m_pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutLineColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Render_OutLine", "Failed to Bind_RawValue");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Render_OutLine", "Failed to Bind_Float");
	}

	if (FAILED(__super::Render(2)))
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

HRESULT CPointGizmo::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pVIBuffer_Sphere = Get_Component<CVIBuffer_Sphere>(COMPONENT::VIBUFFER_SPHERE);
	if (nullptr == m_pVIBuffer_Sphere)
	{
		MSG_RETURN(E_FAIL, "CPointGizmo::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

shared_ptr<CPointGizmo> CPointGizmo::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CPointGizmo> pInstance = make_private_shared(CPointGizmo, pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CPointGizmo::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CPointGizmo::Clone(any)
{
	return shared_from_gameobject();
}
