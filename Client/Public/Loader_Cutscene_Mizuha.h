#pragma once
#include "ClientPCH.h"
#include "Loader.h"
#include "GameInstance.h"
#include "VIBuffer_Cube.h"
#include "Camera_Main.h"
#include "SkyCube.h"
#include "Model.h"
#include "Keyframe.h"

#pragma region Character
#include "Kyoka.h"
#include "Naomi.h"
#include "Kasane.h"
#include "Haruka.h"
#include "Arashi.h"
#include "Gemma.h"
#include "Shiden.h"
#pragma endregion

#include "CameraKeyframeGenerator.h"
#include "ObjectKeyframeGenerator.h"
#include "DebugCamera.h"
#include "ModelRenderer.h"

#pragma region Cutscene
#include "CutsceneBuffer.h"
#include "CutsceneCamera.h"
#pragma endregion

#pragma region Map
#include "Map_NeonLine.h"
#include "Map_Stage1_Road.h"
#include "Map_Stage1_Static.h"
#include "Map_Stage1_Building.h"
#include "Map_Stage1_Dynamic.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "Map_Stage1_ETC.h"
#include "Map_Wall.h"
#pragma endregion 

#include "MenuCharacter.h"

HRESULT	CLoader::Load_Cutscene_Mizuha()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

#pragma region Prototype Texture
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_MASK_HORIZONTAL,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Mask/Mask_Horizontal.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_MASK_HORIZONTAL");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_MASK_VERTICAL,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Mask/Mask_Vertical.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_MASK_VERTICAL");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_NOISE_GRID,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Common/T_NoiseGrid.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_GRID");
	}
#pragma endregion
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER,
		CMenuCharacter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER");
	}
#pragma region Prototype Component
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_KYOKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kyoka/Kyoka_Default_Blend.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KYOKA");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NAOMI,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Naomi/Naomi_Blend.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NAOMI");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_KASANE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Player/Kasane_Cutscene.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KASANE");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_HARUKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Haruka/Haruka_Blend.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_HARUKA");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_SHIDEN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Shiden/Shiden.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHIDEN");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_ARASHI,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_GEMMA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Gemma/Gemma.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_GEMMA");
	}

	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_KAGERO,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Kagero/Kagero.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MizuhaScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_KAGERO");
	}

#ifdef _DEBUG
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_DEBUG_CINE_CAMERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/CinematicCamera/CinematicCamera.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_DEBUG_CINE_CAMERA");
	}
#endif

#pragma endregion

#pragma region Prototype GameObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CLOUD,
		CNoiseCloud::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CLOUD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA,
		CKyoka::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_NAOMI,
		CNaomi::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_NAOMI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE,
		CKasane::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_HARUKA,
		CHaruka::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_HARUKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI,
		CArashi::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_GEMMA,
		CGemma::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_GEMMA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN,
		CShiden::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CUTSCENE_BUFFER,
		CCutsceneBuffer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CUTSCENE_BUFFER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CUTSCENE_CAMERA,
		CCutsceneCamera::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CUTSCENE_CAMERA");
	}

#ifdef _DEBUG
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR,
		CCameraKeyframeGenerator::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_OBJECT_KEYFRAME_GENERATOR,
		CObjectKeyframeGenerator::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_GAMEOBJECT_OBJECT_KEYFRAME_GENERATOR");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_DEBUG_CINE_CAMERA,
		CDebugCamera::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: PROTOTYPE_GAMEOBJECT_DEBUG_CINE_CAMERA");
	}
#endif

#pragma endregion

	auto pModelRenderer = dynamic_pointer_cast<CModelRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER));

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Common_BaseWall.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL");
	}
