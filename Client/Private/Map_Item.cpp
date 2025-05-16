#include "ClientPCH.h"
#include "Map_Item.h"
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

#include "Effect_Manager.h"
#include "Bone.h"

#include "UI_Manager.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"

CMap_Item::CMap_Item(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Item::CMap_Item(const CMap_Item& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CMap_Item::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_COMMON_ITEM, g_aNull);

	return S_OK;
}

HRESULT CMap_Item::Initialize(any pArg)
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
	if (std::get<0>(tObjTuple) == SCENE::TEST ||
		std::get<0>(tObjTuple) == SCENE::SUOH ||
		std::get<0>(tObjTuple) == SCENE::SUOH_BATTLE ||
		std::get<0>(tObjTuple) == SCENE::HIDEOUT)
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

	m_eItemState = ITEMSTATE::STATE_WAIT;
	m_pModel->Set_Animation(0, 4.f, false, 0.1f, true);

	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMap_Item::Start_Effect, this));
	m_matPivot = XMMatrixScaling(1.5f, 1.5f, 1.5f) * m_pModel->Get_Bone("joint0")->Get_CombinedTransformation() * m_pTransform->Get_Matrix();

	m_bNoticeItem	= false;
	m_iSoundCounter = 0;

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Item::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif

	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

	Item_State(fTimeDelta);

}

void CMap_Item::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_Item", &bRender);
			ImGui::End();
		}
	if (!bRender) return;

	//if (CImGui_Manager::Get_Instance()->Is_Enable())
	//{
	//	ImGui::Begin("Glitch");

	//	ImGui::SliderFloat4("Glitch Color", &m_fGlitchBaseColor.x, 0.f, 10.f);
	//	ImGui::SliderFloat("Glitch Bloom", &m_fGlitchBloomStrength, 0.f, 10.f);

	//	ImGui::End();
	//}

#endif

	m_fTimeFactor += fTimeDelta * 10.f;
	if (m_fTimeFactor >= 2.f)
	{
		m_fTimeFactor = 0.f;
		m_fSeed0 += fTimeDelta;
	}
	if (m_fTimeFactor >= 1.f)
	{
		m_fSeed1 += fTimeDelta;
	}

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::NEON);
}

HRESULT CMap_Item::Render()
{
	if (FAILED(m_pShader->Bind_RawValue("g_vTilingFactor", &m_vTilingFactor, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(m_pShader->Bind_Float("g_fSeed", m_fSeed0)))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Render", "Failed to Bind_Float");
	}

	if (FAILED(__super::Render(12)))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMap_Item::Render_Bloom()
{
	return S_OK;
}

HRESULT CMap_Item::Render_Neon()
{
	if (FAILED(m_pShader->Bind_RawValue("g_vBaseLineColor", &m_vItemLineColor, sizeof(_float4))))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Render_Bloom", "Failed to Bind_RawValue");
	}
	if (FAILED(__super::Render(14)))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Render_Bloom", "Failed to CGameObject::Render");
	}

	if (FAILED(m_pGlitchShader->Bind_Matrix(SHADER_MATWORLD, m_matPivot)))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pGlitchShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pGlitchShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}

	if (FAILED(m_pGlitchShader->Bind_Vector(SHADER_CAMPOS, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION))))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Vector");
	}

	if (FAILED(m_pGlitchTexture->Bind_ShaderResourceView(m_pGlitchShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to Bind_ShaderResourceView");
	}
	if (FAILED(m_pMaskTexture->Bind_ShaderResourceView(m_pGlitchShader, aiTextureType_DIFFUSE, "g_NoiseMask")))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pGlitchShader->Bind_Float("g_fSeed", m_fSeed1)))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Render", "Failed to Bind_Float");
	}
	if (FAILED(m_pGlitchShader->Bind_RawValue("g_vGlitchBaseColor", &m_fGlitchBaseColor, sizeof(_float4))))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Render_Bloom", "Failed to Bind_RawValue");
	}

	if (FAILED(m_pGlitchBuffer->Render(m_pGlitchShader, 5)))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Render_Bloom", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMap_Item::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	//if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Item::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	//}

	//if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Item::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	//}

	return S_OK;
}

