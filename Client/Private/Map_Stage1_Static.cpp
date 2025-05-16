#include "ClientPCH.h"
#include "Map_Stage1_Static.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Stage1_Static::CMap_Stage1_Static(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Stage1_Static::CMap_Stage1_Static(const CMap_Stage1_Static& _rhs)
	:CGameObject(_rhs)
	, m_iStaticIndex(_rhs.m_iStaticIndex)
{
}

HRESULT CMap_Stage1_Static::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iStaticIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Stage1_Static::Initialize(any pArg)
{
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
	
	if (m_strName == L"FencingA"
		|| m_strName == L"FencingB"
		|| m_strName == L"FencingC"
		|| m_strName == L"FencingD"
		|| m_strName == L"FencingE"
		)
		m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform, false);
	else
		m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform);

	m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
	m_tInstModelDesc.iPassIndex = 15;
	m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
			}
		};

	// Shadow
	{
		m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelShadowDesc.iPassIndex = 2;
	}

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Stage1_Static::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif
	if (m_bDead)
		return;
	
	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}


	m_fTimer += fTimeDelta * 0.1f;
}

void CMap_Stage1_Static::Late_Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		bCheckOnce = false;
		ImGui::Begin("Please");
		ImGui::Checkbox("CMap_Stage1_Static", &bRender);
		ImGui::End();
	}
	if (!bRender) return;
#endif

	__super::Late_Tick(fTimeDelta);

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);

	//if (CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
	//{
	//	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//}
	//Add_RenderObject(RENDER_GROUP::SHADOW);
}

HRESULT CMap_Stage1_Static::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMap_Stage1_Static::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Static::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Static::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Stage1_Static::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Stage1_Static::Ready_Components()
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

void CMap_Stage1_Static::Using_Map_Tool()
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

	/*if (CMap_ToolManager::Get_Instance()->Get_PickObjMode())
	{
		Mouse();

		_vector vCamPos = CMap_ToolManager::Get_Instance()->Get_CameraPos();

		PxRaycastBuffer CallBack;
		PxQueryFilterData FilterData;
		m_vMouseRay = XMVector3Normalize(m_vMouseRay);
		FilterData.flags = PxQueryFlag::Enum::eSTATIC;
		if (CGameInstance::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			if (CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->raycast(
				PxVec3(XMVectorGetX(vCamPos), XMVectorGetY(vCamPos), XMVectorGetZ(vCamPos)),
				PxVec3(XMVectorGetX(m_vMouseRay), XMVectorGetY(m_vMouseRay), XMVectorGetZ(m_vMouseRay)),
				200.f,
				CallBack,
				PxHitFlag::Enum::eMESH_BOTH_SIDES, FilterData))
			{
				PxVec3 HitPos = CallBack.block.position;

				CMap_ToolManager::Get_Instance()->Set_PickingObject(true);
				CMap_ToolManager::Get_Instance()->Set_PickObjName(m_strName);
			}
		}
	}*/
}

void CMap_Stage1_Static::Add_ModelCom()
{
	if (m_iStaticIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FENCEA;
	}
	else if (m_iStaticIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FENCEB;
	}
	else if (m_iStaticIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FENCEC;
	}
	else if (m_iStaticIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLF;
	}
	else if (m_iStaticIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STEELWALL5A;
	}
	else if (m_iStaticIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTA;
	}
	else if (m_iStaticIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTC;
	}

	else if (m_iStaticIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTE;
	}
	else if (m_iStaticIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STEELWALL5B;
	}
	else if (m_iStaticIndex == 10)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STEELWALL5C;
	}
	else if (m_iStaticIndex == 11)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLA;
	}
	else if (m_iStaticIndex == 12)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLB;
	}
	else if (m_iStaticIndex == 13)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLC;
	}
	else if (m_iStaticIndex == 14)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STEELWALL4A;
	}
	else if (m_iStaticIndex == 16)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_PREFABA;
	}
	else if (m_iStaticIndex == 17)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_PREFABB;
	}
	else if (m_iStaticIndex == 18)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_ROCKA;
	}
	else if (m_iStaticIndex == 19)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_ROCKB;
	}
	else if (m_iStaticIndex == 20)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_ROCKC;
	}
	else if (m_iStaticIndex == 21)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_RUBBLEA;
	}
	else if (m_iStaticIndex == 22)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_RUBBLEB;
	}
	else if (m_iStaticIndex == 23)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FENCINGA;
	}
	else if (m_iStaticIndex == 24)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FENCINGB;
	}
	else if (m_iStaticIndex == 25)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FENCINGC;
	}
	else if (m_iStaticIndex == 26)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORA;
		}
	else if (m_iStaticIndex == 27)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORB;
		}
	else if (m_iStaticIndex == 28)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORC;
		}
	else if (m_iStaticIndex == 29)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORD;
		}
	else if (m_iStaticIndex == 30)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORE;
		}
	else if (m_iStaticIndex == 31)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_OUTSIDEFLOORF;
		}
	else if (m_iStaticIndex == 32)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_NDF0;
		}
	else if (m_iStaticIndex == 33)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_PLYWOODA;
		}
	else if (m_iStaticIndex == 34)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_PLYWOODB;
		}
	else if (m_iStaticIndex == 35)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BIOTOILET;
		}
	else if (m_iStaticIndex == 36)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_RUBBLEC;
		}
	else if (m_iStaticIndex == 37)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_RUBBLED;
		}
	else if (m_iStaticIndex == 38)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEWALLA;
		}
	else if (m_iStaticIndex == 39)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STEELPOLEA;
		}
	else if (m_iStaticIndex == 40)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWERCRANE_BASE;
		}
	else if (m_iStaticIndex == 41)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FENCINGD;
		}
	else if (m_iStaticIndex == 42)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FENCINGE;
		}	
	else if (m_iStaticIndex == 43)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_ROADBLOCKB;
		}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

void CMap_Stage1_Static::Mouse()
{
	POINT		ptMouse = CGameInstance::Get_Instance()->Get_CursorPos();
	_matrix		mView = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW);
	_matrix		mProj = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);
	_vector		vTemp;
	_matrix		mInvView = XMMatrixInverse(&vTemp, mView);
	_matrix		mInvProj = XMMatrixInverse(&vTemp, mProj);
	_float3		vMouseNearPos;
	_vector		vecMouseNearPos;
	RECT		rcClient;
	HWND		hWnd = CMap_ToolManager::Get_Instance()->Get_Hwnd();

	GetClientRect(hWnd, &rcClient);

	vMouseNearPos.x = ptMouse.x / (rcClient.right * 0.5f) - 1.f;
	vMouseNearPos.y = -ptMouse.y / (rcClient.bottom * 0.5f) + 1.f;
	vMouseNearPos.z = 0.f;

	vecMouseNearPos = XMLoadFloat3(&vMouseNearPos);
	vecMouseNearPos = XMVector3TransformCoord(vecMouseNearPos, mInvProj);

	_vector MousePos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	m_vMouseRay = vecMouseNearPos - MousePos;
	m_vMouseRay = XMVector4Transform(m_vMouseRay, mInvView);
}

shared_ptr<CMap_Stage1_Static> CMap_Stage1_Static::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Stage1_Static> pInstance = make_private_shared(CMap_Stage1_Static, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Static::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Stage1_Static::Clone(any pArg)
{
	shared_ptr<CMap_Stage1_Static> pInstance = make_private_shared_copy(CMap_Stage1_Static, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Static::Clone", "Failed to Initialize");
	}

	return pInstance;
}
