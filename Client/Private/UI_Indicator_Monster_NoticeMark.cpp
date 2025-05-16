#include "ClientPCH.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define MINSCALEX       _float(45.f)
#define MINSCALEY       _float(60.f)

#define MINSCALEX_SIDE  _float(24.f)
#define MINSCALEY_SIDE  _float(36.f)


CUI_Indicator_Monster_NoticeMark::CUI_Indicator_Monster_NoticeMark(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Monster_NoticeMark::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Monster_NoticeMark::Initialize(any _aDesc)
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

void CUI_Indicator_Monster_NoticeMark::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Monster_NoticeMark::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_4);

    Set_IndicatorScaleDown();
    Set_ScalingSystem(fTimeDelta);

    if (true == m_bScaleDownStart && false == m_bScaleDisappear)
    {
        m_fShaderIndex += fTimeDelta * 2.f;

        if (1 == (_uint)m_fShaderIndex % 2)
        {
            m_fShaderTime += fTimeDelta;
        }
        if (0 == (_uint)m_fShaderIndex % 2)
        {
            m_fShaderTime -= fTimeDelta;
        }
    }
    if (true == m_bScaleDisappear)
    {
        m_fShaderTime = 1.f;
    }
}

HRESULT CUI_Indicator_Monster_NoticeMark::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (true == m_bRenderSwitch && true == m_bRenderDegree)
        {
            // M
            if (FAILED(m_pTexture_Indicator->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pIndicator->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (true == m_bScaleDownStart)
            {
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShaderTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark", "Failed to m_pShader::Bind_RawValue");
                }
            }
            if (FAILED(__super::Render(31))) // need repair
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CUI_Indicator_Monster_NoticeMark::Render");
            }

            // L
            if (FAILED(m_pTexture_Indicator_L->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pIndicator_L->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (true == m_bScaleDownStart)
            {
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShaderTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark", "Failed to m_pShader::Bind_RawValue");
                }
            }
            if (FAILED(__super::Render(32))) // need repair
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CUI_Indicator_Monster_NoticeMark::Render");
            }

            // R
            if (FAILED(m_pTexture_Indicator_R->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pIndicator_R->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (true == m_bScaleDownStart)
            {
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShaderTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark", "Failed to m_pShader::Bind_RawValue");
                }
            }
            if (FAILED(__super::Render(32))) // need repair
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Render", "Failed to CUI_Indicator_Monster_NoticeMark::Render");
            }
        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_Monster_NoticeMark::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_Monster_NoticeMark::Initialize_UI()
{
//=====================================================================
    m_vScale_Indicator      = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_L    = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_R    = _float3(100.f, 120.f, 1.f);
    m_vPosition_Indicator   = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_L = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_R = _float4(0.f, 0.f, 0.f, 1.f);
//=====================================================================
    m_pIndicator            = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_L          = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_R          = CTransform::Create(m_pDevice, m_pContext);
    m_pTexture_Indicator    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Notice_M.png"));
    m_pTexture_Indicator_L  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Notice_L.png"));
    m_pTexture_Indicator_R  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Notice_R.png"));
//=====================================================================
    m_pIndicator->Set_Scale(m_vScale_Indicator);
    m_pIndicator_L->Set_Scale(m_vScale_Indicator_L);
    m_pIndicator_R->Set_Scale(m_vScale_Indicator_R);
    m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pIndicator_L->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L);
    m_pIndicator_R->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R);
//=====================================================================
    m_bRenderSwitch         = false;
    m_bScaleDownStart       = false;
    m_bScaleDisappear       = false;
    m_bRenderDegree         = false;

    m_fTime                 = 0.f;
    m_fAccTime              = 0.f;
    m_fShaderTime           = 1.f;
    m_fShaderIndex          = 0.f;
    m_fAcceleration         = 1.f;
}

void CUI_Indicator_Monster_NoticeMark::Set_IndicatorScaleDown()
{
    if (true == m_bRenderSwitch)    // When Monster Notice The Player
    {
        m_bScaleDownStart = true;   // Set True
    }
}