HRESULT CMap_Item::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Item::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to Get_Component: RENDERER");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pGlitchShader = dynamic_pointer_cast<CShader>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE));
	if (nullptr == m_pGlitchShader)
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to Clone_Component: VTXPOSTEX_CUTSCENE");
	}

	m_pGlitchBuffer = dynamic_pointer_cast<CVIBuffer_Rect>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_RECT));
	if (nullptr == m_pGlitchBuffer)
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to Clone_Component: VIBUFFER_RECT");
	}

	m_pGlitchTexture = dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_GLITCH_PATTERN));
	if (nullptr == m_pGlitchTexture)
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to Clone_Component: PROTOTYPE_COMPONENT_TEXTURE_GLITCH_PATTERN");
	}
	m_pMaskTexture = dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_MASK_CIRCULAR));
	if (nullptr == m_pMaskTexture)
	{
		MSG_RETURN(E_FAIL, "CMap_Item::Ready_Components", "Failed to Clone_Component: PROTOTYPE_COMPONENT_TEXTURE_MASK_CIRCULAR");
	}

	return S_OK;
}

void CMap_Item::Using_Map_Tool()
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

	/*if (pGameInstance->Get_AllClear())
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
	}*/

	if (pGameInstance->Get_Delete())
	{
		if (m_strName == pGameInstance->Get_SelectName())
		{
			pGameInstance->Set_Delete(false);

			m_bDead = true;
		}
	}
}

void CMap_Item::Item_State(_float fTimeDelta)
{
	if (!m_bSceneMapTool)
	{
		shared_ptr<CTransform> TempPlayerTramsform =
			CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

		_float3 PlayerPos = TempPlayerTramsform->Get_State(TRANSFORM::POSITION);
		_float3 ItemPos = m_pTransform->Get_State(TRANSFORM::POSITION);

		_float3 TempDistance = { 0.f,0.f,0.f };

		TempDistance.x = abs(PlayerPos.x - ItemPos.x);
		TempDistance.y = abs(PlayerPos.y - ItemPos.y);
		TempDistance.z = abs(PlayerPos.z - ItemPos.z);

		if (TempDistance.x <= 1.f &&
			TempDistance.y <= 1.f &&
			TempDistance.z <= 1.f)
		{
			if (10 >= CUI_Manager::Get_Instance()->Get_ItemCount() && m_eItemState != ITEMSTATE::STATE_GET)
			{
				m_eItemState = ITEMSTATE::STATE_GET;

				CGameInstance::Get_Instance()->PlaySoundW(TEXT("UI_06.Item_Get.wav"), 2.f);
			}
		}

		if (25.f >= TempDistance.x && 25.f >= TempDistance.z && 2.f >= TempDistance.y)
		{
			m_bNoticeItem = true;
		}
		else
		{
			m_bNoticeItem = false;
			//m_iSoundCounter = 0;
		}

		if (true == m_bNoticeItem && 0 == m_iSoundCounter)
		{
			auto UI_BrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			UI_BrainTalk->Set_BrainTalk_SpecialSituation(10);
			m_iSoundCounter += 1;
		}

	}
	if (m_eItemState == ITEMSTATE::STATE_WAIT)
	{
		m_pModel->Set_Animation(0, 4.f, false, 0.1f, true);

		m_fEffectTimer += fTimeDelta;
		if (m_fEffectTimer >= 0.4f)
		{
			CEffect_Manager::Get_Instance()->Fetch(VFX::ITEM_WAIT, make_pair(shared_from_gameobject(), "joint0"));
			m_fEffectTimer = 0.f;
		}
	}
	if (m_eItemState == ITEMSTATE::STATE_GET)
	{
		m_pModel->Set_Animation(1, 4.f, false, 0.1f, false);

		if (m_pModel->Is_AnimationFinished(1))
		{
			CUI_Manager::Get_Instance()->Set_ItemCountPlus();
			m_bDead = true;

			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_SpecialSituation(CUI_Indicator_BrainTalk_Mizuha::BT_KASANE_GET_ITEM);
		}
	}

	m_pModel->Tick_Animation(fTimeDelta);
}

_bool CMap_Item::Return()
{
	return m_bDead;
}

void CMap_Item::Start_Effect()
{
	CEffect_Manager::Get_Instance()->Fetch(VFX::ITEM_GET_0, make_pair(shared_from_gameobject(), "joint0"));
	CEffect_Manager::Get_Instance()->Fetch(VFX::ITEM_GET_1, make_pair(shared_from_gameobject(), "joint0"));
}

shared_ptr<CMap_Item> CMap_Item::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMap_Item> pInstance = make_private_shared(CMap_Item, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMap_Item::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Item::Clone(any pArg)
{
	shared_ptr<CMap_Item> pInstance = make_private_shared_copy(CMap_Item, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Item::Clone", "Failed to Initialize");
	}

	return pInstance;
}
