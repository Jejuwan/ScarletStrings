#include "ClientPCH.h"
#include "Map_MindRoom_Dynamic.h"
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

#include "Effect_Manager.h"
#include <regex>

CMap_MindRoom_Dynamic::CMap_MindRoom_Dynamic(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CMap_Dynamic(_pDevice, _pContext)
{
}

CMap_MindRoom_Dynamic::CMap_MindRoom_Dynamic(const CMap_MindRoom_Dynamic& _rhs)
    : CMap_Dynamic(_rhs)
    , m_iDynamicIndex(_rhs.m_iDynamicIndex)
{
}

HRESULT CMap_MindRoom_Dynamic::Initialize_Prototype(_uint iIndex)
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL) | BIT(COMPONENT::TRANSFORM);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
    m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX, g_aNull);

    m_iDynamicIndex = iIndex;
    Add_DissolveModel();

    return S_OK;
}

HRESULT CMap_MindRoom_Dynamic::Initialize(any pArg)
{
    m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    m_tMaterialDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    m_tMaterialDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
    m_tMaterialDesc.vEmissive = _float4(0.f, 0.f, 0.f, 1.f);
    m_tMaterialDesc.fShininess = 64.f;

    if (FAILED(__super::Initialize(pArg)))
    {
        MSG_RETURN(E_FAIL, "CMap_MindRoom_Dynamic::Initialize", "Failed to CGameObject::Initialize");
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
    if (std::get<0>(tObjTuple) == SCENE::MINDROOM)
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
    m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

    m_fMass *= 1.5f;
    m_fThrowForce = 50000.f;
    PxRigidBodyExt::updateMassAndInertia(*m_pActor, m_fMass);
    pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);
    m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
    // Hide_Mesh
    {
        if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA ||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB)
        {
            //m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
        }

        m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
        m_tInstModelDesc.iPassIndex = 0;
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
            if (FAILED(CGameInstance::Get_Instance()->Bind_Reflection(pShader)))
            {
                MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Reflection");
            }
            _float4 vCamPosition = *(_float4*)CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).m[3];
            vCamPosition.w = 1.f;
            if (FAILED(pShader->Bind_RawValue(SHADER_CAMPOS, &vCamPosition, sizeof(_float4))))
            {
                MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_CamPosition");
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

        if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA ||
            m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB)
        {
            m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
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

                    //if (FAILED(pModel->Bind_ShaderResourceViews(1, pShader)))
                    //{
                    //    MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
                    //}

                    return S_OK;
                };

            m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
            m_tInstModelMeshDesc.vecPassIndex.push_back(6);
        }
    }

    if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA ||
        m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB)
    {
        m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
        m_tInstModelMeshDesc.vecPassIndex.push_back(6);
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


    PxSetGroup(*m_pActor, (_uint)ACTORTAG::INTERACTIVE);
    m_ActorDesc.strTag = ACTORTAG::INTERACTIVE;
    m_ActorDesc.bThrow = false;
    m_ActorDesc.bThrowHit = false;
    m_ActorDesc.bActive = false;
    m_ActorDesc.stAttackDesc.iDamage = 30.f ;
    m_pActor->userData = &m_ActorDesc;

//    m_pTransform->Set_Scale(XMVectorSet(10.f, 10.f, 10.f, 1.f));
    m_bCalculateScreenDistance = false;

    m_ThrowHitTime = 0.07f;

    return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_MindRoom_Dynamic::Tick(_float fTimeDelta)
{
    auto pGameInstance = CGameInstance::Get_Instance();
    //__super::Tick(fTimeDelta);

    if(m_bRot)
        m_pTransform->Rotate(TRANSFORM::RIGHT, fTimeDelta);
    if (nullptr != m_pActor && m_pActor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)
    {
        _float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
        _float4 q = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());

        PxTransform p;
        p.p = PxVec3(vPos.x, vPos.y, vPos.z);
        p.q = PxQuat(q.x, q.y, q.z, q.w);

        m_pActor->setKinematicTarget(p);
    }
    else
    {
        if (m_bActivePhysX)
        {
            if (nullptr != m_pActor)
            {
                //PxMat44 worldMat(m_pActor->getGlobalPose());
                m_pActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);

                _float4 q = { m_pActor->getGlobalPose().q.x,m_pActor->getGlobalPose().q.y,m_pActor->getGlobalPose().q.z,m_pActor->getGlobalPose().q.w };
                _float4 p = { m_pActor->getGlobalPose().p.x,m_pActor->getGlobalPose().p.y,m_pActor->getGlobalPose().p.z,1.f };
                _float4x4 rotmat = XMMatrixRotationQuaternion(q);

                _float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
                _float3 OriginScale = m_pTransform->Get_Scale();
                m_pTransform->Set_Matrix(rotmat);
                m_pTransform->Set_State(TRANSFORM::POSITION, _float4(p.x, p.y, p.z, 1.f));
                m_pTransform->Set_Scale(OriginScale);
            }
        }
        else
        {
            if (nullptr != m_pActor)
            {
                //PxMat44 worldMat(m_pActor->getGlobalPose());


                _float4 q = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());
                _float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

                PxTransform px;
                px.p = PxVec3(vPos.x, vPos.y, vPos.z);
                px.q = PxQuat(q.x, q.y, q.z, q.w);

                m_pActor->setGlobalPose(px);
            }
        }
    }

    if (m_ActorDesc.bThrowHit && m_ActorDesc.bActive && m_bLagEnable)
    {
        _float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
        pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
        pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
        pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
        pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
        pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.3f);
        m_bThrowHit = true;
        m_bLagEnable = false;
        static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_Camera()->Set_Shake_NonDelta(0.3f, 5.f);
    }


    if (nullptr != m_pActor && m_pActor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)
    {
        if (m_bThrowHit)
        {
            m_ThrowHitTime -= fTimeDelta;
            //cout << m_ThrowHitTime << endl;

            if (m_ThrowHitTime < 0.f)
            {
                m_bThrowHit = false;
                m_ThrowHitTime = 0.07f;
                _float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
                pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale);
                pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
                pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale);
                pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale);
                pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale);
            }
        }
    }


    Duplicate_Model(fTimeDelta);

    if (m_bSceneMapTool)
        Using_Map_Tool();

    if (nullptr != m_pActor)
    {
        ACTORDESC* pActorDesc = (ACTORDESC*)m_pActor->userData;
        if (pActorDesc != nullptr && pActorDesc->bThrowHit && m_bThrow)
        {
            if (!m_bExecuted[0])
            {
                m_bExecuted[0] = true;
                ThrowHit();
            }
        }

        if (nullptr != m_pActor && !(m_pActor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC))
        {
            m_ActorDesc.stAttackDesc.iDamage = 10.f;
            if (m_bThrow)
            {
                m_fThrowTime -= fTimeDelta;
                if (m_fThrowTime < 1.5f)

                {
                    SetPhysX(true);
                    Set_Gravity(true);
                }
                if (m_fThrowTime < 0.f)
                {
                       m_bDissolve = true;
                       m_bInteractivable = false;

                       m_iPassIndex = 16;
                       m_iGlassPassIndex = 16;
                }
            }

            if (m_bThrowHit)
            {
                if (!m_bExecuted[55])
                {
                    m_bExecuted[55] = true;
                    m_vThrowLook = _float3(Function::RandomFloat(-1.f, 1.f), Function::RandomFloat(-1.f, 0.f), Function::RandomFloat(-1.f, 1.f));
                }
                m_ThrowHitTime -= fTimeDelta;

                if (m_ThrowHitTime > 0.f)
                {
                    m_pTransform->Translate(_float3(-m_vThrowLook.x * fTimeDelta * 30.f, -m_vThrowLook.y * fTimeDelta * 30.f, -m_vThrowLook.z * fTimeDelta * 30.f));
                    m_pTransform->Rotate(m_vThrowLook, fTimeDelta * 2.f);
                }

                if (m_ThrowHitTime < 0.f && !m_bExecuted[1])
                {
                    m_bExecuted[1] = true;
                    SetPhysX(true);
                    Set_Gravity(true);
                    if (nullptr != m_pActor)
                    {
                        m_pActor->addForce(PxVec3(-m_vThrowLook.x * m_fThrowForce * 0.25f, -m_vThrowLook.y * m_fThrowForce * 0.25f, -m_vThrowLook.z * m_fThrowForce * 0.25f));
                        m_pActor->addTorque(PxVec3(5, 5, 5));
                    }
                    _float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
                    pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale);
                    pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
                    pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale);
                    pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale);
                    pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale);

                    if (m_pDuplicatedObject)
                    {
                        m_pDuplicatedObject->SetPhysX(true);
                        m_pDuplicatedObject->Set_Gravity(true);
                        m_pDuplicatedObject->Throw();
                    }
                }
                if (m_ThrowHitTime < -0.3f)
                {
                    m_bDissolve = true;
                    m_bInteractivable = false;
                    Interactive_PhychoKinesis(false);
                    m_iPassIndex = 16;
                    m_iGlassPassIndex = 16;
                }
            }
        }
    }

    if (m_bDissolve)
    {
        m_fDissolveTime += fTimeDelta * 0.5f;

        if (m_fDissolveTime >= 1.f)
        {
            m_bAlive = false;
            CGameInstance::Get_Instance()->Erase_Light(m_pLight);
        }
    }
}

