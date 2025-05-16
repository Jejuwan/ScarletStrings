#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "Camera_Main.h"
#include "GlobalGizmo.h"
#include "SkyCube.h"
#include "VIBufferInstance_Point.h"
#include "VIBufferInstance_Rect.h"

#if ACTIVATE_TOOL

HRESULT CLoader::Load_ToolScene()
{
#pragma region Prototype Texture
#pragma endregion
#pragma region Prototype Model
#pragma endregion
#pragma region Prototype VIBuffer

#pragma endregion
#pragma region Prototype GameObject

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TOOL, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}
	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TOOL, PROTOTYPE_GAMEOBJECT_GLOBALGIZMO,
		CGlobalGizmo::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_GLOBALGIZMO");
	}
	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TOOL, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE");
	}

#pragma endregion

	return S_OK;
}


#endif
