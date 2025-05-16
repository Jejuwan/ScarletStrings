#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "VIBuffer_Cube.h"
#include "Camera_Main.h"
#include "SkyCube.h"
#include "NoiseCloud.h"

#pragma region Map
#include "Map_Wall.h"
#include "Map_MindRoom_Road.h"
#include "Map_MindRoom_Static.h"
#include "Map_MindRoom_Neon.h"
#include "Map_MindRoom_Dynamic.h"
#pragma endregion 

#pragma region Teammate
#include "Arashi_Teammate.h"
#include "Arashi_Weapon.h"

#include "Shiden_Teammate.h"
#include "Shiden_Weapon.h"
#pragma endregion

#include "Kyoka.h"
#include "Kagero.h"
#include "Arashi.h"
#include "Shiden.h"
#include "HitCrackDecal.h"

#include "Monster_Statue.h"
#include "Monster_FallenObject.h"
#include "Monster_Karen.h"

#include "IceDecal.h"
#include "DecalTargetBuffer.h"

#include "CutsceneTentacle.h"
#include "BrainFieldTentacle.h"
#include "RedLaserEye.h"

#ifdef _DEBUG
#include "TestModel.h"

#include "CameraKeyframeGenerator.h"
#include "DebugCamera.h"
#endif

#include "MenuCharacter.h"
#include "Monster_Karen_Ice.h"


inline HRESULT CLoader::Load_MindRoomScene()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

#pragma region Shaders

#pragma endregion

#pragma region Textures
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_014,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ef_scl_noi_014.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MindRoomScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_014");
	}

#pragma endregion

#pragma region GameObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER,
		CPlayer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS,
		CPsychokinesis::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON,
		CPlayerWeapon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE,
		CPlayerTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER_BRAINFIELD_TENTACLE,
		CBrainFieldTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_BRAINFIELD_TENTACLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER_RED_LASER_EYE,
		CRedLaserEye::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_RED_LASER_EYE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER,
		CCamera_Player::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK,
		CHitCrackDecal::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI,
		CArashi::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN,
		CShiden::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA,
		CKyoka::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO,
		CKagero::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_DECAL_ICE,
		CIceDecal::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_DECAL_ICE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_DECAL_TARGET_BUFFER,
		CDecalTargetBuffer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_DECAL_TARGET_BUFFER");
	}

#pragma endregion

#pragma region MODEL
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_KASANE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kasane/Kasane.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KASANE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_ARASHI,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_SHIDEN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Shiden/Shiden.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHIDEN");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_KYOKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kyoka/Kyoka_Default_Blend.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_KAGERO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kagero/Kagero.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KAGERO");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_PLAYER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/player.mdl"), g_mUnit, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Weapon/wp200.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacle.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_PLAYER_BRAINFIELD_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacleA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_BRAINFIELD_TENTACLE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Statue.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Treverce.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN_ICE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Ice2.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_KAREN_ICE,
		CMonster_Karen_Ice::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_STATUE,
		CMonster_Statue::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_KAREN,
		CMonster_Karen::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Bullet.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_PROJECTILE,
		CMonster_Projectile::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PROJECTILE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Weak01.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_PART_WEAK,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PART_TWINCEBOX");
	}

	//if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_WEAK,
	//	CMonster_Weak::Create(m_pDevice, m_pContext))))
	//{
	//	MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_WEAK");
	//}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_FLOWER,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Statue_Flower.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_FLOWER,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_FLOWER))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_FLOWER");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK1,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Mask1.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK1,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK1");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK2,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Mask2.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK2,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK2");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK3,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Mask3.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK3,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_MASK3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK3");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_FAN,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Statue_Fan.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_FAN,
		CPart::Create(m_pDevice, m_pContext, PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE_FAN))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MONSTER_PART_TWINCEBOX");
	}


	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP,
		CUI_Indicator_Monster_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS,
		CUI_Indicator_Name_Boss::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS");
	}
	//if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS,
	//	CUI_Indicator_Tooltip_Tutorial_Tips::Create(m_pDevice, m_pContext))))
	//{
	//	MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS");
	//}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK,
		CUI_Indicator_Monster_NoticeMark::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP,
		CUI_Indicator_Boss_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT,
		CUI_Indicator_Damage_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT,
		CUI_Indicator_Heal_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL,
		CUI_Indicator_InstKill::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_SPECIAL_BEAMA_PIECE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BeamPiece.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPOOLA");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Special_Rock0.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0");
	}

