#include "ClientPCH.h"
#include "DecalTargetBuffer.h"

#include "GameInstance.h"

CDecalTargetBuffer::CDecalTargetBuffer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CDecalTargetBuffer::CDecalTargetBuffer(const CDecalTargetBuffer& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CDecalTargetBuffer::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX_DECAL, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	return S_OK;
}

HRESULT CDecalTargetBuffer::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CDecalTargetBuffer::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(90.f));
	m_pTransform->Set_Scale(XMVectorSet(100.f, 100.f, 1.f, 0.f));
	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.5f, 0.f, 1.f));

	return S_OK;
}

void CDecalTargetBuffer::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

#ifdef _DEBUG
	ImGui::Begin("Decal TargetBuffer");
	{
		_bool bTransformDirtyFlag = false;


		ImGui::SeparatorText("Decal TargetBuffer Transform");
		_float3 vNewPosition = m_pTransform->Get_State(TRANSFORM::POSITION);
		if (ImGui::DragFloat3("Decal TargetBuffer Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
			bTransformDirtyFlag = true;
		_float3 vNewScale = m_pTransform->Get_Scale();
		if (ImGui::DragFloat3("Decal TargetBuffer Scale", &vNewScale.x, 0.005f, -FLT_MAX, +FLT_MAX))
			bTransformDirtyFlag = true;

		if (bTransformDirtyFlag)
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, vNewPosition);
			m_pTransform->Set_Scale(vNewScale);
		}
	}
	ImGui::End();
#endif // _DEBUG
}

void CDecalTargetBuffer::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CDecalTargetBuffer::Render()
{
	if (FAILED(__super::Render(5)))
	{
		MSG_RETURN(E_FAIL, "CDecalTargetBuffer::Initialize", "Failed to CGameObject::Render");
	}

//#ifdef _DEBUG
//	if (FAILED(__super::Render(1)))
//	{
//		MSG_RETURN(E_FAIL, "CDecalTargetBuffer::Initialize", "Failed to CGameObject::Render");
//	}
//#endif

	return S_OK;
}

HRESULT CDecalTargetBuffer::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}
	m_pTransform->Set_Matrix(g_mUnit);

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: VIBUFFER_RECT");
	}

	return S_OK;
}

shared_ptr<CDecalTargetBuffer> CDecalTargetBuffer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CDecalTargetBuffer> pInstance = make_private_shared(CDecalTargetBuffer, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CDecalTargetBuffer::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CDecalTargetBuffer::Clone(any)
{
	shared_ptr<CDecalTargetBuffer> pInstance = make_private_shared_copy(CDecalTargetBuffer, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CDecalTargetBuffer::Clone", "Failed to Initialize");
	}

	return pInstance;
}