void CUI_Indicator_Monster_NoticeMark::Set_ScalingSystem(_float fTimeDelta)
{
    if (true == m_bScaleDownStart && false == m_bScaleDisappear)
    {
        if (m_vScale_Indicator.x <= MINSCALEX && m_vScale_Indicator.y <= MINSCALEY)
        {
            m_fAccTime += fTimeDelta;

            if (2.f <= m_fAccTime)
            {
                m_bScaleDisappear = true;
            }
        }
        else
        {
            m_vScale_Indicator.x    -= fTimeDelta * 700.f;
            m_vScale_Indicator_L.x  -= fTimeDelta * 700.f;
            m_vScale_Indicator_R.x  -= fTimeDelta * 700.f;

            m_vScale_Indicator.y    -= fTimeDelta * 700.f;
            m_vScale_Indicator_L.y  -= fTimeDelta * 700.f;
            m_vScale_Indicator_R.y  -= fTimeDelta * 700.f;

            m_pIndicator->Set_Scale(m_vScale_Indicator);
            m_pIndicator_L->Set_Scale(m_vScale_Indicator_L);
            m_pIndicator_R->Set_Scale(m_vScale_Indicator_R);

            if (m_vScale_Indicator.x <= MINSCALEX)
            {
                m_vScale_Indicator.x = MINSCALEX;
                m_pIndicator->Set_Scale(m_vScale_Indicator);
            }
            if (m_vScale_Indicator.y <= MINSCALEY)
            {
                m_vScale_Indicator.y = MINSCALEY;
                m_pIndicator->Set_Scale(m_vScale_Indicator);
            }
            if (m_vScale_Indicator_L.x <= MINSCALEX_SIDE || m_vScale_Indicator_R.x <= MINSCALEX_SIDE)
            {
                m_vScale_Indicator_L.x = MINSCALEX_SIDE;
                m_vScale_Indicator_R.x = MINSCALEX_SIDE;
                m_pIndicator_L->Set_Scale(m_vScale_Indicator_L);
                m_pIndicator_R->Set_Scale(m_vScale_Indicator_R);
            }
            if (m_vScale_Indicator_L.y <= MINSCALEY_SIDE || m_vScale_Indicator_R.x <= MINSCALEY_SIDE)
            {
                m_vScale_Indicator_L.y = MINSCALEY_SIDE;
                m_vScale_Indicator_R.y = MINSCALEY_SIDE;
                m_pIndicator_L->Set_Scale(m_vScale_Indicator_L);
                m_pIndicator_R->Set_Scale(m_vScale_Indicator_R);
            }

        }
    }
    else if (true == m_bScaleDownStart && true == m_bScaleDisappear)
    {
        m_vScale_Indicator.x +=  696.5f - (m_fAcceleration * 695.f);
        m_vScale_Indicator.y +=  696.5f - (m_fAcceleration * 695.f);
        m_vScale_Indicator_L.x += 696.5f - (m_fAcceleration * 695.f);
        m_vScale_Indicator_L.y += 696.5f - (m_fAcceleration * 695.f);
        m_vScale_Indicator_R.x += 696.5f - (m_fAcceleration * 695.f);
        m_vScale_Indicator_R.y += 696.5f - (m_fAcceleration * 695.f);

        m_pIndicator->Set_Scale(m_vScale_Indicator);
        m_pIndicator_L->Set_Scale(m_vScale_Indicator_L);
        m_pIndicator_R->Set_Scale(m_vScale_Indicator_R);

        if (0.f >= m_vScale_Indicator.x || 0.f >= m_vScale_Indicator_L.x || 0.f >= m_vScale_Indicator_R.x)
        {
            m_vScale_Indicator.x    = 0.f;
            m_vScale_Indicator_L.x  = 0.f;
            m_vScale_Indicator_R.x  = 0.f;
        }

        if (0 >= m_vScale_Indicator.y || 0.f >= m_vScale_Indicator_L.y || 0.f >= m_vScale_Indicator_R.y)
        {
            m_vScale_Indicator.y    = 0.f;
            m_vScale_Indicator_L.y  = 0.f;
            m_vScale_Indicator_R.y  = 0.f;
        }

        if (0 >= m_vScale_Indicator.x && 0 >= m_vScale_Indicator.y)
        {
            m_bRenderSwitch = false;
            m_bScaleDownStart = false;
            m_bScaleDisappear = false;
        }

        m_fAcceleration += 2.5f * fTimeDelta * fTimeDelta;
    }
}

