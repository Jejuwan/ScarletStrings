#include "ClientPCH.h"
#include "Map_Suoh_Dynamic.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "Player.h"
#include "Camera_Player.h"

#include "Monster.h"
#include "Psychokinesis.h"
#include <regex>

CMap_Suoh_Dynamic::CMap_Suoh_Dynamic(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CMap_Dynamic(_pDevice, _pContext)
{
}

CMap_Suoh_Dynamic::CMap_Suoh_Dynamic(const CMap_Suoh_Dynamic& _rhs)
    : CMap_Dynamic(_rhs)
    , m_iDynamicIndex(_rhs.m_iDynamicIndex)
{
}

HRESULT CMap_Suoh_Dynamic::Initialize_Prototype(_uint iIndex)
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER) | BIT(COMPONENT::MODEL);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
    m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

    m_iDynamicIndex = iIndex;

    Add_DissolveModel();

    return S_OK;
}

HRESULT CMap_Suoh_Dynamic::Initialize(any pArg)
{

    m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    m_tMaterialDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    m_tMaterialDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
    m_tMaterialDesc.vEmissive = _float4(0.f, 0.f, 0.f, 1.f);
    m_tMaterialDesc.fShininess = 0.1f;

    if (FAILED(__super::Initialize(pArg)))
    {
        MSG_RETURN(E_FAIL, "CMap_Suoh_Dynamic::Initialize", "Failed to CGameObject::Initialize");
    }

    Add_ModelCom();

    Create_Light(m_pTransform);

    std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
        any_cast<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(pArg);

    m_copyTuplePivot = tObjTuple;

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
    if (std::get<0>(tObjTuple) == SCENE::SUOH ||
        std::get<0>(tObjTuple) == SCENE::SUOH_BATTLE)
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

    if (std::get<0>(tObjTuple) == SCENE::SUOH)
        m_bSouhScene = true;

    _float4x4 mat = m_pTransform->Get_Matrix();
    _vector Rotation, Scale, Pos;
    XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);

    PxTransform pose = PxTransform(PxIdentity);
    pose.p = PxVec3(mat._41, mat._42, mat._43);
    _float4 vQuat;
    XMStoreFloat4(&vQuat, Rotation);
    pose.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

    shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
    _float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
    m_pActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(pose);
    m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(m_vExtents.x, m_vExtents.y, m_vExtents.z), *pGameInstance->Get_PhysXEngine()->GetMaterial());
    m_pShape->setLocalPose(PxTransform(PxVec3(m_vLocalPose.x, m_vLocalPose.y, m_vLocalPose.z)));

    m_pActor->attachShape(*m_pShape);
    if(CGameInstance::Get_Instance()->Current_Scene() == SCENE::CUTSCENE_MIZUHA)
        SetPhysX(false);
    else 
        SetPhysX(true);
    //m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

    ACTORDESC actorDesc;
    actorDesc.bThrow = false;
    m_pActor->userData = &actorDesc;

    m_fMass *= 1.5f;
    m_fThrowForce = 150000.f;
    PxRigidBodyExt::updateMassAndInertia(*m_pActor, m_fMass);
    pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);

    // Hide_Mesh
    {
        if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARA ||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARC)
        {
            m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 2);
        }
        else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARB ||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARD)
        {
            m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
        }

        if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARB ||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARD)
        {
            m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
        }
        else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARC)
        {
            m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
        }
        else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARA)
        {
            m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
        }

        m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
        m_tInstModelDesc.iPassIndex = m_iPassIndex;
        m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
        {
            if (m_bDissolve)
            {
                if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
                {
                    return E_FAIL;
                }

                if (FAILED(pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
                {
                    return E_FAIL;
                }
            }

            if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
            {
                MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
            }

            return S_OK;
        };
    }

    // Mesh Render
    {
        m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
        m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
        {
            if (m_bDissolve)
            {
                if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
                {
                    return E_FAIL;
                }

                if (FAILED(pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
                {
                    return E_FAIL;
                }
            }

            if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
            {
                MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
            }

            return S_OK;
        };

        /* 창문 */
        if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARA ||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARC)
        {
            m_tInstModelMeshDesc.vecMeshIndex.push_back(2);
            m_tInstModelMeshDesc.vecPassIndex.push_back(7);
        }
        else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARB ||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARD)
        {
            m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
            m_tInstModelMeshDesc.vecPassIndex.push_back(7);
        }

        /* 뼈대 */
        if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARB ||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARD)
        {
            m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
            m_tInstModelMeshDesc.vecPassIndex.push_back(6);
        }
        else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARC)
        {
            m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
            m_tInstModelMeshDesc.vecPassIndex.push_back(6);
        }
        else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CARA)
        {
            m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
            m_tInstModelMeshDesc.vecPassIndex.push_back(6);
        }
    }

    // Shadow
    {
        m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
        m_tInstModelShadowDesc.iPassIndex = 2;
    }

    // Outline
    {
        m_tInstModelOutlineDesc.wstrModelTag = m_wstrModelTag;
        m_tInstModelOutlineDesc.iPassIndex = 3;
        m_tInstModelOutlineDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
            {
                if (FAILED(pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
                {
                    MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
                }

                if (FAILED(pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutlineColor, sizeof(_float3))))
                {
                    MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
                }

                return S_OK;
            };
    }

    if (m_bSouhScene)
    {
        m_CarPos.CreatePos = m_Translation;
        m_eCarState = CARSTATE::STATE_MOVE;
        Input_PointPos();
        Car_Root();
    }

    m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
    m_tInstModelShadowDesc.iPassIndex = 2;

    if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::SUOH_BATTLE)
        SetPhysX(false);

    return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Suoh_Dynamic::Tick(_float fTimeDelta)
{
    if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::SUOH_BATTLE)
    {
        if (m_bInitPhysX)
        {
            m_bInitPhysX = false;
            SetPhysX(true);
            Set_Gravity(true);
        }
        else if (m_bInitPhysXDelay)
        {
            m_fInitPhysXAccTime += fTimeDelta;
            if (m_fInitPhysXAccTime >= m_fInitPhysXDelay)
            {
                m_bInitPhysX = true;
                m_bInitPhysXDelay = false;
            }
        }
    }

    if (!m_bSouhScene)
    {
        __super::Tick(fTimeDelta);

        Duplicate_Model(fTimeDelta);
    }
    else
        Car_Moving(fTimeDelta);


    if (m_bSceneMapTool)
    {
        Using_Map_Tool();
    }
}

