#include "EnginePCH.h"
#include "PhysXEngine.h"
#include "GameInstance.h"
#include "Renderer.h"
#include "Scene_Manager.h"

CPhysXEngine::CPhysXEngine()
{
}

CPhysXEngine::~CPhysXEngine()
{
    gManager->purgeControllers();
    
    PX_RELEASE(gManager)
        //   PX_RELEASE(gCooking)
        PX_RELEASE(gMaterial)
        PX_RELEASE(gScene);
    PX_RELEASE(gDispatcher);
    PX_RELEASE(gPhysics);
    if (gPvd) {
        PxPvdTransport* transport = gPvd->getTransport();
        gPvd->release();
        gPvd = NULL;
        PX_RELEASE(transport);
    }
    PX_RELEASE(gFoundation);
}

PxFilterFlags FilterShaderExample(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;

    return PxFilterFlag::eCALLBACK;
}

HRESULT CPhysXEngine::Initialize()
{
    /*shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
    
    m_pEffect = make_shared<BasicEffect>(pGameInstance->Get_Device().Get());
    m_pBatch = make_shared<PrimitiveBatch<VertexPositionColor>>(pGameInstance->Get_Context().Get());

    const void* pShaderByteCode = nullptr;
    size_t		nShaderByteCodeLength = 0;
    m_pEffect->SetVertexColorEnabled(true);
    m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &nShaderByteCodeLength);
    
    if (FAILED(pGameInstance->Get_Device()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
        pShaderByteCode, nShaderByteCodeLength, m_pInputLayout.GetAddressOf())))
    {
        MSG_RETURN(E_FAIL, "CPhysx::Initialize_Prototype", "Failed to CreateInputLayout");
    }*/

    gFoundation =
        PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport =
        PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
        PxTolerancesScale(), true, gPvd);


    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f * 3.f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    //sceneDesc.filterShader = FilterShaderExample;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    //gSimulationFilterCallBack = make_shared<CSimulationFilterCallBack>();
    //sceneDesc.filterCallback = gSimulationFilterCallBack.get();


    gScene = gPhysics->createScene(sceneDesc);

    gScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
    
#if ACTIVATE_PHYSX_DEBUG
	//gScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	//gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
#endif
    //gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_DYNAMIC, 1.0f);

    PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
    if (pvdClient) {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES,
            true);
    }
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);

    gManager = PxCreateControllerManager(*gScene);

  /*  PxReal halfExtent = 1;
    PxU32 size = 25;
    PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
    for (PxU32 i = 0; i < size; i++)
    {
        for (PxU32 j = 0; j < size - i; j++)
        {
            PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
            PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(PxVec3(0,0,0)).transform(localTm));
            body->attachShape(*shape);
            PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
            gScene->addActor(*body);
        }
    }
    shape->release();*/

    return S_OK;
}

HRESULT CPhysXEngine::SetVIBuffer()
{
    Tick(0.1f);
    const PxRenderBuffer& rb = gScene->getRenderBuffer();
    
    if (rb.empty())
        return S_OK;
    PxU32 size = rb.getNbLines();
     

    shared_ptr<CGameInstance> pGameinstance = CGameInstance::Get_Instance();
    if (nullptr == (m_pVIBuffer_LineList = dynamic_pointer_cast<CVIBuffer_LineList>(CVIBuffer_LineList::Create(pGameinstance->Get_Device(), pGameinstance->Get_Context(), size))))
    {
        MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE");
    }
    if (nullptr == (m_pShader = dynamic_pointer_cast<CShader>(CComponent_Manager::Get_Instance()->Clone_Component(SCENE(0), TEXT("Prototype:Component:Shader:Pos")))))
    {
        MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Component_Prototype: Shader");
    }
    return S_OK;
}

HRESULT CPhysXEngine::SetVIBuffer(const PxRenderBuffer& rb)
{
    PxU32 size = rb.getNbLines();


    shared_ptr<CGameInstance> pGameinstance = CGameInstance::Get_Instance();
    if (nullptr == (m_pVIBuffer_LineList = dynamic_pointer_cast<CVIBuffer_LineList>(CVIBuffer_LineList::Create(pGameinstance->Get_Device(), pGameinstance->Get_Context(), size))))
    {
        MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE");
    }
    if (nullptr == (m_pShader = dynamic_pointer_cast<CShader>(CComponent_Manager::Get_Instance()->Clone_Component(SCENE(0), TEXT("Prototype:Component:Shader:Pos")))))
    {
        MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Component_Prototype: Shader");
    }
    return S_OK;
}

void CPhysXEngine::Tick(_float fTimeDelta)
{
    if (m_fTimeScale > 0.f)
        gScene->simulate(fTimeDelta * m_fTimeScale);
    gScene->fetchResults(true);
#ifdef _DEBUG
    if(m_bDebugRender)
    {
        if (CGameInstance::Get_Instance()->Key_Hold('0'))
        {
            gScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
            gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
            UpdatePoints();
        }
        m_pRenderer->Add_RenderObject(RENDER_GROUP::DEBUG, shared_from_this());
    }
#endif // _DEBUG
}

