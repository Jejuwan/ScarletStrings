#include "ClientPCH.h"
#include "Shiden.h"
#include "GameInstance.h"

#include "Client_Animation.h"

CShiden::CShiden(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCharacter(_pDevice, _pContext)
{
}

CShiden::CShiden(const CShiden& _rhs)
	: CCharacter(_rhs)
{
}

HRESULT CShiden::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SHIDEN, g_aNull);

	return S_OK;
}

HRESULT CShiden::Initialize(any SASMode)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CShiden::Initialize", "Failed to CGameObject::Initialize");
	}

	// Hide Mask && Hood
	m_pModel->Hide_Mesh(4);
	m_pModel->Hide_Mesh(5);

	m_vecMeshPassIndexPair.push_back({ 1, 17 });
	m_vecMeshPassIndexPair.push_back({ 2, 17 });
	m_vecMeshPassIndexPair.push_back({ 3, 17 });

	m_vecMeshPassIndexPair.push_back({ 0, 0 });
	m_vecMeshPassIndexPair.push_back({ 6, 0 });
	m_vecMeshPassIndexPair.push_back({ 7, 0 });
	m_vecMeshPassIndexPair.push_back({ 8, 0 });

	m_bSASMode = any_cast<_bool>(SASMode);

	return S_OK;
}

void CShiden::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bSASMode)
	{
		Tick_SAS(10, fTimeDelta);
	}
	else
	{
		m_pModel->Tick_Animation(fTimeDelta);
	}


#ifdef _DEBUG
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Shiden"))
		{
			if (ImGui::BeginMenu("Animation Settings"))
			{
				ImGui::Text("Shiden Animation Index : %d", m_iCurrentAnimIndex);

				ImGui::SameLine();
				if (ImGui::ArrowButton("Shiden Left", ImGuiDir_Left))
				{
					--m_iCurrentAnimIndex;
					if (m_iCurrentAnimIndex < 0)
						m_iCurrentAnimIndex = _int(ANIMATION::SHIDEN::MAX) - 1;

					m_pModel->Set_Animation(m_iCurrentAnimIndex);
				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("Shiden Right", ImGuiDir_Right))
				{
					m_iCurrentAnimIndex++;
					if (m_iCurrentAnimIndex >= _int(ANIMATION::SHIDEN::MAX))
						m_iCurrentAnimIndex = 0;

					m_pModel->Set_Animation(m_iCurrentAnimIndex);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tramsform Batcher"))
			{
				_float4x4 mChracterMatrix = m_pTransform->Get_WorldMatrix();

				_bool bTransformDirtyFlag = false;
				_vector vScale, vQuaternion, vTranslation;

				_float3 vNewPosition;
				_float3 vNewRotation;
				if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mChracterMatrix))
				{
					ImGui::SeparatorText("Shiden Transform");
					vNewPosition = vTranslation;
					if (ImGui::DragFloat3("Shiden Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
						bTransformDirtyFlag = true;
					vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
					if (ImGui::DragFloat3("Shiden Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
						bTransformDirtyFlag = true;
				}

				if (bTransformDirtyFlag)
				{
					_float4x4 mNewWorld = XMMatrixAffineTransformation(vScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
						_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);

					m_pTransform->Set_Matrix(mNewWorld);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//// Material Settings
	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("Shiden Material"))
	//{
	//	_uint iNumMesh = m_pModel->Get_NumMeshes();
	//	for (_uint i = 0; i < iNumMesh; ++i)
	//	{
	//		string MeshName = m_pModel->Get_MeshName(i);
	//		if (ImGui::TreeNode(MeshName.c_str()))
	//		{
	//			SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//			_bool bDirtyFlag = false;
	//			if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
	//				bDirtyFlag = true;

	//			if (bDirtyFlag)
	//			{
	//				m_pModel->Set_ShaderDesc(i, tShaderDesc);
	//			}

	//			ImGui::TreePop();
	//		}
	//	}

	//	if (ImGui::Button("Save"))
	//	{
	//		m_pModel->Export_ShaderMaterial();
	//	}
	//}

	//ImGui::End();
#endif
}

void CShiden::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//Add_RenderObject(RENDER_GROUP::OUTLINE);

	if (m_bExecuteSAS || m_bDissolve)
	{
		Add_RenderObject(RENDER_GROUP::OUTLINE);
	}
}

HRESULT CShiden::Render()
{
	if (m_bSASMode == false)
	{
		if (FAILED(Bind_Transform(m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CGemma::Render", "Failed to CGameObject::Bind_Transform");
		}

		for (_uint i = 0; i < m_vecMeshPassIndexPair.size(); ++i)
		{
			_uint iMeshIndex = m_vecMeshPassIndexPair[i].first;
			_uint iPassIndex = m_vecMeshPassIndexPair[i].second;
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
	}

	if (m_bExecuteSAS || m_bDissolve)
	{
		return Render_SAS_Cutscene();
	}

	return S_OK;
}

void CShiden::Move_Mouth()
{
	m_pModel->Set_Animation(3);
	m_pModel->Tick_Animation(5.f);
	m_pTransform->Rotate(TRANSFORM::UP, XMConvertToRadians(30));
}

void CShiden::Stop_Mouth()
{
	m_pModel->Set_Animation(0);
}

void CShiden::Execute_SAS(const std::function<void()>& fpFinished)
{
	m_bExecuteSAS = true;
	m_pModel->Set_Animation(10, 2.f, false, 0.f, false);

	m_vOutLineColor = _float3(1.0f, 0.86f, 0.7f);
	m_vAdditiveColor = _float3(1.0f, 0.f, 0.f);

	m_iDissolve = 0;
	m_bDissolve = false;

	_matrix matTranslationMatrix = XMMatrixTranslationFromVector(XMVectorSet(0.02f, 0.0f, 0.5f, 1.f));
	_matrix matRotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	_matrix matWorldMatrix = matRotationMatrix * matTranslationMatrix;

	m_pTransform->Set_Matrix(matWorldMatrix);

	m_fpFinished = fpFinished;
}

HRESULT CShiden::Render_OutLine()
{
	//if (FAILED(__super::Render(3)))
	//{
	//	MSG_RETURN(E_FAIL, "CKyoka::Render", "Failed to CGameObject::Render");
	//}

	if (m_bExecuteSAS)
	{
		return Render_SAS_Cutscene_Outline();
	}

	return S_OK;
}

HRESULT CShiden::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CShiden::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CShiden> CShiden::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CShiden> pInstance = make_private_shared(CShiden, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CShiden::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CShiden::Clone(any SASMode)
{
	shared_ptr<CShiden> pInstance = make_private_shared_copy(CShiden, *this);

	if (FAILED(pInstance->Initialize(SASMode)))
	{
		MSG_RETURN(nullptr, "CShiden::Clone", "Failed to Initialize");
	}

	return pInstance;
}
