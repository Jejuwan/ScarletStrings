#include "ClientPCH.h"
#include "UI_Menu_Category_BrainMap.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Menu_Category_BrainMap::CUI_Menu_Category_BrainMap(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Category_BrainMap::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Category_BrainMap::Initialize(any _pPlayerEntityDesc)
{   
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Initialize", "Failed to CGameObject::Initialize");
    }
    
    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    Initialize_UI();

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Category_BrainMap::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Menu_Category_BrainMap::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_3);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Menu_Category_BrainMap::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
#pragma region Title - <Brain Map>
        m_strFont = TEXT("브레인 맵");
#pragma endregion
#pragma region BrainMap Frame
        if (1 != CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            if (FAILED(m_pTexture_Off->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFont, _float2(380.f, 94.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);
        }
        else
        {
            if (FAILED(m_pTexture_On->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFont, _float2(380.f, 89.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);

#pragma endregion

#pragma region BrainMap Components
#pragma region Connect Line
            // TagLine 1
            if (FAILED(m_pTexture_SkillTagLines->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTagLines[0]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // TagLine 2
            if (FAILED(m_pTexture_SkillTagLines->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTagLines[1]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // TagLine 3
            if (FAILED(m_pTexture_SkillTagLines->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTagLines[2]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
            // TagLine 4
            if (FAILED(m_pTexture_SkillTagLines->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTagLines[3]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // TagLine 5
            if (FAILED(m_pTexture_SkillTagLines->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTagLines[4]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
#pragma endregion
#pragma region Tags
            // Tag 1
            if (FAILED(m_pTexture_SkillTags->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTags[0]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strSkillTags[0], _float2(945.f + m_fMaster_Position_X, 493.f + m_fMaster_Position_Y), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.35f);


            // Tag 2
            if (FAILED(m_pTexture_SkillTags->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTags[1]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strSkillTags[1], _float2(878.f + m_fMaster_Position_X, 538.f + m_fMaster_Position_Y), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.35f);

            // Tag 3
            if (FAILED(m_pTexture_SkillTags->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTags[2]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strSkillTags[2], _float2(1015.f + m_fMaster_Position_X, 538.f + m_fMaster_Position_Y), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.35f);

            // Tag 4
            if (FAILED(m_pTexture_SkillTags->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTags[3]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strSkillTags[3], _float2(865.f + m_fMaster_Position_X, 583.f + m_fMaster_Position_Y), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.35f);

            // Tag 5
            if (FAILED(m_pTexture_SkillTags->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTags[4]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strSkillTags[4], _float2(993.f + m_fMaster_Position_X, 583.f + m_fMaster_Position_Y), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.35f);
#pragma endregion
#pragma region Icons
#pragma region 확장
#pragma region  확장-0
            if (false == m_bSkillOnOff[0])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region 확장-1
            if (false == m_bSkillOnOff[1])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[0]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[0]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[0]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-2
            if (false == m_bSkillOnOff[2])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[2]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[1]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[2]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[2]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[1]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[1]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-3
            if (false == m_bSkillOnOff[3])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[3]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[2]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[3]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[3]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[2]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[2]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-4
            if (false == m_bSkillOnOff[4])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[4]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[3]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[4]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[4]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[3]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[3]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-5
            if (false == m_bSkillOnOff[5])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[5]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[4]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[5]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[5]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[4]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[4]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-6
            if (false == m_bSkillOnOff[6])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[6]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[5]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[6]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[6]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[5]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[5]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-7
            if (false == m_bSkillOnOff[7])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[7]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[6]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[7]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[7]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[6]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[6]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-8
            if (false == m_bSkillOnOff[8])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[8]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[7]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[8]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[8]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[7]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[7]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-9
            if (false == m_bSkillOnOff[9])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[9]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[8]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[9]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[9]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[8]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[8]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-10
            if (false == m_bSkillOnOff[10])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[10]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[9]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[10]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[10]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[9]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[9]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-11
            if (false == m_bSkillOnOff[11])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[11]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[10]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[11]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[11]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[10]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[10]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-12
            if (false == m_bSkillOnOff[12])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[12]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[11]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[12]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[12]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[11]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[11]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-13
            if (false == m_bSkillOnOff[13])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[13]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[12]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[13]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[13]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[12]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[12]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-14
            if (false == m_bSkillOnOff[14])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[14]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[13]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[14]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[14]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[13]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[13]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-15
            if (false == m_bSkillOnOff[15])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[15]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[14]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[15]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[15]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[14]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[14]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-16
            if (false == m_bSkillOnOff[16])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[16]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[15]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[16]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 9)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[16]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[15]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[15]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 확장-17
            if (false == m_bSkillOnOff[17])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[17]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[16]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[17]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[17]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[16]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[16]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma endregion
#pragma region 보조
#pragma region 보조-18
            if (false == m_bSkillOnOff[18])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[18]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[18]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[18]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region 보조-19
            if (false == m_bSkillOnOff[19])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[17]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[19]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[19]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[19]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[17]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[17]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-20
            if (false == m_bSkillOnOff[20])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[18]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[20]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[20]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[20]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[18]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[18]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-21
            if (false == m_bSkillOnOff[21])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[19]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[21]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[21]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[21]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[19]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[19]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }

#pragma endregion
#pragma region 보조-22
            if (false == m_bSkillOnOff[22])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[20]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[22]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[22]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[22]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[20]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[20]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-23
            if (false == m_bSkillOnOff[23])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[21]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[23]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[23]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[23]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[21]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[21]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-24
            if (false == m_bSkillOnOff[24])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[22]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[24]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[24]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[24]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[22]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[22]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-25
            if (false == m_bSkillOnOff[25])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[23]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[25]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[25]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[25]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[23]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[23]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-26
            if (false == m_bSkillOnOff[26])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[24]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[26]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[26]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[26]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[24]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[24]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-27
            if (false == m_bSkillOnOff[27])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[25]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[27]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[27]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[27]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[25]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[25]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-28
            if (false == m_bSkillOnOff[28])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[26]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[28]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[28]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[28]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[26]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[26]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-29
            if (false == m_bSkillOnOff[29])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[27]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[29]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[29]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[29]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[27]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[27]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 보조-30
            if (false == m_bSkillOnOff[30])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[28]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[30]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[30]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 5)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[30]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[28]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[28]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma endregion
#pragma region 브레인 필드
#pragma region 브레인 필드-31
            if (false == m_bSkillOnOff[31])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[31]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[31]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[31]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region 브레인 필드-32
            if (false == m_bSkillOnOff[32])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[29]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[32]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[32]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[32]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[29]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[29]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 브레인 필드-33
            if (false == m_bSkillOnOff[33])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[30]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[33]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[33]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[33]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[30]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[30]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 브레인 필드-34
            if (false == m_bSkillOnOff[34])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[31]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[34]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[34]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[34]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[31]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[31]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 브레인 필드-35
            if (false == m_bSkillOnOff[35])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[32]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[35]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[35]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[35]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[32]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[32]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 브레인 필드-36
            if (false == m_bSkillOnOff[36])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[33]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[36]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[36]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[36]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[33]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[33]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 브레인 필드-37
            if (false == m_bSkillOnOff[37])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[34]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[37]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[37]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[37]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[34]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[34]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 브레인 필드-38
            if (false == m_bSkillOnOff[38])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[35]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[38]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[38]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[38]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[35]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[35]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 브레인 필드-39
            if (false == m_bSkillOnOff[39])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[36]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[39]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[39]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[39]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[36]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[36]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 브레인 필드-40
            if (false == m_bSkillOnOff[40])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[37]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[40]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[40]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[40]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[37]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[37]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma endregion
#pragma region 드라이브
#pragma region 드라이브-41
            if (false == m_bSkillOnOff[41])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[41]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[41]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[41]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region 드라이브-42
            if (false == m_bSkillOnOff[42])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[38]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[42]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[42]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[42]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[38]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[38]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 드라이브-43
            if (false == m_bSkillOnOff[43])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[39]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[43]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[43]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[43]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[39]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[39]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 드라이브-44
            if (false == m_bSkillOnOff[44])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[40]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[44]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[44]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[44]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[40]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[40]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 드라이브-45
            if (false == m_bSkillOnOff[45])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[41]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[45]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[45]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[45]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[41]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[41]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 드라이브-46
            if (false == m_bSkillOnOff[46])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[42]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[46]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[46]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[46]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[42]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[42]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 드라이브-47
            if (false == m_bSkillOnOff[47])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[43]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[47]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[47]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[47]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[43]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[43]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 드라이브-48
            if (false == m_bSkillOnOff[48])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[44]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[48]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[48]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[48]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[44]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[44]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 드라이브-49
            if (false == m_bSkillOnOff[49])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[45]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[49]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[49]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[49]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[45]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[45]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma endregion
#pragma region 강화
#pragma region 강화-50
            if (false == m_bSkillOnOff[50])
            {
                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[50]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[50]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[50]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region 강화-51
            if (false == m_bSkillOnOff[51])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[46]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[51]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[51]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[51]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[46]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[46]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-52
            if (false == m_bSkillOnOff[52])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[47]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[52]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[52]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[52]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[47]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[47]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-53
            if (false == m_bSkillOnOff[53])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[48]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[53]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[53]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[53]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[48]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[48]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-54
            if (false == m_bSkillOnOff[54])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[49]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[54]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[54]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 7)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[54]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[49]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[49]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-55
            if (false == m_bSkillOnOff[55])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[50]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[55]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[55]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[55]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[50]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[50]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-56
            if (false == m_bSkillOnOff[56])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[51]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 10)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[56]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[56]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 10)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[56]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[51]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[51]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-57
            if (false == m_bSkillOnOff[57])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[52]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[57]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[57]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 6)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[57]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[52]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[52]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-58
            if (false == m_bSkillOnOff[58])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[53]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[58]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[58]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[58]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[53]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[53]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-59
            if (false == m_bSkillOnOff[59])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[54]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[59]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[59]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[59]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[54]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[54]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma region 강화-60
            if (false == m_bSkillOnOff[60])
            {
                // Connector
                if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBM_Connector[55]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // Icon
                if (FAILED(m_pTexture_SkillIconsOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[60]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
            else
            {
                // Color
                if (FAILED(m_pTexture_SkillIconColor->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillColor[60]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Icon
                if (FAILED(m_pTexture_SkillIconsOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcons[60]->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // Connector
                if (0 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[55]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
                else if (1 == m_iConnectorBlinkIndex)
                {
                    if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                    if (FAILED(m_pBM_Connector[55]->Bind_OnShader(m_pShader)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                    }
                    if (FAILED(__super::Render(1)))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                    }
                }
            }
#pragma endregion
#pragma endregion
#pragma endregion
#pragma region Explanations

#pragma endregion
#pragma region Brain
            // Brain_Background
            if (FAILED(m_pTexture_Brain->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrain[5]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(76)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // Brain 0
            if (FAILED(m_pTexture_Brain->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrain[0]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fBrainShader, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(75)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // Brain 1
            if (FAILED(m_pTexture_Brain->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrain[1]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fBrainShader1, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(75)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // Brain 2
            if (FAILED(m_pTexture_Brain->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 2)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrain[2]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fBrainShader2, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(75)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // Brain 3
            if (FAILED(m_pTexture_Brain->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 3)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrain[3]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fBrainShader3, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(75)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // Brain 4
            if (FAILED(m_pTexture_Brain->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 4)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrain[4]->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fBrainShader, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(75)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
#pragma endregion
#pragma region MouseIndicator
            if (FAILED(m_pTexture_BM_Connector->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pMouseIndicator->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fMouseIndicatorShader, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(31)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }
#pragma endregion
#pragma region SkillOpen_Gauge
            if (true == m_bSkillOpen_Gauge)
            {
                if (FAILED(m_pTexture_SkillOpen_Gague->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillOpen_Gauge->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fSkillOpen_Gauge, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(77)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region SkillWindow
            // ExplainWindow - Frame_Top
            if (FAILED(m_pTexture_ExplainFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainFrameT->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - Frame_Bottom
            if (FAILED(m_pTexture_ExplainFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainFrameB->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - Frame_Line1
            if (FAILED(m_pTexture_ExplainFrameLine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainFrameLine1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - Frame_Line2
            if (FAILED(m_pTexture_ExplainFrameLine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainFrameLine2->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - Frame_Line3
            if (FAILED(m_pTexture_ExplainFrameLine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainFrameLine3->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - VideoFrame
            if (FAILED(m_pTexture_ExplainVideoFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainVideoFrame->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - Video
            if (FAILED(m_pTexture_ExplainVideo->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainVideo->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - Profile
            if (FAILED(m_pTexture_ExplainProfile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainProfile->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - ProfileFrame
            if (FAILED(m_pTexture_ExplainProfileFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainProfileFrame->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - ProfileFrameLine1
            if (FAILED(m_pTexture_ExplainProfileFrameLine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainProfileFrameLine1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - ProfileFrameLine2
            if (FAILED(m_pTexture_ExplainProfileFrameLine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainProfileFrameLine2->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // ExplainWindow - ProfileFrameLine3
            if (FAILED(m_pTexture_ExplainProfileFrameLine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pExplainProfileFrameLine3->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
            }

            // GetSkillWindeo

            // Explanation
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplainTitle,  _float2(1320.f, 215.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.7f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHP,            _float2(1480.f, 525.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAblilty,       _float2(1480.f, 551.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAttack,        _float2(1480.f, 575.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strDefence,       _float2(1480.f, 602.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplain1,      _float2(1300.f, 660.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplain2,      _float2(1300.f, 695.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplain3,      _float2(1300.f, 730.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplain4,      _float2(1300.f, 765.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);            
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGetorNot,      _float2(1300.f, 855.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.55f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPlayerHp,      _float2(1740.f, 525.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPlayerAblity,  _float2(1740.f, 551.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPlayerAttack,  _float2(1740.f, 575.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPlayerDefence, _float2(1740.f, 602.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);


#pragma endregion
#pragma region BlackFillter
            if (true == m_bBlackFillter)
            {
                // BlackFillter - Background
                if (FAILED(m_pTexture_BlackFillter->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBlackFillter->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(78)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // BlackFillter - Window
                if (FAILED(m_pTexture_BlackFillterWindow->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBlackFillterWindow->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(78)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                // GetSkill - Tag
                if (FAILED(m_pTexture_GetSkillTag->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGetSkillTag->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(73)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }
                
                // GetSkill - Frame_Top
                if (FAILED(m_pTexture_GetSkillFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 0)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGetSkillFrameT->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // GetSkill - Frame_Bottom
                if (FAILED(m_pTexture_GetSkillFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", 1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGetSkillFrameB->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // GetSkill - Line
                if (FAILED(m_pTexture_GetSkillLine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGetSkillLine->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                // GetSkill - Return
                if (FAILED(m_pTexture_GetSkillReturn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pGetSkillReturn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGetSkillTag,   _float2(674.f, 452.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.4f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGetSkillName,  _float2(738.f, 534.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.65f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGetSkillEnd,   _float2(888.f, 614.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.6f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strGetSkillClose, _float2(1178.f, 693.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);

            }
#pragma endregion
#pragma endregion
        }
    }
    
    return S_OK;
}

HRESULT CUI_Menu_Category_BrainMap::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category_BrainMap::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Menu_Category_BrainMap::Initialize_UI()
{
#pragma region Transform
    m_pBackground = CTransform::Create(m_pDevice, m_pContext);

    for (size_t i = 0; i < 5; i++)
    {
        m_pSkillTags[i] = CTransform::Create(m_pDevice, m_pContext);
        m_pSkillTagLines[i] = CTransform::Create(m_pDevice, m_pContext);
    }
    for (size_t i = 0; i < 61; i++)
    {
        m_pSkillIcons[i] = CTransform::Create(m_pDevice, m_pContext);
        m_pSkillColor[i] = CTransform::Create(m_pDevice, m_pContext);
    }
    for (size_t i = 0; i < 59; i++)
    {
        m_pBM_Connector[i] = CTransform::Create(m_pDevice, m_pContext);
    }
    for (size_t i = 0; i < 6; i++)
    {
        m_pBrain[i] = CTransform::Create(m_pDevice, m_pContext);
    }
    m_pMouseIndicator                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillOpen_Gauge                  = CTransform::Create(m_pDevice, m_pContext);
    m_pBlackFillter                     = CTransform::Create(m_pDevice, m_pContext);
    m_pBlackFillterWindow               = CTransform::Create(m_pDevice, m_pContext);
    m_pGetSkillTag                      = CTransform::Create(m_pDevice, m_pContext);
    m_pGetSkillFrameT                   = CTransform::Create(m_pDevice, m_pContext);
    m_pGetSkillFrameB                   = CTransform::Create(m_pDevice, m_pContext);
    m_pGetSkillLine                     = CTransform::Create(m_pDevice, m_pContext);
    m_pGetSkillReturn                   = CTransform::Create(m_pDevice, m_pContext);

    m_pExplainFrameT                    = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainFrameB                    = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainFrameLine1                = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainFrameLine2                = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainFrameLine3                = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainVideo                     = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainVideoFrame                = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainProfile                   = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainProfileFrame              = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainProfileFrameLine1         = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainProfileFrameLine2         = CTransform::Create(m_pDevice, m_pContext);
    m_pExplainProfileFrameLine3         = CTransform::Create(m_pDevice, m_pContext);


#pragma endregion
#pragma region Texture
    m_pTexture_On                       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOn.png"));
    m_pTexture_Off                      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds"));
    m_pTexture_SkillTags                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/SkillTag.png"));
    m_pTexture_SkillTagLines            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/BP_Line.png"));
    m_pTexture_SkillIconsOn             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Icon/BM%d_On.png"), 11);
    m_pTexture_SkillIconsOff            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Icon/BM%d_Off.png"), 11);
    m_pTexture_SkillIconColor           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Icon/BM_Color%d.png"), 4);
    m_pTexture_BM_Connector             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Adds/BM_Connector%d.png"), 8);
    m_pTexture_Brain                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Adds/Brain%d.png"), 5);
    m_pTexture_SkillOpen_Gague          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Adds/SkillOpenGauge.png"));
    m_pTexture_BlackFillter             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png"));
    m_pTexture_BlackFillterWindow       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png"));
    m_pTexture_GetSkillTag              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/GetSkillTag.png"));
    m_pTexture_GetSkillFrame            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/GetSkillFrame%d.png"), 2);
    m_pTexture_GetSkillLine             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/BP_Line.png"));
    m_pTexture_GetSkillReturn           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/ENTER.png"));

    m_pTexture_ExplainFrame             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/GetSkillFrame%d.png"), 2);
    m_pTexture_ExplainFrameLine         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/BP_Line.png"));
    m_pTexture_ExplainVideo             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/T_ui_SasLink_00_00_common.dds"));
    m_pTexture_ExplainVideoFrame        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Tips_background.png"));
    m_pTexture_ExplainProfile           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Kasane.png"));
    m_pTexture_ExplainProfileFrame      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Skill_Frame/ProfileFrame.png"));
    m_pTexture_ExplainProfileFrameLine  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin//Resources/Texture/UI/Menu/Skill_Frame/ProfileFrameLine.png"));
#pragma endregion
#pragma region _bool
    for (size_t i = 0; i < 61; i++)
    {
        m_bSkillOnOff[i] = false;
    }
    m_bBlackFillter                 = false;
    m_bSkillOpen_Gauge              = false;


#pragma endregion
#pragma region _int
    m_iBrainPhase                   = 0;
    m_iConnectorBlinkIndex          = 0;
    m_iMouseIndicatorShaderBlink    = 1;

    for (size_t i = 0; i < 61; i++)
    {
        m_iCursorSoundOnce[i] = 0;
        m_iClickSoundOnce[i] = 0;
        m_iActivateSoundOnce[i] = 0;
    }
#pragma endregion
#pragma region _float
    m_fMaster_Position_X            = -340.f;
    m_fMaster_Position_Y            = 20.f;

    m_fBrainShader                  = 1.f;
    m_fBrainShader1                 = 0.f;
    m_fBrainShader2                 = 0.f;
    m_fBrainShader3                 = 0.f;
    m_fTime                         = 0.f;
    m_fConnectorBlink               = 0.f;
    m_fMouseIndicatorShader         = 0.5f;
    m_fSkillOpen_Gauge              = 0.f;

#pragma endregion
#pragma region _float3
    m_vScale_Background_On          = _float3(198.f, 67.f, 1.f);
    m_vScale_Background_Off         = _float3(186.f, 56.f, 1.f);

    m_vScale_SkillTags              = _float3(125.f, 31.f, 1.f);
    m_vScale_SkillTagLines          = _float3(18.f, 5.f, 1.f);
    m_vScale_SkillIcons             = _float3(52.f, 52.f, 1.f);
    m_vScale_SkillColor             = _float3(52.f, 52.f, 1.f);
    m_vScale_BM_Connector[0]        = _float3(88.f, 88.f, 1.f);
    m_vScale_BM_Connector[1]        = _float3(74.f, 32.f, 1.f);
    m_vScale_BM_Connector[2]        = _float3(36.f, 28.f, 1.f);
    m_vScale_BM_Connector[3]        = _float3(54.f, 26.f, 1.f);
    m_vScale_BM_Connector[4]        = _float3(14.f, 30.f, 1.f);
    m_vScale_BM_Connector[5]        = _float3(14.f, 22.f, 1.f);
    m_vScale_BM_Connector[6]        = _float3(14.f, 14.f, 1.f);
    m_vScale_Brain[0]               = _float3(53.f, 80.f, 1.f);
    m_vScale_Brain[1]               = _float3(154.f, 128.f, 1.f);
    m_vScale_Brain[2]               = _float3(128.f, 107.f, 1.f);
    m_vScale_Brain[3]               = _float3(77.f, 60.f, 1.f);
    m_vScale_Brain[4]               = _float3(258.f, 18.f, 1.f);
    m_vScale_Brain[5]               = _float3(154.f, 128.f, 1.f);
    m_vScale_MouseIndicator         = _float3(88.f, 88.f, 1.f);
    m_vScale_SkillOpen_Gauge        = _float3(57.f, 57.f, 1.f);
    m_vScale_BlackFillter           = _float3(g_iWinCX, g_iWinCY, 1.f);
    m_vScale_BlackFillterWindow     = _float3(590.f, 250.f, 1.f);
    m_vScale_GetSkillTag            = _float3(586.f, 26.f, 1.f);
    m_vScale_GetSkillFrame          = _float3(594.f, 34.f, 1.f);
    m_vScale_GetSkillLine           = _float3(560.f, 5.f, 1.f);
    m_vScale_GetSkillReturn         = _float3(28.f, 28.f, 1.f);

    m_vScale_ExplainFrame           = _float3(600.f, 30.f, 1.f);
    m_vScale_ExplainLine            = _float3(570.f, 5.f, 1.f);
    m_vScale_Video                  = _float3(414.f, 224.f, 1.f);
    m_vScale_VideoFrame             = _float3(420.f, 230.f, 1.f);
    m_vScale_Profile                = _float3(98.f, 93.f, 1.f);
    m_vScale_ProfileFrame           = _float3(135.f, 102.f, 1.f);
    m_vScale_ProfileFrameLine       = _float3(320.f, 3.f, 1.f);
    
#pragma endregion
#pragma region _float4
    m_vPosition_Background_On       = _float4(-540.f, 440.f, 0.5f, 1.f);
    m_vPosition_Background_Off      = _float4(-538.f, 437.f, 0.5f, 1.f);
#pragma region Tags
    m_vPosition_SkillTags[0]        = _float4(m_fMaster_Position_X, m_fMaster_Position_Y, 0.f, 1.f);
    m_vPosition_SkillTags[1]        = _float4(m_fMaster_Position_X - 68.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillTags[2]        = _float4(m_fMaster_Position_X + 68.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillTags[3]        = _float4(m_fMaster_Position_X - 68.f, m_fMaster_Position_Y - 90.f, 0.f, 1.f);
    m_vPosition_SkillTags[4]        = _float4(m_fMaster_Position_X + 68.f, m_fMaster_Position_Y - 90.f, 0.f, 1.f);
#pragma endregion
#pragma region TagLines
    m_vPosition_SkillTagLines[0]    = _float4(m_fMaster_Position_X, m_fMaster_Position_Y + 23.f, 0.f, 1.f);
    m_vPosition_SkillTagLines[1]    = _float4(m_fMaster_Position_X - 138.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillTagLines[2]    = _float4(m_fMaster_Position_X + 138.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillTagLines[3]    = _float4(m_fMaster_Position_X - 68.f, m_fMaster_Position_Y - 113.f, 0.f, 1.f);
    m_vPosition_SkillTagLines[4]    = _float4(m_fMaster_Position_X + 68.f, m_fMaster_Position_Y - 113.f, 0.f, 1.f);
#pragma endregion
#pragma region Brain
    m_vPosition_Brain[0]            = _float4(m_fMaster_Position_X, m_fMaster_Position_Y - 345.f, 0.f, 1.f);
    m_vPosition_Brain[1]            = _float4(m_fMaster_Position_X, m_fMaster_Position_Y - 280.f, 0.f, 1.f);
    m_vPosition_Brain[2]            = _float4(m_fMaster_Position_X, m_fMaster_Position_Y - 280.f, 0.f, 1.f);
    m_vPosition_Brain[3]            = _float4(m_fMaster_Position_X, m_fMaster_Position_Y - 280.f, 0.f, 1.f);
    m_vPosition_Brain[4]            = _float4(m_fMaster_Position_X, m_fMaster_Position_Y - 400.f, 0.f, 1.f);
    m_vPosition_Brain[5]            = _float4(m_fMaster_Position_X, m_fMaster_Position_Y - 280.f, 0.f, 1.f); // For. Background
#pragma endregion
#pragma region Icons
#pragma region 확장
    m_vPosition_SkillIcons[0]       = _float4(m_fMaster_Position_X, m_fMaster_Position_Y + 54.f, 0.f, 1.f);
    m_vPosition_SkillIcons[1]       = _float4(m_fMaster_Position_X - 54.f, m_fMaster_Position_Y + 151.f, 0.f, 1.f);
    m_vPosition_SkillIcons[2]       = _float4(m_fMaster_Position_X + 44.f, m_fMaster_Position_Y + 212.f, 0.f, 1.f);
    m_vPosition_SkillIcons[3]       = _float4(m_fMaster_Position_X - 16.f, m_fMaster_Position_Y + 310.f, 0.f, 1.f);
    m_vPosition_SkillIcons[4]       = _float4(m_fMaster_Position_X + 154.f, m_fMaster_Position_Y + 212.f, 0.f, 1.f);
    m_vPosition_SkillIcons[5]       = _float4(m_fMaster_Position_X + 212.f, m_fMaster_Position_Y + 310.f, 0.f, 1.f);
    m_vPosition_SkillIcons[6]       = _float4(m_fMaster_Position_X + 94.f, m_fMaster_Position_Y + 310.f, 0.f, 1.f);
    m_vPosition_SkillIcons[7]       = _float4(m_fMaster_Position_X - 108.f, m_fMaster_Position_Y + 250.f, 0.f, 1.f);
    m_vPosition_SkillIcons[8]       = _float4(m_fMaster_Position_X - 203.f, m_fMaster_Position_Y + 310.f, 0.f, 1.f);
    m_vPosition_SkillIcons[9]       = _float4(m_fMaster_Position_X - 164.f, m_fMaster_Position_Y + 151.f, 0.f, 1.f);
    m_vPosition_SkillIcons[10]      = _float4(m_fMaster_Position_X + 110.f, m_fMaster_Position_Y + 54.f, 0.f, 1.f);
    m_vPosition_SkillIcons[11]      = _float4(m_fMaster_Position_X + 170.f, m_fMaster_Position_Y + 146.f, 0.f, 1.f);
    m_vPosition_SkillIcons[12]      = _float4(m_fMaster_Position_X + 281.f, m_fMaster_Position_Y + 146.f, 0.f, 1.f);
    m_vPosition_SkillIcons[13]      = _float4(m_fMaster_Position_X + 312.f, m_fMaster_Position_Y + 253.f, 0.f, 1.f);
    m_vPosition_SkillIcons[14]      = _float4(m_fMaster_Position_X + 362.f, m_fMaster_Position_Y + 220.f, 0.f, 1.f);
    m_vPosition_SkillIcons[15]      = _float4(m_fMaster_Position_X + 471.f, m_fMaster_Position_Y + 220.f, 0.f, 1.f);
    m_vPosition_SkillIcons[16]      = _float4(m_fMaster_Position_X + 413.f, m_fMaster_Position_Y + 314.f, 0.f, 1.f);
    m_vPosition_SkillIcons[17]      = _float4(m_fMaster_Position_X + 529.f, m_fMaster_Position_Y + 314.f, 0.f, 1.f);
#pragma endregion
#pragma region 보조
    m_vPosition_SkillIcons[18]      = _float4(m_fMaster_Position_X + 170.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillIcons[19]      = _float4(m_fMaster_Position_X + 201.f, m_fMaster_Position_Y + 61.f, 0.f, 1.f);
    m_vPosition_SkillIcons[20]      = _float4(m_fMaster_Position_X + 310.f, m_fMaster_Position_Y + 61.f, 0.f, 1.f);
    m_vPosition_SkillIcons[21]      = _float4(m_fMaster_Position_X + 390.f, m_fMaster_Position_Y + 135.f, 0.f, 1.f);
    m_vPosition_SkillIcons[22]      = _float4(m_fMaster_Position_X + 500.f, m_fMaster_Position_Y + 135.f, 0.f, 1.f);
    m_vPosition_SkillIcons[23]      = _float4(m_fMaster_Position_X + 500.f, m_fMaster_Position_Y + 25.f, 0.f, 1.f);
    m_vPosition_SkillIcons[24]      = _float4(m_fMaster_Position_X + 420.f, m_fMaster_Position_Y + 61.f, 0.f, 1.f);
    m_vPosition_SkillIcons[25]      = _float4(m_fMaster_Position_X + 265.f, m_fMaster_Position_Y + 12.f, 0.f, 1.f);
    m_vPosition_SkillIcons[26]      = _float4(m_fMaster_Position_X + 374.f, m_fMaster_Position_Y + 12.f, 0.f, 1.f);
    m_vPosition_SkillIcons[27]      = _float4(m_fMaster_Position_X + 469.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillIcons[28]      = _float4(m_fMaster_Position_X + 360.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillIcons[29]      = _float4(m_fMaster_Position_X + 327.f, m_fMaster_Position_Y - 151.f, 0.f, 1.f);
    m_vPosition_SkillIcons[30]      = _float4(m_fMaster_Position_X + 425.f, m_fMaster_Position_Y - 96.f, 0.f, 1.f);
#pragma endregion
#pragma region 브레인 필드
    m_vPosition_SkillIcons[31]      = _float4(m_fMaster_Position_X + 68.f, m_fMaster_Position_Y - 145.f, 0.f, 1.f);
    m_vPosition_SkillIcons[32]      = _float4(m_fMaster_Position_X + 178.f, m_fMaster_Position_Y - 145.f, 0.f, 1.f);
    m_vPosition_SkillIcons[33]      = _float4(m_fMaster_Position_X + 280.f, m_fMaster_Position_Y - 186.f, 0.f, 1.f);
    m_vPosition_SkillIcons[34]      = _float4(m_fMaster_Position_X + 163.f, m_fMaster_Position_Y - 200.f, 0.f, 1.f);
    m_vPosition_SkillIcons[35]      = _float4(m_fMaster_Position_X + 258.f, m_fMaster_Position_Y - 255.f, 0.f, 1.f);
    m_vPosition_SkillIcons[36]      = _float4(m_fMaster_Position_X + 368.f, m_fMaster_Position_Y - 255.f, 0.f, 1.f);
    m_vPosition_SkillIcons[37]      = _float4(m_fMaster_Position_X + 122.f, m_fMaster_Position_Y - 241.f, 0.f, 1.f);
    m_vPosition_SkillIcons[38]      = _float4(m_fMaster_Position_X + 217.f, m_fMaster_Position_Y - 298.f, 0.f, 1.f);//
    m_vPosition_SkillIcons[39]      = _float4(m_fMaster_Position_X + 327.f, m_fMaster_Position_Y - 298.f, 0.f, 1.f);
    m_vPosition_SkillIcons[40]      = _float4(m_fMaster_Position_X + 177.f, m_fMaster_Position_Y - 336.f, 0.f, 1.f);//
#pragma endregion
#pragma region 드라이브
    m_vPosition_SkillIcons[41]      = _float4(m_fMaster_Position_X - 68.f, m_fMaster_Position_Y - 145.f, 0.f, 1.f);
    m_vPosition_SkillIcons[42]      = _float4(m_fMaster_Position_X - 121.f, m_fMaster_Position_Y - 240.f, 0.f, 1.f);
    m_vPosition_SkillIcons[43]      = _float4(m_fMaster_Position_X - 199.f, m_fMaster_Position_Y - 319.f, 0.f, 1.f);
    m_vPosition_SkillIcons[44]      = _float4(m_fMaster_Position_X - 230.f, m_fMaster_Position_Y - 240.f, 0.f, 1.f);
    m_vPosition_SkillIcons[45]      = _float4(m_fMaster_Position_X - 310.f, m_fMaster_Position_Y - 319.f, 0.f, 1.f);
    m_vPosition_SkillIcons[46]      = _float4(m_fMaster_Position_X - 339.f, m_fMaster_Position_Y - 240.f, 0.f, 1.f);
    m_vPosition_SkillIcons[47]      = _float4(m_fMaster_Position_X - 177.f, m_fMaster_Position_Y - 145.f, 0.f, 1.f);
    m_vPosition_SkillIcons[48]      = _float4(m_fMaster_Position_X - 287.f, m_fMaster_Position_Y - 145.f, 0.f, 1.f);
    m_vPosition_SkillIcons[49]      = _float4(m_fMaster_Position_X - 256.f, m_fMaster_Position_Y - 68.f, 0.f, 1.f);
#pragma endregion
#pragma region 강화
    m_vPosition_SkillIcons[50]      = _float4(m_fMaster_Position_X - 170.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillIcons[51]      = _float4(m_fMaster_Position_X - 265.f, m_fMaster_Position_Y + 12.f, 0.f, 1.f);
    m_vPosition_SkillIcons[52]      = _float4(m_fMaster_Position_X - 375.f, m_fMaster_Position_Y + 12.f, 0.f, 1.f);
    m_vPosition_SkillIcons[53]      = _float4(m_fMaster_Position_X - 469.f, m_fMaster_Position_Y - 45.f, 0.f, 1.f);
    m_vPosition_SkillIcons[54]      = _float4(m_fMaster_Position_X - 525.f, m_fMaster_Position_Y - 141.f, 0.f, 1.f);
    m_vPosition_SkillIcons[55]      = _float4(m_fMaster_Position_X - 469.f, m_fMaster_Position_Y + 67.f, 0.f, 1.f);
    m_vPosition_SkillIcons[56]      = _float4(m_fMaster_Position_X - 218.f, m_fMaster_Position_Y + 55.f, 0.f, 1.f);
    m_vPosition_SkillIcons[57]      = _float4(m_fMaster_Position_X - 314.f, m_fMaster_Position_Y + 110.f, 0.f, 1.f);
    m_vPosition_SkillIcons[58]      = _float4(m_fMaster_Position_X - 409.f, m_fMaster_Position_Y + 165.f, 0.f, 1.f);
    m_vPosition_SkillIcons[59]      = _float4(m_fMaster_Position_X - 264.f, m_fMaster_Position_Y + 156.f, 0.f, 1.f);
    m_vPosition_SkillIcons[60]      = _float4(m_fMaster_Position_X - 310.f, m_fMaster_Position_Y + 257.f, 0.f, 1.f);
#pragma endregion
#pragma endregion
#pragma region Connector
#pragma region 확장
    m_vPosition_BM_Connector[0] = _float4(m_fMaster_Position_X - 29.f, m_fMaster_Position_Y + 101.f, 0.f, 1.f);
    m_vPosition_BM_Connector[1] = _float4(m_fMaster_Position_X - 5.f, m_fMaster_Position_Y + 181.f, 0.f, 1.f);
    m_vPosition_BM_Connector[2] = _float4(m_fMaster_Position_X + 13.f, m_fMaster_Position_Y + 258.f, 0.f, 1.f);
    m_vPosition_BM_Connector[3] = _float4(m_fMaster_Position_X + 98.f, m_fMaster_Position_Y + 212.f, 0.f, 1.f);//_float4(m_fMaster_Position_X - 60.f, m_fMaster_Position_Y + 278.f, 0.f, 1.f);
    m_vPosition_BM_Connector[4] = _float4(m_fMaster_Position_X + 184.f, m_fMaster_Position_Y + 260.f, 0.f, 1.f);//_float4(m_fMaster_Position_X + 278.f, m_fMaster_Position_Y + 258.f, 0.f, 1.f);
    m_vPosition_BM_Connector[5] = _float4(m_fMaster_Position_X + 38.f, m_fMaster_Position_Y + 312.f, 0.f, 1.f);
    m_vPosition_BM_Connector[6] = _float4(m_fMaster_Position_X - 60.f, m_fMaster_Position_Y + 279.f, 0.f, 1.f);
    m_vPosition_BM_Connector[7] = _float4(m_fMaster_Position_X - 155.f, m_fMaster_Position_Y + 278.f, 0.f, 1.f);
    m_vPosition_BM_Connector[8] = _float4(m_fMaster_Position_X - 108.f, m_fMaster_Position_Y + 151.f, 0.f, 1.f);
    m_vPosition_BM_Connector[9] = _float4(m_fMaster_Position_X + 55.f, m_fMaster_Position_Y + 54.f, 0.f, 1.f);
    m_vPosition_BM_Connector[10] = _float4(m_fMaster_Position_X + 137.f, m_fMaster_Position_Y + 101.f, 0.f, 1.f);
    m_vPosition_BM_Connector[11] = _float4(m_fMaster_Position_X + 225.f, m_fMaster_Position_Y + 146.f, 0.f, 1.f);
    m_vPosition_BM_Connector[12] = _float4(m_fMaster_Position_X + 296.f, m_fMaster_Position_Y + 199.f, 0.f, 1.f);
    m_vPosition_BM_Connector[13] = _float4(m_fMaster_Position_X + 321.f, m_fMaster_Position_Y + 183.f, 0.f, 1.f);
    m_vPosition_BM_Connector[14] = _float4(m_fMaster_Position_X + 416.f, m_fMaster_Position_Y + 220.f, 0.f, 1.f);
    m_vPosition_BM_Connector[15] = _float4(m_fMaster_Position_X + 442.f, m_fMaster_Position_Y + 265.f, 0.f, 1.f);
    m_vPosition_BM_Connector[16] = _float4(m_fMaster_Position_X + 500.f, m_fMaster_Position_Y + 266.f, 0.f, 1.f);

#pragma endregion
#pragma region 보조
    m_vPosition_BM_Connector[17]    = _float4(m_fMaster_Position_X + 185.f, m_fMaster_Position_Y + 7.f, 0.f, 1.f);
    m_vPosition_BM_Connector[18]    = _float4(m_fMaster_Position_X + 255.f, m_fMaster_Position_Y + 61.f, 0.f, 1.f);
    m_vPosition_BM_Connector[19]    = _float4(m_fMaster_Position_X + 348.f, m_fMaster_Position_Y + 100.f, 0.f, 1.f);
    m_vPosition_BM_Connector[20]    = _float4(m_fMaster_Position_X + 445.f, m_fMaster_Position_Y + 135.f, 0.f, 1.f);
    m_vPosition_BM_Connector[21]    = _float4(m_fMaster_Position_X + 500.f, m_fMaster_Position_Y + 80.f, 0.f, 1.f);
    m_vPosition_BM_Connector[22]    = _float4(m_fMaster_Position_X + 364.f, m_fMaster_Position_Y + 61.f, 0.f, 1.f);
    m_vPosition_BM_Connector[23]    = _float4(m_fMaster_Position_X + 217.f, m_fMaster_Position_Y - 16.f, 0.f, 1.f);
    m_vPosition_BM_Connector[24]    = _float4(m_fMaster_Position_X + 319.f, m_fMaster_Position_Y + 12.f, 0.f, 1.f);
    m_vPosition_BM_Connector[25]    = _float4(m_fMaster_Position_X + 423.f, m_fMaster_Position_Y - 15.f, 0.f, 1.f);
    m_vPosition_BM_Connector[26]    = _float4(m_fMaster_Position_X + 313.f, m_fMaster_Position_Y - 16.f, 0.f, 1.f);
    m_vPosition_BM_Connector[27]    = _float4(m_fMaster_Position_X + 344.f, m_fMaster_Position_Y - 97.f, 0.f, 1.f);
    m_vPosition_BM_Connector[28]    = _float4(m_fMaster_Position_X + 375.f, m_fMaster_Position_Y - 124.f, 0.f, 1.f);
#pragma endregion
#pragma region 브레인 필드
    m_vPosition_BM_Connector[29]    = _float4(m_fMaster_Position_X + 123.f, m_fMaster_Position_Y - 145.f, 0.f, 1.f);
    m_vPosition_BM_Connector[30]    = _float4(m_fMaster_Position_X + 228.f, m_fMaster_Position_Y - 167.f, 0.f, 1.f);
    m_vPosition_BM_Connector[31]    = _float4(m_fMaster_Position_X + 115.f, m_fMaster_Position_Y - 172.f, 0.f, 1.f);
    m_vPosition_BM_Connector[32]    = _float4(m_fMaster_Position_X + 210.f, m_fMaster_Position_Y - 227.f, 0.f, 1.f);
    m_vPosition_BM_Connector[33]    = _float4(m_fMaster_Position_X + 312.f, m_fMaster_Position_Y - 255.f, 0.f, 1.f);
    m_vPosition_BM_Connector[34]    = _float4(m_fMaster_Position_X + 94.f, m_fMaster_Position_Y - 192.f, 0.f, 1.f);
    m_vPosition_BM_Connector[35]    = _float4(m_fMaster_Position_X + 169.f, m_fMaster_Position_Y - 268.f, 0.f, 1.f); //
    m_vPosition_BM_Connector[36]    = _float4(m_fMaster_Position_X + 271.f, m_fMaster_Position_Y - 298.f, 0.f, 1.f);
    m_vPosition_BM_Connector[37]    = _float4(m_fMaster_Position_X + 147.f, m_fMaster_Position_Y - 288.f, 0.f, 1.f); //
#pragma endregion
#pragma region 드라이브
    m_vPosition_BM_Connector[38]    = _float4(m_fMaster_Position_X - 93.f, m_fMaster_Position_Y - 192.f, 0.f, 1.f);
    m_vPosition_BM_Connector[39]    = _float4(m_fMaster_Position_X - 159.f, m_fMaster_Position_Y - 280.f, 0.f, 1.f);
    m_vPosition_BM_Connector[40]    = _float4(m_fMaster_Position_X - 175.f, m_fMaster_Position_Y - 240.f, 0.f, 1.f);
    m_vPosition_BM_Connector[41]    = _float4(m_fMaster_Position_X - 269.f, m_fMaster_Position_Y - 280.f, 0.f, 1.f);
    m_vPosition_BM_Connector[42]    = _float4(m_fMaster_Position_X - 284.f, m_fMaster_Position_Y - 240.f, 0.f, 1.f);
    m_vPosition_BM_Connector[43]    = _float4(m_fMaster_Position_X - 122.f, m_fMaster_Position_Y - 145.f, 0.f, 1.f);
    m_vPosition_BM_Connector[44]    = _float4(m_fMaster_Position_X - 232.f, m_fMaster_Position_Y - 145.f, 0.f, 1.f);
    m_vPosition_BM_Connector[45]    = _float4(m_fMaster_Position_X - 215.f, m_fMaster_Position_Y - 107.f, 0.f, 1.f);
#pragma endregion
#pragma region 강화
    m_vPosition_BM_Connector[46]    = _float4(m_fMaster_Position_X - 217.f, m_fMaster_Position_Y - 16.f, 0.f, 1.f);
    m_vPosition_BM_Connector[47]    = _float4(m_fMaster_Position_X - 320.f, m_fMaster_Position_Y + 12.f, 0.f, 1.f);
    m_vPosition_BM_Connector[48]    = _float4(m_fMaster_Position_X - 422.f, m_fMaster_Position_Y - 16.f, 0.f, 1.f);
    m_vPosition_BM_Connector[49]    = _float4(m_fMaster_Position_X - 494.f, m_fMaster_Position_Y - 93.f, 0.f, 1.f);
    m_vPosition_BM_Connector[50]    = _float4(m_fMaster_Position_X - 422.f, m_fMaster_Position_Y + 40.f, 0.f, 1.f);
    m_vPosition_BM_Connector[51]    = _float4(m_fMaster_Position_X - 192.f, m_fMaster_Position_Y + 5.f, 0.f, 1.f);
    m_vPosition_BM_Connector[52]    = _float4(m_fMaster_Position_X - 264.f, m_fMaster_Position_Y + 83.f, 0.f, 1.f);
    m_vPosition_BM_Connector[53]    = _float4(m_fMaster_Position_X - 361.f, m_fMaster_Position_Y + 137.f, 0.f, 1.f);
    m_vPosition_BM_Connector[54]    = _float4(m_fMaster_Position_X - 241.f, m_fMaster_Position_Y + 105.f, 0.f, 1.f);
    m_vPosition_BM_Connector[55]    = _float4(m_fMaster_Position_X - 292.f, m_fMaster_Position_Y + 203.f, 0.f, 1.f);
#pragma endregion
#pragma endregion
#pragma region Color
    for (size_t i = 0; i < 61; i++)
    {
        m_vPosition_SkillColor[i] = m_vPosition_SkillIcons[i];
    }
#pragma endregion
#pragma region MouseIndicator
    m_vPosition_MouseIndicator      = _float4(m_vPosition_SkillIcons[0].x, m_vPosition_SkillIcons[0].y, 0.f, 1.f);
#pragma endregion
#pragma region SkillOpen_Gauge
    m_vPosition_SkillOpen_Gauge     = _float4(0.f, 0.f, 0.f, 1.f);
#pragma endregion
#pragma endregion
    m_vPosition_BlackFillter        = _float4(0.f, 0.f, 0.f, 1.f);

    m_vPosition_BlackFillterWindow  = _float4(0.f, -70.f, 0.f, 1.f);
    m_vPosition_GetSkillTag         = _float4(0.f, 80.f, 0.f, 1.f);
    m_vPosition_GetSkillFrame[0]    = _float4(0.f, 42.f, 0.f, 1.f);
    m_vPosition_GetSkillFrame[1]    = _float4(0.f, -182.f, 0.f, 1.f);
    m_vPosition_GetSkillLine        = _float4(0.f, -133.f, 0.f, 1.f);
    m_vPosition_GetSkillReturn      = _float4(200.f, -163.f, 0.f, 1.f);

    m_vPosition_ExplainFrameT       = _float4(600.f, 330.f, 0.f, 1.f);
    m_vPosition_ExplainFrameB       = _float4(600.f, -340.f, 0.f, 1.f);
    m_vPosition_ExplainFrameLine[0] = _float4(600.f, 275.f, 0.f, 1.f);
    m_vPosition_ExplainFrameLine[1] = _float4(600.f, -100.f, 0.f, 1.f);
    m_vPosition_ExplainFrameLine[2] = _float4(600.f, -305.f, 0.f, 1.f);
    m_vPosition_Video               = _float4(600.f, 150.f, 0.f, 1.f);
    m_vPosition_VideoFrame          = _float4(600.f, 150.f, 0.f, 1.f);
    m_vPosition_Profile             = _float4(440.f, -30.f, 0.f, 1.f);
    m_vPosition_ProfileFrame        = _float4(457.f, -30.f, 0.f, 1.f);
    m_vPosition_ProfileFrameLine[0] = _float4(655.f, -4.5f, 0.f, 1.f);
    m_vPosition_ProfileFrameLine[1] = _float4(655.f, -30.5f, 0.f, 1.f);
    m_vPosition_ProfileFrameLine[2] = _float4(655.f, -57.f, 0.f, 1.f);

#pragma region wstring
    // SkillIcon - Tag
    m_strSkillTags[0]               = TEXT("확장");
    m_strSkillTags[1]               = TEXT("강화");
    m_strSkillTags[2]               = TEXT("보조");
    m_strSkillTags[3]               = TEXT("드라이브");
    m_strSkillTags[4]               = TEXT("브레인 필드");

    // GetSkill - Small Window
    m_strGetSkillTag                = TEXT("스킬 습득");
    m_strGetSkillName               = TEXT("");
    m_strGetSkillEnd                = TEXT("습득했습니다.");
    m_strGetSkillClose              = TEXT("닫기");

    // Skill - Explain
    m_strExplainTitle               = TEXT("");
    m_strHP                         = TEXT("체력");
    m_strAblilty                    = TEXT("초뇌능력");
    m_strAttack                     = TEXT("공격력");
    m_strDefence                    = TEXT("방어력");
    m_strExplain1                   = TEXT("");
    m_strExplain2                   = TEXT("");
    m_strExplain3                   = TEXT("");
    m_strExplain4                   = TEXT("");
    m_strGetorNot                   = TEXT("");

    m_strPlayerHp                   = TEXT("100");
    m_strPlayerAblity               = TEXT("76");
    m_strPlayerAttack               = TEXT("71");
    m_strPlayerDefence              = TEXT("29");

#pragma endregion
#pragma region Set_Pivot
    _matrix PivotMatrix             = XMMatrixIdentity();
    PivotMatrix                     = XMMatrixRotationZ(XMConvertToRadians(90.f));
    m_pSkillTagLines[0]->Set_Matrix(PivotMatrix);
    m_pSkillTagLines[3]->Set_Matrix(PivotMatrix);
    m_pSkillTagLines[4]->Set_Matrix(PivotMatrix);

    m_pSkillOpen_Gauge->Set_Matrix(PivotMatrix);

#pragma region 확장
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(120.f));
    m_pBM_Connector[0]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(30.f));
    m_pBM_Connector[1]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(120.f));
    m_pBM_Connector[2]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(60.f));
    m_pBM_Connector[4]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(210.f));
    m_pBM_Connector[6]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(150.f));
    m_pBM_Connector[7]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(180.f));
    m_pBM_Connector[8]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(60.f));
    m_pBM_Connector[10]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(75.f));
    m_pBM_Connector[12]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(45.f));
    m_pBM_Connector[13]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(120.f));
    m_pBM_Connector[15]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(60.f));
    m_pBM_Connector[16]->Set_Matrix(PivotMatrix);
#pragma endregion
#pragma region 보조
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(75.f));
    m_pBM_Connector[17]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(45.f));
    m_pBM_Connector[19]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(270.f));
    m_pBM_Connector[21]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(30.f));
    m_pBM_Connector[23]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(330.f));
    m_pBM_Connector[25]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(330.f));
    m_pBM_Connector[26]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(255.f));
    m_pBM_Connector[27]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(30.f));
    m_pBM_Connector[28]->Set_Matrix(PivotMatrix);
#pragma endregion
#pragma region 브레인 필드
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-20.f));
    m_pBM_Connector[30]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-30.f));
    m_pBM_Connector[31]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-30.f));
    m_pBM_Connector[32]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-60.f));
    m_pBM_Connector[34]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-30.f));
    m_pBM_Connector[35]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-60.f));
    m_pBM_Connector[37]->Set_Matrix(PivotMatrix);
#pragma endregion
#pragma region 드라이브
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-120.f));
    m_pBM_Connector[38]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(225.f));
    m_pBM_Connector[39]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(180.f));
    m_pBM_Connector[40]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(225.f));
    m_pBM_Connector[41]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(180.f));
    m_pBM_Connector[42]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(180.f));
    m_pBM_Connector[43]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(180.f));
    m_pBM_Connector[44]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(135.f));
    m_pBM_Connector[45]->Set_Matrix(PivotMatrix);
#pragma endregion
#pragma region 강화
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(150.f));
    m_pBM_Connector[46]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(180.f));
    m_pBM_Connector[47]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(210.f));
    m_pBM_Connector[48]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(240.f));
    m_pBM_Connector[49]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(150.f));
    m_pBM_Connector[50]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(115.f));
    m_pBM_Connector[51]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(150.f));
    m_pBM_Connector[52]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(150.f));
    m_pBM_Connector[53]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(115.f));
    m_pBM_Connector[54]->Set_Matrix(PivotMatrix);

    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(115.f));
    m_pBM_Connector[55]->Set_Matrix(PivotMatrix);
#pragma endregion


#pragma endregion
#pragma region Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background_Off);
    for (size_t i = 0; i < 5; i++)
    {
        m_pSkillTags[i]->Set_Scale(m_vScale_SkillTags);
        m_pSkillTagLines[i]->Set_Scale(m_vScale_SkillTagLines);
    }
    for (size_t i = 0; i < 61; i++)
    {
        m_pSkillIcons[i]->Set_Scale(m_vScale_SkillIcons);
        m_pSkillColor[i]->Set_Scale(m_vScale_SkillColor);
    }
    for (size_t i = 0; i < 59; i++)
    {
        m_pBM_Connector[i]->Set_Scale(m_vScale_BM_Connector[1]);
    }
    for (size_t i = 0; i < 6; i++)
    {
        m_pBrain[i]->Set_Scale(m_vScale_Brain[i]);
    }
    m_pMouseIndicator->Set_Scale(m_vScale_MouseIndicator);
    m_pSkillOpen_Gauge->Set_Scale(m_vScale_SkillOpen_Gauge);
    m_pBlackFillter->Set_Scale(m_vScale_BlackFillter);
    m_pBlackFillterWindow->Set_Scale(m_vScale_BlackFillterWindow);
    m_pGetSkillTag->Set_Scale(m_vScale_GetSkillTag);
    m_pGetSkillFrameT->Set_Scale(m_vScale_GetSkillFrame);
    m_pGetSkillFrameB->Set_Scale(m_vScale_GetSkillFrame);
    m_pGetSkillLine->Set_Scale(m_vScale_GetSkillLine);
    m_pGetSkillReturn->Set_Scale(m_vScale_GetSkillReturn);
    
    m_pExplainFrameT->Set_Scale(m_vScale_ExplainFrame);
    m_pExplainFrameB->Set_Scale(m_vScale_ExplainFrame);
    m_pExplainFrameLine1->Set_Scale(m_vScale_ExplainLine);
    m_pExplainFrameLine2->Set_Scale(m_vScale_ExplainLine);
    m_pExplainFrameLine3->Set_Scale(m_vScale_ExplainLine);
    m_pExplainVideo->Set_Scale(m_vScale_Video);
    m_pExplainVideoFrame->Set_Scale(m_vScale_VideoFrame);
    m_pExplainProfile->Set_Scale(m_vScale_Profile);
    m_pExplainProfileFrame->Set_Scale(m_vScale_ProfileFrame);
    m_pExplainProfileFrameLine1->Set_Scale(m_vScale_ProfileFrameLine);
    m_pExplainProfileFrameLine2->Set_Scale(m_vScale_ProfileFrameLine);
    m_pExplainProfileFrameLine3->Set_Scale(m_vScale_ProfileFrameLine);



#pragma endregion
#pragma region Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background_Off);

    for (size_t i = 0; i < 5; i++)
    {
        m_pSkillTags[i]->Set_State(TRANSFORM::POSITION, m_vPosition_SkillTags[i]);
        m_pSkillTagLines[i]->Set_State(TRANSFORM::POSITION, m_vPosition_SkillTagLines[i]);
    }
    for (size_t i = 0; i < 61; i++)
    {
        m_pSkillIcons[i]->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIcons[i]);
        m_pSkillColor[i]->Set_State(TRANSFORM::POSITION, m_vPosition_SkillColor[i]);
    }
    for (size_t i = 0; i < 59; i++)
    {
        m_pBM_Connector[i]->Set_State(TRANSFORM::POSITION, m_vPosition_BM_Connector[i]);
    }
    for (size_t i = 0; i < 6; i++)
    {
        m_pBrain[i]->Set_State(TRANSFORM::POSITION, m_vPosition_Brain[i]);
    }
    m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);
    m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);
    m_pBlackFillter->Set_State(TRANSFORM::POSITION, m_vPosition_BlackFillter);
    m_pBlackFillterWindow->Set_State(TRANSFORM::POSITION, m_vPosition_BlackFillterWindow);
    m_pGetSkillTag->Set_State(TRANSFORM::POSITION, m_vPosition_GetSkillTag);
    m_pGetSkillFrameT->Set_State(TRANSFORM::POSITION, m_vPosition_GetSkillFrame[0]);
    m_pGetSkillFrameB->Set_State(TRANSFORM::POSITION, m_vPosition_GetSkillFrame[1]);
    m_pGetSkillLine->Set_State(TRANSFORM::POSITION, m_vPosition_GetSkillLine);
    m_pGetSkillReturn->Set_State(TRANSFORM::POSITION, m_vPosition_GetSkillReturn);

    m_pExplainFrameT->Set_State(TRANSFORM::POSITION, m_vPosition_ExplainFrameT);
    m_pExplainFrameB->Set_State(TRANSFORM::POSITION, m_vPosition_ExplainFrameB);
    m_pExplainFrameLine1->Set_State(TRANSFORM::POSITION, m_vPosition_ExplainFrameLine[0]);
    m_pExplainFrameLine2->Set_State(TRANSFORM::POSITION, m_vPosition_ExplainFrameLine[1]);
    m_pExplainFrameLine3->Set_State(TRANSFORM::POSITION, m_vPosition_ExplainFrameLine[2]);
    m_pExplainVideo->Set_State(TRANSFORM::POSITION, m_vPosition_Video);
    m_pExplainVideoFrame->Set_State(TRANSFORM::POSITION, m_vPosition_VideoFrame);
    m_pExplainProfile->Set_State(TRANSFORM::POSITION, m_vPosition_Profile);
    m_pExplainProfileFrame->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrame);
    m_pExplainProfileFrameLine1->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameLine[0]);
    m_pExplainProfileFrameLine2->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameLine[1]);
    m_pExplainProfileFrameLine3->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameLine[2]);


#pragma endregion
}

void CUI_Menu_Category_BrainMap::Tick_UI(_float fTimeDelta)
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (1 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            Menu_Shift();
            Set_BrainShader(fTimeDelta);
            Set_MousePos();
            Set_MouseIndicator_Shader(fTimeDelta);
            Set_MouseIndicator_Position(fTimeDelta);
            Set_ConnectorBlink(fTimeDelta);
            Set_BlackFillterClose();
        }
    }
}

void CUI_Menu_Category_BrainMap::Menu_Shift()
{
    if (CGameInstance::Get_Instance()->Key_Down(0x31)) // 1
    {
        if (1 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_pBackground->Set_Scale(m_vScale_Background_On);
            m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background_On);
        }
        else if(1 != CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_pBackground->Set_Scale(m_vScale_Background_Off);
            m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background_Off);
        }
    }
    if (CGameInstance::Get_Instance()->Key_Down(0x33)) // 3
    {
        if (1 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_pBackground->Set_Scale(m_vScale_Background_On);
            m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background_On);
        }
        else if (1 != CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_pBackground->Set_Scale(m_vScale_Background_Off);
            m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background_Off);
        }
    }
}

void CUI_Menu_Category_BrainMap::Set_BrainShader(_float fTimeDelta)
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (1 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            if (0 == m_iBrainPhase)
            {
                if (2.f <= m_fBrainShader1)
                {
                    m_fBrainShader1 = 2.f;
                    m_iBrainPhase += 1;
                }
                else
                {
                    m_fBrainShader1 += fTimeDelta * 2.f;
                }
            }
            else if (1 == m_iBrainPhase)
            {
                if (0.f >= m_fBrainShader1)
                {
                    m_fBrainShader1 = 0.f;
                    m_iBrainPhase += 1;
                }
                else
                {
                    m_fBrainShader1 -= fTimeDelta * 2.f;
                }
            }
            else if (2 == m_iBrainPhase)
            {
                if (2.f <= m_fBrainShader2)
                {
                    m_fBrainShader2 = 2.f;
                    m_iBrainPhase += 1;
                }
                else
                {
                    m_fBrainShader2 += fTimeDelta * 2.f;
                }
            }
            else if (3 == m_iBrainPhase)
            {
                if (0.f >= m_fBrainShader2)
                {
                    m_fBrainShader2 = 0.f;
                    m_iBrainPhase += 1;
                }
                else
                {
                    m_fBrainShader2 -= fTimeDelta * 2.f;
                }
            }
            else if (4 == m_iBrainPhase)
            {
                if (2.f <= m_fBrainShader3)
                {
                    m_fBrainShader3 = 2.f;
                    m_iBrainPhase += 1;
                }
                else
                {
                    m_fBrainShader3 += fTimeDelta * 2.f;
                }
            }
            else if (5 == m_iBrainPhase)
            {
                if (0.f >= m_fBrainShader3)
                {
                    m_fBrainShader3 = 0.f;
                    m_iBrainPhase = 0;
                }
                else
                {
                    m_fBrainShader3 -= fTimeDelta * 2.f;
                }
            }
        }
    }
}

void CUI_Menu_Category_BrainMap::Set_MousePos()
{
    // Set Mouse_X
    m_ptMousePos.x = CGameInstance::Get_Instance()->Get_CursorPos().x - (g_iWinCX * 0.5f);

    // Set Mouse_Y
    m_ptMousePos.y = (g_iWinCY * 0.5f) - CGameInstance::Get_Instance()->Get_CursorPos().y;
}

void CUI_Menu_Category_BrainMap::Set_MouseIndicator_Shader(_float fTimeDelta)
{
    m_fMouseIndicatorShader += fTimeDelta * m_iMouseIndicatorShaderBlink * 0.5f;

    if (0.65f <= m_fMouseIndicatorShader)
    {
        m_iMouseIndicatorShaderBlink = -1;
    }
    if (0.25f >= m_fMouseIndicatorShader)
    {
        m_iMouseIndicatorShaderBlink = 1;
    }
}

void CUI_Menu_Category_BrainMap::Set_MouseIndicator_Position(_float fTimeDelta)
{
    if (m_vPosition_SkillIcons[0].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[0].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[0].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[0].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[0].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[0].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[0].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[0].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("공중 점프");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("공중에서 한번 더 점프 할 수 있다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (0 == m_iCursorSoundOnce[0])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[0] = 1;
        }

        if (true == m_bSkillOnOff[0])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(0))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[0])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[0] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[0] = true;
                    m_bBlackFillter = true;
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    m_fSkillOpen_Gauge = 0.f;
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(0, true);
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[0] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[0] = 0;
    }

    if (m_vPosition_SkillIcons[1].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[1].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[1].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[1].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[1].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[1].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[1].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[1].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("공중 스탭");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("공중에서 공중 스텝을 할 수 있다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[1])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[1])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[1] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(1))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[1])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[1] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(1, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[1] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;

                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[1] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[1] = 0;
    }

    if (m_vPosition_SkillIcons[2].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[2].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[2].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[2].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[2].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[2].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[2].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[2].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("아이템 끌어당기기");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("근처에 떨어져있는 드롭 아이템을 자동으로 끌어당긴다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[2])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[2])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[2] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(2))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[2])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[2] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(2, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[2] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[2] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[2] = 0;
    }

    if (m_vPosition_SkillIcons[3].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[3].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[3].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[3].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[3].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[3].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[3].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[3].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("추가 공격 확장 1");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("염력 추가 공격과 러시 추가 공격을");
        m_strExplain2 = TEXT("3회까지 연속해서 할 수 있다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[3])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[3])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[3] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(3))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[3])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[3] = 1;
                }
                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(3, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[3] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[3] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[3] = 0;
    }

    if (m_vPosition_SkillIcons[4].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[4].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[4].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[4].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[4].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[4].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[4].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[4].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("내려치기 공격");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("체공 중에 공중에서 하강하며 무기 공격을 한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[4])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[4])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[4] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(4))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[4])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[4] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(4, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[4] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[4] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[4] = 0;


    }

    if (m_vPosition_SkillIcons[5].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[5].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[5].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[5].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[5].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[5].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[5].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[5].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("연속 공중 스텝");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("공중 스텝 중 한번 더 스텝할 수 있다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[5])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[5])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[5] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(5))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[5])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[5] = 1;
                }
                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(5, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[5] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[5] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[5] = 0;
    }


    if (m_vPosition_SkillIcons[6].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[6].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[6].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[6].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[6].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[6].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[6].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[6].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("무기 콤보 4단");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("무기 공격을 4회까지 연속해서 할 수 있다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[6])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }


        if (0 == m_iCursorSoundOnce[6])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[6] = 1;

        }
        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(6))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[6])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[6] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(6, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[6] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[6] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[6] = 0;
    }

    if (m_vPosition_SkillIcons[7].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[7].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[7].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[7].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[7].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[7].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[7].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[7].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("차지 공격");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("힘을 모아 위력을 높인 무기 공격을 한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[7])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }


        if (0 == m_iCursorSoundOnce[7])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[7] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(7))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[7])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[7] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(7, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[7] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[7] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[7] = 0;
    }

    if (m_vPosition_SkillIcons[8].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[8].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[8].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[8].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[8].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[8].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[8].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[8].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("추가 공격 확장 2");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("염력 추가 공격과 러시 추가 공격을");
        m_strExplain2 = TEXT("4회까지 연속해서 사용할 수 있다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[8])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[8])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[8] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(8))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[8])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[8] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(8, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[8] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[8] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[8] = 0;
    }

    if (m_vPosition_SkillIcons[9].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[9].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[9].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[9].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[9].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[9].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[9].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[9].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("저스트 회피 공격");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("저스트 회피 성공 시 무기 공격을 한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[9])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[9])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[9] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(9))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;


                if (0 == m_iClickSoundOnce[9])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[9] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(9, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[9] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[9] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[9] = 0;
    }

    if (m_vPosition_SkillIcons[10].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[10].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[10].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[10].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[10].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[10].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[10].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[10].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("낙법");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("날려질 때 자세를 바로 잡아 다운을 방지한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[10])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[10])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[10] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(10))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[10])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[10] = 1;
                }
                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(10, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[10] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[10] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[10] = 0;


    }

    if (m_vPosition_SkillIcons[11].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[11].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[11].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[11].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[11].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[11].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[11].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[11].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("염력 후속타");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("다운된 적에게 염력 공격을 하면");
        m_strExplain2 = TEXT("내려치듯이 염력 공격을 한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[11])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[11])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[11] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(11))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[11])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[11] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(11, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[11] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[11] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[11] = 0;


    }

    if (m_vPosition_SkillIcons[12].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[12].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[12].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[12].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[12].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[12].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[12].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[12].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("공중 추가 공격");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("공중 무기 콤보 중에 공중 추가 공격을 한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[12])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[12])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[12] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(12))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[12])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[12] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(12, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[12] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[12] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[12] = 0;


    }


    if (m_vPosition_SkillIcons[13].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[13].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[13].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[13].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[13].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[13].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[13].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[13].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("백스텝 공격 확장");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("카사네에게 적합한 거리까지 스텝으로 후퇴하면서");
        m_strExplain2 = TEXT("무기 공격을 하는 <백스텝 공격> 을 ");
        m_strExplain3 = TEXT("한 콤보 중에 2번까지 할 수 있다.");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[13])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[13])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[13] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(13))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[13])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[13] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(13, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[13] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[13] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[13] = 0;
    }

    if (m_vPosition_SkillIcons[14].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[14].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[14].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[14].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[14].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[14].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[14].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[14].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("저스트 회피 염력 게이지 회복");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("저스트 회피 성공 시 염력 게이지를 조금 회복한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[14])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[14])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[14] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(14))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[14])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[14] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(14, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[14] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[14] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[14] = 0;
    }

    if (m_vPosition_SkillIcons[15].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[15].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[15].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[15].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[15].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[15].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[15].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[15].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("공중 대시 공격");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("<공중 스텝> 중 전진하면서 무기 공격을 한다.");
        m_strExplain2 = TEXT("필요스킬 <공중 스텝>");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[15])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[15])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[15] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(15))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;


                if (0 == m_iClickSoundOnce[15])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[15] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(15, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[15] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[15] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[15] = 0;
    }

    if (m_vPosition_SkillIcons[16].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[16].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[16].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[16].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[16].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[16].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[16].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[16].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("공중 무기 콤보 3단");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("공중에서 무기 공격을 3회까지 연속으로 할 수 있다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[16])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[16])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[16] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(16))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[16])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[16] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(16, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[16] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[16] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[16] = 0;
    }

    if (m_vPosition_SkillIcons[17].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[17].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[17].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[17].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[17].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[17].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[17].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[17].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("공중 추가 공격 확장");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("공중 추가 공격을 2회까지 연속해서 할 수 있다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[17])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[17])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[17] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(17))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[17])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[17] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(17, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[17] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[17] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[17] = 0;
    }


    if (m_vPosition_SkillIcons[18].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[18].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[18].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[18].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[18].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[18].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[18].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[18].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("잠재 뇌능력");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("체력이 30% 이하일 때, 주는 데미지가 20% 증가하고");
        m_strExplain2 = TEXT("받는 데미지가 20% 감소한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[18])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[18])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[18] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(18))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[18])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[18] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(18, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[18] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[18] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[18] = 0;
    }

    if (m_vPosition_SkillIcons[19].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[19].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[19].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[19].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[19].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[19].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[19].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[19].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 크러시 SAS 회복");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("SAS 효과 중에 브레인 크러시로 적을 쓰러뜨리면");
        m_strExplain2 = TEXT("사용 중인 SAS의 SAS 게이지가 조금 회복한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[19])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[19])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[19] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(19))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[19])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[19] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(19, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[19] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[19] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[19] = 0;
    }


    if (m_vPosition_SkillIcons[20].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[20].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[20].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[20].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[20].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[20].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[20].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[20].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("아이템 사용 단축 제한");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("아이템 사용 후의 사용 제한 시간을 단축시킨다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[20])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[20])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[20] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(20))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[20])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[20] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(20, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[20] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[20] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[20] = 0;
    }


    if (m_vPosition_SkillIcons[21].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[21].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[21].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[21].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[21].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[21].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[21].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[21].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("오버 킬");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("크러시 게이지가 다 깎여 활동을 정지한");
        m_strExplain2 = TEXT("적에게 공격을 명중시키면 EXP 보너스가 증가한다.");
        m_strExplain3 = TEXT("단, 브레인 크러시로밖에 쓰러뜨릴 수 없는 강적의 경우,");
        m_strExplain4 = TEXT("체력을 다 깎은 상황에서는 효과가 없다.");

        if (true == m_bSkillOnOff[21])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[21])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[21] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(21))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[21])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[21] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(21, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[21] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[21] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[21] = 0;
    }

    if (m_vPosition_SkillIcons[22].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[22].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[22].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[22].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[22].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[22].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[22].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[22].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 크러시 찬스 연장");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("적의 크러시 게이지를 다 깎은 후");
        m_strExplain2 = TEXT("적이 더 오랜 시간 활동을 정지하게 되어");
        m_strExplain3 = TEXT("브레인 크러시 가능한 시간이 연장된다.");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[22])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[22])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[22] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(22))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[22])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[22] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(22, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[22] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[22] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[22] = 0;
    }


    if (m_vPosition_SkillIcons[23].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[23].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[23].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[23].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[23].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[23].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[23].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[23].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("SAS 동시 발동");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("같은 이름의 스킬이 2개 있으며,");
        m_strExplain2 = TEXT("하나를 습득하면, SAS를 동시에 2개까지 발동할 수 있으며,");
        m_strExplain3 = TEXT("2개를 습득하면, SAS를 동시에 4개까지 발동할 수 있다.");
        m_strExplain4 = TEXT("SAS 발화와 SAS 방전은 동시에 발동할 수 없다.");

        if (true == m_bSkillOnOff[23])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[23])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[23] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(23))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[23])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[23] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(23, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[23] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[23] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[23] = 0;
    }

    if (m_vPosition_SkillIcons[24].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[24].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[24].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[24].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[24].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[24].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[24].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[24].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("아이템 즉시 효과");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("아이템 사용 시의 동작을 생략해");
        m_strExplain2 = TEXT("즉시 효과를 얻을 수 있다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[24])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[24])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[24] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(24))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[24])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[24] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(24, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[24] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[24] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[24] = 0;
    }

    if (m_vPosition_SkillIcons[25].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[25].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[25].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[25].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[25].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[25].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[25].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[25].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("플러그인 장비 슬롯 증가 1");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("장비 화면에서 적용할 수 있는 플러그인이 하나 더 증가한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[25])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[25])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[25] = 1;
        }


        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(25))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[25])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[25] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(25, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[25] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[25] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[25] = 0;
    }

    if (m_vPosition_SkillIcons[26].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[26].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[26].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[26].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[26].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[26].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[26].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[26].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 크러시 획득 금액 보너스");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("브레인 크러시로 적을 쓰러뜨리면");
        m_strExplain2 = TEXT("적을 쓰러뜨렸을 때 입수할 수 있는 돈이 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[26])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }


        if (0 == m_iCursorSoundOnce[26])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[26] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(26))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[26])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[26] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(26, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[26] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[26] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[26] = 0;
    }

    if (m_vPosition_SkillIcons[27].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[27].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[27].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[27].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[27].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[27].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[27].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[27].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("SAS 동시 발동");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("같은 이름의 스킬이 2개 있으며,");
        m_strExplain2 = TEXT("하나를 습득하면, SAS를 동시에 2개까지 발동할 수 있으며,");
        m_strExplain3 = TEXT("2개를 습득하면, SAS를 동시에 4개까지 발동할 수 있다.");
        m_strExplain4 = TEXT("SAS 발화와 SAS 방전은 동시에 발동할 수 없다.");

        if (true == m_bSkillOnOff[27])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }


        if (0 == m_iCursorSoundOnce[27])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[27] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(27))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[27])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[27] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(27, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[27] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[27] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {


        m_iCursorSoundOnce[27] = 0;
    }


    if (m_vPosition_SkillIcons[28].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[28].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[28].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[28].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[28].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[28].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[28].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[28].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("SAS 동시 발동");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("같은 이름의 스킬이 2개 있으며,");
        m_strExplain2 = TEXT("하나를 습득하면, SAS를 동시에 2개까지 발동할 수 있으며,");
        m_strExplain3 = TEXT("2개를 습득하면, SAS를 동시에 4개까지 발동할 수 있다.");
        m_strExplain4 = TEXT("SAS 발화와 SAS 방전은 동시에 발동할 수 없다.");

        if (true == m_bSkillOnOff[28])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[28])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[28] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(28))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[28])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[28] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(28, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[28] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[28] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {


        m_iCursorSoundOnce[28] = 0;
    }

    if (m_vPosition_SkillIcons[29].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[29].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[29].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[29].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[29].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[29].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[29].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[29].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 크러시 획득 EXP 보너스");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("브레인 크러시로 적을 쓰러뜨리면 획득 EXP가 증가한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[29])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[29])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[29] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(29))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[29])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[29] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(29, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[29] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[29] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[29] = 0;
    }


    if (m_vPosition_SkillIcons[30].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[30].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[30].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[30].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[30].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[30].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[30].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[30].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("체력 자동 회복");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("비전투 시 체력이 서서히 회복된다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[30])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[30])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[30] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(30))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[30])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[30] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(30, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[30] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_iClickSoundOnce[30] = 0;
                m_bSkillOpen_Gauge = false;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[30] = 0;
    }

    if (m_vPosition_SkillIcons[31].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[31].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[31].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[31].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[31].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[31].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);


        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[31].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[31].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 시간 연장");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("브레인 필드의 전개 시간을 5초 연장한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[31])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[31])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[31] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(31))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[31])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[31] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(31, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[31] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[31] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[31] = 0;
    }


    if (m_vPosition_SkillIcons[32].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[32].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[32].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[32].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[32].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[32].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);


        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[32].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[32].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 차지 투하 레벨 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("자신이 브레인 필드를 전개하고 있을 때");
        m_strExplain2 = TEXT("[G] 길게 누르기로 모이는 오브젝트가 증가하여");
        m_strExplain3 = TEXT("오브젝트를 낙하시켜서 주는 데미지가 증가한다.");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[32])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[32])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[32] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(32))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[32])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[32] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(32, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[32] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[32] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[32] = 0;
    }


    if (m_vPosition_SkillIcons[33].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[33].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[33].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[33].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[33].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[33].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[33].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[33].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 데미지 경감");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("자신이 브레인 필드를 전개하고 있을 때 받는 데미지가");
        m_strExplain2 = TEXT("경감된다. 같은 이름의 스킬이 2개 있으며");
        m_strExplain3 = TEXT("1개 습득하면 데미지가 반감,");
        m_strExplain4 = TEXT("2개 습득하면 데미지가 무효화된다.");

        if (true == m_bSkillOnOff[33])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[33])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[33] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(33))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[33])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[33] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(33, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[33] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[33] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[33] = 0;
    }

    if (m_vPosition_SkillIcons[34].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[34].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[34].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[34].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[34].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[34].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[34].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[34].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 오버킬");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("브레인 필드 전개 시, 크러시 게이지가 다 깎인");
        m_strExplain2 = TEXT("적에게 공격을 명중시키면 EXP 보너스가 증가한다.");
        m_strExplain3 = TEXT("단, 브레인 크러시로밖에 쓰러뜨릴 수 없는 강적의 경우");
        m_strExplain4 = TEXT("체력을 다 깎은 상황에서는 효과가 없다.");

        if (true == m_bSkillOnOff[34])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[34])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[34] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(34))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[34])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[34] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(34, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[34] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[34] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[34] = 0;
    }

    if (m_vPosition_SkillIcons[35].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[35].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[35].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[35].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[35].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[35].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[35].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[35].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 타격 레벨 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("자신의 브레인 필드를 전개하고 있을 때");
        m_strExplain2 = TEXT("[왼쪽 마우스] 로 휘두르는 오브젝트가 증가하여");
        m_strExplain3 = TEXT("데미지를 줄 수 있는 범위가 넓어진다.");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[35])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[35])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[35] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(35))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[35])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[35] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(35, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[35] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[35] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[35] = 0;
    }

    if (m_vPosition_SkillIcons[36].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[36].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[36].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[36].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[36].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[36].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[36].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[36].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 드라이브 중 스톡 소비 없음");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("브레인 필드의 준비 아이콘이 표시된 상태에서");
        m_strExplain2 = TEXT("드라이브를 발동했을 경우, 드라이브 중 스톡을");
        m_strExplain3 = TEXT("소비하지 않고 브레인 필드를 전개할 수 있다.");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[36])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[36])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[36] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(36))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[36])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[36] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(36, true);
                    m_fSkillOpen_Gauge = 0.f;
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[36] = true;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[36] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[36] = 0;
    }

    if (m_vPosition_SkillIcons[37].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[37].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[37].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[37].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[37].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[37].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[37].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[37].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 발사 레벨 상승 1");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("자신이 브레인 필드를 전개하고 있을 때,");
        m_strExplain2 = TEXT("[왼쪽 마우스2][오른쪽 마우스]나");
        m_strExplain3 = TEXT("[왼쪽 마우스3][오른쪽 마우스]의 [오른쪽 마우스]로");
        m_strExplain4 = TEXT("날리는 오브젝트가 증가한다.");

        if (true == m_bSkillOnOff[37])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[37])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[37] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(37))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[37])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[37] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(37, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[37] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[37] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[37] = 0;
    }

    if (m_vPosition_SkillIcons[38].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[38].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[38].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[38].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[38].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[38].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[38].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[38].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 발사 레벨 상승 2");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("자신이 브레인 필드를 전개하고 있을 때");
        m_strExplain2 = TEXT("[왼쪽 마우스3][오른쪽 마우스]의");
        m_strExplain3 = TEXT("[오른쪽 마우스]로 날리는 오브젝트의 수가 증가한다.");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[38])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[38])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[38] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(38))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[38])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[38] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(38, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[38] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[38] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[38] = 0;
    }


    if (m_vPosition_SkillIcons[39].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[39].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[39].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[39].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[39].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[39].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[39].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[39].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 시간 연장 2");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("브레인 필드 전개 시간을 5초 연장한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[39])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[39])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[39] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(39))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[39])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[39] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(39, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[39] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[39] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[39] = 0;
    }

    if (m_vPosition_SkillIcons[40].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[40].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[40].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[40].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[40].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[40].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[40].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[40].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("브레인 필드: 스테이터스 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("브레인 필드 전개 시간동안 ");
        m_strExplain2 = TEXT("주는 데미지가 +10% 상승한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[40])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[40])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[40] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(40))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[40])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[40] = 1;
                }



                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(40, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[40] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[40] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {


        m_iCursorSoundOnce[40] = 0;
    }

    if (m_vPosition_SkillIcons[41].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[41].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[41].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[41].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[41].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[41].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[41].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[41].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브 게이지 축적량 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브 게이지 축적량 +20%");
        m_strExplain2 = TEXT("적을 쓰러뜨리거나 적에게 데미지를 받는 등");
        m_strExplain3 = TEXT("특정 행동으로 인한 축적량이 증가하여");
        m_strExplain4 = TEXT("드라이브 게이지가 쌓이기 쉬워진다.");

        if (true == m_bSkillOnOff[41])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[41])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[41] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(41))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[41])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[41] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(41, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[41] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[41] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {


        m_iCursorSoundOnce[41] = 0;
    }

    if (m_vPosition_SkillIcons[42].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[42].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[42].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[42].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[42].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[42].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[42].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[42].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브: 체력 자동 회복");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브 중 체력이 서서히 회복된다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[42])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[42])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[42] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(42))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[42])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[42] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(42, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bSkillOnOff[42] = true;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[42] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[42] = 0;
    }

    if (m_vPosition_SkillIcons[43].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[43].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[43].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[43].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[43].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[43].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[43].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[43].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브: 연속 어썰트 비전");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브 중 [어썰트 비전] 이");
        m_strExplain2 = TEXT("연속으로 발생하게 된다.");
        m_strExplain3 = TEXT("[어썰트 비전]은 유대 레벨이 상승하면 발생");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[43])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[43])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[43] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(43))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[43])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[43] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(43, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bSkillOnOff[43] = true;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[43] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[43] = 0;
    }

    if (m_vPosition_SkillIcons[44].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[44].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[44].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[44].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[44].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[44].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[44].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[44].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브: SAS 게이지 회복");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브를 발동하면");
        m_strExplain2 = TEXT("모든 SAS 게이지가 모두 회복된다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[44])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[44])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[44] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(44))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[44])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[44] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(44, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[44] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[44] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[44] = 0;
    }

    if (m_vPosition_SkillIcons[45].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[45].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[45].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[45].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[45].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[45].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[45].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[45].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브: 드라이브 게이지 회복 2");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브 중 브레인 크러시로 적을 쓰러뜨리면");
        m_strExplain2 = TEXT("드라이브 게이지가 조금 회복한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[45])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[45])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[45] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(45))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[45])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[45] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(45, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bSkillOnOff[45] = true;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[45] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[45] = 0;
    }


    if (m_vPosition_SkillIcons[46].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[46].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[46].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[46].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[46].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[46].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[46].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[46].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브: 보너스 배율 증가량 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브 중 획득 EXP의 보너스 배율의");
        m_strExplain2 = TEXT("증가량이 상승한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[46])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[46])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[46] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(46))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[46])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[46] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(46, true);
                    m_fSkillOpen_Gauge = 0.f;
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[46] = true;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[46] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[46] = 0;
    }


    if (m_vPosition_SkillIcons[47].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[47].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[47].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[47].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[47].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[47].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[47].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[47].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브: 스테이터스 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브 중 초뇌능력, 공격력, 방어력 + 10%");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[47])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[47])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[47] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(47))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[47])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[47] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(47, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[47] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[47] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[47] = 0;
    }


    if (m_vPosition_SkillIcons[48].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[48].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[48].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[48].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[48].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[48].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[48].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[48].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브: 상태이상 회복");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브를 발동하면 상태이상을 회복한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[48])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[48])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[48] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(48))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[48])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[48] = 1;
                }



                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(48, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bSkillOnOff[48] = true;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[48] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[48] = 0;
    }

    if (m_vPosition_SkillIcons[49].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[49].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[49].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[49].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[49].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[49].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[49].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[49].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("드라이브: 드라이브 게이지 회복 1");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("드라이브 중 브레인 크러시로 적을 쓰러뜨리면");
        m_strExplain2 = TEXT("드라이브 게이지가 조금 회복한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[49])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[49])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[49] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(49))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[49])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[49] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(49, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[49] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[49] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {


        m_iCursorSoundOnce[49] = 0;
    }

    if (m_vPosition_SkillIcons[50].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[50].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[50].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[50].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[50].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[50].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[50].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[50].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("초뇌능력 상승 1");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("초뇌능력 +5%");
        m_strExplain2 = TEXT("염력 공격으로 주는 데미지가 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[50])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[50])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[50] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(50))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[50])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[50] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(50, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[50] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[50] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {


        m_iCursorSoundOnce[50] = 0;
    }

    if (m_vPosition_SkillIcons[51].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[51].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[51].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[51].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[51].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[51].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[51].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[51].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("염력 게이지 회복량 상승(공중 공격)");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("공중 공격으로 인한 염력 게이지의 회복량이 증가한다.");
        m_strExplain2 = TEXT("올려베기 공격은 포함되지 않는다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[51])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[51])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[51] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(51))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[51])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[51] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(51, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[51] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[51] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[51] = 0;
    }

    if (m_vPosition_SkillIcons[52].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[52].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[52].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[52].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[52].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[52].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[52].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[52].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("염력 게이지 최대치 증가");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("같은 이름의 스킬이 2개 있으며, 1개를 습득하면 50%,");
        m_strExplain2 = TEXT("2개를 습득하면 100% 염력 게이지의 최대치가 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[52])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[52])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[52] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(52))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[52])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[52] = 1;
                }



                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(52, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[52] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[52] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[52] = 0;
    }

    if (m_vPosition_SkillIcons[53].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[53].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[53].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[53].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[53].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[53].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[53].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[53].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("무기 공격 크러시 효과 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("무기 공격으로 크러시 게이지를 많이 깎을 수 있다.");
        m_strExplain2 = TEXT("크러시 효과가 +5% 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[53])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[53])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[53] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(53))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[53])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[53] = 1;
                }



                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(53, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[53] = true;
                    m_bBlackFillter = true;
                    m_fSkillOpen_Gauge = 0.f;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[53] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[53] = 0;
    }

    if (m_vPosition_SkillIcons[54].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[54].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[54].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[54].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[54].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[54].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[54].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[54].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("초뇌능력 상승 2");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("초뇌능력 +5%");
        m_strExplain2 = TEXT("염력 공격으로 주는 데미지가 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[54])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[54])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[54] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(54))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[54])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[54] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(54, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bSkillOnOff[54] = true;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[54] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[54] = 0;
    }

    if (m_vPosition_SkillIcons[55].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[55].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[55].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[55].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[55].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[55].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[55].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[55].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("소생 속도 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("동료를 소생하는 속도가 상승한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[55])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[55])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[55] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(55))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[55])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[55] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(55, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[55] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[55] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[55] = 0;
    }

    if (m_vPosition_SkillIcons[56].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[56].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[56].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[56].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[56].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[56].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[56].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[56].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("공격력 상승");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("공격력 +5%");
        m_strExplain2 = TEXT("무기 공격으로 주는 데미지가 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[56])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[56])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[56] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(56))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[56])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[56] = 1;
                }
                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(56, true);
                    m_fSkillOpen_Gauge = 0.f;
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[56] = true;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[56] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[56] = 0;
    }

    if (m_vPosition_SkillIcons[57].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[57].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[57].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[57].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[57].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[57].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[57].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[57].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("후속타");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("다운 중인 적에게 주는 데미지가 증가한다.");
        m_strExplain2 = TEXT("");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[57])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[57])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[57] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(57))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[57])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[57] = 1;
                }
                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(57, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[57] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[57] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[57] = 0;
    }


    if (m_vPosition_SkillIcons[58].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[58].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[58].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[58].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[58].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[58].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[58].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[58].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("염력 게이지 최대치 증가");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("같은 이름의 스킬이 2개 있으며, 1개를 습득하면 50%,");
        m_strExplain2 = TEXT("2개를 습득하면 100% 염력 게이지의 최대치가 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[58])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[58])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[58] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(58))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[58])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[58] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(58, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[58] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[58] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {
        m_iCursorSoundOnce[58] = 0;
    }

    if (m_vPosition_SkillIcons[59].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[59].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[59].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[59].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[59].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[59].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[59].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[59].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("염력 공격 크러시 효과 상승 1");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("염력 공격으로 크러시 게이지를 많이 깎을 수 있다.");
        m_strExplain2 = TEXT("크러시 효과가 5% 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[59])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[59])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[59] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(59))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[59])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[59] = 1;
                }


                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(59, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[59] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;
                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[59] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[59] = 0;

    }
    if (m_vPosition_SkillIcons[60].x - m_vScale_SkillIcons.x * 0.5f <= m_ptMousePos.x     // Left
        && m_vPosition_SkillIcons[60].x + m_vScale_SkillIcons.x * 0.5f >= m_ptMousePos.x  // Right
        && m_vPosition_SkillIcons[60].y + m_vScale_SkillIcons.y * 0.5f >= m_ptMousePos.y  // Top
        && m_vPosition_SkillIcons[60].y - m_vScale_SkillIcons.y * 0.5f <= m_ptMousePos.y) // Bottom
    {
        m_vPosition_MouseIndicator.x = m_vPosition_SkillIcons[60].x;
        m_vPosition_MouseIndicator.y = m_vPosition_SkillIcons[60].y;
        m_pMouseIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_MouseIndicator);

        m_vPosition_SkillOpen_Gauge.x = m_vPosition_SkillIcons[60].x;
        m_vPosition_SkillOpen_Gauge.y = m_vPosition_SkillIcons[60].y;
        m_pSkillOpen_Gauge->Set_State(TRANSFORM::POSITION, m_vPosition_SkillOpen_Gauge);

        m_strGetSkillName = TEXT("염력 공격 크러시 효과 상승 2");
        m_strExplainTitle = m_strGetSkillName;
        m_strExplain1 = TEXT("염력 공격으로 크러시 게이지를 많이 깎을 수 있다.");
        m_strExplain2 = TEXT("크러시 효과가 5% 증가한다.");
        m_strExplain3 = TEXT("");
        m_strExplain4 = TEXT("");

        if (true == m_bSkillOnOff[60])
        {
            m_strGetorNot = TEXT("습득 완료");
        }
        else
        {
            m_strGetorNot = TEXT("");
        }

        if (0 == m_iCursorSoundOnce[60])
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Select.wav", 1.f);
            m_iCursorSoundOnce[60] = 1;
        }

        if (false == CUI_Manager::Get_Instance()->Get_SkillIconBecomeActivated(60))
        {
            if (CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = true;

                m_fSkillOpen_Gauge += fTimeDelta;

                if (0 == m_iClickSoundOnce[60])
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Gauge.wav", 1.f);
                    m_iClickSoundOnce[60] = 1;
                }

                if (1.f <= m_fSkillOpen_Gauge)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_BrainMap_Activate.wav", 1.f);
                    CUI_Manager::Get_Instance()->Set_SkillIconBecomeActivated(60, true);
                    m_fSkillOpen_Gauge = 1.f;
                    m_bSkillOnOff[60] = true;
                    m_fSkillOpen_Gauge = 0.f;
                    m_bBlackFillter = true;

                }
            }
            else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
            {
                m_bSkillOpen_Gauge = false;
                m_iClickSoundOnce[60] = 0;

                m_fSkillOpen_Gauge = 0.f;
            }
        }
    }
    else
    {

        m_iCursorSoundOnce[60] = 0;

    }
}

void CUI_Menu_Category_BrainMap::Set_ConnectorBlink(_float fTimeDelta)
{
    m_fConnectorBlink += fTimeDelta * 2.f;

    m_iConnectorBlinkIndex = (_uint)m_fConnectorBlink % 2;
}

void CUI_Menu_Category_BrainMap::Set_BlackFillterClose()
{
    if (true == m_bBlackFillter)
    {
        if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
        {
            m_bBlackFillter = false;
        }
    }
}

shared_ptr<CUI_Menu_Category_BrainMap> CUI_Menu_Category_BrainMap::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Category_BrainMap> pInstance = make_private_shared(CUI_Menu_Category_BrainMap, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Category_BrainMap::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Category_BrainMap::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Category_BrainMap> pInstance = make_private_shared_copy(CUI_Menu_Category_BrainMap, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Category_BrainMap::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
