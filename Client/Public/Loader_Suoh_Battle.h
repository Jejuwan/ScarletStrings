#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "VIBuffer_Cube.h"
#include "Camera_Main.h"
#include "SkyCube.h"
#include "NoiseCloud.h"

#pragma region UI
#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Electric.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Acceleration.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Replication.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Stealth.h"

#pragma endregion
#pragma region Map
#include "Map_NeonLine.h"
#include "Map_Suoh_Road.h"
#include "Map_Suoh_Building.h"
#include "Map_Suoh_Static.h"
#include "Map_Wall.h"
#include "Map_Suoh_Signboard.h"
#include "Map_Suoh_ETC.h"
#include "Map_Suoh_Dynamic.h"
#include "Map_Suoh_Child.h"
#include "Map_Suoh_Man.h"
#include "Map_Suoh_Woman.h"
#include "NPC_Part.h"
#include "Map_Suoh_Neon.h"
#include "Map_Item.h"
#include "Map_Suoh_Bus.h"
#pragma endregion 

#include "Kyoka.h"
#include "Kagero.h"
#include "Arashi.h"
#include "Shiden.h"

#include "Monster_FlowerHead.h"
#include "Monster_Projectile.h"

#ifdef _DEBUG
#include "TestModel.h"
#endif

#include "MenuCharacter.h"

inline HRESULT CLoader::Load_SuohBattleScene()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

#pragma region Shaders

#pragma endregion

#pragma region Textures

#pragma endregion

#pragma region GameObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_PLAYER,
		CPlayer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS,
		CPsychokinesis::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON,
		CPlayerWeapon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE,
		CPlayerTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER,
		CCamera_Player::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI,
		CArashi::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN,
		CShiden::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA,
		CKyoka::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO,
		CKagero::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER,
		CMenuCharacter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER");
	}

#pragma endregion

#pragma region MODEL
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_KASANE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kasane/Kasane.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KASANE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_ARASHI,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_SHIDEN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Shiden/Shiden.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_KYOKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kyoka/Kyoka_Default_Blend.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_KAGERO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kagero/Kagero.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KAGERO");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_PLAYER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/player.mdl"), g_mUnit, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Weapon/wp200.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacle.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_MONSTER_DISPENPERRY,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/DispenPerry.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/FlowerHead.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_SUOH_BATTLE", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Bullet.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Weak01.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK");
	}

#ifdef _DEBUG
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, TEXT("TestModel"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/shadowtest.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, TEXT("TestModel"),
		CTestModel::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX");
	}

#endif
#pragma endregion

#pragma region Monster

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MONSTER_PROJECTILE,
		CMonster_Projectile::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PROJECTILE");
	}


	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MONSTER_WEAK,
		CMonster_Weak::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_WEAK");
	}

#pragma endregion

#pragma region Prototype Model_NeonLine
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONCORNERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONCORNERB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONCORNERC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerC.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONCORNERD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerD.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONCRASHA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCrashA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCRASHA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONCRASHB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCrashB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCRASHB");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONLINEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonLineA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONLINEA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONLINEB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonLineB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONLINEB");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONROLLA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonRollA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONROLLA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONSWAYA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONSWAYB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_NEONSWAYC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayC.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}

#pragma endregion

	auto pModelRenderer = dynamic_pointer_cast<CModelRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER));

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_COMMON_ITEM,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Common/mdl/Common_Item.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_COMMON_ITEM");
	}

#pragma region Prototype Model_SuohRoad

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_MINIMAP,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_MiniMap.mdl")), 1)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_MINIMAP");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_GROUND_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_Ground_A.mdl")), 1)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_GROUND_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PEDESTRIANBRIDGEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Battle_PedestrianBridgeA.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PEDESTRIANBRIDGEA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_DODAI,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Dodai.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_DODAI");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_TILE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Tile.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_TILE");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_ROADB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_RoadB.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_ROADB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_ROADC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_RoadC.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_ROADC");
	}

#pragma endregion

