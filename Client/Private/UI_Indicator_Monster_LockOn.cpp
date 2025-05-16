#include "ClientPCH.h"
#include "UI_Indicator_Monster_LockOn.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Camera_Player.h"
#include "Monster.h"

CUI_Indicator_Monster_LockOn::CUI_Indicator_Monster_LockOn(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Monster_LockOn::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Monster_LockOn::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Initialize", "Failed to CGameObject::Initialize");
    }


    Initialize_UI();


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Monster_LockOn::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Monster_LockOn::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_5);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Monster_LockOn::Render()
{
    if (nullptr != m_pCamera_Player->Get_LockOnTransform() && true == m_pCamera_Player->Get_LockOn())
    {
        /* Top */
        if (FAILED(m_pTexture_Indicator_T->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pIndicator_T->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBlinkTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(40)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CUI_Indicator_Monster_LockOn::Render");
        }

        /* Right */
        if (FAILED(m_pTexture_Indicator_R->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pIndicator_R->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBlinkTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(40)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CUI_Indicator_Monster_LockOn::Render");
        }

        /* Bottom */
        if (FAILED(m_pTexture_Indicator_B->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pIndicator_B->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBlinkTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(40)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CUI_Indicator_Monster_LockOn::Render");
        }

        /* Left */
        if (FAILED(m_pTexture_Indicator_L->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pIndicator_L->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBlinkTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(40)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CUI_Indicator_Monster_LockOn::Render");
        }

        if (true == m_bRenderBackground)
        {
            /* BackGround */
            if (FAILED(m_pTexture_Indicator_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pIndicator_Background->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBackgroundAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (FAILED(__super::Render(41)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Render", "Failed to CUI_Indicator_Monster_LockOn::Render");
            }
        }
    }
    return S_OK;
}

HRESULT CUI_Indicator_Monster_LockOn::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_LockOn::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_Monster_LockOn::Initialize_UI()
{
//================================================================================================ Transform
    m_pIndicator_T                      = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_R                      = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_B                      = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_L                      = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_Background             = CTransform::Create(m_pDevice, m_pContext);

//================================================================================================ Texture
    m_pTexture_Indicator_T              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/LockOn.png"));
    m_pTexture_Indicator_R              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/LockOn.png"));
    m_pTexture_Indicator_B              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/LockOn.png"));
    m_pTexture_Indicator_L              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/LockOn.png"));
    m_pTexture_Indicator_Background     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_Background.png"));

//================================================================================================ _float3
    m_vScale_Indicator_T                = _float3(35.f, 35.f, 0.f);
    m_vScale_Indicator_R                = _float3(35.f, 35.f, 0.f);
    m_vScale_Indicator_B                = _float3(35.f, 35.f, 0.f);
    m_vScale_Indicator_L                = _float3(35.f, 35.f, 0.f);
    m_vScale_Indicator_Background       = _float3(400.f, 400.f, 1.f);

//================================================================================================ _float4
    m_vPosition_Indicator_T             = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_R             = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_B             = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_L             = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_Background    = _float4(0.f, 0.f, 0.f, 1.f);

//================================================================================================ Rotation
    _matrix PivotMatrix                 = XMMatrixIdentity();
    
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(45.f));
    m_pIndicator_T->Set_Matrix(PivotMatrix);
    m_pIndicator_Background->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(315.f));
    m_pIndicator_R->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(225.f));
    m_pIndicator_B->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(135.f));
    m_pIndicator_L->Set_Matrix(PivotMatrix);

//================================================================================================ Position
    m_pIndicator_T->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_T);
    m_pIndicator_R->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R);
    m_pIndicator_B->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_B);
    m_pIndicator_L->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L);
    m_pIndicator_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Background);

//================================================================================================ Scale
    m_pIndicator_T->Set_Scale(m_vScale_Indicator_T);
    m_pIndicator_R->Set_Scale(m_vScale_Indicator_R);
    m_pIndicator_B->Set_Scale(m_vScale_Indicator_B);
    m_pIndicator_L->Set_Scale(m_vScale_Indicator_L);
    m_pIndicator_Background->Set_Scale(m_vScale_Indicator_Background);

