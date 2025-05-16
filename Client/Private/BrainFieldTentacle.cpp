#include "ClientPCH.h"
#include "BrainFieldTentacle.h"

#include "GameInstance.h"
#include "Player.h"

CBrainFieldTentacle::CBrainFieldTentacle(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CBrainFieldTentacle::CBrainFieldTentacle(const CBrainFieldTentacle& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CBrainFieldTentacle::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_PLAYER_BRAINFIELD_TENTACLE, g_aNull);

	return S_OK;
}

HRESULT CBrainFieldTentacle::Initialize(any matCombineBone)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CPlayerTentacle::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pCombineBone = any_cast<const _float4x4*>(matCombineBone);
	m_matPivot = XMMatrixIdentity();

	return S_OK;
}

void CBrainFieldTentacle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModel->Tick_Animation(fTimeDelta);

#ifdef _DEBUG
	ImGui::Begin("Tentacle Pivot");
	{
		_bool bTransformDirtyFlag = false;
		_vector vScale, vQuaternion, vTranslation;

		_float3 vNewScale;
		_float3 vNewPosition;
		_float3 vNewRotation;
		if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, m_matPivot))
		{
			ImGui::SeparatorText("Tentacle Transform");
			vNewPosition = vTranslation;
			if (ImGui::DragFloat3("Tentacle Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;
			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
			if (ImGui::DragFloat3("Tentacle Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;
			vNewScale = vScale;
			if (ImGui::DragFloat3("Tentacle Scale", &vNewScale.x, 0.005f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;
		}
		
		if (bTransformDirtyFlag)
		{
			m_matPivot = XMMatrixAffineTransformation(vNewScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
				_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);
		}
	}
	ImGui::End();
#endif // _DEBUG

	auto pPlayerTransform = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_pTransform->Set_Matrix(m_matPivot * XMLoadFloat4x4(m_pCombineBone) * m_pModel->Get_Pivot() * pPlayerTransform->Get_Matrix());
}

void CBrainFieldTentacle::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_bDissolve)
	{
		Add_RenderObject(RENDER_GROUP::NEON);
		Add_RenderObject(RENDER_GROUP::NONBLEND);

		m_fDissolveFactor += fTimeDelta * 0.35f;
		//if (m_fDissolveFactor > 1.f)
		//	m_bDissolve = false;
	}
	else
	{
		if (m_bEmissiveMode)
			Add_RenderObject(RENDER_GROUP::NONBLEND);
		else
			Add_RenderObject(RENDER_GROUP::NEON);
	}
}

HRESULT CBrainFieldTentacle::Render()
{
	if (m_bEmissiveMode)
	{
		if (FAILED(m_pShader->Bind_RawValue("g_vGradientStartColor", &m_vRootColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Render", "Failed to Bind_RawValue: RootColor");
		}
		if (FAILED(m_pShader->Bind_RawValue("g_vGradientEndColor", &m_vTipColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Render", "Failed to Bind_RawValue: TipColor");
		}

		if (FAILED(m_pShader->Bind_Float("g_fGradientFactor", m_fZFactor)))
		{
			MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Render", "Failed to Bind_Float: GradientFactor");
		}

		if (FAILED(__super::Render(27)))
		{
			MSG_RETURN(E_FAIL, "CPlayerTentacle::Render", "Failed to CGameObject::Render");
		}
	}
	else
	{
		if (FAILED(__super::Render(0)))
		{
			MSG_RETURN(E_FAIL, "CPlayerTentacle::Render", "Failed to CGameObject::Render");
		}
	}


	return S_OK;
}

HRESULT CBrainFieldTentacle::Render_Neon()
{
	if (m_bDissolve)
	{
		if (FAILED(m_pNoiseTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_vLineNoiseTexture")))
		{
			MSG_RETURN(E_FAIL, "CCutsceneTentacle::Render_Neon", "Failed to CGameObject::Bind_ShaderResourceView");
		}

		if (FAILED(m_pShader->Bind_Float("g_fThreshold", m_fDissolveFactor)))
		{
			MSG_RETURN(E_FAIL, "CCutsceneTentacle::Render_Neon", "Failed to CGameObject::Bind_Float");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue("g_vStripeColor", &m_vStripeColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CCutsceneTentacle::Render_Neon", "Failed to CGameObject::Bind_RawValue");
	}

	if (FAILED(__super::Render(m_iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CCutsceneTentacle::Render_Neon", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CBrainFieldTentacle::Render_Screen_Effect()
{
	return S_OK;
}

void CBrainFieldTentacle::Dissolve_Stripe()
{
	m_bDissolve = true;
	m_iPassIndex = 25;
}

void CBrainFieldTentacle::Set_Pivot(const _float4x4& matPivot)
{
	m_matPivot = matPivot;
}

void CBrainFieldTentacle::EmissiveMode()
{
	m_bDissolve = false;
	m_iPassIndex = 27;

	m_bEmissiveMode = true;
	
	//m_tMaterialDesc.vEmissive = _float4(1.5f, 0.f, 0.f, 1.f);
	//for (_uint i = 0; i < m_pModel->Get_NumMeshes(); ++i)
	//{
	//	SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//	tShaderDesc.tMaterialDesc.vEmissive = _float4(1.5f, 0.f, 0.f, 1.f);

	//	m_pModel->Set_ShaderDesc(i, tShaderDesc);
	//}
}

HRESULT CBrainFieldTentacle::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	m_pNoiseTexture = CGameInstance::Get_Instance()->Clone_Component<CTexture>(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_002);
	if (nullptr == m_pNoiseTexture)
	{
		MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Ready_Components", "Failed to Get_Component: Noise Texture");
	}
	
	return S_OK;
}

shared_ptr<CBrainFieldTentacle> CBrainFieldTentacle::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CBrainFieldTentacle> pInstance = make_private_shared(CBrainFieldTentacle, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CBrainFieldTentacle::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CBrainFieldTentacle::Clone(any matCombineBone)
{
	shared_ptr<CBrainFieldTentacle> pInstance = make_private_shared_copy(CBrainFieldTentacle, *this);

	if (FAILED(pInstance->Initialize(matCombineBone)))
	{
		MSG_RETURN(nullptr, "CBrainFieldTentacle::Clone", "Failed to Initialize");
	}

	return pInstance;
}
