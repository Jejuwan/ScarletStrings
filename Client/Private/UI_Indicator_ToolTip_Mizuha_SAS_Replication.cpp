#include "ClientPCH.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Replication.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define MOVIE_MAX       176

CUI_Indicator_ToolTip_Mizuha_SAS_Replication::CUI_Indicator_ToolTip_Mizuha_SAS_Replication(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Initialize", "Failed to CGameObject::Initialize");
    }

    Initialize_UI();


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Render()
{
    if (true == m_bMovieStart)
    {
        /* Background_Frame */
        if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieBackgroundTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(37)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CGameObject::Render");
        }

        /* Movie */
        if (FAILED(m_pTexture_Movie->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", (_uint)m_fCurrentMovieIndex)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pMovie->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(38)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CGameObject::Render");
        }

        /* SepearateLine1 */
        if (FAILED(m_pTexture_SeperateLine1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pSeperate_Line1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(39)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CGameObject::Render");
        }

        /* SepearateLine2 */
        if (FAILED(m_pTexture_SeperateLine2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pSeperate_Line2->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(39)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CGameObject::Render");
        }

        /* Key_ENTER */
        if (FAILED(m_pTexture_Key_ENTER->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pKey_ENTER->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(65)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Render", "Failed to CGameObject::Render");
        }

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTitle,             _float2(590.f, 110.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.9f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1,      _float2(603.f, 500.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1_1,    _float2(915.f, 500.f), XMVectorSet(255.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2,      _float2(603.f, 540.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2_1,    _float2(710.f, 540.f), XMVectorSet(255.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3,      _float2(603.f, 580.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation4,      _float2(603.f, 620.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation5,      _float2(603.f, 690.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strEnter,             _float2(1282.f, 783.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.47f);
    }

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Initialize_UI()
{
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
    m_pKey_ENTER                        = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== Texture
    m_pTexture_Background               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png"));
    m_pTexture_Movie                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Mizuha_Tips/SAS_Replication/T_ui_TutoTips_026_01_common_m_MV_Movie_40534656.mkv_%d.png"), MOVIE_MAX, true);
    m_pTexture_SeperateLine1            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));
    m_pTexture_SeperateLine2            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));    
    m_pTexture_Key_ENTER                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/ENTER.png"));

//=========================================================================== _bool
    m_bScaleUp                          = true;
    m_bMovieStart                       = false;
    m_bToolTip_End                      = false;
    m_bIsRunning                        = false;

//=========================================================================== _bool
    m_iSoundIndex                       = 0;

//=========================================================================== _float3
    m_vScale_Background                 = _float3(840.f, 760.f, 1.f);
    m_vScale_Movie                      = _float3(512.f, 256.f, 1.f);
    m_vScale_SeperateLine1              = _float3(780.f, 1.5f, 1.f);
    m_vScale_SeperateLine2              = _float3(780.f, 1.5f, 1.f);
    m_vScale_Key_ENTER                  = _float3(30.f, 30.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Background              = _float4(0.f, 90.f, 0.f, 1.f);
    m_vPosition_Movie                   = _float4(0.f, 200.f, 0.f, 1.f);
    m_vPosition_SeperateLine1           = _float4(0.f, 360.f, 0.f, 1.f);
    m_vPosition_SeperateLine2           = _float4(0.f, -220.f, 0.f, 1.f);
    m_vPosition_Key_ENTER               = _float4(300.f, -253.f, 0.f, 1.f);

//=========================================================================== Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pMovie->Set_Scale(m_vScale_Movie);
    m_pSeperate_Line1->Set_Scale(m_vScale_SeperateLine1);
    m_pSeperate_Line2->Set_Scale(m_vScale_SeperateLine2);
    m_pKey_ENTER->Set_Scale(m_vScale_Key_ENTER);


//=========================================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pMovie->Set_State(TRANSFORM::POSITION, m_vPosition_Movie);
    m_pSeperate_Line1->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine1);
    m_pSeperate_Line2->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine2);
    m_pKey_ENTER->Set_State(TRANSFORM::POSITION, m_vPosition_Key_ENTER);

//=========================================================================== Explanation
    m_strTitle          = L"SAS 복제";

    m_strExplanation1   = L"복제 뇌능력을 빌려 염력으로 다루는";
    m_strExplanation1_1 = L"오브젝트를 늘릴 수 있다.";
    m_strExplanation2   = L"효과 중에는";
    m_strExplanation2_1 = L"적의 겉껍데기를 파괴하기 쉬워지며";
    m_strExplanation3   = L"한 번에 다수의 오브젝트로 공격할 수 있게 된다.";
    m_strExplanation4   = L"단숨에 몰아치고 싶을 때 발동하면 매우 효과적이다.";
    m_strExplanation5   = L"쿄카와의 유대가 깊어질수록 추가 효과가 늘어난다.";

    m_strEnter          = L"닫기";

}

void CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Tick_UI(_float fTimeDelta)
{
    Movie_System(fTimeDelta);
    //Movie_Start();
    Movie_End(fTimeDelta);
    Sound(fTimeDelta);
}

void CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Movie_System(_float fTimeDelta)
{
#pragma region Movie_Start
    if (true == m_bMovieStart && false == m_bToolTip_End)
    {
        m_fMovieBackgroundTime += fTimeDelta;
        m_fMovieShaderTime += fTimeDelta;
        m_fSeperateLineTime += fTimeDelta;

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
            m_fMovieBackgroundTime -= fTimeDelta * 2.f;
        }

        if (0.f >= m_fMovieShaderTime)
        {
            m_fMovieShaderTime = 0.f;
        }
        else
        {
            m_fMovieShaderTime -= fTimeDelta * 2.f;
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

void CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Sound(_float fTimeDelta)
{
    if (true == m_bMovieStart && false == m_bToolTip_End)
    {
        if (0 == m_iSoundIndex)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.Open_ToolTip.wav", 1.f);
            ++m_iSoundIndex;
        }
    }
    if (true == m_bToolTip_End && 0 != m_iSoundIndex)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.CloseYes_ToolTip.wav", 1.f);
        m_iSoundIndex = 0;
    }
}

void CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Movie_Start()
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
    pGameInstance->Iterate_Layers(SCENE::MIZUHA, [](pair<wstring, shared_ptr<CObjectLayer>> tLayerDesc)
        {
            if (tLayerDesc.first == LAYER_UI)
                return true;

            tLayerDesc.second->Set_TimeScale(0.f);

            return true;
        });
}

void CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Movie_End(_float fTimeDelta)
{
    if (true == m_bIsRunning && CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    {
        m_bToolTip_End = true;
        
        auto pGameInstance = CGameInstance::Get_Instance();
        pGameInstance->Iterate_Layers(SCENE::MIZUHA, [](pair<wstring, shared_ptr<CObjectLayer>> tLayerDesc)
            {
                if (tLayerDesc.first == LAYER_UI)
                    return true;

                tLayerDesc.second->Set_TimeScale(1.f);

                return true;
            });

        m_bIsRunning = false;
    }
}

shared_ptr<CUI_Indicator_ToolTip_Mizuha_SAS_Replication> CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_ToolTip_Mizuha_SAS_Replication> pInstance = make_private_shared(CUI_Indicator_ToolTip_Mizuha_SAS_Replication, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_ToolTip_Mizuha_SAS_Replication> pInstance = make_private_shared_copy(CUI_Indicator_ToolTip_Mizuha_SAS_Replication, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
