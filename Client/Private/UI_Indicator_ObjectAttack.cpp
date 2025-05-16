#include "ClientPCH.h"
#include "UI_Indicator_ObjectAttack.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "InteractiveObject.h"

CUI_Indicator_ObjectAttack::CUI_Indicator_ObjectAttack(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_ObjectAttack::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_ObjectAttack::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    else
    {
        //m_fGauge            = 0.f;
        //m_vPosition_Gauge   = _float4(-800.f, 0.f, 0.f, 1.f);
    }
    

    _matrix PivotMatrix = XMMatrixIdentity();
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-96.5f));

    
    // Indicator
    m_vScale_Indicator      = _float3(64.f, 64.f, 1.f);
    m_vPosition_Indicator   = _float4(-900.f, 0.f, 0.4f, 1.f);

    // Gauge
    m_vScale_Gauge          = _float3(67.5f ,67.5f, 1.f);
    m_vPosition_Gauge       = _float4(-800.f, 0.f, 0.f, 1.f);

    /* Transform */
    m_pIndicator            = CTransform::Create(m_pDevice, m_pContext);
    m_pGauge                = CTransform::Create(m_pDevice, m_pContext);

    /* Texture */
    m_pTexture_Indicator    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_R.png"));
    m_pTexture_Gauge        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/PsycoCoolOn.png"));

    m_pGauge->Set_Matrix(PivotMatrix);
    m_pIndicator->Set_Scale(CUI_Manager::Get_Instance()->Get_IndicatorScale());
    m_pGauge->Set_Scale(m_vScale_Gauge);

    m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pGauge->Set_State(TRANSFORM::POSITION, m_vPosition_Gauge);



    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }
    
    return S_OK;
}

void CUI_Indicator_ObjectAttack::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_ObjectAttack::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);

    //else if (CGameInstance::Get_Instance()->Key_Up(VK_RBUTTON))
    //{
    //    Set_Indicator_DefaultState(fTimeDelta);
    //}

    //Ready_PsycoAttack(fTimeDelta);
}

HRESULT CUI_Indicator_ObjectAttack::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (SCENE::CUTSCENE_MIZUHA != CGameInstance::Get_Instance()->Current_Scene()
            && SCENE::HIDEOUT != CGameInstance::Get_Instance()->Current_Scene()
            && SCENE::SUOH != CGameInstance::Get_Instance()->Current_Scene()
            && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene()
            && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
            && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
            && false == CUI_Manager::Get_Instance()->Get_RenderJW())
        {
            if (true == m_bRenderSwitch && true == m_bRenderDegree)
            {
                if (FAILED(m_pTexture_Indicator->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pIndicator->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fRatio, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
                }

                if (FAILED(m_pTexture_Gauge->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGauge->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(12)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
                }
            }
        }


        //m_bRenderSwitch = false;
    }

    return S_OK;
}

void CUI_Indicator_ObjectAttack::Set_Ratio(_float fRatio)
{
    if (0.f > fRatio || 1.5f < fRatio)
        return;

    _float _fRatio = fRatio / 1.5f;

    m_fRatio = _fRatio;
}

void CUI_Indicator_ObjectAttack::Set_Indicator_DefaultState(_float fTimeDelta)
{
    m_fRatio -= fTimeDelta;

    if (0.f >= m_fRatio)
        m_fRatio = 0.f;
}

void CUI_Indicator_ObjectAttack::Ready_PsycoAttack(shared_ptr<CInteractiveObject> pFocusedObject, _float fTimeDelta)
{
    if (pFocusedObject)
    {
        Set_IndicatorPosition(pFocusedObject->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION), 
            pFocusedObject->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_Matrix());
    }
}