#pragma region Prototype Model_Stage1Road
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAP, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Area_MiniMap.mdl")), 1);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_Dirt_Tile, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Dirt_Tile.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SandyOil_Tile, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SandyOil_Tile.mdl")), 1);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CIRCLE_TILE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Circle_Tile.mdl")), 1);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAPA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Area_MiniMapA.mdl")), 1);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraA.mdl")), 128);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraB.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraC.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraD.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraE.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAF, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraF.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAG, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraG.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAH, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraH.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAI, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraI.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAJ, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraJ.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAK, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraK.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAL, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePiraL.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FOOTHOLDD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FootholdD.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FOOTHOLDE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FootholdE.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FOOTHOLDF, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FootholdF.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FOOTHOLDSHAPEA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FootholdShapeA.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FOOTHOLDSHAPEB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FootholdShapeB.mdl")), 128);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FOOTHOLDG, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FootholdG.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_DIRT_GROUND, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Dirt_Ground.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BASEMENTA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BasementA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BASEMENTB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BasementB.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BASEMENTC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BasementC.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BASEMENTD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BasementD.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONBUILDINGC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConstructionBuildingC.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BRIDGE_ENKEIB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Bridge_EnkeiB.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BRIDGE_ENKEIC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Bridge_EnkeiC.mdl")), 8);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BASEMENTBENDA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Basement_BendA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BASEMENTE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BasementE.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLA, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallA.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLB, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallB.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLC, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallC.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallD.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallE.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLF, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallF.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLG,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallG.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLH,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallH.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSWALLI,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossWallI.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossBanisterA.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossBanisterB.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossBanisterC.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossBanisterD.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossBanisterE.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERF,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BossBanisterF.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONSLOPEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConstructionSlopeA.mdl")), 1);
#pragma endregion

#pragma region Prototype Model_Stage1Static
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FENCEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FenceA.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FENCEC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FenceC.mdl")), 256);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLF,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RealSteelWallF.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STEELWALL5A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelWall5A.mdl")), 64);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentA.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentC.mdl")), 8);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentE.mdl")), 8);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STEELWALL5B,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelWall5B.mdl")), 32);


	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STEELWALL5C,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelWall5C.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RealSteelWallA.mdl")), 128);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RealSteelWallB.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RealSteelWallC.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STEELWALL4A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelWall4A.mdl")), 64);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PREFABA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/PrefabA.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PREFABB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/PrefabB.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_ROCKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RockA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_ROCKB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RockB.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_ROCKC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RockC.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_RUBBLEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RubbleA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_RUBBLEB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RubbleB.mdl")), 16);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/OutsideFloorA.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/OutsideFloorB.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/OutsideFloorC.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/OutsideFloorD.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/OutsideFloorE.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORF,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/OutsideFloorF.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_NDF0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/NDF0.mdl")), 64);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PLYWOODA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/PlyWoodA.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PLYWOODB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/PlyWoodB.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BIOTOILET,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BioToilet.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_RUBBLEC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RubbleC.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_RUBBLED,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RubbleD.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEWALLA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcreteWallA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STEELPOLEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelPoleA.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWERCRANE_BASE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TowerCrane_Base.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FENCINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FencingA.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FENCINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FencingB.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FENCINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FencingC.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FENCINGD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FencingD.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FENCINGE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FencingE.mdl")), 64);	
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_ROADBLOCKB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RoadBlockB.mdl")), 64);
#pragma endregion

