#pragma once

#include "Map_Tool.h"
#include "GameInstance.h"

#if ACTIVATE_TOOL

void CMap_Tool::Create_Tutorial() 
{
	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
	{ SCENE::MAPTOOL, m_strObjName,std::get<1>(m_tuplePivot),std::get<2>(m_tuplePivot),std::get<3>(m_tuplePivot),std::get<4>(m_tuplePivot), m_strObjName };

#pragma region Road
	if (m_strFileName == TEXT("Tutorial_Road0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_Road1"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD1, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_Road2"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD2, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_Road3"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD3, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_Road4"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD4, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_Road5"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD5, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_Sakura"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_SAKURA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad0"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD0, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad1"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD1, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad2"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD2, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad3"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD3, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad4"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD4, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad5"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD5, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad6"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD6, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad7"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD7, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad8"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD8, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoad9"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD9, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoadA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoadB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_EnterRoadC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_RoadWall"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROADWALL, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

	if (m_strFileName == TEXT("LogsA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_LOGSA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("LogsB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_LOGSB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("LogsC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_LOGSC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Sakura_BrakA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Sakura_BrakB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Sakura_BranchA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Sakura_BranchB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Sakura_BranchC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("StoneA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_STONEA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("BeechE"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_BEECHE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Pillar"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_PILLAR, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("HugeTree"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HUGETREE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_RootA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_RootB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_RootC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_RootD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_RootE"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_RootF"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTF, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_RootG"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTG, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("HugeDarkTree"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HUGEDARKTREE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_DarkRootA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_DarkRootB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_DarkRootC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_DarkRootD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tutorial_DarkRootE"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Dynamic
	if (m_strFileName == TEXT("CarA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_CARA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("CarB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_CARB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("CarC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_CARC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("CarD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_CARD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("CarD_Brk"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_CARD_Brk, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("ConcreteBarrierA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("ConcreteBarrierB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SeiranCar"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("SeiranCar_Brk"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR_BRK, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Tractor"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TRACTOR, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Static
	if (m_strFileName == TEXT("TruckA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TRUCKA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Truck_CampingCar"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CAMPINGCAR, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Truck_Container"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CONTAINER, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Truck_Gas"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GAS, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Truck_GasBrk"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GASBRK, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("BusA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_BUSA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

	if (m_strFileName == TEXT("Grass"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_GRASS, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("BeechA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_BEECHA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("BeechB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_BEECHB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("BeechC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_BEECHC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("BeechD"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_BEECHD, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
}

#endif
