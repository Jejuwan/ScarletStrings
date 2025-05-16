#pragma once

#include "Map_Tool.h"
#include "GameInstance.h"

#if ACTIVATE_TOOL

void CMap_Tool::Create_Suoh()
{
	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
	{ SCENE::MAPTOOL, m_strObjName,std::get<1>(m_tuplePivot),std::get<2>(m_tuplePivot),std::get<3>(m_tuplePivot),std::get<4>(m_tuplePivot), m_strObjName };

	if (m_strFileName == TEXT("Common_Item"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Shopkeeper"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SHOPKEEPER, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

	if (m_strFileName == TEXT("BusA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_BUSA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

#pragma region Road
	if (m_strFileName == TEXT("Suoh_Minimap"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_MINIMAP, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_Ground_A"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_GROUND_A, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("PedestrianBridgeA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_PEDESTRIANBRIDGEA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Dodai"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_DODAI, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Tile"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_TILE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_RoadB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_RoadC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Static
	if (m_strFileName == TEXT("Suoh_WallB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_WallC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_WallD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_ShutterBase"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_RoadA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_ShutterA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_FlowerPot"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_FLOWERPOT, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Keijiban"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_KEIJIBAN, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Sekihi"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SEKIHI, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_OmikujiB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_OMIKUJIB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_ArchA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_ARCHA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Sumeragi_KaidanA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUMERAGI_KAIDANA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_RubbleA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_RUBBLEA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_PlantsA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_PLANTSA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_TreeA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_TREEA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_Sumeragi_Tourou"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_SUMERAGI_TOUROU, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("MaitreyaTrii"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MAITREYATRII, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Common_BaseWall"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_CameraA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_CameraD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("HydrantA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HYDRANTA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("TelephoneBoxA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TELEPHONEBOXA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Building
	if (m_strFileName == TEXT("SG_AcademyA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_ACADEMYA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Tower"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_TOWER, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Bridge"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BRIDGE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_BuildingA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_BuildingB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SG_BuildingC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_A_BuildingA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_A_BuildingB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_B_BuildingA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_B_BUILDINGA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_C_BuildingA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_C_BuildingB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_C_BuildingC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_C_BuildingD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_D_BuildingA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_D_BuildingB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_D_BuildingC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_D_BuildingD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_E_BuildingA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_E_BuildingB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_E_BuildingC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_E_BuildingD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_E_BuildingE"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_E_BuildingF"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGF, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_F_BuildingA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_F_BuildingB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_G_BuildingA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_G_BuildingB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_G_BuildingC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_CafeBuilding"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAFEBUILDING, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

#pragma endregion

#pragma region Signboard

	if (m_strFileName == TEXT("Suoh_Billboard_A0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_A1"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A1, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_A2"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A2, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_A3"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A3, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

	if (m_strFileName == TEXT("Suoh_Billboard_C0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_C0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_D0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_D0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

	if (m_strFileName == TEXT("Suoh_Billboard_G0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

	if (m_strFileName == TEXT("Suoh_Billboard_I0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_I1"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I1, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_I2"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I2, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_I3"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I3, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_J0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_J0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_GuideA_A"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_A, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_GuideA_B"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_B, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_GuideA_C"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_C, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_GuideB_A"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEB_A, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_GuideC_A"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_A, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_GuideC_B"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_B, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_GuideC_C"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_C, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_GuideC_D"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_D, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_SignA_A"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_A, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_SignA_B"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_B, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_SignA_C"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_C, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_SignB_A"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNB_A, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_SignD_D"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGND_D, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_SignE"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BillboardX_A"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_A, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BillboardX_B"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_B, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BillboardX_C"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_C, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BillboardX_D"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_D, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BillboardX_E"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_E, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BillboardX_F"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_F, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BillboardX_G"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_G, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}


#pragma endregion

#pragma region ETC
	if (m_strFileName == TEXT("Suoh_BicycleA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BicycleB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_BicycleC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_ScooterA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_ScooterB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_ShopBox"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_SHOPBOX, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_B0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_B1"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B1, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_B2"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B2, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_B3"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B3, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_F0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_F0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_G1"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G1, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_G2"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G2, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_H0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Billboard_H1"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H1, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_BoxA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_BoxB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_BoxC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_BoxD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_CarrierA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_CarrierB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Town_CarrierC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Dynamic
	if (m_strFileName == TEXT("Suoh_CarA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_CarB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_CarC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_CarD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region NPC
	if (m_strFileName == TEXT("Suoh_NPC_Child"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_CHILD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_NPC_Man"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_MAN, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_NPC_Woman"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_WOMAN, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Neon
	if (m_strFileName == TEXT("Suoh_Battle_WarningWallA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Suoh_Battle_WarningWallB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion
}

#endif
