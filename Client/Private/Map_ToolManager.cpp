#include "ClientPCH.h"
#include "Map_ToolManager.h"

#include "GameInstance.h"
#include "Player.h"
#include "PlayerBrainFieldExitState.h"
#include "Scene_MindRoom.h"
#include <set>
#include <regex>

HRESULT CMap_ToolManager::NameNum_List(wstring NameNum)
{
	m_listNameNum.emplace_back(NameNum);

	return S_OK;
}

void CMap_ToolManager::Tick(_float fTimeDelta)
{
	//if (CGameInstance::Get_Instance()->Key_Down('5') && CGameInstance::Get_Instance()->Current_Scene() == SCENE::MINDROOM)
	//{
	//	if (false == m_IsMindRoom)
	//	{
	//		m_IsMindRoom = true;
	//	}
	//	else 
	//	{
	//		static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Set_ChangeState(make_shared<CPlayerBrainFieldExitState>());
	//	}
	//}

	if (Clear_List())
	{
		if (m_listClearObject.size() == m_listNameNum.size())
		{
			m_listPivot.clear();
			m_listNameNum.clear();
			m_listClearObject.clear();
			m_bAllClear = false;
			m_bClearDone = true;
		}
	}
	
}

void CMap_ToolManager::Late_Tick(_float fTimeDelta)
{
}

_bool CMap_ToolManager::Clear_List(_bool bClear)
{
	if (bClear)
		m_bAllClear = true;

	return m_bAllClear;
}

void CMap_ToolManager::Clear_Obj()
{
	m_listClearObject.emplace_back();
}

HRESULT CMap_ToolManager::Emplace_Object(void* pArg)
{
	if (pArg != nullptr)
	{
		OBJECTDESC* ObjDesc = (OBJECTDESC*)pArg;

		std::get<0>(m_tuplePivot) = ObjDesc->strName;
		std::get<1>(m_tuplePivot) = ObjDesc->Scale;
		std::get<2>(m_tuplePivot) = ObjDesc->Rotation;
		std::get<3>(m_tuplePivot) = ObjDesc->Translation;
		std::get<4>(m_tuplePivot) = ObjDesc->matrix;
		m_bSave = ObjDesc->bSave;
	}

	if (!m_bSave)
	{
		wstring pivotToSearch = std::get<0>(m_tuplePivot);

		// set에 첫 번째 요소를 추가하고, 이미 존재하는지 여부를 확인
		auto [it, inserted] = m_setElement.insert(pivotToSearch);

		auto findItPivot = std::find_if(m_listPivot.begin(), m_listPivot.end(),
			[&](const auto& tuple) { return std::get<0>(tuple) == pivotToSearch; });

		if (inserted) {
			// 중복이 없으면 그냥 리스트에 추가
			m_listPivot.emplace_back(m_tuplePivot);
		}
		else {
			// 중복이 있으면 기존 요소를 덮어쓰기
			if (findItPivot != m_listPivot.end()) {
				*findItPivot = m_tuplePivot;
			}
			else {
				// 중복이 있으면 기존 요소를 삭제하고 새로운 값을 삽입
				m_setElement.erase(it);
				m_listPivot.emplace_back(m_tuplePivot);
			}
		}

		// m_listNameNum에서 중복 검사
		auto findItNameNum = std::find(m_listNameNum.begin(), m_listNameNum.end(), pivotToSearch);

		if (findItNameNum != m_listNameNum.end()) {
			// 중복이 있으면 기존 요소를 덮어쓰기
			*findItNameNum = pivotToSearch;
		}
		else {
			// 중복이 없으면 그냥 리스트에 추가
			m_listNameNum.emplace_back(pivotToSearch);
		}

		m_bEmplace = false;
	}
	else
    {
		if (m_iStageNum == 0)
			Save_File(0);
		else if (m_iStageNum == 1)
			Save_File(1);
		else if (m_iStageNum == 2)
			Save_File(2);
		else if (m_iStageNum == 3)
			Save_File(3);
		else if (m_iStageNum == 4)
			Save_File(4);
		else if (m_iStageNum == 5)
			Save_File(5);
    }

	return S_OK;
}