//================================================================================================ _bool
    m_bPositionMove                     = false;
    m_bPositionMoveEnd                  = false;
    m_bRenderBackground                 = false;
    m_bRenderBackgroundEnd              = false;
    m_bBlink                            = false;

//================================================================================================ _float
    m_fTime                             = 0.06f;
    m_fBackgroundAlpha                  = 0.f;
    m_fBlinkTime                        = 1.f;

//================================================================================================ _int
    m_iSwitch                           = -1;
    m_iBlinkCount                       = 0;
    m_iSoundIndex                       = 0;
}

void CUI_Indicator_Monster_LockOn::Tick_UI(_float fTimeDelta)
{
    LockOn(fTimeDelta);
    Sound(fTimeDelta);
}

void CUI_Indicator_Monster_LockOn::LockOn(_float fTimeDelta)
{
    shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_CAMERA);
    m_pCamera_Player = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));

    shared_ptr<CObjectLayer> pLayerObject = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER);
    m_pLockOnMonster = static_pointer_cast<CMonster>(pLayerObject->Get_GameObject(0));
    
    if (nullptr == m_pLockOnMonster)
        return;
    else
    {
        if (SCENE::TEST == CGameInstance::Get_Instance()->Current_Scene())
        {
            if (nullptr == m_pCamera_Player)
            {
                return;
            }
            else
            {
                if (nullptr != m_pCamera_Player->Get_LockOnTransform() && true == m_pCamera_Player->Get_LockOn())
                {
                    Set_IndicatorPosition_MIZUHA(m_pCamera_Player->Get_State(TRANSFORM::POSITION), m_pCamera_Player->Get_LockOnTransform()->Get_WorldMatrix());
                    Set_Positioning(fTimeDelta);
                    Render_Background(fTimeDelta);
                    Blink(fTimeDelta);
                }
                else
                {
                    m_fTime = 0.06f;
                    m_iBlinkCount = 0;
                    m_fBlinkTime = 1.f;
                    m_fBackgroundAlpha = 0.f;
                    m_bBlink = false;
                    m_bPositionMove = false;
                    m_bPositionMoveEnd = false;
                    m_bRenderBackground = false;
                    m_bRenderBackgroundEnd = false;
                }
            }
        }
        if (SCENE::MIZUHA == CGameInstance::Get_Instance()->Current_Scene())
        {
            if (nullptr == m_pCamera_Player)
            {
                return;
            }
            else
            {
                if (nullptr != m_pCamera_Player->Get_LockOnTransform() && true == m_pCamera_Player->Get_LockOn())
                {
                    Set_IndicatorPosition_MIZUHA(m_pCamera_Player->Get_State(TRANSFORM::POSITION), m_pCamera_Player->Get_LockOnTransform()->Get_WorldMatrix());
                    Set_Positioning(fTimeDelta);
                    Render_Background(fTimeDelta);
                    Blink(fTimeDelta);
                }
                else
                {
                    m_fTime = 0.06f;
                    m_iBlinkCount = 0;
                    m_fBlinkTime = 1.f;
                    m_fBackgroundAlpha = 0.f;
                    m_bBlink = false;
                    m_bPositionMove = false;
                    m_bPositionMoveEnd = false;
                    m_bRenderBackground = false;
                    m_bRenderBackgroundEnd = false;
                }
            }
        }
    }
}

void CUI_Indicator_Monster_LockOn::Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix)
{
    _matrix ProjectedMatrix = _WorldMatrix * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);

    _vector vPosition = XMVectorSet(0.f, 2.5f, 0.f, 1.f);

    vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);

//===========================================================================================================================
    _float fAspectRatio                 = (_float)g_iWinCX / (_float)g_iWinCY;
    
    _vector vLookCam                    = MatCam.r[3] - vPosition;

    _vector vCrossLookCamY              = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));
    _vector vCrossLookCamX              = XMVector3Normalize(XMVector3Cross(vCrossLookCamY, vLookCam));
    
    _vector vPosition_T                 = vPosition + vCrossLookCamX * (fAspectRatio * m_fTime);
    _vector vPosition_B                 = vPosition + vCrossLookCamX * (- fAspectRatio * m_fTime);
    _vector vPosition_L                 = vPosition + vCrossLookCamY * -m_fTime;
    _vector vPosition_R                 = vPosition + vCrossLookCamY * m_fTime;

