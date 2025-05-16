#include "ClientPCH.h"
#include "ObjectKeyframeGenerator.h"

#include "GameInstance.h"

#include "ObjectLayer.h"
#include "Camera.h"

CObjectKeyframeGenerator::CObjectKeyframeGenerator(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CKeyframeGenerator(_pDevice, _pContext)
{
}

CObjectKeyframeGenerator::CObjectKeyframeGenerator(const CObjectKeyframeGenerator& _rhs)
	: CKeyframeGenerator(_rhs)
{
}

HRESULT CObjectKeyframeGenerator::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);

	return S_OK;
}

HRESULT CObjectKeyframeGenerator::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CObjectKeyframeGenerator::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CObjectKeyframeGenerator::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#if ACTIVATE_IMGUI

	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("Object Keyframe Generator"))
	{
	if (ImGui::CollapsingHeader("Object"))
	{
		auto pGameInstance = CGameInstance::Get_Instance();

		std::function<_bool(pair<wstring, shared_ptr<CObjectLayer>>)> fpIterate_Layer = std::bind(&CObjectKeyframeGenerator::Iterate_Layers, this, std::placeholders::_1);
		pGameInstance->Iterate_Layers(pGameInstance->Current_Scene(), fpIterate_Layer);
	}

	if (m_pSelectedObject.lock())
	{
		shared_ptr<CGameObject> pSelectedObject = m_pSelectedObject.lock();
		auto pSelectedTransform = dynamic_pointer_cast<CTransform>(pSelectedObject->Get_Component(COMPONENT::TRANSFORM));

		if (ImGui::CollapsingHeader("Transform"))
		{
			_bool bTransformDirtyFlag = false;
			_vector vScale, vQuaternion, vTranslation;

			_float3 vNewPosition;
			_float3 vNewRotation;
			_float3 vNewScale;

			if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, pSelectedTransform->Get_Matrix()))
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

				pSelectedTransform->Set_Matrix(mNewWorld);
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
				if (ImGui::Button(strLable.c_str(), ImVec2((_uint)strLable.size() * 10, ImGui::GetFrameHeightWithSpacing() * 6)))
				{
					//auto pCamera = pPipeLine->Get_Camera();
					//dynamic_pointer_cast<CTransform>(pCamera->Get_Component(COMPONENT::TRANSFORM))->Set_Matrix(m_vecKeyframes[i].first);
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
				m_vecKeyframes.push_back({ pSelectedTransform->Get_Matrix(), 1.f });
			}

			ImGui::Spacing();

			ImGui::SeparatorText("Keyframe Items");
			ImGui::Text("Current Selected Keyframe #%d", m_iCurrentSelectedKeyframe);

			ImGui::SmallButton("-");
			ImGui::SameLine();
			if (ImGui::Selectable("Reset", false))
			{
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
					pSelectedTransform
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
		}
	}
	
	if (ImGui::CollapsingHeader("Import / Export"))
	{
		Render_File_Dialog();
	}

	ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	}
#endif

	shared_ptr<CGameObject> pSelectedObject = m_pSelectedObject.lock();
	
	if (pSelectedObject && m_bPlaying && false == m_vecKeyframes.empty())
	{
		if (m_vecKeyframes.size() <= m_iCurrentPlayingKeyframe)
		{
			m_iCurrentPlayingKeyframe = 0;
			m_bCurrentKeyframeAccTime = 0.f;

			m_bPlaying = false;

			return;
		}

		auto pSelectedTransform = dynamic_pointer_cast<CTransform>(pSelectedObject->Get_Component(COMPONENT::TRANSFORM));

		const std::pair<_float4x4, _float>& tKeyframe = m_vecKeyframes[m_iCurrentPlayingKeyframe];
		if (m_bCurrentKeyframeAccTime >= tKeyframe.second)
		{
			m_bCurrentKeyframeAccTime = 0.f;
			++m_iCurrentPlayingKeyframe;
		}

		_float4x4 mInterpolatedMatrix;
		if (Interpolate_Keyframe(m_iCurrentPlayingKeyframe, m_bCurrentKeyframeAccTime, mInterpolatedMatrix))
		{
			pSelectedTransform->Set_Matrix(mInterpolatedMatrix);
		}
		m_bCurrentKeyframeAccTime += fTimeDelta;
	}
}

