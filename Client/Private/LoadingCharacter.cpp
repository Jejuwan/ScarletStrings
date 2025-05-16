#include "ClientPCH.h"
#include "LoadingCharacter.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"

CLoadingCharacter::CLoadingCharacter(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CLoadingCharacter::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingCharacter::Initialize(any tLoadingCharcterDesc)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);

	m_tLoadingCharcterDesc = any_cast<LOADINGCHARACTERDESC>(tLoadingCharcterDesc);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Matrix(m_tLoadingCharcterDesc.matTransform);
	m_matView = XMMatrixIdentity();
	m_matProjection = XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pModel->Set_Animation(m_tLoadingCharcterDesc.iWalkAnimIndex, 2.f, false, 0.f, true);

	return S_OK;
}

void CLoadingCharacter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModel->Tick_Animation(fTimeDelta);

//#ifdef _DEBUG
//	if (CImGui_Manager::Get_Instance()->Is_Enable())
//	{
//		int size = WideCharToMultiByte(CP_UTF8, 0, m_tLoadingCharcterDesc.wszModelPath, -1, nullptr, 0, nullptr, nullptr);
//		std::string strModelTag(size, 0);
//		WideCharToMultiByte(CP_UTF8, 0, m_tLoadingCharcterDesc.wszModelPath, -1, &strModelTag[0], size, nullptr, nullptr);
//
//		ImGui::Begin(strModelTag.c_str());
//
//		_float4x4 matBackGround = m_pTransform->Get_WorldMatrix();
//
//		_bool bTransformDirtyFlag = false;
//		_vector vScale, vQuaternion, vTranslation;
//
//		_float3 vNewPosition;
//		_float3 vNewRotation;
//		_float3 vNewScale;
//		if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, matBackGround))
//		{
//			ImGui::SeparatorText("Character Transform");
//			vNewPosition = vTranslation;
//			if (ImGui::DragFloat3("Character Position", &vNewPosition.x, 1.f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
//			if (ImGui::DragFloat3("Character Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewScale = vScale;
//			if (ImGui::DragFloat3("Character Scale", &vNewScale.x, 1.0f, -FLT_MAX, +FLT_MAX))
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
//
//		ImGui::End();
//	}
//#endif
}

void CLoadingCharacter::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(UI_LAYER::LAYER_4);
}

HRESULT CLoadingCharacter::Render()
{
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CLoadingCharacter::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, m_matView)))
	{
		MSG_RETURN(E_FAIL, "CLoadingCharacter::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, m_matProjection)))
	{
		MSG_RETURN(E_FAIL, "CLoadingCharacter::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}

	if (FAILED(m_pLineNoiseTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_vLineNoiseTexture")))
	{
		MSG_RETURN(E_FAIL, "CLoadingCharacter::Bind_ShaderResourceView", "Failed to Bind LineNoiseTexture");
	}

	for (_uint i = 0; i < m_pModel->Get_NumMeshes(); ++i)
	{
		if (FAILED(m_pModel->Bind_BoneMatrices(i, m_pShader, SHADER_BONE)))
		{
			MSG_RETURN(E_FAIL, "CLoadingCharacter::Render", "Failed to Bind_BoneMatrices");
		}

		if (FAILED(m_pModel->Render(i, m_pShader, 26)))
		{
			MSG_RETURN(E_FAIL, "CKyoka::Render", "Failed to Render Model");
		}
	}

	return S_OK;
}

HRESULT CLoadingCharacter::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Ready_Components", "Failed to CGameObject::Ready_Components");
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

	m_pModel = CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, m_tLoadingCharcterDesc.wszModelPath);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to CModel::Create");
	}

	m_pLineNoiseTexture = CTexture::Create(m_pDevice, m_pContext, m_tLoadingCharcterDesc.wszLineNoiseTexturePath);
	if (nullptr == m_pLineNoiseTexture)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to CTexture::Create");
	}

	return S_OK;
}

shared_ptr<CLoadingCharacter> CLoadingCharacter::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, any tLoadingCharcterDesc)
{
	shared_ptr<CLoadingCharacter> pInstance = make_private_shared(CLoadingCharacter, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(tLoadingCharcterDesc)))
	{
		MSG_RETURN(nullptr, "CLoadingCharacter::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CLoadingCharacter::Clone(any)
{
	return shared_ptr<CGameObject>();
}