//===========================================================================================================================
    _float fScreenX                     = XMVectorGetX(vPosition);
    _float fScreenY                     = XMVectorGetY(vPosition);

    _float fScreenX_T                   = XMVectorGetX(vPosition_T);
    _float fScreenY_T                   = XMVectorGetY(vPosition_T);

    _float fScreenX_B                   = XMVectorGetX(vPosition_B);
    _float fScreenY_B                   = XMVectorGetY(vPosition_B);

    _float fScreenX_L                   = XMVectorGetX(vPosition_L);
    _float fScreenY_L                   = XMVectorGetY(vPosition_L);

    _float fScreenX_R                   = XMVectorGetX(vPosition_R);
    _float fScreenY_R                   = XMVectorGetY(vPosition_R);

//===========================================================================================================================
    m_vPosition_Indicator_Background.x  = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_Background.y  = (fScreenY) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_T.x           = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_T.y           = (fScreenY_B) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_B.x           = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_B.y           = (fScreenY_T) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_L.x           = (fScreenX_L) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_L.y           = (fScreenY) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_R.x           = (fScreenX_R) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_R.y           = (fScreenY) * 0.5f * g_iWinCY;
    
//===========================================================================================================================
    m_pIndicator_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Background);
    m_pIndicator_T->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_T);
    m_pIndicator_B->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_B);
    m_pIndicator_L->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L);
    m_pIndicator_R->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R);
}

void CUI_Indicator_Monster_LockOn::Set_IndicatorPosition_MIZUHA(_vectorf _vWorldPosition, _matrixf _WorldMatrix)
{
    _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);
    _float4 Pivot = m_pLockOnMonster->Get_PhysXPivot();

    m_vPosition = XMVectorSet(0.f, 2.f, 0.f, 1.f) - m_pLockOnMonster->Get_PhysXPivot();
    

//===========================================================================================================================
    /* World Dimension */
    m_vPosition = XMVector3TransformCoord(m_vPosition, _WorldMatrix);
    
    _vector vLookCam                    = MatCam.r[3] - m_vPosition;

    _float fAspectRatio                 = (_float)g_iWinCX / (_float)g_iWinCY;
    
    _vector vCrossLookCamY              = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));
    _vector vCrossLookCamX              = XMVector3Normalize(XMVector3Cross(vCrossLookCamY, vLookCam));
    
    _vector vPosition_T                 = m_vPosition + vCrossLookCamX * (fAspectRatio * m_fTime * 5.f);
    _vector vPosition_B                 = m_vPosition + vCrossLookCamX * (- fAspectRatio * m_fTime * 5.f);
    _vector vPosition_L                 = m_vPosition + vCrossLookCamY * -m_fTime * 8.f;
    _vector vPosition_R                 = m_vPosition + vCrossLookCamY * m_fTime * 8.f;

    _float  fDistance                   = abs(XMVectorGetX(XMVector3Length(vLookCam)));

//===========================================================================================================================
    /* Projection Dimension */
    m_vPosition                         = XMVector3TransformCoord(m_vPosition, ProjectedMatrix);
    vPosition_L                         = XMVector3TransformCoord(vPosition_L, ProjectedMatrix);
    vPosition_R                         = XMVector3TransformCoord(vPosition_R, ProjectedMatrix);
    vPosition_T                         = XMVector3TransformCoord(vPosition_T, ProjectedMatrix);
    vPosition_B                         = XMVector3TransformCoord(vPosition_B, ProjectedMatrix);

//===========================================================================================================================
    /* Set Screen Position X and Y */
    _float fScreenX                     = XMVectorGetX(m_vPosition);
    _float fScreenY                     = XMVectorGetY(m_vPosition);

    _float fScreenX_T                   = XMVectorGetX(vPosition_T);
    _float fScreenY_T                   = XMVectorGetY(vPosition_T);

    _float fScreenX_B                   = XMVectorGetX(vPosition_B);
    _float fScreenY_B                   = XMVectorGetY(vPosition_B);

    _float fScreenX_L                   = XMVectorGetX(vPosition_L);
    _float fScreenY_L                   = XMVectorGetY(vPosition_L);

    _float fScreenX_R                   = XMVectorGetX(vPosition_R);
    _float fScreenY_R                   = XMVectorGetY(vPosition_R);

