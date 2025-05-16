#include "ClientPCH.h"
#include "Kasane.h"
#include "GameInstance.h"

#include "Client_Animation.h"

CKasane::CKasane(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCharacter(_pDevice, _pContext)
{
}

CKasane::CKasane(const CKasane& _rhs)
	: CCharacter(_rhs)
{
}

HRESULT CKasane::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_KASANE, g_aNull);

	return S_OK;
}

HRESULT CKasane::Initialize(any PlayMode)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CKasane::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pModel->Set_Animation(0);
	m_pTransform->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	// Hide Mask && Hood
	m_pModel->Hide_Mesh(4);
	m_pModel->Hide_Mesh(5);

	m_vecMeshPassIndexPair.push_back({ 0, 17 });
	m_vecMeshPassIndexPair.push_back({ 2, 17 });
	m_vecMeshPassIndexPair.push_back({ 3, 17 });

	m_vecMeshPassIndexPair.push_back({ 1, 0 });
	m_vecMeshPassIndexPair.push_back({ 6, 0 });
	m_vecMeshPassIndexPair.push_back({ 7, 0 });

	m_bPlayMode = any_cast<_bool>(PlayMode);

	return S_OK;
}

void CKasane::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModel->Tick_Animation(fTimeDelta);

#ifdef _DEBUG
	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("Kasane"))
	{
	if (ImGui::BeginMenu("Animation Settings"))
	{
	ImGui::Text("Kasane Animation Index : %d", m_iCurrentAnimIndex);

	ImGui::SameLine();
	if (ImGui::ArrowButton("Kasane Left", ImGuiDir_Left))
	{
		--m_iCurrentAnimIndex;
		if (m_iCurrentAnimIndex < 0)
			m_iCurrentAnimIndex = _int(ANIMATION::KASANE_CUTSCENE::MAX) - 1;

		m_pModel->Set_Animation(m_iCurrentAnimIndex);
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("Kasane Right", ImGuiDir_Right))
	{
		m_iCurrentAnimIndex++;
		if (m_iCurrentAnimIndex >= _int(ANIMATION::KASANE_CUTSCENE::MAX))
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
			ImGui::SeparatorText("Kasane Transform");
			vNewPosition = vTranslation;
			if (ImGui::DragFloat3("Kasane Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
				bTransformDirtyFlag = true;
			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
			if (ImGui::DragFloat3("Kasane Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
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

	if (ImGui::BeginMenu("Mask Dissolve"))
	{
		ImGui::SliderFloat("Dissolve", &m_fDissolveAccTime, 0.f, 2.f);
		ImGui::EndMenu();
	}

	ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	}

	// Material Settings
	/*ImGui::Begin("Material Settings");
	if (ImGui::CollapsingHeader("Kasane Material"))
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

	if (m_bDriveMode)
	{
		m_fDissolveAccTime += fTimeDelta * 0.5f;
		if (m_bScreenEffect)
		{
			m_fLineStripeTime += fTimeDelta;
		}

		auto pGameInstance = CGameInstance::Get_Instance();
		if (false == m_bMaskOn && m_fDissolveAccTime >= 0.f)
		{
			m_bMaskOn = true;
		}

		if (false == m_bScreenEffect && m_fDissolveAccTime >= 1.f)
		{
			m_bScreenEffect = true;

			pGameInstance->Set_LUTIndex(2);
			pGameInstance->MaskingLUT(true, m_pCircularMask);
		}

		if (m_fDissolveAccTime >= 1.5f)
		{
			m_bDriveMode = false;
			m_bMaskOn = false;
			m_bScreenEffect = false;

			if (m_fpFinished)
			{
				m_fpFinished();
				m_fpFinished = nullptr;
			}

			pGameInstance->Set_LUTIndex(4);
			pGameInstance->MaskingLUT(false);

			pGameInstance->Set_DOF_DepthStart(m_fOriginDOFStart);
			pGameInstance->Set_DOF_DepthRange(m_fOriginDOFRange);

			pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, m_fOriginBloomStrength);
			pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, m_fOriginExposure);
			pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, m_fOriginGamma);

			m_pModel->Hide_Mesh(4);
			m_pModel->Reset_Animation(27);
			m_pModel->Set_Animation(0.f, 1.f);
		}
	}
}

void CKasane::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);

	if (m_bScreenEffect)
	{
		Add_RenderObject(RENDER_GROUP::SCREEN_EFFECT);
	}
}

HRESULT CKasane::Render()
{
	if (m_bDriveMode)
	{
		if (FAILED(Render_Drive_Cutscene()))
		{
			MSG_RETURN(E_FAIL, "CKasane::Render", "Failed to Render_Drive_Cutscene");
		}
		if (FAILED(Render_Drive_Mask()))
		{
			MSG_RETURN(E_FAIL, "CKasane::Render", "Failed to Render_Drive_Mask");
		}
	}

	if (m_bPlayMode == false)
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
		/*if (FAILED(__super::Render(0)))
		{
			MSG_RETURN(E_FAIL, "CKasane::Render", "Failed to CGameObject::Render");
		}

		if (FAILED(m_pModel->Bind_ShaderResourceViews(3, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CKasane::Render", "Failed to Bind_ShaderResourceViews");
		}

		m_pModel->Render(m_pShader, 4);*/
	}

	return S_OK;
}

void CKasane::Move_Mouth()
{
}

void CKasane::Stop_Mouth()
{
}

void CKasane::Open_Drive_Mode(const std::function<void()>& fpFinished)
{
	if (m_bDriveMode)
		return;

	m_bDriveMode = true;
	m_fpFinished = fpFinished;

	_matrix matTranslationMatrix = XMMatrixTranslationFromVector(XMVectorSet(0.0f, -1.13f, 0.55f, 1.f));
	_matrix matRotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	_matrix matWorldMatrix = matRotationMatrix * matTranslationMatrix;

	m_pTransform->Set_Matrix(matWorldMatrix);

	m_pModel->Set_Animation(27, 1.5f, false, 0.f, false);

	m_pModel->Hide_Mesh(4, false);

	auto pGameInstance = CGameInstance::Get_Instance();
	m_fOriginDOFStart = pGameInstance->Get_DOF_DepthStart();
	m_fOriginDOFRange = pGameInstance->Get_DOF_DepthRange();

	m_fOriginBloomStrength = pGameInstance->Get_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING);
	m_fOriginExposure = pGameInstance->Get_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING);
	m_fOriginGamma = pGameInstance->Get_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING);

	pGameInstance->Set_DOF_DepthStart(1.f);
	pGameInstance->Set_DOF_DepthRange(0.f);

	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.5f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.1f);

	m_fDissolveAccTime = -1.f;
	m_fLineStripeTime = 0.f;
}

