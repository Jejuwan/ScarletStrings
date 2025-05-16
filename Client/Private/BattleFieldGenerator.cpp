#include "ClientPCH.h"
#include "BattleFieldGenerator.h"
#include "GameInstance.h"

#include "PointGizmo.h"
#include "Map_Tool.h"
#include "ModelRenderer.h"
#include "BattleFieldWall.h"

#ifdef _DEBUG

#define DIALOG_LOAD			"Dialog::Load"
#define DIALOG_EXPORT		"Dialog::Export"

CBattleFieldGenerator::CBattleFieldGenerator(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CBattleFieldGenerator::CBattleFieldGenerator(const CBattleFieldGenerator& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBattleFieldGenerator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattleFieldGenerator::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CBattleFieldGenerator::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CBattleFieldGenerator::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#if ACTIVATE_IMGUI

	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("BattleField Generator"))
	{
	if (ImGui::CollapsingHeader("Transform"))
	{
		if (m_pSelectedGizmo)
		{
			ImGui::SeparatorText("Object Selected");
			auto pTransform = dynamic_pointer_cast<CTransform>(m_pSelectedGizmo->Get_Component(COMPONENT::TRANSFORM));

			_float3 vObjectPosition = pTransform->Get_State(TRANSFORM::POSITION);
			if (ImGui::DragFloat3("Position", &vObjectPosition.x))
			{
				pTransform->Set_State(TRANSFORM::POSITION, vObjectPosition);
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

		_uint iNumPoints = (_uint)m_lstPointGizmo.size();
		for (_uint i = 0; i < iNumPoints; ++i)
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
				m_iSelectedGizmoIndex = (_int)i;
				auto pSelectedGizmo = *std::next(m_lstPointGizmo.begin(), m_iSelectedGizmoIndex);

				if (m_pSelectedGizmo)
					m_pSelectedGizmo->On_Selected(false);

				m_pSelectedGizmo = pSelectedGizmo;
				m_pSelectedGizmo->On_Selected(true);
			}

			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

		if (CGameInstance::Get_Instance()->Key_Down(VK_DELETE))
		{
			if (false == m_lstPointGizmo.empty())
				m_lstPointGizmo.pop_back();
			if (false == m_lstBattleField.empty())
				m_lstBattleField.pop_back();
		}

		if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
		{
			auto pFirst = dynamic_pointer_cast<CTransform>((*m_lstPointGizmo.begin())->Get_Component(COMPONENT::TRANSFORM));
			auto pLast = dynamic_pointer_cast<CTransform>((*m_lstPointGizmo.rbegin())->Get_Component(COMPONENT::TRANSFORM));

			_vector vFirstPosition = pFirst->Get_State(TRANSFORM::POSITION);
			_vector vLastPosition = pLast->Get_State(TRANSFORM::POSITION);

			auto pObject = CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_BATTLEFIELD_WALL);

			auto pBattleField = dynamic_pointer_cast<CBattleFieldWall>(pObject);
			m_lstBattleField.emplace_back(pBattleField);

			auto pBattleFieldTransform = dynamic_pointer_cast<CTransform>(pObject->Get_Component(COMPONENT::TRANSFORM));

			Create_BattleField(vLastPosition, vFirstPosition, pBattleFieldTransform);

			shared_ptr<CPointGizmo> pPointGizmo = CPointGizmo::Create(m_pDevice, m_pContext);
			auto pTransform = dynamic_pointer_cast<CTransform>(pPointGizmo->Get_Component(COMPONENT::TRANSFORM));
			pTransform->Set_State(TRANSFORM::POSITION, vFirstPosition);
			m_lstPointGizmo.push_back(pPointGizmo);
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
			shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), TEXT("BattleField"));
			assert(pLayer && "CBattleFieldGenerator::Find_Layer: BattleField Failed");

			shared_ptr<CPointGizmo> pPointGizmo = CPointGizmo::Create(m_pDevice, m_pContext);
			if (pPointGizmo)
			{
				auto pTransform = dynamic_pointer_cast<CTransform>(pPointGizmo->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Set_State(TRANSFORM::POSITION, m_vCurrentPoint);
				m_lstPointGizmo.push_back(pPointGizmo);

				if (m_lstPointGizmo.size() >= 2)
				{
					auto pObject = CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_BATTLEFIELD_WALL);
					
					auto pBattleField = dynamic_pointer_cast<CBattleFieldWall>(pObject);
					m_lstBattleField.emplace_back(pBattleField);

					auto pBattleFieldTransform = dynamic_pointer_cast<CTransform>(pObject->Get_Component(COMPONENT::TRANSFORM));

					_float3 vPivotPoint = dynamic_pointer_cast<CTransform>((*std::next(m_lstPointGizmo.rbegin(), 1))->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
					_vector pPenultimate = XMLoadFloat3(&vPivotPoint);
					vPivotPoint = dynamic_pointer_cast<CTransform>((*m_lstPointGizmo.rbegin())->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
					_vector pLast = XMLoadFloat3(&vPivotPoint);

					Create_BattleField(pPenultimate, pLast, pBattleFieldTransform);
				}
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

				if (m_pSelectedGizmo)
				{
					auto pTransform = dynamic_pointer_cast<CTransform>(m_pSelectedGizmo->Get_Component(COMPONENT::TRANSFORM));
					pTransform->Set_State(TRANSFORM::POSITION, m_vCurrentPoint);

					auto pLeft = (m_iSelectedGizmoIndex - 1) >= 0 ? std::next(m_lstPointGizmo.begin(), m_iSelectedGizmoIndex - 1) : m_lstPointGizmo.end();
					auto pCenter = std::next(m_lstPointGizmo.begin(), m_iSelectedGizmoIndex);
					auto pRight = std::next(m_lstPointGizmo.begin(), m_iSelectedGizmoIndex + 1);

					if (pLeft != m_lstPointGizmo.end())
					{
						auto pLeftGizmo = *pLeft;
						auto pCenterGizmo = *pCenter;

						_vector vLeftPivot = dynamic_pointer_cast<CTransform>(pLeftGizmo->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
						_vector vCenterPivot = dynamic_pointer_cast<CTransform>(pCenterGizmo->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);

						auto pBattleField = *std::next(m_lstBattleField.begin(), m_iSelectedGizmoIndex - 1);
						Create_BattleField(vLeftPivot, vCenterPivot, dynamic_pointer_cast<CTransform>(pBattleField->Get_Component(COMPONENT::TRANSFORM)));
					}
					if (pRight != m_lstPointGizmo.end())
					{
						auto pCenterGizmo = *pCenter;
						auto pRightGizmo = *pRight;

						_vector vCenterPivot = dynamic_pointer_cast<CTransform>(pCenterGizmo->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
						_vector vRightPivot = dynamic_pointer_cast<CTransform>(pRightGizmo->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);

						auto pBattleField = *std::next(m_lstBattleField.begin(), m_iSelectedGizmoIndex);
						Create_BattleField(vCenterPivot, vRightPivot, dynamic_pointer_cast<CTransform>(pBattleField->Get_Component(COMPONENT::TRANSFORM)));
					}
				}
			}
		}

		ImGui::Spacing();

		ImGui::SeparatorText("Keyframe Items");
		ImGui::Text("Current Selected Keyframe #%d", m_iSelectedGizmoIndex);

		ImGui::SmallButton("-");
		ImGui::SameLine();
		if (ImGui::Selectable("Reset", false))
		{
			m_iSelectedGizmoIndex = 0;
			m_pSelectedGizmo = nullptr;
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

	for (auto& Iter : m_lstPointGizmo)
	{
		Iter->Tick(fTimeDelta);
	}
	for (auto& Iter : m_lstBattleField)
	{
		Iter->Tick(fTimeDelta);
	}
}

void CBattleFieldGenerator::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	for (auto& Iter : m_lstPointGizmo)
	{
		Iter->Late_Tick(fTimeDelta);
	}
	for (auto& Iter : m_lstBattleField)
	{
		Iter->Late_Tick(fTimeDelta);
	}
}

HRESULT CBattleFieldGenerator::Render()
{
	return S_OK;
}

void CBattleFieldGenerator::Bind_MapTool(shared_ptr<CMap_Tool> pMapTool)
{
	m_pMapTool = pMapTool;
}

HRESULT CBattleFieldGenerator::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBattleFieldGenerator::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

HRESULT CBattleFieldGenerator::Import(const wstring& wstrImportPath)
{
	std::ifstream inFile(wstrImportPath, std::ios::binary);
	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CBattleFieldGenerator::Import", "Failed to Open File");
	}

	// Read BattleField
	{
		_uint iNumBattleField = 0;
		inFile.read(reinterpret_cast<char*>(&iNumBattleField), sizeof(_uint));

		m_lstBattleField.clear();

		for (_uint i = 0; i < iNumBattleField; ++i)
		{
			_float4x4 matWorld;
			inFile.read(reinterpret_cast<char*>(&matWorld), sizeof(_float4x4));

			auto pObject = CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_BATTLEFIELD_WALL);

			auto pBattleField = dynamic_pointer_cast<CBattleFieldWall>(pObject);
			m_lstBattleField.emplace_back(pBattleField);

			auto pBattleFieldTransform = dynamic_pointer_cast<CTransform>(pObject->Get_Component(COMPONENT::TRANSFORM));
			pBattleFieldTransform->Set_Matrix(matWorld);
		}
	}
	
	// Read Points
	{
		_uint iNumPoints = 0;
		inFile.read(reinterpret_cast<char*>(&iNumPoints), sizeof(_uint));

		m_lstPointGizmo.clear();

		for (_uint i = 0; i < iNumPoints; ++i)
		{
			_float3 vPoint;
			inFile.read(reinterpret_cast<char*>(&vPoint), sizeof(_float3));

			shared_ptr<CPointGizmo> pPointGizmo = CPointGizmo::Create(m_pDevice, m_pContext);

			auto pTransform = dynamic_pointer_cast<CTransform>(pPointGizmo->Get_Component(COMPONENT::TRANSFORM));
			pTransform->Set_State(TRANSFORM::POSITION, vPoint);
			m_lstPointGizmo.push_back(pPointGizmo);
		}
	}

	return S_OK;
}

HRESULT CBattleFieldGenerator::Export(const wstring& wstrExportPath)
{
	std::ofstream outFile(wstrExportPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CBattleFieldGenerator::Export", "Failed to Open File");
	}

	_uint iNumBattleFiled = (_uint)m_lstBattleField.size();
	outFile.write(reinterpret_cast<const char*>(&iNumBattleFiled), sizeof(_uint));

	for (auto& Iter : m_lstBattleField)
	{
		const _float4x4& matWorld = dynamic_pointer_cast<CTransform>(Iter->Get_Component(COMPONENT::TRANSFORM))->Get_Matrix();
		outFile.write(reinterpret_cast<const char*>(&matWorld), sizeof(_float4x4));
	}

	_uint iNumPoints = (_uint)m_lstPointGizmo.size();
	outFile.write(reinterpret_cast<const char*>(&iNumPoints), sizeof(_uint));

	for (auto& Iter : m_lstPointGizmo)
	{
		const _float3 vPosition = dynamic_pointer_cast<CTransform>(Iter->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
		outFile.write(reinterpret_cast<const char*>(&vPosition), sizeof(_float3));
	}

	outFile.close();

	return S_OK;
}

void CBattleFieldGenerator::Render_File_Dialog()
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

void CBattleFieldGenerator::Create_BattleField(_vector vLeftPivot, _vector vRightPivot, shared_ptr<CTransform> pTransform)
{
	_vector vRight = (vLeftPivot - vRightPivot) * 0.25f;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vLook = XMVector3Normalize(XMVector3Cross(XMVector3Normalize(vRight), vUp));
	_vector vTranslation = vLeftPivot;

	_matrix matWorld;
	matWorld.r[0] = vRight;
	matWorld.r[1] = vUp;
	matWorld.r[2] = vLook;
	matWorld.r[3] = XMVectorSet(XMVectorGetX(vLeftPivot), XMVectorGetY(vLeftPivot), XMVectorGetZ(vLeftPivot), 1.f);

	pTransform->Set_Matrix(matWorld);
}

shared_ptr<CBattleFieldGenerator> CBattleFieldGenerator::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CBattleFieldGenerator> pInstance = make_private_shared(CBattleFieldGenerator, pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CBattleFieldGenerator::Create", "Failed to Initialze_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CBattleFieldGenerator::Clone(any)
{
	shared_ptr<CBattleFieldGenerator> pInstance = make_private_shared_copy(CBattleFieldGenerator, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CBattleFieldGenerator::Clone", "Failed to Initialze");
	}

	return pInstance;
}

#endif