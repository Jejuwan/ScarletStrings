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
#include "WaterDrop.h"
#include "Part.h"

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
#include "UI_Menu_Fade_Fillter.h"
#include "UI_Menu_Models.h"
#include "UI_Indicator_Name_Town.h"
#include "UI_Indicator_ToolTip_Tutorial_PMCC.h"
#include "UI_Indicator_ToolTip_Tutorial_LockOn.h"
#include "UI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle.h"
#include "UI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack.h"
#include "UI_Indicator_ToolTip_Tutorial_Success.h"
#include "UI_Indicator_ToolTip_Tutorial_Tips.h"
#include "UI_Indicator_Monster_LockOn.h"
#include "UI_Indicator_Name_Boss.h"
#include "UI_Indicator_Monster_Hp.h"
#include "UI_Indicator_Monster_NoticeMark.h"

#include "UI_Indicator_Boss_Hp.h"
#include "UI_Indicator_Cursor.h"
#include "UI_Indicator_BrainTalk_Tutorial.h"
#include "UI_Indicator_Damage_Font.h"
#include "UI_Indicator_Heal_Font.h"
#include "UI_Indicator_Script.h"
#include "UI_Indicator_InstKill.h"


#include "MenuCharacter.h"
#pragma endregion



#pragma region Monster
#include "Monster_TutoBoss.h"
#include "Monster_FlowerChest.h"
#include "Monster_FlowerHead.h"
#include "Monster_Fly.h"
#include "Monster_Buffalo.h"
#include "Monster_Snail.h"
#include "Monster_Twince.h"
#include "Monster_Projectile.h"
#include "Monster_Weak.h"
#pragma endregion

#pragma region VFX
#include "EffectRegular_Visual.h"
#include "EffectRegular_Distortion.h"
#include "EffectInstance_ParticlePoint.h"
#pragma endregion

#pragma region Map
#include "Map_Road.h"
#include "Map_TestRoad.h"
#include "Map_Tutorial_Dynamic.h"
#include "Map_Tutorial_Static.h"
#include "Map_Plant.h"
#include "Map_Stage1_Road.h"
#include "Map_Stage1_Static.h"

#include "BattleFieldWall.h"
#include "BattleField.h"
#include "EventCollider.h"
#pragma endregion 

#pragma region Skill
#include "Psychokinesis.h"
#pragma endregion

#pragma region Character
#include "Arashi.h"
#include "Shiden.h"
#include "Kyoka.h"
#include "Kagero.h"
#include "Kasane.h"
#pragma endregion

#include "HitCrackDecal.h"

HRESULT CLoader::Load_TestScene()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