#pragma region Prototype Model_Stage1Dynamic
	auto pBarrelA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BarrelA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BARRELA, pBarrelA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_BARRELA, pBarrelA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BARRELA");
	}

	auto pBarrelB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BarrelB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BARRELB, pBarrelB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_BARRELB, pBarrelB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BARRELB");
	}

	auto pDumpster = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Dumpster.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_DUMPSTER, pDumpster, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_DUMPSTER, pDumpster)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_DUMPSTER");
	}

	auto pBoxA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BoxA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOXA, pBoxA, 64);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_BOXA, pBoxA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BOXA");
	}

	auto pBoxB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BoxB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOXB, pBoxB, 64);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_BOXB, pBoxB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BOXB");
	}

	auto pConcreteBagA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcreteBagA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGA, pConcreteBagA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGA, pConcreteBagA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGA");
	}

	auto pConcreteBagB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcreteBagB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGB, pConcreteBagB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGB, pConcreteBagB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGB");
	}

	auto pConcretePipeA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePipeA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEA, pConcretePipeA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEA, pConcretePipeA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEA");
	}

	auto pConcretePipeB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePipeB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEB, pConcretePipeB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEB, pConcretePipeB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEB");
	}

	auto pConcretePipeC = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePipeC.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEC, pConcretePipeC, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEC, pConcretePipeC)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEC");
	}

	auto pConcretePipeD = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePipeD.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED, pConcretePipeD, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED, pConcretePipeD)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED");
	}

	auto pSpoolA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SpoolA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SPOOLA, pSpoolA, 16);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_SPOOLA, pSpoolA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPOOLA");
	}

	auto pGroundCarC = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Ground_CarC.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_GROUND_CARC, pGroundCarC, 16);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_GROUND_CARC, pGroundCarC)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_GROUND_CARC");
	}

	auto pBrickPalette = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Brick_Palette.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BRICK_PALETTE, pBrickPalette, 16);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_BRICK_PALETTE, pBrickPalette)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BRICK_PALETTE");
	}

	auto pCableReel = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/CableReel.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CABLEREEL, pCableReel, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_CABLEREEL, pCableReel)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CABLEREEL");
	}

	auto pIceBox = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Icebox.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_ICEBOX, pIceBox, 16);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_ICEBOX, pIceBox)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CABLEREEL");
	}
#pragma endregion

#pragma region Prototype Model_Stage1_Special_Dynamic
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_SPECIAL_BEAMA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Special_BeamA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTG,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentG.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPOOLA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_SPECIAL_BEAMA_PIECE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BeamPiece.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPOOLA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Special_Rock0.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTH,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentH.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTH");
	}
#pragma endregion

#pragma region Prototype Model_Stage1Building
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STEELWALL3A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelWall3A.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STEELWALL3B,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelWall3B.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_STEELWALL4ANON,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SteelWall4ANon.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RealSteelWallD.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RealSteelWallE.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CITYBUILDING_PARTSA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/CityBuilding_PartsA.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CITYBUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/CityBuildingA.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONBUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConstructionBuildingA.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONBUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConstructionBuildingB.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CRANE_ENKEIA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Crane_EnkeiA.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_DISTANTBUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/DistantBuildingA.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_IRONPILLAR,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Ironpillar.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TARPA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TarpA.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TARPB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TarpB.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TARPC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TarpC.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PLYWOODC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/PlyWoodC.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PLYWOODD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/PlyWoodD.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_GROUND_COALA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Ground_CoalA.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CITYBUILDING_TOWER,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/CityBuilding_Tower.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/MizuhaBuildingA.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/MizuhaBuildingB.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/MizuhaBuildingC.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/MizuhaBuildingD.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/MizuhaBuildingE.mdl")), 4);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGF,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/MizuhaBuildingF.mdl")), 4);

#pragma endregion

#pragma region Prototype Model_Stage1_ETC
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_GROUND_GRASS,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Ground_Grass.mdl")), 64);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_ROADBLOCKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/RoadBlockA.mdl")), 32);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_GROUND_CONEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Ground_ConeA.mdl")), 66);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_GROUND_CONEB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Ground_ConeB.mdl")), 64);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TANA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TanA.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TANB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TanB.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TANC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TanC.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TAND,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TanD.mdl")), 8);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TANE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/TanE.mdl")), 8);

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONGATEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConstructionGateA.mdl")), 4);
#pragma endregion
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONELEVATOR,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConstructionElevator.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONELEVATOR");
	}
#pragma region Prototype Model_Stage1_LightObject
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_SPOTLIGHTA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SpotlightA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_FLOODLIGHTB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FloodlightB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_FLOODLIGHTB");
	}
