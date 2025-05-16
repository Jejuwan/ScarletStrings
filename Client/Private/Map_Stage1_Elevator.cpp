#include "ClientPCH.h"
#include "Map_Stage1_Elevator.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

#include "CCTV.h"

#include "SceneCapture.h"
#include "Player.h"
#include "PlayerState.h"
#include "Teammate.h"

CMap_Stage1_Elevator::CMap_Stage1_Elevator(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Stage1_Elevator::CMap_Stage1_Elevator(const CMap_Stage1_Elevator& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CMap_Stage1_Elevator::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONELEVATOR, g_aNull);

	return S_OK;
}

HRESULT CMap_Stage1_Elevator::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple = 
		any_cast<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(pArg);
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (std::get<0>(tObjTuple) == SCENE::MAPTOOL)
	{
		m_strName = std::get<1>(tObjTuple);
		m_bSceneMapTool = true;

		if (CMap_ToolManager::Get_Instance()->Get_Load())
		{
			m_strNameNum = std::get<6>(tObjTuple);

			CMap_ToolManager::Get_Instance()->NameNum_List(m_strNameNum);

			m_Scale = std::get<2>(tObjTuple);
			m_Rotation = std::get<3>(tObjTuple);
			m_Translation = std::get<4>(tObjTuple);
			m_Mat = std::get<5>(tObjTuple);
			m_pTransform->Set_Matrix(m_Mat);
		   
			CMap_ToolManager::OBJECTDESC objDesc;

			objDesc.Scale = std::get<2>(tObjTuple);
			objDesc.Rotation = std::get<3>(tObjTuple);
			objDesc.Translation = std::get<4>(tObjTuple);
			objDesc.matrix = std::get<5>(tObjTuple);
			objDesc.strName = std::get<6>(tObjTuple);
			CMap_ToolManager::Get_Instance()->Emplace_Object(&objDesc);

		}
	}
	if (std::get<0>(tObjTuple) == SCENE::TEST)
	{
		m_strName = std::get<1>(tObjTuple);
		m_Scale = std::get<2>(tObjTuple);
		m_Rotation = std::get<3>(tObjTuple);
		m_Translation = std::get<4>(tObjTuple);
		m_Mat = std::get<5>(tObjTuple);
		m_pTransform->Set_Matrix(m_Mat);
		_matrix RPY = XMMatrixIdentity();
		RPY *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		RPY *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
		RPY *= XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_pTransform->Set_Matrix(RPY);
	}
	
	_float4x4 mat = m_pTransform->Get_Matrix();
	_vector Rotation, Scale, Pos;
	XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);

	PxTransform pose = PxTransform(PxIdentity);
	pose.p = PxVec3(mat._41, mat._42, mat._43);
	_float4 vQuat;
	XMStoreFloat4(&vQuat, Rotation);
	pose.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

	for (int i = 0; i < m_pModel->Get_TriangleMeshes().size(); i++)
	{
		PxRigidStatic* hfActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidStatic(pose);
		PxTriangleMeshGeometry hfGeom(m_pModel->Get_TriangleMeshes()[i], PxMeshScale(PxVec3(XMVectorGetX(Scale), XMVectorGetY(Scale), XMVectorGetZ(Scale))));
		PxShape* hfShape = PxRigidActorExt::createExclusiveShape(*hfActor, hfGeom, *pGameInstance->Get_PhysXEngine()->GetMaterial());

		pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*hfActor);
	}

	m_pModel->Set_Animation(0,1.f, false, 0.1f, false);


	if(!m_bSceneMapTool)
		m_pCCTV = dynamic_pointer_cast<CCCTV>(pGameInstance->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CCTV, m_pTransform));

	 // For Physx
	
	PxBoxControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { m_Translation.x,m_Translation.y,m_Translation.z };
	cDesc.halfHeight = 0.3f;
	cDesc.halfSideExtent = 1.5f;
	cDesc.halfForwardExtent = 1.5f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0f;
	cDesc.reportCallback = NULL;

	m_pController = pGameInstance->Get_PhysXEngine()->GetControllerManager()->createController(cDesc);
	m_pActorDesc.strTag = ACTORTAG::STATIC;
	m_pController->getActor()->userData = &m_pActorDesc;

	//Left
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { m_Translation.x,m_Translation.y,m_Translation.z };
	cDesc.halfHeight = 1.5f;
	cDesc.halfSideExtent = 0.3f;
	cDesc.halfForwardExtent = 1.5f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0f;
	cDesc.reportCallback = NULL;

	m_pControllerLeft = pGameInstance->Get_PhysXEngine()->GetControllerManager()->createController(cDesc);
	
	m_pActorDesc.strTag = ACTORTAG::STATIC;

	_vector Position = m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.5f + m_pTransform->Get_State(TRANSFORM::UP);
	pose.p = PxVec3(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position));
	
	_matrix mRotation = m_pTransform->Get_Matrix();
	mRotation.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	mRotation = XMMatrixRotationZ(XMConvertToRadians(90.f));
	
	Rotation = XMQuaternionRotationMatrix(mRotation);
	pose.q = PxQuat(XMVectorGetX(Rotation), XMVectorGetY(Rotation), XMVectorGetZ(Rotation), XMVectorGetW(Rotation));
	
	m_pControllerLeft->getActor()->setGlobalPose(pose);
	m_pControllerLeft->getActor()->userData = &m_pActorDesc;
	
	 //Right
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { m_Translation.x,m_Translation.y,m_Translation.z };
	cDesc.halfHeight = 1.5f;
	cDesc.halfSideExtent = 0.3f;
	cDesc.halfForwardExtent = 1.5f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0f;
	cDesc.reportCallback = NULL;

	m_pControllerRight = pGameInstance->Get_PhysXEngine()->GetControllerManager()->createController(cDesc);
	
	m_pActorDesc.strTag = ACTORTAG::STATIC;

	Position = m_pTransform->Get_State(TRANSFORM::POSITION) + m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.5f + m_pTransform->Get_State(TRANSFORM::UP);
	pose.p = PxVec3(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position));
	
	mRotation = m_pTransform->Get_Matrix();
	mRotation.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	mRotation = XMMatrixRotationZ(XMConvertToRadians(90.f));
	
	Rotation = XMQuaternionRotationMatrix(mRotation);
	pose.q = PxQuat(XMVectorGetX(Rotation), XMVectorGetY(Rotation), XMVectorGetZ(Rotation), XMVectorGetW(Rotation));
	
	m_pControllerRight->getActor()->setGlobalPose(pose);
	m_pControllerRight->getActor()->userData = &m_pActorDesc;
	
	 //Back
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { m_Translation.x,m_Translation.y,m_Translation.z };
	cDesc.halfHeight = 1.5f;
	cDesc.halfSideExtent = 1.5f;
	cDesc.halfForwardExtent = 0.3f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0f;
	cDesc.reportCallback = NULL;

	m_pControllerBack = pGameInstance->Get_PhysXEngine()->GetControllerManager()->createController(cDesc);
	
	m_pActorDesc.strTag = ACTORTAG::STATIC;

	Position = m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 1.5f + m_pTransform->Get_State(TRANSFORM::UP);
	pose.p = PxVec3(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position));
	
	mRotation = m_pTransform->Get_Matrix();
	mRotation.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	mRotation = XMMatrixRotationZ(XMConvertToRadians(90.f));
	
	Rotation = XMQuaternionRotationMatrix(mRotation);
	pose.q = PxQuat(XMVectorGetX(Rotation), XMVectorGetY(Rotation), XMVectorGetZ(Rotation), XMVectorGetW(Rotation));
	
	m_pControllerBack->getActor()->setGlobalPose(pose);
	m_pControllerBack->getActor()->userData = &m_pActorDesc;
	
	// Open
	m_pModel->Set_Animation(1, 1000.f, false, 0.f, false);
	if (!m_bSceneMapTool)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
		m_fOriginGravity = m_pPlayer->Get_Gravity();
	}

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Stage1_Elevator::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif
	if (m_bDead)
		return;
	
	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
		return;
	}
	/* Open */
	if (CGameInstance::Get_Instance()->Key_Down(VK_LEFT))
		m_pModel->Set_Animation(1, 1.f, false, 0.1f, false);

	/* Close */
	if (CGameInstance::Get_Instance()->Key_Down(VK_RIGHT))
		m_pModel->Set_Animation(0, 1.f, false, 0.1f, false);

	m_pModel->Tick_Animation(fTimeDelta);

	// ��ȣ�ۿ� ��
	if (m_bStop)
	{
		return;
	}

	auto pPlayerTransform = dynamic_pointer_cast<CTransform>(m_pPlayer->Get_Component(COMPONENT::TRANSFORM));
	float3 vPlayerPosition = pPlayerTransform->Get_State(TRANSFORM::POSITION);
	_float3 vPosition = m_pTransform->Get_State(TRANSFORM::POSITION);

	_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));

	const _float fExecuteThreshold = 1.f;
	if (false == m_bElevatorDown && fDistance <= fExecuteThreshold)
	{
		//m_bElevatorDown = true;
		m_pModel->Set_Animation(0, 0.5f, false, 0.1f, false);
		m_bEnableCCTV = true;

		auto pSceneCapture = CSceneCapture::Get_Instance();
		pSceneCapture->Ready(m_pRenderer);
		pSceneCapture->Record(CUTSCENE_CHANNEL::CHANNEL_MAIN);

		m_pRenderer->Set_CutsceneMode(true);
		m_pCCTV->Lock_Camera(true);
	}

	auto pPlayerState = m_pPlayer->Get_PlayerState();
	if (m_bEnableCCTV && m_pModel->Is_AnimationFinished(0) && pPlayerState->Get_CurrentState() == PLAYER_STATE::IDLE)
	{
		m_bElevatorDown = true;
	}

	if (m_bElevatorDown)
	{
		m_fTimer += fTimeDelta;

		if (m_fTimer > 0.01f)
		{
			vPosition.y -= 0.1f;
			m_pTransform->Set_State(TRANSFORM::POSITION, vPosition);
			m_fTimer = 0.f;

			m_pPlayer->Set_Gravity(0.f);

			float3 vPivotPosition = float3(vPlayerPosition.x, vPosition.y + 0.6f, vPlayerPosition.z);
			m_pPlayer->Get_Controller()->setFootPosition(PxExtendedVec3(vPivotPosition.x, vPivotPosition.y, vPivotPosition.z));
			pPlayerTransform->Set_State(TRANSFORM::POSITION, vPivotPosition);

			_float3 vLook = m_pTransform->Get_State(TRANSFORM::LOOK) * 0.6f;
			_float3 vRight = m_pTransform->Get_State(TRANSFORM::RIGHT) * 0.6f;

			auto pGameInstance = CGameInstance::Get_Instance();
			auto pTeammateLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_TEAMMATE);
			for (auto Iter = pTeammateLayer->Begin(); Iter != pTeammateLayer->End(); ++Iter)
			{
				auto pTeammate = dynamic_pointer_cast<CTeammate>(*Iter);
				auto pTeammateController = pTeammate->Get_Controller();
				auto pTeammateTransform = dynamic_pointer_cast<CTransform>(pTeammate->Get_Component(COMPONENT::TRANSFORM));
				
				float3 vTeammatePivot = float3(vPosition.x, vPosition.y + 0.6f, vPosition.z) - vLook - vRight;
				pTeammateController->setFootPosition(PxExtendedVec3(vTeammatePivot.x, vTeammatePivot.y, vTeammatePivot.z));
				pTeammateTransform->Set_State(TRANSFORM::POSITION, vTeammatePivot);
				pTeammate->Enable_Gravity(false);

				pTeammateTransform->LookAtDir(vLook);

				vRight *= -1.f;
			}
		}
		if (m_pTransform->Get_State(TRANSFORM::POSITION).y <= -48.5f)
		{
			m_bStop = true;
			m_pPlayer->Set_Gravity(m_fOriginGravity);

			m_bEnableCCTV = false;
			m_pRenderer->Set_CutsceneMode(false);
			m_pCCTV->Lock_Camera(false);

			/* Open */
			m_pModel->Set_Animation(1, 0.1f, false, 0.1f, false);

			auto pGameInstance = CGameInstance::Get_Instance();
			auto pTeammateLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_TEAMMATE);
			for (auto Iter = pTeammateLayer->Begin(); Iter != pTeammateLayer->End(); ++Iter)
			{
				auto pTeammate = dynamic_pointer_cast<CTeammate>(*Iter);
				pTeammate->Enable_Gravity(true);
			}
		}
	}

	if (m_bEnableCCTV)
		m_pCCTV->Tick(fTimeDelta);

	_vector Position = m_pTransform->Get_State(TRANSFORM::POSITION);

	m_pController->setFootPosition(PxExtendedVec3(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position)));
	
	Position = m_pTransform->Get_State(TRANSFORM::POSITION)  - m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.5f + m_pTransform->Get_State(TRANSFORM::UP);
	m_pControllerLeft->setFootPosition(PxExtendedVec3(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position)));

	Position = m_pTransform->Get_State(TRANSFORM::POSITION) + m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.5f + m_pTransform->Get_State(TRANSFORM::UP);
	m_pControllerRight->setFootPosition(PxExtendedVec3(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position)));

	Position = m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 1.5f + m_pTransform->Get_State(TRANSFORM::UP);
	m_pControllerBack->setFootPosition(PxExtendedVec3(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position)));
}

