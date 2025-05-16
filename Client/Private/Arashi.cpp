#include "ClientPCH.h"
#include "Arashi.h"
#include "GameInstance.h"

#include "Client_Animation.h"
#include "Pipeline.h"
#include "Camera.h"

CArashi::CArashi(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCharacter(_pDevice, _pContext)
{
}

CArashi::CArashi(const CArashi& _rhs)
	: CCharacter(_rhs)
{
}

HRESULT CArashi::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_ARASHI, g_aNull);

	return S_OK;
}

HRESULT CArashi::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CArashi::Initialize", "Failed to CGameObject::Initialize");
	}

	// Hide Mask && Hood
	m_pModel->Hide_Mesh(4);
	m_pModel->Hide_Mesh(5);

	return S_OK;
}

void CArashi::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Tick_SAS(0, fTimeDelta);

#ifdef _DEBUG
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Animation Settings"))
		{

			ImGui::Text("Arashi Animation Index : %d", m_iCurrentAnimIndex);

			ImGui::SameLine();
			if (ImGui::ArrowButton("Arashi Left", ImGuiDir_Left))
			{
				--m_iCurrentAnimIndex;
				if (m_iCurrentAnimIndex < 0)
					m_iCurrentAnimIndex = _int(ANIMATION::ARASHI::MAX) - 1;

				m_pModel->Set_Animation(m_iCurrentAnimIndex);
			}
			ImGui::SameLine();
			if (ImGui::ArrowButton("Arashi Right", ImGuiDir_Right))
			{
				m_iCurrentAnimIndex++;
				if (m_iCurrentAnimIndex >= _int(ANIMATION::ARASHI::MAX))
					m_iCurrentAnimIndex = 0;

				m_pModel->Set_Animation(m_iCurrentAnimIndex);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}




	// Material Settings
	/*ImGui::Begin("Material Settings");
	if (ImGui::CollapsingHeader("Arashi Material"))
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

void CArashi::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	
	if (m_bExecuteSAS || m_bDissolve)
	{
		Add_RenderObject(RENDER_GROUP::NONBLEND);
		Add_RenderObject(RENDER_GROUP::OUTLINE);
	}
}

HRESULT CArashi::Render()
{
	if (m_bExecuteSAS || m_bDissolve)
	{
		return Render_SAS_Cutscene();
	}
	
	return S_OK;
}

void CArashi::Move_Mouth()
{
}

void CArashi::Stop_Mouth()
{
}

void CArashi::Execute_SAS(const std::function<void()>& fpFinished)
{
	__super::Execute_SAS(fpFinished);

	_matrix matTranslationMatrix = XMMatrixTranslationFromVector(XMVectorSet(-0.07f, 0.f, 0.6482f, 1.f));
	_matrix matRotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.f - 25.f));
	_matrix matWorldMatrix = matRotationMatrix * matTranslationMatrix;

	m_pTransform->Set_Matrix(matWorldMatrix);
}

HRESULT CArashi::Render_OutLine()
{
	if (m_bExecuteSAS)
	{
		return Render_SAS_Cutscene_Outline();
	}
	
	return S_OK;
}

HRESULT CArashi::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CArashi::Ready_Components", "Failed to CGameObject::Ready_Components");
	}
	
	return S_OK;
}

shared_ptr<CArashi> CArashi::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CArashi> pInstance = make_private_shared(CArashi, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CArashi::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CArashi::Clone(any)
{
	shared_ptr<CArashi> pInstance = make_private_shared_copy(CArashi, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CArashi::Clone", "Failed to Initialize");
	}

	return pInstance;
}
