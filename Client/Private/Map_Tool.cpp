#include "ClientPCH.h"
#include "Map_Tool.h"
#include "GameInstance.h"
#include <sstream>
#include "Map_ToolManager.h"
#include "PhysXEngine.h"
#include "Pipeline.h"

#include "Maptool_Create_Tutorial.h"
#include "Maptool_Create_Mizuha.h"
#include "Maptool_Create_Suoh.h"
#include "Maptool_Create_MindRoom.h"
#include "Maptool_Create_Hideout.h"

#if ACTIVATE_TOOL

int g_iSelectedFileIndex = -1;
int g_iSelectedStage1FileIndex = -1;

auto g_pMapToolInstance = CMap_ToolManager::Get_Instance();

CMap_Tool::CMap_Tool(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
}

HRESULT CMap_Tool::Initialize()
{
	m_RoadLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MAPTOOL, LAYER_MAP_ROAD);

	g_pMapToolInstance->Set_Layer(m_RoadLayer);

    return S_OK;
}

void CMap_Tool::Tick(_float fTimeDelta)
{
	Menu();

	if (m_bCreate)
	{
		wstring Name_Number = Object_Numbering(m_strFileName, m_listObjName);
		m_listObjName.emplace_back(Name_Number);
		//m_listObjName.sort();

		for (auto& objName : m_listObjName) {

			m_strObjName = objName;
		}

		g_pMapToolInstance->NameNum_List(Name_Number);

		switch (m_eStage)
		{
		case TUTORIAL :
			Create_Tutorial();
			break;

		case MIZUHA :
			Create_Mizuha();
			Create_NeonLine();
			break;

		case SUOH :
			Create_Suoh();
			break;

		case SUOH_BATTLE:
			Create_Suoh();
			break;

		case MINDROOM:
			Create_MindRoom();
			break;

		case HIDEOUT:
			Create_Hideout();
			break;
		default:
			break;
		}

		m_bCreate = false;
	}

	if (m_bCheck_UI)
	{
		ImGui::SetNextWindowPos(ImVec2(g_iWinCX * 0.5f - 75, g_iWinCY * 0.5f - 200));
		Check_UI();
		m_fTimer += fTimeDelta;

		if (m_fTimer >= 1.f)
		{
			m_bCheck_UI = false;
			m_bCheck[0] = false;
			m_bCheck[1] = false;
			m_bCheck[2] = false;
			m_bCheck[3] = false;
			m_bCheck[4] = false;
			m_fTimer = 0.f;
		}
	}

	Mouse_Ray();
}

void CMap_Tool::Late_Tick(_float fTimeDelta)
{

}

HRESULT CMap_Tool::Render()
{
    return S_OK;
}

