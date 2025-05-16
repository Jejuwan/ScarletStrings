#include "ClientPCH.h"
#include "UI_Indicator_ToolTip_Tutorial_Tips.h"
#include "GameInstance.h" 
#include "UI_Manager.h"

CUI_Indicator_ToolTip_Tutorial_Tips::CUI_Indicator_ToolTip_Tutorial_Tips(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Tips::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Tips::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Initialize", "Failed to CGameObject::Initialize");
    }

    Initialize_UI();

    
    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Late_Tick(_float fTimeDelta)
{
     __super::Late_Tick(fTimeDelta);
    
    Add_RenderObject(UI_LAYER::LAYER_5);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Tips::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (true == m_bTipRenderStart)
        {
#pragma region Render_Common
            /* Background_Frame */
            if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(37)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
            }
            /* Tips_Tag */
            if (FAILED(m_pTexture_Tips_Tag->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pTips_Tag->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
            }
            /* TIPS */
            if (FAILED(m_pTexture_Tips->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pTips->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(48)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
            }
#pragma endregion
#pragma region Render_Tips - Tutorial
            if (true == m_bTip_Jump)
            {
                /* Key_SpaceBar */
                if (FAILED(m_pTexture_Key_SpaceBar->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_SpaceBar->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1,   _float2(75.f, 300.f),  XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1_1, _float2(170.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
            }

            if (true == m_bTip_PsycoKinesis)
            {
                /* Key_RButton */
                if (FAILED(m_pTexture_Key_RButton->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_RButton->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(115.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(75.f, 360.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }

            if (true == m_bTip_BasicAttack)
            {
                /* Key_LButton */
                if (FAILED(m_pTexture_Key_LButton->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_LButton->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(115.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }

            if (true == m_bTip_SomerSaultKick)
            {
                /* Key_F */
                if (FAILED(m_pTexture_Key_F->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_F->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(115.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }

            if (true == m_bTip_SpecialAttack)
            {
                /* Key_F */
                if (FAILED(m_pTexture_Key_C->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_C->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(115.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(75.f, 360.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }
#pragma endregion
#pragma region Render_Tips - Mizuha
            if (true == m_bTip_SystemLog)
            {
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(75.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(75.f, 360.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
            }
            if (true == m_bTip_SAS_Electric)
            {
                /* Key_2 */
                if (FAILED(m_pTexture_Key_2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(115.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(75.f, 360.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }

            if (true == m_bTip_SAS_Acceleration)
            {
                /* Key_1 */
                if (FAILED(m_pTexture_Key_1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(115.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(75.f, 360.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }

            if (true == m_bTip_SAS_Clone)
            {
                /* Key_4 */
                if (FAILED(m_pTexture_Key_4->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_4->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(115.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(75.f, 360.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }

            if (true == m_bTip_SAS_Stealth)
            {
                /* Key_3 */
                if (FAILED(m_pTexture_Key_3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_3->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                /* Key_Z */
                if (FAILED(m_pTexture_Key_Z->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_Z->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(115.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(75.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(115.f, 360.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }

            if (true == m_bTip_Gimmick_DumpTruck)
            {
                /* Key_G */
                if (FAILED(m_pTexture_Key_G->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pKey_G->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTipsBackgroundTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(65)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation1, _float2(75.f, 300.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation2, _float2(115.f, 330.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strExplanation3, _float2(75.f, 360.f), XMVectorSet(m_fSeperateLineTime, m_fSeperateLineTime, m_fSeperateLineTime, m_fTipsBackgroundTime), 0.475f);

            }
#pragma endregion
        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Tips::Ready_Components()
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

void CUI_Indicator_ToolTip_Tutorial_Tips::Initialize_UI()
{
//=========================================================================== _bool
    m_bTipRenderStart                   = false;
    m_bTipRenderEnd                     = false;

    m_bScale_Background                 = false;
    m_bScale_Tips_Tag                   = false;
    m_bScale_Tips                       = false;

    m_bTip_Jump                         = false;
    m_bTip_PsycoKinesis                 = false;
    m_bTip_BasicAttack                  = false;
    m_bTip_SomerSaultKick               = false;
    m_bTip_SpecialAttack                = false;
    m_bTip_SystemLog                    = false;
    m_bTip_SAS_Electric                 = false;
    m_bTip_SAS_Acceleration             = false;
    m_bTip_SAS_Clone                    = false;
    m_bTip_SAS_Stealth                  = false;
    m_bTip_Gimmick_DumpTruck            = false;

    m_bSoundStart                       = false;
    m_bSoundEnd                         = false;

//=========================================================================== _uint
    m_iRenderPhase                      = 0;
    
//=========================================================================== _float
    m_fTipsBackgroundTime               = 0.f;
    m_fRunningTime                      = 0.f;
    m_fSeperateLineTime                 = 0.f;

//=========================================================================== Transform
    m_pBackground                       = CTransform::Create(m_pDevice, m_pContext);
    m_pTips_Tag                         = CTransform::Create(m_pDevice, m_pContext);
    m_pTips                             = CTransform::Create(m_pDevice, m_pContext);

    m_pKey_SpaceBar                     = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_RButton                      = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_LButton                      = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_C                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_F                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_G                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_Z                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_1                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_2                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_3                            = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_4                            = CTransform::Create(m_pDevice, m_pContext);
   
//=========================================================================== Texture
    m_pTexture_Background               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Tips_background.png"));
    m_pTexture_Tips_Tag                 = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Tips_Tag.png"));
    m_pTexture_Tips                     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/TIPS.png"));

    m_pTexture_Key_SpaceBar             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/SpaceBar.png"));
    m_pTexture_Key_RButton              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/VK_RButton.png"));
    m_pTexture_Key_LButton              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/VK_LButton.png"));
    m_pTexture_Key_C                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/C.png"));
    m_pTexture_Key_F                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/F.png"));
    m_pTexture_Key_G                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/G.png"));
    m_pTexture_Key_Z                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/Z.png"));
    m_pTexture_Key_1                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/1.png"));
    m_pTexture_Key_2                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/2.png"));
    m_pTexture_Key_3                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/3.png"));
    m_pTexture_Key_4                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/KeyBoard/4.png"));


//=========================================================================== _float3
    m_vScale_Background                 = _float3(450.f, 190.f, 0.f);
    m_vScale_Tips_Tag                   = _float3(170.f, 40.f, 0.f);
    m_vScale_Tips                       = _float3(60.f, 15.f, 0.f);

    m_vScale_Key_SpaceBar               = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_RButton                = _float3(22.f, 29.f, 0.f);
    m_vScale_Key_LButton                = _float3(22.f, 29.f, 0.f);
    m_vScale_Key_C                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_F                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_G                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_Z                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_1                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_2                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_3                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_4                      = _float3(28.f, 28.f, 0.f);


//=========================================================================== _float4
    m_vPosition_Background              = _float4(-700.f, 220.f, 0.f, 1.f);
    m_vPosition_Tips_Tag                = _float4(-810.f, 280.f, 0.f, 1.f);
    m_vPosition_Tips                    = _float4(-810.f, 280.f, 0.f, 1.f);

    m_vPosition_Key_SpaceBar            = _float4(-805.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_RButton             = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_LButton             = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_C                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_F                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_G                   = _float4(-865.f, 200.f, 0.f, 1.f);
    m_vPosition_Key_Z                   = _float4(-865.f, 170.f, 0.f, 1.f);
    m_vPosition_Key_1                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_2                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_3                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_4                   = _float4(-865.f, 230.f, 0.f, 1.f);


//=========================================================================== Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pTips_Tag->Set_Scale(m_vScale_Tips_Tag);
    m_pTips->Set_Scale(m_vScale_Tips);

    m_pKey_SpaceBar->Set_Scale(m_vScale_Key_SpaceBar);
    m_pKey_RButton->Set_Scale(m_vScale_Key_RButton);
    m_pKey_LButton->Set_Scale(m_vScale_Key_LButton);
    m_pKey_C->Set_Scale(m_vScale_Key_C);
    m_pKey_F->Set_Scale(m_vScale_Key_F);
    m_pKey_G->Set_Scale(m_vScale_Key_G);
    m_pKey_Z->Set_Scale(m_vScale_Key_Z);
    m_pKey_1->Set_Scale(m_vScale_Key_1);
    m_pKey_2->Set_Scale(m_vScale_Key_2);
    m_pKey_3->Set_Scale(m_vScale_Key_3);
    m_pKey_4->Set_Scale(m_vScale_Key_4);

//=========================================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pTips_Tag->Set_State(TRANSFORM::POSITION, m_vPosition_Tips_Tag);
    m_pTips->Set_State(TRANSFORM::POSITION, m_vPosition_Tips);
    
    m_pKey_SpaceBar->Set_State(TRANSFORM::POSITION, m_vPosition_Key_SpaceBar);
    m_pKey_RButton->Set_State(TRANSFORM::POSITION, m_vPosition_Key_RButton);
    m_pKey_LButton->Set_State(TRANSFORM::POSITION, m_vPosition_Key_LButton);
    m_pKey_C->Set_State(TRANSFORM::POSITION, m_vPosition_Key_C);
    m_pKey_F->Set_State(TRANSFORM::POSITION, m_vPosition_Key_F);
    m_pKey_G->Set_State(TRANSFORM::POSITION, m_vPosition_Key_G);
    m_pKey_Z->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Z);
    m_pKey_1->Set_State(TRANSFORM::POSITION, m_vPosition_Key_1);
    m_pKey_2->Set_State(TRANSFORM::POSITION, m_vPosition_Key_2);
    m_pKey_3->Set_State(TRANSFORM::POSITION, m_vPosition_Key_3);
    m_pKey_4->Set_State(TRANSFORM::POSITION, m_vPosition_Key_4);

//=========================================================================== wstring(Explanation)
    m_strExplanation1   = L"";
    m_strExplanation1_1 = L"";
    m_strExplanation2   = L"";
    m_strExplanation3   = L"";

}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tick_UI(_float fTimeDelta)
{
    //TestKey();


    Render_Tips_System(fTimeDelta);

    // Tutorial
    Tip_Jump();
    Tip_Psycokinesis();
    Tip_BasicAttack();
    Tip_SomerSaultKick();
    Tip_SpecialAttack();

    // Mizuha
    Tip_SystemLog();
    Tip_SAS_Electric();
    Tip_SAS_Acceleration();
    Tip_SAS_Clone();
    Tip_SAS_Stealth();
    Tip_Gimmick_DumpTruck();
}

void CUI_Indicator_ToolTip_Tutorial_Tips::TestKey()
{
    if (CGameInstance::Get_Instance()->Key_Down(VK_F6))
    {
        m_bTipRenderStart       = true;
        m_bTip_Gimmick_DumpTruck   = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Render_Tips_System(_float fTimeDelta)
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
#pragma region Set_Scale - Appear
        if (true == m_bTipRenderStart && false == m_bTipRenderEnd)
        {
            m_fTipsBackgroundTime += fTimeDelta * 2.f;

            if (0.9f <= m_fTipsBackgroundTime)
            {
                m_fTipsBackgroundTime = 0.9f;
            }

            // Running Time = Life Time
            if (3.f <= m_fRunningTime)
            {
                m_bTipRenderEnd = true;

            }
            else
            {
                m_fRunningTime += fTimeDelta;
            }

            // For. RenderFont - Alpha
            if (0.5f <= m_fSeperateLineTime)
            {
                m_fSeperateLineTime = 0.5f;
            }
            else
            {
                m_fSeperateLineTime += fTimeDelta;
            }

            // Set_Scale - Background
            if (430.f >= m_vScale_Background.x)
            {
                m_vScale_Background.x = 430.f;
                m_pBackground->Set_Scale(m_vScale_Background);
            }
            else
            {
                m_vScale_Background.x -= fTimeDelta * 100.f;
                m_pBackground->Set_Scale(m_vScale_Background);
            }
            // Set_Scale - Tips_Tag
            if (150.f >= m_vScale_Tips_Tag.x)
            {
                m_vScale_Tips_Tag.x = 150.f;
                m_pTips_Tag->Set_Scale(m_vScale_Tips_Tag);

            }
            else
            {
                m_vScale_Tips_Tag.x -= fTimeDelta * 100.f;
                m_pTips_Tag->Set_Scale(m_vScale_Tips_Tag);
            }
            // Set_Scale - Tips
            if (40.f >= m_vScale_Tips.x)
            {
                m_vScale_Tips.x = 40.f;
                m_pTips->Set_Scale(m_vScale_Tips);
            }
            else
            {
                m_vScale_Tips.x -= fTimeDelta * 100.f;
                m_pTips->Set_Scale(m_vScale_Tips);
            }
        }
#pragma endregion

#pragma region Set_Scale - Remove
        if (true == m_bTipRenderStart && true == m_bTipRenderEnd)
        {
#pragma region Set_Scale
            if (0 == m_iRenderPhase)
            {
                // Set_Scale - Background
                if (440.f <= m_vScale_Background.x)
                {
                    m_vScale_Background.x = 440.f;
                    m_pBackground->Set_Scale(m_vScale_Background);
                    m_bScale_Background = true;                     // Trigger
                }
                else
                {
                    m_vScale_Background.x += fTimeDelta * 300.f;
                    m_pBackground->Set_Scale(m_vScale_Background);
                }
                // Set_Scale - Tips_Tag
                if (160.f <= m_vScale_Tips_Tag.x)
                {
                    m_vScale_Tips_Tag.x = 160.f;
                    m_pTips_Tag->Set_Scale(m_vScale_Tips_Tag);
                    m_bScale_Tips_Tag = true;
                }
                else
                {
                    m_vScale_Tips_Tag.x += fTimeDelta * 300.f;
                    m_pTips_Tag->Set_Scale(m_vScale_Tips_Tag);
                }
                // Set_Scale - Tips
                if (60.f <= m_vScale_Tips.x)
                {
                    m_vScale_Tips.x = 60.f;
                    m_pTips->Set_Scale(m_vScale_Tips);
                    m_bScale_Tips = true;
                }
                else
                {
                    m_vScale_Tips.x += fTimeDelta * 300.f;
                    m_pTips->Set_Scale(m_vScale_Tips);
                }

                if (true == m_bScale_Background && true == m_bScale_Tips_Tag && true == m_bScale_Tips)
                {
                    m_iRenderPhase += 1;
                }
            }
            if (1 == m_iRenderPhase)
            {
                // Set_Scale - Background
                if (430.f >= m_vScale_Background.x)
                {
                    m_vScale_Background.x = 430.f;
                    m_pBackground->Set_Scale(m_vScale_Background);
                    m_bScale_Background = false;                     // Trigger
                }
                else
                {
                    m_vScale_Background.x -= fTimeDelta * 400.f;
                    m_pBackground->Set_Scale(m_vScale_Background);
                }
                // Set_Scale - Tips_Tag
                if (150.f >= m_vScale_Tips_Tag.x)
                {
                    m_vScale_Tips_Tag.x = 150.f;
                    m_pTips_Tag->Set_Scale(m_vScale_Tips_Tag);
                    m_bScale_Tips_Tag = false;
                }
                else
                {
                    m_vScale_Tips_Tag.x -= fTimeDelta * 400.f;
                    m_pTips_Tag->Set_Scale(m_vScale_Tips_Tag);
                }
                // Set_Scale - Tips
                if (40.f >= m_vScale_Tips.x)
                {
                    m_vScale_Tips.x = 40.f;
                    m_pTips->Set_Scale(m_vScale_Tips);
                    m_bScale_Tips = false;
                }
                else
                {
                    m_vScale_Tips.x -= fTimeDelta * 400.f;
                    m_pTips->Set_Scale(m_vScale_Tips);
                }

                if (false == m_bScale_Background && false == m_bScale_Tips_Tag && false == m_bScale_Tips)
                {
                    m_iRenderPhase += 1;
                }
            }
#pragma endregion
#pragma region Set_Alpha
            if (2 == m_iRenderPhase)
            {
                m_fTipsBackgroundTime -= fTimeDelta * 3.f;

                if (0.f >= m_fSeperateLineTime)
                {
                    m_fSeperateLineTime = 0.f;
                }
                else
                {
                    m_fSeperateLineTime -= fTimeDelta * 3.f;
                }

                if (0.f >= m_fTipsBackgroundTime)
                {

                    if (false == m_bSoundEnd)
                    {
                        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
                        m_bSoundEnd = true;
                    }

                    Reset();
                }
            }
#pragma endregion
        }
#pragma endregion
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_Jump()
{
    if (true == m_bTip_Jump)
    {
        m_strExplanation1   = L"단차는";
        m_strExplanation1_1 = L"로 뛰어넘을 수 있다.";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_Psycokinesis()
{
    if (true == m_bTip_PsycoKinesis)
    {
        m_strExplanation1 = L"길게 누르기로 염력 공격.";
        m_strExplanation2 = L"염력 게이지를 소비하여";
        m_strExplanation3 = L"주위의 물체를 조작해 공격한다.";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_BasicAttack()
{
    if (true == m_bTip_BasicAttack)
    {
        m_strExplanation1 = L"로 무기 공격.";
        m_strExplanation2 = L"무기 공격으로 염력 게이지가 조금 회복.";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_SomerSaultKick()
{
    if (true == m_bTip_SomerSaultKick)
    {
        m_strExplanation1 = L"로 올려베기 공격을 해서.";
        m_strExplanation2 = L"적을 다운시켜 쓰러뜨린다.";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_SpecialAttack()
{
    if (true == m_bTip_SpecialAttack)
    {
        m_strExplanation1 = L"로 특수 공격.";
        m_strExplanation2 = L"적에게 명중하면";
        m_strExplanation3 = L"염력 게이지가 많이 회복된다."; 
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_SystemLog()
{
    if (true == m_bTip_SystemLog)
    {
        m_strExplanation1 = L"적을 쓰러뜨리면 화면 우측에";
        m_strExplanation2 = L"시스템 로그가 표시된다.";
        m_strExplanation3 = L"획득한 EXP와 아이템 등을 확인할 수 있다.";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_SAS_Electric()
{
    if (true == m_bTip_SAS_Electric)
    {
        m_strExplanation1 = L"로 감전 효과를 부여한다.";
        m_strExplanation2 = L"";
        m_strExplanation3 = L"";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_SAS_Acceleration()
{
    if (true == m_bTip_SAS_Acceleration)
    {
        m_strExplanation1 = L"로 초고속 효과를 부여한다.";
        m_strExplanation2 = L"시간의 흐름이 느려진다.";
        m_strExplanation3 = L"";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_SAS_Clone()
{
    if (true == m_bTip_SAS_Clone)
    {
        m_strExplanation1 = L"로 복제 효과를 부여한다.";
        m_strExplanation2 = L"길게 누르기로 염력 공격을 하면";
        m_strExplanation3 = L"오브젝트를 복제해서 연속 발사한다.";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_SAS_Stealth()
{
    if (true == m_bTip_SAS_Stealth)
    {
        m_strExplanation1 = L"로 투명화 효과를 부여한다.";
        m_strExplanation2 = L"적에게 존재가 감지되지 않게 된다.";
        m_strExplanation3 = L"길게 누르기로 차지공격이 발동한다.";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Tip_Gimmick_DumpTruck()
{
    if (true == m_bTip_Gimmick_DumpTruck)
    {
        m_strExplanation1 = L"위쪽의 특수 오브젝트 [덤프트럭]을";
        m_strExplanation2 = L"로 떨어뜨려서 괴이에 부딪친다.";
        m_strExplanation3 = L"타이밍을 잘 맞춰야 한다.";
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Reset()
{
//=========================================================================== _bool
    m_bTipRenderStart                   = false;
    m_bTipRenderEnd                     = false;

    m_bScale_Background                 = false;
    m_bScale_Tips_Tag                   = false;
    m_bScale_Tips                       = false;

    m_bTip_Jump                         = false;
    m_bTip_PsycoKinesis                 = false;
    m_bTip_BasicAttack                  = false;
    m_bTip_SomerSaultKick               = false;
    m_bTip_SpecialAttack                = false;
    m_bTip_SystemLog                    = false;
    m_bTip_SAS_Electric                 = false;
    m_bTip_SAS_Acceleration             = false;
    m_bTip_SAS_Clone                    = false;
    m_bTip_SAS_Stealth                  = false;
    m_bTip_Gimmick_DumpTruck            = false;

    m_bSoundEnd = false;
    m_bSoundStart = false;

//=========================================================================== _uint
    m_iRenderPhase                      = 0;
    
//=========================================================================== _float
    m_fTipsBackgroundTime               = 0.f;
    m_fRunningTime                      = 0.f;
    m_fSeperateLineTime                 = 0.f;

//=========================================================================== _float3
    m_vScale_Background                 = _float3(450.f, 190.f, 0.f);
    m_vScale_Tips_Tag                   = _float3(170.f, 40.f, 0.f);
    m_vScale_Tips                       = _float3(60.f, 15.f, 0.f);

    m_vScale_Key_SpaceBar               = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_RButton                = _float3(22.f, 29.f, 0.f);
    m_vScale_Key_LButton                = _float3(22.f, 29.f, 0.f);
    m_vScale_Key_C                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_F                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_G                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_Z                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_1                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_2                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_3                      = _float3(28.f, 28.f, 0.f);
    m_vScale_Key_4                      = _float3(28.f, 28.f, 0.f);


//=========================================================================== _float4
    m_vPosition_Background              = _float4(-700.f, 220.f, 0.f, 1.f);
    m_vPosition_Tips_Tag                = _float4(-810.f, 280.f, 0.f, 1.f);
    m_vPosition_Tips                    = _float4(-810.f, 280.f, 0.f, 1.f);

    m_vPosition_Key_SpaceBar            = _float4(-805.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_RButton             = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_LButton             = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_C                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_F                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_G                   = _float4(-865.f, 200.f, 0.f, 1.f);
    m_vPosition_Key_Z                   = _float4(-865.f, 170.f, 0.f, 1.f);
    m_vPosition_Key_1                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_2                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_3                   = _float4(-865.f, 230.f, 0.f, 1.f);
    m_vPosition_Key_4                   = _float4(-865.f, 230.f, 0.f, 1.f);


//=========================================================================== Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pTips_Tag->Set_Scale(m_vScale_Tips_Tag);
    m_pTips->Set_Scale(m_vScale_Tips);

    m_pKey_SpaceBar->Set_Scale(m_vScale_Key_SpaceBar);
    m_pKey_RButton->Set_Scale(m_vScale_Key_RButton);
    m_pKey_LButton->Set_Scale(m_vScale_Key_LButton);
    m_pKey_C->Set_Scale(m_vScale_Key_C);
    m_pKey_F->Set_Scale(m_vScale_Key_F);
    m_pKey_G->Set_Scale(m_vScale_Key_G);
    m_pKey_Z->Set_Scale(m_vScale_Key_Z);
    m_pKey_1->Set_Scale(m_vScale_Key_1);
    m_pKey_2->Set_Scale(m_vScale_Key_2);
    m_pKey_3->Set_Scale(m_vScale_Key_3);
    m_pKey_4->Set_Scale(m_vScale_Key_4);

//=========================================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pTips_Tag->Set_State(TRANSFORM::POSITION, m_vPosition_Tips_Tag);
    m_pTips->Set_State(TRANSFORM::POSITION, m_vPosition_Tips);
    
    m_pKey_SpaceBar->Set_State(TRANSFORM::POSITION, m_vPosition_Key_SpaceBar);
    m_pKey_RButton->Set_State(TRANSFORM::POSITION, m_vPosition_Key_RButton);
    m_pKey_LButton->Set_State(TRANSFORM::POSITION, m_vPosition_Key_LButton);
    m_pKey_C->Set_State(TRANSFORM::POSITION, m_vPosition_Key_C);
    m_pKey_F->Set_State(TRANSFORM::POSITION, m_vPosition_Key_F);
    m_pKey_G->Set_State(TRANSFORM::POSITION, m_vPosition_Key_G);
    m_pKey_Z->Set_State(TRANSFORM::POSITION, m_vPosition_Key_Z);
    m_pKey_1->Set_State(TRANSFORM::POSITION, m_vPosition_Key_1);
    m_pKey_2->Set_State(TRANSFORM::POSITION, m_vPosition_Key_2);
    m_pKey_3->Set_State(TRANSFORM::POSITION, m_vPosition_Key_3);
    m_pKey_4->Set_State(TRANSFORM::POSITION, m_vPosition_Key_4);

//=========================================================================== wstring(Explanation)
    m_strExplanation1   = L"";
    m_strExplanation1_1 = L"";
    m_strExplanation2   = L"";
    m_strExplanation3   = L"";

}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_Jump()
{
    m_bTipRenderStart           = true;
    m_bTip_Jump                 = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_Psycokinesis()
{
    m_bTipRenderStart           = true;
    m_bTip_PsycoKinesis         = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_BasicAttack()
{
    m_bTipRenderStart           = true;
    m_bTip_BasicAttack          = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_SomerSaultKick()
{
    m_bTipRenderStart           = true;
    m_bTip_SomerSaultKick       = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_Tip_SpecialAttack()
{
    m_bTipRenderStart           = true;
    m_bTip_SpecialAttack        = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_SystemLog()
{
    m_bTipRenderStart           = true;
    m_bTip_SystemLog            = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_SAS_Electric()
{
    m_bTipRenderStart           = true;
    m_bTip_SAS_Electric         = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_SAS_Clone()
{
    m_bTipRenderStart           = true;
    m_bTip_SAS_Clone            = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_SAS_Stealth()
{
    m_bTipRenderStart           = true;
    m_bTip_SAS_Stealth          = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Tips::Start_Tip_Gimmick_DumpTruck()
{
    m_bTipRenderStart           = true;
    m_bTip_Gimmick_DumpTruck    = true;

    if (false == m_bSoundStart)
    {
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Appear_Tips.wav", 1.f);
        m_bSoundStart = true;
    }
}

shared_ptr<CUI_Indicator_ToolTip_Tutorial_Tips> CUI_Indicator_ToolTip_Tutorial_Tips::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_Tips> pInstance = make_private_shared(CUI_Indicator_ToolTip_Tutorial_Tips, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Tutorial_Tips::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_ToolTip_Tutorial_Tips::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_Tips> pInstance = make_private_shared_copy(CUI_Indicator_ToolTip_Tutorial_Tips, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Tutorial_Tips::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
