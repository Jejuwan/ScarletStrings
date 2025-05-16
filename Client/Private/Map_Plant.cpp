#include "ClientPCH.h"
#include "Map_Plant.h"
#include "GameInstance.h"
#include "Light.h"
#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Plant::CMap_Plant(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Plant::CMap_Plant(const CMap_Plant& _rhs)
	:CGameObject(_rhs)
	, m_iPlantIndex(_rhs.m_iPlantIndex)
{
}

HRESULT CMap_Plant::Initialize_Prototype(_int iPlantIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iPlantIndex = iPlantIndex;

	return S_OK;
}

HRESULT CMap_Plant::Initialize(any pArg)
{
	m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	m_tMaterialDesc.vEmissive = _float4(0.f, 0.f, 0.f, 1.f);
	m_tMaterialDesc.fShininess = 0.1f;

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	Add_ModelCom();

	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
		any_cast<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(pArg);

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

	/*
	// As Plant
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
		PxRigidStatic* hfActor = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetPhysics()->createRigidStatic(pose);
		PxTriangleMeshGeometry hfGeom(m_pModel->Get_TriangleMeshes()[i], PxMeshScale(PxVec3(XMVectorGetX(Scale), XMVectorGetY(Scale), XMVectorGetZ(Scale))));
		PxShape* hfShape = PxRigidActorExt::createExclusiveShape(*hfActor, hfGeom, *CGameInstance::Get_Instance()->Get_PhysXEngine()->GetMaterial());

		CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->addActor(*hfActor);
	}
	*/

	//Hide_Mesh(m_strName);

	// Hide Mesh
	{
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_GRASS || m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHB)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHA || m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHC
			|| m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHE ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHD)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}
		m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelDesc.iPassIndex = 15;
	}

	// Mesh Render
	{
		m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_GRASS)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(10);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHA || m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHB ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHC || m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHE ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BEECHD)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}
	}

	// Shadow
	{
		m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelShadowDesc.iPassIndex = 2;
	}

	return S_OK;
}

void CMap_Plant::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

	m_fTimer += fTimeDelta * 0.1f;
}

void CMap_Plant::Late_Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	__super::Late_Tick(fTimeDelta);

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);
	//Add_RenderObject(RENDER_GROUP::NONBLEND);
	//Add_RenderObject(RENDER_GROUP::SHADOW);
}

HRESULT CMap_Plant::Render()
{
	return S_OK;
}

HRESULT CMap_Plant::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	//if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Plant::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	//}

	//if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Plant::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	//}

	return S_OK;
}

HRESULT CMap_Plant::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Plant::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModelRenderer = Get_Component<CModelRenderer>(COMPONENT::MODEL_RENDERER);
	if (nullptr == m_pModelRenderer)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: MODEL_RENDERER");
	}

	return S_OK;
}

void CMap_Plant::Using_Map_Tool()
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

void CMap_Plant::Add_ModelCom()
{
	if (m_iPlantIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_GRASS;
		//m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_GRASS, g_aNull);
	}
	else if (m_iPlantIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BEECHA;
		//m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BEECHA, g_aNull);
	}
	else if (m_iPlantIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BEECHB;
		//m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BEECHB, g_aNull);
	}
	else if (m_iPlantIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BEECHC;
		//m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BEECHC, g_aNull);
	}
	else if (m_iPlantIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BEECHD;
		//m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BEECHD, g_aNull);
	}
	else if (m_iPlantIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BEECHE;
		//m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BEECHE, g_aNull);
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

void CMap_Plant::Hide_Mesh(wstring MeshName)
{
    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    if (m_strIndexName == TEXT("Grass") || m_strIndexName == TEXT("BeechB"))
    {
        //m_pModel->Hide_Mesh(0);
    }
    else if (m_strIndexName == TEXT("BeechA") || m_strIndexName == TEXT("BeechC") 
        || m_strIndexName == TEXT("BeechE") || m_strIndexName == TEXT("HugeTree")||
        m_strIndexName == TEXT("BeechD"))
    {
        //m_pModel->Hide_Mesh(1);
    }
}

HRESULT CMap_Plant::Mesh_Render(wstring MeshName)
{
    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    if (m_strIndexName == TEXT("Grass"))
    {
        m_pModel->Render(0, m_pShader, 10);
    }
    else if (m_strIndexName == TEXT("BeechA") || m_strIndexName == TEXT("BeechB") ||
        m_strIndexName == TEXT("BeechC") || m_strIndexName == TEXT("BeechE") || m_strIndexName == TEXT("HugeTree") ||
        m_strIndexName == TEXT("BeechD"))
    {
        m_pModel->Render(1, m_pShader, 5);
    }
    else if (m_strIndexName == TEXT("BeechB"))
    {
        m_pModel->Render(0, m_pShader, 5);
    }
    return S_OK;
}

void CMap_Plant::Mesh_Index(wstring MeshName)
{

    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    //네온 매쉬 안그림
    if (m_strIndexName == TEXT("Grass") || m_strIndexName == TEXT("BeechB"))
    {
        m_iMeshIndex = 0;
    }
    else if (m_strIndexName == TEXT("BeechA")  ||  m_strIndexName == TEXT("BeechC") || m_strIndexName == TEXT("BeechE") || m_strIndexName == TEXT("HugeTree")||
        m_strIndexName == TEXT("BeechD"))
    {
        m_iMeshIndex = 1;
    }
}

shared_ptr<CMap_Plant> CMap_Plant::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Plant> pInstance = make_private_shared(CMap_Plant, _pDevice, _pContext);

	//g_iPlant = iIndex;

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Plant::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Plant::Clone(any pArg)
{
	shared_ptr<CMap_Plant> pInstance = make_private_shared_copy(CMap_Plant, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Plant::Clone", "Failed to Initialize");
	}

	return pInstance;
}