#pragma endregion



#pragma region Prototype Model_NeonLine
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONCORNERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONCORNERB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONCORNERC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerC.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONCORNERD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerD.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONCRASHA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCrashA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCRASHA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONCRASHB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCrashB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCRASHB");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONLINEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonLineA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONLINEA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONLINEB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonLineB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONLINEB");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONROLLA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonRollA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONROLLA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONSWAYA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONSWAYB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_MODEL_NEONSWAYC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayC.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}

#pragma region Prototype Component_UI
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_UI_NAME_TOWN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Background1.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Fillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK_ROCK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Fillter.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_UI_INDICATOR_SPCOBJECTATTACK_ROCK");
	}
#pragma endregion
#pragma endregion

#pragma region Prototype GameObject_UI
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN,
		CUI_Indicator_Name_Town::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND,
		CUI_HP_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_HP,
		CUI_HP::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE,
		CUI_Psyco_Wave::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPPROTOTYPE_GAMEOBJECT_UI_PSYCOWAVEE_GAMEOBJECT_UI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX,
		CUI_ItemTag::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG,
		CUI_ItemBox::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMTAG");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L,
		CUI_ItemHUD_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R,
		CUI_ItemHUD_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B,
		CUI_ItemHUD_B::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT,
		CUI_SkillSlot::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER,
		CUI_SkillChanger::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON,
		CUI_ChangerIcon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_CHANGERICON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L,
		CUI_SkillChangerKey_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R,
		CUI_SkillChangerKey_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT,
		CUI_SkillChanger_Light::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND,
		CUI_Drive_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE,
		CUI_DriveGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE,
		CUI_Drive_SideGauge::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND,
		CUI_Menu_Black_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND,
		CUI_Menu_Background::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER,
		CUI_Menu_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY,
		CUI_Menu_Category_Party::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM,
		CUI_Menu_Category_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP,
		CUI_Menu_Category_Equip::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP,
		CUI_Menu_Category_BrainMap::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BrainMap");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS,
		CUI_Menu_Category_SAS::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY,
		CUI_Menu_Category_Story::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY,
		CUI_Menu_Category_Library::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION,
		CUI_Menu_Category_Option::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L,
		CUI_Menu_Arrow_L::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R,
		CUI_Menu_Arrow_R::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON,
		CUI_Menu_Button::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER,
		CUI_Menu_Profile_PartyLeader::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1,
		CUI_Menu_Profile_PartyMember1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2,
		CUI_Menu_Profile_PartyMember2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1,
		CUI_Menu_Profile_PartySupport1::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2,
		CUI_Menu_Profile_PartySupport2::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE,
		CUI_Menu_Seperate_Line::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER,
		CUI_Menu_Profile_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD,
		CUI_Menu_Profile_ID_Card::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER,
		CUI_Menu_Fade_Fillter::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK,
		CUI_Indicator_ObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_OBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK,
		CUI_Indicator_SpcObjectAttack::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU,
		CUI_Menu_Models::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC,
		CUI_Indicator_Tooltip_Tutorial_PMCC::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_CUTSCENE_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_LOCKON,
		CUI_Indicator_Monster_LockOn::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_LOCKON");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP,
		CUI_Indicator_Monster_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS,
		CUI_Indicator_Name_Boss::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS,
		CUI_Indicator_ToolTip_Tutorial_Tips::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK,
		CUI_Indicator_Monster_NoticeMark::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_MISSPLAYER,
		CUI_Indicator_Monster_MissPlayer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_MISSPLAYER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP,
		CUI_Indicator_Boss_Hp::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BRAINTALK_MIZUHA,
		CUI_Indicator_BrainTalk_Mizuha::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BRAINTALK_MIZUHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT,
		CUI_Indicator_Damage_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT,
		CUI_Indicator_Heal_Font::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ELECTRIC,
		CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ELECTRIC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ACCELERATION,
		CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ACCELERATION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_REPLICATION,
		CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_REPLICATION");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_STEALTH,
		CUI_Indicator_ToolTip_Mizuha_SAS_Stealth::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_STEALTH");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM,
		CUI_Indicator_SpcObjectAttack_Rotation_Beam::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROCK,
		CUI_Indicator_SpcObjectAttack_Rock::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROCK");
	}