void CUI_Indicator_Monster_NoticeMark::Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix)
{
    if (true == m_bRenderSwitch)
    {
        if (SCENE::MIZUHA != CGameInstance::Get_Instance()->Current_Scene())
        {
            _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
                * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

            _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);
          
            _vector vPosition = XMVectorSet(0.f, 3.f, 0.f, 1.f);     // 0.f, 0.f, 0.f, 1.f

            vPosition = XMVector3TransformCoord(vPosition, _WorldMatrix);

            _vector vLookCam = MatCam.r[3] - vPosition;

            _vector vCrossLookCam = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));
            _vector vPosition_L = vPosition + vCrossLookCam * 0.55f;
            _vector vPosition_R = vPosition + vCrossLookCam * -0.55f;

            vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);
            vPosition_L = XMVector3TransformCoord(vPosition_L, ProjectedMatrix);
            vPosition_R = XMVector3TransformCoord(vPosition_R, ProjectedMatrix);

            _float fScreenX = XMVectorGetX(vPosition);
            _float fScreenY = XMVectorGetY(vPosition);

            _float fScreenX_L = XMVectorGetX(vPosition_L);
            _float fScreenY_L = XMVectorGetY(vPosition_L);

            _float fScreenX_R = XMVectorGetX(vPosition_R);
            _float fScreenY_R = XMVectorGetY(vPosition_R);

            m_vPosition_Indicator.x = (fScreenX) * 0.5f * g_iWinCX;
            m_vPosition_Indicator.y = (fScreenY) * 0.5f * g_iWinCY;

            m_vPosition_Indicator_L.x = (fScreenX_L) * 0.5f * g_iWinCX;
            m_vPosition_Indicator_L.y = (fScreenY) * 0.5f * g_iWinCY;

            m_vPosition_Indicator_R.x = (fScreenX_R) * 0.5f * g_iWinCX;
            m_vPosition_Indicator_R.y = (fScreenY) * 0.5f * g_iWinCY;

            m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
            m_pIndicator_L->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L);
            m_pIndicator_R->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R);

            //=========================================================================================================================== Culling
            // Look Vector Monster ---> Camera
            _vector vLookMonster = MatCam.r[3] - _WorldMatrix.r[3];

            // Calculate the angle between vLookMonster and MatCam.r[2]
            _float fDotProduct = XMVectorGetX(XMVector3Dot(vLookMonster, MatCam.r[2]));
            _float fDeterminant = XMVectorGetX(XMVector3Length(vLookMonster)) * XMVectorGetX(XMVector3Length(MatCam.r[2]));

            // Calculate the angle in degrees
            _float fDegree = XMConvertToDegrees(acosf(fDotProduct / fDeterminant));

            if (90.f <= fDegree && 270.f >= fDegree)
            {
                m_bRenderDegree = true;
            }
            else
            {
                m_bRenderDegree = false;
            }
        }