#pragma region Prototype Component

	// Monster RMA
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_EM0400_BODY_RMA,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/FlowerChest/T_em0400_body_RMA.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_EM0400_BODY_RMA");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_TESTCUBE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Cubemap/T_Cubemap.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TESTCUBE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Background2.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Psyco_Wave/PsycoWave.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemBox.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemTag.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_L.png")))))
   	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_L");
	}
  
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_PURPLE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Purple1px.png"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_PURPLE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_BLACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_BLACK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_PART_WEAK,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PART_TWINCEBOX");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_PLAYER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/player.mdl"), g_mUnit, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Weapon/wp200.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacle.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_TEST_ALPHA,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Temp/vfx_muzzle_flash_01_cannon_fiery_front_di_ma.dds"), 1, false))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TEST_ALPHA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_B,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_B.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_B");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLSLOT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillSlot.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillSlot");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_CHANGERICON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/ChangerIcon.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ChangerIcon");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER_LIGHT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger_Light.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger_Light");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_Drive_Background");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Gauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_SideGauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BLACK_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/UI_MenuBackground.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BRAINMAP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Seperate_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_ID_CARD,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MENU,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Menu/Menu_Sofa.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_G.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_LOCKON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_KASANE_BATTLESTYLE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_KASANE_BATTLESTYLE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_KASANE_SPECIALATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_KASANE_SPECIALATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_SUCCESS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background_Success.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_SUCCESS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_TOWN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Background1.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_NOTICEMARK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Notice_M.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_LOCKON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/LockOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Monster_HpBackground.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_MONSTER_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_BOSS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/BossName_Background1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_BOSS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_TIPS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Tips_background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_TIPS");
	}


	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BOSS_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Boss_Hp_Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BOSS_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_CURSOR,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Cursor_1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_CURSOR");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BRAINTALK_TUTORIAL,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_Frame.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_BRAINTALK_TUTORIAL");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_DAMAGE_FONT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/0.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_HEAL_FONT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/0.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_PROJECTILE_WATERDROP,
		CWaterDrop::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PROJECTILE_WATERDROP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SCRIPT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Script/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SCRIPT");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_INSTKILL,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_INSTKILL");
	}


	auto pModelRenderer = dynamic_pointer_cast<CModelRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER));

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD0, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_Road0.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD1, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_Road1.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD2, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_Road2.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD3, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_Road3.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD4, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_Road4.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_Road5.mdl")), 16);
	
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_SAKURA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_Sakura.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD0, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad0.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD1, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad1.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD2, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad2.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD3, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad3.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD4, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad4.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD5, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad5.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD6, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad6.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD7, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad7.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD8, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad8.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD9, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoad9.mdl")), 16);
	
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROADA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoadA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROADB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoadB.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROADC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_EnterRoadC.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROADWALL, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RoadWall.mdl")), 16);
	
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_LOGSA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/LogsA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_LOGSB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/LogsB.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_LOGSC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/LogsC.mdl")), 16);
	
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SAKURA_BRAKA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Sakura_BrakA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SAKURA_BRAKB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Sakura_BrakB.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SAKURA_BRANCHA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Sakura_BranchA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SAKURA_BRANCHB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Sakura_BranchB.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SAKURA_BRANCHC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Sakura_BranchC.mdl")), 16);
	
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STONEA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/StoneA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PILLAR, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Pillar.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HUGETREE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/HugeTree.mdl")), 16);
	
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootB.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootC.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootD.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootE.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTF, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootF.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTG, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootG.mdl")), 16);
	
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HUGEDARKTREE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/HugeDarkTree.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootB.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootC.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootD.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Tutorial_RootE.mdl")), 16);

	// Dynamic
	auto pCarA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/CarA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CARA, pCarA, 8);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_CARA, pCarA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CARA");
	}

	auto pCarB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/CarB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CARB, pCarB, 8);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_CARB, pCarB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CARB");
	}

	auto pCarC = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/CarC.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CARC, pCarC, 8);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_CARC, pCarC)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CARC");
	}

	auto pCarD = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/CarD.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CARD, pCarD, 8);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_CARD, pCarD)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CARD");
	}

	auto pConcreteA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/ConcreteBarrierA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA, pConcreteA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA, pConcreteA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA");
	}

	auto pConcreteB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/ConcreteBarrierB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERB, pConcreteB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERB, pConcreteB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERB");
	}

	// Static
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCKA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/TruckA.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCK_CAMPINGCAR, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Truck_CampingCar.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCK_CONTAINER, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Truck_Container.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCK_GAS, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Truck_Gas.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCK_GASBRK, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Truck_GasBrk.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BUSA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BusA.mdl")), 8);

	// Plant
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_GRASS, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Grass.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechA.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechB.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechC.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechD.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechE.mdl")), 8);

	// BattleField
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BATTLEFIELD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/BattleField/BattleField.mdl")), 128);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_DEBUG_CUBE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/DEBUG_OBJECT/DEBUG_CUBE.mdl")), 16);

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAP,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Area_MiniMap.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAPA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Area_MiniMapA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAPA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_Dirt_Tile,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Dirt_Tile.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_Dirt_Tile");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_FENCEC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FenceC.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_FENCEA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLF,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RealSteelWallF.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLF");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_STEELWALL5B,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelWall5B.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_STEELWALL5B");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentC.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTC");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONGATEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConstructionGateA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONGATEA");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_ARASHI,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_SHIDEN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Shiden/Shiden.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_KYOKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kyoka/Kyoka_Default_Blend.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_KAGERO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kagero/Kagero.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KAGERO");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_KASANE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kasane/Kasane.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KASANE");
	}
