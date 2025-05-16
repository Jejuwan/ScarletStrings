#include "ClientPCH.h"
#include "CameraKeyframeGenerator.h"

#include "GameInstance.h"
#include "Pipeline.h"

#include "Camera.h"
#include "DebugCamera.h"

#ifdef _DEBUG

CCameraKeyframeGenerator::CCameraKeyframeGenerator(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CKeyframeGenerator(_pDevice, _pContext)
{
}

CCameraKeyframeGenerator::CCameraKeyframeGenerator(const CCameraKeyframeGenerator& _rhs)
	: CKeyframeGenerator(_rhs)
{
}

HRESULT CCameraKeyframeGenerator::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);

	return S_OK;
}

HRESULT CCameraKeyframeGenerator::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCameraKeyframeGenerator::Initialize", "Failed to CGameObject::Initialize");
	}

	CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_DEBUG_OBJECT);

	return S_OK;
}

void CCameraKeyframeGenerator::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#if ACTIVATE_IMGUI

	if (ImGui::Begin("Camera Keyframe Generator"))
	{
		auto pPipeLine = CPipeLine::Get_Instance();
		_float4x4 mCamWorldTransform = pPipeLine->Get_Transform(PIPELINE::WORLD);
		_float4x4 mCamViewTransform = pPipeLine->Get_Transform(PIPELINE::VIEW);

		if (ImGui::CollapsingHeader("Transform"))
		{
			_bool bTransformDirtyFlag = false;
			_vector vScale, vQuaternion, vTranslation;

			_float3 vNewPosition;
			_float3 vNewRotation;

			if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mCamWorldTransform))
			{
				ImGui::SeparatorText("Position");
				vNewPosition = vTranslation;
				if (ImGui::DragFloat3("Pos", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
					bTransformDirtyFlag = true;

				ImGui::SeparatorText("Rotation");
				vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
				if (ImGui::DragFloat3("Rot", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
					bTransformDirtyFlag = true;
			}

			ImGui::Checkbox("Modify Mode", &m_bModifyMode);

			if (bTransformDirtyFlag)
			{
				_float4x4 mNewWorld = XMMatrixAffineTransformation(XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
					_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);

				auto pCamera = pPipeLine->Get_Camera();
				dynamic_pointer_cast<CTransform>(pCamera->Get_Component(COMPONENT::TRANSFORM))->Set_Matrix(mNewWorld);

				if (m_bModifyMode)
				{
					m_vecKeyframes[m_iSelectedKeyframe].first = mNewWorld;
				}
			}
		}

		if (ImGui::CollapsingHeader("Keyframe"))
		{
			ImGui::SeparatorText("Keyframe Viewer");
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
			ImGui::BeginChild("Keyframe Viewer", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7),
				true, ImGuiWindowFlags_HorizontalScrollbar);

			_uint iNumKeyframes = (_uint)m_vecKeyframes.size();
			for (_uint i = 0; i < iNumKeyframes; ++i)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				string strLable = "#" + std::to_string(i);
				float fHue = i * 0.05f;
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(fHue, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(fHue, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(fHue, 0.8f, 0.8f));

				// Move to Keyframe
				if (ImGui::Button(strLable.c_str(), ImVec2((_float)strLable.size() * 10.f, ImGui::GetFrameHeightWithSpacing() * 6.f)))
				{
					auto pCamera = pPipeLine->Get_Camera();
					dynamic_pointer_cast<CTransform>(pCamera->Get_Component(COMPONENT::TRANSFORM))->Set_Matrix(m_vecKeyframes[i].first);

					m_bModifyMode = false;
					m_iSelectedKeyframe = i;
				}

				ImGui::PopStyleColor(3);
				ImGui::PopID();
			}

			float scroll_x = ImGui::GetScrollX();
			float scroll_max_x = ImGui::GetScrollMaxX();
			ImGui::EndChild();
			ImGui::PopStyleVar(2);

			ImGui::SeparatorText("Keyframe Capture");
			ImGui::SmallButton("+");
			ImGui::SameLine();
			if (ImGui::Selectable("Section", false))
			{
				m_vecKeyframes.push_back({ mCamWorldTransform, 1.f });

				/*shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_DEBUG_OBJECT);
				assert(pLayer && "CCameraKeyframeGenerator::Find_Layer: LAYER_DEBUG_OBJECT Failed");

				shared_ptr<CDebugCamera> pCineCamera = dynamic_pointer_cast<CDebugCamera>(
					CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
						PROTOTYPE_GAMEOBJECT_DEBUG_CINE_CAMERA, mCamWorldTransform));
				if (pCineCamera)
				{
					m_vecCinematicCamera.push_back(pCineCamera);
				}*/
			}

			ImGui::Spacing();

			ImGui::SeparatorText("Keyframe Items");
			ImGui::Text("Current Selected Keyframe #%d", m_iCurrentSelectedKeyframe);

			ImGui::SmallButton("-");
			ImGui::SameLine();
			if (ImGui::Selectable("Reset", false))
			{
				m_bModifyMode = false;
				m_iCurrentSelectedKeyframe = 0;
			}

			ImGui::SeparatorText("Keyframe Interpolation Settings");
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));

			ImGui::BeginChild("Interpolation Viewer", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 6),
				true, ImGuiWindowFlags_HorizontalScrollbar);

			for (_uint i = 0; i < iNumKeyframes; ++i)
			{
				ImGui::PushID(iNumKeyframes + i);
				string strLable = "#" + std::to_string(i);
				float fHue = i * 0.05f;
				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(fHue, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(fHue, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(fHue, 0.8f, 0.8f));
				ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(fHue, 0.9f, 0.9f));

				ImGui::SliderFloat(strLable.c_str(), &m_vecKeyframes[i].second, 0.f, 50.f);

				ImGui::PopStyleColor(4);
				ImGui::PopID();
			}

			ImGui::EndChild();
			ImGui::PopStyleVar(2);

			ImGui::SeparatorText("Play Keyframe");

			ImGui::BeginGroup();
			ImVec2 vSize = ImGui::GetItemRectSize();
			
			if (ImGui::Button("Play", ImVec2((vSize.x - ImGui::GetStyle().ItemSpacing.x) * 0.333f, vSize.y)))
			{
				m_bPlaying = true;

				m_iCurrentPlayingKeyframe = 0;
				m_bCurrentKeyframeAccTime = 0.f;

				if (false == m_vecKeyframes.empty())
				{
					auto pCamera = pPipeLine->Get_Camera();
					dynamic_pointer_cast<CTransform>(pCamera->Get_Component(COMPONENT::TRANSFORM))
						->Set_Matrix(m_vecKeyframes[m_iCurrentPlayingKeyframe].first);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Pause", ImVec2((vSize.x - ImGui::GetStyle().ItemSpacing.x) * 0.333f, vSize.y)))
			{
				m_bPlaying = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Continue", ImVec2((vSize.x - ImGui::GetStyle().ItemSpacing.x) * 0.333f, vSize.y)))
			{
				m_bPlaying = true;
			}
			ImGui::EndGroup();

			if (CGameInstance::Get_Instance()->Key_Down(VK_SPACE))
			{
				m_bPlaying = true;

				m_iCurrentPlayingKeyframe = 0;
				m_bCurrentKeyframeAccTime = 0.f;

				if (false == m_vecKeyframes.empty())
				{
					auto pCamera = pPipeLine->Get_Camera();
					dynamic_pointer_cast<CTransform>(pCamera->Get_Component(COMPONENT::TRANSFORM))
						->Set_Matrix(m_vecKeyframes[m_iCurrentPlayingKeyframe].first);
				}
			}

			if (CGameInstance::Get_Instance()->Key_Down(VK_DELETE))
			{
				m_vecKeyframes.pop_back();
			}
		}

		if (ImGui::CollapsingHeader("Import / Export"))
		{
			Render_File_Dialog();
		}
	}
	ImGui::End();
#endif

	if (m_bPlaying && false == m_vecKeyframes.empty())
	{
		if (m_vecKeyframes.size() <= m_iCurrentPlayingKeyframe)
		{
			m_iCurrentPlayingKeyframe = 0;
			m_bCurrentKeyframeAccTime = 0.f;

			m_bPlaying = false;

			return;
		}

		const std::pair<_float4x4, _float>& tKeyframe = m_vecKeyframes[m_iCurrentPlayingKeyframe];
		if (m_bCurrentKeyframeAccTime >= tKeyframe.second)
		{
			m_bCurrentKeyframeAccTime = 0.f;
			++m_iCurrentPlayingKeyframe;
		}

		_float4x4 mInterpolatedMatrix;
		if (Interpolate_Keyframe(m_iCurrentPlayingKeyframe, m_bCurrentKeyframeAccTime, mInterpolatedMatrix))
		{
			auto pPipeLine = CPipeLine::Get_Instance();
			auto pCamera = pPipeLine->Get_Camera();
			dynamic_pointer_cast<CTransform>(pCamera->Get_Component(COMPONENT::TRANSFORM))
				->Set_Matrix(mInterpolatedMatrix);
		}
		m_bCurrentKeyframeAccTime += fTimeDelta;
	}
}