void CMap_Suoh_Dynamic::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
    static _bool bRender(true);
    if (bCheckOnce)
    if (CImGui_Manager::Get_Instance()->Is_Enable())
    {
        bCheckOnce = false;
        ImGui::Begin("Please");
        ImGui::Checkbox("CMap_Suoh_Dynamic", &bRender);
        ImGui::End();
    }
    if (!bRender) return;
#endif
   
    if (m_bDead)
        return;

    if (m_bDissolve)
    {
        Add_RenderObject(RENDER_GROUP::NONBLEND);
    }
    else
    {
        m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
        m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);
    }

    m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);

    if (m_bFocused)
    {
        //Add_RenderObject(RENDER_GROUP::OUTLINE);
        m_pModelRenderer->Add_RenderObject(RENDER_GROUP::OUTLINE, m_tInstModelOutlineDesc, m_pTransform);
        m_bFocused = false;
    }

    //if (CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
    //{
    //    Add_RenderObject(RENDER_GROUP::NONBLEND);
    //}
    //Add_RenderObject(RENDER_GROUP::SHADOW);
    
}

HRESULT CMap_Suoh_Dynamic::Render()
{
    if (m_bDissolve)
    {
        if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
        {
            return E_FAIL;
        }

        if (FAILED(m_pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
        {
            return E_FAIL;
        }

        if (FAILED(CInteractiveObject::Render(16)))
        {
            MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
        }
    }
    else
    {
        if (FAILED(CInteractiveObject::Render(0)))
        {
            MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
        }

        Mesh_Index(m_strIndexName);

        if (FAILED(m_pModel->Bind_ShaderResourceViews(m_iMeshIndex, m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
        }

        Mesh_Render(m_strIndexName);
    }
    
    return S_OK;
}

HRESULT CMap_Suoh_Dynamic::Render_OutLine()
{
    return __super::Render_OutLine();
}

HRESULT CMap_Suoh_Dynamic::Ready_Components()
{
    if (FAILED(CInteractiveObject::Ready_Components()))
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

    m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
    if (nullptr == m_pModel)
    {
        MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: Model");
    }

    m_pDissolveTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_ONE_002));
    if (nullptr == m_pDissolveTexture)
    {
        MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TEXTURE");
    }

    return S_OK;
}

void CMap_Suoh_Dynamic::Using_Map_Tool()
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

void CMap_Suoh_Dynamic::Add_ModelCom()
{
    if (m_iDynamicIndex == 0)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_CARA;
        m_vExtents = { 0.8f,0.7f,2.3f };
        m_vLocalPose = { 0.f,0.8f,0.f };
        m_fMass = 7.f;
    }
    else if (m_iDynamicIndex == 1)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_CARB;
        m_vExtents = { 0.8f,0.7f,2.3f };
        m_vLocalPose = { 0.f,0.8f,0.f };
        m_fMass = 7.f;
    }
    else if (m_iDynamicIndex == 2)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_CARC; // small truck
        m_vExtents = { 0.8f,0.8f,1.9f };
        m_vLocalPose = { 0.f,0.8f,0.f };
        m_fMass = 6.5f;
    }
    else if (m_iDynamicIndex == 3)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_CARD; //camping car
        m_vExtents = { 1.2f,1.4f,2.2f };
        m_vLocalPose = { 0.f,1.5f,0.3f };
        m_fMass = 2.f;
    }
    else
    {
        int a = 0;
    }

    m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
    Add_Component(COMPONENT::TRANSFORM, m_pTransform);
}

