#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "Camera_Main.h"
#include "GlobalGizmo.h"
#include "SkyCube.h"
#include "VIBufferInstance_Rect.h"
#include "Camera_MapTool.h"
#include "Player.h"

#include "BattleFieldWall.h"
#include "EventCollider.h"

#pragma region Map
/* Common */
#include "Map_NeonLine.h"
#include "Map_Wall.h"
#include "Map_Item.h"
#include "Map_ShopKeeper.h"

/* Tutorial */
#include "Map_Road.h"
#include "Map_Tutorial_Dynamic.h"
#include "Map_Tutorial_Static.h"
#include "Map_Plant.h"

/* Mizuha */
#include "Map_Stage1_Road.h"
#include "Map_Stage1_Static.h"
#include "Map_Stage1_Building.h"
#include "Map_Stage1_Dynamic.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "Map_Stage1_ETC.h"
#include "Map_Stage1_Elevator.h"
#include "Map_Stage1_LightObject.h"

/* Suoh */
#include "Map_Suoh_Road.h"
#include "Map_Suoh_Building.h"
#include "Map_Suoh_Static.h"
#include "Map_Suoh_Signboard.h"
#include "Map_Suoh_ETC.h"
#include "Map_Suoh_Dynamic.h"
#include "Map_Suoh_Child.h"
#include "Map_Suoh_Man.h"
#include "Map_Suoh_Woman.h"
#include "Map_Suoh_Neon.h"
#include "Map_Suoh_Bus.h"

/* MindRoom */
#include "Map_MindRoom_Road.h"
#include "Map_MindRoom_Static.h"
#include "Map_MindRoom_Neon.h"
#include "Map_MindRoom_Dynamic.h"

/* Hideout */
#include "Map_Hideout_Static.h"
#include "Map_Hideout_Arashi.h"

#pragma endregion 

#if ACTIVATE_TOOL
HRESULT		CLoader::Load_MapToolScene()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	auto pModelRenderer = dynamic_pointer_cast<CModelRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER));

#pragma region BattleField
	// BattleField
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BATTLEFIELD, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/BattleField/BattleField.mdl")), 128);
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_BATTLEFIELD_WALL,
		CBattleFieldWall::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_BATTLEFIELD");
	}

	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_DEBUG_CUBE, CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/DEBUG_OBJECT/DEBUG_CUBE.mdl")), 16);
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER,
		CEventCollider::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER");
	}

#pragma endregion

#pragma region Prototype Model_NeonLine
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONCORNERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONCORNERB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONCORNERC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerC.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONCORNERD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCornerD.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCORNERA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONCRASHA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCrashA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCRASHA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONCRASHB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonCrashB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONCRASHB");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONLINEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonLineA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONLINEA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONLINEB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonLineB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONLINEB");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONROLLA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonRollA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONROLLA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONSWAYA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONSWAYB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayB.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_NEONSWAYC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Neon/NeonSwayC.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_NEONSWAYA");
	}

#pragma endregion

#pragma region Prototype Model_TutorialRoad
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
#pragma endregion

#pragma region Prototype Model_Tutorial_Dynamic
	auto pTutoCarA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/CarA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CARA, pTutoCarA, 8);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CARA, pTutoCarA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MapToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CARA");
	}

	auto pTutoCarB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/CarB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CARB, pTutoCarB, 8);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CARB, pTutoCarB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MapToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CARB");
	}

	auto pTutoCarC = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/CarC.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CARC, pTutoCarC, 8);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CARC, pTutoCarC)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MapToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CARC");
	}

	auto pTutoCarD = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/CarD.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CARD, pTutoCarD, 8);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CARD, pTutoCarD)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MapToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CARD");
	}

	auto pConcreteA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/ConcreteBarrierA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA, pConcreteA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA, pConcreteA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA");
	}

	auto pConcreteB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/ConcreteBarrierB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERB, pConcreteB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERB, pConcreteB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MapToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERB");
	}