HRESULT CKasane::Render_Drive_Cutscene()
{
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLDIF, m_tMaterialDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLAMB, m_tMaterialDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLSPC, m_tMaterialDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLEMS, m_tMaterialDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_MTRLSHN, m_tMaterialDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render", "Failed to Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, m_mView)))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render", "Failed to Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pModel->Render(m_pShader, 4)))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render", "Failed to CModel::Render");
	}

	return S_OK;
}

HRESULT CKasane::Render_Drive_Mask()
{
	if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXDISSOLVE, m_pMaskNoise->Get_ShaderResourceView())))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Drive_Mask", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pShader->Bind_Float("g_fThreshold", m_fDissolveAccTime)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pModel->Bind_ShaderResourceViews(5, m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Drive_Mask", "Failed to CModel::Render");
	}
	if (FAILED(m_pModel->Render(5, m_pShader, 11)))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Drive_Mask", "Failed to CModel::Render");
	}

	return S_OK;
}

HRESULT CKasane::Render_Screen_Effect()
{
	if (FAILED(m_pEffectShader->Bind_Matrix(SHADER_MATWORLD, m_pScreenBufferTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Screen_Effect", "Failed to Bind_Matrix: World");
	}
	if (FAILED(m_pEffectShader->Bind_Matrix(SHADER_MATVIEW, XMMatrixIdentity())))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Screen_Effect", "Failed to Bind_Matrix: View");
	}
	if (FAILED(m_pEffectShader->Bind_Matrix(SHADER_MATPROJ, m_matOrthoProj)))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Screen_Effect", "Failed to Bind_Matrix: Projection");
	}

	if (FAILED(m_pEffectShader->Bind_Float("g_fTime", m_fLineStripeTime)))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Screen_Effect", "Failed to Bind_Float: LineStripeTime");
	}

	if (FAILED(m_pEffectShader->Bind_ShaderResourceView(SHADER_TEXDIFFUSE, m_pNoiseLine->Get_ShaderResourceView())))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Screen_Effect", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pVIBuffer_Rect->Render(m_pEffectShader, 4)))
	{
		MSG_RETURN(E_FAIL, "CKasane::Render_Screen_Effect", "Failed to Render");
	}

	return S_OK;
}

HRESULT CKasane::Render_OutLine()
{
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, m_mView)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutLineColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(m_pModel->Render(m_pShader, 3)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to CModel::Render");
	}

	return S_OK;
}

HRESULT CKasane::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CKasane::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pMaskNoise = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_MASK));
	if (nullptr == m_pMaskNoise)
	{
		MSG_RETURN(E_FAIL, "CKasane::Ready_Components", "Failed to Clone Component: PROTOTYPE_COMPONENT_TEXTURE_NOISE_MASK");
	}

	m_pCircularMask = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Mask/CircularMask.dds"));
	if (nullptr == m_pCircularMask)
	{
		MSG_RETURN(E_FAIL, "CPlayer::Ready_Components", "Failed to Create: CircularMask");
	}

	// Screen Effect
	m_pNoiseLine = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_LINE_022));
	if (nullptr == m_pNoiseLine)
	{
		MSG_RETURN(E_FAIL, "CKasane::Ready_Components", "Failed to Clone Component: PROTOTYPE_COMPONENT_TEXTURE_NOISE_LINE_022");
	}
	m_pEffectShader = dynamic_pointer_cast<CShader>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE));
	if (nullptr == m_pEffectShader)
	{
		MSG_RETURN(E_FAIL, "CKasane::Ready_Components", "Failed to Clone Component: PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX");
	}
	m_pVIBuffer_Rect = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_RECT));
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CKasane::Ready_Components", "Failed to Clone Component: PROTOTYPE_COMPONENT_VIBUFFER_RECT");
	}

	m_pScreenBufferTransform = CTransform::Create(m_pDevice, m_pContext);
	m_pScreenBufferTransform->Set_Scale(XMVectorSet(g_iWinCX, g_iWinCY, 1.f, 0.f));
	m_pScreenBufferTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 0.1f, 1.f));
	m_matOrthoProj = XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f);

	return S_OK;
}

shared_ptr<CKasane> CKasane::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CKasane> pInstance = make_private_shared(CKasane, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CKasane::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CKasane::Clone(any PlayMode)
{
	shared_ptr<CKasane> pInstance = make_private_shared_copy(CKasane, *this);

	if (FAILED(pInstance->Initialize(PlayMode)))
	{
		MSG_RETURN(nullptr, "CKasane::Clone", "Failed to Initialize");
	}

	return pInstance;
}
