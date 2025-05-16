#include "ClientPCH.h"
#include "Map_Stage1_LightObject.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Stage1_LightObject::CMap_Stage1_LightObject(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

CMap_Stage1_LightObject::CMap_Stage1_LightObject(const CMap_Stage1_LightObject& _rhs)
    :CGameObject(_rhs)
    , m_iLightObjectIndex(_rhs.m_iLightObjectIndex)
{
}

HRESULT CMap_Stage1_LightObject::Initialize_Prototype(_uint iIndex)
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
    m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

    m_iLightObjectIndex = iIndex;

    return S_OK;
}

HRESULT CMap_Stage1_LightObject::Initialize(any pArg)
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

    LIGHTDESC Desc;

    Desc.eLightType = LIGHTTYPE::POINT;
    Desc.vDiffuse = _float4(1.0f, 0.925f, 0.850f, 1.0f);
    //Desc.vDiffuse = _float4(0.5f, 0.f, 0.f, 1.0f);

    Desc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    Desc.vSpecular = _float4(0.f, 0.f, 0.f, 0.f);
    Desc.fRange = 15.f;
    Desc.fLifeTime = 0.f;
    //Desc.vPosition = m_pTransform->Get_State(TRANSFORM::POSITION);

    CGameInstance::Get_Instance()->Add_Light(SCENE::MIZUHA, Desc, m_pTransform);

    m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
    m_tInstModelDesc.iPassIndex = 15;
    m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
        {
            if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
            {
                MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
            }
        };

    return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Stage1_LightObject::Tick(_float fTimeDelta)
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

void CMap_Stage1_LightObject::Late_Tick(_float fTimeDelta)
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
        ImGui::Checkbox("CMap_Stage1_LightObject", &bRender);
        ImGui::End();
    }
    if (!bRender) return;
#endif

    //if (CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
    //{
    //    Add_RenderObject(RENDER_GROUP::NONBLEND);
    //}

    m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
}

HRESULT CMap_Stage1_LightObject::Render()
{
    if (FAILED(__super::Render(0))) 
    {
        MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render"); 
    }

    /*Mesh_Index(m_strIndexName);

    if (FAILED(m_pModel->Bind_ShaderResourceViews(0, m_pShader)))
    {
        MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
    }

    Mesh_Render(m_strIndexName);*/
    
    return S_OK;
}

HRESULT CMap_Stage1_LightObject::Render_OutLine()
{
    return S_OK;
}

HRESULT CMap_Stage1_LightObject::Ready_Components()
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

void CMap_Stage1_LightObject::Using_Map_Tool()
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

void CMap_Stage1_LightObject::Add_ModelCom()
{
    if (m_iLightObjectIndex == 0)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SPOTLIGHTA;
    }
    else if (m_iLightObjectIndex == 1)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FLOODLIGHTB;
    }

    m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

shared_ptr<CMap_Stage1_LightObject> CMap_Stage1_LightObject::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
    shared_ptr<CMap_Stage1_LightObject> pInstance = make_private_shared(CMap_Stage1_LightObject, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype(iIndex)))
    {
        MSG_RETURN(nullptr, "CMap_Stage1_LightObject::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CMap_Stage1_LightObject::Clone(any pArg)
{
    shared_ptr<CMap_Stage1_LightObject> pInstance = make_private_shared_copy(CMap_Stage1_LightObject, *this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_RETURN(nullptr, "CMap_Stage1_LightObject::Clone", "Failed to Initialize");
    }

    return pInstance;
}