#pragma endregion

#pragma region Prototype Model_Tutorial_Static
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCKA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/TruckA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCK_CAMPINGCAR, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Truck_CampingCar.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCK_CONTAINER, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Truck_Container.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCK_GAS, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Truck_Gas.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TRUCK_GASBRK, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Truck_GasBrk.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BUSA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BusA.mdl")), 16);
#pragma endregion

#pragma region Prototype Model_Plant
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_GRASS, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/Grass.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHA, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechA.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHB, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechB.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHC, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechC.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHD, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechD.mdl")), 16);
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BEECHE, 
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Tutorial/mdl/BeechE.mdl")), 16);

#pragma endregion

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
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_BARRELA, pBarrelA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BARRELA");
	}

	auto pBarrelB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BarrelB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BARRELB, pBarrelB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_BARRELB, pBarrelB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BARRELB");
	}

	auto pDumpster = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Dumpster.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_DUMPSTER, pDumpster, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_DUMPSTER, pDumpster)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_DUMPSTER");
	}

	auto pBoxA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BoxA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOXA, pBoxA, 64);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_BOXA, pBoxA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BOXA");
	}

	auto pBoxB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/BoxB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BOXB, pBoxB, 64);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_BOXB, pBoxB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BOXB");
	}

	auto pConcreteBagA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcreteBagA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGA, pConcreteBagA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGA, pConcreteBagA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGA");
	}

	auto pConcreteBagB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcreteBagB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGB, pConcreteBagB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGB, pConcreteBagB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGB");
	}

	auto pConcretePipeA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePipeA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEA, pConcretePipeA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEA, pConcretePipeA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEA");
	}

	auto pConcretePipeB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePipeB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEB, pConcretePipeB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEB, pConcretePipeB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEB");
	}

	auto pConcretePipeC = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePipeC.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEC, pConcretePipeC, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEC, pConcretePipeC)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEC");
	}

	auto pConcretePipeD = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConcretePipeD.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED, pConcretePipeD, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED, pConcretePipeD)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED");
	}

	auto pSpoolA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SpoolA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SPOOLA, pSpoolA, 16);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SPOOLA, pSpoolA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPOOLA");
	}



	auto pGroundCarC = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Ground_CarC.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_GROUND_CARC, pGroundCarC, 16);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_GROUND_CARC, pGroundCarC)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_GROUND_CARC");
	}

	auto pBrickPalette = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Brick_Palette.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_BRICK_PALETTE, pBrickPalette, 16);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_BRICK_PALETTE, pBrickPalette)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BRICK_PALETTE");
	}

	auto pCableReel = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/CableReel.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_CABLEREEL, pCableReel, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CABLEREEL, pCableReel)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CABLEREEL");
	}

	auto pIceBox = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Icebox.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_ICEBOX, pIceBox, 16);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_ICEBOX, pIceBox)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CABLEREEL");
	}
#pragma endregion

#pragma region Prototype Model_Stage1_Special_Dynamic
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SPECIAL_BEAMA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Special_BeamA.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTG,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentG.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPOOLA");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/Special_Rock0.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTH,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/HeavyEquipmentH.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTH");
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

#pragma region Prototype Model_Stage1_Elevator
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONELEVATOR,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Stage1/mdl/ConstructionElevator.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONELEVATOR");
	}
#pragma endregion

#pragma region Prototype Model_Stage1_LightObject
	auto pSpotLightA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/SpotlightA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SPOTLIGHTA, pSpotLightA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SPOTLIGHTA, pSpotLightA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SPOTLIGHTA");
	}

	auto pFloodLightB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Stage1/mdl/FloodlightB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_FLOODLIGHTB, pFloodLightB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_FLOODLIGHTB, pFloodLightB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_FLOODLIGHTB");
	}
#pragma endregion

