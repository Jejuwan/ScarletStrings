#include "physx/PxPhysicsAPI.h"
#include "Engine_Define.h"
#include "System.h"

using namespace physx;

BEGIN(Engine)
class CPhysXEngine : public CSingleton<CPhysXEngine>, public ISystem
{
public:
    CPhysXEngine();
    virtual ~CPhysXEngine();
    
    HRESULT Initialize();
    HRESULT SetVIBuffer();
    HRESULT SetVIBuffer(const PxRenderBuffer& rb);
    void Tick(_float fTimeDelta);
    void Clear();
    void Free();

    PxController* CreateController(PxCapsuleControllerDesc* desc);
#ifdef _DEBUG
    HRESULT UpdatePoints();
    HRESULT Render();
    HRESULT SetRenderer();
    _bool GetDrawStatus() { return m_bDebugRender; }
    void SetDrawStatus(_bool Render)
    {
        m_bDebugRender = Render;
    }
#endif // _DEBUG

private:
    PxDefaultAllocator gAllocator;
    PxDefaultErrorCallback gErrorCallback;
    PxFoundation* gFoundation = NULL;
    PxPhysics* gPhysics = NULL;
    PxDefaultCpuDispatcher* gDispatcher = NULL;
    PxScene* gScene = NULL;
    PxMaterial* gMaterial = NULL;
    PxPvd* gPvd = NULL;
    //PxCooking* gCooking = NULL;
    PxControllerManager* gManager = NULL;
    PxObstacleContext* gObstacleContext = NULL;

    shared_ptr<class CSimulationFilterCallBack> gSimulationFilterCallBack = NULL;

    //ComPtr<ID3D11InputLayout>							m_pInputLayout;
    //shared_ptr<PrimitiveBatch<VertexPositionColor>>		m_pBatch;
    //shared_ptr<BasicEffect>								m_pEffect;

    shared_ptr<class CRenderer>                             m_pRenderer;
    shared_ptr<class CVIBuffer_LineList>                    m_pVIBuffer_LineList;
    shared_ptr<class CShader>                               m_pShader;
    _bool m_bDebugRender = false;

    _float m_fTimeScale = 1.f;
public:
    PxPhysics* GetPhysics() { return gPhysics; }
    PxScene* GetScene() { return gScene; }
    PxMaterial* GetMaterial() { return gMaterial; }
    //PxCooking* GetCooking() { return gCooking; }
    PxObstacleContext* GetObstacleContext() { return gObstacleContext; }
    PxControllerManager* GetControllerManager() { return gManager; }


    void Set_TimeScale(_float f) { m_fTimeScale = f; }
    _float Get_TimeScale() { return m_fTimeScale; }

    friend CSingleton<CPhysXEngine>;
};

class CSimulationFilterCallBack :
    public PxSimulationFilterCallback
{
public:
    CSimulationFilterCallBack();
    ~CSimulationFilterCallBack() = default;
    virtual		PxFilterFlags	pairFound(PxU64 /*pairID*/,
        PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/, const PxActor* a0, const PxShape* s0,
        PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/, const PxActor* a1, const PxShape* s1,
        PxPairFlags& pairFlags)	PX_OVERRIDE;

    virtual		void	pairLost(PxU64 /*pairID*/,
        PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/,
        PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/,
        bool /*objectRemoved*/)	PX_OVERRIDE;

    virtual		bool	statusChange(PxU64& /*pairID*/, PxPairFlags& /*pairFlags*/, PxFilterFlags& /*filterFlags*/)	PX_OVERRIDE;
};

END

