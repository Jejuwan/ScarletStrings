#pragma once
#include "Loader.h"
#include "GameInstance.h"

#include "TitleBackground.h"
#include "TitleCharacter.h"
#include "Camera_Main.h"

#include "TitleLogo.h"
#include "TitleIcon.h"

inline HRESULT CLoader::Load_TitleScene()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_CAMERA_CUTSCENE,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MindRoom", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TITLE, PROTOTYPE_COMPONENT_TEXTURE_TITLE_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Title/TitleBG.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TITLE_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_BACKGROUND,
		CTitleBackground::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TITLE_BACKGROUND");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TITLE, PROTOTYPE_COMPONENT_TEXTURE_TITLE_LOGO,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Title/T_ui_Start_Logo_1.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TITLE_LOGO");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_LOGO,
		CTitleLogo::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TITLE_LOGO");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TITLE, PROTOTYPE_COMPONENT_TEXTURE_TITLE_ICON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Title/TitleIcon.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TITLE_ICON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_ICON,
		CTitleIcon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TITLE_ICON");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TITLE, PROTOTYPE_COMPONENT_MODEL_KASANE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/Kasane_Cutscene.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KASANE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TITLE, PROTOTYPE_COMPONENT_MODEL_YUITO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Yuito/Yuito.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_YUITO");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_CHARACTER,
		CTitleCharacter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TitleScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TITLE_CHARACTER");
	}
	

	return S_OK;
}