#pragma region Prototype Model_Suoh_Road

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_MINIMAP,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_MiniMap.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_MINIMAP");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_GROUND_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_Ground_A.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_GROUND_A");
	}



	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_PEDESTRIANBRIDGEA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/PedestrianBridgeA.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_PEDESTRIANBRIDGEA");
	}*/

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
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_RoadB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_ROADB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_TOWN_ROADC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_RoadC.mdl")), 32)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_ROADC");
	}
#pragma endregion

#pragma region Prototype Model_Suoh_Static
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
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Town_RoadA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_TOWN_ROADA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_SHUTTERA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_ShutterA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_SHUTTERA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_FlowerPot.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_KEIJIBAN,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Keijiban.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_KEIJIBAN");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_SEKIHI,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Sekihi.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_SEKIHI");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_OMIKUJIB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_OmikujiB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_OMIKUJIB");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_ARCHA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_ArchA.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_ARCHA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUMERAGI_KAIDANA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Sumeragi_KaidanA.mdl")), 4)))
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

	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MAITREYATRII,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/MaitreyaTrii.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MAITREYATRII");
	}*/
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MAITREYATRII,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Battle_MaitreyaTrii.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MAITREYATRII");
	}


	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CameraA.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAA");
	}


	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CameraD.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAD");
	}*/
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

#pragma region Prototype Model_Suoh_Building
	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SG_ACADEMYA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/SG_AcademyA.mdl")), 2)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SG_ACADEMYA");
	}*/
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


	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BRIDGE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Bridge.mdl")), 2)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BRIDGE");
	}*/
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


	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_B_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_B_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_B_BUILDINGA");
	}*/
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_B_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_B_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_B_BUILDINGA");
	}


	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_C_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGA");
	}*/
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


	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_C_BuildingD.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_C_BUILDINGD");
	}*/
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


	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_D_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGB");
	}*/
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGB,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_D_BuildingB.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGB");
	}



	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_D_BuildingC.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGC");
	}*/
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGC,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_D_BuildingC.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGC");
	}



	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_D_BuildingD.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGD");
	}*/
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_Battle_D_BuildingD.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_D_BUILDINGD");
	}




	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_E_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_E_BUILDINGA");
	}*/
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



	/*if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_G_BUILDINGA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_G_BuildingA.mdl")), 8)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_G_BUILDINGA");
	}*/
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
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_F0.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_F0");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G1,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_G1.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G1");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G2,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_G2.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_Suoh_Billboard_G2");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_H0,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_H0.mdl")), 64)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_H0");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_H1,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/Signboard_mdl/Suoh_Billboard_H1.mdl")), 64)))
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
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SUOH_CARA, pCarA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARA");
	}
	auto pCarB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CarB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CARB, pCarB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SUOH_CARB, pCarB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARB");
	}
	auto pCarC = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CarC.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CARC, pCarC, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SUOH_CARC, pCarC)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARC");
	}
	auto pCarD = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_CarD.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_SUOH_CARD, pCarD, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SUOH_CARD, pCarD)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_CARD");
	}

#pragma endregion

#pragma region Prototype Model_Suoh_NPC
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_CHILD,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_NPC_Child.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_CHILD");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_MAN,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Suoh_NPC_Man.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_MAN");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_WOMAN,
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

#pragma region Prototype Model_MindRoom_Road
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_FUTA,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Sankaku_futa.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_FUTA");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_WATER,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/MindRoom_Water.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_WATER");
	}
#pragma endregion

#pragma region Prototype Model_MindRoom_Static

	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/Sankaku_A.mdl")), 256)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_WATER");
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

#pragma region Prototype Model_MindRoom_Neon

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
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_TWIST");
	}	
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_REDLINE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/MindRoom/mdl/MindRoom_Redline.mdl")), 16)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_REDLINE");
	}
#pragma endregion

