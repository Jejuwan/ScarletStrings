#include "ClientPCH.h"
#include "CutsceneBuffer.h"

#include "GameInstance.h"
#include "Pipeline.h"

#include "SceneCapture.h"

#include "Camera.h"

CCutsceneBuffer::CCutsceneBuffer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CCutsceneBuffer::CCutsceneBuffer(const CCutsceneBuffer& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCutsceneBuffer::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT) | BIT(COMPONENT::KEYFRAME);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::KEYFRAME] = make_pair(PROTOTYPE_COMPONENT_KEYFRAME, g_aNull);
	
	return S_OK;
}

HRESULT CCutsceneBuffer::Initialize(any _eCutsceneChannel)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Initialize", "Failed to CGameObject::Initialize");
	}

	m_eCutsceneChannel = any_cast<CUTSCENE_CHANNEL>(_eCutsceneChannel);

	Set_Enable(false);
	
	auto pPipeLine = CPipeLine::Get_Instance();
	auto tCameraDesc = pPipeLine->Get_Camera()->Get_Desc();
	m_mProjection = XMMatrixPerspectiveFovLH(tCameraDesc.fFovAngleY, tCameraDesc.fAspect, tCameraDesc.fNear, tCameraDesc.fFar);

	return S_OK;
}

void CCutsceneBuffer::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_pKeyframe->Tick(m_pTransform, _fTimeDelta);

	if (m_bDisappear)
	{
		m_fAccTime += _fTimeDelta;
		if (m_fAccTime >= m_fDuration)
		{
			m_fAccTime = 0.f;
			m_bDisappear = false;
		}

		m_pTransform->Translate(m_vVelocity * _fTimeDelta);
	}

#if ACTIVATE_IMGUI
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

void CCutsceneBuffer::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_pRenderer->Add_RenderObject(m_eCutsceneChannel, shared_from_this());
}

HRESULT CCutsceneBuffer::Render()
{
	auto pSceneCapture = CSceneCapture::Get_Instance();
	if (FAILED(pSceneCapture->Bind_ShaderResourceView(m_pShader, SHADER_TEXDIFFUSE, m_eCutsceneChannel)))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}
	m_pShader->Add_Flag(SHADER_FLAG_TEXDIFFUSE);



	if (m_eCutsceneChannel == CUTSCENE_CHANNEL::CHANNEL_MAIN)
	{
		if (FAILED(m_pHorizontalMaskTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXNORMAL)))
		{
			MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CTexture::Bind_ShaderResourceView");
		}
	}
	else
	{
		if (FAILED(m_pVerticalMaskTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXNORMAL)))
		{
			MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CTexture::Bind_ShaderResourceView");
		}
	}

	if (FAILED(m_pNoiseGridTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texNoiseGrid")))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CTransform::Bind_OnShader");
	}

	auto pTransform = dynamic_pointer_cast<CTransform>(m_pCamera->Get_Component(COMPONENT::TRANSFORM));
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, pTransform->Get_Matrix().inverse())))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CTransform::Bind_OnShader");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, m_mProjection)))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CTransform::Bind_OnShader");
	}

	if (FAILED(m_pVIBuffer_Rect->Render(m_pShader, 0)))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CCutsceneBuffer::Render_OutLine()
{
	//if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	//{
	//	MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CTransform::Bind_OnShader");
	//}

	//if (FAILED(__super::Render(3)))
	//{
	//	MSG_RETURN(E_FAIL, "CCutsceneBuffer::Render", "Failed to CGameObject::Render");
	//}

	return S_OK;
}

void CCutsceneBuffer::Bind_Camera(shared_ptr<CCamera> pCamera, _matrix mInitTransform)
{
	m_pCamera = pCamera;

	auto pTransform = dynamic_pointer_cast<CTransform>(m_pCamera->Get_Component(COMPONENT::TRANSFORM));
	pTransform->Set_Matrix(mInitTransform);
}

void CCutsceneBuffer::Append_Keyframe(const wstring& wstrFilePath, const wstring& wstrAnimTag)
{
	m_pKeyframe->Append_Keyframe(wstrFilePath, wstrAnimTag);
}

void CCutsceneBuffer::Play_Animation(const wstring& wstrAnimTag, const std::function<void(const wstring& wstrAnimTag)>& fpFinished)
{
	if (m_pKeyframe->Play_Animation(wstrAnimTag, g_mUnit))
	{
		m_pKeyframe->Subscribe_Animation_Finish_Callback(fpFinished);
	}
	else
	{
		fpFinished(TEXT(""));
	}
}

void CCutsceneBuffer::Disappear(_float3 vVelocity, _float fDuration)
{
	m_bDisappear = true;
	m_vVelocity = vVelocity;

	m_fDuration = fDuration;
	m_fAccTime = 0.f;
}

HRESULT CCutsceneBuffer::Ready_Components()
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

	m_pKeyframe = Get_Component<CKeyframe>(COMPONENT::KEYFRAME);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: KEYFRAME");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: RENDERER");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pHorizontalMaskTexture =
		dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_MASK_HORIZONTAL));
	m_pVerticalMaskTexture =
		dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_MASK_VERTICAL));
	m_pNoiseGridTexture =
		dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_NOISE_GRID));

	return S_OK;
}

shared_ptr<CCutsceneBuffer> CCutsceneBuffer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCutsceneBuffer> pInstance = make_private_shared(CCutsceneBuffer, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CCutsceneBuffer::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCutsceneBuffer::Clone(any _eCutsceneChannel)
{
	shared_ptr<CCutsceneBuffer> pInstance = make_private_shared_copy(CCutsceneBuffer, *this);

	if (FAILED(pInstance->Initialize(_eCutsceneChannel)))
	{
		MSG_RETURN(nullptr, "CCutsceneBuffer::Clone", "Failed to Initialize");
	}

	return pInstance;
}
