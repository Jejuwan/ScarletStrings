#include "ClientPCH.h"
#include "EventColliderGenerator.h"
#include "GameInstance.h"

#include "EventCollider.h"
#include "Map_Tool.h"

#ifdef _DEBUG

#define DIALOG_LOAD			"Dialog::Load"
#define DIALOG_EXPORT		"Dialog::Export"

CEventColliderGenerator::CEventColliderGenerator(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CEventColliderGenerator::CEventColliderGenerator(const CEventColliderGenerator& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEventColliderGenerator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEventColliderGenerator::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CBattleFieldGenerator::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CEventColliderGenerator::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#if ACTIVATE_IMGUI

	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("EventCollider Generator"))
	{
	if (ImGui::CollapsingHeader("Transform"))
	{
		if (m_pSelectedCollider)
		{
			ImGui::SeparatorText("Object Selected");
			auto pTransform = dynamic_pointer_cast<CTransform>(m_pSelectedCollider->Get_Component(COMPONENT::TRANSFORM));

			_float4x4 matColliderMatrix = pTransform->Get_Matrix();

			_float3 vNewPosition;
			_float3 vNewRotation;
			_float3 vNewScale;

			_vector vScale, vQuaternion, vTranslation;
			_bool bTransformDirtyFlag = false;
			if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, matColliderMatrix))
			{
				ImGui::SeparatorText("Collider Transform");
				vNewPosition = vTranslation;
				if (ImGui::DragFloat3("Collider Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
					bTransformDirtyFlag = true;
				vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
				if (ImGui::DragFloat3("Collider Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
					bTransformDirtyFlag = true;
				vNewScale = vScale;
				if (ImGui::DragFloat3("Collider Scale", &vNewScale.x, 0.005f, -FLT_MAX, +FLT_MAX))
					bTransformDirtyFlag = true;
			}

			if (bTransformDirtyFlag)
			{
				_float4x4 mNewWorld = XMMatrixAffineTransformation(vNewScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
					_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);

				pTransform->Set_Matrix(mNewWorld);
			}
		}
		else
		{
			ImGui::SeparatorText("Object Not Selected");
			ImGui::DragFloat3("Position", &m_vCurrentPoint.x);
		}
	}

	if (ImGui::CollapsingHeader("Field Points"))
	{
		ImGui::SeparatorText("Field Viewer");
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
		ImGui::BeginChild("Field Viewer", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7),
			true, ImGuiWindowFlags_HorizontalScrollbar);

		_uint iNumEventCollider = (_uint)m_lstEventCollider.size();
		for (_uint i = 0; i < iNumEventCollider; ++i)
		{
			if (i > 0) ImGui::SameLine();
			ImGui::PushID(i);
			string strLable = "#" + std::to_string(i);
			float fHue = i * 0.05f;
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(fHue, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(fHue, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(fHue, 0.8f, 0.8f));

			if (ImGui::Button(strLable.c_str(), ImVec2((_float)strLable.size() * 10.f, ImGui::GetFrameHeightWithSpacing() * 6.f)))
			{
				m_iSelectedCollider = (_int)i;
				auto pSelectedEventCollider = *std::next(m_lstEventCollider.begin(), m_iSelectedCollider);

				m_pSelectedCollider = pSelectedEventCollider;
			}

			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

		if (CGameInstance::Get_Instance()->Key_Down(VK_DELETE))
		{
			if (false == m_lstEventCollider.empty())
				m_lstEventCollider.pop_back();
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
			shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), TEXT("EventCollider"));
			assert(pLayer && "CEventColliderGenerator::Find_Layer: EventCollider Failed");

			auto pGameInstance = CGameInstance::Get_Instance();
			shared_ptr<CEventCollider> pEventCollider = dynamic_pointer_cast<CEventCollider>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER));
			if (pEventCollider)
			{
				auto pTransform = dynamic_pointer_cast<CTransform>(pEventCollider->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Set_State(TRANSFORM::POSITION, m_vCurrentPoint);
				m_lstEventCollider.push_back(pEventCollider);
			}
		}
		ImGui::SameLine();
		if (CGameInstance::Get_Instance()->Key_Down('P'))
		{
			m_bPickingMode = !m_bPickingMode;
		}
		if (m_bPickingMode)
		{
			if (CGameInstance::Get_Instance()->Key_Down(VK_RBUTTON))
			{
				if (m_pMapTool)
				{
					m_vCurrentPoint = m_pMapTool->Get_PickingPos();
				}

				if (m_pSelectedCollider)
				{
					auto pTransform = dynamic_pointer_cast<CTransform>(m_pSelectedCollider->Get_Component(COMPONENT::TRANSFORM));
					pTransform->Set_State(TRANSFORM::POSITION, m_vCurrentPoint);
				}
			}
		}

		ImGui::Spacing();

		ImGui::SeparatorText("Keyframe Items");
		ImGui::Text("Current Selected Keyframe #%d", m_iSelectedCollider);

		ImGui::SmallButton("-");
		ImGui::SameLine();
		if (ImGui::Selectable("Reset", false))
		{
			m_iSelectedCollider = 0;
			m_pSelectedCollider = nullptr;
		}
	}

	if (ImGui::CollapsingHeader("Event Type"))
	{
		if (m_pSelectedCollider)
		{
			const char* EventTypeName[] = { "Battle Field", "Tutorial", "Dialog", "Cutscene", "Boss" };

			EVENT_TYPE eSelectedEventType = m_pSelectedCollider->Get_EventType();
			if (ImGui::Combo("EventTypeCombo", reinterpret_cast<int*>(&eSelectedEventType), EventTypeName, static_cast<int>(EVENT_TYPE::NONE)))
			{
				m_pSelectedCollider->Set_EventType(eSelectedEventType);
			}

			std::string strEventTag = m_pSelectedCollider->Get_EventTag();
			char inputBuffer[256];
			strcpy_s(inputBuffer, strEventTag.c_str());

			if (ImGui::InputText("Event Tag", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_AllowTabInput))
			{
				m_pSelectedCollider->Set_EventTag(inputBuffer);
			}

			/*string strEventTag = m_pSelectedCollider->Get_EventTag();
			if (ImGui::InputText("Event Tag", reinterpret_cast<char*>(&strEventTag[0]), strEventTag.capacity() + 1, ImGuiInputTextFlags_AllowTabInput))
			{
				m_pSelectedCollider->Set_EventTag(strEventTag);
			}*/
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

	for (auto& Iter : m_lstEventCollider)
	{
		Iter->Tick(fTimeDelta);
	}
}

void CEventColliderGenerator::Late_Tick(_float fTimeDelta)
{
	for (auto& Iter : m_lstEventCollider)
	{
		Iter->Late_Tick(fTimeDelta);
	}
}

HRESULT CEventColliderGenerator::Render()
{
	return S_OK;
}

void CEventColliderGenerator::Bind_MapTool(shared_ptr<class CMap_Tool> pMapTool)
{
	m_pMapTool = pMapTool;
}

HRESULT CEventColliderGenerator::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBattleFieldGenerator::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

HRESULT CEventColliderGenerator::Import(const wstring& wstrImportPath)
{
	std::ifstream inFile(wstrImportPath, std::ios::binary);
	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEventColliderGenerator::Import", "Failed to Open File");
	}

	EVENT_TYPE eEventType;
	inFile.read(reinterpret_cast<char*>(&eEventType), sizeof(EVENT_TYPE));

	size_t iTagSize;
	inFile.read(reinterpret_cast<char*>(&iTagSize), sizeof(size_t));
	std::string strEventTag(iTagSize, '\0');
	inFile.read(&strEventTag[0], iTagSize);

	_float4x4 matWorld;
	inFile.read(reinterpret_cast<char*>(&matWorld), sizeof(_float4x4));

	auto pGameInstance = CGameInstance::Get_Instance();
	shared_ptr<CEventCollider> pEventCollider = dynamic_pointer_cast<CEventCollider>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER));
	if (pEventCollider)
	{
		pEventCollider->Set_EventType(eEventType);
		pEventCollider->Set_EventTag(strEventTag);

		auto pTransform = dynamic_pointer_cast<CTransform>(pEventCollider->Get_Component(COMPONENT::TRANSFORM));
		pTransform->Set_Matrix(matWorld);

		m_lstEventCollider.push_back(pEventCollider);
	}
	
	inFile.close();

	return S_OK;
}