#pragma region Prototype Model_Mindroom_Dynamic
	auto pBox = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Mindroom/mdl/Mindroom_Box.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX, pBox, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX, pBox)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BARRELA");
	}

	auto pCrateA = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Mindroom/mdl/Mindroom_CrateA.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA, pCrateA, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA, pCrateA)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA");
	}

	auto pCrateB = CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Mindroom/mdl/Mindroom_CrateB.mdl"));
	pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB, pCrateB, 32);
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB, pCrateB)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MIZUHAScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB");
	}
#pragma endregion

#pragma region Prototype Model_Hideout_Static
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
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_HIDEOUT_PERFECTBODYPHOTO");
	}
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_HIDEOUT_TABLE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Hideout/mdl/Hideout_Table.mdl")), 4)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_HIDEOUT_TABLE");
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
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_ARASHI_TEAMMATE,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Arashi/Arashi_Teammate.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SUOH", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_ARASHI_TEAMMATE");
	}
#pragma region Prototype Model_Common
	if (FAILED(pModelRenderer->Create_Model_Instance(PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Map/Suoh/mdl/Common_BaseWall.mdl")), 128)))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_COMMON_BASEWALL");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_COMMON_ITEM,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Common/mdl/Common_Item.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_COMMON_ITEM");
	}
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_MODEL_SHOPKEEPER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Map/Common/mdl/Shopkeeper.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_SuohScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_SHOPKEEPER");
	}
#pragma endregion

#pragma region Prototype VIBuffer

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MAPTOOL, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT,
		CVIBufferInstance_Rect::Create(m_pDevice, m_pContext, 10000))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT");
	}

#pragma endregion

#pragma region Prototype GameObject

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_CAMERA_MAPTOOL,
		CCamera_MapTool::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAPTOOL");
	}
	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_GLOBALGIZMO,
		CGlobalGizmo::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_GLOBALGIZMO");
	}
	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYCUBE");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_NeonLine
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERC,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERD,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC,
		CMap_NeonLine::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Road

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD0,
		CMap_Road::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROAD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD1,
		CMap_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD2,
		CMap_Road::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD3,
		CMap_Road::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD4,
		CMap_Road::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD4");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD5,
		CMap_Road::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD5");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_SAKURA,
		CMap_Road::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_SAKURA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD0,
		CMap_Road::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD0");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD1,
		CMap_Road::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD1");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD2,
		CMap_Road::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD2");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD3,
		CMap_Road::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD3");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD4,
		CMap_Road::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD4");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD5,
		CMap_Road::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD5");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD6,
		CMap_Road::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD6");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD7,
		CMap_Road::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD7");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD8,
		CMap_Road::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD8");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD9,
		CMap_Road::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROAD9");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADA,
		CMap_Road::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADB,
		CMap_Road::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADC,
		CMap_Road::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROADWALL,
		CMap_Road::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ENTERROADC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_LOGSA,
		CMap_Road::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_LOGSA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_LOGSB,
		CMap_Road::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_LOGSB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_LOGSC,
		CMap_Road::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_LOGSC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKA,
		CMap_Road::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKB,
		CMap_Road::Create(m_pDevice, m_pContext, 29))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRAKB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHA,
		CMap_Road::Create(m_pDevice, m_pContext, 30))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHB,
		CMap_Road::Create(m_pDevice, m_pContext, 31))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHC,
		CMap_Road::Create(m_pDevice, m_pContext, 32))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SAKURA_BRANCHC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STONEA,
		CMap_Road::Create(m_pDevice, m_pContext, 33))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STONEA");
	}
	
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_PILLAR,
		CMap_Road::Create(m_pDevice, m_pContext, 35))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HUGETREE,
		CMap_Road::Create(m_pDevice, m_pContext, 36))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTA,
		CMap_Road::Create(m_pDevice, m_pContext, 37))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTB,
		CMap_Road::Create(m_pDevice, m_pContext, 38))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTC,
		CMap_Road::Create(m_pDevice, m_pContext, 39))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTD,
		CMap_Road::Create(m_pDevice, m_pContext, 40))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTE,
		CMap_Road::Create(m_pDevice, m_pContext, 41))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTF,
		CMap_Road::Create(m_pDevice, m_pContext, 42))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTG,
		CMap_Road::Create(m_pDevice, m_pContext, 43))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_ROOTG");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HUGEDARKTREE,
		CMap_Road::Create(m_pDevice, m_pContext, 44))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HUGEDARKTREE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTA,
		CMap_Road::Create(m_pDevice, m_pContext, 45))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTB,
		CMap_Road::Create(m_pDevice, m_pContext, 46))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTC,
		CMap_Road::Create(m_pDevice, m_pContext, 47))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTD,
		CMap_Road::Create(m_pDevice, m_pContext, 48))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTE,
		CMap_Road::Create(m_pDevice, m_pContext, 49))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TUTORIAL_DARKROOTE");
	}