#ifdef _DEBUG
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, TEXT("TestModel"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/shadowtest.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, TEXT("TestModel"),
		CTestModel::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR,
		CCameraKeyframeGenerator::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MindRoom", "Failed to Add_Component_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_DEBUG_CINE_CAMERA,
		CDebugCamera::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MindRoom", "Failed to Add_Component_Prototype: PROTOTYPE_GAMEOBJECT_DEBUG_CINE_CAMERA");
	}
#endif
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER,
		CMenuCharacter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER");
	}
#pragma endregion

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CAMERA_CUTSCENE,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MindRoom", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

#pragma region Teammate

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_ARASHI_TEAMMATE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi_Teammate.mdl"), g_mUnit, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI_TEAMMATE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_ARASHI_WEAPON,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi_Weapon.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI_WEAPON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI_TEAMMATE,
		CArashi_Teammate::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI_TEAMMATE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_ARASHI_WEAPON,
		CArashi_Weapon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_ARASHI_WEAPON");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_SHIDEN_TEAMMATE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Shiden/Shiden_Teammate.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHIDEN_TEAMMATE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_SHIDEN_WEAPON,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Shiden/Shiden_Weapon.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHIDEN_WEAPON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN_TEAMMATE,
		CShiden_Teammate::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN_TEAMMATE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_SHIDEN_WEAPON,
		CShiden_Weapon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SHIDEN_WEAPON");
	}
#pragma endregion

#pragma region Cutscene
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_PLAYER_CUTSCENE_TENTACLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/Tentacle/tentacleB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PLAYER_CUTSCENE_TENTACLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER_CUTSCENE_TENTACLE,
		CCutsceneTentacle::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_PLAYER_CUTSCENE_TENTACLE");
	}
#pragma endregion

#pragma region Monster

#pragma endregion

	auto pModelRenderer = dynamic_pointer_cast<CModelRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER));

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Common_BaseWall.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL");
	}

#pragma region Prototype Model_Road

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_FUTA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Sankaku_futa.mdl")), 1)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_FUTA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_WATER,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/MindRoom_Water.mdl")), 1)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_WATER");
	}
#pragma endregion

#pragma region Prototype Model_Static
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Sankaku_A.mdl")), 256)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Mindroom_StoneA.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Mindroom_StoneB.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Mindroom_StoneC.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_SPACESHIP,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Mindroom_Spaceship.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEA");
	}
#pragma endregion

#pragma region Prototype Model_ETC
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_TENKYU,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/MindRoom_Tenkyu.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_TENKYU");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_FLOOR,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/MindRoom_Floor.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_FLOOR");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_TWIST,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/MindRoom_Twist.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_FLOOR");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_REDLINE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/MindRoom_Redline.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_REDLINE");
	}
#pragma endregion

#pragma region Prototype Model_Dynamic

	auto pBox = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Mindroom/mdl/Mindroom_Box.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX, pBox, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX, pBox)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BARRELA");
	}

	auto pCrateA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Mindroom/mdl/Mindroom_CrateA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA, pCrateA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA, pCrateA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA");
	}

	auto pCrateB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Mindroom/mdl/Mindroom_CrateB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB, pCrateB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB, pCrateB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB");
	}

	pBox = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Grill_Throw.mdl"));
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MINDROOM_GRILL, pBox)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_GRILL");
	}

	pBox = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/long.mdl"));
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MINDROOM_LONG, pBox)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_GRILL");
	}
#pragma endregion


#pragma region Prototype Model_Dynamic
	/*auto pCarA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CarA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CARA, pCarA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_SUOH_CARA, pCarA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARA");
	}*/
#pragma endregion

#pragma region UI
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Background2.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_HP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Psyco_Wave/PsycoWave.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_PSYCO_WAVE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemBox.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemTag.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMTAG");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_ITEMHUD_B,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_B.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ItemHUD_B");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLSLOT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillSlot.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillSlot");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_CHANGERICON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/ChangerIcon.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_ChangerIcon");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGERKEY_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChangerKey_R");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_SKILLCHANGER_LIGHT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger_Light.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_SkillChanger_Light");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_Drive_Background");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Gauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_SideGauge.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BLACK_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/UI_MenuBackground.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BRAINMAP,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_CATEGORY_OPTION");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_L.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_R,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/1.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOn.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Seperate_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_SEPERATE_LINE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_ID_CARD,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_MODEL_MENU,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Menu/Menu_Sofa.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_R.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_G.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_TOWN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Background1.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Fillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK_ROCK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Fillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK_ROCK");
	}
