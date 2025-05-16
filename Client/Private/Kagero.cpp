#include "ClientPCH.h"
#include "Kagero.h"
#include "GameInstance.h"

CKagero::CKagero(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCharacter(_pDevice, _pContext)
{
}

CKagero::CKagero(const CKagero& _rhs)
	: CCharacter(_rhs)
{
}

HRESULT CKagero::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_KAGERO, g_aNull);

	return S_OK;
}

HRESULT CKagero::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CKagero::Initialize", "Failed to CGameObject::Initialize");
	}

	// Hide Mask && Hood
	m_pModel->Hide_Mesh(4);
	m_pModel->Hide_Mesh(5);

	return S_OK;
}

void CKagero::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Tick_SAS(IDX(ANIMATION::KAGERO::SAS), fTimeDelta);

#ifdef _DEBUG
	// Material Settings
	/*ImGui::Begin("Material Settings");
	if (ImGui::CollapsingHeader("Kagero Material"))
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

void CKagero::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_bExecuteSAS || m_bDissolve)
	{
		Add_RenderObject(RENDER_GROUP::NONBLEND);
		Add_RenderObject(RENDER_GROUP::OUTLINE);
	}
}

HRESULT CKagero::Render()
{
	if (m_bExecuteSAS || m_bDissolve)
	{
		return Render_SAS_Cutscene();
	}

	return S_OK;
}

void CKagero::Move_Mouth()
{
}

void CKagero::Stop_Mouth()
{
}

void CKagero::Execute_SAS(const std::function<void()>& fpFinished)
{
	//__super::Execute_SAS(fpFinished);
	m_bExecuteSAS = true;
	m_pModel->Set_Animation(IDX(ANIMATION::KAGERO::SAS), 2.f, false, 0.f, false);

	m_vOutLineColor = _float3(1.0f, 0.86f, 0.7f);
	m_vAdditiveColor = _float3(1.0f, 0.f, 0.f);

	m_iDissolve = 0;
	m_bDissolve = false;

	m_fpFinished = fpFinished;

	_matrix matTranslationMatrix = XMMatrixTranslationFromVector(XMVectorSet(-0.07f, 0.f, 0.78f, 1.f));
	_matrix matRotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.f + 10.f));
	_matrix matWorldMatrix = matRotationMatrix * matTranslationMatrix;

	m_pTransform->Set_Matrix(matWorldMatrix);
}

HRESULT CKagero::Render_OutLine()
{
	if (m_bExecuteSAS)
	{
		return Render_SAS_Cutscene_Outline();
	}

	return S_OK;
}

HRESULT CKagero::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CKagero::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CKagero> CKagero::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CKagero> pInstance = make_private_shared(CKagero, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CKagero::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CKagero::Clone(any)
{
	shared_ptr<CKagero> pInstance = make_private_shared_copy(CKagero, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CKagero::Clone", "Failed to Initialize");
	}

	return pInstance;
}