#pragma endregion

#pragma region Prototype Model_Tutorial_Dynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CARA,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CARB,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CARC,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CARD,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CARD_Brk,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CARD_Brk");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERB,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR_BRK,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR_BRK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TRACTOR,
		CMap_Tutorial_Dynamic::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRACTOR");
	}

#pragma endregion

#pragma region Prototype Model_Tutorial_Static
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TRUCKA,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CAMPINGCAR,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CAMPINGCAR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CONTAINER,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCK_CONTAINER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GAS,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GAS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GASBRK,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TRUCK_GASBRK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BUSA,
		CMap_Tutorial_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BUSA");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Plant
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_GRASS,
		CMap_Plant::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GRASS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BEECHA,
		CMap_Plant::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BEECHB,
		CMap_Plant::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BEECHC,
		CMap_Plant::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BEECHD,
		CMap_Plant::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BEECHE,
		CMap_Plant::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BEECHE");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Road
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_Dirt_Tile,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SandyOil_Tile,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CIRCLE_TILE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CIRCLE_TILE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAPA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAPA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAF,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAG,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAG");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAH,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAH");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAI,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAJ,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAJ");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAK,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAK");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDF,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDG,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_DIRT_GROUND,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_DIRT_GROUND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 29))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTBENDA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 30))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTBENDA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BASEMENTE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 31))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BASEMENTBENDA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 32))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 33))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 34))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 35))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 36))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLF,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 37))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLG,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 38))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLH,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 39))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLI,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 40))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 41))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERB,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 42))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 43))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERD,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 44))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERE,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 45))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERF,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 46))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONSLOPEA,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 47))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONSLOPEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIC,
		CMap_Stage1_Road::Create(m_pDevice, m_pContext, 48))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIC");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Static

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FENCEA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FENCEB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FENCEC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLF,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5A,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTE,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5B,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5C,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4A,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4A");
	}
	
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_PREFABA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PREFABA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_PREFABB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PREFABB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_ROCKA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROCKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_ROCKB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROCKB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_ROCKC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROCKC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_RUBBLEA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_RUBBLEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_RUBBLEB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_RUBBLEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FENCINGA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FENCINGB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FENCINGC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORD,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 29))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORE,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 30))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORF,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 31))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_NDF0,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 32))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_PLYWOODA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 33))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PLYWOODA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_PLYWOODB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 34))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PLYWOODB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BIOTOILET,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 35))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BIOTOILET");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_RUBBLEC,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 36))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_RUBBLEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_RUBBLED,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 37))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_RUBBLED");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEWALLA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 38))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEWALLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STEELPOLEA,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 39))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELPOLEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWERCRANE_BASE,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 40))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWERCRANE_BASE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FENCINGD,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 41))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FENCINGE,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 42))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FENCINGE");
	}	
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKB,
		CMap_Stage1_Static::Create(m_pDevice, m_pContext, 43))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKB");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Dynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BARRELA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BARRELA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BARRELB,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BARRELB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_DUMPSTER,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_DUMPSTER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOXA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOXA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BOXB,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BOXB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGB,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEB,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEC,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPED,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPED");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SPOOLA,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOOLA");
	}


	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_BRICK_PALETTE,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_BRICK_PALETTE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CABLEREEL,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CABLEREEL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_ICEBOX,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ICEBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_GROUND_CARC,
		CMap_Stage1_Dynamic::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_CARC");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_SpecialDynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTG,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOOLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTH,
		CMap_Stage1_SpecialDynamic::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOOLA");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Building
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLD,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLE,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4ANON,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3A,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3B,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_PARTSA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_PARTSA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDINGA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGB,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CRANE_ENKEIA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CRANE_ENKEIA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_DISTANTBUILDINGA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_DISTANTBUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_IRONPILLAR,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_IRONPILLAR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TARPA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TARPA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TARPB,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TARPB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TARPC,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TARPC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_PLYWOODC,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PLYWOODC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_PLYWOODD,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PLYWOODD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_TOWER,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGA,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGB,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGC,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGD,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGE,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGF,
		CMap_Stage1_Building::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_ETC

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_GROUND_GRASS,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_GRASS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKA,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEA,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEB,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TANA,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TANA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TANB,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TANB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TANC,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TANC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TAND,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TAND");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TANE,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TANE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONGATEA,
		CMap_Stage1_ETC::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONGATEA");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_Elevator

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONELEVATOR,
		CMap_Stage1_Elevator::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONELEVATOR");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Stage1_LightObject
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA,
		CMap_Stage1_LightObject::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SPOTLIGHTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_FLOODLIGHTB,
		CMap_Stage1_LightObject::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_FLOODLIGHTB");
	}
