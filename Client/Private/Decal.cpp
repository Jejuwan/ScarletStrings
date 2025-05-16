#include "ClientPCH.h"
#include "Decal.h"

#include "GameInstance.h"
#include "ObjectPool.h"

CDecal::CDecal(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CDecal::CDecal(const CDecal& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDecal::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_CUBE);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX_DECAL, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_CUBE] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_CUBE, g_aNull);

	return S_OK;
}

HRESULT CDecal::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CDecal::Initialize", "Failed to CGameObject::Initialize");
	}

	m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);

	return S_OK;
}

void CDecal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

//#ifdef _DEBUG
//#ifdef ACTIVATE_IMGUI
//	ImGui::Begin("Decal Transform");
//	{
//		_float4x4 mDecalMatrix = m_pTransform->Get_WorldMatrix();
//
//		_bool bTransformDirtyFlag = false;
//		_vector vScale, vQuaternion, vTranslation;
//
//		_float3 vNewPosition;
//		_float3 vNewRotation;
//		_float3 vNewScale;
//		if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mDecalMatrix))
//		{
//			ImGui::SeparatorText("Decal Transform");
//			vNewPosition = vTranslation;
//			if (ImGui::DragFloat3("Decal Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
//			if (ImGui::DragFloat3("Decal Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewScale = vScale;
//			if (ImGui::DragFloat3("Decal Scale", &vNewScale.x, 0.005f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//		}
//
//		if (bTransformDirtyFlag)
//		{
//			_float4x4 mNewWorld = XMMatrixAffineTransformation(vNewScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
//				_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);
//
//			m_pTransform->Set_Matrix(mNewWorld);
//		}
//	}
//	ImGui::End();
//#endif
//#endif
}

void CDecal::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CDecal::Render()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_OnShader: Transform");
	}

	auto pPipeLine = CPipeLine::Get_Instance();
	if (FAILED(m_pShader->Bind_Matrix("g_mWorldInv", XMMatrixInverse(nullptr, m_pTransform->Get_Matrix()))))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_Matrix: g_mWorldInv");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEWINV, XMMatrixInverse(nullptr, pPipeLine->Get_Transform(PIPELINE::VIEW)))))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_Matrix: SHADER_MATVIEWINV");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJINV, XMMatrixInverse(nullptr, pPipeLine->Get_Transform(PIPELINE::PROJECTION)))))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_Matrix: SHADER_MATPROJINV");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pDepthTarget = pGameInstance->Get_RenderTarget_ShaderResourceView(RENDERTARGET_DEPTH);
	if (FAILED(m_pShader->Bind_ShaderResourceView("g_texDepthTarget", pDepthTarget)))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_ShaderResourceView: DepthTarget");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_MTRLDIF, &m_tMaterialDesc.vDiffuse, sizeof(_float4))))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(m_pVIBuffer_Cube->Render(m_pShader, 0)))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Render");
	}

	return S_OK;
}

HRESULT CDecal::Fetch(any)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CDecal::Fetch", "Failed to CGameObject::Fetch");
	}

	return S_OK;
}

HRESULT CDecal::Ready_Components()
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

	m_pVIBuffer_Cube = Get_Component<CVIBuffer_Cube>(COMPONENT::VIBUFFER_CUBE);
	if (nullptr == m_pVIBuffer_Cube)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

HRESULT CDecal::Render(_uint iPassIndex)
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_OnShader: Transform");
	}

	auto pPipeLine = CPipeLine::Get_Instance();
	if (FAILED(m_pShader->Bind_Matrix("g_mWorldInv", XMMatrixInverse(nullptr, m_pTransform->Get_Matrix()))))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_Matrix: g_mWorldInv");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEWINV, XMMatrixInverse(nullptr, pPipeLine->Get_Transform(PIPELINE::VIEW)))))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_Matrix: SHADER_MATVIEWINV");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJINV, XMMatrixInverse(nullptr, pPipeLine->Get_Transform(PIPELINE::PROJECTION)))))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_Matrix: SHADER_MATPROJINV");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pDepthTarget = pGameInstance->Get_RenderTarget_ShaderResourceView(RENDERTARGET_DEPTH);
	if (FAILED(m_pShader->Bind_ShaderResourceView("g_texDepthTarget", pDepthTarget)))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_ShaderResourceView: DepthTarget");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_MTRLDIF, &m_tMaterialDesc.vDiffuse, sizeof(_float4))))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(m_pVIBuffer_Cube->Render(m_pShader, iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Render");
	}

//#ifdef _DEBUG
//	if (FAILED(m_pVIBuffer_Cube->Render(m_pShader, 1)))
//	{
//		MSG_RETURN(E_FAIL, "CDecal::Render", "Failed to Render");
//	}
//#endif

	return S_OK;
}