void CMap_MindRoom_Dynamic::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
    static _bool bRender(true);
    if (bCheckOnce)
    if (CImGui_Manager::Get_Instance()->Is_Enable())
    {
        bCheckOnce = false;
        ImGui::Begin("Please");
        ImGui::Checkbox("CMap_MindRoom_Dynamic", &bRender);
        ImGui::End();
    }
    if (!bRender) return;
#endif
   
    if (m_bDead)
        return;
    Add_RenderObject(RENDER_GROUP::NONBLEND);
    return;
    if (m_bDissolve)
    {
        Add_RenderObject(RENDER_GROUP::NONBLEND);
    }
    else
    {
        //m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
        //m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);
    }

    //m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);

    if (m_bFocused)
    {
        //Add_RenderObject(RENDER_GROUP::OUTLINE);
        //m_pModelRenderer->Add_RenderObject(RENDER_GROUP::OUTLINE, m_tInstModelOutlineDesc, m_pTransform);
        m_bFocused = false;
    }

    //if (CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
    //{
    //    Add_RenderObject(RENDER_GROUP::NONBLEND);
    //}
    //Add_RenderObject(RENDER_GROUP::SHADOW);
    
}

HRESULT CMap_MindRoom_Dynamic::Render()
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
        if (FAILED(CInteractiveObject::Render(6)))
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