void CObjectKeyframeGenerator::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_pSelectedObject.lock())
	{
		//m_pRenderer->Add_RenderObject(RENDER_GROUP::OUTLINE, m_pSelectedObject.lock());
	}
}

HRESULT CObjectKeyframeGenerator::Render()
{
	return S_OK;
}

HRESULT CObjectKeyframeGenerator::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCameraKeyframeGenerator::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CObjectKeyframeGenerator::Ready_Components", "Failed to Get_Component: RENDERER");
	}

	return S_OK;
}

HRESULT CObjectKeyframeGenerator::Import(const wstring& wstrImportPath)
{
	return __super::Import(wstrImportPath);
}

_bool CObjectKeyframeGenerator::Iterate_Layers(pair<wstring, shared_ptr<CObjectLayer>> _tLayer)
{
	const auto& tLayerPair = _tLayer;
	int wideStrSize = (_uint)tLayerPair.first.size();
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, tLayerPair.first.c_str(), wideStrSize, nullptr, 0, nullptr, nullptr);
	std::string narrowStr(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, tLayerPair.first.c_str(), wideStrSize, &narrowStr[0], bufferSize, nullptr, nullptr);

#if ACTIVATE_IMGUI
	if (ImGui::TreeNode(narrowStr.c_str()))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
		ImGui::BeginChild("Keyframe Viewer", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7),
			true, ImGuiWindowFlags_HorizontalScrollbar);

		auto Iter = _tLayer.second->Begin();
		_uint iIndex = 0;
		while (Iter != _tLayer.second->End())
		{
			if ((iIndex % 30) < 15)
				ImGui::SameLine();
			ImGui::PushID(iIndex);
			string strLable = "#" + std::to_string(iIndex);
			float fHue = iIndex * 0.05f;
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(fHue, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(fHue, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(fHue, 0.8f, 0.8f));

			// Move to Keyframe
			if (ImGui::Button(strLable.c_str(), ImVec2((_float)strLable.size() * 10.f, ImGui::GetFrameHeightWithSpacing() * 6.f)))
			{
				m_pSelectedObject = *Iter;
				
				auto pPipeLine = CPipeLine::Get_Instance();
				auto pCamera = pPipeLine->Get_Camera();

				if (m_pSelectedObject.lock())
				{
					//auto pTransform = dynamic_pointer_cast<CTransform>(m_pSelectedObject.lock()->Get_Component(COMPONENT::TRANSFORM));
					//dynamic_pointer_cast<CTransform>(pCamera->Get_Component(COMPONENT::TRANSFORM))->Set_Matrix(pTransform->Get_Matrix());
				}
			}

			ImGui::PopStyleColor(3);
			ImGui::PopID();

			++iIndex;
			++Iter;
		}

		float scroll_x = ImGui::GetScrollX();
		float scroll_max_x = ImGui::GetScrollMaxX();
		ImGui::EndChild();
		ImGui::PopStyleVar(2);

		ImGui::TreePop();
	}
#endif

	return true;
}

shared_ptr<CObjectKeyframeGenerator> CObjectKeyframeGenerator::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CObjectKeyframeGenerator> pInstance = make_private_shared(CObjectKeyframeGenerator, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CObjectKeyframeGenerator::Create", "Failed to Initialze_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CObjectKeyframeGenerator::Clone(any)
{
	shared_ptr<CObjectKeyframeGenerator> pInstance = make_private_shared_copy(CObjectKeyframeGenerator, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CObjectKeyframeGenerator::Clone", "Failed to Initialze");
	}

	return pInstance;
}