void CMap_Tool::Menu()
{
	ImGui::SetNextWindowPos(ImVec2(0, 50));

	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings); // Create a window called "Hello, world!" and append into it.

	ImGui::BeginChild("Pivot", ImVec2(300, 360), true, ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::CollapsingHeader("Select_Scene"))
	{
		ImGui::Checkbox("Tutorial_Scene", &m_bScene_Change[0]);
		ImGui::Checkbox("Mizuha_Scene", &m_bScene_Change[1]);
		ImGui::Checkbox("Suoh_Scene", &m_bScene_Change[2]);
		ImGui::Checkbox("Suoh_Battle_Scene", &m_bScene_Change[3]);
		ImGui::Checkbox("MindRoom_Scene", &m_bScene_Change[4]);
		ImGui::Checkbox("Hideout_Scene", &m_bScene_Change[5]);
	}

	ImGui::Separator();
	ImGui::Spacing();

	switch (m_eStage)
	{
	case TUTORIAL:
		ImGui::Text("Current Scene : Tutorial");
		break;

	case MIZUHA:
		ImGui::Text("Current Scene : Mizuha");
		break;

	case SUOH:
		ImGui::Text("Current Scene : Suoh");
		break;

	case SUOH_BATTLE:
		ImGui::Text("Current Scene : Suoh_Battle");
		break;

	case MINDROOM:
		ImGui::Text("Current Scene : MindRoom");
		break;

	case HIDEOUT:
		ImGui::Text("Current Scene : Hideout");
		break;
	default:
		break;
	}

	Change_Scene();

	if (ImGui::CollapsingHeader("Menu_Close"))
	{
		ImGui::Checkbox("Object Menu", &m_bObject_Menu);
		ImGui::Checkbox("ObjectList Menu", &m_bObjectList_Menu);
		ImGui::Checkbox("Pivot Menu", &m_bPivot_Menu);
		ImGui::Checkbox("ChangePivot Menu", &m_bChangePivotList_Menu);
		ImGui::Checkbox("EmplaceList Menu", &m_bEmplaceList_Menu);
	}

	ImGui::Separator();
	ImGui::Spacing();

	if (m_bPivot_Menu)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 440));
		PivotWindow();
	}

	if (m_bObject_Menu)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 720));
		MenuWindow();
	}

	if (m_bObjectList_Menu)
	{
		ImGui::SetNextWindowPos(ImVec2(1450, 0));
		ObjectListWindow();
	}

	if (m_bEmplaceList_Menu)
	{
		ImGui::SetNextWindowPos(ImVec2(1450, 360));
		EmplaceListWindow();
	}
	if (m_bChangePivotList_Menu)
	{
		/*ImGui::SetNextWindowPos(ImVec2(1450, 690));
		ChangePivotListWindow();*/
	}

	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::Button("Create"))
	{
		m_bCheck_UI = true;
		m_bCheck[0] = true;
		m_bCreate = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete"))
	{
		m_bCheck_UI = true;
		m_bCheck[1] = true;

		m_bDelete = true;

		g_pMapToolInstance->Delete_Object(m_strSelectedName);

		m_listObjName = g_pMapToolInstance->Get_ListNameNum();
		//m_listEmplaceName = g_pMapToolInstance->Get_ListNameNum();
	}

	ImGui::SameLine();

	if (ImGui::Button("Emplace"))
	{
		m_bCheck_UI = true;
		m_bCheck[4] = true;

		m_bEmplace = true;
		g_pMapToolInstance->Set_Emplace(true);
		CMap_ToolManager::OBJECTDESC objDesc;

		objDesc.strName = std::get<0>(m_tuplePivot) = m_strSelectedName;
		objDesc.Scale = std::get<1>(m_tuplePivot);
		objDesc.Rotation = std::get<2>(m_tuplePivot);
		objDesc.Translation = std::get<3>(m_tuplePivot);
		objDesc.matrix = std::get<4>(m_tuplePivot);

		g_pMapToolInstance->Emplace_Object(&objDesc);

		/*auto findItNameNum = std::find(m_listEmplaceName.begin(), m_listEmplaceName.end(), m_strSelectedName);

		if (findItNameNum != m_listEmplaceName.end()) {
			*findItNameNum = m_strSelectedName;
		}
		else {
			m_listEmplaceName.emplace_back(m_strSelectedName);
		}

		for (auto it = m_listObjName.begin(); it != m_listObjName.end();)
		{
			if (*it == m_strSelectedName) {
				it = m_listObjName.erase(it);
			}
			else {
				++it;
			}
		}*/
	}

	ImGui::Separator();
	ImGui::Spacing();

	switch (m_eStage)
	{
	case TUTORIAL:
		Change_Save_Load_Button(0);
		break;
	case MIZUHA:
		Change_Save_Load_Button(1);
		break;
	case SUOH:
		Change_Save_Load_Button(2);
		break;
	case SUOH_BATTLE:
		Change_Save_Load_Button(3);
		break;
	case MINDROOM:
		Change_Save_Load_Button(4);
		break;
	case HIDEOUT:
		Change_Save_Load_Button(5);
		break;
	default:
		break;
	}

	ImGui::Separator();
	ImGui::Spacing();

#ifdef _DEBUG
	if (ImGui::Checkbox("Physx Render", &m_bPhysxRender))
	{
		CGameInstance::Get_Instance()->SetPhysxDrawStatus(m_bPhysxRender);
	}
#endif // _DEBUG

	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::Checkbox("Apply Picking Pos / KeyDown('T')", &m_bApplyPickingPos))
	{
		m_bApplyPos = m_bApplyPickingPos;
	}

	if (CGameInstance::Get_Instance()->Key_Down('T'))
	{
		if(m_bApplyPickingPos == true)
			m_bApplyPos = m_bApplyPickingPos = false;
		else
			m_bApplyPos = m_bApplyPickingPos = true;
	}

	//ImGui::Separator();
	//ImGui::Spacing();

	//if (ImGui::Checkbox("Picking Object / KeyDown('V')", &m_bPickingObjectMode))
	//{
	//	//m_bPickObjMode = m_bPickingObjectMode;
	//}

	//if (CGameInstance::Get_Instance()->Key_Down('V'))
	//{
	//	if (m_bPickingObjectMode == true)
	//		/*m_bPickObjMode = */m_bPickingObjectMode = false;
	//	else
	//		/*m_bPickObjMode = */m_bPickingObjectMode = true;
	//}

	if (m_bSave)
	{
		m_bSave = g_pMapToolInstance->Get_Save();
	}
	if (m_bLoad)
	{
		m_bLoad = g_pMapToolInstance->Get_Load();
	}
	if (m_bEmplace)
	{
		m_bEmplace = g_pMapToolInstance->Get_Emplace();
	}
	if (m_bDelete)
	{
		m_bDelete = g_pMapToolInstance->Get_Delete();
	}

	ImGui::Separator();
	ImGui::Spacing();

	ImGui::BulletText("Mouse Ray Pos");
	ImGui::Text("X : %f", m_MousePos.x);
	ImGui::SameLine();
	ImGui::Text("Y : %f", m_MousePos.y);
	ImGui::SameLine();
	ImGui::Text("Z : %f", m_MousePos.z);

	ImGui::Separator();
	ImGui::Spacing();

	ImGui::BulletText("Picking Pos");
	ImGui::Text("X : %f", m_PickingPos.x);
	ImGui::SameLine();
	ImGui::Text("Y : %f", m_PickingPos.y);
	ImGui::SameLine();
	ImGui::Text("Z : %f", m_PickingPos.z);

	g_pMapToolInstance->Set_PickingPos(m_PickingPos);

	/*ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::Button("All Clear"))
	{
		g_pMapToolInstance->Clear_List(true);
		m_listObjName.clear();
	}*/

	ImGui::EndChild();
	ImGui::End();
}