#pragma endregion


#pragma region Prototype GameObject_Map_Suoh_Road
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_MINIMAP,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_MINIMAP");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_GROUND_A,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_GROUND_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_PEDESTRIANBRIDGEA,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_PEDESTRIANBRIDGEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_DODAI,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_DODAI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_TILE,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_TILE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADB,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADC,
		CMap_Suoh_Road::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADC");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_Static

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLB,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLC,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLD,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_WALLD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_ROADA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_FLOWERPOT,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_FLOWERPOT");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_KEIJIBAN,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_KEIJIBAN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SEKIHI,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SEKIHI");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_OMIKUJIB,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_OMIKUJIB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_ARCHA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_ARCHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUMERAGI_KAIDANA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUMERAGI_KAIDANA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_RUBBLEA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_RUBBLEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_PLANTSA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_PLANTSA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_TREEA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_TREEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_SUMERAGI_TOUROU,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SUMERAGI_TOUROU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MAITREYATRII,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MAITREYATRII");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAD,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAMERAD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HYDRANTA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HYDRANTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TELEPHONEBOXA,
		CMap_Suoh_Static::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TELEPHONEBOXA");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_Building

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_ACADEMYA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_ACADEMYA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_TOWER,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_TOWER");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BRIDGE,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_SHUTTERBASE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SG_BUILDINGC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_A_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_B_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_B_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGD,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_C_BUILDINGD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_Suoh_D_BuildingC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGD,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_D_BUILDINGD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGC");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGD,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGD");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGE,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGE");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGF,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_E_BUILDINGF");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_F_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGA,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGA");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGB,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGB");
	}

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGC,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_G_BUILDINGC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAFEBUILDING,
		CMap_Suoh_Building::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CAFEBUILDING");
	}


