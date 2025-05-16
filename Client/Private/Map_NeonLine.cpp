#include "ClientPCH.h"
#include "Map_NeonLine.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

int g_iNeonLineIndex = { 0 };

CMap_NeonLine::CMap_NeonLine(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

CMap_NeonLine::CMap_NeonLine(const CMap_NeonLine& _rhs)
    :CGameObject(_rhs)
{
}

HRESULT CMap_NeonLine::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);

    Add_ModelCom();

    return S_OK;
}

HRESULT CMap_NeonLine::Initialize(any pArg)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
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

    return S_OK;
}

void CMap_NeonLine::Tick(_float fTimeDelta)
{
    if (m_bDead)
        return;
    
    if (m_bSceneMapTool)
    {
        Using_Map_Tool();
    }

    auto pGameInstance = CMap_ToolManager::Get_Instance();

    if (pGameInstance->Get_Delete())
    {
        if (m_strName == pGameInstance->Get_SelectName())
        {
            pGameInstance->Set_Delete(false);

            m_bDead = true;
        }
    }
}

void CMap_NeonLine::Late_Tick(_float fTimeDelta)
{
    if (m_bDead)
        return;

    __super::Late_Tick(fTimeDelta);

    if (CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
    {
        Add_RenderObject(RENDER_GROUP::NEON);
    }
}

HRESULT CMap_NeonLine::Render()
{
    return S_OK;
}

HRESULT CMap_NeonLine::Render_OutLine()
{
    return S_OK;
}

HRESULT CMap_NeonLine::Render_Neon()
{
    if (FAILED(__super::Render(14))) 
    {
        MSG_RETURN(E_FAIL, "CMap_NeonLine::Render", "Failed to CGameObject::Render_Neon"); 
    }

    return S_OK;
}

HRESULT CMap_NeonLine::Ready_Components()
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

    m_pTransform->Set_Matrix(g_mUnit);

    auto pGameInstance = CGameInstance::Get_Instance();
    m_pNeonCrashTexture = dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_NEON_CRASH));

    return S_OK;
}

void CMap_NeonLine::Using_Map_Tool()
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
}

void CMap_NeonLine::Add_ModelCom()
{
    if (g_iNeonLineIndex == 0)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONCORNERA, g_aNull);
    }
    else if (g_iNeonLineIndex == 1)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONCORNERB, g_aNull);
    }
    else if (g_iNeonLineIndex == 2)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONCORNERC, g_aNull);
    }
    else if (g_iNeonLineIndex == 3)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONCORNERD, g_aNull);
    }
    else if (g_iNeonLineIndex == 4)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONCRASHA, g_aNull);
    }
    else if (g_iNeonLineIndex == 5)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONCRASHB, g_aNull);
    }
    else if (g_iNeonLineIndex == 6)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONLINEA, g_aNull);
    }
    else if (g_iNeonLineIndex == 7)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONLINEB, g_aNull);
    }
    else if (g_iNeonLineIndex == 8)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONROLLA, g_aNull);
    }
    else if (g_iNeonLineIndex == 9)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONSWAYA, g_aNull);
    }
    else if (g_iNeonLineIndex == 10)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONSWAYB, g_aNull);
    }
    else if (g_iNeonLineIndex == 11)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_NEONSWAYC, g_aNull);
    }
}

shared_ptr<CMap_NeonLine> CMap_NeonLine::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
    shared_ptr<CMap_NeonLine> pInstance = make_private_shared(CMap_NeonLine, _pDevice, _pContext);

    g_iNeonLineIndex = iIndex;

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CMap_NeonLine::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CMap_NeonLine::Clone(any pArg)
{
    shared_ptr<CMap_NeonLine> pInstance = make_private_shared_copy(CMap_NeonLine, *this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_RETURN(nullptr, "CMap_NeonLine::Clone", "Failed to Initialize");
    }

    return pInstance;
}