void CUI_Indicator_ObjectAttack::Set_IndicatorPosition(_vectorf vWorldPosition, _matrixf WorldMatrix)
{
#pragma region 이전
    //   _matrix ProjectedMatrix = WorldMatrix * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW) *
    //       CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);
    //   _vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);/*vWorldPosition*/;

    //   vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);

    //   _float fScreenX = XMVectorGetX(vPosition);
    //   _float fScreenY = XMVectorGetY(vPosition);

    //   m_vPosition_Indicator.x = (fScreenX) * 0.5f * g_iWinCX;
    //   m_vPosition_Indicator.y = (fScreenY) * 0.5f * g_iWinCY;

    //   _float4x4 mCamWorld = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);
    //   _vector vCamLook = XMLoadFloat4((_float4*)mCamWorld.m[2]);
    //   _vector vTargetDir = vWorldPosition - XMLoadFloat4((_float4*)mCamWorld.m[3]);

       //if (XMVectorGetX(XMVector3Dot(XMVector3Normalize(vCamLook), XMVector3Normalize(vTargetDir))) < 0) // Behind Pass
       //{
    //       if (m_vPosition_Indicator.x < 0.f)
    //           m_vPosition_Indicator.x = -0.5f * g_iWinCX + 30.f;
    //       else
    //           m_vPosition_Indicator.x = 0.5f * g_iWinCX - 30.f;
       //}

    //   m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    //   m_pGauge->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);

    //   m_bRenderSwitch = true;
    //   //m_bEnable = true;

    //   // Look Vector Objects ---> Camera
    //   _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);


    //   _vector vLookMonster = MatCam.r[3] - WorldMatrix.r[3];

    //   // Calculate the angle between vLookMonster and MatCam.r[2]
    //   _float fDotProduct = XMVectorGetX(XMVector3Dot(vLookMonster, MatCam.r[2]));
    //   _float fDeterminant = XMVectorGetX(XMVector3Length(vLookMonster)) * XMVectorGetX(XMVector3Length(MatCam.r[2]));

    //   // Calculate the angle in degrees
    //   _float fDegree = XMConvertToDegrees(acosf(fDotProduct / fDeterminant));

    //   if (90.f <= fDegree && 270.f >= fDegree)
    //   {
    //       m_bRenderDegree = true;
    //   }
    //   else
    //   {
    //       m_bRenderDegree = false;
    //   }
#pragma endregion
#pragma region 새거
    _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);

    _vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

    /* World */
    vPosition = XMVector3TransformCoord(vPosition, WorldMatrix);

    _vector vLookCam = MatCam.r[3] - vPosition ;

    _vector vCrossLookCam = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));

    vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);

    _float fScreenX = XMVectorGetX(vPosition);
    _float fScreenY = XMVectorGetY(vPosition);

    m_vPosition_Indicator.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator.y = (fScreenY) * 0.5f * g_iWinCY;

    m_vPosition_Gauge.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Gauge.y = (fScreenY) * 0.5f * g_iWinCY;

    m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pGauge->Set_State(TRANSFORM::POSITION, m_vPosition_Gauge);

    m_bRenderSwitch = true;

//=========================================================================================================================== Culling
    // Look_Vector (Monster ---> Camera)
    _vector vLookCamera_FromMonster = MatCam.r[3] - WorldMatrix.r[3];
    _vector vLookMonster_FromCamera = WorldMatrix.r[3] - MatCam.r[3];

    _float fDotProduct = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLookCamera_FromMonster), XMVector3Normalize(MatCam.r[2])))));

    if (90.f <= fDotProduct && 270.f >= fDotProduct)
    {
        m_bRenderDegree = true;
    }
    else
    {
        m_bRenderDegree = false;
    }
#pragma endregion
}

HRESULT CUI_Indicator_ObjectAttack::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Indicator_ObjectAttack> CUI_Indicator_ObjectAttack::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_ObjectAttack> pInstance = make_private_shared(CUI_Indicator_ObjectAttack, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ObjectAttack::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_ObjectAttack::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_ObjectAttack> pInstance = make_private_shared_copy(CUI_Indicator_ObjectAttack, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ObjectAttack::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