void CMap_Suoh_Dynamic::Add_DissolveModel()
{
    if (m_iDynamicIndex == 0)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SUOH_CARA, g_aNull);
    }
    else if (m_iDynamicIndex == 1)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SUOH_CARB, g_aNull);
    }
    else if (m_iDynamicIndex == 2)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SUOH_CARC, g_aNull);
    }
    else if (m_iDynamicIndex == 3)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SUOH_CARD, g_aNull);
    }
}

void CMap_Suoh_Dynamic::Duplicate_Model(_float fTimeDelta)
{
    auto pGameInstance = CGameInstance::Get_Instance();
    if (m_bReadyDuplicate)
    {
        m_fDuplicateTime -= fTimeDelta;
        if (m_fDuplicateTime < 0.f)
        {
            m_bReadyDuplicate = false;
            shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC);
            wstring strPrototype;

            if (m_strName == TEXT("Suoh_CarA"))
                strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARA;
            else if (m_strName == TEXT("Suoh_CarB"))
                strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARB;
            else if (m_strName == TEXT("Suoh_CarC"))
                strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARC;
            else if (m_strName == TEXT("Suoh_CarD"))
                strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_CARD;

            m_pDuplicatedObject = static_pointer_cast<CMap_Dynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), strPrototype, m_copyTuplePivot));
            pLayer->Add(m_pDuplicatedObject);
            if (!m_bStrong)
                m_pDuplicatedObject->Throw();
            else
            {
                static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_DuplicatedObject(m_pDuplicatedObject);
                m_pDuplicatedObject->SetPhysX(false);
            }
            Interactive_PhychoKinesis(true);
            m_pDuplicatedObject->Interactive_PhychoKinesis(true);
        }
    }
}