void CMap_Stage1_Elevator::Late_Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	__super::Late_Tick(fTimeDelta);

#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		bCheckOnce = false;
		ImGui::Begin("Please");
		ImGui::Checkbox("CMap_Stage1_Elevator", &bRender);
		ImGui::End();
	}
	if (!bRender) return;
#endif

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::SHADOW);

	if (m_bEnableCCTV)
		m_pCCTV->Late_Tick(fTimeDelta);
}

HRESULT CMap_Stage1_Elevator::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
	}

	if (FAILED(__super::Render(16)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMap_Stage1_Elevator::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Elevator::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Elevator::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Stage1_Elevator::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Stage1_Elevator::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: Model");
	}
	//m_pModel->Initialize_MaterialDesc(&m_tMaterialDesc);

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: RENDERER");
	}
	

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

void CMap_Stage1_Elevator::Using_Map_Tool()
{
	m_strSelectedName = CMap_ToolManager::Get_Instance()->Get_SelectName();
	if (m_strName == m_strSelectedName)
	{
		/* Scale, Radian, Translation, matrix */
		m_tuplePivot = CMap_ToolManager::Get_Instance()->Get_TuplePivot(); 

		m_strObjName = std::get<0>(m_tuplePivot);
		m_Scale = std::get<1>(m_tuplePivot);
		m_Rotation = std::get<2>(m_tuplePivot);
		m_Translation = std::get<3>(m_tuplePivot);
		m_Mat = std::get<4>(m_tuplePivot);
		m_pTransform->Set_Matrix(m_Mat);

		_matrix RPY = XMMatrixIdentity();
		RPY *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		RPY *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
		RPY *= XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_pTransform->Set_Matrix(RPY);

		if (CMap_ToolManager::Get_Instance()->Get_Emplace())
		{
			CMap_ToolManager::OBJECTDESC objDesc;
			ZeroMemory(&objDesc, sizeof(CMap_ToolManager::OBJECTDESC));

			objDesc.strName = m_strName;
			objDesc.Scale = m_Scale;
			objDesc.Rotation = m_Rotation;
			objDesc.Translation = m_Translation;
			objDesc.matrix = m_pTransform->Get_Matrix();
			objDesc.bSave = false;
			CMap_ToolManager::Get_Instance()->Emplace_Object(&objDesc);

			CMap_ToolManager::Get_Instance()->Set_Emplace(false);
		}
	}
	auto pGameInstance = CMap_ToolManager::Get_Instance();

	if (pGameInstance->Get_AllClear())
	{
		m_bDead = true;
		pGameInstance->Set_AllClear(false);
	}

	if (pGameInstance->Get_Delete())
	{
		if (m_strName == pGameInstance->Get_SelectName())
		{
			pGameInstance->Set_Delete(false);

			m_bDead = true;
		}
	}
}

shared_ptr<CMap_Stage1_Elevator> CMap_Stage1_Elevator::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMap_Stage1_Elevator> pInstance = make_private_shared(CMap_Stage1_Elevator, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Elevator::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Stage1_Elevator::Clone(any pArg)
{
	shared_ptr<CMap_Stage1_Elevator> pInstance = make_private_shared_copy(CMap_Stage1_Elevator, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Elevator::Clone", "Failed to Initialize");
	}

	return pInstance;
}