#pragma region Prototype Model_SuohStatic
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_WALLB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_WallB.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAP");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_WALLC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_WallC.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_WALLC");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_WALLD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_WallD.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_WALLD");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_SHUTTERBASE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_ShutterBase.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_SHUTTERBASE");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_ROADA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_RoadA.mdl")), 2)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_ROADA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_SHUTTERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_ShutterA.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_SHUTTERA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_FlowerPot.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_KEIJIBAN,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Keijiban.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_KEIJIBAN");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SEKIHI,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Sekihi.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SEKIHI");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_OMIKUJIB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_OmikujiB.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_OMIKUJIB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_ARCHA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_ArchA.mdl")), 1)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_ARCHA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUMERAGI_KAIDANA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Sumeragi_KaidanA.mdl")), 2)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUMERAGI_KAIDANA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_RUBBLEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_RubbleA.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_RUBBLEA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_PLANTSA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_PlantsA.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_PLANTSA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_TREEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_TreeA.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_TREEA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_SUMERAGI_TOUROU,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_Sumeragi_Tourou.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_SUMERAGI_TOUROU");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MAITREYATRII,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Battle_MaitreyaTrii.mdl")), 1)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MAITREYATRII");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Common_BaseWall.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CameraA.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_CameraD.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAD");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HYDRANTA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/HydrantA.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_HYDRANTA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TELEPHONEBOXA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/TelephoneBoxA.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TELEPHONEBOXA");
	}
#pragma endregion

#pragma region Prototype Model_SuohBuilding

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_ACADEMYA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_Battle_Academy.mdl")), 2)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_ACADEMYA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_TOWER,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Tower.mdl")), 2)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_TOWER");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BRIDGE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_Bridge.mdl")), 2)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BRIDGE");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_BuildingA.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_BUILDINGA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_BuildingB.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_BUILDINGB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_BUILDINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_BuildingC.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_BUILDINGC");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_A_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_A_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_A_BUILDINGA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_A_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_A_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_A_BUILDINGB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_B_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_B_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_B_BUILDINGA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_C_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_C_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_C_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_C_BuildingC.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGC");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_C_BuildingD.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGD");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_D_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_D_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_D_BuildingC.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGC");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_D_BuildingD.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGD");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_E_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_E_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_E_BuildingC.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGC");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_E_BuildingD.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGD");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_E_BuildingE.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGE");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGF,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_E_BuildingF.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGF");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_F_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_F_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_F_BUILDINGA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_F_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_F_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_F_BUILDINGB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_G_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_G_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_G_BUILDINGA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_G_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_G_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_G_BUILDINGB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_G_BUILDINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_G_BuildingC.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_G_BUILDINGC");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CAFEBUILDING,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CafeBuilding.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CAFEBUILDING");
	}


#pragma endregion

#pragma region Prototype Model_Suoh_Signboard
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_A0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A1,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_A1.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A1");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A2,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_A2.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A2");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A3,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_A3.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A3");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_C0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_C0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_C0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_D0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_D0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_D0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_G0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_I0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I1,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_I1.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I1");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I2,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_I2.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I2");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I3,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_I3.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I3");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_J0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_J0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_J0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEA_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_GuideA_A.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEA_A");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEA_B,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_GuideA_B.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I3");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEA_C,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_GuideA_C.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_I3");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEB_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_GuideB_A.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEB_A");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEC_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_GuideC_A.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEC_A");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEC_B,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_GuideC_B.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEC_B");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEC_C,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_GuideC_C.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEC_C");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEC_D,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_GuideC_D.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_GUIDEC_D");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNA_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_SignA_A.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNA_A");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNA_B,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_SignA_B.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNA_B");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNA_C,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_SignA_C.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNA_C");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNB_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_SignB_A.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNB_A");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SIGND_D,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_SignD_D.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGND_D");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_SignE.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNE");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_BillboardX_A.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_B,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_BillboardX_B.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_C,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_BillboardX_C.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_D,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_BillboardX_D.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_E,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_BillboardX_E.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_F,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_BillboardX_F.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_G,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_BillboardX_G.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARDX_A");
	}

#pragma endregion

#pragma region Prototype Model_Suoh_ETC
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_BicycleA.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNE");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_BicycleB.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEB");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_BicycleC.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEC");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SCOOTERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_ScooterA.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNE");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SCOOTERB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_ScooterB.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SIGNE");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SHOPBOX,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_ShopBox.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SHOPBOX");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_B0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B1,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_B1.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_A0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B2,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_B2.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B2");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B3,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_B3.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B3");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_F0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_F0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_F0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G1,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_G1.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G1");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G2,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_G2.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_Suoh_Billboard_G2");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_H0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_H0.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_H0");
	}

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_H1,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_H1.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_F0");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_BOXA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_BoxA.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_BOXA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_BOXB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_BoxB.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_BOXB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_BOXC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_BoxC.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_BOXC");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_BOXD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_BoxD.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_BOXD");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_CarrierA.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_CarrierB.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_CarrierC.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERC");
	}
