#include "ClientPCH.h"
#include "UI_Indicator_ToolTip_Tutorial_LockOn.h"
#include "GameInstance.h"

#define MOVIE_MAX		65

CUI_Indicator_ToolTip_Tutorial_LockOn::CUI_Indicator_ToolTip_Tutorial_LockOn(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_ToolTip_Tutorial_LockOn::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_LockOn::Initialize(any _aDesc)
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

void CUI_Indicator_ToolTip_Tutorial_LockOn::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_LockOn::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_ToolTip_Tutorial_LockOn::Render()
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

        /* Key_Q */
        if (FAILED(m_pTexture_Key_Q->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pKey_Q->Bind_OnShader(m_pShader)))
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

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTitle, _float2(590.f, 110.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.9f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(643.f, 530.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(603.f, 570.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(603.f, 630.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation4, _float2(603.f, 670.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strEnter, _float2(1282.f, 783.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.47f);
    }

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_LockOn::Ready_Components()
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

void CUI_Indicator_ToolTip_Tutorial_LockOn::Initialize_UI()
{
//=========================================================================== _uint
    m_iSoundIndex                       = 0;

//=========================================================================== _float
    m_fMovieShaderTime                  = 0.f;
    m_fMovieBackgroundTime              = 0.f;
    m_fSeperateLineTime                 = 0.f;

    m_fCurrentMovieIndex                = 1.f;
    m_fScaleX_ToolTip                   = 0.f;

//=========================================================================== Transform
    m_pBackground                       = CTransform::Create(m_pDevice, m_pContext);
    m_pMovie                            = CTransform::Create(m_pDevice, m_pContext);
    m_pSeperate_Line1                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSeperate_Line2                   = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_Q                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_ENTER                        = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== Texture
    m_pTexture_Background               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png"));
    m_pTexture_Movie                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Tutorial_Tips/LockOn/T_ui_TutoTips_007_01_common_f_MV_Movie_40534656.mkv_%d.png"), MOVIE_MAX, true);
    m_pTexture_SeperateLine1            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));
    m_pTexture_SeperateLine2            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));
    m_pTexture_Key_Q                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/Q.png"));
    m_pTexture_Key_ENTER                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/ENTER.png"));

//=========================================================================== _bool
    m_bScaleUp                          = true;
    m_bMovieStart                       = false;
    m_bToolTip_End                      = false;
    m_bIsRunning                        = false;

//=========================================================================== _float3
    m_vScale_Background                 = _float3(840.f, 760.f, 1.f);
    m_vScale_Movie                      = _float3(512.f, 256.f, 1.f);
    m_vScale_SeperateLine1              = _float3(780.f, 1.5f, 1.f);
    m_vScale_SeperateLine2              = _float3(780.f, 1.5f, 1.f);
    m_vScale_Key_Q                      = _float3(30.f, 30.f, 1.f);
    m_vScale_Key_ENTER                  = _float3(30.f, 30.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Background              = _float4(0.f, 90.f, 0.f, 1.f);
    m_vPosition_Movie                   = _float4(0.f, 200.f, 0.f, 1.f);
    m_vPosition_SeperateLine1           = _float4(0.f, 360.f, 0.f, 1.f);
    m_vPosition_SeperateLine2           = _float4(0.f, -220.f, 0.f, 1.f);
    m_vPosition_Key_Q                   = _float4(-340.f, 0.f, 0.f, 1.f);
    m_vPosition_Key_ENTER               = _float4(300.f, -253.f, 0.f, 1.f);

//=========================================================================== Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pMovie->Set_Scale(m_vScale_Movie);
    m_pSeperate_Line1->Set_Scale(m_vScale_SeperateLine1);
    m_pSeperate_Line2->Set_Scale(m_vScale_SeperateLine2);
    m_pKey_Q->Set_Scale(m_vScale_Key_Q);
    m_pKey_ENTER->Set_Scale(m_vScale_Key_ENTER);

//=========================================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pMovie->Set_State(TRANSFORM::POSITION, m_vPosition_Movie);
    m_pSeperate_Line1->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine1);
    m_pSeperate_Line2->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine2);
    m_pKey_Q->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Q);
    m_pKey_ENTER->Set_State(TRANSFORM::POSITION, m_vPosition_Key_ENTER);

//=========================================================================== Explanation
    m_strTitle          = L"록 온";

    m_strExplanation1   = L"로 적을 록 온.";
    m_strExplanation2   = L"한 번 더 누르면 해제할 수 있다.";
    m_strExplanation3   = L"공격이 맞히기 쉬워지기 때문에 적을 만나면";
    m_strExplanation4   = L"우선 록 온으로 적을 포착하는 것이 기본이다.";

    m_strEnter          = L"닫기";
}

void CUI_Indicator_ToolTip_Tutorial_LockOn::Tick_UI(_float fTimeDelta)
{
    Movie_System(fTimeDelta);
    //Movie_Start(fTimeDelta);
    Movie_End(fTimeDelta);
    Sound(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_LockOn::Movie_System(_float fTimeDelta)
{
#pragma region Movie_Start
    if (true == m_bMovieStart && false == m_bToolTip_End)
    {
        m_fMovieBackgroundTime  += fTimeDelta;
        m_fMovieShaderTime      += fTimeDelta;
        m_fSeperateLineTime     += fTimeDelta;

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
        
        if (1.f <= m_fMovieShaderTime)
        {
            m_fCurrentMovieIndex += fTimeDelta * 30.f;  // FPS = 30

            if (MOVIE_MAX <= m_fCurrentMovieIndex)
            {
                m_fCurrentMovieIndex = 0.f;
            }
        }

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
                m_vScale_Background.x   -= m_fScaleX_ToolTip;
                m_vScale_Movie.x        -= m_fScaleX_ToolTip;

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
        }
    }
#pragma endregion
}

void CUI_Indicator_ToolTip_Tutorial_LockOn::Sound(_float fTimeDelta)
{
    if (true == m_bMovieStart && false == m_bToolTip_End)
    {
        if (0 == m_iSoundIndex)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.Open_ToolTip.wav", 1.5f);
            ++m_iSoundIndex;
        }
    }
    if (true == m_bToolTip_End && 0 != m_iSoundIndex)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.CloseYes_ToolTip.wav", 1.f);
        m_iSoundIndex = 0;
    }
}

void CUI_Indicator_ToolTip_Tutorial_LockOn::Movie_Start()
{
    //if (CGameInstance::Get_Instance()->Key_Down(VK_F3))
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

void CUI_Indicator_ToolTip_Tutorial_LockOn::Movie_End(_float fTimeDelta)
{
    if (true == m_bIsRunning && CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    {
        m_bToolTip_End = true;

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

shared_ptr<CUI_Indicator_ToolTip_Tutorial_LockOn> CUI_Indicator_ToolTip_Tutorial_LockOn::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_LockOn> pInstance = make_private_shared(CUI_Indicator_ToolTip_Tutorial_LockOn, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "UI_Indicator_ToolTip_Tutorial_LockOn::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_ToolTip_Tutorial_LockOn::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_LockOn> pInstance = make_private_shared_copy(CUI_Indicator_ToolTip_Tutorial_LockOn, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "UI_Indicator_ToolTip_Tutorial_LockOn::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
