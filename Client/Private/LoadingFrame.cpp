#include "ClientPCH.h"
#include "LoadingFrame.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"

CLoadingFrame::CLoadingFrame(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CLoadingFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingFrame::Initialize(any _aDesc)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT) | BIT(COMPONENT::TRANSFORM);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_LOADING, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pBackground->Set_Scale({ g_iWinCX * 1.69f, g_iWinCY * 3.f, 1.f });
	m_pBackground->Set_State(TRANSFORM::POSITION, { 0.f, -30.f, 0.9f, 1.f });
	m_pBackground->Rotate(TRANSFORM::LOOK, XMConvertToRadians(-2.f));

	m_tMaterialDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);

	return S_OK;
}

void CLoadingFrame::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

//#ifdef _DEBUG
//	if (CImGui_Manager::Get_Instance()->Is_Enable())
//	{
//		ImGui::Begin("Loading Frame");
//
//		_float4x4 matBackGround = m_pBackground->Get_WorldMatrix();
//
//		_bool bTransformDirtyFlag = false;
//		_vector vScale, vQuaternion, vTranslation;
//
//		_float3 vNewPosition;
//		_float3 vNewRotation;
//		_float3 vNewScale;
//		if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, matBackGround))
//		{
//			ImGui::SeparatorText("BackGround Transform");
//			vNewPosition = vTranslation;
//			if (ImGui::DragFloat3("BackGround Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
//			if (ImGui::DragFloat3("BackGround Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewScale = vScale;
//			if (ImGui::DragFloat3("BackGround Scale", &vNewScale.x, 0.005f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//		}
//
//		if (bTransformDirtyFlag)
//		{
//			_float4x4 mNewWorld = XMMatrixAffineTransformation(vNewScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
//				_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);
//
//			m_pBackground->Set_Matrix(mNewWorld);
//		}
//
//		ImGui::End();
//	}
//#endif
}

void CLoadingFrame::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CLoadingFrame::Render()
{
	if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CTexture::Bind_OnShader");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_MTRLDIF, &m_tMaterialDesc.vDiffuse, sizeof(_float4))))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to Bind Material");
	}

	if (FAILED(m_pFrameTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}
	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CLoadingFrame::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pBackground = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pBackground)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pBackground->Set_Matrix(g_mUnit);

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: CVIBuffer_Rect");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pFrameTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Loading/T_ef_ev_m09s01010_waku_001.dds"));
	if (nullptr == m_pFrameTexture)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::CTexture::Create", "Failed to CTexture::Create");
	}

	return S_OK;
}

shared_ptr<CLoadingFrame> CLoadingFrame::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CLoadingFrame> pInstance = make_private_shared(CLoadingFrame, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CLoadingFrame::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CLoadingFrame::Clone(any)
{
	return shared_ptr<CGameObject>();
}
