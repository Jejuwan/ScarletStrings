#include "ClientPCH.h"
#include "UI_Indicator_Communication.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Indicator_Communication::CUI_Indicator_Communication(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Communication::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Communication::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Initialize", "Failed to CGameObject::Initialize");
    }


    Initialize_UI();


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Communication::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Communication::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_5);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Communication::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
        && true == m_bRenderDegree
        && SCENE::HIDEOUT == CGameInstance::Get_Instance()->Current_Scene())
    {
        if (true == m_bRenderIconSwitch)
        {
            // Backgrounds
            if (FAILED(m_pTexture_Icon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pIcon_Background->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            if (false == m_bNearIcon)
            {
                // Icon_OFF
                if (FAILED(m_pTexture_Icon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pIcon_Off->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
            }
            else
            {
                // Icon_ON
                if (FAILED(m_pTexture_Icon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pIcon_On->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
            }
        }

        if (true == m_bRenderMenuSwitch)
        {
            // Line0
            if (FAILED(m_pTexture_Line->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pLine[0]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fLineAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(38)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            // Menu
            if (FAILED(m_pTexture_Menu->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pMenu->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMenuAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(38)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            // Selector0
            if (FAILED(m_pTexture_Selector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSelector[0]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fSelectorAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(53)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            // Selector1
            if (FAILED(m_pTexture_Selector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSelector[1]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fSelectorAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(53)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            // Selector2
            if (FAILED(m_pTexture_Selector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSelector[2]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fSelectorAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fBlinkTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(54)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            // Selector3
            if (FAILED(m_pTexture_Selector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSelector[3]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fSelectorAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fBlinkTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(54)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strMenuTag[0], _float2(1310.f, 757.f), XMVectorSet(255.f * m_fSelectorAlpha / 255.f, 233.f * m_fSelectorAlpha/ 255.f, 170.f * m_fSelectorAlpha/ 255.f, m_fSelectorAlpha * 1.f), 0.55f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strMenuTag[1], _float2(1310.f, 807.f), XMVectorSet(255.f * m_fSelectorAlpha / 255.f, 233.f * m_fSelectorAlpha/ 255.f, 170.f * m_fSelectorAlpha/ 255.f, m_fSelectorAlpha * 1.f), 0.55f);
        }

        if (true == m_bRenderGiftWindow)
        {
            // WindowBackground
            if (FAILED(m_pTexture_WindowBackground->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pWindow_Backgrouund->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowBackgroundAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(58)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            // WindowTitle
            if (FAILED(m_pTexture_WindowTitle->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pWindow_Title->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(73)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            if (2 == m_iGiftWindowOpen_Phase)
            {
                // ProfileFrameUp
                if (FAILED(m_pTexture_ProfileFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileFrames[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // Profile - Arasi
                if (FAILED(m_pTexture_Profile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfile->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(79)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ProfileFrame - Main
                if (FAILED(m_pTexture_ProfileFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileFrame->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ProfileFrameDw
                if (FAILED(m_pTexture_ProfileFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileFrames[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                
                // Line1
                if (FAILED(m_pTexture_Line->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLine[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // GiftListFrame
                if (FAILED(m_pTexture_GiftFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGiftFrames[2]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(84)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // GiftIcon0
                if (FAILED(m_pTexture_GiftIcon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGiftIcon[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // GiftIcon1
                if (FAILED(m_pTexture_GiftIcon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGiftIcon[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // GiftFrameUp
                if (FAILED(m_pTexture_GiftFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGiftFrames[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // GiftFrameDw
                if (FAILED(m_pTexture_GiftFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGiftFrames[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateCharacter0 - Kasane
                if (FAILED(m_pTexture_ActivateCharacter->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateCharacter[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // MiniFillter
                if (FAILED(m_pTexture_WindowBackground->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pMiniFillter->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowBackgroundAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(58)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateCharacter1 - Siden
                if (FAILED(m_pTexture_ActivateCharacter->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateCharacter[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateCharacter2 - Arasi
                if (FAILED(m_pTexture_ActivateCharacter->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateCharacter[2]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateCharacter3 - Kyoka
                if (FAILED(m_pTexture_ActivateCharacter->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateCharacter[3]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateCharacter4 - Kagero
                if (FAILED(m_pTexture_ActivateCharacter->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateCharacter[4]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                // ActivateProfile0
                if (FAILED(m_pTexture_ActivateProfile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateProfile[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateProfile1
                if (FAILED(m_pTexture_ActivateProfile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateProfile[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateProfile2
                if (FAILED(m_pTexture_ActivateProfile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateProfile[2]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateProfile3
                if (FAILED(m_pTexture_ActivateProfile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateProfile[3]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // ActivateProfile4
                if (FAILED(m_pTexture_ActivateProfile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pActivateProfile[4]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(83)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // Caprisun
                if (FAILED(m_pTexture_Gift->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 10)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGift ->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // Button - Enter
                if (FAILED(m_pTexture_Button->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pButton[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }

                // Button - Enter
                if (FAILED(m_pTexture_Button->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pButton[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fWindowTitleAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }
                if (FAILED(__super::Render(38)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                }


                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGiftWindowTitle, _float2(215.f, 222.f), XMVectorSet(0.f, 0.f, 0.f, m_fTextAlpha * 1.f), 0.4f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strArasiSpring, _float2(440.f, 285.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLove, _float2(478.f, 337.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGift, _float2(335.f, 432.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGiftPossesion, _float2(860.f, 430.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGift, _float2(1130.f, 290.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.58f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHowManyPossese, _float2(1470.f, 335.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGiftPossesion, _float2(1550.f, 335.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strMaxPossese, _float2(1580.f, 335.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplain[0], _float2(1080.f, 735.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplain[1], _float2(1080.f, 775.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strButton[0], _float2(1530.f, 900.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strButton[1], _float2(1630.f, 900.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.45f);


                if (true == m_bOpenReAnswer)
                {
                    // ReAnswer - Background
                    if (FAILED(m_pTexture_Menu->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pReAnswer->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fReAnswerAlpha, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }
                    if (FAILED(__super::Render(38)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }

                    // Line2
                    if (FAILED(m_pTexture_Line->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pLine[2]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fReAnswerAlpha, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }
                    if (FAILED(__super::Render(38)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }

                    // ReAnswerSelector0
                    if (FAILED(m_pTexture_Selector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pReAnswerSelector[0]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fSelectorAlpha, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }
                    if (FAILED(__super::Render(53)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }

                    // Selector1
                    if (FAILED(m_pTexture_Selector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pReAnswerSelector[1]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fReAnswerSelectorAlpha, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }
                    if (FAILED(__super::Render(53)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }

                    // Selector2
                    if (FAILED(m_pTexture_Selector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pReAnswerSelector[2]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fReAnswerSelectorAlpha, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fReAnswerBlinkTime, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
                    }
                    if (FAILED(__super::Render(54)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }

                    // Selector3
                    if (FAILED(m_pTexture_Selector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pReAnswerSelector[3]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fReAnswerSelectorAlpha, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }
                    if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fReAnswerBlinkTime, sizeof(_float))))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCCL::Render", "Failed to CTexture::Bind_RawValue");
                    }
                    if (FAILED(__super::Render(54)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
                    }


                    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strReAnswer[0], _float2(g_iWinCX * 0.5f - 100.f, g_iWinCY * 0.5f - 70.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strReAnswer[1], _float2(g_iWinCX * 0.5f - 25.f, g_iWinCY * 0.5f - 5.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strReAnswer[2], _float2(g_iWinCX * 0.5f - 25.f, g_iWinCY * 0.5f + 45.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
                }
            }
        }

        if (1 == CUI_Manager::Get_Instance()->Get_ArasiJoinParty())
        {
            // Join - Background
            if (FAILED(m_pTexture_Menu->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pJoinBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fJoinAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(38)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }

            // Join - Enter
            if (FAILED(m_pTexture_Button->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pEnterAgain->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fJoinAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(38)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strJoinParty,         _float2(g_iWinCX * 0.5f - 150.f, g_iWinCY * 0.5f - 50.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strJoinParty_Check,   _float2(g_iWinCX * 0.5f + 110.f, g_iWinCY * 0.5f + 50.f), XMVectorSet(1.f, 1.f, 1.f, m_fTextAlpha * 1.f), 0.5f);
        }
    }

    return S_OK;
}

void CUI_Indicator_Communication::Initialize_UI()
{
//============================================================= CTransform
    m_pIcon_Background              = CTransform::Create(m_pDevice, m_pContext);
    m_pIcon_On                      = CTransform::Create(m_pDevice, m_pContext);
    m_pIcon_Off                     = CTransform::Create(m_pDevice, m_pContext);
    m_pMenu                         = CTransform::Create(m_pDevice, m_pContext);
    for (size_t i = 0; i < 4; i++)
    {
        m_pSelector[i]              = CTransform::Create(m_pDevice, m_pContext);
    }
    for (size_t i = 0; i < 3; i++)
    {
        m_pLine[i] = CTransform::Create(m_pDevice, m_pContext);
    }
    m_pWindow_Backgrouund           = CTransform::Create(m_pDevice, m_pContext);
    m_pWindow_Title                 = CTransform::Create(m_pDevice, m_pContext);
    for (size_t i = 0; i < 2; i++)
    {
        m_pProfileFrames[i]         = CTransform::Create(m_pDevice, m_pContext);
    }
    m_pProfileFrame                 = CTransform::Create(m_pDevice, m_pContext);
    m_pProfile                      = CTransform::Create(m_pDevice, m_pContext);
    for (size_t i = 0; i < 3; i++)
    {
        m_pGiftFrames[i]            = CTransform::Create(m_pDevice, m_pContext);
    }
    for (size_t i = 0; i < 2; i++)
    {
        m_pGiftIcon[i]              = CTransform::Create(m_pDevice, m_pContext);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateProfile[i]       = CTransform::Create(m_pDevice, m_pContext);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateCharacter[i]     = CTransform::Create(m_pDevice, m_pContext);
    }
    m_pMiniFillter                  = CTransform::Create(m_pDevice, m_pContext);
    m_pGift                         = CTransform::Create(m_pDevice, m_pContext);
    for (size_t i = 0; i < 2; i++)
    {
        m_pButton[i]                = CTransform::Create(m_pDevice, m_pContext);
    }
    m_pReAnswer                     = CTransform::Create(m_pDevice, m_pContext);
    for (size_t i = 0; i < 4; i++)
    {
        m_pReAnswerSelector[i]      = CTransform::Create(m_pDevice, m_pContext);
    }
    m_pJoinBackground               = CTransform::Create(m_pDevice, m_pContext);
    m_pEnterAgain                   = CTransform::Create(m_pDevice, m_pContext);

//============================================================= CTexture
    m_pTexture_Icon                 = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Icon%d.png"), 3, true);
    m_pTexture_Menu                 = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Menu.png"));
    m_pTexture_Selector             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Selector%d.png"), 4, true);
    m_pTexture_Line                 = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Line.png"));
    m_pTexture_WindowBackground     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png"));
    m_pTexture_WindowTitle          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/GetSkillTag.png"));
    m_pTexture_ProfileFrame         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/ProfileFrame%d.png"), 3, true);
    m_pTexture_GiftFrame            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/GiftFrame%d.png"), 3, true);
    m_pTexture_Profile              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/ProfileArasi.png"));
    m_pTexture_GiftIcon             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/GiftIcon%d.png"), 2, true);
    m_pTexture_ActivateProfile      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Mini_ProfileFrame.png"));
    m_pTexture_ActivateCharacter    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Face%d.png"), 5, true);
    m_pTexture_Gift                 = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Gift%d.png"), 11, true);
    m_pTexture_Button               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Communication/Button%d.png"), 2, true);

//============================================================= _bool
    m_bRenderIconSwitch             = true;
    m_bRenderDegree                 = false;
    m_bRenderGiftWindow             = false;
    m_bNearIcon                     = false;
    m_bRenderMenuSwitch             = false;
    m_bCanKeyInput                  = false;
    m_bScaleDoneTitle               = false;
    m_bScaleDoneWindow              = false;
    m_bOpenReAnswer                 = false;
    m_bCloseReAnswer                = false;
    m_bJoin                         = false;
//============================================================= _int
    m_iBlinkIndex                   = 1;
    m_iOpenSystem_Phase             = 0;
    m_iOpenMenu_Squence             = 0;
    m_iMenu_SelectorIndex           = 0;
    m_iGiftWindowOpen_Phase         = 0;
    m_iLove                         = 1;
    m_iPossesion                    = 0;
    m_iReAnswerOpen_Phase           = 0;
    m_iReAnswerBlinkIndex           = 1;
    m_iMenu_ReAnswerSelectorIndex   = 0;
    m_iJoin_Phase                   = 0;
//============================================================= _float
    m_fSelectorAlpha                = 0.f;
    m_fReAnswerSelectorAlpha        = 0.f;
    m_fBlinkTime                    = 0.f;
    m_fLineAlpha                    = 0.f;
    m_fMenuAlpha                    = 0.f;
    m_fWindowBackgroundAlpha        = 0.85f;
    m_fWindowTitleAlpha             = 0.f;
    m_fTextAlpha                    = 0.f;
    m_fReAnswerAlpha                = 0.f;
    m_fReAnswerBlinkTime            = 0.f;
    m_fJoinAlpha                    = 0.f;
    m_fAccTime                      = 0.f;

//============================================================= _float3
    m_vScale_Icon_Background        = _float3(64.f, 64.f, 1.f);
    m_vScale_Icon_On                = _float3(28.f, 28.f, 1.f);
    m_vScale_Icon_Off               = _float3(28.f, 28.f, 1.f);
    m_vScale_Menu                   = _float3(400.f, 3.f, 1.f); // y = 160
    m_vScale_Selector[0]            = _float3(204.f, 42.f, 1.f);
    m_vScale_Selector[1]            = _float3(41.f, 41.f, 1.f);
    m_vScale_Selector[2]            = _float3(16.f, 27.f, 1.f);
    m_vScale_Selector[3]            = _float3(15.f, 15.f, 1.f);
    m_vScale_Line[0]                = _float3(1.f, 3.f, 1.f);
    m_vScale_Line[1]                = _float3(350.f, 1.f, 1.f);
    m_vScale_Line[2]                = _float3(350.f, 1.f, 1.f);
    m_vScale_WindowBackground       = _float3(1400.f, 700.f, 1.f);
    m_vScale_WindowTitle            = _float3(1403.f, 26.f ,1.f);
    m_vScale_ProfileFrameUpDw       = _float3(642.f, 33.f, 1.f);
    m_vScale_ProfileFrame           = _float3(248.f, 100.f, 1.f);
    m_vScale_Profile                = _float3(138.f, 86.f, 1.f);
    m_vScale_GiftFrameUp            = _float3(670.f, 124.f, 1.f);
    m_vScale_GiftFrameDw            = _float3(670.f, 36.f, 1.f);
    m_vScale_GiftListFrame          = _float3(642.f, 65.f, 1.f);
    m_vScale_GiftIcon[0]            = _float3(32.f, 32.f, 1.f);
    m_vScale_GiftIcon[1]            = _float3(65.f, 65.f, 1.f);
    m_vScale_GiftFrameUp            = _float3(670.f, 124.f, 1.f);
    m_vScale_GiftFrameDw            = _float3(670.f, 36.f, 1.f);
    m_vScale_ActivateProfile        = _float3(66.f, 56.f, 1.f);
    m_vScale_ActivateCharacter      = _float3(62.f, 52.f, 1.f);
    m_vScale_MiniFillter            = _float3(64.f, 54.f, 1.f);
    m_vScale_Gift                   = _float3(500.f, 250.f, 1.f);
    m_vScale_Button                 = _float3(32.f, 32.f, 1.f);
    m_vScale_ReAnswer               = _float3(350.f, 200.f, 1.f);
    m_vScale_ReAnswerSelector[0]    = _float3(204.f, 42.f, 1.f);
    m_vScale_ReAnswerSelector[1]    = _float3(41.f, 41.f, 1.f);
    m_vScale_ReAnswerSelector[2]    = _float3(16.f, 27.f, 1.f);
    m_vScale_ReAnswerSelector[3]    = _float3(15.f, 15.f, 1.f);
    m_vScale_JoinParty              = _float3(350.f, 200.f, 1.f);

//============================================================= _float4
    m_vPosition_Icon                = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Menu                = _float4(460.f, -260.f, 0.f, 1.f);
    m_vPosition_Selector[0]         = _float4(390.f, -230.f, 0.f, 1.f);
    m_vPosition_Selector[1]         = _float4(310.f, -230.f, 0.f, 1.f);
    m_vPosition_Selector[2]         = _float4(316.f, -230.f, 0.f, 1.f);
    m_vPosition_Selector[3]         = _float4(308.f, -230.f, 0.f, 1.f);
    m_vPosition_WindowBackground    = _float4(0.f, -50.f, 0.f, 1.f);
    m_vPosition_WindowTitle         = _float4(0.f, 310.f, 0.f, 1.f);
    m_vPosition_ProfileFrameUpDw[0] = _float4(-380.f, 270.f, 0.f, 1.f);
    m_vPosition_ProfileFrame        = _float4(-560.f, 220.f, 0.f, 1.f);
    m_vPosition_ProfileFrameUpDw[1] = _float4(-380.f, 170.f, 0.f, 1.f);
    m_vPosition_Profile             = _float4(-608.f, 218.f, 0.f, 1.f);
    m_vPosition_Line[0]             = _float4(-268.f, 215.f, 0.f, 1.f);
    m_vPosition_Line[1]             = _float4(0.f, 30.f, 0.f, 1.f);
    m_vPosition_GiftListFrame       = _float4(-380.f, 100.f, 0.f, 1.f);
    m_vPosition_GiftIcon[0]         = _float4(-668.f, 100.f, 0.f, 1.f);
    m_vPosition_GiftIcon[1]         = _float4(120.f, 225.f, 0.f, 1.f);
    m_vPosition_GiftFrameUpDw[0]    = _float4(380.f, 226.f, 0.f, 1.f);
    m_vPosition_GiftFrameUpDw[1]    = _float4(380.f, -330.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[0]  = _float4(130.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[1]  = _float4(200.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[2]  = _float4(270.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[3]  = _float4(340.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[4]  = _float4(410.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[0] = _float4(130.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[1] = _float4(200.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[2] = _float4(270.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[3] = _float4(340.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[4] = _float4(410.f, 130.f, 0.f, 1.f);
    m_vPosition_MiniFillter         = _float4(130.f, 130.f, 0.f, 1.f);
    m_vPosition_Gift                = _float4(380.f, -40.f, 0.f, 1.f);
    m_vPosition_Button[0]           = _float4(550.f, -370.f, 0.f, 1.f);
    m_vPosition_Button[1]           = _float4(650.f, -370.f, 0.f, 1.f);
    m_vPosition_ReAnswer            = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_ReAnswerSelector[0] = _float4(0.f, -10.f, 0.f, 1.f);
    m_vPosition_ReAnswerSelector[1] = _float4(-80.f, -10.f, 0.f, 1.f);
    m_vPosition_ReAnswerSelector[2] = _float4(-74.f, -10.f, 0.f, 1.f);
    m_vPosition_ReAnswerSelector[3] = _float4(-82.f, -10.f, 0.f, 1.f);
    m_vPosition_JoinParty           = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_EnterAgain          = _float4(70.f, -60.f, 0.f, 1.f);
//============================================================= Set_Scale
    m_pIcon_Background->Set_Scale(m_vScale_Icon_Background);
    m_pIcon_On->Set_Scale(m_vScale_Icon_On);
    m_pIcon_Off->Set_Scale(m_vScale_Icon_Off);
    m_pMenu->Set_Scale(m_vScale_Menu);
    for (size_t i = 0; i < 4; i++)
    {
        m_pSelector[i]->Set_Scale(m_vScale_Selector[i]);
    }
    for (size_t i = 0; i < 3; i++)
    {
        m_pLine[i]->Set_Scale(m_vScale_Line[i]);
    }
    m_pWindow_Backgrouund->Set_Scale(m_vScale_WindowBackground);
    m_pWindow_Title->Set_Scale(m_vScale_WindowTitle);
    for (size_t i = 0; i < 2; i++)
    {
        m_pProfileFrames[i]->Set_Scale(m_vScale_ProfileFrameUpDw);
    }
    m_pProfileFrame->Set_Scale(m_vScale_ProfileFrame);
    m_pProfile->Set_Scale(m_vScale_Profile);

    m_pGiftFrames[0]->Set_Scale(m_vScale_GiftFrameUp);
    m_pGiftFrames[1]->Set_Scale(m_vScale_GiftFrameDw);
    m_pGiftFrames[2]->Set_Scale(m_vScale_GiftListFrame);
    for (size_t i = 0; i < 2; i++)
    {
        m_pGiftIcon[i]->Set_Scale(m_vScale_GiftIcon[i]);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateProfile[i]->Set_Scale(m_vScale_ActivateProfile);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateCharacter[i]->Set_Scale(m_vScale_ActivateCharacter);
    }
    m_pMiniFillter->Set_Scale(m_vScale_MiniFillter);
    m_pGift->Set_Scale(m_vScale_Gift);
    for (size_t i = 0; i < 2; i++)
    {
        m_pButton[i]->Set_Scale(m_vScale_Button);
    }
    m_pReAnswer->Set_Scale(m_vScale_ReAnswer);
    for (size_t i = 0; i < 4; i++)
    {
        m_pReAnswerSelector[i]->Set_Scale(m_vScale_ReAnswerSelector[i]);
    }
    m_pJoinBackground->Set_Scale(m_vScale_JoinParty);
    m_pEnterAgain->Set_Scale(m_vScale_Button);
//============================================================= Set_Position
    m_pIcon_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);
    m_pIcon_On->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);
    m_pIcon_Off->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);
    m_pMenu->Set_State(TRANSFORM::POSITION, m_vPosition_Menu);
    for (size_t i = 0; i < 4; i++)
    {
        m_pSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Selector[i]);
    }
    m_pLine[0]->Set_State(TRANSFORM::POSITION, m_vPosition_Menu);
    for (size_t i = 1; i < 3; i++)
    {
        m_pLine[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Line[i - 1]);
    }
    m_pWindow_Backgrouund->Set_State(TRANSFORM::POSITION, m_vPosition_WindowBackground);
    m_pWindow_Title->Set_State(TRANSFORM::POSITION, m_vPosition_WindowTitle);
    for (size_t i = 0; i < 2; i++)
    {
        m_pProfileFrames[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameUpDw[i]);
    }
    m_pProfileFrame->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrame);
    m_pProfile->Set_State(TRANSFORM::POSITION, m_vPosition_Profile);
    m_pGiftFrames[0]->Set_State(TRANSFORM::POSITION, m_vPosition_GiftFrameUpDw[0]);
    m_pGiftFrames[1]->Set_State(TRANSFORM::POSITION, m_vPosition_GiftFrameUpDw[1]);
    m_pGiftFrames[2]->Set_State(TRANSFORM::POSITION, m_vPosition_GiftListFrame);
    for (size_t i = 0; i < 2; i++)
    {
        m_pGiftIcon[i]->Set_State(TRANSFORM::POSITION, m_vPosition_GiftIcon[i]);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateProfile[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ActivateProfile[i]);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateCharacter[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ActivateCharacter[i]);
    }
    m_pMiniFillter->Set_State(TRANSFORM::POSITION, m_vPosition_MiniFillter);
    m_pGift->Set_State(TRANSFORM::POSITION, m_vPosition_Gift);
    for (size_t i = 0; i < 2; i++)
    {
        m_pButton[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Button[i]);
    }
    m_pReAnswer->Set_State(TRANSFORM::POSITION, m_vPosition_ReAnswer);
    for (size_t i = 0; i < 4; i++)
    {
        m_pReAnswerSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ReAnswerSelector[i]);
    }
    m_pJoinBackground->Set_State(TRANSFORM::POSITION, m_vPosition_JoinParty);
    m_pEnterAgain->Set_State(TRANSFORM::POSITION, m_vPosition_EnterAgain);

//============================================================= wstring
    m_strMenuTag[0]                 = TEXT("선물을 준다");
    m_strMenuTag[1]                 = TEXT("그만둔다");

    m_strGiftWindowTitle            = TEXT("선물");

    m_strArasiSpring                = TEXT("아라시 스프링");
    m_strLove                       = std::to_wstring(m_iLove);

    m_strGift                       = TEXT("즐거운 과자 모음");
    m_strGiftPossesion              = TEXT("");
    m_strHowManyPossese             = TEXT("소지 수");
    m_strMaxPossese                 = TEXT("/ 99");
    m_strExplain[0]                 = TEXT("달콤한 쿠키에 짭잘한 감자칩이다.");
    m_strExplain[1]                 = TEXT("누군가 받으면 굉장히 좋아할 것 같다.");
    m_strButton[0]                  = TEXT("결정");
    m_strButton[1]                  = TEXT("뒤로");

    m_strReAnswer[0]                = TEXT("선물을 건네시겠습니까?");
    m_strReAnswer[1]                = TEXT("예");
    m_strReAnswer[2]                = TEXT("아니오");

    m_strJoinParty                  = TEXT("아라시가 파티에 합류했다.");
    m_strJoinParty_Check            = TEXT("확인");
}

void CUI_Indicator_Communication::Tick_UI(_float fTimeDelta)
{
    //TestKey();

    Communication_System(fTimeDelta);
}

void CUI_Indicator_Communication::TestKey()
{
    if (false == m_bRenderIconSwitch)
    {
        if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
        {
            m_bRenderIconSwitch = true;
        }
    }
    else
    {
        if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
        {
            m_bRenderIconSwitch = false;
        }
        if (true == m_bNearIcon)
        {
            if (CGameInstance::Get_Instance()->Key_Down(VK_BACK))
            {
                m_bNearIcon = false;
            }
        }
        else
        {
            if (CGameInstance::Get_Instance()->Key_Down(VK_BACK))
            {
                m_bNearIcon = true;
            }
        }
    }
}

void CUI_Indicator_Communication::Communication_System(_float fTimeDelta)
{
    // always
    KeyInput();


    Open_Menu(fTimeDelta);
    if (true == m_bOpenReAnswer)
    {
        Open_ReAnswer(fTimeDelta);
    }
    if (true == m_bCloseReAnswer)
    {
        Close_ReAnswer(fTimeDelta);
    }

    if (1 == CUI_Manager::Get_Instance()->Get_ArasiJoinParty())
    {
        Arash_JoinParty(fTimeDelta);
    }

}

void CUI_Indicator_Communication::KeyInput()
{
    if (0 == m_iOpenSystem_Phase && true == m_bRenderIconSwitch && true == m_bNearIcon && false == m_bRenderMenuSwitch)
    {
        if (CGameInstance::Get_Instance()->Key_Down('E'))
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);

            Set_Render_Icon_OnOff(false);
            Set_Render_Menu_OnOff(true);
            m_iOpenSystem_Phase = 1;
        }
    }

    if (true == m_bCanKeyInput)
    {
        if (CGameInstance::Get_Instance()->Key_Down(VK_UP))
        {
            m_vPosition_Selector[0] = _float4(390.f, -230.f, 0.f, 1.f);
            m_vPosition_Selector[1] = _float4(310.f, -230.f, 0.f, 1.f);
            m_vPosition_Selector[2] = _float4(316.f, -230.f, 0.f, 1.f);
            m_vPosition_Selector[3] = _float4(308.f, -230.f, 0.f, 1.f);

            for (size_t i = 0; i < 4; i++)
            {
                m_pSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Selector[i]);
            }
            //CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Detail_Shift.wav", 1.f);

            m_iMenu_SelectorIndex = 0;
        }
        if (CGameInstance::Get_Instance()->Key_Down(VK_DOWN))
        {
            m_vPosition_Selector[0] = _float4(390.f, -280.f, 0.f, 1.f);
            m_vPosition_Selector[1] = _float4(310.f, -280.f, 0.f, 1.f);
            m_vPosition_Selector[2] = _float4(316.f, -280.f, 0.f, 1.f);
            m_vPosition_Selector[3] = _float4(308.f, -280.f, 0.f, 1.f);

            for (size_t i = 0; i < 4; i++)
            {
                m_pSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Selector[i]);
            }
            //CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Detail_Shift.wav", 1.f);

            m_iMenu_SelectorIndex = 1;
        }

        if (0 == m_iMenu_SelectorIndex && CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
        {
            m_iOpenMenu_Squence = 0;
            m_iOpenSystem_Phase = 2;
            m_bRenderGiftWindow = true;
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_Open.ogg", 1.f);

        }
        if (1 == m_iMenu_SelectorIndex && CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_03.LockOff.wav", 1.f);

            Quit_Menu();
        }
    }
}

void CUI_Indicator_Communication::Open_Menu(_float fTimeDelta)
{
#pragma region Open_Menu
    if (1 == m_iOpenSystem_Phase)
    {
        if (true == m_bRenderMenuSwitch)
        {
            if (0 == m_iOpenMenu_Squence)
            {
                if (403.f <= m_vScale_Line[0].x)
                {
                    m_vScale_Line[0].x = 403.f;
                    m_pLine[0]->Set_Scale(m_vScale_Line[0]);

                    m_iOpenMenu_Squence = 1;
                }
                else
                {
                    m_vScale_Line[0].x += fTimeDelta * 1700.f;
                    m_pLine[0]->Set_Scale(m_vScale_Line[0]);
                }

                if (1.f <= m_fLineAlpha)
                {
                    m_fLineAlpha = 1.f;
                }
                else
                {
                    m_fLineAlpha += fTimeDelta * 2.f;
                }
            }
            else if (1 == m_iOpenMenu_Squence)
            {
                if (160.f <= m_vScale_Menu.y)
                {
                    m_vScale_Menu.y = 160.f;
                    m_pMenu->Set_Scale(m_vScale_Menu);

                    m_bCanKeyInput = true;
                }
                else
                {
                    m_vScale_Menu.y += fTimeDelta * 2000.f;
                    m_pMenu->Set_Scale(m_vScale_Menu);
                }

                if (1.f <= m_fMenuAlpha)
                {
                    m_fMenuAlpha = 1.f;
                }
                else
                {
                    m_fMenuAlpha += fTimeDelta * 2.f;
                }

                if (0.f >= m_fLineAlpha)
                {
                    m_fLineAlpha = 0.f;
                }
                else
                {
                    m_fLineAlpha -= fTimeDelta * 3.f;
                }

                if (0.5f <= m_fSelectorAlpha)
                {
                    m_fSelectorAlpha = 0.5f;
                }
                else
                {
                    m_fSelectorAlpha += fTimeDelta;
                }
            }
        }
    }
#pragma endregion
#pragma region Selector_Blink
    m_fBlinkTime += fTimeDelta * m_iBlinkIndex;

    if (0.75f <= m_fBlinkTime)
    {
        m_fBlinkTime = 0.75f;
        m_iBlinkIndex = -1;
    }
    if (0.f >= m_fBlinkTime)
    {
        m_fBlinkTime = 0.f;
        m_iBlinkIndex = 1;
    }
#pragma endregion

#pragma region Open Gift Window
    if (2 == m_iOpenSystem_Phase)
    {
        if (0 == m_iOpenMenu_Squence)
        {
            if (true == m_bRenderGiftWindow)
            {
                m_bRenderMenuSwitch = false;

                CGameInstance::Get_Instance()->Enable_DOF(true);
                CGameInstance::Get_Instance()->Set_DOF_DepthRange(5.f);
                CGameInstance::Get_Instance()->Set_DOF_DepthStart(0.f);

                if (0 == m_iGiftWindowOpen_Phase)
                {
                    if (1550.f <= m_vScale_WindowBackground.x)
                    {
                        m_vScale_WindowBackground.x = 1550.f;
                        m_pWindow_Backgrouund->Set_Scale(m_vScale_WindowBackground);

                        m_bScaleDoneWindow = true;
                    }
                    else
                    {
                        m_vScale_WindowBackground.x += fTimeDelta * 3000.f;
                        m_pWindow_Backgrouund->Set_Scale(m_vScale_WindowBackground);
                    }

                    if (1552.f <= m_vScale_WindowTitle.x)
                    {
                        m_vScale_WindowTitle.x = 1552.f;
                        m_pWindow_Title->Set_Scale(m_vScale_WindowTitle);

                        m_bScaleDoneTitle = true;
                    }
                    else
                    {
                        m_vScale_WindowTitle.x += fTimeDelta * 3000.f;
                        m_pWindow_Title->Set_Scale(m_vScale_WindowTitle);
                    }

                    if (true == m_bScaleDoneTitle && true == m_bScaleDoneWindow)
                    {
                        m_iGiftWindowOpen_Phase = 1;
                        m_bScaleDoneTitle = false;
                        m_bScaleDoneWindow = false;
                    }
                }

                if (1 == m_iGiftWindowOpen_Phase)
                {
                    if (1500.f >= m_vScale_WindowBackground.x)
                    {
                        m_vScale_WindowBackground.x = 1500.f;
                        m_pWindow_Backgrouund->Set_Scale(m_vScale_WindowBackground);

                        m_bScaleDoneWindow = true;
                    }
                    else
                    {
                        m_vScale_WindowBackground.x -= fTimeDelta * 1500.f;
                        m_pWindow_Backgrouund->Set_Scale(m_vScale_WindowBackground);
                    }

                    if (1503.f >= m_vScale_WindowTitle.x)
                    {
                        m_vScale_WindowTitle.x = 1503.f;
                        m_pWindow_Title->Set_Scale(m_vScale_WindowTitle);

                        m_bScaleDoneTitle = true;
                    }
                    else
                    {
                        m_vScale_WindowTitle.x -= fTimeDelta * 1500.f;
                        m_pWindow_Title->Set_Scale(m_vScale_WindowTitle);
                    }

                    if (1.f <= m_fWindowTitleAlpha)
                    {
                        m_fWindowTitleAlpha = 1.f;
                    }
                    else
                    {
                        m_fWindowTitleAlpha += fTimeDelta * 25.f;
                    }

                    if (true == m_bScaleDoneTitle && true == m_bScaleDoneWindow)
                    {
                        m_iGiftWindowOpen_Phase = 2;
                        m_bScaleDoneTitle = false;
                        m_bScaleDoneWindow = false;
                    }
                }

                if (2 == m_iGiftWindowOpen_Phase)
                {
                    m_bCanKeyInput = false;

                    m_iPossesion = CUI_Manager::Get_Instance()->Get_GiftPossession();
                    m_strGiftPossesion = std::to_wstring(m_iPossesion);

                    if (1.f <= m_fTextAlpha)
                    {
                        m_fTextAlpha = 1.f;
                    }
                    else
                    {
                        m_fTextAlpha += fTimeDelta * 25.f;
                    }

                    if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
                    {
                        CGameInstance::Get_Instance()->PlaySoundW(L"UI_Open.ogg", 1.f);

                        m_bOpenReAnswer = true;
                    }

                    if (CGameInstance::Get_Instance()->Key_Down(VK_BACK))
                    {
                        Quit_Menu();
                        CGameInstance::Get_Instance()->PlaySoundW(L"UI_03.LockOff.wav", 1.f);

                        CGameInstance::Get_Instance()->Enable_DOF(false);
                    }
                }

            }
        }
    }
#pragma endregion
}

void CUI_Indicator_Communication::Quit_Menu()
{
//============================================================= _bool
    m_bRenderIconSwitch             = true;
    m_bRenderDegree                 = false;
    m_bRenderGiftWindow             = false;
    m_bNearIcon                     = false;
    m_bRenderMenuSwitch             = false;
    m_bCanKeyInput                  = false;
    m_bScaleDoneTitle               = false;
    m_bScaleDoneWindow              = false;
    m_bOpenReAnswer                 = false;
    m_bCloseReAnswer                = false;
    m_bJoin                         = false;
//============================================================= _int
    m_iBlinkIndex                   = 1;
    m_iOpenSystem_Phase             = 0;
    m_iOpenMenu_Squence             = 0;
    m_iMenu_SelectorIndex           = 0;
    m_iGiftWindowOpen_Phase         = 0;
    m_iLove                         = 1;
    m_iPossesion                    = 0;
    m_iReAnswerOpen_Phase           = 0;
    m_iReAnswerBlinkIndex           = 1;
    m_iMenu_ReAnswerSelectorIndex   = 0;
    m_iJoin_Phase                   = 0;
//============================================================= _float
    m_fSelectorAlpha                = 0.f;
    m_fReAnswerSelectorAlpha        = 0.f;
    m_fBlinkTime                    = 0.f;
    m_fLineAlpha                    = 0.f;
    m_fMenuAlpha                    = 0.f;
    m_fWindowBackgroundAlpha        = 0.85f;
    m_fWindowTitleAlpha             = 0.f;
    m_fTextAlpha                    = 0.f;
    m_fReAnswerAlpha                = 0.f;
    m_fReAnswerBlinkTime            = 0.f;
    m_fJoinAlpha                    = 0.f;
    m_fAccTime                      = 0.f;

//============================================================= _float3
    m_vScale_Icon_Background        = _float3(64.f, 64.f, 1.f);
    m_vScale_Icon_On                = _float3(28.f, 28.f, 1.f);
    m_vScale_Icon_Off               = _float3(28.f, 28.f, 1.f);
    m_vScale_Menu                   = _float3(400.f, 3.f, 1.f); // y = 160
    m_vScale_Selector[0]            = _float3(204.f, 42.f, 1.f);
    m_vScale_Selector[1]            = _float3(41.f, 41.f, 1.f);
    m_vScale_Selector[2]            = _float3(16.f, 27.f, 1.f);
    m_vScale_Selector[3]            = _float3(15.f, 15.f, 1.f);
    m_vScale_Line[0]                = _float3(1.f, 3.f, 1.f);
    m_vScale_Line[1]                = _float3(350.f, 1.f, 1.f);
    m_vScale_Line[2]                = _float3(350.f, 1.f, 1.f);
    m_vScale_WindowBackground       = _float3(1400.f, 700.f, 1.f);
    m_vScale_WindowTitle            = _float3(1403.f, 26.f ,1.f);
    m_vScale_ProfileFrameUpDw       = _float3(642.f, 33.f, 1.f);
    m_vScale_ProfileFrame           = _float3(248.f, 100.f, 1.f);
    m_vScale_Profile                = _float3(138.f, 86.f, 1.f);
    m_vScale_GiftFrameUp            = _float3(670.f, 124.f, 1.f);
    m_vScale_GiftFrameDw            = _float3(670.f, 36.f, 1.f);
    m_vScale_GiftListFrame          = _float3(642.f, 65.f, 1.f);
    m_vScale_GiftIcon[0]            = _float3(32.f, 32.f, 1.f);
    m_vScale_GiftIcon[1]            = _float3(65.f, 65.f, 1.f);
    m_vScale_GiftFrameUp            = _float3(670.f, 124.f, 1.f);
    m_vScale_GiftFrameDw            = _float3(670.f, 36.f, 1.f);
    m_vScale_ActivateProfile        = _float3(66.f, 56.f, 1.f);
    m_vScale_ActivateCharacter      = _float3(62.f, 52.f, 1.f);
    m_vScale_MiniFillter            = _float3(64.f, 54.f, 1.f);
    m_vScale_Gift                   = _float3(500.f, 250.f, 1.f);
    m_vScale_Button                 = _float3(32.f, 32.f, 1.f);
    m_vScale_ReAnswer               = _float3(350.f, 200.f, 1.f);
    m_vScale_ReAnswerSelector[0]    = _float3(204.f, 42.f, 1.f);
    m_vScale_ReAnswerSelector[1]    = _float3(41.f, 41.f, 1.f);
    m_vScale_ReAnswerSelector[2]    = _float3(16.f, 27.f, 1.f);
    m_vScale_ReAnswerSelector[3]    = _float3(15.f, 15.f, 1.f);
    m_vScale_JoinParty              = _float3(350.f, 200.f, 1.f);

//============================================================= _float4
    m_vPosition_Icon                = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Menu                = _float4(460.f, -260.f, 0.f, 1.f);
    m_vPosition_Selector[0]         = _float4(390.f, -230.f, 0.f, 1.f);
    m_vPosition_Selector[1]         = _float4(310.f, -230.f, 0.f, 1.f);
    m_vPosition_Selector[2]         = _float4(316.f, -230.f, 0.f, 1.f);
    m_vPosition_Selector[3]         = _float4(308.f, -230.f, 0.f, 1.f);
    m_vPosition_WindowBackground    = _float4(0.f, -50.f, 0.f, 1.f);
    m_vPosition_WindowTitle         = _float4(0.f, 310.f, 0.f, 1.f);
    m_vPosition_ProfileFrameUpDw[0] = _float4(-380.f, 270.f, 0.f, 1.f);
    m_vPosition_ProfileFrame        = _float4(-560.f, 220.f, 0.f, 1.f);
    m_vPosition_ProfileFrameUpDw[1] = _float4(-380.f, 170.f, 0.f, 1.f);
    m_vPosition_Profile             = _float4(-608.f, 218.f, 0.f, 1.f);
    m_vPosition_Line[0]             = _float4(-268.f, 215.f, 0.f, 1.f);
    m_vPosition_Line[1]             = _float4(0.f, 30.f, 0.f, 1.f);
    m_vPosition_GiftListFrame       = _float4(-380.f, 100.f, 0.f, 1.f);
    m_vPosition_GiftIcon[0]         = _float4(-668.f, 100.f, 0.f, 1.f);
    m_vPosition_GiftIcon[1]         = _float4(120.f, 225.f, 0.f, 1.f);
    m_vPosition_GiftFrameUpDw[0]    = _float4(380.f, 226.f, 0.f, 1.f);
    m_vPosition_GiftFrameUpDw[1]    = _float4(380.f, -330.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[0]  = _float4(130.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[1]  = _float4(200.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[2]  = _float4(270.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[3]  = _float4(340.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateProfile[4]  = _float4(410.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[0] = _float4(130.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[1] = _float4(200.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[2] = _float4(270.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[3] = _float4(340.f, 130.f, 0.f, 1.f);
    m_vPosition_ActivateCharacter[4] = _float4(410.f, 130.f, 0.f, 1.f);
    m_vPosition_MiniFillter         = _float4(130.f, 130.f, 0.f, 1.f);
    m_vPosition_Gift                = _float4(380.f, -40.f, 0.f, 1.f);
    m_vPosition_Button[0]           = _float4(550.f, -370.f, 0.f, 1.f);
    m_vPosition_Button[1]           = _float4(650.f, -370.f, 0.f, 1.f);
    m_vPosition_ReAnswer            = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_ReAnswerSelector[0] = _float4(0.f, -10.f, 0.f, 1.f);
    m_vPosition_ReAnswerSelector[1] = _float4(-80.f, -10.f, 0.f, 1.f);
    m_vPosition_ReAnswerSelector[2] = _float4(-74.f, -10.f, 0.f, 1.f);
    m_vPosition_ReAnswerSelector[3] = _float4(-82.f, -10.f, 0.f, 1.f);
    m_vPosition_JoinParty           = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_EnterAgain          = _float4(70.f, -60.f, 0.f, 1.f);
//============================================================= Set_Scale
    m_pIcon_Background->Set_Scale(m_vScale_Icon_Background);
    m_pIcon_On->Set_Scale(m_vScale_Icon_On);
    m_pIcon_Off->Set_Scale(m_vScale_Icon_Off);
    m_pMenu->Set_Scale(m_vScale_Menu);
    for (size_t i = 0; i < 4; i++)
    {
        m_pSelector[i]->Set_Scale(m_vScale_Selector[i]);
    }
    for (size_t i = 0; i < 3; i++)
    {
        m_pLine[i]->Set_Scale(m_vScale_Line[i]);
    }
    m_pWindow_Backgrouund->Set_Scale(m_vScale_WindowBackground);
    m_pWindow_Title->Set_Scale(m_vScale_WindowTitle);
    for (size_t i = 0; i < 2; i++)
    {
        m_pProfileFrames[i]->Set_Scale(m_vScale_ProfileFrameUpDw);
    }
    m_pProfileFrame->Set_Scale(m_vScale_ProfileFrame);
    m_pProfile->Set_Scale(m_vScale_Profile);

    m_pGiftFrames[0]->Set_Scale(m_vScale_GiftFrameUp);
    m_pGiftFrames[1]->Set_Scale(m_vScale_GiftFrameDw);
    m_pGiftFrames[2]->Set_Scale(m_vScale_GiftListFrame);
    for (size_t i = 0; i < 2; i++)
    {
        m_pGiftIcon[i]->Set_Scale(m_vScale_GiftIcon[i]);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateProfile[i]->Set_Scale(m_vScale_ActivateProfile);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateCharacter[i]->Set_Scale(m_vScale_ActivateCharacter);
    }
    m_pMiniFillter->Set_Scale(m_vScale_MiniFillter);
    m_pGift->Set_Scale(m_vScale_Gift);
    for (size_t i = 0; i < 2; i++)
    {
        m_pButton[i]->Set_Scale(m_vScale_Button);
    }
    m_pReAnswer->Set_Scale(m_vScale_ReAnswer);
    for (size_t i = 0; i < 4; i++)
    {
        m_pReAnswerSelector[i]->Set_Scale(m_vScale_ReAnswerSelector[i]);
    }
    m_pJoinBackground->Set_Scale(m_vScale_JoinParty);
    m_pEnterAgain->Set_Scale(m_vScale_Button);
//============================================================= Set_Position
    m_pIcon_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);
    m_pIcon_On->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);
    m_pIcon_Off->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);
    m_pMenu->Set_State(TRANSFORM::POSITION, m_vPosition_Menu);
    for (size_t i = 0; i < 4; i++)
    {
        m_pSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Selector[i]);
    }
    m_pLine[0]->Set_State(TRANSFORM::POSITION, m_vPosition_Menu);
    for (size_t i = 1; i < 3; i++)
    {
        m_pLine[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Line[i - 1]);
    }
    m_pWindow_Backgrouund->Set_State(TRANSFORM::POSITION, m_vPosition_WindowBackground);
    m_pWindow_Title->Set_State(TRANSFORM::POSITION, m_vPosition_WindowTitle);
    for (size_t i = 0; i < 2; i++)
    {
        m_pProfileFrames[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameUpDw[i]);
    }
    m_pProfileFrame->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrame);
    m_pProfile->Set_State(TRANSFORM::POSITION, m_vPosition_Profile);
    m_pGiftFrames[0]->Set_State(TRANSFORM::POSITION, m_vPosition_GiftFrameUpDw[0]);
    m_pGiftFrames[1]->Set_State(TRANSFORM::POSITION, m_vPosition_GiftFrameUpDw[1]);
    m_pGiftFrames[2]->Set_State(TRANSFORM::POSITION, m_vPosition_GiftListFrame);
    for (size_t i = 0; i < 2; i++)
    {
        m_pGiftIcon[i]->Set_State(TRANSFORM::POSITION, m_vPosition_GiftIcon[i]);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateProfile[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ActivateProfile[i]);
    }
    for (size_t i = 0; i < 5; i++)
    {
        m_pActivateCharacter[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ActivateCharacter[i]);
    }
    m_pMiniFillter->Set_State(TRANSFORM::POSITION, m_vPosition_MiniFillter);
    m_pGift->Set_State(TRANSFORM::POSITION, m_vPosition_Gift);
    for (size_t i = 0; i < 2; i++)
    {
        m_pButton[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Button[i]);
    }
    m_pReAnswer->Set_State(TRANSFORM::POSITION, m_vPosition_ReAnswer);
    for (size_t i = 0; i < 4; i++)
    {
        m_pReAnswerSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ReAnswerSelector[i]);
    }
    m_pJoinBackground->Set_State(TRANSFORM::POSITION, m_vPosition_JoinParty);
    m_pEnterAgain->Set_State(TRANSFORM::POSITION, m_vPosition_EnterAgain);

//============================================================= wstring
    m_strMenuTag[0]                 = TEXT("선물을 준다");
    m_strMenuTag[1]                 = TEXT("그만둔다");

    m_strGiftWindowTitle            = TEXT("선물");

    m_strArasiSpring                = TEXT("아라시 스프링");
    m_strLove                       = std::to_wstring(m_iLove);

    m_strGift                       = TEXT("즐거운 과자 모음");
    m_strGiftPossesion              = TEXT("");
    m_strHowManyPossese             = TEXT("소지 수");
    m_strMaxPossese                 = TEXT("/ 99");
    m_strExplain[0]                 = TEXT("달콤한 쿠키에 짭잘한 감자칩이다.");
    m_strExplain[1]                 = TEXT("누군가 받으면 굉장히 좋아할 것 같다.");
    m_strButton[0]                  = TEXT("결정");
    m_strButton[1]                  = TEXT("뒤로");

    m_strReAnswer[0]                = TEXT("선물을 건네시겠습니까?");
    m_strReAnswer[1]                = TEXT("예");
    m_strReAnswer[2]                = TEXT("아니오");

    m_strJoinParty                  = TEXT("아라시가 파티에 합류했다.");
    m_strJoinParty_Check            = TEXT("확인");
}

void CUI_Indicator_Communication::Set_IndicatorPosition(_matrixf _WorldMatrix)
{
    _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);

    _vector vPosition = XMVectorSet(0.f, 2.f, 0.f, 1.f);

    /* World */
    vPosition = XMVector3TransformCoord(vPosition, _WorldMatrix);

    _vector vLookCam = MatCam.r[3] - vPosition;

    _vector vCrossLookCam = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));

    vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);


    //vPosition_L = XMVectorSetX(vPosition_L, XMVectorGetX(vPosition_L) - 22.f);
    //vPosition_R = XMVectorSetX(vPosition_R, XMVectorGetX(vPosition_R) + 22.f);

    _float fScreenX = XMVectorGetX(vPosition);
    _float fScreenY = XMVectorGetY(vPosition);

    m_vPosition_Icon.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Icon.y = (fScreenY) * 0.5f * g_iWinCY;
    m_pIcon_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);
    m_pIcon_On->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);
    m_pIcon_Off->Set_State(TRANSFORM::POSITION, m_vPosition_Icon);

//=========================================================================================================================== Culling
    // Look_Vector (Arasi ---> Camera)
    _vector vLookCamera_FromArasi = MatCam.r[3] - _WorldMatrix.r[3];
    _vector vLookMonster_FromCamera = _WorldMatrix.r[3] - MatCam.r[3];

    _float fDotProduct = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLookCamera_FromArasi), XMVector3Normalize(MatCam.r[2])))));

    if (90.f <= fDotProduct && 270.f >= fDotProduct)
    {
        m_bRenderDegree = true;
    }
    else
    {
        m_bRenderDegree = false;
    }
}

void CUI_Indicator_Communication::Open_ReAnswer(_float fTimeDelta)
{
    if (false == m_bCloseReAnswer)
    {
#pragma region ShaderAlpha
        if (1.f <= m_fReAnswerAlpha)
        {
            m_fReAnswerAlpha = 1.f;
        }
        else
        {
            m_fReAnswerAlpha += fTimeDelta * 25.f;
        }

        if (0.5f <= m_fReAnswerSelectorAlpha)
        {
            m_fReAnswerSelectorAlpha = 0.5f;
        }
        else
        {
            m_fReAnswerSelectorAlpha += fTimeDelta;
        }
#pragma endregion
#pragma region ReAnswerSelector_Blink
        m_fReAnswerBlinkTime += fTimeDelta * m_iReAnswerBlinkIndex;

        if (0.75f <= m_fReAnswerBlinkTime)
        {
            m_fReAnswerBlinkTime = 0.75f;
            m_iReAnswerBlinkIndex = -1;
        }
        if (0.f >= m_fReAnswerBlinkTime)
        {
            m_fReAnswerBlinkTime = 0.f;
            m_iReAnswerBlinkIndex = 1;
        }
#pragma endregion
#pragma region Scaling
        if (0 == m_iReAnswerOpen_Phase)
        {
            if (430.f <= m_vScale_ReAnswer.x)
            {
                m_vScale_ReAnswer.x = 430.f;
                m_pReAnswer->Set_Scale(m_vScale_ReAnswer);

                m_iReAnswerOpen_Phase = 1;
            }
            else
            {
                m_vScale_ReAnswer.x += fTimeDelta * 3000.f;
                m_pReAnswer->Set_Scale(m_vScale_ReAnswer);
            }
        }

        if (1 == m_iReAnswerOpen_Phase)
        {
            if (400.f >= m_vScale_ReAnswer.x)
            {
                m_vScale_ReAnswer.x = 400.f;
                m_pReAnswer->Set_Scale(m_vScale_ReAnswer);

                m_iReAnswerOpen_Phase = 2;
            }
            else
            {
                m_vScale_ReAnswer.x -= fTimeDelta * 1500.f;
                m_pReAnswer->Set_Scale(m_vScale_ReAnswer);
            }
        }

        if (2 == m_iReAnswerOpen_Phase)
        {
            if (CGameInstance::Get_Instance()->Key_Down(VK_UP))
            {
                m_vPosition_ReAnswerSelector[0] = _float4(0.f, -10.f, 0.f, 1.f);
                m_vPosition_ReAnswerSelector[1] = _float4(-80.f, -10.f, 0.f, 1.f);
                m_vPosition_ReAnswerSelector[2] = _float4(-74.f, -10.f, 0.f, 1.f);
                m_vPosition_ReAnswerSelector[3] = _float4(-82.f, -10.f, 0.f, 1.f);

                m_iMenu_ReAnswerSelectorIndex = 0;

                for (size_t i = 0; i < 4; i++)
                {
                    m_pReAnswerSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ReAnswerSelector[i]);
                }
            }
            if (CGameInstance::Get_Instance()->Key_Down(VK_DOWN))
            {
                m_vPosition_ReAnswerSelector[0] = _float4(0.f, -60.f, 0.f, 1.f);
                m_vPosition_ReAnswerSelector[1] = _float4(-80.f, -60.f, 0.f, 1.f);
                m_vPosition_ReAnswerSelector[2] = _float4(-74.f, -60.f, 0.f, 1.f);
                m_vPosition_ReAnswerSelector[3] = _float4(-82.f, -60.f, 0.f, 1.f);

                for (size_t i = 0; i < 4; i++)
                {
                    m_pReAnswerSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ReAnswerSelector[i]);
                }

                m_iMenu_ReAnswerSelectorIndex = 1;
            }

            if (0 == m_iMenu_ReAnswerSelectorIndex)
            {
                if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
                {
                    if (0 <= m_iPossesion)
                    {
                        CUI_Manager::Get_Instance()->Set_UI_Gift_Possession_Minus(1);
                        CGameInstance::Get_Instance()->PlaySoundW(L"UI_Open.ogg", 1.f);

                        CUI_Manager::Get_Instance()->Set_LovePlus();
                        m_iLove = CUI_Manager::Get_Instance()->Get_Love();
                        m_strLove = std::to_wstring(m_iLove);

                        CUI_Manager::Get_Instance()->Set_ArasiJoinParty();
                        Quit_Menu();
                    }
                }
            }
            else if (1 == m_iMenu_ReAnswerSelectorIndex)
            {
                if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_03.LockOff.wav", 1.f);
                    m_bCloseReAnswer = true;
                    m_bOpenReAnswer = false;
                    m_iReAnswerOpen_Phase = 0;
                }
            }
        }
#pragma endregion
    }
}

void CUI_Indicator_Communication::Close_ReAnswer(_float fTimeDelta)
{

#pragma region ShaderAlpha
    if (0.f >= m_fReAnswerAlpha)
    {
        m_fReAnswerAlpha = 0.f;
    }
    else
    {
        m_fReAnswerAlpha -= fTimeDelta * 25.f;
    }

    if (0.f >= m_fReAnswerSelectorAlpha)
    {
        m_fReAnswerSelectorAlpha = 0.f;
    }
    else
    {
        m_fReAnswerSelectorAlpha -= fTimeDelta;
    }
#pragma endregion
#pragma region Scaling
    if (0 == m_iReAnswerOpen_Phase)
    {
        if (430.f <= m_vScale_ReAnswer.x)
        {
            m_vScale_ReAnswer.x = 430.f;
            m_pReAnswer->Set_Scale(m_vScale_ReAnswer);

            m_iReAnswerOpen_Phase = 1;
        }
        else
        {
            m_vScale_ReAnswer.x += fTimeDelta * 3000.f;
            m_pReAnswer->Set_Scale(m_vScale_ReAnswer);
        }
    }

    if (1 == m_iReAnswerOpen_Phase)
    {
        if (350.f >= m_vScale_ReAnswer.x)
        {
            m_vScale_ReAnswer.x = 350.f;
            m_pReAnswer->Set_Scale(m_vScale_ReAnswer);

            m_iReAnswerOpen_Phase = 2;
        }
        else
        {
            m_vScale_ReAnswer.x -= fTimeDelta * 3000.f;
            m_pReAnswer->Set_Scale(m_vScale_ReAnswer);
        }
    }

    if (2 == m_iReAnswerOpen_Phase)
    {
        m_iReAnswerOpen_Phase = 0;
        m_iReAnswerBlinkIndex = 1;
        m_iMenu_ReAnswerSelectorIndex = 0;

        m_fReAnswerSelectorAlpha = 0.f;
        m_fReAnswerAlpha = 0.f;
        m_fReAnswerBlinkTime = 0.f;

        m_vScale_ReAnswer = _float3(350.f, 200.f, 1.f);
        m_vScale_ReAnswerSelector[0] = _float3(204.f, 42.f, 1.f);
        m_vScale_ReAnswerSelector[1] = _float3(41.f, 41.f, 1.f);
        m_vScale_ReAnswerSelector[2] = _float3(16.f, 27.f, 1.f);
        m_vScale_ReAnswerSelector[3] = _float3(15.f, 15.f, 1.f);

        m_pReAnswer->Set_Scale(m_vScale_ReAnswer);
        for (size_t i = 0; i < 4; i++)
        {
            m_pReAnswerSelector[i]->Set_Scale(m_vScale_ReAnswerSelector[i]);
        }

        m_vPosition_ReAnswer = _float4(0.f, 0.f, 0.f, 1.f);
        m_vPosition_ReAnswerSelector[0] = _float4(0.f, -10.f, 0.f, 1.f);
        m_vPosition_ReAnswerSelector[1] = _float4(-80.f, -10.f, 0.f, 1.f);
        m_vPosition_ReAnswerSelector[2] = _float4(-74.f, -10.f, 0.f, 1.f);
        m_vPosition_ReAnswerSelector[3] = _float4(-82.f, -10.f, 0.f, 1.f);

        m_pReAnswer->Set_State(TRANSFORM::POSITION, m_vPosition_ReAnswer);
        for (size_t i = 0; i < 4; i++)
        {
            m_pReAnswerSelector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_ReAnswerSelector[i]);
        }

        m_bCanKeyInput = true;
        m_bOpenReAnswer = false;
        m_bCloseReAnswer = false;
    }

}

void CUI_Indicator_Communication::Arash_JoinParty(_float fTimeDelta)
{
    if (0 == m_iJoin_Phase)
    {
#pragma region Alpha
        if (1.f <= m_fJoinAlpha)
        {
            m_fJoinAlpha = 1.f;
        }
        else
        {
            m_fJoinAlpha += fTimeDelta * 25.f;
        }
#pragma endregion
#pragma region Scaling
        if (450.f <= m_vScale_JoinParty.x)
        {
            m_vScale_JoinParty.x = 450.f;
            m_pJoinBackground->Set_Scale(m_vScale_JoinParty);

            m_iJoin_Phase = 1;
        }
        else
        {
            m_vScale_JoinParty.x += fTimeDelta * 3000.f;
            m_pJoinBackground->Set_Scale(m_vScale_JoinParty);
        }
#pragma endregion
    }
    if (1 == m_iJoin_Phase)
    {
#pragma region Scaling
        if (400.f >= m_vScale_JoinParty.x)
        {
            m_vScale_JoinParty.x = 400.f;
            m_pJoinBackground->Set_Scale(m_vScale_JoinParty);

            m_iJoin_Phase = 2;
        }
        else
        {
            m_vScale_JoinParty.x -= fTimeDelta * 1400.f;
            m_pJoinBackground->Set_Scale(m_vScale_JoinParty);
        }
#pragma endregion
    }
    if (2 == m_iJoin_Phase)
    {
        m_bCanKeyInput = false;
#pragma region Scaling
        if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
        {
            CUI_Manager::Get_Instance()->Set_ArasiJoinParty();
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_03.LockOff.wav", 1.f);

            CGameInstance::Get_Instance()->Enable_DOF(false);

            m_bCanKeyInput = true;
            Quit_Menu();
        }
#pragma endregion
    }
}

HRESULT CUI_Indicator_Communication::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Ready_Components", "Failed to CGameObject::Ready_Components");
    }

    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);

    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Indicator_Communication> CUI_Indicator_Communication::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Communication> pInstance = make_private_shared(CUI_Indicator_Communication, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Communication::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Communication::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_Communication> pInstance = make_private_shared_copy(CUI_Indicator_Communication, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Communication::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
