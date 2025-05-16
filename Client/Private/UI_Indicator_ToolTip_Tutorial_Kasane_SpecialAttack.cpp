#include "ClientPCH.h"
#include "UI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack.h"
#include "GameInstance.h"

#define MOVIE_MAX		80

CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "UI_Indicator_ToolTip_Tutorial_LockOn::Initialize", "Failed to CGameObject::Initialize");
    }

    Initialize_UI();


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "UI_Indicator_ToolTip_Tutorial_LockOn::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Render()
{
    if (true == m_bMovieStart)
    {
        /* Background_Frame */
        if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieBackgroundTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(37)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
        }

        /* Movie */
        if (FAILED(m_pTexture_Movie->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", (_uint)m_fCurrentMovieIndex)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pMovie->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(38)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
        }

        /* SepearateLine1 */
        if (FAILED(m_pTexture_SeperateLine1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pSeperate_Line1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(39)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
        }


        /* SepearateLine2 */
        if (FAILED(m_pTexture_SeperateLine2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pSeperate_Line2->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(39)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
        }

        /* Key_C */
        if (FAILED(m_pTexture_Key_C->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pKey_C->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(65)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
        }

        /* Key_ENTER */
        if (FAILED(m_pTexture_Key_ENTER->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pKey_ENTER->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(65)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
        }

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTitle,             _float2(590.f, 110.f),  XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.9f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1,      _float2(643.f, 530.f),  XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2,      _float2(603.f, 580.f),  XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2_1,    _float2(753.f, 580.f),  XMVectorSet(255.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2_2,    _float2(885.f, 580.f),  XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2_3,    _float2(963.f, 580.f),  XMVectorSet(255.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3,      _float2(603.f, 630.f),  XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strEnter,             _float2(1282.f, 783.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.47f);
    
        if (true == m_bCall_BackgroundClose)
        {
            /* Background_Close */
            if (FAILED(m_pTexture_Background_Close->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground_Close->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fCloseBackgroundTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(37)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
            }

            /* SepearateLine3 */
            if (FAILED(m_pTexture_SeperateLine3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSeperate_Line3->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fCloseSeperateLineTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(39)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
            }

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strClose_Answer,  _float2(753.f, 460.f), XMVectorSet(m_fCloseBackgroundTime, m_fCloseBackgroundTime, m_fCloseBackgroundTime, m_fCloseBackgroundTime), 0.6f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strClose_Yes,     _float2(923.f, 560.f), XMVectorSet(m_fCloseBackgroundTime, m_fCloseBackgroundTime, m_fCloseBackgroundTime, m_fCloseBackgroundTime), 0.55f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strClose_No,      _float2(923.f, 620.f), XMVectorSet(m_fCloseBackgroundTime, m_fCloseBackgroundTime, m_fCloseBackgroundTime, m_fCloseBackgroundTime), 0.55f);

            /* Answer */
            if (FAILED(m_pTexture_Answer->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pAnswer->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fCloseAnswerTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(53)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
            }

            /*  Selector_0 */
            if (FAILED(m_pTexture_Selector_0->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSelector_0->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fCloseAnswerTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(53)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
            }

            /* Selector_1 */
            if (FAILED(m_pTexture_Selector_1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSelector_1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTimeSelector_1, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fBlinkTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(54)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
            }

            /* Selector_2 */
            if (FAILED(m_pTexture_Selector_2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSelector_2->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTimeSelector_2, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fBlinkTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(54)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Initialize_UI()
{
//=========================================================================== _float
    m_fMovieShaderTime                  = 0.f;
    m_fMovieBackgroundTime              = 0.f;
    m_fSeperateLineTime                 = 0.f;

    m_fCurrentMovieIndex                = 1.f;
    m_fScaleX_ToolTip                   = 0.f;

    m_fCloseBackgroundTime              = 0.f;
    m_fCloseSeperateLineTime            = 0.f;
    m_fScaleX_CloseToolTip              = 0.f;
    m_fCloseAnswerTime                  = 0.f;
    m_fAllCloseDelayTime                = 0.f;
    m_fTimeSelector_1                   = 0.f;
    m_fTimeSelector_2                   = 0.f;
    m_fBlinkTime                        = 0.f;

//=========================================================================== Transform
    m_pBackground                       = CTransform::Create(m_pDevice, m_pContext);
    m_pMovie                            = CTransform::Create(m_pDevice, m_pContext);
    m_pSeperate_Line1                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSeperate_Line2                   = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_C                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_ENTER                        = CTransform::Create(m_pDevice, m_pContext);
    m_pBackground_Close                 = CTransform::Create(m_pDevice, m_pContext);
    m_pSeperate_Line3                   = CTransform::Create(m_pDevice, m_pContext);
    m_pAnswer                           = CTransform::Create(m_pDevice, m_pContext);
    m_pSelector_0                       = CTransform::Create(m_pDevice, m_pContext);
    m_pSelector_1                       = CTransform::Create(m_pDevice, m_pContext);
    m_pSelector_2                       = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== Texture
    m_pTexture_Background               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png"));
    m_pTexture_Movie                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Tutorial_Tips/Kasane_SpecialAttack/T_ui_Skill_031_common_f_MV_Movie_40534656.mkv_%d.png"), MOVIE_MAX, true);
    m_pTexture_SeperateLine1            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));
    m_pTexture_SeperateLine2            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));
    m_pTexture_Key_C                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/C.png"));
    m_pTexture_Key_ENTER                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/ENTER.png"));
    m_pTexture_Background_Close         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background_Close.png"));
    m_pTexture_SeperateLine3            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));
    m_pTexture_Answer                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Answer.png"));
    m_pTexture_Selector_0               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Selector_0.png"));
    m_pTexture_Selector_1               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Selector_1.png"));
    m_pTexture_Selector_2               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Selector_2.png"));

//=========================================================================== _bool
    m_bScaleUp                          = true;
    m_bMovieStart                       = false;
    m_bToolTip_End                      = false;
    m_bIsRunning                        = false;
    m_bCall_BackgroundClose             = false;
    m_bScaleUp_Close                    = true;
    m_bCall_BackgroundEnd               = false;
    m_bIsReallyClose                    = false;
    m_bBlinkOn                          = false;
    m_bLightOn_Selector_1               = false;
    m_bLightOn_Selector_2               = false;
    m_bBlinkOn                          = false;

//=========================================================================== _uint
    m_iBlinkIndex                       = 1;
    m_iCloseIndex                       = 0;
    m_iSoundCount                       = 0;
    m_iSoundIndex                       = 0;

//=========================================================================== _float3
    m_vScale_Background                 = _float3(840.f, 760.f, 1.f);
    m_vScale_Movie                      = _float3(512.f, 256.f, 1.f);
    m_vScale_SeperateLine1              = _float3(780.f, 1.5f, 1.f);
    m_vScale_SeperateLine2              = _float3(780.f, 1.5f, 1.f);
    m_vScale_Key_C                      = _float3(30.f, 30.f, 1.f);
    m_vScale_Key_ENTER                  = _float3(30.f, 30.f, 1.f);
    
    m_vScale_Background_Close           = _float3(560.f, 270.f, 1.f);
    m_vScale_SeperateLine3              = _float3(500.f, 2.f, 1.f);
    m_vScale_Answer                     = _float3(150.f, 60.f, 1.f);
    m_vScale_Selector_0                 = _float3(40.f, 40.f, 1.f);
    m_vScale_Selector_1                 = _float3(17.f, 28.f, 1.f);
    m_vScale_Selector_2                 = _float3(15.f, 15.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Background              = _float4(0.f, 90.f, 0.f, 1.f);
    m_vPosition_Movie                   = _float4(0.f, 200.f, 0.f, 1.f);
    m_vPosition_SeperateLine1           = _float4(0.f, 360.f, 0.f, 1.f);
    m_vPosition_SeperateLine2           = _float4(0.f, -220.f, 0.f, 1.f);
    m_vPosition_Key_C                   = _float4(-340.f, 0.f, 0.f, 1.f);
    m_vPosition_Key_ENTER               = _float4(300.f, -253.f, 0.f, 1.f);

    m_vPosition_Background_Close        = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_SeperateLine3           = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Answer                  = _float4(-20.f, -33.f, 0.f, 1.f);
    m_vPosition_Selector_0              = _float4(-60.f, -32.f, 0.f, 1.f);
    m_vPosition_Selector_1              = _float4(-54.f, -32.f, 0.f, 1.f);
    m_vPosition_Selector_2              = _float4(-62.f, -32.f, 0.f, 1.f);

//=========================================================================== Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pMovie->Set_Scale(m_vScale_Movie);
    m_pSeperate_Line1->Set_Scale(m_vScale_SeperateLine1);
    m_pSeperate_Line2->Set_Scale(m_vScale_SeperateLine2);
    m_pKey_C->Set_Scale(m_vScale_Key_C);
    m_pKey_ENTER->Set_Scale(m_vScale_Key_ENTER);

    m_pBackground_Close->Set_Scale(m_vScale_Background_Close);
    m_pSeperate_Line3->Set_Scale(m_vScale_SeperateLine3);
    m_pAnswer->Set_Scale(m_vScale_Answer);
    m_pSelector_0->Set_Scale(m_vScale_Selector_0);
    m_pSelector_1->Set_Scale(m_vScale_Selector_1);
    m_pSelector_2->Set_Scale(m_vScale_Selector_2);

//=========================================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pMovie->Set_State(TRANSFORM::POSITION, m_vPosition_Movie);
    m_pSeperate_Line1->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine1);
    m_pSeperate_Line2->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine2);
    m_pKey_C->Set_State(TRANSFORM::POSITION, m_vPosition_Key_C);
    m_pKey_ENTER->Set_State(TRANSFORM::POSITION, m_vPosition_Key_ENTER);

    m_pBackground_Close->Set_State(TRANSFORM::POSITION, m_vPosition_Background_Close);
    m_pSeperate_Line3->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine3);
    m_pAnswer->Set_State(TRANSFORM::POSITION, m_vPosition_Answer);
    m_pSelector_0->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_0);
    m_pSelector_1->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_1);
    m_pSelector_2->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_2);

//=========================================================================== Explanation
    m_strTitle          = L"카사네의 특수 공격";

    m_strExplanation1   = L"로 카사네에게 적절한 거리까지 스텝으로 후퇴하면서";
    m_strExplanation2   = L"무기 공격을 하는 ";
    m_strExplanation2_1 = L"[ 백스텝 공격 ]";
    m_strExplanation2_2 = L"이 발동.";
    m_strExplanation2_3 = L"염력게이지의 회복량이 높아,";
    m_strExplanation3   = L"콤보로 활용하면 염력 게이지를 유지하면서 싸울 수 있게 된다.";

    m_strEnter          = L"닫기";

//=========================================================================== Explanation
    m_strClose_Answer   = L"확인 중인 튜토리얼을 닫으시겠습니까?";
    m_strClose_Yes      = L"예";
    m_strClose_No       = L"아니오"; 
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Tick_UI(_float fTimeDelta)
{
    Movie_System(fTimeDelta);
    //Movie_Start();
    Movie_End(fTimeDelta);
    Movie_Really_Close(fTimeDelta);
    Key_Control();

    LightOn_Selectors(fTimeDelta);
    Blink_Selectors(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Movie_System(_float fTimeDelta)
{
#pragma region Movie_Start
    if (true == m_bMovieStart && false == m_bToolTip_End)
    {
        m_fMovieBackgroundTime += fTimeDelta;
        m_fMovieShaderTime  += fTimeDelta;
        m_fSeperateLineTime += fTimeDelta;

        // Appear Backgrounds
        if (0.9f <= m_fMovieBackgroundTime)
        {
            m_fMovieBackgroundTime = 0.9f;
        }
        if (1.f <= m_fMovieShaderTime)
        {
            m_fMovieShaderTime = 1.f;
        }
        if (0.5f <= m_fSeperateLineTime)
        {
            m_fSeperateLineTime = 0.5f;
        } 

        // Shift MovieIndex
        if (1.f <= m_fMovieShaderTime)
        {
            m_fCurrentMovieIndex += fTimeDelta * 30.f;  // FPS = 30

            if (MOVIE_MAX <= m_fCurrentMovieIndex)
            {
                m_fCurrentMovieIndex = 0.f;
            }
        }

        // Background Size Control
        if (true == m_bScaleUp)
        {
            m_fScaleX_ToolTip += fTimeDelta * 100.f;

            if (20.f <= m_fScaleX_ToolTip)
            {
                m_bScaleUp = false;
            }
            else
            {
                m_vScale_Background.x += m_fScaleX_ToolTip;
                m_vScale_Movie.x += m_fScaleX_ToolTip;

                m_pBackground->Set_Scale(m_vScale_Background);
                m_pMovie->Set_Scale(m_vScale_Movie);
            }
        }
        else
        {
            m_fScaleX_ToolTip -= fTimeDelta * 90.f;

            if (0.f >= m_fScaleX_ToolTip)
            {
                m_fScaleX_ToolTip = 0.f;
            }
            else
            {
                m_vScale_Background.x -= m_fScaleX_ToolTip;
                m_vScale_Movie.x -= m_fScaleX_ToolTip;

                m_pBackground->Set_Scale(m_vScale_Background);
                m_pMovie->Set_Scale(m_vScale_Movie);
            }
        }
    }
#pragma endregion

#pragma region Movie_End
    if (true == m_bToolTip_End)
    {
        if (0.f >= m_fMovieBackgroundTime)
        {
            m_fMovieBackgroundTime = 0.f;
        }
        else
        {
            m_fMovieBackgroundTime -= fTimeDelta;
        }

        if (0.f >= m_fMovieShaderTime)
        {
            m_fMovieShaderTime = 0.f;
        }
        else
        {
            m_fMovieShaderTime -= fTimeDelta;
        }

        if (0.f >= m_fSeperateLineTime)
        {
            m_fSeperateLineTime = 0.f;
        }
        else
        {
            m_fSeperateLineTime -= fTimeDelta;
        }

        if (0.f >= m_fMovieBackgroundTime && 0 >= m_fMovieShaderTime)
        {
            m_bMovieStart = false;
            m_fAllCloseDelayTime = 0.f;
        }
    }
#pragma endregion
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Key_Control()
{
    if (true == m_bCall_BackgroundClose && CGameInstance::Get_Instance()->Key_Down(VK_UP))
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Detail_Shift.wav", 1.f);

        m_iCloseIndex           = 0;

        m_vPosition_Answer      = _float4(-20.f, -33.f, 0.f, 1.f);
        m_vPosition_Selector_0  = _float4(-60.f, -32.f, 0.f, 1.f);
        m_vPosition_Selector_1  = _float4(-54.f, -32.f, 0.f, 1.f);
        m_vPosition_Selector_2  = _float4(-62.f, -32.f, 0.f, 1.f);

        m_pAnswer->Set_State(TRANSFORM::POSITION, m_vPosition_Answer);
        m_pSelector_0->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_0);
        m_pSelector_1->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_1);
        m_pSelector_2->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_2);

    }
    if (true == m_bCall_BackgroundClose && CGameInstance::Get_Instance()->Key_Down(VK_DOWN))
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Detail_Shift.wav", 1.f);

        m_iCloseIndex           = 1;

        m_vPosition_Answer      = _float4(-20.f, -91.f, 0.f, 1.f);
        m_vPosition_Selector_0  = _float4(-60.f, -90.f, 0.f, 1.f);
        m_vPosition_Selector_1  = _float4(-54.f, -90.f, 0.f, 1.f);
        m_vPosition_Selector_2  = _float4(-62.f, -90.f, 0.f, 1.f);

        m_pAnswer->Set_State(TRANSFORM::POSITION, m_vPosition_Answer);
        m_pSelector_0->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_0);
        m_pSelector_1->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_1);
        m_pSelector_2->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_2);
    }
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::LightOn_Selectors(_float fTimeDelta)
{
#pragma region Selector_1
    if (true == m_bLightOn_Selector_1)
    {
        if (0.7f <= m_fTimeSelector_1)
        {
            m_fTimeSelector_1 = 0.7f;
        }
        else
        {
            m_fTimeSelector_1 += fTimeDelta;
        }
    }
#pragma endregion

#pragma region Selector_2
    if (true == m_bLightOn_Selector_2)
    {
        if (0.7f <= m_fTimeSelector_2)
        {
            m_fTimeSelector_2 = 0.7f;
        }
        else
        {
            m_fTimeSelector_2 += fTimeDelta;
        }
    }
#pragma endregion

    if (0.7f <= m_fTimeSelector_1 && 0.7f <= m_fTimeSelector_2)
    {
        m_bBlinkOn = true;
    }
    else
    {
        m_bBlinkOn = false;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Blink_Selectors(_float fTimeDelta)
{
    if (true == m_bBlinkOn)
    {
        m_fBlinkTime += fTimeDelta * m_iBlinkIndex;

        if (0.5f <= m_fBlinkTime)
        {
            m_fBlinkTime = 0.5f;
            m_iBlinkIndex = -1;
        }
        if (0.f >= m_fBlinkTime)
        {
            m_fBlinkTime = 0.f;
            m_iBlinkIndex = 1;
        }
    }
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Sound(_float fTimeDelta)
{


}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Movie_Start()
{
    //if (CGameInstance::Get_Instance()->Key_Down(VK_F2))
    //{
    //    m_bMovieStart   = true;
    //    m_bToolTip_End  = false;
    //}
    m_bMovieStart = true;
    m_bToolTip_End = false;
    m_bIsRunning = true;

    auto pGameInstance = CGameInstance::Get_Instance();
    pGameInstance->Iterate_Layers(SCENE::TEST, [](pair<wstring, shared_ptr<CObjectLayer>> tLayerDesc)
        {
            if (tLayerDesc.first == LAYER_UI)
                return true;

            tLayerDesc.second->Set_TimeScale(0.f);

            return true;
        });
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Movie_End(_float fTimeDelta)
{
    if (true == m_bIsRunning && true == m_bMovieStart && false == m_bToolTip_End && false == m_bCall_BackgroundClose && CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    {
        m_bCall_BackgroundClose = true;
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.Open_ToolTip.wav", 1.f);
    }
    else if (true == m_bIsRunning && 0 == m_iCloseIndex && true == m_bCall_BackgroundClose && CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    {
        // All Close
        m_bIsReallyClose            = true;
        m_bScaleUp_Close            = true;
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.CloseYes_ToolTip.wav", 1.f);

        //m_bCall_BackgroundClose   = false;
        //m_bToolTip_End            = true;
    }
    else if (true == m_bIsRunning && 1 == m_iCloseIndex && true == m_bCall_BackgroundClose && CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    {
        // Close Only Small_Window
        m_bIsReallyClose            = true;
        m_bScaleUp_Close            = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Movie_Really_Close(_float fTimeDelta)
{
    if (true == m_bCall_BackgroundClose)
    {
        if (false == m_bIsReallyClose)
        {
            m_fCloseBackgroundTime      += fTimeDelta;
            m_fCloseSeperateLineTime    += fTimeDelta;
            m_fCloseAnswerTime          += fTimeDelta;

            // Set Alpha CloseBackground
            if (0.95f <= m_fCloseBackgroundTime)
            {
                m_fCloseBackgroundTime = 0.95f;
            }
            if (0.75f <= m_fCloseSeperateLineTime)
            {
                m_fCloseSeperateLineTime = 0.75f;
            }
            if (0.7f <= m_fCloseAnswerTime)
            {
                m_fCloseAnswerTime = 0.7f;
            }
            if (0.4f <= m_fCloseAnswerTime)
            {
                m_bLightOn_Selector_2 = true;
            }
            if (0.4f <= m_fTimeSelector_2)
            {
                m_bLightOn_Selector_1 = true;
            }

            // Background Size Control
            if (true == m_bScaleUp_Close)
            {
                m_fScaleX_CloseToolTip += fTimeDelta * 500.f;
                
                if (10.f <= m_fScaleX_CloseToolTip)
                {
                    m_bScaleUp_Close = false;
                }
                else
                {
                    m_vScale_Background_Close.x += m_fScaleX_CloseToolTip;
                    m_vScale_SeperateLine3.x    += m_fScaleX_CloseToolTip;

                    m_pBackground_Close->Set_Scale(m_vScale_Background_Close);
                    m_pSeperate_Line3->Set_Scale(m_vScale_SeperateLine3);
                }
            }
            else
            {
                m_fScaleX_CloseToolTip -= fTimeDelta * 500.f;

                if (0.f >= m_fScaleX_CloseToolTip)
                {
                    m_fScaleX_CloseToolTip = 0.f;
                }
                else
                {
                    m_vScale_Background_Close.x -= m_fScaleX_CloseToolTip;
                    m_vScale_SeperateLine3.x    -= m_fScaleX_CloseToolTip;

                    m_pBackground_Close->Set_Scale(m_vScale_Background_Close);
                    m_pSeperate_Line3->Set_Scale(m_vScale_SeperateLine3);
                }
            }
        }
        else
        {
            m_bLightOn_Selector_1       = false;
            m_bLightOn_Selector_2       = false;

            m_fCloseBackgroundTime      -= fTimeDelta;
            m_fCloseSeperateLineTime    -= fTimeDelta;
            m_fCloseAnswerTime          -= fTimeDelta;
            m_fTimeSelector_1           -= fTimeDelta;
            m_fTimeSelector_2           -= fTimeDelta;

            // Set Alpha CloseBackground
            if (0.f >= m_fCloseBackgroundTime)
            {
                m_fCloseBackgroundTime = 0.f;
            }
            if (0.f >= m_fCloseSeperateLineTime)
            {
                m_fCloseSeperateLineTime = 0.f;
            }
            if (0.f >= m_fCloseAnswerTime)
            {
                m_fCloseAnswerTime = 0.f;
            }
            if (0.f >= m_fTimeSelector_1)
            {
                m_fTimeSelector_1 = 0.f;
            }
            if (0.f >= m_fTimeSelector_2)
            {
                m_fTimeSelector_2 = 0.f;
            }

            // Background Size Control
            if (true == m_bScaleUp_Close)
            {
                m_fScaleX_CloseToolTip += fTimeDelta * 500.f;
                
                if (10.f <= m_fScaleX_CloseToolTip)
                {
                    m_bScaleUp_Close = false;
                    //m_bIsReallyClose = false;
                }
                else
                {
                    m_vScale_Background_Close.x += m_fScaleX_CloseToolTip;
                    m_vScale_SeperateLine3.x    += m_fScaleX_CloseToolTip;

                    m_pBackground_Close->Set_Scale(m_vScale_Background_Close);
                    m_pSeperate_Line3->Set_Scale(m_vScale_SeperateLine3);
                }
            }
            else
            {
                m_fScaleX_CloseToolTip -= fTimeDelta * 500.f;

                if (0.f >= m_fScaleX_CloseToolTip)
                {
                    m_fScaleX_CloseToolTip = 0.f;



                    if (0 == m_iCloseIndex)
                    {
                        if (1.f <= m_fAllCloseDelayTime)
                        {
                            m_fAllCloseDelayTime = 1.f;
                            m_bCall_BackgroundClose = false;
                            //m_bMovieStart = false;
                            m_bToolTip_End = true;

                            if (0 == m_iSoundCount && true == m_bIsRunning)
                            {
                                CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.CloseYes_ToolTip.wav", 1.f);

                                m_iSoundCount += 1;

                                auto pGameInstance = CGameInstance::Get_Instance();
                                pGameInstance->Iterate_Layers(SCENE::TEST, [](pair<wstring, shared_ptr<CObjectLayer>> tLayerDesc)
                                    {
                                        if (tLayerDesc.first == LAYER_UI)
                                            return true;

                                        tLayerDesc.second->Set_TimeScale(1.f);

                                        return true;
                                    });

                                m_bIsRunning = false;
                            }
                        }
                        else
                        {
                            m_fAllCloseDelayTime += fTimeDelta * 2.f;
                        }
                    }

                    if (1 == m_iCloseIndex)
                    {
                        m_vPosition_Answer          = _float4(-20.f, -33.f, 0.f, 1.f);
                        m_vPosition_Selector_0      = _float4(-60.f, -30.f, 0.f, 1.f);
                        m_vPosition_Selector_1      = _float4(-54.f, -30.f, 0.f, 1.f);
                        m_vPosition_Selector_2      = _float4(-62.f, -30.f, 0.f, 1.f);

                        m_pAnswer->Set_State(TRANSFORM::POSITION, m_vPosition_Answer);
                        m_pSelector_0->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_0);
                        m_pSelector_1->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_1);
                        m_pSelector_2->Set_State(TRANSFORM::POSITION, m_vPosition_Selector_2);

                        m_iCloseIndex = 0;
                    }
                }
                else
                {
                    if (0 == m_iSoundIndex)
                    {
                        //CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.CloseYes_ToolTip.wav", 1.f);

                        m_iSoundIndex += 1;

                    }
                    m_vScale_Background_Close.x -= m_fScaleX_CloseToolTip;
                    m_vScale_SeperateLine3.x    -= m_fScaleX_CloseToolTip;

                    m_pBackground_Close->Set_Scale(m_vScale_Background_Close);
                    m_pSeperate_Line3->Set_Scale(m_vScale_SeperateLine3);
                }
            }
        }
    }
}

shared_ptr<CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack> CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack> pInstance = make_private_shared(CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack> pInstance = make_private_shared_copy(CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
