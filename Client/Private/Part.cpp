#include "ClientPCH.h"
#include "Part.h"
#include "Player.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Model.h"
#include "Effect_Manager.h"

CPart::CPart(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CPart::CPart(const CPart& _rhs)
	:CGameObject(_rhs)
{
}

CPart::~CPart()

{
}

HRESULT CPart::Initialize_Prototype(const wstring& ModelTag)
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
    //m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
    m_umapComponentArg[COMPONENT::MODEL] = make_pair(ModelTag, g_aNull);

    return S_OK;
}

HRESULT CPart::Initialize(any pArg)
{
	PARTDESC Desc = any_cast<PARTDESC>(pArg);
	m_bGlitch = Desc.bGlitch;
	m_bNeon = Desc.bNeon;

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTwinceBox::Initialize", "Failed to CGameObject::Initialize");
	}

	m_PivotMatrix = Desc.PivotMatrix ;
	//m_PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_pParentMatrix = Desc.pParentMatrix;
	m_pWeakBoneMatrix = Desc.pWeakBoneMatrix;
	m_PlusMatrix = Desc.PlusMatrix;
	m_pTexDissolve = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_043.dds"), 1, true);

	return S_OK;
}

void CPart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix BoneMatrix = XMLoadFloat4x4(m_pWeakBoneMatrix);
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
	
	_matrix ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
	
	m_pTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f) 
		* m_PlusMatrix 
		* BoneMatrix
		* m_PivotMatrix
		* ParentMatrix
	);

	if (m_bDissolve)
	{
		m_fDissolveThreshold += fTimeDelta / m_fDissolveDuration;
	}

	if (m_bGlitch)
	{
		m_fSeedAccTime += fTimeDelta;
		if (m_fSeedAccTime >= m_fSeedInterval)
		{
			m_fSeedAccTime = 0.f;
			m_fGlitchSeed += fTimeDelta;
		}
	}

	// Material Settings
	/*string MeshName = m_pModel->Get_MeshName(0);
	string ModelTag = "Parts Material##" + MeshName;
	ImGui::Begin("Material Settings");
	if (ImGui::CollapsingHeader(ModelTag.c_str()))
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
}

void CPart::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	if (m_bGlitch)
		Add_RenderObject(RENDER_GROUP::SCREEN_EFFECT);
	if (m_bNeon)
		Add_RenderObject(RENDER_GROUP::NEON);
}

HRESULT CPart::Render()
{
	if (!m_bDissolve)
	{
		if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render", "Failed to Bind_LightDirection");
		}

		if (FAILED(__super::Render(15)))
		{
			MSG_RETURN(E_FAIL, "CTwinceBox::Render", "Failed to CGameObject::Render");
		}
	}
	else
	{
		if (FAILED(m_pShader->Bind_Float("g_fThreshold", m_fDissolveThreshold))) 
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CShader::Bind_Float");
		}

		if ((m_pShader->Bind_ShaderResourceViews(SHADER_TEXDISSOLVE, m_pTexDissolve->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CShader::Bind_ShaderResourceView");
		}

		if (FAILED(CGameObject::Render(17)))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CGameObject::Render");
		}
	}
	return S_OK;
}

HRESULT CPart::Render_Neon()
{
	m_pShader->Bind_Float("g_fAccTime", 0.f);
	if (FAILED(__super::Render(14)))
	{
		MSG_RETURN(E_FAIL, "CTwinceBox::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CPart::Render_Screen_Effect()
{
	if (FAILED(m_pGlitchNoise->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_vLineNoiseTexture")))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render_Screen_Effect", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pShader->Bind_Float("g_fSeed", m_fGlitchSeed)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render_Screen_Effect", "Failed to GlitchSeed");
	}

	if (FAILED(__super::Render(19)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CPart::Render_OutLine()
{
	return S_OK;
}

void CPart::Dissolve()
{
	m_bDissolve = true;
	m_bNeon = false;
	m_fDissolveThreshold = 0.f;

	//CEffect_Manager::Get_Instance()->Fetch(VFX::FADE_PARTICLE, shared_from_gameobject());
}

void CPart::BlackMaskPivotChance()
{
	m_PlusMatrix *= XMMatrixRotationZ(XMConvertToRadians(180.f));
}

HRESULT CPart::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTwinceBox::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTwinceBox::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTwinceBox::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CTwinceBox::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	if (m_bGlitch)
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		m_pGlitchNoise = pGameInstance->Clone_Component<CTexture>(pGameInstance->Current_Scene(), PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_014);
		if (nullptr == m_pGlitchNoise)
		{
			MSG_RETURN(E_FAIL, "CPart::Ready_Components", "Failed to Clone_Component: NoiseTexture");
		}
	}

	return S_OK;
}

shared_ptr<CPart> CPart::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& ModelTag)
{
	shared_ptr<CPart> pInstance = make_private_shared(CPart, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(ModelTag)))
	{
		MSG_RETURN(nullptr, "CTwinceBox::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CPart::Clone(any pArg)
{
	shared_ptr<CPart> pInstance = make_private_shared_copy(CPart, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CTwinceBox::Clone", "Failed to Initialize");
	}

	return pInstance;
}
