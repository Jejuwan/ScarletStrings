#include "ClientPCH.h"
#include "TitleCharacter.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"

CTitleCharacter::CTitleCharacter(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTitleCharacter::CTitleCharacter(const CTitleCharacter& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTitleCharacter::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);

	return S_OK;
}

HRESULT CTitleCharacter::Initialize(any tTitleCharacterDesc)
{
	m_tTitleCharacterDesc = any_cast<TITLECHARACTERDESC>(tTitleCharacterDesc);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(m_tTitleCharacterDesc.wszModelTag, g_aNull);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Matrix(m_tTitleCharacterDesc.matTransform);

	_uint iNumMesh = m_pModel->Get_NumMeshes();
	for (_uint i = 0; i < iNumMesh; ++i)
	{
		SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
		tShaderDesc.tMaterialDesc.vDiffuse = m_tTitleCharacterDesc.vMtrlDiffuse;

		m_pModel->Set_ShaderDesc(i, tShaderDesc);
	}

	m_pModel->Set_Animation(m_tTitleCharacterDesc.iAnimIndex, 2.f, false, 0.f, true);

	return S_OK;
}

void CTitleCharacter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModel->Tick_Animation(fTimeDelta);

//#ifdef _DEBUG
//	if (CImGui_Manager::Get_Instance()->Is_Enable() && m_tTitleCharacterDesc.wszModelTag == PROTOTYPE_COMPONENT_MODEL_YUITO)
//	{
//		ImGui::Begin("Tramsform Batcher");
//		_float4x4 mChracterMatrix = m_pTransform->Get_WorldMatrix();
//
//		_bool bTransformDirtyFlag = false;
//		_vector vScale, vQuaternion, vTranslation;
//
//		_float3 vNewPosition;
//		_float3 vNewRotation;
//		_float3 vNewScale;
//		if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mChracterMatrix))
//		{
//			ImGui::SeparatorText("Kasane Transform");
//			vNewPosition = vTranslation;
//			if (ImGui::DragFloat3("Kasane Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
//			if (ImGui::DragFloat3("Kasane Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewScale = vScale;
//			if (ImGui::DragFloat3("Kasane Scale", &vNewScale.x, 0.005f, -FLT_MAX, +FLT_MAX))
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
//		ImGui::End();
//
//		// Material Settings
//		ImGui::Begin("Material Settings");
//		if (ImGui::CollapsingHeader("Kasane Material"))
//		{
//			_uint iNumMesh = m_pModel->Get_NumMeshes();
//			for (_uint i = 0; i < iNumMesh; ++i)
//			{
//				string MeshName = m_pModel->Get_MeshName(i);
//				if (ImGui::TreeNode(MeshName.c_str()))
//				{
//					SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
//					_bool bDirtyFlag = false;
//					if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
//						bDirtyFlag = true;
//					if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
//						bDirtyFlag = true;
//					if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
//						bDirtyFlag = true;
//					if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
//						bDirtyFlag = true;
//					if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
//						bDirtyFlag = true;
//
//					if (bDirtyFlag)
//					{
//						m_pModel->Set_ShaderDesc(i, tShaderDesc);
//					}
//
//					ImGui::TreePop();
//				}
//			}
//		}
//
//		ImGui::End();
//	}
//#endif
}

void CTitleCharacter::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CTitleCharacter::Render()
{
	if (FAILED(Bind_Transform(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CGemma::Render", "Failed to CGameObject::Bind_Transform");
	}

	for (_uint i = 0; i < m_tTitleCharacterDesc.vecMeshPassIndexPair.size(); ++i)
	{
		_uint iMeshIndex = m_tTitleCharacterDesc.vecMeshPassIndexPair[i].first;
		_uint iPassIndex = m_tTitleCharacterDesc.vecMeshPassIndexPair[i].second;
		if (FAILED(m_pModel->Bind_ShaderResourceViews(iMeshIndex, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CGemma::Render", "Failed to Bind_ShaderResourceViews");
		}
		if (FAILED(m_pModel->Bind_ShaderOptions(iMeshIndex, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CGemma::Render", "Failed to Bind_ShaderOptions");
		}
		if (FAILED(m_pModel->Bind_BoneMatrices(iMeshIndex, m_pShader, SHADER_BONE)))
		{
			MSG_RETURN(E_FAIL, "CGemma::Render", "Failed to Bind_BoneMatrices");
		}

		if (FAILED(m_pModel->Render(iMeshIndex, m_pShader, iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CGemma::Render", "Failed to Render Model");
		}
	}

	return S_OK;
}

HRESULT CTitleCharacter::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CKasane::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: Model");
	}

	return S_OK;
}

shared_ptr<CTitleCharacter> CTitleCharacter::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, any)
{
	shared_ptr<CTitleCharacter> pInstance = make_private_shared(CTitleCharacter, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTitleCharacter::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTitleCharacter::Clone(any tTitleCharacterDesc)
{
	shared_ptr<CTitleCharacter> pInstance = make_private_shared_copy(CTitleCharacter, *this);

	if (FAILED(pInstance->Initialize(tTitleCharacterDesc)))
	{
		MSG_RETURN(nullptr, "CTitleCharacter::Clone", "Failed to Initialize");
	}

	return pInstance;
}
