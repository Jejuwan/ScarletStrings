#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "VIBuffer_Cube.h"
#include "Camera_Main.h"
#include "Camera_Player.h"
#include "SkyCube.h"
#include "TestCube.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "PlayerTentacle.h"
#include "TestSphere.h"
#include "TestAlphaBuffer.h"

#include "TestCar.h"
#include "TestBarrigate.h"

#include "ModelRenderer.h"
#include "BrainFieldTentacle.h"
#include "RedLaserEye.h"

#pragma region UI
#include "UI_HP_Background.h"
#include "UI_HP.h"
#include "UI_ItemBox.h"
#include "UI_ItemTag.h"
#include "UI_ItemHUD_L.h"
#include "UI_ItemHUD_R.h"
#include "UI_ItemHUD_B.h"
#include "UI_SkillSlot.h"
#include "UI_SkillChanger.h"
#include "UI_ChangerIcon.h" 
#include "UI_SkillChangerKey_L.h"
#include "UI_SkillChangerKey_R.h"
#include "UI_SkillChanger_Light.h"
#include "UI_Psyco_Wave.h"
#include "UI_Drive_Background.h"
#include "UI_DriveGauge.h"
#include "UI_Drive_SideGauge.h"
#include "UI_Menu_Black_Background.h"
#include "UI_Menu_Background.h"
#include "UI_Menu_Fillter.h"
#include "UI_Menu_Category_Party.h"
#include "UI_Menu_Category_Item.h"
#include "UI_Menu_Category_Equip.h"
#include "UI_Menu_Category_BrainMap.h"
#include "UI_Menu_Category_SAS.h"
#include "UI_Menu_Category_Story.h"
#include "UI_Menu_Category_Library.h"
#include "UI_Menu_Category_Option.h"
#include "UI_Menu_Arrow_L.h"
#include "UI_Menu_Arrow_R.h"
#include "UI_Menu_Button.h"
#include "UI_Menu_Profile_PartyLeader.h"
#include "UI_Menu_Profile_PartyMember1.h"
#include "UI_Menu_Profile_PartyMember2.h"
#include "UI_Menu_Profile_PartySupport1.h"
#include "UI_Menu_Profile_PartySupport2.h"
#include "UI_Menu_Seperate_Line.h"
#include "UI_Menu_Profile_Fillter.h"
#include "UI_Menu_Profile_Id_Card.h"
#include "UI_Indicator_ObjectAttack.h"
#include "UI_Indicator_SpcObjectAttack.h"
#include "UI_Indicator_ToolTip_Tutorial_PMCC.h"
#include "UI_Menu_Fade_Fillter.h"
#include "UI_Menu_Models.h"
#include "UI_Indicator_Name_Town.h"
#include "UI_Indicator_Name_Boss.h"
#include "UI_Indicator_Monster_MissPlayer.h"


#pragma endregion
#pragma region Monster
#include "Monster_TutoBoss.h"
#include "Monster_FlowerChest.h"
#include "Monster_FlowerHead.h"
#include "Monster_Fly.h"
#include "Monster_Buffalo.h"
#include "Monster_Snail.h"
#include "Monster_Twince.h"
#include "Monster_Karen.h"
#include "Part.h"
#include "Monster_Projectile.h"

#include "WaterDrop.h"
#pragma endregion

#pragma region VFX
#include "EffectRegular_Visual.h"
#include "EffectRegular_Distortion.h"
#include "EffectInstance_ParticlePoint.h"
#pragma endregion

#pragma region Map
#include "Map_Tutorial_Dynamic.h"
#include "Map_MindRoom_Road.h"
#include "Map_Wall.h"
#pragma endregion 

#pragma region Skill
#include "Psychokinesis.h"
#pragma endregion

#pragma region Character
#include "Arashi.h"
#include "Shiden.h"
#include "Kyoka.h"
#include "Kagero.h"
#pragma endregion
#include "HitCrackDecal.h"
#include "Monster_FallenObject.h"
#include "CameraKeyframeGenerator.h"
#include "DebugCamera.h"
#include "CutsceneTentacle.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "Monster_Karen_Ice.h"
#include "IceDecal.h"

HRESULT CLoader::Load_RealTestScene()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK,
		CHitCrackDecal::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CAMERA_CUTSCENE,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MindRoom", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

#ifdef _DEBUG
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR,
		CCameraKeyframeGenerator::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MindRoom", "Failed to Add_Component_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_DEBUG_CINE_CAMERA,
		CDebugCamera::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MindRoom", "Failed to Add_Component_Prototype: PROTOTYPE_GAMEOBJECT_DEBUG_CINE_CAMERA");
	}