void CMap_Tool::PivotWindow()
{
	ImGui::Begin("Pivot Window", &m_bPivot_Menu, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::BeginChild("Pivot", ImVec2(300, 250), true, ImGuiWindowFlags_HorizontalScrollbar);

	static _float3		vPivotScale(1.f, 1.f, 1.f);
	static _float3		vPivotRotation(0.f, 0.f, 0.f);
	static _float3		vPivotTranslation(0.f, 0.f, 0.f);
	static _float4x4	mPivot = g_mUnit;

	static _uint iTab(1);

	if (m_bApplyPos)
	{
		if (CGameInstance::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			vPivotTranslation = m_PickingPos;
		}
	}

	if (ImGui::BeginTabBar("Tab:Pivot"))
	{
		if (ImGui::BeginTabItem("Value"))
		{
			if (ImGui::Button("Reset Pivot"))
			{
				vPivotScale = _float3(1.f, 1.f, 1.f);
				vPivotRotation = _float3(0.f, 0.f, 0.f);
				vPivotTranslation = _float3(0.f, 0.f, 0.f);
			}

			if (iTab == 2)
			{
				iTab = 1;
				_vector vScale, vQuaternion, vTranslation;
				if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mPivot))
				{
					vPivotScale = vScale;
					vPivotRotation = Function::QuaternionToEuler(vQuaternion, false);
					vPivotTranslation = vTranslation;
					
				}
				else
				{
					MSG_BOX("CTool_Model:Tool_System", "Failed to XMMatrixDecompose");
				}
			}

			ImGui::DragFloat3("Scale", reinterpret_cast<_float*>(&vPivotScale));
			//ImGui::InputFloat3("Rotation(Degree)", reinterpret_cast<_float*>(&vPivotRotation));
			ImGui::InputFloat("RotateX", &vPivotRotation.x, 1.f, vPivotRotation.x, "%.2f");
			ImGui::InputFloat("RotateY", &vPivotRotation.y, 1.f, vPivotRotation.y, "%.2f");
			ImGui::InputFloat("RotateZ", &vPivotRotation.z, 1.f, vPivotRotation.z, "%.2f");

			ImGui::DragFloat3("Translation", reinterpret_cast<_float*>(&vPivotTranslation));

			ImGui::InputFloat("TranslationX", &vPivotTranslation.x, 0.1f, vPivotTranslation.x, "%.2f");
			ImGui::InputFloat("TranslationY", &vPivotTranslation.y, 0.1f, vPivotTranslation.y, "%.2f");
			ImGui::InputFloat("TranslationZ", &vPivotTranslation.z, 0.1f, vPivotTranslation.z, "%.2f");

			ImGui::EndTabItem();

			// 기존의 QuaternionToEuler 함수를 사용하지 않고 XMQuaternionRotationRollPitchYaw 함수를 사용+

			//_vector Quaternion = XMQuaternionRotationRollPitchYaw
			XMMATRIX mRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(vPivotRotation.x), XMConvertToRadians(vPivotRotation.y), XMConvertToRadians(vPivotRotation.z));

			// 회전 행렬을 기존의 피봇 행렬에 적용
			mPivot = XMMatrixScalingFromVector(vPivotScale) * mRotation * XMMatrixTranslationFromVector(vPivotTranslation);

			/*mPivot = XMMatrixAffineTransformation(vPivotScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
				_float3(XMConvertToRadians(vPivotRotation.x),
					XMConvertToRadians(vPivotRotation.y), XMConvertToRadians(vPivotRotation.z))), vPivotTranslation);*/

			std::get<1>(m_tuplePivot) = vPivotScale;
			std::get<2>(m_tuplePivot) = vPivotRotation;
			std::get<3>(m_tuplePivot) = vPivotTranslation;
			std::get<4>(m_tuplePivot) = mPivot;

			g_pMapToolInstance->Set_TuplePivot(m_tuplePivot);
		}

		if (ImGui::BeginTabItem("Matrix"))
		{
			if (ImGui::Button("Reset Pivot"))
			{
				vPivotScale = _float3(1.f, 1.f, 1.f);
				vPivotRotation = _float3(0.f, 0.f, 0.f);
				vPivotTranslation = _float3(0.f, 0.f, 0.f);
				mPivot = g_mUnit;
			}

			if (iTab == 1)
			{
				iTab = 2;

				// 기존의 QuaternionToEuler 함수를 사용하지 않고 XMQuaternionRotationRollPitchYaw 함수를 사용
				XMMATRIX mRotation = XMMatrixRotationRollPitchYaw(vPivotRotation.x, vPivotRotation.y, vPivotRotation.z);

				// 회전 행렬을 기존의 피봇 행렬에 적용
				mPivot = XMMatrixScalingFromVector(vPivotScale) * mRotation * XMMatrixTranslationFromVector(vPivotTranslation);
				/*mPivot = XMMatrixAffineTransformation(vPivotScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
					_float3(XMConvertToRadians(vPivotRotation.x), XMConvertToRadians(vPivotRotation.y), XMConvertToRadians(vPivotRotation.z))), vPivotTranslation);*/
			}

			ImGui::DragFloat4("Right", reinterpret_cast<_float*>(&mPivot._11));
			ImGui::DragFloat4("Up", reinterpret_cast<_float*>(&mPivot._21));
			ImGui::DragFloat4("Look", reinterpret_cast<_float*>(&mPivot._31));
			ImGui::DragFloat4("Position", reinterpret_cast<_float*>(&mPivot._41));

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::EndChild();
	ImGui::End();
}