void CMap_Suoh_Dynamic::Hide_Mesh(wstring MeshName)
{
    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    if (m_strIndexName == TEXT("Suoh_CarA") || m_strIndexName == TEXT("Suoh_CarC"))
    {
        m_pModel->Hide_Mesh(2);
    }
    else if (m_strIndexName == TEXT("Suoh_CarB") || m_strIndexName == TEXT("Suoh_CarD"))
    {
        m_pModel->Hide_Mesh(1);
    }

    if (m_strIndexName == TEXT("Suoh_CarB") || m_strIndexName == TEXT("Suoh_CarD"))
    {
        m_pModel->Hide_Mesh(0);
    }
    else if (m_strIndexName == TEXT("Suoh_CarC"))
    {
        m_pModel->Hide_Mesh(1);
    }
    else if (m_strIndexName == TEXT("Suoh_CarA"))
    {
        m_pModel->Hide_Mesh(1);
    }
}

HRESULT CMap_Suoh_Dynamic::Mesh_Render(wstring MeshName)
{
    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    /* 창문 */
    if (m_strIndexName == TEXT("Suoh_CarA") || m_strIndexName == TEXT("Suoh_CarC"))
    {
        m_pModel->Render(2, m_pShader, 7);
    }
    else if (m_strIndexName == TEXT("Suoh_CarB") || m_strIndexName == TEXT("Suoh_CarD"))
    {
        m_pModel->Render(1, m_pShader, 7);
    }

    /* 뼈대 */
    if (m_strIndexName == TEXT("Suoh_CarB") || m_strIndexName == TEXT("Suoh_CarD"))
    {
        m_pModel->Render(0, m_pShader, 6);
    }
    else if (m_strIndexName == TEXT("Suoh_CarC"))
    {
        m_pModel->Render(1, m_pShader, 6);
    }
    else if (m_strIndexName == TEXT("Suoh_CarA"))
    {
        m_pModel->Render(1, m_pShader, 6);
    }

    /* 창문 */
    if (m_strIndexName == TEXT("Suoh_CarC"))
    {
        if (FAILED(m_pModel->Bind_ShaderResourceViews(2, m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
        }
        m_pModel->Render(2, m_pShader, 5);
    }

    if (m_strIndexName == TEXT("Suoh_CarC"))
    {
        if (FAILED(m_pModel->Bind_ShaderResourceViews(1, m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
        }
        m_pModel->Render(1, m_pShader, 6);
    }


    return S_OK;
}

void CMap_Suoh_Dynamic::Mesh_Index(wstring MeshName)
{
    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    //네온 매쉬 안그림
    if (m_strIndexName == TEXT("Suoh_CarA") || m_strIndexName == TEXT("Suoh_CarC"))
    {
        m_iMeshIndex = 2;
    }
    else if (m_strIndexName == TEXT("Suoh_CarB") || m_strIndexName == TEXT("Suoh_CarD"))
    {
        m_iMeshIndex = 1;
    }

    if (m_strIndexName == TEXT("Suoh_CarB") || m_strIndexName == TEXT("Suoh_CarD"))
    {
        m_iMeshIndex = 0;
    }
    else if (m_strIndexName == TEXT("Suoh_CarC"))
    {
        m_iMeshIndex = 1;
    }
    else if (m_strIndexName == TEXT("Suoh_CarA"))
    {
        m_iMeshIndex = 1;
    }
}

void CMap_Suoh_Dynamic::Car_Moving(_float fTimeDelta)
{
    if (CGameInstance::Get_Instance()->Key_Down('R'))
    {
        m_pTransform->Set_State(TRANSFORM::POSITION, m_Translation);
        m_ePointPos = POINTPOS::POS_1;
    }

    _float3 tmpDistance = _float3(0.f, 0.f, 0.f);
    _float3 CurrentPos = m_pTransform->Get_State(TRANSFORM::POSITION);

    if (m_eRoundTrip == ROUNDTRIP::ROUNDTRIP_9)
    {
        switch (m_ePointPos)
        {
        case POINTPOS::POS_1:
            m_Dir = m_CarPos.Pos[0] - m_CarPos.CreatePos;
            m_Dir.y = 0.f;
            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[0].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[0].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_ePointPos = POINTPOS::POS_2;
            }
            break;
        case POINTPOS::POS_2:
            m_Dir = m_CarPos.Pos[1] - m_CarPos.Pos[0];
            m_Dir.y = 0.f;

            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[1].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[1].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_ePointPos = POINTPOS::POS_3;
            }
            break;
        case POINTPOS::POS_3:
            m_Dir = m_CarPos.Pos[2] - m_CarPos.Pos[1];
            m_Dir.y = 0.f;
            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[2].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[2].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_ePointPos = POINTPOS::POS_4;
            }
            break;
        case POINTPOS::POS_4:
            m_Dir = m_CarPos.Pos[3] - m_CarPos.Pos[2];
            m_Dir.y = 0.f;
            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[3].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[3].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_ePointPos = POINTPOS::POS_5;
            }
            break;
        case POINTPOS::POS_5:
            m_Dir = m_CarPos.Pos[4] - m_CarPos.Pos[3];
            m_Dir.y = 0.f;
            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[4].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[4].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_ePointPos = POINTPOS::POS_6;
            }
            break;
        case POINTPOS::POS_6:
            m_Dir = m_CarPos.Pos[5] - m_CarPos.Pos[4];
            m_Dir.y = 0.f;
            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[5].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[5].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_ePointPos = POINTPOS::POS_7;
            }
            break;
        case POINTPOS::POS_7:
            m_Dir = m_CarPos.Pos[6] - m_CarPos.Pos[5];
            m_Dir.y = 0.f;
            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[6].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[6].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_ePointPos = POINTPOS::POS_8;
            }
            break;
        case POINTPOS::POS_8:
            m_Dir = m_CarPos.Pos[7] - m_CarPos.Pos[6];
            m_Dir.y = 0.f;
            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[7].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[7].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_ePointPos = POINTPOS::POS_9;
            }
            break;
        case POINTPOS::POS_9:
            m_Dir = m_CarPos.Pos[8] - m_CarPos.Pos[7];
            m_Dir.y = 0.f;
            tmpDistance.x = abs(CurrentPos.x - m_CarPos.Pos[8].x);
            tmpDistance.z = abs(CurrentPos.z - m_CarPos.Pos[8].z);
            if (tmpDistance.x <= 1.5f && tmpDistance.z <= 1.5f)
            {
                m_pTransform->Set_State(TRANSFORM::POSITION, m_CarPos.CreatePos);
                m_ePointPos = POINTPOS::POS_1;
            }
            break;
        default:
            break;
        }
    }

    if (m_eCarState == CARSTATE::STATE_MOVE)
    {
        m_pTransform->Set_State(TRANSFORM::LOOK, m_Dir);
        m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * m_fSpeed);
    }
}