#endif // _DEBUG

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_PLAYER_CUTSCENE_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacleB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_CUTSCENE_TENTACLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PLAYER_CUTSCENE_TENTACLE,
		CCutsceneTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_CUTSCENE_TENTACLE");
	}

#pragma region Prototype Component

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_TESTCUBE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Cubemap/T_Cubemap.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TESTCUBE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Background2.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Psyco_Wave/PsycoWave.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemBox.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemTag.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_MISSPLAYER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Miss_M1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_MISSPLAYER");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_L");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_PURPLE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/green.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_PURPLE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_BLACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_BLACK");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_PLAYER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/player.mdl"), g_mUnit, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Weapon/wp200.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacle.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_PLAYER_BRAINFIELD_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacleA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_BRAINFIELD_TENTACLE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_TEST_ALPHA,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Temp/vfx_muzzle_flash_01_cannon_fiery_front_di_ma.dds"), 1, false))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TEST_ALPHA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_B,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_B.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_B");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLSLOT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillSlot.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillSlot");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_CHANGERICON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/ChangerIcon.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ChangerIcon");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER_LIGHT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger_Light.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger_Light");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_Drive_Background");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Gauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_SideGauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BLACK_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/UI_MenuBackground.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BRAINMAP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Seperate_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_ID_CARD,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MENU,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Menu/Menu_Sofa.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_G.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_TOWN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Background1.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BOSS_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Boss_Hp_Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BOSS_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_DAMAGE_FONT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/0.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_HEAL_FONT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/0.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_BOSS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/BossName_Background1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_BOSS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_INSTKILL,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_INSTKILL");
	}


	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_ARASHI,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_SHIDEN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Shiden/Shiden.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_KYOKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kyoka/Kyoka_Default_Blend.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_KAGERO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kagero/Kagero.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KAGERO");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_KASANE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kasane/Kasane.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KASANE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN_ICE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Ice2.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_KAREN_ICE,
		CMonster_Karen_Ice::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_DECAL_ICE,
		CIceDecal::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_DECAL_ICE");
	}
#pragma endregion

	auto pModelRenderer = dynamic_pointer_cast<CModelRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER));

	auto pConcreteA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/ConcreteBarrierA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA, pConcreteA, 128);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA, pConcreteA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_SPECIAL_BEAMA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Special_BeamA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED");
	}
#pragma region Prototype GameObject

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI,
		CArashi::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN,
		CShiden::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA,
		CKyoka::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO,
		CKagero::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE,
		CKasane::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_TESTSPHERE,
		CTestSphere::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TESTSPHERE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PLAYER,
		CPlayer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS,
		CPsychokinesis::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON,
		CPlayerWeapon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE,
		CPlayerTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PLAYER_BRAINFIELD_TENTACLE,
		CBrainFieldTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_BRAINFIELD_TENTACLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PLAYER_RED_LASER_EYE,
		CRedLaserEye::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_RED_LASER_EYE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER,
		CCamera_Player::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_TESTCUBE,
		CTestCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TESTCUBE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK,
		CUI_Indicator_Monster_NoticeMark::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP,
		CUI_Indicator_Monster_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND,
		CUI_HP_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_HP,
		CUI_HP::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE,
		CUI_Psyco_Wave::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPPROTOTYPE_GAMEOBJECT_UI_PSYCOWAVEE_GAMEOBJECT_UI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX,
		CUI_ItemTag::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG,
		CUI_ItemBox::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMTAG");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L,
		CUI_ItemHUD_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R,
		CUI_ItemHUD_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B,
		CUI_ItemHUD_B::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT,
		CUI_SkillSlot::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER,
		CUI_SkillChanger::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON,
		CUI_ChangerIcon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_CHANGERICON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L,
		CUI_SkillChangerKey_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R,
		CUI_SkillChangerKey_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT,
		CUI_SkillChanger_Light::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_TEST_ALPHA,
		CTestAlphaBuffer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TEST_ALPHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND,
		CUI_Drive_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE,
		CUI_DriveGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE,
		CUI_Drive_SideGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND,
		CUI_Menu_Black_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND,
		CUI_Menu_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER,
		CUI_Menu_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY,
		CUI_Menu_Category_Party::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM,
		CUI_Menu_Category_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP,
		CUI_Menu_Category_Equip::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP,
		CUI_Menu_Category_BrainMap::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS,
		CUI_Menu_Category_SAS::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY,
		CUI_Menu_Category_Story::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY,
		CUI_Menu_Category_Library::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION,
		CUI_Menu_Category_Option::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L,
		CUI_Menu_Arrow_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R,
		CUI_Menu_Arrow_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON,
		CUI_Menu_Button::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER,
		CUI_Menu_Profile_PartyLeader::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1,
		CUI_Menu_Profile_PartyMember1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2,
		CUI_Menu_Profile_PartyMember2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1,
		CUI_Menu_Profile_PartySupport1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2,
		CUI_Menu_Profile_PartySupport2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE,
		CUI_Menu_Seperate_Line::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER,
		CUI_Menu_Profile_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD,
		CUI_Menu_Profile_ID_Card::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER,
		CUI_Menu_Fade_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK,
		CUI_Indicator_ObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK,
		CUI_Indicator_SpcObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU,
		CUI_Menu_Models::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN,
		CUI_Indicator_Name_Town::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC,
		CUI_Indicator_Tooltip_Tutorial_PMCC::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP,
		CUI_Indicator_Boss_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT,
		CUI_Indicator_Damage_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT,
		CUI_Indicator_Heal_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS,
		CUI_Indicator_Name_Boss::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL,
		CUI_Indicator_InstKill::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL");
	}