HRESULT CMap_MindRoom_Dynamic::Render_OutLine()
{
    return __super::Render_OutLine();
}

HRESULT CMap_MindRoom_Dynamic::Ready_Components()
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

    //m_pModelRenderer = Get_Component<CModelRenderer>(COMPONENT::MODEL_RENDERER);
    //if (nullptr == m_pModelRenderer)
    //{
    //    MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: MODEL_RENDERER");
    //}

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

void CMap_MindRoom_Dynamic::Using_Map_Tool()
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

void CMap_MindRoom_Dynamic::Add_ModelCom()
{
    if (m_iDynamicIndex == 0)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX;
        m_vExtents = { 0.5f,0.5f,0.5f };
        m_vLocalPose = { 0.f, 0.4f, 0.f };
        m_fMass = 3.f;
    }
    if (m_iDynamicIndex == 1)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA;
        m_vExtents = { 1.6f , 0.6f, 0.6f };
        m_vLocalPose = { 0,0.5f,0 };
        m_fMass = 0.4f;
    }
    if (m_iDynamicIndex == 2)
    {
        m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB;
        m_vExtents = { 0.6f, 0.8f, 0.6f };
        m_vLocalPose = { 0,0.6f,0 };
        m_fMass = 2.f;
    }
    

    m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
    if (nullptr == m_pTransform)
    {
        MSG_RETURN(, "CMonster_Statue::Ready_Components", "Failed to Get_Component: TRANSFORM");
    }

}

void CMap_MindRoom_Dynamic::Add_DissolveModel()
{
    if (m_iDynamicIndex == 0)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX, g_aNull);
    }
    if (m_iDynamicIndex == 1)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA, g_aNull);
    }
    if (m_iDynamicIndex == 2)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB, g_aNull);
    }
}

void CMap_MindRoom_Dynamic::Duplicate_Model(_float fTimeDelta)
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
            if (m_strName == TEXT("Mindroom_Box"))
                strPrototype = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX;
            if (m_strName == TEXT("Mindroom_CrateA"))
                strPrototype = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA;
            if (m_strName == TEXT("Mindroom_CrateB"))
                strPrototype = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB;
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

void CMap_MindRoom_Dynamic::Hide_Mesh(wstring MeshName)
{
    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    if (m_strIndexName == TEXT("Mindroom_Box")||
        m_strIndexName == TEXT("Mindroom_CrateA")||
        m_strIndexName == TEXT("Mindroom_CrateB"))
    {
        m_pModel->Hide_Mesh(0);
    }
}

HRESULT CMap_MindRoom_Dynamic::Mesh_Render(wstring MeshName)
{
    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    /* 창문 */
    if (m_strIndexName == TEXT("Mindroom_Box") ||
        m_strIndexName == TEXT("Mindroom_CrateA") ||
        m_strIndexName == TEXT("Mindroom_CrateB"))
    {
        if (FAILED(m_pModel->Bind_ShaderResourceViews(0, m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
        }
        m_pModel->Render(0, m_pShader, 6);

    }

    return S_OK;
}

void CMap_MindRoom_Dynamic::Mesh_Index(wstring MeshName)
{
    std::wregex pattern(L"_number\\d+");
    m_strIndexName = regex_replace(MeshName, pattern, L"");

    if (m_strIndexName == TEXT("Mindroom_Box") ||
        m_strIndexName == TEXT("Mindroom_CrateA") ||
        m_strIndexName == TEXT("Mindroom_CrateB"))
    {
        m_iMeshIndex = 0;
    }
}

shared_ptr<CMap_MindRoom_Dynamic> CMap_MindRoom_Dynamic::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
    shared_ptr<CMap_MindRoom_Dynamic> pInstance = make_private_shared(CMap_MindRoom_Dynamic, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype(iIndex)))
    {
        MSG_RETURN(nullptr, "CMap_MindRoom_Dynamic::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CMap_MindRoom_Dynamic::Clone(any pArg)
{
    shared_ptr<CMap_MindRoom_Dynamic> pInstance = make_private_shared_copy(CMap_MindRoom_Dynamic, *this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_RETURN(nullptr, "CMap_MindRoom_Dynamic::Clone", "Failed to Initialize");
    }

    return pInstance;
}
