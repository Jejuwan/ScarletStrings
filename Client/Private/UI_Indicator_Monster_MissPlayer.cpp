#include "ClientPCH.h"
#include "UI_Indicator_Monster_MissPlayer.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define MINSCALEX       _float(45.f)
#define MINSCALEY       _float(60.f)

#define MINSCALEX_3     _float(46.f)
#define MINSCALEY_3     _float(48.f)

#define MINSCALEX_SIDE  _float(24.f)
#define MINSCALEY_SIDE  _float(36.f)

CUI_Indicator_Monster_MissPlayer::CUI_Indicator_Monster_MissPlayer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Monster_MissPlayer::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Monster_MissPlayer::Initialize(any _aDesc)
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

void CUI_Indicator_Monster_MissPlayer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Monster_MissPlayer::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_4);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Monster_MissPlayer::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (true == m_bRenderSwitch/* && true == m_bRenderDegree*/)
        {
            if (0 == m_iRenderPhase)
            {
                // M1
                if (FAILED(m_pTexture_Indicator_M1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pIndicator_M1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to m_pShader::Bind_RawValue");
                }
                if (FAILED(__super::Render(67)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                }

                // L
                if (FAILED(m_pTexture_Indicator_L1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pIndicator_R1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to m_pShader::Bind_RawValue");
                }
                if (FAILED(__super::Render(67)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                }

                // R
                if (FAILED(m_pTexture_Indicator_R1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pIndicator_R1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to m_pShader::Bind_RawValue");
                }
                if (FAILED(__super::Render(67)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                }
            }
            if (1 == m_iRenderPhase)
            {
                if (1 == m_iTextureShift)
                {
                    // M
                    if (FAILED(m_pTexture_Indicator_M2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pIndicator_M2->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                    }

                    // L
                    if (FAILED(m_pTexture_Indicator_L2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pIndicator_L2->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                    }

                    // R
                    if (FAILED(m_pTexture_Indicator_R2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pIndicator_R2->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                    }
                }
                else if (2 == m_iTextureShift)
                {
                    if (FAILED(m_pTexture_Indicator_M1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pIndicator_M1->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to m_pShader::Bind_RawValue");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                    }

                    // L
                    if (FAILED(m_pTexture_Indicator_L1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pIndicator_L1->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                    }

                    // R
                    if (FAILED(m_pTexture_Indicator_R1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pIndicator_R1->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                    }
                }
            }
            if (2 == m_iRenderPhase)
            {
                if (FAILED(m_pTexture_Indicator_M3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pIndicator_M3->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTexture3Shader, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to m_pShader::Bind_RawValue");
                }
                if (FAILED(__super::Render(67)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_MissPlayer::Render", "Failed to CUI_Indicator_Monster_MissPlayer::Render");
                }
            }
        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_Monster_MissPlayer::Ready_Components()
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

void CUI_Indicator_Monster_MissPlayer::Initialize_UI()
{
//================================================================================== // CTransform
    m_pIndicator_M1             = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_M2             = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_M3             = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_L1             = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_L2             = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_R1             = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_R2             = CTransform::Create(m_pDevice, m_pContext);

//================================================================================== // CTexture
    m_pTexture_Indicator_M1     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Miss_M1.png"));
    m_pTexture_Indicator_M2     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Miss_M2.png"));
    m_pTexture_Indicator_M3     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Miss_M3.png"));
    m_pTexture_Indicator_L1     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Notice_L.png"));
    m_pTexture_Indicator_L2     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Miss_L.png"));
    m_pTexture_Indicator_R1     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Notice_R.png"));
    m_pTexture_Indicator_R2     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Miss_R.png"));

//================================================================================== // _bool
    m_bRenderSwitch             = false;
    m_bScaleDownStart           = false;
    m_bScaleDisappear           = false;
    m_bRenderDegree             = false;

//================================================================================== // _uint
    m_iRenderPhase              = 0;

    // For. Control Shader & Blink
    m_iTextureShift             = 1;

//================================================================================== // _float
    m_fTime                     = 0.5f;
    m_fAccTime                  = 0.f;
    m_fShaderTime               = 0.f;
    m_fShaderIndex              = 0.f;
    m_fTexture3Shader           = 0.f;
    m_fRenderEndTime            = 0.f;

//================================================================================== // _float3
    m_vScale_Indicator_M1       = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_M2       = _float3(MINSCALEX, MINSCALEY, 1.f);
    m_vScale_Indicator_M3       = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_L1       = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_L2       = _float3(MINSCALEX_SIDE, MINSCALEY_SIDE, 1.f);
    m_vScale_Indicator_R1       = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_R2       = _float3(MINSCALEX_SIDE, MINSCALEY_SIDE, 1.f);

//================================================================================== // _float4
    m_vPosition_Indicator_M1    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_M2    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_M3    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_L1    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_L2    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_R1    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_R2    = _float4(0.f, 0.f, 0.f, 1.f);

//================================================================================== // Set_Scale
    m_pIndicator_M1->Set_Scale(m_vScale_Indicator_M1);
    m_pIndicator_M2->Set_Scale(m_vScale_Indicator_M2);
    m_pIndicator_M3->Set_Scale(m_vScale_Indicator_M3);
    m_pIndicator_L1->Set_Scale(m_vScale_Indicator_L1);
    m_pIndicator_L2->Set_Scale(m_vScale_Indicator_L2);
    m_pIndicator_R1->Set_Scale(m_vScale_Indicator_R1);
    m_pIndicator_R2->Set_Scale(m_vScale_Indicator_R2);

//================================================================================== // Set_Position
    m_pIndicator_M1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M1);
    m_pIndicator_M2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M2);
    m_pIndicator_M3->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M3);
    m_pIndicator_L1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L1);
    m_pIndicator_L2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L2);
    m_pIndicator_R1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R1);
    m_pIndicator_R2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R2);
}

void CUI_Indicator_Monster_MissPlayer::Tick_UI(_float fTimeDelta)
{
    //Call_Indicator();

    Set_ScalingSystem(fTimeDelta);
}

void CUI_Indicator_Monster_MissPlayer::Call_Indicator()
{
    //if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    //{
    //    m_bRenderSwitch = true;
    //    m_bScaleDownStart = true;
    //}

    m_bRenderSwitch = true;
    m_bScaleDownStart = true;
}

void CUI_Indicator_Monster_MissPlayer::Set_ScalingSystem(_float fTimeDelta)
{
    if (0 == m_iRenderPhase && true == m_bScaleDownStart)
    {
        if (MINSCALEX >= m_vScale_Indicator_M1.x && MINSCALEY >= m_vScale_Indicator_M1.y)
        {
            m_vScale_Indicator_M1.x = MINSCALEX;
            m_vScale_Indicator_M1.y = MINSCALEY;
            m_pIndicator_M1->Set_Scale(m_vScale_Indicator_M1);

            m_vScale_Indicator_L1.x = MINSCALEX_SIDE;
            m_vScale_Indicator_L1.y = MINSCALEY_SIDE;
            m_pIndicator_L1->Set_Scale(m_vScale_Indicator_L1);

            m_vScale_Indicator_R1.x = MINSCALEX_SIDE;
            m_vScale_Indicator_R1.y = MINSCALEY_SIDE;
            m_pIndicator_R1->Set_Scale(m_vScale_Indicator_R1);

            m_fTime = 1.0f;

            m_bScaleDownStart = false;
            m_iRenderPhase += 1;
        }
        else
        {
            m_vScale_Indicator_M1.x -= fTimeDelta * 700.f;
            m_vScale_Indicator_M1.y -= fTimeDelta * 700.f;
            m_pIndicator_M1->Set_Scale(m_vScale_Indicator_M1);

            m_vScale_Indicator_L1.x -= fTimeDelta * 700.f;
            m_vScale_Indicator_L1.y -= fTimeDelta * 700.f;
            m_pIndicator_L1->Set_Scale(m_vScale_Indicator_L1);

            m_vScale_Indicator_R1.x -= fTimeDelta * 700.f;
            m_vScale_Indicator_R1.y -= fTimeDelta * 700.f;
            m_pIndicator_R1->Set_Scale(m_vScale_Indicator_R1);

            if (1.f <= m_fTime)
            {
                m_fTime = 1.f;
            }
            else
            {
                m_fTime += fTimeDelta * 20.f;
            }
        }
    }
    if (1 == m_iRenderPhase && false == m_bScaleDownStart)
    {
        m_fAccTime += fTimeDelta * 5.f;

        if (0.f <= m_fAccTime && 0.5f > m_fAccTime)
        {
            m_iTextureShift = 1;
        }
        else if (0.5f <= m_fAccTime && 1.f > m_fAccTime)
        {
            m_iTextureShift = 2;
        }
        else if (1.f <= m_fAccTime && 1.5f > m_fAccTime)
        {
            m_iTextureShift = 1;
        }
        else if (1.5f <= m_fAccTime && 2.f > m_fAccTime)
        {
            m_iTextureShift = 2;
        }
        else if (2.f <= m_fAccTime && 2.5f > m_fAccTime)
        {
            m_iTextureShift = 1;
        }
        else if (2.5f <= m_fAccTime && 3.f > m_fAccTime)
        {
            m_iTextureShift = 2;
        }
        else if (3.f <= m_fAccTime && 3.5f > m_iTextureShift)
        {
            m_iTextureShift = 1;
            m_iRenderPhase += 1;

        }
    }
    if (2 == m_iRenderPhase)
    {
        if (MINSCALEX_3 >= m_vScale_Indicator_M3.x && MINSCALEY_3 >= m_vScale_Indicator_M3.y)
        {
            m_vScale_Indicator_M3.x = MINSCALEX_3;
            m_vScale_Indicator_M3.y = MINSCALEY_3;
            m_pIndicator_M3->Set_Scale(m_vScale_Indicator_M3);

            m_fTexture3Shader = 1.0f;
        }
        else
        {
            m_vScale_Indicator_M3.x -= fTimeDelta * 700.f;
            m_vScale_Indicator_M3.y -= fTimeDelta * 700.f;
            m_pIndicator_M3->Set_Scale(m_vScale_Indicator_M3);

            if (1.2f <= m_fTexture3Shader)
            {
                m_fTexture3Shader = 1.2f;
            }
            else
            {
                m_fTexture3Shader += fTimeDelta * 20.f;
            }
        }

        if (2.f <= m_fRenderEndTime)
        {
            Reset_Initialize();
        }
        else
        {
            m_fRenderEndTime += fTimeDelta;
        }
    }


}

void CUI_Indicator_Monster_MissPlayer::Set_IndicatorPosition(_matrixf _WorldMatrix)
{
    if (true == m_bRenderSwitch)
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

        m_vPosition_Indicator_M1.x = (fScreenX) * 0.5f * g_iWinCX;
        m_vPosition_Indicator_M1.y = (fScreenY) * 0.5f * g_iWinCY;
        m_vPosition_Indicator_M2.x = (fScreenX) * 0.5f * g_iWinCX;
        m_vPosition_Indicator_M2.y = (fScreenY) * 0.5f * g_iWinCY;
        m_vPosition_Indicator_M3.x = (fScreenX) * 0.5f * g_iWinCX;
        m_vPosition_Indicator_M3.y = (fScreenY) * 0.5f * g_iWinCY;

        m_vPosition_Indicator_R1.x = (fScreenX_L) * 0.5f * g_iWinCX;
        m_vPosition_Indicator_R1.y = (fScreenY) * 0.5f * g_iWinCY;
        m_vPosition_Indicator_R2.x = (fScreenX_L) * 0.5f * g_iWinCX;
        m_vPosition_Indicator_R2.y = (fScreenY) * 0.5f * g_iWinCY;

        m_vPosition_Indicator_L1.x = (fScreenX_R) * 0.5f * g_iWinCX;
        m_vPosition_Indicator_L1.y = (fScreenY) * 0.5f * g_iWinCY;
        m_vPosition_Indicator_L2.x = (fScreenX_R) * 0.5f * g_iWinCX;
        m_vPosition_Indicator_L2.y = (fScreenY) * 0.5f * g_iWinCY;

        m_pIndicator_M1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M1);
        m_pIndicator_M2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M2);
        m_pIndicator_M3->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M2);
        m_pIndicator_R1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R1);
        m_pIndicator_R2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R2);
        m_pIndicator_L1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L1);
        m_pIndicator_L2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L2);


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

void CUI_Indicator_Monster_MissPlayer::Reset_Initialize()
{
//================================================================================== // _bool
    m_bRenderSwitch             = false;
    m_bScaleDownStart           = false;
    m_bScaleDisappear           = false;
    m_bRenderDegree             = false;

//================================================================================== // _uint
    m_iRenderPhase              = 0;

    // For. Control Shader & Blink
    m_iTextureShift             = 1;

//================================================================================== // _float
    m_fTime                     = 0.5f;
    m_fAccTime                  = 0.f;
    m_fShaderTime               = 0.f;
    m_fShaderIndex              = 0.f;
    m_fTexture3Shader           = 0.f;
    m_fRenderEndTime            = 0.f;

//================================================================================== // _float3
    m_vScale_Indicator_M1       = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_M2       = _float3(MINSCALEX, MINSCALEY, 1.f);
    m_vScale_Indicator_M3       = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_L1       = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_L2       = _float3(MINSCALEX_SIDE, MINSCALEY_SIDE, 1.f);
    m_vScale_Indicator_R1       = _float3(100.f, 120.f, 1.f);
    m_vScale_Indicator_R2       = _float3(MINSCALEX_SIDE, MINSCALEY_SIDE, 1.f);


//================================================================================== // _float4
    m_vPosition_Indicator_M1    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_M2    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_M3    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_L1    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_L2    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_R1    = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_R2    = _float4(0.f, 0.f, 0.f, 1.f);

//================================================================================== // Set_Scale
    m_pIndicator_M1->Set_Scale(m_vScale_Indicator_M1);
    m_pIndicator_M2->Set_Scale(m_vScale_Indicator_M2);
    m_pIndicator_M3->Set_Scale(m_vScale_Indicator_M3);
    m_pIndicator_L1->Set_Scale(m_vScale_Indicator_L1);
    m_pIndicator_L2->Set_Scale(m_vScale_Indicator_L2);
    m_pIndicator_R1->Set_Scale(m_vScale_Indicator_R1);
    m_pIndicator_R2->Set_Scale(m_vScale_Indicator_R2);

//================================================================================== // Set_Position
    m_pIndicator_M1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M1);
    m_pIndicator_M2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M2);
    m_pIndicator_M3->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_M3);
    m_pIndicator_L1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L1);
    m_pIndicator_L2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_L2);
    m_pIndicator_R1->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R1);
    m_pIndicator_R2->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_R2);
}

shared_ptr<CUI_Indicator_Monster_MissPlayer> CUI_Indicator_Monster_MissPlayer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Monster_MissPlayer> pInstance = make_private_shared(CUI_Indicator_Monster_MissPlayer, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Monster_MissPlayer::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Monster_MissPlayer::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_Monster_MissPlayer> pInstance = make_private_shared_copy(CUI_Indicator_Monster_MissPlayer, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Monster_MissPlayer::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
