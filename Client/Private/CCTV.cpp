#include "ClientPCH.h"
#include "CCTV.h"

#include "GameInstance.h"
#include "Pipeline.h"
#include "Camera.h"
#include "Camera_Player.h"

#include "SceneCapture.h"

CCCTV::CCCTV(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CCCTV::CCCTV(const CCCTV& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCCTV::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT) | BIT(COMPONENT::TEXTURE);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE] = make_pair(PROTOTYPE_COMPONENT_TEXTURE_MASK_ONE_LINE, g_aNull);

	return S_OK;
}

HRESULT CCCTV::Initialize(any pParentTransform)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Initialize", "Failed to CGameObject::Initialize");
	}

	auto pPipeLine = CPipeLine::Get_Instance();
	auto tCameraDesc = pPipeLine->Get_Camera()->Get_Desc();
	m_mProjection = XMMatrixPerspectiveFovLH(tCameraDesc.fFovAngleY, tCameraDesc.fAspect, tCameraDesc.fNear, tCameraDesc.fFar);

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 0.86f, 1.f));
	m_pTransform->Set_Scale(XMVectorSet(1.8f, 1.f, 1.f, 0.f));

	m_pParentTransform = any_cast<shared_ptr<CTransform>>(pParentTransform);

	auto pCamLayer = CGameInstance::Get_Instance()->Find_Layer(SCENE::MIZUHA, LAYER_CAMERA);
	m_pCamera = dynamic_pointer_cast<CCamera>(pCamLayer->Get_GameObject(0));

	m_pCCTVPivotTransform = CTransform::Create(m_pDevice, m_pContext);
	m_pCCTVPivotTransform->Set_Matrix(XMMatrixAffineTransformation(XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
		_float3(XMConvertToRadians(40.f), XMConvertToRadians(-40.f), XMConvertToRadians(0.f))), XMVectorSet(1.f, 3.3f, -1.4f, 1.f)));

	return S_OK;
}

void CCCTV::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_fAccTime += _fTimeDelta * 0.01f;

	if (m_bActiveCCTV)
	{
		auto pCamTransform = dynamic_pointer_cast<CTransform>(	m_pCamera->Get_Component(COMPONENT::TRANSFORM));
		pCamTransform->Set_Matrix(m_pCCTVPivotTransform->Get_Matrix() * m_pParentTransform->Get_Matrix());
	}

#if ACTIVATE_IMGUI
	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("Tramsform Batcher"))
	{
		_float4x4 matCCTVMatrix = m_pCCTVPivotTransform->Get_WorldMatrix();

		_bool bTransformDirtyFlag = false;
		_vector vScale, vQuaternion, vTranslation;

		_float3 vNewPosition;
		_float3 vNewRotation;
		if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, matCCTVMatrix))
		{
			ImGui::SeparatorText("CCTV Transform");
			vNewPosition = vTranslation;
			if (ImGui::DragFloat3("CCTV Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;
			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
			if (ImGui::DragFloat3("CCTV Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;
		}

		if (bTransformDirtyFlag)
		{
			_float4x4 mNewWorld = XMMatrixAffineTransformation(vScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
				_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);

			m_pCCTVPivotTransform->Set_Matrix(mNewWorld);
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	}

	// TODO: Test Code
	{
		_float4x4 mTransform = m_pTransform->Get_Matrix();

		if (ImGui::BeginMainMenuBar())
		{
		if (ImGui::BeginMenu("Buffer Test"))
		{
		_bool bTransformDirtyFlag = false;

		_vector vScale, vQuaternion, vTranslation;
		_float3 vNewPosition;
		_float3 vNewRotation;
		_float3 vNewScale;

		if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mTransform))
		{
			ImGui::SeparatorText("Position");
			vNewPosition = vTranslation;
			if (ImGui::DragFloat3("Pos", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;

			ImGui::SeparatorText("Rotation");
			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
			if (ImGui::DragFloat3("Rot", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;

			ImGui::SeparatorText("Scale");
			vNewScale = vScale;
			if (ImGui::DragFloat3("Scale", &vNewScale.x, 0.005f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;
		}

		if (bTransformDirtyFlag)
		{
			_float4x4 mNewWorld = XMMatrixAffineTransformation(vNewScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
				_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);

			m_pTransform->Set_Matrix(mNewWorld);
		}
		ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
		}
	}
#endif
}

void CCCTV::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_pRenderer->Add_RenderObject(CUTSCENE_CHANNEL::CHANNEL_MAIN, shared_from_this());
}

HRESULT CCCTV::Render()
{
	auto pSceneCapture = CSceneCapture::Get_Instance();
	if (FAILED(pSceneCapture->Bind_ShaderResourceView(m_pShader, SHADER_TEXDIFFUSE, CUTSCENE_CHANNEL::CHANNEL_MAIN)))
	{
		MSG_RETURN(E_FAIL, "CCCTV::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}
	m_pShader->Add_Flag(SHADER_FLAG_TEXDIFFUSE);

	if (FAILED(m_pLineTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texWhiteLine")))
	{
		MSG_RETURN(E_FAIL, "CCCTV::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}
	
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CCCTV::Render", "Failed to CTransform::Bind_OnShader");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, XMMatrixInverse(nullptr, XMMatrixIdentity()))))
	{
		MSG_RETURN(E_FAIL, "CCCTV::Render", "Failed to CTransform::Bind_OnShader");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, m_mProjection)))
	{
		MSG_RETURN(E_FAIL, "CCCTV::Render", "Failed to CTransform::Bind_OnShader");
	}

	if (FAILED(m_pShader->Bind_Float("g_fTime", m_fAccTime)))
	{
		MSG_RETURN(E_FAIL, "CCCTV::Render", "Failed to CTransform::Bind_Float");
	}

	if (FAILED(m_pVIBuffer_Rect->Render(m_pShader, 3)))
	{
		MSG_RETURN(E_FAIL, "CCCTV::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CCCTV::Lock_Camera(_bool bLock)
{
	m_bActiveCCTV = bLock;
	m_pCamera->Lock(m_bActiveCCTV);
}

HRESULT CCCTV::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: VIBUFFER_RECT");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: RENDERER");
	}

	m_pLineTexture = Get_Component<CTexture>(COMPONENT::TEXTURE);
	if (nullptr == m_pLineTexture)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

shared_ptr<CCCTV> CCCTV::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCCTV> pInstance = make_private_shared(CCCTV, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CCCTV::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCCTV::Clone(any pParentTransform)
{
	shared_ptr<CCCTV> pInstance = make_private_shared_copy(CCCTV, *this);

	if (FAILED(pInstance->Initialize(pParentTransform)))
	{
		MSG_RETURN(nullptr, "CCCTV::Clone", "Failed to Initialize");
	}

	return pInstance;
}