#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_Signboard

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A1,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A2,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A3,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_A3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_C0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_C0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_D0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_D0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I1,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I2,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I3,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_I3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_J0,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_J0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_B,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_C,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEA_C");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEB_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEB_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_B,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_C,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_C");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_D,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_GUIDEC_D");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_B,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_B");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_C,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 22))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNA_C");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNB_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 23))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNB_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGND_D,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 24))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGND_D");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNE,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 25))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SIGNE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_A,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 26))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_B,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 27))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_C,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 28))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_C");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_D,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 29))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_D");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_E,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 30))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_E");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_F,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 31))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_F");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_G,
		CMap_Suoh_Signboard::Create(m_pDevice, m_pContext, 32))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARDX_G");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_ETC
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEA,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEB,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEC,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERA,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERB,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_SHOPBOX,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_SHOPBOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B0,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B1,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B2,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B3,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B3");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_F0,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_F0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G1,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G2,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G2");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H0,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H0");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H1,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H1");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXA,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXB,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXC,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERA,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERB,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERC,
		CMap_Suoh_ETC::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERC");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_Dynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARA,
		CMap_Suoh_Dynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARB,
		CMap_Suoh_Dynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARC,
		CMap_Suoh_Dynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARD,
		CMap_Suoh_Dynamic::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARD");
	}


#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_NPC
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_CHILD,
		CMap_Suoh_Child::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_CHILD");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_MAN,
		CMap_Suoh_Man::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_MAN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_WOMAN,
		CMap_Suoh_Woman::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_NPC_WOMAN");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Suoh_Neon
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLA,
		CMap_Suoh_Neon::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLB,
		CMap_Suoh_Neon::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SUOH_BATTLE_WARNINGWALLA");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Mindroom_Road
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_FUTA,
		CMap_MindRoom_Road::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_FUTA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_WATER,
		CMap_MindRoom_Road::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_WATER");
	}

#pragma endregion

#pragma region Prototype GameObject_Map_Mindroom_Static
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_A,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SANKAKU_A");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEA,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEB,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEC,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_STONEC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SPACESHIP,
		CMap_MindRoom_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_SPACESHIP");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Mindroom_Neon
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TENKYU,
		CMap_MindRoom_Neon::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TENKYU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_FLOOR,
		CMap_MindRoom_Neon::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TENKYU");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TWIST,
		CMap_MindRoom_Neon::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TWIST");
	}	
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_REDLINE,
		CMap_MindRoom_Neon::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_TWIST");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Mindroom_Dynamic
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX,
		CMap_MindRoom_Dynamic::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA,
		CMap_MindRoom_Dynamic::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB,
		CMap_MindRoom_Dynamic::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB");
	}
#pragma endregion

#pragma region Prototype GameObject_Map_Hideout_Static

	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FLOOR,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FLOOR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_CARPET,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 1))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_CARPET");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_COLE,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 2))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_COLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FIREPLACE,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 3))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FIREPLACE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KITCHEN,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 4))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KITCHEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_LOWTABLE,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 5))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_LOWTABLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_POUF,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 6))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_POUF");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAA,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 7))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAB,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 8))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_SOFAC,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 9))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_Hideout_SofaC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FABRICCHAIR,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_FABRICCHAIR");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_MOUNATINSCREEN,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 11))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_MOUNATINSCREEN");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_PERFECTBODYPHOTO,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 12))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_PERFECTBODYPHOTO");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TABLE,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 13))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TABLE");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TEAMPHOTO,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 14))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_TEAMPHOTO");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHA,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 15))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHPRESS,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 16))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_BENCHPRESS");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLA,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 17))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLA");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLB,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 18))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLB");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLC,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 19))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLC");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KETTLEBELL,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 20))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KETTLEBELL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_DUMBBELLRACK,
		CMap_Hideout_Static::Create(m_pDevice, m_pContext, 21))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_HIDEOUT_KETTLEBELL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI_TEAMMATE,
		CMap_Hideout_Arashi::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI");
	}
#pragma endregion


	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL,
		CMap_Wall::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM,
		CMap_Item::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_COMMON_ITEM");
	}
	if (FAILED(pGameInstance->Add_Object_Prototype(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_MAP_SHOPKEEPER,
		CMap_Shopkeeper::Create(m_pDevice, m_pContext, 0))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_TestScene", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MAP_SHOPKEEPER");
	}
#endif
	return S_OK;
}