#pragma endregion
#pragma region Prototype GameObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_BATTLEFIELD_WALL,
		CBattleFieldWall::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_BATTLEFIELD_WALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_BATTLEFIELD,
		CBattleField::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_BATTLEFIELD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER,
		CEventCollider::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI,
		CArashi::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN,
		CShiden::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA,
		CKyoka::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO,
		CKagero::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE,
		CKasane::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK,
		CHitCrackDecal::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_TESTSPHERE,
		CTestSphere::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TESTSPHERE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_PLAYER,
		CPlayer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS,
		CPsychokinesis::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON,
		CPlayerWeapon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE,
		CPlayerTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER,
		CCamera_Player::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_TESTCUBE,
		CTestCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TESTCUBE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND,
		CUI_HP_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_HP,
		CUI_HP::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE,
		CUI_Psyco_Wave::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPPROTOTYPE_GAMEOBJECT_UI_PSYCOWAVEE_GAMEOBJECT_UI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX,
		CUI_ItemTag::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG,
		CUI_ItemBox::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMTAG");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L,
		CUI_ItemHUD_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R,
		CUI_ItemHUD_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B,
		CUI_ItemHUD_B::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT,
		CUI_SkillSlot::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER,
		CUI_SkillChanger::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON,
		CUI_ChangerIcon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_CHANGERICON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L,
		CUI_SkillChangerKey_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R,
		CUI_SkillChangerKey_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT,
		CUI_SkillChanger_Light::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_TEST_ALPHA,
		CTestAlphaBuffer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TEST_ALPHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND,
		CUI_Drive_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE,
		CUI_DriveGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE,
		CUI_Drive_SideGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND,
		CUI_Menu_Black_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND,
		CUI_Menu_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER,
		CUI_Menu_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY,
		CUI_Menu_Category_Party::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM,
		CUI_Menu_Category_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP,
		CUI_Menu_Category_Equip::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP,
		CUI_Menu_Category_BrainMap::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS,
		CUI_Menu_Category_SAS::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY,
		CUI_Menu_Category_Story::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY,
		CUI_Menu_Category_Library::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION,
		CUI_Menu_Category_Option::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L,
		CUI_Menu_Arrow_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R,
		CUI_Menu_Arrow_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON,
		CUI_Menu_Button::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER,
		CUI_Menu_Profile_PartyLeader::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1,
		CUI_Menu_Profile_PartyMember1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2,
		CUI_Menu_Profile_PartyMember2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1,
		CUI_Menu_Profile_PartySupport1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2,
		CUI_Menu_Profile_PartySupport2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE,
		CUI_Menu_Seperate_Line::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER,
		CUI_Menu_Profile_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD,
		CUI_Menu_Profile_ID_Card::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER,
		CUI_Menu_Fade_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK,
		CUI_Indicator_ObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK,
		CUI_Indicator_SpcObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU,
		CUI_Menu_Models::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN,
		CUI_Indicator_Name_Town::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC,
		CUI_Indicator_Tooltip_Tutorial_PMCC::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_LOCKON,
		CUI_Indicator_ToolTip_Tutorial_LockOn::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_KASANE_BATTLESTYLE,
		CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_KASANE_BATTLESTYLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_KASANE_SPECIALATTACK,
		CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_KASANE_SPECIALATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SUCCESS,
		CUI_Indicator_ToolTip_Tutorial_Success::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SUCCESS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_LOCKON,
		CUI_Indicator_Monster_LockOn::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP,
		CUI_Indicator_Monster_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS,
		CUI_Indicator_Name_Boss::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS,
		CUI_Indicator_ToolTip_Tutorial_Tips::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK,
		CUI_Indicator_Monster_NoticeMark::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER,
		CMenuCharacter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP,
		CUI_Indicator_Boss_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_CURSOR,
		CUI_Indicator_Cursor::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_CURSOR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BRAINTALK_TUTORIAL,
		CUI_Indicator_BrainTalk_Tutorial::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BRAINTALK_TUTORIAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT,
		CUI_Indicator_Damage_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT,
		CUI_Indicator_Heal_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SCRIPT,
		CUI_Indicator_Script::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SCRIPT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL,
		CUI_Indicator_InstKill::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL");
	}
#pragma endregion
#pragma region VFX

#pragma endregion

#pragma region Test Env
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_TEST_CAR_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/Test/CarA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TEST_CAR_A");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_TEST_BARRIGATE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/Test/Barrigate.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TEST_BARRIGATE");
	}
#pragma endregion


#pragma region Monster
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERCHEST,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/FlowerChest.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERCHEST");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_FLY,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Fly.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLY");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Bullet.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE");
	}
	
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Weak01.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/TutoBoss.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/FlowerHead.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_SNAIL,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Snail.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD");
	}
	
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_BUFFALO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Buffalo.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD");
	}

	/*
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_MONSTER_TWINCE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Twince.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD");
	}
	*/

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST,
		CMonster_FlowerChest::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLY,
		CMonster_Fly::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_FLY");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_TUTOBOSS,
		CMonster_TutoBoss::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD,
		CMonster_FlowerHead::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL,
		CMonster_Snail::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL");
	}
	/*
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE,
		CMonster_Twince::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE");
	}
	*/

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_BUFFALO,
		CMonster_Buffalo::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_BUFFALO");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_PROJECTILE,
		CMonster_Projectile::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PROJECTILE");
	}

	//if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_WEAK,
	//	CMonster_Weak::Create(m_pDevice, m_pContext))))
	//{
	//	MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_WEAK");
	//}