#pragma endregion

#pragma region Prototype Model_Suoh_Dynamic
	auto pCarA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CarA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CARA, pCarA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_SUOH_CARA, pCarA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARA");
	}
	auto pCarB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CarB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CARB, pCarB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_SUOH_CARB, pCarB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARA");
	}
	auto pCarC = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CarC.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CARC, pCarC, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_SUOH_CARC, pCarC)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARA");
	}
	auto pCarD = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CarD.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CARD, pCarD, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_SUOH_CARD, pCarD)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARA");
	}
	auto pBusA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/BusA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BUSA, pBusA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_BUSA, pBusA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BUS");
	}

#pragma endregion

#pragma region Prototype Model_Suoh_NPC
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_CHILD,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_NPC_Child.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_CHILD");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_MAN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_NPC_Man.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_MAN");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_WOMAN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_NPC_Woman.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_WOMAN");
	}
#pragma endregion

#pragma region Prototype Model_Suoh_Neon
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_WarningWallA.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_WarningWallB.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLB");
	}
#pragma endregion



#pragma region UI
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Background2.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Psyco_Wave/PsycoWave.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemBox.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemTag.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_B,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_B.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_B");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLSLOT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillSlot.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillSlot");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_CHANGERICON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/ChangerIcon.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ChangerIcon");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER_LIGHT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger_Light.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger_Light");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_Drive_Background");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Gauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_SideGauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BLACK_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/UI_MenuBackground.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BRAINMAP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Seperate_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_ID_CARD,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_MODEL_MENU,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Menu/Menu_Sofa.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_G.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_TOWN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Background1.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_NOTICEMARK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Notice_M.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_LOCKON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/LockOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Monster_HpBackground.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_BOSS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/BossName_Background1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_BOSS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_TIPS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Tips_background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_TIPS");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BOSS_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Boss_Hp_Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BOSS_HP");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BRAINTALK_MIZUHA,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_Frame.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BRAINTALK_SUOH_BATTLE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_DAMAGE_FONT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/0.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_HEAL_FONT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/0.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_SAS_ELECTRIC,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_SAS_ELECTRIC");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_SAS_ACCELERATION,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_SAS_ACCELERATION");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_SAS_STEALTH,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_SAS_STEALTH");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_INSTKILL,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_INSTKILL");
	}
#pragma endregion

#pragma region Prototype GameObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CLOUD,
		CNoiseCloud::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CLOUD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD,
		CMonster_FlowerHead::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_NeonLine
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERC,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERD,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA");
	}
		
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB");;
	}
		
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC");
	}
		
#pragma endregion

#pragma region Prototype GameObject_Map_SuohRoad
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_MINIMAP,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_MINIMAP");
	}
		
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_GROUND_A,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_GROUND_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_PEDESTRIANBRIDGEA,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PEDESTRIANBRIDGEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_DODAI,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_DODAI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_TILE,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_TILE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADB,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADC,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADC");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_SuohStatic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLB,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLC,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLD,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_FLOWERPOT,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_FLOWERPOT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_KEIJIBAN,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_KEIJIBAN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SEKIHI,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SEKIHI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_OMIKUJIB,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_OMIKUJIB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_ARCHA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_ARCHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUMERAGI_KAIDANA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUMERAGI_KAIDANA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_RUBBLEA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_RUBBLEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_PLANTSA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_PLANTSA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_TREEA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_TREEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_SUMERAGI_TOUROU,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SUMERAGI_TOUROU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_MAITREYATRII,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MAITREYATRII");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAD,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_HYDRANTA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HYDRANTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TELEPHONEBOXA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TELEPHONEBOXA");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_SuohBuilding
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_ACADEMYA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_ACADEMYA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_TOWER,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_TOWER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BRIDGE,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_B_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_B_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGD,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_Suoh_D_BuildingC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGD,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGD,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGE,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGF,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGF");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAFEBUILDING,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAFEBUILDING");
	}
#pragma endregion


	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL,
		CMap_Wall::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK,
		CHitCrackDecal::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK");
	}