HRESULT CMap_ToolManager::Save_File(_int iStageNum)
{
	_ulong		dwByte = 0;
	HANDLE	hFile = nullptr;

	switch (iStageNum)
	{
	case 0:
		hFile = CreateFile(TEXT("../Resources/Map/Tutorial/dat/Test.txt"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 1:
		hFile = CreateFile(TEXT("../Resources/Map/Stage1/dat/Stage1_Test.txt"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 2:
		hFile = CreateFile(TEXT("../Resources/Map/Suoh/dat/Suoh_Test.txt"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 3:
		hFile = CreateFile(TEXT("../Resources/Map/Suoh/dat/Suoh_Battle_Test.txt"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 4:
		hFile = CreateFile(TEXT("../Resources/Map/MindRoom/dat/MindRoom_Test.txt"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 5:
		hFile = CreateFile(TEXT("../Resources/Map/Hideout/dat/Hideout_Test.txt"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	default:
		break;
	}

	if (hFile == nullptr)
	{
		MSG_RETURN(E_FAIL, "CMap_ToolManager::Save_File", "hFile is nullptr");
	}

	SetFilePointer(hFile, 0, nullptr, FILE_END);

	for (const auto& obj : m_listPivot) {
		size_t dataLength = std::get<0>(obj).size();
		WriteFile(hFile, &dataLength, sizeof(size_t), &dwByte, nullptr);
		WriteFile(hFile, std::get<0>(obj).c_str(), sizeof(wchar_t) * dataLength, &dwByte, nullptr);
		WriteFile(hFile, &std::get<1>(obj), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &std::get<2>(obj), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &std::get<3>(obj), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &std::get<4>(obj), sizeof(_float4x4), &dwByte, nullptr);
	}

	m_bSave = false;

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMap_ToolManager::Delete_Object(wstring Name)
{
	m_bDelete = true;

	auto findIt = std::find_if(m_listPivot.begin(), m_listPivot.end(),
		[&](const auto& tuple) { return std::get<0>(tuple) == Name; });

	// 찾은 경우 삭제
	if (findIt != m_listPivot.end()) {
		m_listPivot.erase(findIt);
	}

	for (auto it = m_listNameNum.begin(); it != m_listNameNum.end();)
	{
		if (*it == Name) {
			it = m_listNameNum.erase(it); // 삭제 후 다음 위치를 반환
		}
		else {
			++it;
		}
	}

	return S_OK;
}



HRESULT CMap_ToolManager::Ready_TestMap()
{
	//m_pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MAPTOOL, LAYER_MAP_ROAD);

#pragma region Road
	Load_MapFile(TEXT("Tutorial_Road0"), 0);
	Load_MapFile(TEXT("Tutorial_Road1"), 0);
	Load_MapFile(TEXT("Tutorial_Road2"), 0);
	Load_MapFile(TEXT("Tutorial_Road3"), 0);
	Load_MapFile(TEXT("Tutorial_Road4"), 0);
	Load_MapFile(TEXT("Tutorial_Road5"), 0);
	Load_MapFile(TEXT("Tutorial_Sakura"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad0"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad1"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad2"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad3"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad4"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad5"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad6"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad7"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad8"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoad9"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoadA"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoadB"), 0);
	Load_MapFile(TEXT("Tutorial_EnterRoadC"), 0);
	Load_MapFile(TEXT("Tutorial_RoadWall"), 0);
	Load_MapFile(TEXT("LogsA"), 0);
	Load_MapFile(TEXT("LogsB"), 0);
	Load_MapFile(TEXT("LogsC"), 0);
	Load_MapFile(TEXT("Sakura_BrakA"), 0);
	Load_MapFile(TEXT("Sakura_BrakB"), 0);
	Load_MapFile(TEXT("Sakura_BranchA"), 0);
	Load_MapFile(TEXT("Sakura_BranchB"), 0);
	Load_MapFile(TEXT("Sakura_BranchC"), 0);
	Load_MapFile(TEXT("StoneA"), 0);
	Load_MapFile(TEXT("Pillar"), 0);
	Load_MapFile(TEXT("HugeTree"), 0);
	Load_MapFile(TEXT("Tutorial_RootA"), 0);
	Load_MapFile(TEXT("Tutorial_RootB"), 0);
	Load_MapFile(TEXT("Tutorial_RootC"), 0);
	Load_MapFile(TEXT("Tutorial_RootD"), 0);
	Load_MapFile(TEXT("Tutorial_RootE"), 0);
	Load_MapFile(TEXT("Tutorial_RootF"), 0);
	Load_MapFile(TEXT("Tutorial_RootG"), 0);
	Load_MapFile(TEXT("HugeDarkTree"), 0);
	Load_MapFile(TEXT("Tutorial_DarkRootA"), 0);
	Load_MapFile(TEXT("Tutorial_DarkRootB"), 0);
	Load_MapFile(TEXT("Tutorial_DarkRootC"), 0);
	Load_MapFile(TEXT("Tutorial_DarkRootD"), 0);
	Load_MapFile(TEXT("Tutorial_DarkRootE"), 0);
#pragma endregion

#pragma region Dynamic
	Load_MapFile(TEXT("CarA"), 0);
	Load_MapFile(TEXT("CarB"), 0);
	Load_MapFile(TEXT("CarC"), 0);
	Load_MapFile(TEXT("CarD"), 0);
	Load_MapFile(TEXT("CarD_Brk"), 0);
	Load_MapFile(TEXT("ConcreteBarrierA"), 0);
	Load_MapFile(TEXT("ConcreteBarrierB"), 0);
	Load_MapFile(TEXT("SeiranCar"), 0);
	Load_MapFile(TEXT("SeiranCar_Brk"), 0);
	Load_MapFile(TEXT("Tractor"), 0);
#pragma endregion

#pragma region Static
	Load_MapFile(TEXT("TruckA"), 0);
	Load_MapFile(TEXT("Truck_CampingCar"), 0);
	Load_MapFile(TEXT("Truck_Container"), 0);
	Load_MapFile(TEXT("Truck_Gas"), 0);
	Load_MapFile(TEXT("Truck_GasBrk"), 0);
	Load_MapFile(TEXT("BusA"), 0);
#pragma endregion

	Load_MapFile(TEXT("Grass"), 0);
	Load_MapFile(TEXT("BeechA"), 0);
	Load_MapFile(TEXT("BeechB"), 0);
	Load_MapFile(TEXT("BeechC"), 0);
	Load_MapFile(TEXT("BeechD"), 0);
	Load_MapFile(TEXT("BeechE"), 0);

	m_bLoad = false;

	return S_OK;
}

HRESULT CMap_ToolManager::Ready_MizuhaMap()
{
	Load_MapFile(TEXT("Common_Item"), 1);
	Load_MapFile(TEXT("Common_BaseWall"), 1);

#pragma region Road
	Load_MapFile(TEXT("Area_MiniMapA"), 1);
	//Load_MapFile(TEXT("Area_MiniMap"));
	Load_MapFile(TEXT("Dirt_Tile"), 1);
	Load_MapFile(TEXT("SandyOil_Tile"), 1);
	Load_MapFile(TEXT("Circle_Tile"), 1);
	Load_MapFile(TEXT("ConcretePiraA"), 1);
	Load_MapFile(TEXT("ConcretePiraB"), 1);
	Load_MapFile(TEXT("ConcretePiraC"), 1);
	Load_MapFile(TEXT("ConcretePiraD"), 1);
	Load_MapFile(TEXT("ConcretePiraE"), 1);
	Load_MapFile(TEXT("ConcretePiraF"), 1);
	Load_MapFile(TEXT("ConcretePiraG"), 1);
	Load_MapFile(TEXT("ConcretePiraH"), 1);
	Load_MapFile(TEXT("ConcretePiraI"), 1);
	Load_MapFile(TEXT("ConcretePiraJ"), 1);
	Load_MapFile(TEXT("ConcretePiraK"), 1);
	Load_MapFile(TEXT("ConcretePiraL"), 1);
	Load_MapFile(TEXT("FootholdD"), 1);
	Load_MapFile(TEXT("FootholdE"), 1);
	Load_MapFile(TEXT("FootholdF"), 1);
	Load_MapFile(TEXT("FootholdShapeA"), 1);
	Load_MapFile(TEXT("FootholdShapeB"), 1);
	Load_MapFile(TEXT("FootholdG"), 1);
	Load_MapFile(TEXT("Dirt_Ground"), 1);
	Load_MapFile(TEXT("BasementA"), 1);
	Load_MapFile(TEXT("BasementB"), 1);
	Load_MapFile(TEXT("BasementC"), 1);
	Load_MapFile(TEXT("BasementD"), 1);
	Load_MapFile(TEXT("BasementE"), 1);
	Load_MapFile(TEXT("ConstructionBuildingC"), 1);
	Load_MapFile(TEXT("Bridge_EnkeiB"), 1);
	Load_MapFile(TEXT("Basement_BendA"), 1);
	Load_MapFile(TEXT("BossWallA"), 1);
	Load_MapFile(TEXT("BossWallB"), 1);
	Load_MapFile(TEXT("BossWallC"), 1);
	Load_MapFile(TEXT("BossWallD"), 1);
	Load_MapFile(TEXT("BossWallE"), 1);
	Load_MapFile(TEXT("BossWallF"), 1);
	Load_MapFile(TEXT("BossWallG"), 1);
	Load_MapFile(TEXT("BossWallH"), 1);
	Load_MapFile(TEXT("BossWallI"), 1);
	Load_MapFile(TEXT("BossBanisterA"), 1);
	Load_MapFile(TEXT("BossBanisterB"), 1);
	Load_MapFile(TEXT("BossBanisterC"), 1);
	Load_MapFile(TEXT("BossBanisterD"), 1);
	Load_MapFile(TEXT("BossBanisterF"), 1);
	Load_MapFile(TEXT("BossBanisterE"), 1);
	Load_MapFile(TEXT("Bridge_EnkeiC"), 1);
#pragma endregion

#pragma region Static
	Load_MapFile(TEXT("FenceA"), 1);
	Load_MapFile(TEXT("FenceB"), 1);
	Load_MapFile(TEXT("FenceC"), 1);
	Load_MapFile(TEXT("RealSteelWallF"), 1);
	Load_MapFile(TEXT("RealSteelWallA"), 1);
	Load_MapFile(TEXT("RealSteelWallB"), 1);
	Load_MapFile(TEXT("RealSteelWallC"), 1);
	Load_MapFile(TEXT("SteelWall5A"), 1);
	Load_MapFile(TEXT("SteelWall5B"), 1);
	Load_MapFile(TEXT("SteelWall5C"), 1);
	Load_MapFile(TEXT("HeavyEquipmentA"), 1);
	Load_MapFile(TEXT("HeavyEquipmentC"), 1);
	Load_MapFile(TEXT("HeavyEquipmentE"), 1);
	Load_MapFile(TEXT("SteelWall4A"), 1);
	Load_MapFile(TEXT("ConstructionSlopeA"), 1);
	Load_MapFile(TEXT("PrefabA"), 1);
	Load_MapFile(TEXT("PrefabB"), 1);
	Load_MapFile(TEXT("RockA"), 1);
	Load_MapFile(TEXT("RockB"), 1);
	Load_MapFile(TEXT("RockC"), 1);
	Load_MapFile(TEXT("RubbleA"), 1);
	Load_MapFile(TEXT("RubbleB"), 1);
	Load_MapFile(TEXT("FencingA"), 1);
	Load_MapFile(TEXT("FencingB"), 1);
	Load_MapFile(TEXT("FencingC"), 1);
	Load_MapFile(TEXT("OutsideFloorA"), 1);
	Load_MapFile(TEXT("OutsideFloorB"), 1);
	Load_MapFile(TEXT("OutsideFloorC"), 1);
	Load_MapFile(TEXT("OutsideFloorD"), 1);
	Load_MapFile(TEXT("OutsideFloorE"), 1);
	Load_MapFile(TEXT("OutsideFloorF"), 1);
	Load_MapFile(TEXT("NDF0"), 1);
	Load_MapFile(TEXT("PlyWoodA"), 1);
	Load_MapFile(TEXT("PlyWoodB"), 1);
	Load_MapFile(TEXT("BioToilet"), 1);
	Load_MapFile(TEXT("RubbleC"), 1);
	Load_MapFile(TEXT("RubbleD"), 1);
	Load_MapFile(TEXT("ConcreteWallA"), 1);
	Load_MapFile(TEXT("SteelPoleA"), 1);
	Load_MapFile(TEXT("TowerCrane_Base"), 1);
	Load_MapFile(TEXT("FencingD"), 1);
	Load_MapFile(TEXT("FencingE"), 1);
	Load_MapFile(TEXT("RoadBlockB"), 1);
#pragma endregion

#pragma region Dynamic
	Load_MapFile(TEXT("BarrelA"), 1);
	Load_MapFile(TEXT("BarrelB"), 1);
	Load_MapFile(TEXT("Dumpster"), 1);
	Load_MapFile(TEXT("BoxA"), 1);
	Load_MapFile(TEXT("BoxB"), 1);
	Load_MapFile(TEXT("ConcreteBagA"), 1);
	Load_MapFile(TEXT("ConcreteBagB"), 1);
	Load_MapFile(TEXT("ConcretePipeA"), 1);
	Load_MapFile(TEXT("ConcretePipeB"), 1);
	Load_MapFile(TEXT("ConcretePipeC"), 1);
	Load_MapFile(TEXT("ConcretePipeD"), 1);
	Load_MapFile(TEXT("SpoolA"), 1);
	Load_MapFile(TEXT("HeavyEquipmentH"), 1);
	Load_MapFile(TEXT("Brick_Palette"), 1);
	Load_MapFile(TEXT("CableReel"), 1);
	Load_MapFile(TEXT("IceBox"), 1);

#pragma endregion

#pragma region Special_Dynamic
	Load_MapFile(TEXT("Special_BeamA"), 1);

	Load_MapFile(TEXT("HeavyEquipmentG"), 1);
	Load_MapFile(TEXT("Special_Rock0"), 1);
#pragma endregion

#pragma region Building
	Load_MapFile(TEXT("RealSteelWallD"), 1);
	Load_MapFile(TEXT("RealSteelWallE"), 1);
	Load_MapFile(TEXT("SteelWall4ANon"), 1);
	Load_MapFile(TEXT("SteelWall3A"), 1);
	Load_MapFile(TEXT("SteelWall3B"), 1);
	Load_MapFile(TEXT("CityBuilding_PartsA"), 1);
	Load_MapFile(TEXT("CityBuildingA"), 1);
	Load_MapFile(TEXT("ConstructionBuildingA"), 1);
	Load_MapFile(TEXT("ConstructionBuildingB"), 1);
	Load_MapFile(TEXT("Crane_EnkeiA"), 1);
	Load_MapFile(TEXT("DistantBuildingA"), 1);
	Load_MapFile(TEXT("Ironpillar"), 1);
	Load_MapFile(TEXT("TarpA"), 1);
	Load_MapFile(TEXT("TarpB"), 1);
	Load_MapFile(TEXT("TarpC"), 1);
	Load_MapFile(TEXT("PlyWoodC"), 1);
	Load_MapFile(TEXT("PlyWoodD"), 1);
	Load_MapFile(TEXT("Ground_CoalA"), 1);
	Load_MapFile(TEXT("CityBuilding_Tower"), 1);
	Load_MapFile(TEXT("MizuhaBuildingA"), 1);
	Load_MapFile(TEXT("MizuhaBuildingB"), 1);
	Load_MapFile(TEXT("MizuhaBuildingC"), 1);
	Load_MapFile(TEXT("MizuhaBuildingD"), 1);
	Load_MapFile(TEXT("MizuhaBuildingE"), 1);
	Load_MapFile(TEXT("MizuhaBuildingF"), 1);
#pragma endregion

#pragma region ETC
	Load_MapFile(TEXT("Ground_CarC"), 1);
	Load_MapFile(TEXT("Ground_Grass"), 1);
	Load_MapFile(TEXT("RoadBlockA"), 1);
	Load_MapFile(TEXT("Ground_ConeA"), 1);
	Load_MapFile(TEXT("Ground_ConeB"), 1);
	Load_MapFile(TEXT("TanA"), 1);
	Load_MapFile(TEXT("TanB"), 1);
	Load_MapFile(TEXT("TanC"), 1);
	Load_MapFile(TEXT("TanD"), 1);
	Load_MapFile(TEXT("TanE"), 1);
	Load_MapFile(TEXT("ConstructionGateA"), 1);
#pragma endregion

#pragma region Elevator
	Load_MapFile(TEXT("ConstructionElevator"), 1);
#pragma endregion

#pragma region LightObject

	Load_MapFile(TEXT("SpotlightA"), 1);
	Load_MapFile(TEXT("FloodlightB"), 1);
#pragma endregion


#pragma region Neon
	Load_MapFile(TEXT("NeonCornerA"), 1);
	Load_MapFile(TEXT("NeonCornerB"), 1);
	Load_MapFile(TEXT("NeonCornerC"), 1);
	Load_MapFile(TEXT("NeonCornerD"), 1);
	Load_MapFile(TEXT("NeonCrashA"), 1);
	Load_MapFile(TEXT("NeonCrashB"), 1);
	Load_MapFile(TEXT("NeonLineA"), 1);
	Load_MapFile(TEXT("NeonLineB"), 1);
	Load_MapFile(TEXT("NeonRollA"), 1);
	Load_MapFile(TEXT("NeonSwayA"), 1);
	Load_MapFile(TEXT("NeonSwayB"), 1);
	Load_MapFile(TEXT("NeonSwayC"), 1);
#pragma endregion
	m_bLoad = false;

	return S_OK;
}

HRESULT CMap_ToolManager::Ready_SuohMap(_int iIndex)
{
	Load_MapFile(TEXT("Common_BaseWall"), iIndex);
	Load_MapFile(TEXT("Common_Item"), iIndex);
	Load_MapFile(TEXT("Shopkeeper"), iIndex);

#pragma region Road
	Load_MapFile(TEXT("Suoh_Minimap"), iIndex);
	Load_MapFile(TEXT("SG_Ground_A"), iIndex);
	Load_MapFile(TEXT("PedestrianBridgeA"), iIndex);
	Load_MapFile(TEXT("Suoh_Dodai"), iIndex);
	Load_MapFile(TEXT("Suoh_Tile"), iIndex);
	Load_MapFile(TEXT("Town_RoadB"), iIndex);
	Load_MapFile(TEXT("Town_RoadC"), iIndex);

#pragma endregion

#pragma region Static
	Load_MapFile(TEXT("Suoh_WallB"), iIndex);
	Load_MapFile(TEXT("Suoh_WallC"), iIndex);
	Load_MapFile(TEXT("Suoh_WallD"), iIndex);
	Load_MapFile(TEXT("SG_ShutterBase"), iIndex);
	Load_MapFile(TEXT("Town_RoadA"), iIndex);
	Load_MapFile(TEXT("SG_ShutterA"), iIndex);
	Load_MapFile(TEXT("SG_FlowerPot"), iIndex);
	Load_MapFile(TEXT("Suoh_Keijiban"), iIndex);
	Load_MapFile(TEXT("Suoh_Sekihi"), iIndex);
	Load_MapFile(TEXT("Suoh_OmikujiB"), iIndex);
	Load_MapFile(TEXT("SG_ArchA"), iIndex);
	Load_MapFile(TEXT("Sumeragi_KaidanA"), iIndex);
	Load_MapFile(TEXT("Suoh_RubbleA"), iIndex);
	Load_MapFile(TEXT("SG_PlantsA"), iIndex);
	Load_MapFile(TEXT("SG_TreeA"), iIndex);
	Load_MapFile(TEXT("SG_Sumeragi_Tourou"), iIndex);
	Load_MapFile(TEXT("MaitreyaTrii"), iIndex);
	Load_MapFile(TEXT("Suoh_CameraA"), iIndex);
	Load_MapFile(TEXT("Suoh_CameraD"), iIndex);
	Load_MapFile(TEXT("HydrantA"), iIndex);
	Load_MapFile(TEXT("TelephoneBoxA"), iIndex);

#pragma endregion

#pragma region Building
	Load_MapFile(TEXT("SG_AcademyA"), iIndex);
	Load_MapFile(TEXT("SG_AcademyB"), iIndex);
	Load_MapFile(TEXT("SG_AcademyC"), iIndex);
	Load_MapFile(TEXT("Suoh_Tower"), iIndex);
	Load_MapFile(TEXT("Suoh_Bridge"), iIndex);
	Load_MapFile(TEXT("SG_BuildingA"), iIndex);
	Load_MapFile(TEXT("SG_BuildingB"), iIndex);
	Load_MapFile(TEXT("SG_BuildingC"), iIndex);
	Load_MapFile(TEXT("Suoh_A_BuildingA"), iIndex);
	Load_MapFile(TEXT("Suoh_A_BuildingB"), iIndex);
	Load_MapFile(TEXT("Suoh_B_BuildingA"), iIndex);
	Load_MapFile(TEXT("Suoh_C_BuildingA"), iIndex);
	Load_MapFile(TEXT("Suoh_C_BuildingB"), iIndex);
	Load_MapFile(TEXT("Suoh_C_BuildingC"), iIndex);
	Load_MapFile(TEXT("Suoh_C_BuildingD"), iIndex);
	Load_MapFile(TEXT("Suoh_D_BuildingA"), iIndex);
	Load_MapFile(TEXT("Suoh_D_BuildingB"), iIndex);
	Load_MapFile(TEXT("Suoh_D_BuildingC"), iIndex);
	Load_MapFile(TEXT("Suoh_D_BuildingD"), iIndex);
	Load_MapFile(TEXT("Suoh_E_BuildingA"), iIndex);
	Load_MapFile(TEXT("Suoh_E_BuildingB"), iIndex);
	Load_MapFile(TEXT("Suoh_E_BuildingC"), iIndex);
	Load_MapFile(TEXT("Suoh_E_BuildingD"), iIndex);
	Load_MapFile(TEXT("Suoh_E_BuildingE"), iIndex);
	Load_MapFile(TEXT("Suoh_E_BuildingF"), iIndex);
	Load_MapFile(TEXT("Suoh_F_BuildingA"), iIndex);
	Load_MapFile(TEXT("Suoh_F_BuildingB"), iIndex);
	Load_MapFile(TEXT("Suoh_G_BuildingA"), iIndex);
	Load_MapFile(TEXT("Suoh_G_BuildingB"), iIndex);
	Load_MapFile(TEXT("Suoh_CafeBuilding"), iIndex);
#pragma endregion

#pragma region Signboard
	Load_MapFile(TEXT("Suoh_Billboard_A0"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_A1"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_AiIndex"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_A3"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_C0"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_D0"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_G0"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_I0"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_I1"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_IiIndex"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_I3"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_J0"), iIndex);
	Load_MapFile(TEXT("Suoh_GuideA_A"), iIndex);
	Load_MapFile(TEXT("Suoh_GuideA_B"), iIndex);
	Load_MapFile(TEXT("Suoh_GuideA_C"), iIndex);
	Load_MapFile(TEXT("Suoh_GuideB_A"), iIndex);
	Load_MapFile(TEXT("Suoh_GuideC_A"), iIndex);
	Load_MapFile(TEXT("Suoh_GuideC_B"), iIndex);
	Load_MapFile(TEXT("Suoh_GuideC_C"), iIndex);
	Load_MapFile(TEXT("Suoh_GuideC_D"), iIndex);
	Load_MapFile(TEXT("Suoh_SignA_A"), iIndex);
	Load_MapFile(TEXT("Suoh_SignA_B"), iIndex);
	Load_MapFile(TEXT("Suoh_SignA_C"), iIndex);
	Load_MapFile(TEXT("Suoh_SignB_A"), iIndex);
	Load_MapFile(TEXT("Suoh_SignD_D"), iIndex);
	Load_MapFile(TEXT("Suoh_SignE"), iIndex);
	Load_MapFile(TEXT("Suoh_BillboardX_A"), iIndex);
	Load_MapFile(TEXT("Suoh_BillboardX_B"), iIndex);
	Load_MapFile(TEXT("Suoh_BillboardX_C"), iIndex);
	Load_MapFile(TEXT("Suoh_BillboardX_D"), iIndex);
	Load_MapFile(TEXT("Suoh_BillboardX_E"), iIndex);
	Load_MapFile(TEXT("Suoh_BillboardX_F"), iIndex);
	Load_MapFile(TEXT("Suoh_BillboardX_G"), iIndex);
#pragma endregion

#pragma region ETC
	Load_MapFile(TEXT("Suoh_BicycleA"), iIndex);
	Load_MapFile(TEXT("Suoh_BicycleB"), iIndex);
	Load_MapFile(TEXT("Suoh_BicycleC"), iIndex);
	Load_MapFile(TEXT("Suoh_ScooterA"), iIndex);
	Load_MapFile(TEXT("Suoh_ScooterB"), iIndex);
	Load_MapFile(TEXT("Suoh_ShopBox"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_B0"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_B1"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_BiIndex"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_B3"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_F0"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_G1"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_GiIndex"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_H0"), iIndex);
	Load_MapFile(TEXT("Suoh_Billboard_H1"), iIndex);
	Load_MapFile(TEXT("Town_BoxA"), iIndex);
	Load_MapFile(TEXT("Town_BoxB"), iIndex);
	Load_MapFile(TEXT("Town_BoxC"), iIndex);
	Load_MapFile(TEXT("Town_BoxD"), iIndex);
	Load_MapFile(TEXT("Town_CarrierA"), iIndex);
	Load_MapFile(TEXT("Town_CarrierB"), iIndex);
	Load_MapFile(TEXT("Town_CarrierC"), iIndex);
#pragma endregion

#pragma region Dynamic
	Load_MapFile(TEXT("Suoh_CarA"), iIndex);
	Load_MapFile(TEXT("Suoh_CarB"), iIndex);
	Load_MapFile(TEXT("Suoh_CarC"), iIndex);
	Load_MapFile(TEXT("Suoh_CarD"), iIndex);
#pragma endregion


#pragma region NPC
	Load_MapFile(TEXT("Suoh_NPC_Child"), iIndex);
	Load_MapFile(TEXT("Suoh_NPC_Man"), iIndex);
	Load_MapFile(TEXT("Suoh_NPC_Woman"), iIndex);
#pragma endregion

#pragma region Neon
	Load_MapFile(TEXT("Suoh_Battle_WarningWallA"), iIndex);
	Load_MapFile(TEXT("Suoh_Battle_WarningWallB"), iIndex);
#pragma endregion
	Load_MapFile(TEXT("BusA"), iIndex);

	m_bLoad = false;

	return S_OK;
}

HRESULT CMap_ToolManager::Ready_MindRoom()
{
	Load_MapFile(TEXT("Common_BaseWall"), 4);

#pragma region Road
	Load_MapFile(TEXT("Sankaku_futa"), 4);
	Load_MapFile(TEXT("MindRoom_Water"), 4);
#pragma endregion

#pragma region Static
	Load_MapFile(TEXT("Sankaku_A"), 4);
	Load_MapFile(TEXT("Mindroom_StoneA"), 4);
	Load_MapFile(TEXT("Mindroom_StoneB"), 4);
	Load_MapFile(TEXT("Mindroom_StoneC"), 4);
	Load_MapFile(TEXT("Mindroom_Spaceship"), 4);
#pragma endregion

#pragma region Neon
	Load_MapFile(TEXT("MindRoom_Tenkyu"), 4);
	Load_MapFile(TEXT("MindRoom_Floor"), 4);
	Load_MapFile(TEXT("MindRoom_Twist"), 4);
	Load_MapFile(TEXT("MindRoom_Redline"), 4);
#pragma endregion

#pragma region Dynamic
	Load_MapFile(TEXT("Mindroom_Box"), 4);
	Load_MapFile(TEXT("Mindroom_CrateA"), 4);
	Load_MapFile(TEXT("Mindroom_CrateB"), 4);
#pragma endregion

	return S_OK;
}

HRESULT CMap_ToolManager::Ready_Hideout()
{
	Load_MapFile(TEXT("Common_Item"), 5);
	Load_MapFile(TEXT("Arashi_Teammate"), 5);

#pragma region Static
	Load_MapFile(TEXT("Hideout_Floor"), 5);
	Load_MapFile(TEXT("Hideout_Carpet"), 5);
	Load_MapFile(TEXT("Hideout_Cole"), 5);
	Load_MapFile(TEXT("Hideout_Fireplace"), 5);
	Load_MapFile(TEXT("Hideout_Kitchen"), 5);
	Load_MapFile(TEXT("Hideout_LowTable"), 5);
	Load_MapFile(TEXT("Hideout_Pouf"), 5);
	Load_MapFile(TEXT("Hideout_SofaA"), 5);
	Load_MapFile(TEXT("Hideout_SofaB"), 5);
	Load_MapFile(TEXT("Hideout_SofaC"), 5);
	Load_MapFile(TEXT("Hideout_FabricChair"), 5);
	Load_MapFile(TEXT("Hideout_MounatinScreen"), 5);
	Load_MapFile(TEXT("Hideout_PerfectBodyPhoto"), 5);
	Load_MapFile(TEXT("Hideout_Table"), 5);
	Load_MapFile(TEXT("Hideout_TeamPhoto"), 5);
	Load_MapFile(TEXT("Hideout_BenchA"), 5);
	Load_MapFile(TEXT("Hideout_BenchPress"), 5);
	Load_MapFile(TEXT("Hideout_DumbbellA"), 5);
	Load_MapFile(TEXT("Hideout_DumbbellB"), 5);
	Load_MapFile(TEXT("Hideout_DumbbellC"), 5);
	Load_MapFile(TEXT("Hideout_Kettlebell"), 5);
	Load_MapFile(TEXT("Hideout_DumbbellRack"), 5);
#pragma endregion
	return S_OK;
}

HRESULT CMap_ToolManager::Load_MapFile(wstring strObjName, _int iIndex)
{
	_ulong		dwByte = 0;
	HANDLE		hFile = nullptr;

	switch (iIndex)
	{
	case 0:
		hFile = CreateFile(TEXT("../Resources/Map/Tutorial/dat/Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 1:
		hFile = CreateFile(TEXT("../Resources/Map/Stage1/dat/Stage1_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 2:
		hFile = CreateFile(TEXT("../Resources/Map/Suoh/dat/Suoh_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 3:
		hFile = CreateFile(TEXT("../Resources/Map/Suoh/dat/Suoh_Battle_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 4:
		hFile = CreateFile(TEXT("../Resources/Map/MindRoom/dat/MindRoom_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	case 5:
		hFile = CreateFile(TEXT("../Resources/Map/Hideout/dat/Hideout_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		break;
	default:
		break;
	}

	if (hFile == nullptr)
	{
		MSG_RETURN(E_FAIL, "CMap_ToolManager::Load_MapFile", "hFile is nullptr");
	}

	size_t      dataLength;
	wstring     Name;
	_float3		Scale = { 0.f,0.f,0.f };
	_float3     Rotation = { 0.f,0.f,0.f };
	_float3		Tanslation = { 0.f,0.f,0.f };
	_float4x4	Mat;

	while (true)
	{
		if (!ReadFile(hFile, &dataLength, sizeof(size_t), &dwByte, nullptr))
			break;
		if (dataLength == 0)
			break;
		if (0 == dwByte)
			break;
		Name.resize(dataLength);
		if (!ReadFile(hFile, &Name[0], sizeof(wchar_t) * dataLength, &dwByte, nullptr))
			break;
		if (!ReadFile(hFile, &Scale, sizeof(_float3), &dwByte, nullptr))
			break;
		if (!ReadFile(hFile, &Rotation, sizeof(_float3), &dwByte, nullptr))
			break;
		if (!ReadFile(hFile, &Tanslation, sizeof(_float3), &dwByte, nullptr))
			break;
		if (!ReadFile(hFile, &Mat, sizeof(_float4x4), &dwByte, nullptr))
			break;

		/*wstring targetString = strObjName;
		if (Name.find(targetString) != wstring::npos)
		{
			wstring extractedString = Name.substr(0, targetString.length());
			m_strObjName = extractedString;
		}
		else
			m_strObjName = TEXT("");*/

		wstring targetString = strObjName;

		// 정규 표현식을 사용하여 숫자를 무시하고 부분 문자열을 찾음
		std::wregex regexPattern(targetString + L"(_\\d+)?");

		std::wsmatch match;
		if (std::regex_search(Name, match, regexPattern)) {
			m_strObjName = match.str();
		}
		else {
			m_strObjName = TEXT("");
		}

		if (m_strObjName == strObjName)
		{
			switch (iIndex)
			{
			case 0:
				m_strPrototypeName = TutorialMap_Name(m_strObjName);
				break;
			case 1:
				m_strPrototypeName = MizuhaMap_Name(m_strObjName);
				break;
			case 2:
				m_strPrototypeName = SuohMap_Name(m_strObjName);
				break;
			case 3:
				m_strPrototypeName = SuohMap_Name(m_strObjName);
				break;
			case 4:
				m_strPrototypeName = MindRoom_Name(m_strObjName);
				break;
			case 5:
				m_strPrototypeName = Hideout_Name(m_strObjName);
				break;
			default:
				break;
			}

			m_strNameNum = Name;
			std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple_Road0 = 
			{ SCENE::MAPTOOL, m_strNameNum, Scale,Rotation,Tanslation, Mat, m_strNameNum };

			if (FAILED(m_pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
				m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
			{
				MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
			}
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

wstring CMap_ToolManager::TutorialMap_Name(wstring strObjectName)
{
	wstring strPrototypeName = TEXT("");

#pragma region Road
	if (strObjectName == TEXT("Tutorial_Road4"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD4;
	if (strObjectName == TEXT("Tutorial_Road5"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD5;
	if (strObjectName == TEXT("Tutorial_EnterRoad0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD0;
	if (strObjectName == TEXT("Tutorial_EnterRoad4"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD4;
	if (strObjectName == TEXT("Tutorial_EnterRoad5"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD5;
	if (strObjectName == TEXT("Tutorial_EnterRoad6"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD6;
	if (strObjectName == TEXT("Tutorial_EnterRoad7"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD7;
	if (strObjectName == TEXT("Tutorial_EnterRoad8"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD8;
	if (strObjectName == TEXT("Tutorial_EnterRoad9"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD9;
	if (strObjectName == TEXT("Tutorial_EnterRoadA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADA;
	if (strObjectName == TEXT("Tutorial_EnterRoadB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADB;
	if (strObjectName == TEXT("Tutorial_EnterRoadC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADC;
	if (strObjectName == TEXT("Tutorial_RoadWall"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROADWALL;
	if (strObjectName == TEXT("BeechA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BEECHA;
	if (strObjectName == TEXT("BeechB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BEECHB;
	if (strObjectName == TEXT("BeechC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BEECHC;
	if (strObjectName == TEXT("BeechD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BEECHD;
	if (strObjectName == TEXT("BeechE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BEECHE;
	if (strObjectName == TEXT("StoneA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STONEA;
	if (strObjectName == TEXT("HugeTree"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HUGETREE;
	if (strObjectName == TEXT("Tutorial_RootA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTA;
	if (strObjectName == TEXT("Tutorial_RootB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTB;
	if (strObjectName == TEXT("Tutorial_RootC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTC;
	if (strObjectName == TEXT("Tutorial_RootD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTD;
	if (strObjectName == TEXT("Tutorial_RootE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTE;
	if (strObjectName == TEXT("Tutorial_RootF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTF;
	if (strObjectName == TEXT("Tutorial_RootG"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTG;
	if (strObjectName == TEXT("HugeDarkTree"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HUGEDARKTREE;
	if (strObjectName == TEXT("Tutorial_DarkRootA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTA;
	if (strObjectName == TEXT("Tutorial_DarkRootB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTB;
	if (strObjectName == TEXT("Tutorial_DarkRootC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTC;
	if (strObjectName == TEXT("Tutorial_DarkRootD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTD;
	if (strObjectName == TEXT("Tutorial_DarkRootE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTE;
#pragma endregion

#pragma region Dynamic
	if (strObjectName == TEXT("CarA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CARA;
	if (strObjectName == TEXT("CarB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CARB;
	if (strObjectName == TEXT("CarC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CARC;
	if (strObjectName == TEXT("CarD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CARD;
	if (strObjectName == TEXT("ConcreteBarrierA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA;
	if (strObjectName == TEXT("ConcreteBarrierB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERB;
#pragma endregion

#pragma region Static
	if (strObjectName == TEXT("TruckA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TRUCKA;
	if (strObjectName == TEXT("Truck_CampingCar"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CAMPINGCAR;
	if (strObjectName == TEXT("Truck_Container"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CONTAINER;
	if (strObjectName == TEXT("Truck_Gas"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GAS;
	if (strObjectName == TEXT("Truck_GasBrk"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GASBRK;
	if (strObjectName == TEXT("BusA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BUSA;
#pragma endregion

	if (strObjectName == TEXT("Grass"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GRASS;

	return strPrototypeName;

}

wstring CMap_ToolManager::MizuhaMap_Name(wstring strObjectName)
{
	wstring strPrototypeName = TEXT("");

	if (strObjectName == TEXT("Common_BaseWall"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL;
	if (strObjectName == TEXT("Common_Item"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM;
#pragma region Road

	if (strObjectName == TEXT("Area_MiniMap"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP;
	if (strObjectName == TEXT("Dirt_Tile"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_Dirt_Tile;
	if (strObjectName == TEXT("Area_MiniMapA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAPA;
	if (strObjectName == TEXT("ConcretePiraA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAA;
	if (strObjectName == TEXT("ConcretePiraB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAB;
	if (strObjectName == TEXT("ConcretePiraC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAC;
	if (strObjectName == TEXT("ConcretePiraD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAD;
	if (strObjectName == TEXT("ConcretePiraE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAE;
	if (strObjectName == TEXT("ConcretePiraF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAF;
	if (strObjectName == TEXT("ConcretePiraG"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAG;
	if (strObjectName == TEXT("ConcretePiraH"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAH;
	if (strObjectName == TEXT("ConcretePiraI"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAI;
	if (strObjectName == TEXT("ConcretePiraJ"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAJ;
	if (strObjectName == TEXT("ConcretePiraK"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAK;
	if (strObjectName == TEXT("ConcretePiraL"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL;
	if (strObjectName == TEXT("FootholdD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDD;
	if (strObjectName == TEXT("FootholdE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDE;
	if (strObjectName == TEXT("FootholdF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDF;
	if (strObjectName == TEXT("FootholdShapeA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEA;
	if (strObjectName == TEXT("FootholdShapeB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEB;
	if (strObjectName == TEXT("FootholdG"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDG;
	if (strObjectName == TEXT("Dirt_Ground"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_DIRT_GROUND;
	if (strObjectName == TEXT("BasementA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTA;
	if (strObjectName == TEXT("BasementB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTB;
	if (strObjectName == TEXT("BasementC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTC;
	if (strObjectName == TEXT("BasementD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTD;
	if (strObjectName == TEXT("BasementE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTE;
	if (strObjectName == TEXT("ConstructionBuildingC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGC;
	if (strObjectName == TEXT("Bridge_EnkeiB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIB;
	if (strObjectName == TEXT("Basement_BendA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTBENDA;
	if (strObjectName == TEXT("BossWallA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLA;
	if (strObjectName == TEXT("BossWallB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLB;
	if (strObjectName == TEXT("BossWallC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLC;
	if (strObjectName == TEXT("BossWallD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLD;	
	if (strObjectName == TEXT("BossWallE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE;
	if (strObjectName == TEXT("BossWallF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLF;
	if (strObjectName == TEXT("BossWallG"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLG;
	if (strObjectName == TEXT("BossWallH"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLH;
	if (strObjectName == TEXT("BossWallI"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLI;
	if (strObjectName == TEXT("BossBanisterA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERA;
	if (strObjectName == TEXT("BossBanisterB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERB;
	if (strObjectName == TEXT("BossBanisterC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERC;
	if (strObjectName == TEXT("BossBanisterD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERD;
	if (strObjectName == TEXT("BossBanisterE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERE;
	if (strObjectName == TEXT("BossBanisterF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERF;
	if (strObjectName == TEXT("Bridge_EnkeiC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIC;
#pragma endregion

#pragma region Static
	if (strObjectName == TEXT("FenceA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCEA;
	if (strObjectName == TEXT("FenceB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCEB;
	if (strObjectName == TEXT("FenceC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCEC;
	if (strObjectName == TEXT("RealSteelWallF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLF;
	if (strObjectName == TEXT("RealSteelWallA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLA;
	if (strObjectName == TEXT("RealSteelWallB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLB;
	if (strObjectName == TEXT("RealSteelWallC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLC;
	if (strObjectName == TEXT("SteelWall5B"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5B;
	if (strObjectName == TEXT("SteelWall5A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5A;
	if (strObjectName == TEXT("SteelWall5C"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5C;
	if (strObjectName == TEXT("HeavyEquipmentA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTA;
	if (strObjectName == TEXT("HeavyEquipmentC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTC;

	if (strObjectName == TEXT("HeavyEquipmentE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTE;
	if (strObjectName == TEXT("SteelWall4A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4A;
	if (strObjectName == TEXT("ConstructionSlopeA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONSLOPEA;
	if (strObjectName == TEXT("PrefabA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PREFABA;
	if (strObjectName == TEXT("PrefabB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PREFABB;
	if (strObjectName == TEXT("RockA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROCKA;
	if (strObjectName == TEXT("RockB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROCKB;
	if (strObjectName == TEXT("RockC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROCKC;
	if (strObjectName == TEXT("RubbleA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_RUBBLEA;
	if (strObjectName == TEXT("RubbleB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_RUBBLEB;
	if (strObjectName == TEXT("FencingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGA;
	if (strObjectName == TEXT("FencingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGB;
	if (strObjectName == TEXT("FencingC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGC;
	if (strObjectName == TEXT("OutsideFloorA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA;
	if (strObjectName == TEXT("OutsideFloorB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORB;
	if (strObjectName == TEXT("OutsideFloorC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORC;
	if (strObjectName == TEXT("OutsideFloorD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORD;
	if (strObjectName == TEXT("OutsideFloorE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORE;
	if (strObjectName == TEXT("OutsideFloorF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORF;
	if (strObjectName == TEXT("NDF0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NDF0;
	if (strObjectName == TEXT("PlyWoodA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PLYWOODA;
	if (strObjectName == TEXT("PlyWoodB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PLYWOODB;
	if (strObjectName == TEXT("BioToilet"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BIOTOILET;
	if (strObjectName == TEXT("RubbleC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_RUBBLEC;
	if (strObjectName == TEXT("RubbleD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_RUBBLED;
	if (strObjectName == TEXT("ConcreteWallA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEWALLA;
	if (strObjectName == TEXT("SteelPoleA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELPOLEA;
	if (strObjectName == TEXT("TowerCrane_Base"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWERCRANE_BASE;
	if (strObjectName == TEXT("FencingD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGD;
	if (strObjectName == TEXT("FencingE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGE;	
	if (strObjectName == TEXT("RoadBlockB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKB;
#pragma endregion

#pragma region Dynamic
	if (strObjectName == TEXT("BarrelA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BARRELA;
	if (strObjectName == TEXT("BarrelB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BARRELB;
	if (strObjectName == TEXT("Dumpster"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_DUMPSTER;
	if (strObjectName == TEXT("BoxA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOXA;
	if (strObjectName == TEXT("BoxB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOXB;
	if (strObjectName == TEXT("ConcreteBagA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGA;
	if (strObjectName == TEXT("ConcreteBagB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGB;
	if (strObjectName == TEXT("ConcretePipeA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEA;
	if (strObjectName == TEXT("ConcretePipeB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEB;
	if (strObjectName == TEXT("ConcretePipeC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEC;
	if (strObjectName == TEXT("ConcretePipeD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPED;
	if (strObjectName == TEXT("SpoolA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SPOOLA;
	if (strObjectName == TEXT("HeavyEquipmentH"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTH;
	if (strObjectName == TEXT("Brick_Palette"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BRICK_PALETTE;
	if (strObjectName == TEXT("CableReel"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CABLEREEL;
	if (strObjectName == TEXT("IceBox"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ICEBOX;
#pragma endregion
#pragma region SpecialDynamic
	if (strObjectName == TEXT("Special_BeamA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA;
	if (strObjectName == TEXT("HeavyEquipmentG"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTG;
	if (strObjectName == TEXT("Special_Rock0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0;
#pragma endregion

#pragma region Building
	if (strObjectName == TEXT("RealSteelWallD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLD;
	if (strObjectName == TEXT("RealSteelWallE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLE;
	if (strObjectName == TEXT("SteelWall4ANon"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4ANON;
	if (strObjectName == TEXT("SteelWall3A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3A;
	if (strObjectName == TEXT("SteelWall3B"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3B;
	if (strObjectName == TEXT("CityBuilding_PartsA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_PARTSA;
	if (strObjectName == TEXT("CityBuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDINGA;
	if (strObjectName == TEXT("ConstructionBuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGA;
	if (strObjectName == TEXT("ConstructionBuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGB;
	if (strObjectName == TEXT("Crane_EnkeiA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CRANE_ENKEIA;
	if (strObjectName == TEXT("DistantBuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_DISTANTBUILDINGA;
	if (strObjectName == TEXT("Ironpillar"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_IRONPILLAR;
	if (strObjectName == TEXT("TarpA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TARPA;
	if (strObjectName == TEXT("TarpB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TARPB;
	if (strObjectName == TEXT("TarpC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TARPC;
	if (strObjectName == TEXT("PlyWoodC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PLYWOODC;
	if (strObjectName == TEXT("PlyWoodD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PLYWOODD;
	if (strObjectName == TEXT("Ground_CoalA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA;
	if (strObjectName == TEXT("CityBuilding_Tower"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_TOWER;
	if (strObjectName == TEXT("MizuhaBuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGA;
	if (strObjectName == TEXT("MizuhaBuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGB;
	if (strObjectName == TEXT("MizuhaBuildingC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGC;
	if (strObjectName == TEXT("MizuhaBuildingD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGD;
	if (strObjectName == TEXT("MizuhaBuildingE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGE;
	if (strObjectName == TEXT("MizuhaBuildingF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGF;
#pragma endregion

#pragma region ETC
	if (strObjectName == TEXT("Ground_CarC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_CARC;
	if (strObjectName == TEXT("Ground_Grass"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_GRASS;
	if (strObjectName == TEXT("RoadBlockA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKA;
	if (strObjectName == TEXT("Ground_ConeA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEA;
	if (strObjectName == TEXT("Ground_ConeB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEB;
	if (strObjectName == TEXT("TanA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TANA;
	if (strObjectName == TEXT("TanB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TANB;
	if (strObjectName == TEXT("TanC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TANC;
	if (strObjectName == TEXT("TanD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TAND;
	if (strObjectName == TEXT("TanE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TANE;
	if (strObjectName == TEXT("ConstructionGateA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONGATEA;
#pragma endregion

#pragma region Elevator
	if (strObjectName == TEXT("ConstructionElevator"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONELEVATOR;

#pragma endregion

#pragma region LightObject
	if (strObjectName == TEXT("SpotlightA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA;
	if (strObjectName == TEXT("FloodlightB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FLOODLIGHTB;
#pragma endregion

#pragma region Neon
	if (strObjectName == TEXT("NeonCornerA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERA;
	if (strObjectName == TEXT("NeonCornerB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERB;
	if (strObjectName == TEXT("NeonCornerC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERC;
	if (strObjectName == TEXT("NeonCornerD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERD;
	if (strObjectName == TEXT("NeonCrashA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA;
	if (strObjectName == TEXT("NeonCrashB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB;
	if (strObjectName == TEXT("NeonLineA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA;
	if (strObjectName == TEXT("NeonLineB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB;
	if (strObjectName == TEXT("NeonRollA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA;
	if (strObjectName == TEXT("NeonSwayA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA;
	if (strObjectName == TEXT("NeonSwayB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB;
	if (strObjectName == TEXT("NeonSwayC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC;
#pragma endregion

	return strPrototypeName;

}

wstring CMap_ToolManager::SuohMap_Name(wstring strObjectName)
{
	wstring strPrototypeName = TEXT("");

	if (strObjectName == TEXT("Common_BaseWall"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL;
	if (strObjectName == TEXT("Common_Item"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM;
	if (strObjectName == TEXT("Shopkeeper"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SHOPKEEPER;
	if (strObjectName == TEXT("BusA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BUSA;

#pragma region Road

	if (strObjectName == TEXT("Suoh_Minimap"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_MINIMAP;
	if (strObjectName == TEXT("SG_Ground_A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_GROUND_A;
	if (strObjectName == TEXT("PedestrianBridgeA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PEDESTRIANBRIDGEA;
	if (strObjectName == TEXT("Suoh_Dodai"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_DODAI;
	if (strObjectName == TEXT("Suoh_Tile"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_TILE;
	if (strObjectName == TEXT("Town_RoadB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADB;
	if (strObjectName == TEXT("Town_RoadC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADC;
#pragma endregion

#pragma region Static
	if (strObjectName == TEXT("Suoh_WallB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLB;
	if (strObjectName == TEXT("Suoh_WallC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLC;
	if (strObjectName == TEXT("Suoh_WallD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLD;
	if (strObjectName == TEXT("SG_ShutterBase"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE;
	if (strObjectName == TEXT("Town_RoadA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADA;
	if (strObjectName == TEXT("SG_ShutterA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERA;
	if (strObjectName == TEXT("SG_FlowerPot"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_FLOWERPOT;
	if (strObjectName == TEXT("Suoh_Keijiban"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_KEIJIBAN;
	if (strObjectName == TEXT("Suoh_Sekihi"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SEKIHI;
	if (strObjectName == TEXT("Suoh_OmikujiB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_OMIKUJIB;
	if (strObjectName == TEXT("SG_ArchA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_ARCHA;
	if (strObjectName == TEXT("Sumeragi_KaidanA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUMERAGI_KAIDANA;
	if (strObjectName == TEXT("Suoh_RubbleA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_RUBBLEA;
	if (strObjectName == TEXT("SG_PlantsA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_PLANTSA;
	if (strObjectName == TEXT("SG_TreeA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_TREEA;
	if (strObjectName == TEXT("SG_Sumeragi_Tourou"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_SUMERAGI_TOUROU;	
	if (strObjectName == TEXT("MaitreyaTrii"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MAITREYATRII;
	if (strObjectName == TEXT("Suoh_CameraA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAA;
	if (strObjectName == TEXT("Suoh_CameraD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAD;
	if (strObjectName == TEXT("HydrantA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HYDRANTA;
	if (strObjectName == TEXT("TelephoneBoxA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TELEPHONEBOXA;
#pragma endregion

#pragma region Building
	if (strObjectName == TEXT("SG_AcademyA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_ACADEMYA;
	if (strObjectName == TEXT("Suoh_Tower"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_TOWER;
	if (strObjectName == TEXT("Suoh_Bridge"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BRIDGE;
	if (strObjectName == TEXT("SG_BuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGA;
	if (strObjectName == TEXT("SG_BuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGB;
	if (strObjectName == TEXT("SG_BuildingC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGC;
	if (strObjectName == TEXT("Suoh_A_BuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGA;
	if (strObjectName == TEXT("Suoh_A_BuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGB;
	if (strObjectName == TEXT("Suoh_B_BuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_B_BUILDINGA;
	if (strObjectName == TEXT("Suoh_C_BuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGA;
	if (strObjectName == TEXT("Suoh_C_BuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGB;
	if (strObjectName == TEXT("Suoh_C_BuildingC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGC;
	if (strObjectName == TEXT("Suoh_C_BuildingD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGD;
	if (strObjectName == TEXT("Suoh_D_BuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGA;
	if (strObjectName == TEXT("Suoh_D_BuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGB;
	if (strObjectName == TEXT("Suoh_D_BuildingC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGC;
	if (strObjectName == TEXT("Suoh_D_BuildingD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGD;
	if (strObjectName == TEXT("Suoh_E_BuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGA;
	if (strObjectName == TEXT("Suoh_E_BuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGB;
	if (strObjectName == TEXT("Suoh_E_BuildingC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGC;
	if (strObjectName == TEXT("Suoh_E_BuildingD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGD;
	if (strObjectName == TEXT("Suoh_E_BuildingE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGE;
	if (strObjectName == TEXT("Suoh_E_BuildingF"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGF;
	if (strObjectName == TEXT("Suoh_F_BuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGA;
	if (strObjectName == TEXT("Suoh_F_BuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGB;
	if (strObjectName == TEXT("Suoh_G_BuildingA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGA;
	if (strObjectName == TEXT("Suoh_G_BuildingB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGB;
	if (strObjectName == TEXT("Suoh_G_BuildingC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGC;
	if (strObjectName == TEXT("Suoh_CafeBuilding"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAFEBUILDING;
#pragma endregion

#pragma region Signboard
	if (strObjectName == TEXT("Suoh_Billboard_A0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A0;
	if (strObjectName == TEXT("Suoh_Billboard_A1"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A1;
	if (strObjectName == TEXT("Suoh_Billboard_A2"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A2;
	if (strObjectName == TEXT("Suoh_Billboard_A3"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A3;
	if (strObjectName == TEXT("Suoh_Billboard_C0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_C0;
	if (strObjectName == TEXT("Suoh_Billboard_D0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_D0;
	if (strObjectName == TEXT("Suoh_Billboard_G0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G0;
	if (strObjectName == TEXT("Suoh_Billboard_I0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I0;
	if (strObjectName == TEXT("Suoh_Billboard_I1"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I1;
	if (strObjectName == TEXT("Suoh_Billboard_I2"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I2;
	if (strObjectName == TEXT("Suoh_Billboard_I3"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I3;
	if (strObjectName == TEXT("Suoh_Billboard_J0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_J0;
	if (strObjectName == TEXT("Suoh_GuideA_A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_A;
	if (strObjectName == TEXT("Suoh_GuideA_B"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_B;
	if (strObjectName == TEXT("Suoh_GuideA_C"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_C;
	if (strObjectName == TEXT("Suoh_GuideB_A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEB_A;
	if (strObjectName == TEXT("Suoh_GuideC_A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_A;
	if (strObjectName == TEXT("Suoh_GuideC_B"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_B;
	if (strObjectName == TEXT("Suoh_GuideC_C"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_C;
	if (strObjectName == TEXT("Suoh_GuideC_D"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_D;
	if (strObjectName == TEXT("Suoh_SignA_A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_A;
	if (strObjectName == TEXT("Suoh_SignA_B"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_B;
	if (strObjectName == TEXT("Suoh_SignA_C"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_C;
	if (strObjectName == TEXT("Suoh_SignB_A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNB_A;
	if (strObjectName == TEXT("Suoh_SignD_D"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGND_D;
	if (strObjectName == TEXT("Suoh_SignE"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNE;
	if (strObjectName == TEXT("Suoh_BillboardX_A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_A;
	if (strObjectName == TEXT("Suoh_BillboardX_B"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_B;
	if (strObjectName == TEXT("Suoh_BillboardX_C"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_C;
	if (strObjectName == TEXT("Suoh_BillboardX_D"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_D;
	if (strObjectName == TEXT("Suoh_BillboardX_E"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_E;
	if (strObjectName == TEXT("Suoh_BillboardX_F"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_F;
	if (strObjectName == TEXT("Suoh_BillboardX_G"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_G;
#pragma endregion

#pragma region ETC
	if (strObjectName == TEXT("Suoh_BicycleA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEA;
	if (strObjectName == TEXT("Suoh_BicycleB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEB;
	if (strObjectName == TEXT("Suoh_BicycleC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEC;
	if (strObjectName == TEXT("Suoh_ScooterA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERA;
	if (strObjectName == TEXT("Suoh_ScooterB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERB;
	if (strObjectName == TEXT("Suoh_ShopBox"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SHOPBOX;
	if (strObjectName == TEXT("Suoh_Billboard_B0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B0;
	if (strObjectName == TEXT("Suoh_Billboard_B1"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B1;
	if (strObjectName == TEXT("Suoh_Billboard_B2"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B2;
	if (strObjectName == TEXT("Suoh_Billboard_B3"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B3;
	if (strObjectName == TEXT("Suoh_Billboard_F0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_F0;
	if (strObjectName == TEXT("Suoh_Billboard_G1"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G1;
	if (strObjectName == TEXT("Suoh_Billboard_G2"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G2;
	if (strObjectName == TEXT("Suoh_Billboard_H0"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H0;
	if (strObjectName == TEXT("Suoh_Billboard_H1"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H1;
	if (strObjectName == TEXT("Town_BoxA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXA;
	if (strObjectName == TEXT("Town_BoxB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXB;
	if (strObjectName == TEXT("Town_BoxC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXC;
	if (strObjectName == TEXT("Town_BoxD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD;
	if (strObjectName == TEXT("Town_CarrierA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERA;
	if (strObjectName == TEXT("Town_CarrierB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERB;
	if (strObjectName == TEXT("Town_CarrierC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERC;
#pragma endregion

#pragma region Dynamic
	if (strObjectName == TEXT("Suoh_CarA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARA;
	if (strObjectName == TEXT("Suoh_CarB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARB;
	if (strObjectName == TEXT("Suoh_CarC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARC;
	if (strObjectName == TEXT("Suoh_CarD"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARD;
#pragma endregion

#pragma region NPC
	if (strObjectName == TEXT("Suoh_NPC_Child"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_CHILD;
	if (strObjectName == TEXT("Suoh_NPC_Man"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_MAN;
	if (strObjectName == TEXT("Suoh_NPC_Woman"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_WOMAN;
#pragma endregion

#pragma region Neon
	if (strObjectName == TEXT("Suoh_Battle_WarningWallA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLA;
	if (strObjectName == TEXT("Suoh_Battle_WarningWallB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLB;
#pragma endregion

	return strPrototypeName;
}

wstring CMap_ToolManager::MindRoom_Name(wstring strObjectName)
{
	wstring strPrototypeName = TEXT("");

	if (strObjectName == TEXT("Common_BaseWall"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL;
	if (strObjectName == TEXT("Common_Item"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM;
#pragma region Road
	if (strObjectName == TEXT("Sankaku_futa"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_FUTA;
	if (strObjectName == TEXT("MindRoom_Water"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_WATER;
#pragma endregion

#pragma region Static
	if (strObjectName == TEXT("Sankaku_A"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_A;
	if (strObjectName == TEXT("Mindroom_StoneA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEA;
	if (strObjectName == TEXT("Mindroom_StoneB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEB;
	if (strObjectName == TEXT("Mindroom_StoneC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEC;
	if (strObjectName == TEXT("Mindroom_Spaceship"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SPACESHIP;
#pragma endregion

#pragma region Neon
	if (strObjectName == TEXT("MindRoom_Tenkyu"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TENKYU;
	if (strObjectName == TEXT("MindRoom_Floor"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_FLOOR;	
	if (strObjectName == TEXT("MindRoom_Twist"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TWIST;	
	if (strObjectName == TEXT("MindRoom_Redline"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_REDLINE;
#pragma endregion

#pragma region Dynamic
	if (strObjectName == TEXT("Mindroom_Box"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX;
	if (strObjectName == TEXT("Mindroom_CrateA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA;
	if (strObjectName == TEXT("Mindroom_CrateB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB;
#pragma endregion

	return strPrototypeName;
}

wstring CMap_ToolManager::Hideout_Name(wstring strObjectName)
{
	wstring strPrototypeName = TEXT("");

	if (strObjectName == TEXT("Common_BaseWall"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL;
	if (strObjectName == TEXT("Common_Item"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM;	
	if (strObjectName == TEXT("Arashi_Teammate"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI_TEAMMATE;

#pragma region Static
	if (strObjectName == TEXT("Hideout_Floor"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FLOOR;
	if (strObjectName == TEXT("Hideout_Carpet"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_CARPET;
	if (strObjectName == TEXT("Hideout_Cole"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_COLE;
	if (strObjectName == TEXT("Hideout_Fireplace"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FIREPLACE;
	if (strObjectName == TEXT("Hideout_Kitchen"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KITCHEN;
	if (strObjectName == TEXT("Hideout_LowTable"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_LOWTABLE;
	if (strObjectName == TEXT("Hideout_Pouf"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_POUF;
	if (strObjectName == TEXT("Hideout_SofaA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAA;
	if (strObjectName == TEXT("Hideout_SofaB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAB;
	if (strObjectName == TEXT("Hideout_SofaC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAC;
	if (strObjectName == TEXT("Hideout_FabricChair"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FABRICCHAIR;
	if (strObjectName == TEXT("Hideout_MounatinScreen"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_MOUNATINSCREEN;
	if (strObjectName == TEXT("Hideout_PerfectBodyPhoto"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_PERFECTBODYPHOTO;
	if (strObjectName == TEXT("Hideout_Table"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TABLE;
	if (strObjectName == TEXT("Hideout_TeamPhoto"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TEAMPHOTO;
	if (strObjectName == TEXT("Hideout_BenchA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHA;
	if (strObjectName == TEXT("Hideout_BenchPress"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHPRESS;
	if (strObjectName == TEXT("Hideout_DumbbellA"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLA;
	if (strObjectName == TEXT("Hideout_DumbbellB"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLB;
	if (strObjectName == TEXT("Hideout_DumbbellC"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLC;
	if (strObjectName == TEXT("Hideout_Kettlebell"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KETTLEBELL;
	if (strObjectName == TEXT("Hideout_DumbbellRack"))
		strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLRACK;
#pragma endregion

	return strPrototypeName;
}