//============================================================================================================================================================================================
        else if (SCENE::MIZUHA == CGameInstance::Get_Instance()->Current_Scene())
        {
            _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
                * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

            _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);


            _vector vPosition = XMVectorSet(0.f, 3.f, 0.f, 1.f);     // 0.f, 0.f, 0.f, 1.f

            /* World */
            vPosition = XMVector3TransformCoord(vPosition, _WorldMatrix);

            _vector vLookCam = MatCam.r[3] - vPosition;

            _vector vCrossLookCam = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));
            _vector vPosition_L = vPosition + vCrossLookCam * -0.5f;
            _vector vPosition_R = vPosition + vCrossLookCam * 0.5f;

            vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);
            vPosition_L = XMVector3TransformCoord(vPosition_L, ProjectedMatrix);
            vPosition_R = XMVector3TransformCoord(vPosition_R, ProjectedMatrix);

            _float fScreenX = XMVectorGetX(vPosition);
            _float fScreenY = XMVectorGetY(vPosition);

            _float fScreenX_L = XMVectorGetX(vPosition_L);
            _float fScreenY_L = XMVectorGetY(vPosition_L);

            _float fScreenX_R = XMVectorGetX(vPosition_R);
            _float fScreenY_R = XMVectorGetY(vPosition_R);

            m_vPosition_Indicator.x = (fScreenX) * 0.5f * g_iWinCX;
            m_vPosition_Indicator.y = (fScreenY) * 0.5f * g_iWinCY;

            m_vPosition_Indicator_R.x = (fScreenX_L) * 0.5f * g_iWinCX;
            m_vPosition_Indicator_R.y = (fScreenY) * 0.5f * g_iWinCY;

            m_vPosition_Indicator_L.x = (fScreenX_R) * 0.5f * g_iWinCX;
            m_vPosition_Indicator_L.y = (fScreenY) * 0.5f * g_iWinCY;

            m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
            m_pIndicator_R->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R);
            m_pIndicator_L->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L);
        
           /* vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);

            _vector vLookCam = MatCam.r[3] - vPosition;

            _vector vCrossLookCam = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));
            _vector vPosition_L = vPosition + vCrossLookCam * -0.3f;
            _vector vPosition_R = vPosition + vCrossLookCam * 0.3f;

            _float fScreenX = XMVectorGetX(vPosition);
            _float fScreenY = XMVectorGetY(vPosition);

            _float fScreenX_L = XMVectorGetX(vPosition_L);
            _float fScreenY_L = XMVectorGetY(vPosition_L);

            _float fScreenX_R = XMVectorGetX(vPosition_R);
            _float fScreenY_R = XMVectorGetY(vPosition_R);

            m_vPosition_Indicator.x = (fScreenX) * 0.5f * g_iWinCX;
            m_vPosition_Indicator.y = (fScreenY) * 0.5f * g_iWinCY;

            m_vPosition_Indicator_R.x = (fScreenX_L) * 0.5f * g_iWinCX;
            m_vPosition_Indicator_R.y = (fScreenY) * 0.5f * g_iWinCY;

            m_vPosition_Indicator_L.x = (fScreenX_R) * 0.5f * g_iWinCX;
            m_vPosition_Indicator_L.y = (fScreenY) * 0.5f * g_iWinCY;

            m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
            m_pIndicator_R->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R);
            m_pIndicator_L->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L);
            */
            //=========================================================================================================================== Culling
            // Look Vector Monster ---> Camera
            _vector vLookMonster = MatCam.r[3] - _WorldMatrix.r[3];

            // Calculate the angle between vLookMonster and MatCam.r[2]
            _float fDotProduct = XMVectorGetX(XMVector3Dot(vLookMonster, MatCam.r[2]));
            _float fDeterminant = XMVectorGetX(XMVector3Length(vLookMonster)) * XMVectorGetX(XMVector3Length(MatCam.r[2]));

            // Calculate the angle in degrees
            _float fDegree = XMConvertToDegrees(acosf(fDotProduct / fDeterminant));

            if (90.f <= fDegree && 270.f >= fDegree)
            {
                m_bRenderDegree = true;
            }
            else
            {
                m_bRenderDegree = false;
            }
        }
    }
}

void CUI_Indicator_Monster_NoticeMark::Set_Initialize()
{
//=====================================================================
    m_vScale_Indicator      = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_L    = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_R    = _float3(100.f, 120.f, 1.f);
    m_vPosition_Indicator   = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_L = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_R = _float4(0.f, 0.f, 0.f, 1.f);

//=====================================================================
    m_pIndicator->Set_Scale(m_vScale_Indicator);
    m_pIndicator_L->Set_Scale(m_vScale_Indicator_L);
    m_pIndicator_R->Set_Scale(m_vScale_Indicator_R);
    m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pIndicator_L->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L);
    m_pIndicator_R->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R);

//=====================================================================
    m_bRenderSwitch         = false;
    m_bScaleDownStart       = false;
    m_bScaleDisappear       = false;

    m_fTime                 = 0.f;
    m_fAccTime              = 0.f;
    m_fShaderTime           = 1.f;
    m_fShaderIndex          = 0.f;
    m_fAcceleration         = 1.f;

}

shared_ptr<CUI_Indicator_Monster_NoticeMark> CUI_Indicator_Monster_NoticeMark::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Monster_NoticeMark> pInstance = make_private_shared(CUI_Indicator_Monster_NoticeMark, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Monster_NoticeMark::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Monster_NoticeMark::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_Monster_NoticeMark> pInstance = make_private_shared_copy(CUI_Indicator_Monster_NoticeMark, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Monster_NoticeMark::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