#pragma endregion

#pragma region Prototype GameObject_Map
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD0,
		CMap_Road::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROAD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD1,
		CMap_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD2,
		CMap_Road::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD3,
		CMap_Road::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD4,
		CMap_Road::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD4");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD5,
		CMap_Road::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD5");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_SAKURA,
		CMap_Road::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_SAKURA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD0,
		CMap_Road::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD0");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD1,
		CMap_Road::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD1");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD2,
		CMap_Road::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD2");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD3,
		CMap_Road::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD3");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD4,
		CMap_Road::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD4");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD5,
		CMap_Road::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD5");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD6,
		CMap_Road::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD6");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD7,
		CMap_Road::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD7");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD8,
		CMap_Road::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD8");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD9,
		CMap_Road::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD9");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADA,
		CMap_Road::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADB,
		CMap_Road::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADC,
		CMap_Road::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROADWALL,
		CMap_Road::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADC");
	}
	
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_LOGSA,
		CMap_Road::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_LOGSA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_LOGSB,
		CMap_Road::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_LOGSB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_LOGSC,
		CMap_Road::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_LOGSC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKA,
		CMap_Road::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKB,
		CMap_Road::Create(m_pDevice, m_pContext, 29))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHA,
		CMap_Road::Create(m_pDevice, m_pContext, 30))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHB,
		CMap_Road::Create(m_pDevice, m_pContext, 31))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHC,
		CMap_Road::Create(m_pDevice, m_pContext, 32))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_STONEA,
		CMap_Road::Create(m_pDevice, m_pContext, 33))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STONEA");
	}
	
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_PILLAR,
		CMap_Road::Create(m_pDevice, m_pContext, 35))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_HUGETREE,
		CMap_Road::Create(m_pDevice, m_pContext, 36))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTA,
		CMap_Road::Create(m_pDevice, m_pContext, 37))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTB,
		CMap_Road::Create(m_pDevice, m_pContext, 38))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTC,
		CMap_Road::Create(m_pDevice, m_pContext, 39))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTD,
		CMap_Road::Create(m_pDevice, m_pContext, 40))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTE,
		CMap_Road::Create(m_pDevice, m_pContext, 41))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTF,
		CMap_Road::Create(m_pDevice, m_pContext, 42))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTG,
		CMap_Road::Create(m_pDevice, m_pContext, 43))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTG");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_HUGEDARKTREE,
		CMap_Road::Create(m_pDevice, m_pContext, 44))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HUGEDARKTREE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTA,
		CMap_Road::Create(m_pDevice, m_pContext, 45))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTB,
		CMap_Road::Create(m_pDevice, m_pContext, 46))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTC,
		CMap_Road::Create(m_pDevice, m_pContext, 47))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTD,
		CMap_Road::Create(m_pDevice, m_pContext, 48))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTE,
		CMap_Road::Create(m_pDevice, m_pContext, 49))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTE");
	}


	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_CARA,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_CARB,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_CARC,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_CARD,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_CARD_Brk,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARD_Brk");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERB,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR_BRK,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR_BRK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TRACTOR,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRACTOR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TRUCKA,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CAMPINGCAR,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CAMPINGCAR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CONTAINER,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CONTAINER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GAS,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GAS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GASBRK,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GASBRK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_BUSA,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BUSA");
	}


	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_GRASS,
		CMap_Plant::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GRASS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_BEECHA,
		CMap_Plant::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_BEECHB,
		CMap_Plant::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_BEECHC,
		CMap_Plant::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_BEECHD,
		CMap_Plant::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_BEECHE,
		CMap_Plant::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_Dirt_Tile,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAPA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_FENCEC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLF,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5A,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONGATEA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONGATEA");
	}
#pragma endregion
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TESTROAD,
		CMap_TestRoad::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TEST");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_TEST_CAR,
		CTestCar::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TEST_CAR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_TEST_BARRIGATE,
		CTestBarrigate::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TEST_BARRIGATE");
	}

	return S_OK;
}