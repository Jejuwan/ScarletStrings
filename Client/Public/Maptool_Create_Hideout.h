#pragma once

#include "Map_Tool.h"
#include "GameInstance.h"

#if ACTIVATE_TOOL

void CMap_Tool::Create_Hideout()
{
	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
	{ SCENE::MAPTOOL, m_strObjName,std::get<1>(m_tuplePivot),std::get<2>(m_tuplePivot),std::get<3>(m_tuplePivot),std::get<4>(m_tuplePivot), m_strObjName };

	if (m_strFileName == TEXT("Common_BaseWall"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Common_Item"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Arashi_Teammate"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI_TEAMMATE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

#pragma region Static
	if (m_strFileName == TEXT("Hideout_Floor"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FLOOR, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_Carpet"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_CARPET, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_Cole"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_COLE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_Fireplace"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FIREPLACE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_Kitchen"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KITCHEN, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_LowTable"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_LOWTABLE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_Pouf"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_POUF, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_SofaA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_SofaB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_SofaC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_FabricChair"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FABRICCHAIR, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_MounatinScreen"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_MOUNATINSCREEN, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_PerfectBodyPhoto"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_PERFECTBODYPHOTO, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_Table"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TABLE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_TeamPhoto"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TEAMPHOTO, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_BenchA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_BenchPress"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHPRESS, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_DumbbellA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_DumbbellB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_DumbbellC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_Kettlebell"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KETTLEBELL, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Hideout_DumbbellRack"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLRACK, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

#pragma endregion

}

#endif