#pragma region Prototype GameObject_Map_Suoh_Signboard
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A1,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A2,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A3,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_C0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_C0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_D0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_D0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I1,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I2,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I3,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_J0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_J0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_B,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_C,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_C");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEB_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEB_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_B,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_C,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_C");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_D,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_D");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_B,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_C,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_C");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNB_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNB_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGND_D,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGND_D");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNE,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_B,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_C,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_C");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_D,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 29))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_D");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_E,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 30))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_E");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_F,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 31))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_F");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_G,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 32))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_G");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_ETC
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEA,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEB,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEC,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERA,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERB,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SHOPBOX,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SHOPBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B0,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B1,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B2,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B3,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_F0,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_F0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G1,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G2,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H0,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H1,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXA,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXB,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXC,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERA,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERB,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERC,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_Dynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARA,
		CMap_Suoh_Dynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARB,
		CMap_Suoh_Dynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARC,
		CMap_Suoh_Dynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARD,
		CMap_Suoh_Dynamic::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARD");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_NPC
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_CHILD,
		CMap_Suoh_Child::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_CHILD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_PART,
		CNPC_Part::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_CHILD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_MAN,
		CMap_Suoh_Man::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_MAN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_WOMAN,
		CMap_Suoh_Woman::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_WOMAN");
	}
#pragma endregion

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM,
		CMap_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_BUSA,
		CMap_Suoh_Bus::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLB");
	}

#pragma region Prototype GameObject_Map_Suoh_Neon
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLA,
		CMap_Suoh_Neon::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLB,
		CMap_Suoh_Neon::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLA");
	}
#pragma endregion


#pragma region UI
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND,
		CUI_HP_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_HP,
		CUI_HP::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE,
		CUI_Psyco_Wave::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPPROTOTYPE_GAMEOBJECT_UI_PSYCOWAVEE_GAMEOBJECT_UI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX,
		CUI_ItemTag::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG,
		CUI_ItemBox::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMTAG");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L,
		CUI_ItemHUD_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R,
		CUI_ItemHUD_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B,
		CUI_ItemHUD_B::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT,
		CUI_SkillSlot::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER,
		CUI_SkillChanger::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON,
		CUI_ChangerIcon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_CHANGERICON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L,
		CUI_SkillChangerKey_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R,
		CUI_SkillChangerKey_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT,
		CUI_SkillChanger_Light::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND,
		CUI_Drive_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE,
		CUI_DriveGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE,
		CUI_Drive_SideGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND,
		CUI_Menu_Black_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND,
		CUI_Menu_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER,
		CUI_Menu_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY,
		CUI_Menu_Category_Party::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM,
		CUI_Menu_Category_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP,
		CUI_Menu_Category_Equip::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP,
		CUI_Menu_Category_BrainMap::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS,
		CUI_Menu_Category_SAS::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY,
		CUI_Menu_Category_Story::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY,
		CUI_Menu_Category_Library::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION,
		CUI_Menu_Category_Option::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L,
		CUI_Menu_Arrow_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R,
		CUI_Menu_Arrow_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON,
		CUI_Menu_Button::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER,
		CUI_Menu_Profile_PartyLeader::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1,
		CUI_Menu_Profile_PartyMember1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2,
		CUI_Menu_Profile_PartyMember2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1,
		CUI_Menu_Profile_PartySupport1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2,
		CUI_Menu_Profile_PartySupport2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE,
		CUI_Menu_Seperate_Line::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER,
		CUI_Menu_Profile_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD,
		CUI_Menu_Profile_ID_Card::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER,
		CUI_Menu_Fade_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK,
		CUI_Indicator_ObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK,
		CUI_Indicator_SpcObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU,
		CUI_Menu_Models::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN,
		CUI_Indicator_Name_Town::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC,
		CUI_Indicator_Tooltip_Tutorial_PMCC::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH_BATTLEScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_LOCKON,
		CUI_Indicator_Monster_LockOn::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP,
		CUI_Indicator_Monster_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS,
		CUI_Indicator_Name_Boss::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS,
		CUI_Indicator_ToolTip_Tutorial_Tips::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK,
		CUI_Indicator_Monster_NoticeMark::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP,
		CUI_Indicator_Boss_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BRAINTALK_MIZUHA,
		CUI_Indicator_BrainTalk_Mizuha::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BRAINTALK_SUOH_BATTLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT,
		CUI_Indicator_Damage_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT,
		CUI_Indicator_Heal_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ELECTRIC,
		CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ELECTRIC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ACCELERATION,
		CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ACCELERATION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_REPLICATION,
		CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_REPLICATION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_STEALTH,
		CUI_Indicator_ToolTip_Mizuha_SAS_Stealth::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_STEALTH");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL,
		CUI_Indicator_InstKill::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL");
	}
#pragma endregion

#pragma endregion
	
	return S_OK;
}