HRESULT CEventColliderGenerator::Export(const wstring& wstrExportPath)
{
	if (m_pSelectedCollider)
	{
		std::ofstream outFile(wstrExportPath, std::ios::binary);
		if (!outFile.is_open())
		{
			MSG_RETURN(E_FAIL, "CBattleFieldGenerator::Export", "Failed to Open File");
		}

		EVENT_TYPE eEventType = m_pSelectedCollider->Get_EventType();
		outFile.write(reinterpret_cast<const char*>(&eEventType), sizeof(EVENT_TYPE));

		string strEventTag = m_pSelectedCollider->Get_EventTag();
		size_t iSize = strEventTag.size();
		outFile.write(reinterpret_cast<const char*>(&iSize), sizeof(size_t));
		outFile.write(strEventTag.c_str(), iSize);

		auto pTransform = dynamic_pointer_cast<CTransform>(m_pSelectedCollider->Get_Component(COMPONENT::TRANSFORM));
		_float4x4 matWorld = pTransform->Get_Matrix();
		outFile.write(reinterpret_cast<const char*>(&matWorld), sizeof(_float4x4));

		outFile.close();
	}

	return S_OK;
}

void CEventColliderGenerator::Render_File_Dialog()
{
	ImGui::BeginGroup();
	ImVec2 vSize = ImGui::GetItemRectSize();

	if (ImGui::Button("Import", ImVec2((vSize.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, vSize.y)))
	{
		if (!m_imfdEmbed_Load.IsOpened())
		{
			if (m_imfdEmbed_Export.IsOpened())
			{
				m_imfdEmbed_Export.Close();
			}

			const _char* szFilters = "All files{.*}";
			m_imfdEmbed_Load.OpenDialog(DIALOG_LOAD, "Open", szFilters, "../Resources/", 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType |
				ImGuiFileDialogFlags_NoDialog |
				ImGuiFileDialogFlags_DisableBookmarkMode |
				ImGuiFileDialogFlags_ReadOnlyFileNameField);
		}
		else
		{
			m_imfdEmbed_Load.Close();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Export", ImVec2((vSize.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, vSize.y)))
	{
		if (!m_imfdEmbed_Export.IsOpened())
		{
			if (m_imfdEmbed_Load.IsOpened())
			{
				m_imfdEmbed_Load.Close();
			}

			const _char* szFilters = "All files{.*}";
			m_imfdEmbed_Export.OpenDialog(DIALOG_EXPORT, "Export", szFilters, "../Resources/", "",
				[](const char*, void*, bool*)
				{
					ImGui::Text("");
				},
				0, 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType |
				ImGuiFileDialogFlags_NoDialog |
				ImGuiFileDialogFlags_DisableBookmarkMode |
				ImGuiFileDialogFlags_ConfirmOverwrite);
		}
		else
		{
			m_imfdEmbed_Export.Close();
		}
	}
	ImGui::EndGroup();

	if (m_imfdEmbed_Load.Display(DIALOG_LOAD, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imfdEmbed_Load.IsOk())
		{
			wstring wstrImportPath = Function::ConvertString(m_imfdEmbed_Load.GetFilePathName());
			Import(wstrImportPath);
		}
		m_imfdEmbed_Load.Close();
	}

	if (m_imfdEmbed_Export.Display(DIALOG_EXPORT, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imfdEmbed_Export.IsOk())
		{
			wstring wstrExportPath = Function::ConvertString(m_imfdEmbed_Export.GetFilePathName());
			Export(wstrExportPath);
		}
		m_imfdEmbed_Export.Close();
	}
}

shared_ptr<CEventColliderGenerator> CEventColliderGenerator::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CEventColliderGenerator> pInstance = make_private_shared(CEventColliderGenerator, pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEventColliderGenerator::Create", "Failed to Initialze_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEventColliderGenerator::Clone(any)
{
	shared_ptr<CEventColliderGenerator> pInstance = make_private_shared_copy(CEventColliderGenerator, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEventColliderGenerator::Clone", "Failed to Initialze");
	}

	return pInstance;
}

#endif