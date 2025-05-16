#include "ClientPCH.h"
#include "Gemma.h"
#include "GameInstance.h"

#include "Client_Animation.h"

CGemma::CGemma(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCharacter(_pDevice, _pContext)
{
}

CGemma::CGemma(const CGemma& _rhs)
	: CCharacter(_rhs)
{
}

HRESULT CGemma::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_GEMMA, g_aNull);

	return S_OK;
}

HRESULT CGemma::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CGemma::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pModel->Set_Animation(0);
	m_pTransform->Set_State(TRANSFORM::POSITION, _float4(-1.5f, 0.f, 0.5f, 1.f));

	m_vecMeshPassIndexPair.push_back({ 0, 17 });
	m_vecMeshPassIndexPair.push_back({ 1, 17 });
	m_vecMeshPassIndexPair.push_back({ 4, 17 });
	m_vecMeshPassIndexPair.push_back({ 5, 17 });

	m_vecMeshPassIndexPair.push_back({ 2, 0 });
	m_vecMeshPassIndexPair.push_back({ 3, 0 });
	m_vecMeshPassIndexPair.push_back({ 6, 0 });
	m_vecMeshPassIndexPair.push_back({ 7, 0 });
	m_vecMeshPassIndexPair.push_back({ 8, 0 });

	return S_OK;
}

void CGemma::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModel->Tick_Animation(fTimeDelta);

#ifdef _DEBUG
	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("Gemma"))
	{
	if (ImGui::BeginMenu("Animation Settings"))
	{
	ImGui::Text("Gemma Animation Index : %d", m_iCurrentAnimIndex);

	ImGui::SameLine();
	if (ImGui::ArrowButton("Gemma Left", ImGuiDir_Left))
	{
		--m_iCurrentAnimIndex;
		if (m_iCurrentAnimIndex < 0)
			m_iCurrentAnimIndex = _int(ANIMATION::GEMMA::MAX) - 1;

		m_pModel->Set_Animation(m_iCurrentAnimIndex);
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("Gemma Right", ImGuiDir_Right))
	{
		m_iCurrentAnimIndex++;
		if (m_iCurrentAnimIndex >= _int(ANIMATION::GEMMA::MAX))
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
			ImGui::SeparatorText("Gemma Transform");
			vNewPosition = vTranslation;
			if (ImGui::DragFloat3("Gemma Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;
			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
			if (ImGui::DragFloat3("Gemma Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
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




	// Material Settings
	/*ImGui::Begin("Material Settings");
	if (ImGui::CollapsingHeader("Gemma Material"))
	{
		_uint iNumMesh = m_pModel->Get_NumMeshes();
		for (_uint i = 0; i < iNumMesh; ++i)
		{
			string MeshName = m_pModel->Get_MeshName(i);
			if (ImGui::TreeNode(MeshName.c_str()))
			{
				SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
				_bool bDirtyFlag = false;
				if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
					bDirtyFlag = true;

				if (bDirtyFlag)
				{
					m_pModel->Set_ShaderDesc(i, tShaderDesc);
				}

				ImGui::TreePop();
			}
		}

		if (ImGui::Button("Save"))
		{
			m_pModel->Export_ShaderMaterial();
		}
	}

	ImGui::End();*/
#endif
}

void CGemma::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CGemma::Render()
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

	return S_OK;
}

void CGemma::Move_Mouth()
{
}

void CGemma::Stop_Mouth()
{
}

HRESULT CGemma::Render_OutLine()
{
	return S_OK;
}

HRESULT CGemma::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CGemma::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CGemma> CGemma::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CGemma> pInstance = make_private_shared(CGemma, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CGemma::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CGemma::Clone(any)
{
	shared_ptr<CGemma> pInstance = make_private_shared_copy(CGemma, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CGemma::Clone", "Failed to Initialize");
	}

	return pInstance;
}
