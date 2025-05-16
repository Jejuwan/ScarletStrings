#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "VIBuffer_Cube.h"
#include "Camera_Main.h"
#include "SkyCube.h"
#include "NoiseCloud.h"

#pragma region Map
#include "Map_Wall.h"
#include "Map_Item.h"
#include "Map_Hideout_Static.h"
#include "Map_Hideout_Arashi.h"
#pragma endregion 

#pragma region UI
#include "UI_Shop.h"
#include "UI_Shop_Menu.h"
#include "UI_Shop_Fade.h"
#include "UI_Shop_Info.h"
#include "UI_Indicator_Communication.h"
#pragma endregion



#include "Kyoka.h"
#include "Kagero.h"
#include "Arashi.h"
#include "Shiden.h"

#include "MenuCharacter.h"

#ifdef _DEBUG
#include "TestModel.h"
#endif


inline HRESULT CLoader::Load_HideoutScene()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

#pragma region Shaders

#pragma endregion

#pragma region Textures

#pragma endregion

#pragma region GameObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_PLAYER,
		CPlayer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS,
		CPsychokinesis::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON,
		CPlayerWeapon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE,
		CPlayerTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER,
		CCamera_Player::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI,
		CArashi::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN,
		CShiden::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA,
		CKyoka::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO,
		CKagero::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER,
		CMenuCharacter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER");
	}

#pragma endregion

#pragma region MODEL
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_KASANE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kasane/Kasane.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KASANE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_ARASHI,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_SHIDEN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Shiden/Shiden.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_KYOKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kyoka/Kyoka_Default_Blend.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_KAGERO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kagero/Kagero.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KAGERO");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_PLAYER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/player.mdl"), g_mUnit, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Weapon/wp200.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacle.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_MONSTER_DISPENPERRY,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/DispenPerry.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}



#ifdef _DEBUG
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, TEXT("TestModel"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/shadowtest.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, TEXT("TestModel"),
		CTestModel::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX");
	}

#endif
#pragma endregion

#pragma region Monster

#pragma endregion

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_ARASHI_TEAMMATE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi_Teammate.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI_TEAMMATE");
	}

	auto pModelRenderer = dynamic_pointer_cast<CModelRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER));

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Common_BaseWall.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_COMMON_ITEM,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Common/mdl/Common_Item.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_COMMON_ITEM");
	}

#pragma region Prototype Model_Static
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_FLOOR,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Floor.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_CARPET,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Carpet.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_COLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Cole.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_FIREPLACE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Fireplace.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_KITCHEN,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Kitchen.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_LOWTABLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_LowTable.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_POUF,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Pouf.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_SOFAA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_SofaA.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_SOFAB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_SofaB.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_SOFAC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_SofaC.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_FABRICCHAIR,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_FabricChair.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_MOUNATINSCREEN,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_MounatinScreen.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_PERFECTBODYPHOTO,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_PerfectBodyPhoto.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_TABLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Table.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_TEAMPHOTO,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_TeamPhoto.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_BENCHA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_BenchA.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_BENCHPRESS,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_BenchPress.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_DUMBBELLA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_DumbbellA.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_DUMBBELLB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_DumbbellB.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_DUMBBELLC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_DumbbellC.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_KETTLEBELL,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Kettlebell.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_DUMBBELLRACK,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_DumbbellRack.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
#pragma endregion


#pragma region UI
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Background2.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Psyco_Wave/PsycoWave.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemBox.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemTag.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_B,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_B.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_B");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLSLOT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillSlot.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillSlot");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_CHANGERICON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/ChangerIcon.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ChangerIcon");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER_LIGHT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger_Light.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger_Light");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_Drive_Background");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Gauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_SideGauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BLACK_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/UI_MenuBackground.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BRAINMAP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Seperate_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_ID_CARD,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER");
	}
	
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_SUBMENU,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Sub_Menu.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SUBMENU");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_COMMUNICATION,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Icon%d.png"), 3, true))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_COMMUNICATION");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_MODEL_MENU,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Menu/Menu_Sofa.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_G.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_TOWN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Background1.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::HIDEOUT, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_CURSOR,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Cursor_1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_CURSOR");
	}