#pragma endregion
#pragma region VFX

#pragma endregion

//#pragma region Test Env
//	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_TEST_CAR_A,
//		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/Test/CarA.mdl")))))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TEST_CAR_A");
//	}
//
//	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_TEST_BARRIGATE,
//		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/Test/Barrigate.mdl")))))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TEST_BARRIGATE");
//	}
//#pragma endregion


#pragma region Monster
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERCHEST,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/FlowerChest.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERCHEST");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_FLY,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Fly.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLY");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Bullet.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Weak01.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/TutoBoss.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/FlowerHead.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_SNAIL,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Snail.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_SNAIL");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_BUFFALO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Buffalo.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_BUFFALO");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_TWINCE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Twince.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TWINCE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_TWINCEBOX,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/TwinceBox.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TWINCEBOX");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Treverce.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_PART_WEAK,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PART_TWINCEBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST,
		CMonster_FlowerChest::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLY,
		CMonster_Fly::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_FLY");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_TUTOBOSS,
		CMonster_TutoBoss::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD,
		CMonster_FlowerHead::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL,
		CMonster_Snail::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE,
		CMonster_Twince::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_KAREN,
		CMonster_Karen::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE");
	}
	
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_PART_TWINCEBOX,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_TWINCEBOX))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_TWINCEBOX");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_BUFFALO,
		CMonster_Buffalo::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_BUFFALO");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_PROJECTILE,
		CMonster_Projectile::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PROJECTILE");
	}

	//if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_WEAK,
	//	CMonster_Weak::Create(m_pDevice, m_pContext))))
	//{
	//	MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_WEAK");
	//}
	
#pragma endregion

#pragma region Prototype GameObject_Map
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, L"Test_FallingObject",
		CMonster_FallenObject::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Mindroom/mdl/Mindroom_CrateA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA");
	}
#pragma endregion

//if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_VIBUFFER_MESH_SPHERE,
//	CMesh::Read(m_pDevice, m_pContext, TEXT("Bin/Resources/Mesh/Effect/Sphere.msh")))))
//{
//	MSG_RETURN(E_FAIL, "CLoader::Load_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_MESH_SPHERE");
//}
if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PROJECTILE_WATERDROP,
	CWaterDrop::Create(m_pDevice, m_pContext))))
{
	MSG_RETURN(E_FAIL, "CLoader::Load_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PROJECTILE_WATERDROP");
}
if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::REALTEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_DISPENPERRY,
	CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/DispenPerry.mdl")))))
{
	MSG_RETURN(E_FAIL, "CLoader::Load_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
}
if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MONSTER_DISPENPERRY,
	CMonster_DispenPerry::Create(m_pDevice, m_pContext))))
{
	MSG_RETURN(E_FAIL, "CLoader::Load_MizuhaScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO");
}


#pragma endregion


if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_MISSPLAYER,
	CUI_Indicator_Monster_MissPlayer::Create(m_pDevice, m_pContext))))
{
	MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_MISSPLAYER");
}

if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA,
	CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 0))))
{
	MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA");
}

	return S_OK;
}