#ifdef _DEBUG

HRESULT CPhysXEngine::UpdatePoints()
{
    if (nullptr == m_pShader)
        return S_OK;
    const PxRenderBuffer& rb = gScene->getRenderBuffer();
    if (rb.empty())
        return S_OK;
    _uint IndexNum = m_pVIBuffer_LineList->Get_NumVertices();
    shared_ptr<_float3[]> NewIndices = Function::CreateDynamicArray<_float3>(IndexNum);
    //PxU32 size = rb.getNbPoints();
    PxU32 size = rb.getNbLines();
    if (size * 2 > IndexNum)
    {
        if (FAILED(SetVIBuffer(rb)))
            return S_OK;
        IndexNum = m_pVIBuffer_LineList->Get_NumVertices();
        NewIndices = Function::CreateDynamicArray<_float3>(IndexNum);
    }
    _uint IndexCount = 0;
    for (PxU32 i = 0; i < size; ++i)
    {
        const PxDebugLine& line = rb.getLines()[i];
        // Render the Point
        //NewIndices[IndexCount++] = _float3(line.pos0.x, line.pos0.y, line.pos0.z);
        // render the line
        if (IndexCount >= IndexNum)
            break;
        NewIndices[IndexCount++] = _float3(line.pos0.x, line.pos0.y, line.pos0.z);
        NewIndices[IndexCount++] = _float3(line.pos1.x, line.pos1.y, line.pos1.z);
    }
    m_pVIBuffer_LineList->Update(NewIndices.get());

    return S_OK;
}

HRESULT CPhysXEngine::Render()
{
    if (nullptr == m_pShader)
        SetVIBuffer();
    
    m_pShader->Bind_Matrix("g_mWorld", XMMatrixIdentity());
    m_pShader->Bind_Matrix("g_mView", CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW));
    m_pShader->Bind_Matrix("g_mProj", CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION));
    
    m_pVIBuffer_LineList->Render(m_pShader, 0);
    

    return S_OK;
}
#endif // DEBUG

void CPhysXEngine::Clear()
{
    std::vector<PxRigidActor*> actorsToDelete;

    if(nullptr!=gManager)
     gManager->purgeControllers();
    
    if(nullptr!=gScene)
    {
        PxU32 numActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC);
        if(numActors>0)
        {
            actorsToDelete.resize(numActors);
            gScene->getActors(PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC, reinterpret_cast<PxActor**>(&actorsToDelete[0]), numActors);

            for (PxU32 i = 0; i < numActors; ++i) {
                gScene->removeActor(*actorsToDelete[i]);
                actorsToDelete[i]->release();
            }
        }
    }
}

void CPhysXEngine::Free()
{
  
}

PxController* CPhysXEngine::CreateController(PxCapsuleControllerDesc* desc)
{
    return gManager->createController(*desc);
}

#ifdef _DEBUG
HRESULT CPhysXEngine::SetRenderer()
{
    m_pRenderer = dynamic_pointer_cast<CRenderer>(CComponent_Manager::Get_Instance()->Clone_Component((SCENE)0, TEXT("Component:Prototype:Renderer")));
    if (nullptr == m_pRenderer)
    {
        MSG_RETURN(E_FAIL, "CPhysx::Initialize_Prototype", "Failed to Import Renderer");
    }
    return S_OK;
}
#endif // _DEBUG


CSimulationFilterCallBack::CSimulationFilterCallBack()
{
}

PxFilterFlags	CSimulationFilterCallBack::pairFound(PxU64 /*pairID*/,
    PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/, const PxActor* a0, const PxShape* s0,
    PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/, const PxActor* a1, const PxShape* s1,
    PxPairFlags& pairFlags)
{
    ACTORDESC* desc1 = (ACTORDESC*)(a0->userData);
  	ACTORDESC* desc2 = (ACTORDESC*)(a1->userData);
  
    if(nullptr!=desc1&&nullptr!=desc2)
    {
        if (desc1->strTag == ACTORTAG::INTERACTIVE &&
            desc2->strTag == ACTORTAG::INTERACTIVE)
        {
            return PxFilterFlag::eSUPPRESS;
        }
    }
  	else
  		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
  
    return PxFilterFlag::eDEFAULT;
}

void	CSimulationFilterCallBack::pairLost(PxU64 /*pairID*/,
    PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/,
    PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/,
    bool /*objectRemoved*/)
{

}

bool	CSimulationFilterCallBack::statusChange(PxU64& /*pairID*/, PxPairFlags& /*pairFlags*/, PxFilterFlags& /*filterFlags*/)
{
    return false;
}