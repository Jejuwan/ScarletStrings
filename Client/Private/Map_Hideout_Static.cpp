#include "ClientPCH.h"
#include "Map_Hideout_Static.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Hideout_Static::CMap_Hideout_Static(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Hideout_Static::CMap_Hideout_Static(const CMap_Hideout_Static& _rhs)
	:CGameObject(_rhs)
	, m_iIndex(_rhs.m_iIndex)
{
}

HRESULT CMap_Hideout_Static::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Hideout_Static::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMap_Hideout_Static::Initialize", "Failed to CGameObject::Initialize");
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

	if (std::get<0>(tObjTuple) == SCENE::HIDEOUT)
	{
		m_strName = std::get<1>(tObjTuple);
		m_Scale = std::get<2>(tObjTuple);
		m_Rotation = std::get<3>(tObjTuple);
		m_Translation = std::get<4>(tObjTuple);
		m_Mat = std::get<5>(tObjTuple);
		m_strNameNum = std::get<6>(tObjTuple);
		m_pTransform->Set_Matrix(m_Mat);
		_matrix RPY = XMMatrixIdentity();
		RPY *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		RPY *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
		RPY *= XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_pTransform->Set_Matrix(RPY);
	}

	m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform);

	m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
	m_tInstModelDesc.iPassIndex = 15;
	m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
			}
			else
				return S_OK;
		};

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Hideout_Static::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif

	/*if (m_bDead)
		return;*/
	
	__super::Tick(fTimeDelta);

	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}
	
}

void CMap_Hideout_Static::Late_Tick(_float fTimeDelta)
{
	/*if (m_bDead)
		return;*/

	__super::Late_Tick(fTimeDelta);
	
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
}

HRESULT CMap_Hideout_Static::Render()
{
	return S_OK;
}

_bool CMap_Hideout_Static::Return()
{
	return m_bDead;
}

HRESULT CMap_Hideout_Static::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{

	return S_OK;
}

HRESULT CMap_Hideout_Static::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Hideout_Static::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pModelRenderer = Get_Component<CModelRenderer>(COMPONENT::MODEL_RENDERER);
	if (nullptr == m_pModelRenderer)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: MODEL_RENDERER");
	}

	return S_OK;
}

void CMap_Hideout_Static::Using_Map_Tool()
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
		pGameInstance->Clear_Obj();
		if (pGameInstance->Get_ClearDone())
		{
			m_bDead = true;
		}
	}
	else
	{
		m_bDead = false;
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

void CMap_Hideout_Static::Add_ModelCom()
{
	if (m_iIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_FLOOR;
	}
	if (m_iIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_CARPET;
	}
	if (m_iIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_COLE;
	}
	if (m_iIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_FIREPLACE;
	}
	if (m_iIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_KITCHEN;
	}
	if (m_iIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_LOWTABLE;
	}
	if (m_iIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_POUF;
	}
	if (m_iIndex == 7)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_SOFAA;
	}
	if (m_iIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_SOFAB;
	}
	if (m_iIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_SOFAC;
	}
	if (m_iIndex == 10)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_FABRICCHAIR;
	}
	if (m_iIndex == 11)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_MOUNATINSCREEN;
	}
	if (m_iIndex == 12)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_PERFECTBODYPHOTO;
	}
	if (m_iIndex == 13)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_TABLE;
	}
	if (m_iIndex == 14)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_TEAMPHOTO;
	}
	if (m_iIndex == 15)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_BENCHA;
	}
	if (m_iIndex == 16)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_BENCHPRESS;
	}
	if (m_iIndex == 17)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_DUMBBELLA;
	}
	if (m_iIndex == 18)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_DUMBBELLB;
	}
	if (m_iIndex == 19)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_DUMBBELLC;
	}
	if (m_iIndex == 20)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_KETTLEBELL;
	}
	if (m_iIndex == 21)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HIDEOUT_DUMBBELLRACK;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}


shared_ptr<CMap_Hideout_Static> CMap_Hideout_Static::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Hideout_Static> pInstance = make_private_shared(CMap_Hideout_Static, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Hideout_Static::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Hideout_Static::Clone(any pArg)
{
	shared_ptr<CMap_Hideout_Static> pInstance = make_private_shared_copy(CMap_Hideout_Static, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Hideout_Static::Clone", "Failed to Initialize");
	}

	return pInstance;
}