#pragma endregion


	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL,
		CMap_Wall::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL");
	}


#pragma region Prototype GameObject_Map_Stage1_Road
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_Dirt_Tile,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_SandyOil_Tile,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CIRCLE_TILE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CIRCLE_TILE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAPA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAPA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAF,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAG,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAG");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAH,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAH");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAI,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAJ,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAJ");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAK,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDF,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDG,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_DIRT_GROUND,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_DIRT_GROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 29))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTBENDA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 30))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTBENDA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 31))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTBENDA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 32))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 33))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 34))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 35))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 36))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLF,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 37))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLG,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 38))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLH,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 39))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLI,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 40))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 41))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 42))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 43))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 44))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 45))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERF,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 46))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONSLOPEA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 47))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONSLOPEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 48))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIC");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Static

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FENCEA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FENCEB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FENCEC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLF,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5A,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTE,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5B,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5C,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4A,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_PREFABA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PREFABA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_PREFABB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PREFABB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_ROCKA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROCKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_ROCKB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROCKB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_ROCKC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROCKC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_RUBBLEA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_RUBBLEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_RUBBLEB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_RUBBLEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FENCINGA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FENCINGB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FENCINGC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORD,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 29))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORE,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 30))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORF,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 31))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NDF0,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 32))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_PLYWOODA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 33))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PLYWOODA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_PLYWOODB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 34))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PLYWOODB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BIOTOILET,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 35))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BIOTOILET");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_RUBBLEC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 36))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_RUBBLEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_RUBBLED,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 37))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_RUBBLED");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEWALLA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 38))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEWALLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_STEELPOLEA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 39))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELPOLEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TOWERCRANE_BASE,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 40))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWERCRANE_BASE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FENCINGD,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 41))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FENCINGE,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 42))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGE");
	}	
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 43))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKB");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Dynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BARRELA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BARRELA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BARRELB,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BARRELB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_DUMPSTER,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_DUMPSTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOXA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOXA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BOXB,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOXB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGB,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEB,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEC,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPED,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPED");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_SPOOLA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOOLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_BRICK_PALETTE,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BRICK_PALETTE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CABLEREEL,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CABLEREEL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_ICEBOX,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ICEBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_GROUND_CARC,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_CARC");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Dynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTG,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOOLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTH,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOOLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA_PIECE,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Building
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLD,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLE,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4ANON,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3A,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3B,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_PARTSA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_PARTSA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDINGA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGB,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CRANE_ENKEIA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CRANE_ENKEIA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_DISTANTBUILDINGA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_DISTANTBUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_IRONPILLAR,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_IRONPILLAR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TARPA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TARPA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TARPB,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TARPB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TARPC,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TARPC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_PLYWOODC,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PLYWOODC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_PLYWOODD,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PLYWOODD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_TOWER,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGB,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGC,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGD,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGE,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGF,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_ETC

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_GROUND_GRASS,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_GRASS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKA,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEA,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEB,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TANA,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TANA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TANB,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TANB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TANC,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TANC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TAND,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TAND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_TANE,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TANE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONGATEA,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONGATEA");
	}
#pragma endregion


#pragma region Prototype GameObject_Map_NeonLine
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERC,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERD,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC");
	}
#pragma endregion
#pragma region Prototype GameObject_Map_Stage1_LightObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA,
		CMap_Stage1_LightObject::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_MAP_FLOODLIGHTB,
		CMap_Stage1_LightObject::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FLOODLIGHTB");
	}
#pragma endregion
	

	return S_OK;
}