//===========================================================================================================================
    m_vPosition_Indicator_Background.x  = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_Background.y  = (fScreenY) * 0.5f * g_iWinCY;
    m_vScale_Indicator_Background.x     = g_iWinCX * 2.f / fDistance;
    m_vScale_Indicator_Background.y     = g_iWinCX * 2.f / fDistance;

    m_vPosition_Indicator_T.x           = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_T.y           = (fScreenY_B) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_B.x           = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_B.y           = (fScreenY_T) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_R.x           = (fScreenX_L) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_R.y           = (fScreenY) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_L.x           = (fScreenX_R) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_L.y           = (fScreenY) * 0.5f * g_iWinCY;
    
//===========================================================================================================================
    m_pIndicator_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Background);
    m_pIndicator_Background->Set_Scale(m_vScale_Indicator_Background);
    m_pIndicator_T->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_T);
    m_pIndicator_B->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_B);
    m_pIndicator_L->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L);
    m_pIndicator_R->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R);
}

void CUI_Indicator_Monster_LockOn::Set_Positioning(_float fTimeDelta)
{
    if (false == m_bPositionMove)
    {
        if (0.2f <= m_fTime)    // 2. Increase End  (Biggest Moment)
        {
            m_bPositionMove = true;
        }
        else                    // 1. Increase Start
        {
            m_fTime += fTimeDelta * 1.f;
        }
    }
    if (true == m_bPositionMove && false == m_bPositionMoveEnd)
    {
        if (0.18f >= m_fTime)   // 4. Decrease End
        {
            m_fTime = 0.18f;
            m_bPositionMoveEnd = true;
            m_bRenderBackground = true;
        }
        else                    // 3. Decrease Start
        {
            m_fTime -= fTimeDelta * 0.2f;
        }
    }
}

void CUI_Indicator_Monster_LockOn::Render_Background(_float fTimeDelta)
{
    if (true == m_bRenderBackground)
    {
        if (0.f <= m_fBackgroundAlpha && false == m_bRenderBackgroundEnd)
        {
            m_fBackgroundAlpha += fTimeDelta * 10.f;
            
            if (1.f <= m_fBackgroundAlpha)
            {
                m_fBackgroundAlpha = 1.f;
                m_bRenderBackgroundEnd = true;
            }
        }
        if (true == m_bRenderBackgroundEnd)
        {
            m_fBackgroundAlpha -= fTimeDelta * 15.f;

            if (0.f >= m_fBackgroundAlpha)
            {
                m_fBackgroundAlpha = 0.f;
                m_bBlink = true;
                m_bRenderBackground = false;
            }
        }
    }
}

void CUI_Indicator_Monster_LockOn::Blink(_float fTimeDelta)
{
    if (true == m_bBlink)
    {
        m_fBlinkTime += fTimeDelta * m_iSwitch * 2.f;

        if (1.f <= m_fBlinkTime)
        {
            m_fBlinkTime = 1.f;
            m_iSwitch *= -1;
            m_iBlinkCount += 1;

            if (2 <= m_iBlinkCount)
                m_bBlink = false;
        }
        if (0.f >= m_fBlinkTime)
        {
            m_fBlinkTime = 0.f;
            m_iSwitch *= -1;
        }
    }

}

void CUI_Indicator_Monster_LockOn::Sound(_float fTimeDelta)
{
    if (true == m_pCamera_Player->Get_LockOn())
    {
        if (0 == m_iSoundIndex)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_03.LockOn.wav", 1.f);
            ++m_iSoundIndex;
        }
    }
    else
    {
        if (0 != m_iSoundIndex)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_03.LockOff.wav", 1.f);
            m_iSoundIndex = 0;
        }
    }
}

shared_ptr<CUI_Indicator_Monster_LockOn> CUI_Indicator_Monster_LockOn::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Monster_LockOn> pInstance = make_private_shared(CUI_Indicator_Monster_LockOn, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Monster_LockOn::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Monster_LockOn::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_Monster_LockOn> pInstance = make_private_shared_copy(CUI_Indicator_Monster_LockOn, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Monster_LockOn::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