#pragma endregion

#pragma region Prototype GameObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE");
	}
	//if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_SKYCUBE_BRAINFIELD,
	//	CSkyCube_BrainField::Create(m_pDevice, m_pContext))))
	//{
	//	MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE_BRAINFIELD");
	//}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CLOUD,
		CNoiseCloud::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CLOUD");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Road
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_FUTA,
		CMap_MindRoom_Road::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_FUTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_WATER,
		CMap_MindRoom_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_WATER");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Static
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_A,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEA,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEB,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEC,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SPACESHIP,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SPACESHIP");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Neon
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TENKYU,
		CMap_MindRoom_Neon::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TENKYU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_FLOOR,
		CMap_MindRoom_Neon::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TENKYU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TWIST,
		CMap_MindRoom_Neon::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TWIST");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_REDLINE,
		CMap_MindRoom_Neon::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_REDLINE");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Dynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX,
		CMap_MindRoom_Dynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA,
		CMap_MindRoom_Dynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB,
		CMap_MindRoom_Dynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA_PIECE,
		CMonster_FallenObject::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0,
		CMonster_FallenObject::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0_COPY,
		CMonster_FallenObject::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_LONG,
		CMonster_FallenObject::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0");
	}
#pragma endregion

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL,
		CMap_Wall::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL");
	}
#pragma region Prototype GameObject_Map_Suoh_ETC

#pragma endregion

#pragma region UI
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND,
		CUI_HP_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_HP,
		CUI_HP::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE,
		CUI_Psyco_Wave::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPPROTOTYPE_GAMEOBJECT_UI_PSYCOWAVEE_GAMEOBJECT_UI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX,
		CUI_ItemTag::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG,
		CUI_ItemBox::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMTAG");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L,
		CUI_ItemHUD_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R,
		CUI_ItemHUD_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B,
		CUI_ItemHUD_B::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT,
		CUI_SkillSlot::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER,
		CUI_SkillChanger::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON,
		CUI_ChangerIcon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_CHANGERICON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L,
		CUI_SkillChangerKey_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R,
		CUI_SkillChangerKey_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT,
		CUI_SkillChanger_Light::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND,
		CUI_Drive_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE,
		CUI_DriveGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE,
		CUI_Drive_SideGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND,
		CUI_Menu_Black_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND,
		CUI_Menu_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER,
		CUI_Menu_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY,
		CUI_Menu_Category_Party::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM,
		CUI_Menu_Category_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP,
		CUI_Menu_Category_Equip::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP,
		CUI_Menu_Category_BrainMap::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS,
		CUI_Menu_Category_SAS::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY,
		CUI_Menu_Category_Story::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY,
		CUI_Menu_Category_Library::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION,
		CUI_Menu_Category_Option::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L,
		CUI_Menu_Arrow_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R,
		CUI_Menu_Arrow_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON,
		CUI_Menu_Button::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER,
		CUI_Menu_Profile_PartyLeader::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1,
		CUI_Menu_Profile_PartyMember1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2,
		CUI_Menu_Profile_PartyMember2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1,
		CUI_Menu_Profile_PartySupport1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2,
		CUI_Menu_Profile_PartySupport2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE,
		CUI_Menu_Seperate_Line::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER,
		CUI_Menu_Profile_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD,
		CUI_Menu_Profile_ID_Card::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER,
		CUI_Menu_Fade_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK,
		CUI_Indicator_ObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK,
		CUI_Indicator_SpcObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU,
		CUI_Menu_Models::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN,
		CUI_Indicator_Name_Town::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC,
		CUI_Indicator_Tooltip_Tutorial_PMCC::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOHScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_DAMAGE_FONT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/0.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_HEAL_FONT,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/0.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_INSTKILL,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_Line.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_INSTKILL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM,
		CUI_Indicator_SpcObjectAttack_Rotation_Beam::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROCK,
		CUI_Indicator_SpcObjectAttack_Rock::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROCK");
	}
#pragma endregion

	return S_OK;
}