#pragma once

#include "Map_Tool.h"
#include "GameInstance.h"

#if ACTIVATE_TOOL

void CMap_Tool::Create_MindRoom()
{
	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
	{ SCENE::MAPTOOL, m_strObjName,std::get<1>(m_tuplePivot),std::get<2>(m_tuplePivot),std::get<3>(m_tuplePivot),std::get<4>(m_tuplePivot), m_strObjName };

	if (m_strFileName == TEXT("Common_BaseWall"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}

#pragma region Road
	if (m_strFileName == TEXT("Sankaku_futa"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_FUTA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("MindRoom_Water"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_WATER, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Static
	if (m_strFileName == TEXT("Sankaku_A"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_A, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Mindroom_StoneA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Mindroom_StoneB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Mindroom_StoneC"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEC, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Mindroom_Spaceship"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SPACESHIP, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Building

#pragma endregion

#pragma region ETC
	if (m_strFileName == TEXT("MindRoom_Tenkyu"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TENKYU, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("MindRoom_Floor"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_FLOOR, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("MindRoom_Twist"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TWIST, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}	
	if (m_strFileName == TEXT("MindRoom_Redline"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_REDLINE, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion

#pragma region Dynamic
	if (m_strFileName == TEXT("Mindroom_Box"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Mindroom_CrateA"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
	if (m_strFileName == TEXT("Mindroom_CrateB"))
	{
		if (FAILED(m_RoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL,
			PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple)))))
			return;
	}
#pragma endregion
}

#endif