void CMap_Suoh_Dynamic::Input_PointPos()
{
    // rootA
    if (m_strNameNum == TEXT("Suoh_CarB_number0"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 7.f;
        m_eCarRoot = CARROOT::ROOT_A;
    }
    if (m_strNameNum == TEXT("Suoh_CarC_number0"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 7.5f;
        m_eCarRoot = CARROOT::ROOT_A;
    }
    if (m_strNameNum == TEXT("Suoh_CarA_number0"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 8.f;
        m_eCarRoot = CARROOT::ROOT_A;
    }
    if (m_strNameNum == TEXT("Suoh_CarD_number0"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 8.3f;
        m_eCarRoot = CARROOT::ROOT_A;
    }
    // rootB
    if (m_strNameNum == TEXT("Suoh_CarA_number1"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 10.f;
        m_eCarRoot = CARROOT::ROOT_B;
    }
    if (m_strNameNum == TEXT("Suoh_CarB_number1"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 10.3f;
        m_eCarRoot = CARROOT::ROOT_B;
    }
    if (m_strNameNum == TEXT("Suoh_CarC_number1"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 10.6f;
        m_eCarRoot = CARROOT::ROOT_B;
    }
    if (m_strNameNum == TEXT("Suoh_CarD_number1"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 10.9f;
        m_eCarRoot = CARROOT::ROOT_B;
    }

    // rootC
    if (m_strNameNum == TEXT("Suoh_CarC_number2"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 7.f;
        m_eCarRoot = CARROOT::ROOT_C;
    }
    if (m_strNameNum == TEXT("Suoh_CarA_number2"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 8.3f;
        m_eCarRoot = CARROOT::ROOT_C;
    }
    if (m_strNameNum == TEXT("Suoh_CarD_number2"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 7.6f;
        m_eCarRoot = CARROOT::ROOT_C;
    }
    if (m_strNameNum == TEXT("Suoh_CarB_number2"))
    {
        m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_9;
        m_fSpeed = 7.9f;
        m_eCarRoot = CARROOT::ROOT_C;
    }
}

void CMap_Suoh_Dynamic::Car_Root()
{
    if (m_eCarRoot == CARROOT::ROOT_A)
    {
        m_CarPos.Pos[0] = _float3(-70.f, 0.f, 104.3f);
        m_CarPos.Pos[1] = _float3(-46.f, 0.f, 98.f);
        m_CarPos.Pos[2] = _float3(-5.f, 0.f, 100.f);
        m_CarPos.Pos[3] = _float3(-4.7f, 0.f, 282.f);
        m_CarPos.Pos[4] = _float3(-14.8f, 0.f, 326.f);
        m_CarPos.Pos[5] = _float3(-87.f, 0.f, 456.f);
        m_CarPos.Pos[6] = _float3(-103.f, 0.f, 500.f);
        m_CarPos.Pos[7] = _float3(-97.f, 0.f, 546.f);
        m_CarPos.Pos[8] = _float3(-72.f, 0.f, 576.f);
    }
    if (m_eCarRoot == CARROOT::ROOT_B)
    {
        m_CarPos.Pos[0] = _float3(-57.9f, 0.f, 96.6f);
        m_CarPos.Pos[1] = _float3(-43.3f, 0.f, 95.f);
        m_CarPos.Pos[2] = _float3(-2.f, 0.f, 96.f);
        m_CarPos.Pos[3] = _float3(-1.7f, 0.f, 282.f);
        m_CarPos.Pos[4] = _float3(-13.5f, 0.f, 326.f);
        m_CarPos.Pos[5] = _float3(-81.8f, 0.f, 456.f);
        m_CarPos.Pos[6] = _float3(-98.f, 0.f, 500.f);
        m_CarPos.Pos[7] = _float3(-94.f, 0.f, 546.f);
        m_CarPos.Pos[8] = _float3(-64.f, 0.f, 576.f);
    }
    if (m_eCarRoot == CARROOT::ROOT_C)
    {
        m_CarPos.Pos[0] = _float3(-81.f, 0.f, 554.f);
        m_CarPos.Pos[1] = _float3(-95.f, 0.f, 501.f);
        m_CarPos.Pos[2] = _float3(-67.f, 0.f, 441.f);
        m_CarPos.Pos[3] = _float3(-5.f, 0.f, 328.f);
        m_CarPos.Pos[4] = _float3(8.f, 0.f, 266.f);
        m_CarPos.Pos[5] = _float3(7.5f, 0.f, 102.f);
        m_CarPos.Pos[6] = _float3(56.f, 0.f, 100.f);
        m_CarPos.Pos[7] = _float3(94.7f, 0.f, 76.f);
        m_CarPos.Pos[8] = _float3(110.4f, 0.f, 33.3f);
    }
}


shared_ptr<CMap_Suoh_Dynamic> CMap_Suoh_Dynamic::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
    shared_ptr<CMap_Suoh_Dynamic> pInstance = make_private_shared(CMap_Suoh_Dynamic, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype(iIndex)))
    {
        MSG_RETURN(nullptr, "CMap_Suoh_Dynamic::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CMap_Suoh_Dynamic::Clone(any pArg)
{
    shared_ptr<CMap_Suoh_Dynamic> pInstance = make_private_shared_copy(CMap_Suoh_Dynamic, *this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_RETURN(nullptr, "CMap_Suoh_Dynamic::Clone", "Failed to Initialize");
    }

    return pInstance;
}