void CMap_Tool::MenuWindow()
{
	ImGui::Begin("Object Window", &m_bObject_Menu, ImGuiWindowFlags_AlwaysAutoResize);

	if (!m_strFileName.empty()) {
		ImGui::Text("Selected File: %ls", m_strFileName.c_str());
	}
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::BeginChild("Object List", ImVec2(300, 250), true, ImGuiWindowFlags_HorizontalScrollbar);

	switch (m_eStage)
	{
	case TUTORIAL:
		Use_FileList(TEXT("Bin/Resources/Map/Tutorial/mdl/"), "Tutorial_mdl", true);
		break;
	case MIZUHA:
		Use_FileList(TEXT("Bin/Resources/Map/Stage1/mdl/"), "Mizuha_mdl", true);
		break;
	case SUOH:
		Use_FileList(TEXT("Bin/Resources/Map/Suoh/mdl/"), "Suoh_mdl", true);
		Use_FileList(TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/"), "Signboard_mdl", true);
		break;
	case SUOH_BATTLE:
		Use_FileList(TEXT("Bin/Resources/Map/Suoh/mdl/"), "Suoh_mdl", true);
		Use_FileList(TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/"), "Signboard_mdl", true);
		break;
	case MINDROOM:
		Use_FileList(TEXT("Bin/Resources/Map/MindRoom/mdl/"), "MindRoom_mdl", true);
		break;
	case HIDEOUT:
		Use_FileList(TEXT("Bin/Resources/Map/Hideout/mdl/"), "Hideout_mdl", true);
		break;
	default:
		break;
	}

#pragma region NeonLine
	wstring directoryNeonmdlPath = TEXT("Bin/Resources/Map/Neon/");

	vector<wstring> NeonmdiFileNames = mdlFileNames(directoryNeonmdlPath);
	if (ImGui::CollapsingHeader("NeonLine_mdl"))
	{
		for (int i = 0; i < NeonmdiFileNames.size(); i++) {

			string fileNameUtf8;

			_int size = WideCharToMultiByte(CP_UTF8, 0, NeonmdiFileNames[i].c_str(), -1, nullptr, 0, nullptr, nullptr);
			fileNameUtf8.resize(size);
			WideCharToMultiByte(CP_UTF8, 0, NeonmdiFileNames[i].c_str(), -1, &fileNameUtf8[0], size, nullptr, nullptr);

			if (ImGui::Selectable(fileNameUtf8.c_str())) {
				g_iSelectedStage1FileIndex = i;
			}
		}
		if (g_iSelectedStage1FileIndex != -1)
		{
			ImGui::Separator();
			ImGui::Spacing();

			wstring selectedFileName = NeonmdiFileNames[g_iSelectedStage1FileIndex];

			size_t pos = selectedFileName.find(TEXT(".mdl"));

			if (pos != wstring::npos)
			{
				selectedFileName = selectedFileName.substr(0, pos);
			}

			m_strFileName = selectedFileName;
		}
	}
#pragma endregion

	ImGui::EndChild();
	ImGui::End();
}

void CMap_Tool::ObjectListWindow()
{
	ImGui::Begin("ObjectList Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (!m_strSelectedName.empty())
	{
		ImGui::Text("Selected Name: %ls", m_strSelectedName.c_str());
	}

	ImGui::Separator();
	ImGui::Spacing();

	ImGui::BeginChild("ObjectList", ImVec2(450, 300), true, ImGuiWindowFlags_HorizontalScrollbar);

	string CollapsingHeaderName = "";

	switch (m_eStage)
	{
	case TUTORIAL:
		CollapsingHeaderName = "Create Tutorial ObjectList";
		break;
	case MIZUHA:
		CollapsingHeaderName = "Create Mizuha ObjectList";
		break;
	case SUOH:
		CollapsingHeaderName = "Create Suoh ObjectList";
		break;
	case SUOH_BATTLE:
		CollapsingHeaderName = "Create Suoh_Battle ObjectList";
		break;
	case MINDROOM:
		CollapsingHeaderName = "Create MindRoom ObjectList";
		break;
	case HIDEOUT:
		CollapsingHeaderName = "Create Hideout ObjectList";
		break;
	default:
		break;
	}

	if (CollapsingHeaderName != "")
	{
		if (ImGui::CollapsingHeader(CollapsingHeaderName.c_str()))
		{
			//m_listObjName.sort();

			for (auto& objName : m_listObjName) {

				string ObjNameStr(objName.begin(), objName.end());

				if (ImGui::TreeNode(ObjNameStr.c_str())) {

					if (ImGui::IsItemClicked()) {
						m_strSelectedName = objName;
						g_pMapToolInstance->Set_SelectName(m_strSelectedName);
					}
					ImGui::TreePop();
				}
			}
		}
	}

	/*if (m_bPickingObjectMode)
	{
		g_pMapToolInstance->Set_PickObjMode(true);

		if (g_pMapToolInstance->Get_PickingObject())
		{
			m_strSelectedName = g_pMapToolInstance->Get_PickObjName();
		}
	}
	else
	{
		g_pMapToolInstance->Set_PickObjMode(false);
		g_pMapToolInstance->Set_PickingObject(false);
	}*/

	ImGui::EndChild();
	ImGui::End();
}

void CMap_Tool::EmplaceListWindow()
{
	ImGui::Begin("EmplaceList Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::BeginChild("EmplaceList", ImVec2(450, 300), true, ImGuiWindowFlags_HorizontalScrollbar);

	if (!m_strSelectedName.empty())
	{
		ImGui::Text("Selected Name: %ls", m_strSelectedName.c_str());
	}

	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("EmplaceList"))
	{
		for (auto& objName : m_listEmplaceName) {

			string ObjNameStr(objName.begin(), objName.end());

			if (ImGui::TreeNode(ObjNameStr.c_str())) {

				if (ImGui::IsItemClicked()) {
					m_strSelectedName = objName;
				}

				ImGui::TreePop();
			}
		}
	}

	ImGui::EndChild();
	ImGui::End();
}

void CMap_Tool::ChangePivotListWindow()
{
	ImGui::Begin("ChangePivotList Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::BeginChild("ChangePivotList", ImVec2(450, 300), true, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::Text("After pivot Emplaced , Change pivot again. \nIf you wanna Emplace to change pivot, Again press Emplace button ");

	ImGui::Separator();
	ImGui::Spacing();

	if (!m_strSelectedName.empty())
	{
		ImGui::Text("Selected Name: %ls", m_strSelectedName.c_str());
	}

	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("ChangePivotList"))
	{
		for (auto& objName : m_listObjName) {

			string ObjNameStr(objName.begin(), objName.end());

			if (ImGui::TreeNode(ObjNameStr.c_str())) {

				if (ImGui::IsItemClicked()) {
					m_strSelectedName = objName;
				}

				ImGui::TreePop();
			}
		}
	}

	ImGui::EndChild();
	ImGui::End();
}

void CMap_Tool::Check_UI()
{
	ImGui::Begin("Check", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (m_bCheck[0])
		ImGui::Text("Create Complete");

	if (m_bCheck[1])
		ImGui::Text("Delete Complete");

	if (m_bCheck[2])
		ImGui::Text("Save Complete");

	if (m_bCheck[3])
		ImGui::Text("Load Complete");

	if (m_bCheck[4])
		ImGui::Text("Emplace Complete");

	ImGui::End();
}

void CMap_Tool::Create_NeonLine()
{
	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
	{ SCENE::MAPTOOL, m_strObjName,std::get<1>(m_tuplePivot),std::get<2>(m_tuplePivot),std::get<3>(m_tuplePivot),std::get<4>(m_tuplePivot), m_strObjName };

	if (m_strFileName == TEXT("NeonCornerA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonCornerB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonCornerC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonCornerD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonCrashA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonCrashB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonLineA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonLineB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonRollA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonSwayA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonSwayB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	else if (m_strFileName == TEXT("NeonSwayC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

}

vector<wstring> CMap_Tool::mdlFileNames(const wstring& strPath)
{
	vector<wstring> fileNames;

	for (auto& entry : fs::directory_iterator(strPath)) {
		if (entry.path().extension() == TEXT(".mdl")) {
			fileNames.push_back(entry.path().filename().wstring());
		}
	}
	return fileNames;
}

void CMap_Tool::Use_FileList(wstring strFilePath, string HeaderName, _bool bCheck)
{
	if (bCheck)
	{
		vector<wstring> mdiFileNames = mdlFileNames(strFilePath);
		if (ImGui::CollapsingHeader(HeaderName.c_str()))
		{
			for (int i = 0; i < mdiFileNames.size(); i++) {

				string fileNameUtf8;

				_int size = WideCharToMultiByte(CP_UTF8, 0, mdiFileNames[i].c_str(), -1, nullptr, 0, nullptr, nullptr);
				fileNameUtf8.resize(size);
				WideCharToMultiByte(CP_UTF8, 0, mdiFileNames[i].c_str(), -1, &fileNameUtf8[0], size, nullptr, nullptr);

				if (ImGui::Selectable(fileNameUtf8.c_str())) {
					g_iSelectedFileIndex = i;
				}
			}
			if (g_iSelectedFileIndex != -1)
			{
				ImGui::Separator();
				ImGui::Spacing();

				wstring selectedmdlFileName = mdiFileNames[g_iSelectedFileIndex];

				size_t pos = selectedmdlFileName.find(TEXT(".mdl"));

				if (pos != wstring::npos)
				{
					selectedmdlFileName = selectedmdlFileName.substr(0, pos);
				}

				m_strFileName = selectedmdlFileName;
			}
		}
	}
}

wstring CMap_Tool::Object_Numbering(wstring& baseName, list<wstring>& ListNames)
{
	wstring Name = baseName;

	/*if (find(ListNames.begin(), ListNames.end(), baseName) == ListNames.end()) {
		return Name;
	}*/

	_int number = 0;

	do {
		std::wstringstream ss;
		ss << baseName << L"_number" << number;
		Name = ss.str();

		number++;
	} while (std::find(ListNames.begin(), ListNames.end(), Name) != ListNames.end());

	return Name;
}

void CMap_Tool::Change_Save_Load_Button(_int iIndex)
{
	string SaveButton;
	string LoadButton;

	_int	iNumber = { 0 };

	switch (iIndex)
	{
	case 0:
		SaveButton = "Tutorial_Save";
		LoadButton = "Tutorial_Load";
		iNumber = 0;
		break;
	case 1:
		SaveButton = "Mizuha_Save";
		LoadButton = "Mizuha_Load";
		iNumber = 1;
		break;
	case 2:
		SaveButton = "Suoh_Save";
		LoadButton = "Suoh_Load";
		iNumber = 2;
		break;
	case 3:
		SaveButton = "Suoh_Battle_Save";
		LoadButton = "Suoh_Battle_Load";
		iNumber = 3;
		break;
	case 4:
		SaveButton = "MindRoom_Save";
		LoadButton = "MindRoom_Load";
		iNumber = 4;
		break;
	case 5:
		SaveButton = "Hideout_Save";
		LoadButton = "Hideout_Load";
		iNumber = 5;
		break;
	default:
		break;
	}

	if (ImGui::Button(SaveButton.c_str()))
	{
		m_bCheck_UI = true;
		m_bCheck[2] = true;

		m_bSave = true;

		g_pMapToolInstance->Set_Save(true);
		g_pMapToolInstance->Set_StageNum(iNumber);
		g_pMapToolInstance->Emplace_Object();
	}

	ImGui::SameLine();

	if (ImGui::Button(LoadButton.c_str()))
	{
		m_bCheck_UI = true;
		m_bCheck[3] = true;

		if (m_bLoadWindow)
		{
			m_bLoadWindow = false;
			ImGui::SetNextWindowPos(ImVec2(300, 50));
		}
		else
			m_bLoadWindow = true;

		m_bLoad = true;

		g_pMapToolInstance->Set_Load(true);
		
		switch (iNumber)
		{
		case 0 :
			g_pMapToolInstance->Ready_TestMap();
			break;
		case 1:
			g_pMapToolInstance->Ready_MizuhaMap();
			break;
		case 2:
			g_pMapToolInstance->Ready_SuohMap(2);
			break;
		case 3:
			g_pMapToolInstance->Ready_SuohMap(3);
			break;
		case 4:
			g_pMapToolInstance->Ready_MindRoom();
			break;
		case 5:
			g_pMapToolInstance->Ready_Hideout();
			break;
		default:
			break;
		}

		m_listObjName = g_pMapToolInstance->Get_ListNameNum();
		//m_listEmplaceName = g_pMapToolInstance->Get_ListNameNum();
	}

}

void CMap_Tool::Change_Scene()
{
	if (m_bScene_Change[0])
	{
		m_bScene_Change[1] = false;
		m_bScene_Change[2] = false;
		m_bScene_Change[3] = false;
		m_bScene_Change[4] = false;
		m_bScene_Change[5] = false;

		m_eStage = STAGE::TUTORIAL;
	}
	if (m_bScene_Change[1])
	{
		m_bScene_Change[0] = false;
		m_bScene_Change[2] = false;
		m_bScene_Change[3] = false;
		m_bScene_Change[4] = false;
		m_bScene_Change[5] = false;

		m_eStage = STAGE::MIZUHA;
	}
	if (m_bScene_Change[2])
	{
		m_bScene_Change[0] = false;
		m_bScene_Change[1] = false;
		m_bScene_Change[3] = false;
		m_bScene_Change[4] = false;
		m_bScene_Change[5] = false;

		m_eStage = STAGE::SUOH;
	}
	if (m_bScene_Change[3])
	{
		m_bScene_Change[0] = false;
		m_bScene_Change[1] = false;
		m_bScene_Change[2] = false;
		m_bScene_Change[4] = false;
		m_bScene_Change[5] = false;

		m_eStage = STAGE::SUOH_BATTLE;
	}
	if (m_bScene_Change[4])
	{
		m_bScene_Change[0] = false;
		m_bScene_Change[1] = false;
		m_bScene_Change[2] = false;
		m_bScene_Change[3] = false;
		m_bScene_Change[5] = false;

		m_eStage = STAGE::MINDROOM;
	}
	if (m_bScene_Change[5])
	{
		m_bScene_Change[0] = false;
		m_bScene_Change[1] = false;
		m_bScene_Change[2] = false;
		m_bScene_Change[3] = false;
		m_bScene_Change[4] = false;

		m_eStage = STAGE::HIDEOUT;
	}
}

void CMap_Tool::Mouse_Ray()
{
	POINT		ptMouse = CGameInstance::Get_Instance()->Get_CursorPos();
	_matrix		mView = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW);
	_matrix		mProj = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);
	_vector		vTemp;
	_matrix		mInvView = XMMatrixInverse(&vTemp, mView);
	_matrix		mInvProj = XMMatrixInverse(&vTemp, mProj);
	_float3		vMouseNearPos;
	_vector		vecMouseNearPos;
	RECT		rcClient;
	HWND		hWnd = g_pMapToolInstance->Get_Hwnd();

	GetClientRect(hWnd, &rcClient);

	vMouseNearPos.x = ptMouse.x / (rcClient.right * 0.5f) - 1.f;
	vMouseNearPos.y = -ptMouse.y / (rcClient.bottom * 0.5f) + 1.f;
	vMouseNearPos.z = 0.f;

	vecMouseNearPos = XMLoadFloat3(&vMouseNearPos);
	vecMouseNearPos = XMVector3TransformCoord(vecMouseNearPos, mInvProj);

	_vector MousePos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	m_vMouseRay = vecMouseNearPos - MousePos;

	m_vMouseRayPos = XMVector4Transform(MousePos, mInvProj);
	m_vMouseRay = XMVector4Transform(m_vMouseRay, mInvView);

	_vector vCamPos = g_pMapToolInstance->Get_CameraPos();

	PxRaycastBuffer CallBack;
	PxQueryFilterData FilterData;
	m_vMouseRay = XMVector3Normalize(m_vMouseRay);
	FilterData.flags = PxQueryFlag::Enum::eSTATIC;

	if (CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->raycast(
		PxVec3(XMVectorGetX(vCamPos), XMVectorGetY(vCamPos), XMVectorGetZ(vCamPos)),
		PxVec3(XMVectorGetX(m_vMouseRay), XMVectorGetY(m_vMouseRay), XMVectorGetZ(m_vMouseRay)),
		300.f,
		CallBack,
		PxHitFlag::Enum::eMESH_BOTH_SIDES, FilterData))
	{
		PxVec3 HitPos = CallBack.block.position;

		m_MousePos.x = HitPos.x;
		m_MousePos.y = HitPos.y;
		m_MousePos.z = HitPos.z;
	}

	if (CGameInstance::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->raycast(
			PxVec3(XMVectorGetX(vCamPos), XMVectorGetY(vCamPos), XMVectorGetZ(vCamPos)),
			PxVec3(XMVectorGetX(m_vMouseRay), XMVectorGetY(m_vMouseRay), XMVectorGetZ(m_vMouseRay)),
			300.f,
			CallBack,
			PxHitFlag::Enum::eMESH_BOTH_SIDES, FilterData))
		{
			PxVec3 HitPos = CallBack.block.position;

			m_PickingPos.x = HitPos.x;
			m_PickingPos.y = HitPos.y;
			m_PickingPos.z = HitPos.z;
		}
	}
}

shared_ptr<CMap_Tool> CMap_Tool::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMap_Tool> pInstance = make_private_shared(CMap_Tool, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CMap_Tool::Create", "Failed to Initialize");
	}

	return pInstance;
}

#endif