#pragma endregion

#pragma region Prototype GameObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CLOUD,
		CNoiseCloud::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CLOUD");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Static
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FLOOR,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_CARPET,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_CARPET");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_COLE,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_COLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FIREPLACE,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FIREPLACE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KITCHEN,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KITCHEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_LOWTABLE,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_LOWTABLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_POUF,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_POUF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAA,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAB,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAC,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_Hideout_SofaC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FABRICCHAIR,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FABRICCHAIR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_MOUNATINSCREEN,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_MOUNATINSCREEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_PERFECTBODYPHOTO,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_PERFECTBODYPHOTO");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TABLE,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TABLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TEAMPHOTO,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TEAMPHOTO");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHA,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHPRESS,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHPRESS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLA,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLB,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLC,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KETTLEBELL,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KETTLEBELL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLRACK,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KETTLEBELL");
	}
#pragma endregion

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL,
		CMap_Wall::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM,
		CMap_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI_TEAMMATE,
		CMap_Hideout_Arashi::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI");
	}

#pragma region Prototype GameObject_Map_Suoh_ETC

#pragma endregion

#pragma region UI
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND,
		CUI_HP_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_HP,
		CUI_HP::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE,
		CUI_Psyco_Wave::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPPROTOTYPE_GAMEOBJECT_UI_PSYCOWAVEE_GAMEOBJECT_UI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX,
		CUI_ItemTag::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG,
		CUI_ItemBox::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMTAG");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L,
		CUI_ItemHUD_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R,
		CUI_ItemHUD_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B,
		CUI_ItemHUD_B::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT,
		CUI_SkillSlot::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER,
		CUI_SkillChanger::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON,
		CUI_ChangerIcon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_CHANGERICON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L,
		CUI_SkillChangerKey_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R,
		CUI_SkillChangerKey_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT,
		CUI_SkillChanger_Light::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND,
		CUI_Drive_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE,
		CUI_DriveGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE,
		CUI_Drive_SideGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND,
		CUI_Menu_Black_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND,
		CUI_Menu_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER,
		CUI_Menu_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY,
		CUI_Menu_Category_Party::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM,
		CUI_Menu_Category_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP,
		CUI_Menu_Category_Equip::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP,
		CUI_Menu_Category_BrainMap::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS,
		CUI_Menu_Category_SAS::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY,
		CUI_Menu_Category_Story::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY,
		CUI_Menu_Category_Library::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION,
		CUI_Menu_Category_Option::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L,
		CUI_Menu_Arrow_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R,
		CUI_Menu_Arrow_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON,
		CUI_Menu_Button::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER,
		CUI_Menu_Profile_PartyLeader::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1,
		CUI_Menu_Profile_PartyMember1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2,
		CUI_Menu_Profile_PartyMember2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1,
		CUI_Menu_Profile_PartySupport1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2,
		CUI_Menu_Profile_PartySupport2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE,
		CUI_Menu_Seperate_Line::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER,
		CUI_Menu_Profile_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD,
		CUI_Menu_Profile_ID_Card::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER,
		CUI_Menu_Fade_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK,
		CUI_Indicator_ObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK,
		CUI_Indicator_SpcObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU,
		CUI_Menu_Models::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN,
		CUI_Indicator_Name_Town::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC,
		CUI_Indicator_Tooltip_Tutorial_PMCC::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_COMMUNICATION,
		CUI_Indicator_Communication::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_COMMUNICATION");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_CURSOR,
		CUI_Indicator_Cursor::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_CURSOR");
	}



	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SHOP,
		CUI_Shop::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SHOP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SHOP_MENU,
		CUI_Shop_Menu::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SHOP_MENU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SHOP_FADE,
		CUI_Shop_Fade::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SHOP_FADE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SHOP_INFO,
		CUI_Shop_Info::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SHOP_OUTLINE");
	}


#pragma endregion
	
	return S_OK;
}