void CCameraKeyframeGenerator::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (false == m_bPlaying)
	{
		for (_uint i = 0; i < m_vecCinematicCamera.size(); ++i)
		{
			m_vecCinematicCamera[i]->Late_Tick(fTimeDelta);
		}
	}
}

HRESULT CCameraKeyframeGenerator::Render()
{
	return S_OK;
}

HRESULT CCameraKeyframeGenerator::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCameraKeyframeGenerator::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

HRESULT CCameraKeyframeGenerator::Import(const wstring& _wstrImportPath)
{
	m_vecCinematicCamera.clear();

	__super::Import(_wstrImportPath);

	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_DEBUG_OBJECT);
	if (nullptr == pLayer)
		MSG_RETURN(E_FAIL, "CCameraKeyframeGenerator::Find_Layer", "LAYER_DEBUG_OBJECT Failed");

	for (_uint i = 0; i < m_vecKeyframes.size(); ++i)
	{
		shared_ptr<CDebugCamera> pCineCamera = dynamic_pointer_cast<CDebugCamera>(
			CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
				PROTOTYPE_GAMEOBJECT_DEBUG_CINE_CAMERA, m_vecKeyframes[i].first));
		if (pCineCamera)
		{
			m_vecCinematicCamera.push_back(pCineCamera);
		}
	}

	return S_OK;
}

shared_ptr<CCameraKeyframeGenerator> CCameraKeyframeGenerator::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCameraKeyframeGenerator> pInstance = make_private_shared(CCameraKeyframeGenerator, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CCameraKeyframeGenerator::Create", "Failed to Initialze_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCameraKeyframeGenerator::Clone(any)
{
	shared_ptr<CCameraKeyframeGenerator> pInstance = make_private_shared_copy(CCameraKeyframeGenerator, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CCameraKeyframeGenerator::Clone", "Failed to Initialze");
	}

	return pInstance;
}

#endif
