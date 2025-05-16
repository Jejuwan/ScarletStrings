#include "ClientPCH.h"
#include "Map_Hideout_Arashi.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"
#include "NPC_Part.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

#include "UI_Manager.h"
#include "UI_Indicator_Communication.h"

CMap_Hideout_Arashi::CMap_Hideout_Arashi(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Hideout_Arashi::CMap_Hideout_Arashi(const CMap_Hideout_Arashi& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CMap_Hideout_Arashi::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_ARASHI_TEAMMATE, g_aNull);

	return S_OK;
}

HRESULT CMap_Hideout_Arashi::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMap_Hideout_Arashi::Initialize", "Failed to CGameObject::Initialize");
	}

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

	// Hide Mask && Hood
	m_pModel->Hide_Mesh(4);
	m_pModel->Hide_Mesh(5);

	m_pModel->Set_Animation(IDX(ANIMATION::ARASHI_TEAMMATE::AL_WAIT_SIT01), 1.f, false, 0.1f, true);
	m_vecMeshPassIndexPair.push_back({ 0, 17 }); // AMB
	m_vecMeshPassIndexPair.push_back({ 2, 17 }); // AMB
	m_vecMeshPassIndexPair.push_back({ 3, 17 }); // AMB

	m_vecMeshPassIndexPair.push_back({ 1, 0 });
	m_vecMeshPassIndexPair.push_back({ 6, 0 });
	m_vecMeshPassIndexPair.push_back({ 7, 0 });

	for (_uint i = 0; i < m_pModel->Get_NumMeshes(); ++i)
	{
		SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
		tShaderDesc.tMaterialDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);

		m_pModel->Set_ShaderDesc(i, tShaderDesc);
	}

#ifdef JOKE
	m_pTexDissolve = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_043.dds"), 1, true);
#endif

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Hideout_Arashi::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif
	if (m_bDead)
		return;

	m_pModel->Tick_Animation(fTimeDelta);
	
	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

#ifdef JOKE
	static _float f;
	if (CGameInstance::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (fabsf(Function::RelativeAngle(Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM), m_pTransform)) < XM_PI / 12.f
		&&	Function::Distance(Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM), m_pTransform) < 10.f)
		{
			CGameInstance::Get_Instance()->Register_OnTickListener(shared_from_this(),
				[&](_float fTimeDelta)->_bool
				{
					if (f < 0.1f) f += fTimeDelta;
					else
					{
					Dissolve();
					f = 0.f;
					return false;
					}
					return true;
				}
			);
		}
	}
	if (m_bDissolve)
	{
		m_fDissolveThreshold += fTimeDelta / m_fDissolveDuration;
	}
#endif
}

void CMap_Hideout_Arashi::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_Hideout_Arashi", &bRender);
			ImGui::End();

			ImGui::Begin("test normal shadow");
			static float fNormalDepthOffsetScale = 0;
			if (ImGui::InputFloat("fNormalDepthOffsetScale", &fNormalDepthOffsetScale))
			{
				m_pShader->Bind_Float("fNormalDepthOffsetScale", fNormalDepthOffsetScale);
			}
			ImGui::End();
		}
	if (!bRender) return;
#endif

	if (m_bDead)
		return;

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::SHADOW);
#ifdef JOKE
	Add_RenderObject(RENDER_GROUP::BLEND);
#endif

	__super::Late_Tick(fTimeDelta);

#pragma region UI
	shared_ptr<CTransform> TempPlayerTransform = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	_float3	PlayerPos		= TempPlayerTransform->Get_State(TRANSFORM::POSITION);
	_float3 ArasiPos		= m_pTransform->Get_State(TRANSFORM::POSITION);
	auto	pUI_Manager		= CUI_Manager::Get_Instance()->Get_Indicator_Communication();

	_float3 TempDistance	= _float3(0.f, 0.f, 0.f);
	TempDistance.x			= abs(PlayerPos.x - ArasiPos.x);
	TempDistance.y			= abs(PlayerPos.y - ArasiPos.y);
	TempDistance.z			= abs(PlayerPos.z - ArasiPos.z);

	if (4.f >= TempDistance.x && 4.f >= TempDistance.z)
	{
		pUI_Manager->Set_Switch_Icon_Near(true);
	}
	else
	{
		pUI_Manager->Set_Switch_Icon_Near(false);
	}

	pUI_Manager->Set_IndicatorPosition(this->m_pTransform->Get_WorldMatrix());
#pragma endregion


}

HRESULT CMap_Hideout_Arashi::Render()
{
#ifdef JOKE
	if (m_bDissolve)
	{
		if (FAILED(m_pShader->Bind_Float("g_fThreshold", m_fDissolveThreshold)))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CShader::Bind_Float");
		}

		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDISSOLVE, m_pTexDissolve->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CShader::Bind_ShaderResourceView");
		}

		if (FAILED(CGameObject::Render(5)))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CGameObject::Render");
		}
	}
	else
	{
#else
#endif
	if (FAILED(Bind_Transform(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to CGameObject::Bind_Transform");
	}

	for (_uint i = 0; i < m_vecMeshPassIndexPair.size(); ++i)
	{
		_uint iMeshIndex = m_vecMeshPassIndexPair[i].first;
		_uint iPassIndex = m_vecMeshPassIndexPair[i].second;
		if (FAILED(m_pModel->Bind_ShaderResourceViews(iMeshIndex, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_ShaderResourceViews");
		}
		if (FAILED(m_pModel->Bind_ShaderOptions(iMeshIndex, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_ShaderOptions");
		}
		if (FAILED(m_pModel->Bind_BoneMatrices(iMeshIndex, m_pShader, SHADER_BONE)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_BoneMatrices");
		}

		if (FAILED(m_pModel->Render(iMeshIndex, m_pShader, iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Render Model");
		}
	}
#ifdef JOKE
	}
#endif
	return S_OK;
}

HRESULT CMap_Hideout_Arashi::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Hideout_Arashi::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Hideout_Arashi::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Hideout_Arashi::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Hideout_Arashi::Ready_Components()
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

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: RENDERER");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

void CMap_Hideout_Arashi::Using_Map_Tool()
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

#ifdef JOKE
#include "Effect_Manager.h"
void CMap_Hideout_Arashi::Dissolve()
{
	m_bDissolve = true;
	m_fDissolveThreshold = 0.f;
	CEffect_Manager::Get_Instance()->Fetch(VFX::FADE_PARTICLE, shared_from_gameobject());
}
#endif

shared_ptr<CMap_Hideout_Arashi> CMap_Hideout_Arashi::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Hideout_Arashi> pInstance = make_private_shared(CMap_Hideout_Arashi, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Hideout_Arashi::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Hideout_Arashi::Clone(any pArg)
{
	shared_ptr<CMap_Hideout_Arashi> pInstance = make_private_shared_copy(CMap_Hideout_Arashi, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Hideout_Arashi::Clone", "Failed to Initialize");
	}

	return pInstance;
}
