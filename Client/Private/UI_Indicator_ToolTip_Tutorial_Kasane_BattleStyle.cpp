#include "ClientPCH.h"
#include "UI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle.h"
#include "GameInstance.h"

#define MOVIE_MAX       169

CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Initialize", "Failed to CGameObject::Initialize");
    }

    Initialize_UI();

    
    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render()
{
    if (true == m_bMovieStart)
    {
        /* Background_Frame */
        if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieBackgroundTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(37)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
        }

        /* Movie */
        if (FAILED(m_pTexture_Movie->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", (_uint)m_fCurrentMovieIndex)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pMovie->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(38)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
        }

        /* SepearateLine1 */
        if (FAILED(m_pTexture_SeperateLine1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pSeperate_Line1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(39)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
        }

        if (1 == m_iPageIndex)
        {
            /* Page_1 */
            if (FAILED(m_pTexture_Page_1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pPage_1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(51)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
            }
        }
        if (2 == m_iPageIndex)
        {
            /* Page_1 */
            if (FAILED(m_pTexture_Page_2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pPage_1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(51)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
            }
        }

        /* Page_Slash */
        if (FAILED(m_pTexture_Page_Slash->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pPage_Slash->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(51)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
        }

        /* Page_2 */
        if (FAILED(m_pTexture_Page_2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pPage_2->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(51)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
        }
        
        /* SepearateLine2 */
        if (FAILED(m_pTexture_SeperateLine2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pSeperate_Line2->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(39)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
        }

        if (2 == m_iPageIndex)
        {
            /* Key_VK_LButton */
            if (FAILED(m_pTexture_Key_VK_LButton->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pKey_VK_LButton->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(51)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Render", "Failed to CGameObject::Render");
            }
        }

#pragma region Key_Switch_L
        if (true == m_bSwitchStart_L)
        {
            /* Key_Switch_L2 */
            if (FAILED(m_pTexture_Key_Switch_L2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pKey_Switch_L2->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fAccTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(52)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
            }
        }

        if (true == m_bLightOn_L)
        {
            /* Key_Switch_L1 */
            if (FAILED(m_pTexture_Key_Switch_L1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pKey_Switch_L1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fAccTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(51)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
            }
        }
#pragma endregion

#pragma region Key_Switch_R
        if (true == m_bSwitchStart_R)
        {
            /* Key_Switch_R2 */
            if (FAILED(m_pTexture_Key_Switch_R2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pKey_Switch_R2->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fAccTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(52)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
            }
        }

        if (true == m_bLightOn_R)
        {
            /* Key_Switch_R1 */
            if (FAILED(m_pTexture_Key_Switch_R1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pKey_Switch_R1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fAccTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(51)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
            }
        }
#pragma endregion

        /* Key_Switch_Icon */
        if (FAILED(m_pTexture_Key_Switch_Icon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pKey_Switch_Icon->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(51)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
        }

        if (2 == m_iPageIndex)
        {
            /* Key_ENTER */
            if (FAILED(m_pTexture_Key_ENTER->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pKey_ENTER->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMovieShaderTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(38)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Render", "Failed to CGameObject::Render");
            }
        }

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTitle,             _float2(590.f, 110.f),  XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.9f);

        if (1 == m_iPageIndex)
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1,      _float2(603.f, 530.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Godo"),           m_strExplanation1_1,    _float2(773.f, 530.f), XMVectorSet(255.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1_2,    _float2(848.f, 530.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2,      _float2(603.f, 570.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Godo"),           m_strExplanation2_1,    _float2(773.f, 570.f), XMVectorSet(255.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2_2,    _float2(883.f, 570.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3,      _float2(603.f, 610.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation4,      _float2(603.f, 650.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPageSwitch,        _float2(1235.f, 783.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.45f);
        }

        if (2 == m_iPageIndex)
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation5,      _float2(638.f, 530.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Godo"),           m_strExplanation5_1,    _float2(813.f, 530.f), XMVectorSet(255.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, 180.f * m_fSeperateLineTime / 255.f, m_fMovieShaderTime), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation6,      _float2(603.f, 570.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.5f);

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPageSwitch,        _float2(1137.f, 783.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.45f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strEnter,             _float2(1282.f, 783.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fMovieShaderTime), 0.47f);
        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Ready_Components()
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

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Initialize_UI()
{
//=========================================================================== _bool
    m_bScaleUp                          = true;
    m_bMovieStart                       = false;
    m_bToolTip_End                      = false;
    m_bIsRunning                        = false;
    m_bLightOn_L                        = false;
    m_bLightOn_R                        = true;
    m_bSwitchStart_L                    = false;
    m_bSwitchStart_R                    = false;

//=========================================================================== _uint
    m_iPageIndex                        = 1;
    m_iTimePlusMinus                    = 1;
    m_iTimeIndexCount_L                 = 0;
    m_iTimeIndexCount_R                 = 0;
    m_iSoundIndex                       = 0;
    m_iSoundCount                       = 0;

//=========================================================================== _float
    m_fMovieShaderTime                  = 0.f;
    m_fMovieBackgroundTime              = 0.f;
    m_fSeperateLineTime                 = 0.f;

    m_fCurrentMovieIndex                = 1.f;
    m_fScaleX_ToolTip                   = 0.f;

    m_fAccTime                          = 0.f;
    m_fShaderTime                       = 0.f;

//=========================================================================== Transform
    m_pBackground                       = CTransform::Create(m_pDevice, m_pContext);
    m_pMovie                            = CTransform::Create(m_pDevice, m_pContext);
    m_pSeperate_Line1                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSeperate_Line2                   = CTransform::Create(m_pDevice, m_pContext);
    m_pPage_1                           = CTransform::Create(m_pDevice, m_pContext);
    m_pPage_Slash                       = CTransform::Create(m_pDevice, m_pContext);
    m_pPage_2                           = CTransform::Create(m_pDevice, m_pContext);

    m_pKey_VK_LButton                   = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_Switch_L1                    = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_Switch_L2                    = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_Switch_R1                    = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_Switch_R2                    = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_Switch_Icon                  = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_ENTER                        = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== Texture
    m_pTexture_Background               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background.png"));
    m_pTexture_Movie                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Tutorial_Tips/Kasane_BattleStyle/T_ui_TutoTips_076_01_common_MV_Movie_40534656.mkv_%d.png"), MOVIE_MAX, true);
    m_pTexture_SeperateLine1            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));
    m_pTexture_SeperateLine2            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/SeperateLine.png"));
    m_pTexture_Page_1                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/1.png"));
    m_pTexture_Page_Slash               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/Slash.png"));
    m_pTexture_Page_2                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/2.png"));

    m_pTexture_Key_VK_LButton           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/VK_LButton.png"));
    m_pTexture_Key_Switch_L1            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Swift_L1.png"));
    m_pTexture_Key_Switch_L2            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Swift_L2.png"));
    m_pTexture_Key_Switch_R1            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Swift_R1.png"));
    m_pTexture_Key_Switch_R2            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Swift_R2.png"));
    m_pTexture_Key_Switch_Icon          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Switch_Icon.png"));
    m_pTexture_Key_ENTER                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/ENTER.png"));

//=========================================================================== _float3
    m_vScale_Background                 = _float3(840.f, 760.f, 1.f);
    m_vScale_Movie                      = _float3(512.f, 256.f, 1.f);
    m_vScale_SeperateLine1              = _float3(780.f, 1.5f, 1.f);
    m_vScale_SeperateLine2              = _float3(780.f, 1.5f, 1.f);
    m_vScale_Page_1                     = _float3(14.f, 22.f, 1.f);
    m_vScale_Page_Slash                 = _float3(14.f, 22.f, 1.f);
    m_vScale_Page_2                     = _float3(14.f, 22.f, 1.f);

    m_vScale_Key_VK_LButton             = _float3(32.f, 31.f,  1.f);
    m_vScale_Key_Switch_L1              = _float3(64.f, 78.f, 1.f);
    m_vScale_Key_Switch_L2              = _float3(64.f, 78.f, 1.f);
    m_vScale_Key_Switch_R1              = _float3(64.f, 78.f, 1.f);
    m_vScale_Key_Switch_R2              = _float3(64.f, 78.f, 1.f);
    m_vScale_Key_Switch_Icon            = _float3(31.f, 31.f, 1.f);
    m_vScale_Key_ENTER                  = _float3(30.f, 30.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Background              = _float4(0.f, 90.f, 0.f, 1.f);
    m_vPosition_Movie                   = _float4(0.f, 200.f, 0.f, 1.f);
    m_vPosition_SeperateLine1           = _float4(0.f, 360.f, 0.f, 1.f);
    m_vPosition_SeperateLine2           = _float4(0.f, -220.f, 0.f, 1.f);
    m_vPosition_Page_1                  = _float4(300.f, -185.f, 0.f, 1.f);
    m_vPosition_Page_Slash              = _float4(320.f, -185.f, 0.f, 1.f);
    m_vPosition_Page_2                  = _float4(340.f, -185.f, 0.f, 1.f);

    m_vPosition_Key_VK_LButton          = _float4(-337.f, 0.f, 0.f, 1.f);
    m_vPosition_Key_Switch_L1           = _float4(-380.f, 45.f, 0.f, 1.f);
    m_vPosition_Key_Switch_L2           = _float4(-380.f, 45.f, 0.f, 1.f);
    m_vPosition_Key_Switch_R1           = _float4(380.f, 45.f, 0.f, 1.f);
    m_vPosition_Key_Switch_R2           = _float4(380.f, 45.f, 0.f, 1.f);
    m_vPosition_Key_Switch_Icon         = _float4(250.f, -253.f, 0.f, 1.f);
    m_vPosition_Key_ENTER               = _float4(300.f, -253.f, 0.f, 1.f);

//=========================================================================== Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pMovie->Set_Scale(m_vScale_Movie);
    m_pSeperate_Line1->Set_Scale(m_vScale_SeperateLine1);
    m_pSeperate_Line2->Set_Scale(m_vScale_SeperateLine2);
    m_pPage_1->Set_Scale(m_vScale_Page_1);
    m_pPage_Slash->Set_Scale(m_vScale_Page_Slash);
    m_pPage_2->Set_Scale(m_vScale_Page_2);

    m_pKey_VK_LButton->Set_Scale(m_vScale_Key_VK_LButton);
    m_pKey_Switch_L1->Set_Scale(m_vScale_Key_Switch_L1);
    m_pKey_Switch_L2->Set_Scale(m_vScale_Key_Switch_L2);
    m_pKey_Switch_R1->Set_Scale(m_vScale_Key_Switch_R1);
    m_pKey_Switch_R2->Set_Scale(m_vScale_Key_Switch_R2);
    m_pKey_Switch_Icon->Set_Scale(m_vScale_Key_Switch_Icon);
    m_pKey_ENTER->Set_Scale(m_vScale_Key_ENTER);


//=========================================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pMovie->Set_State(TRANSFORM::POSITION, m_vPosition_Movie);
    m_pSeperate_Line1->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine1);
    m_pSeperate_Line2->Set_State(TRANSFORM::POSITION, m_vPosition_SeperateLine2);
    m_pPage_1->Set_State(TRANSFORM::POSITION, m_vPosition_Page_1);
    m_pPage_Slash->Set_State(TRANSFORM::POSITION, m_vPosition_Page_Slash);
    m_pPage_2->Set_State(TRANSFORM::POSITION, m_vPosition_Page_2);

    m_pKey_VK_LButton->Set_State(TRANSFORM::POSITION, m_vPosition_Key_VK_LButton);
    m_pKey_Switch_L1->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Switch_L1);
    m_pKey_Switch_L2->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Switch_L2);
    m_pKey_Switch_R1->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Switch_R1);
    m_pKey_Switch_R2->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Switch_R2);
    m_pKey_Switch_Icon->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Switch_Icon);
    m_pKey_ENTER->Set_State(TRANSFORM::POSITION, m_vPosition_Key_ENTER);

//=========================================================================== Explanation
    m_strTitle          = L"카사네의 전투 스타일";
    m_strPageSwitch     = L"페이지 전환";
    m_strEnter          = L"닫기";

// PAGE 1
    m_strExplanation1   = L"카사네의 초뇌능력 ";
    m_strExplanation1_1 = L"[ 염력 ]";
    m_strExplanation1_2 = L"을 이용한 오브젝트 공격과";
    m_strExplanation2   = L"장비한 다수의 무기";
    m_strExplanation2_1 = L"[ 소형칼날 ]";
    m_strExplanation2_2 = L"을 통한 공격을 조합하여 싸운다.";
    m_strExplanation3   = L"원거리에서 넓은 범위를 공격하는 전법에 능할 뿐만 아니라";
    m_strExplanation4   = L"공중전에서도 강하다는 특징 또한 가지고 있다.";

// PAGE 2
    m_strExplanation5   = L"로 무기 공격을 하면";
    m_strExplanation5_1 = L"염력 게이지가 회복되기 때문에";
    m_strExplanation6   = L"무기와 염력 공격을 적절히 활용해서 적을 토벌하는 것이 좋다.";
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Tick_UI(_float fTimeDelta)
{
    Movie_System(fTimeDelta);
    //Movie_Start(fTimeDelta);
    Movie_End(fTimeDelta);
    PageSwitch();
    SwitchDatas(fTimeDelta);
    Sound(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Movie_System(_float fTimeDelta)
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

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::PageSwitch()
{
    /* Key Control */
    if (2 == m_iPageIndex)
    {
        if (CGameInstance::Get_Instance()->Key_Down(VK_LEFT))
        {
            m_bSwitchStart_L = true;
            m_bLightOn_L = false;

            if (0 >= m_iSoundIndex)
            {
                m_iSoundIndex = 0;
            }
            else
            {
                m_iSoundIndex -= 1;
            }
        }
    }
    if (1 == m_iPageIndex)
    {
        if (CGameInstance::Get_Instance()->Key_Down(VK_RIGHT))
        {
            m_bSwitchStart_R = true;
            m_bLightOn_R = false;

            if (1 <= m_iSoundIndex)
            {
                m_iSoundIndex = 1;
            }
            else
            {
                m_iSoundIndex += 1;
            }
        }
    }
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::SwitchDatas(_float fTimeDelta)
{
#pragma region Switch - Datas
    if (1 == m_iPageIndex)
    {
        m_vPosition_Key_Switch_Icon = _float4(250.f, -253.f, 0.f, 1.f);
        m_pKey_Switch_Icon->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Switch_Icon);
    }
    if (2 == m_iPageIndex)
    {
        m_vPosition_Key_Switch_Icon = _float4(155.f, -253.f, 0.f, 1.f);
        m_pKey_Switch_Icon->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Switch_Icon);
    }
#pragma endregion

#pragma region Switch - Key_LR_Shader
    // Click - VK_Right
    if (true == m_bSwitchStart_R)
    {
        m_fAccTime += fTimeDelta * m_iTimePlusMinus * 10.f;

        if (1.f <= m_fAccTime)
        {
            m_fAccTime = 1.f;
            m_bLightOn_R = true;
            m_iTimeIndexCount_R += 1;
            m_iTimePlusMinus *= -1;
        }
        if (0.f >= m_fAccTime && 1 <= m_iTimeIndexCount_R)
        {
            m_fAccTime = 0.f;
            m_bLightOn_R = false;
            m_iTimePlusMinus = 1;
            m_iPageIndex += 1;
            m_bLightOn_L = true;
            m_bSwitchStart_R = false;
        }
    }

    // Click - VK_Left
    if (true == m_bSwitchStart_L)
    {
        m_fAccTime += fTimeDelta * m_iTimePlusMinus * 10.f;

        if (1.f <= m_fAccTime)
        {
            m_fAccTime = 1.f;
            m_bLightOn_L = true;
            m_iTimeIndexCount_L += 1;
            m_iTimePlusMinus *= -1;
        }
        if (0.f >= m_fAccTime && 1 <= m_iTimeIndexCount_L)
        {
            m_fAccTime = 0.f;
            m_bLightOn_L = false;
            m_iTimePlusMinus = 1;
            m_iPageIndex -= 1;
            m_bLightOn_R = true;
            m_bSwitchStart_L = false;
        }
    }
#pragma endregion
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Sound(_float fTimeDelta)
{
    if (true == m_bMovieStart && false == m_bToolTip_End)
    {
        if (0 == m_iSoundCount)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.Open_ToolTip.wav", 1.5f);
            ++m_iSoundCount;
        }
    }
    if (true == m_bToolTip_End && 0 != m_iSoundCount)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_00.CloseYes_ToolTip.wav", 1.f);
        m_iSoundCount = 0;
    }

    // Sound LR
    if (true == m_bMovieStart && 2 == m_iPageIndex && CGameInstance::Get_Instance()->Key_Down(VK_LEFT))
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Shift.wav", 1.f);
    }
    if (true == m_bMovieStart && 1 == m_iPageIndex && CGameInstance::Get_Instance()->Key_Down(VK_RIGHT))
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Shift.wav", 1.f);
    }
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Movie_Start()
{
    //if (CGameInstance::Get_Instance()->Key_Down(VK_F5))
    //{
    //    m_bMovieStart = true;
    //    m_bToolTip_End = false;
    //}
    m_bMovieStart = true;
    m_bToolTip_End = false;
    m_bIsRunning    = true;

    auto pGameInstance = CGameInstance::Get_Instance();
    pGameInstance->Iterate_Layers(SCENE::TEST, [](pair<wstring, shared_ptr<CObjectLayer>> tLayerDesc)
        {
            if (tLayerDesc.first == LAYER_UI)
                return true;

            tLayerDesc.second->Set_TimeScale(0.f);

            return true;
        });
}

void CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Movie_End(_float fTimeDelta)
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

shared_ptr<CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle> CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle> pInstance = make_private_shared(CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle> pInstance = make_private_shared_copy(CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
