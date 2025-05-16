#include "ClientPCH.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "GameInstance.h" 
#include "UI_Manager.h"

CUI_Indicator_BrainTalk_Mizuha::CUI_Indicator_BrainTalk_Mizuha(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_BrainTalk_Mizuha::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_BrainTalk_Mizuha::Initialize(any _aDesc)
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

void CUI_Indicator_BrainTalk_Mizuha::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_BrainTalk_Mizuha::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_5);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_BrainTalk_Mizuha::Render()
{
    if (true == m_bBrainTalk_First || true == m_bBrainTalk_Second || true == m_bBrainTalk_Third || true == m_bBrainTalk_Fourth 
        || true == m_bBrainTalk_Fifth || true == m_bBrainTalk_Sixth || true == m_bBrainTalk_Seventh || true == m_bBrainTalk_Crows
        || true == m_bBrainTalk_BossStage || true == m_bBrainTalk_BossStage_Half || true == m_bBrainTalk_BossStage_End
        || true == m_bBrainTalk_Haruka_Player_Damaged_By_Electric || true == m_bBrainTalk_Haruka_Player_Damaged_By_Water
        || true == m_bBrainTalk_Kasane_LevelUp || true == m_bBrainTalk_Kasane_Get_Item
        || true == m_bBrainTalk_Siden_Monster_Get_Wet || true == m_bBrainTalk_Kagero_Monster_Lost_Us
        || true == m_bBrainTalk_Arasi_Use_My_SAS || true == m_bBrainTalk_Siden_Use_My_SAS || true == m_bBrainTalk_Kyoka_Use_My_SAS || true == m_bBrainTalk_Kagero_Use_My_SAS
        || true == m_bBrainTalk_Siden_Notice_Item || true == m_bBrainTalk_Arasi_Notice_Item || true == m_bBrainTalk_Kyoka_Notice_Item || true == m_bBrainTalk_Kagero_Notice_Item)
    {
        if (true == m_bRender_Noise)
        {
            // BrainTalk_Frame_Noise
            if (FAILED(m_pTexture_BrainTalkFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrainTalkFrame->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fNoiseAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(60)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
            }

            // BrainTalk_Face_Noise
            if (FAILED(m_pTexture_FaceFrame_Noise->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pFaceFrame_Noise->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fNoiseAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(60)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
            }
        }

        if (true == m_bRender_FaceFrame)
        {
            // BrainTalk_FaceFrame
            if (FAILED(m_pTexture_FaceFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pFaceFrame->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFaceFrame, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(57)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
            }
        }

        if (true == m_bRender_BTLFN)
        {
            // BrainTalk_LightFrame_Noise
            if (FAILED(m_pTexture_BrainTalk_LightFrame_Noise->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrainTalk_LightFrame_Noise->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBTLFN, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(59)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
            }
        }
        
        if (true == m_bRender_BTLF)
        {
            // BrainTalk_LightFrame
            if (FAILED(m_pTexture_BrainTalk_LightFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrainTalk_LightFrame->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBTLF, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(61)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
            }
        }

        if (true == m_bRender_Face)
        {
            if (true == m_bRender_Face_Kasane)
            {
                // BrainTalk_Face_Kasane
                if (FAILED(m_pTexture_Face_Kasane->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", m_iFaceIndex_Kasane)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pFace_Kasane->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFace_KasaneAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(61)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
                }
            }
            if (true == m_bRender_Face_Siden)
            {
                // BrainTalk_Face_Siden
                if (FAILED(m_pTexture_Face_Siden->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", m_iFaceIndex_Siden)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pFace_Siden->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFace_SidenAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(61)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
                }
            }
            if (true == m_bRender_Face_Arasi)
            {
                // BrainTalk_Face_Arasi
                if (FAILED(m_pTexture_Face_Arasi->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", m_iFaceIndex_Arasi)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pFace_Arasi->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFace_ArasiAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(61)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
                }
            }
            if (true == m_bRender_Face_Kyoka)
            {
                // BrainTalk_Face_Kyoka
                if (FAILED(m_pTexture_Face_Kyoka->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", m_iFaceIndex_Kyoka)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pFace_Kyoka->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFace_KyokaAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(61)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
                }
            }
            if (true == m_bRender_Face_Kagero)
            {
                // BrainTalk_Face_Kagero
                if (FAILED(m_pTexture_Face_Kagero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", m_iFaceIndex_Kagero)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pFace_Kagero->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFace_KageroAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(61)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
                }
            }
            if (true == m_bRender_Face_Haruka)
            {
                // BrainTalk_Face_Haruka
                if (FAILED(m_pTexture_Face_Haruka->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", m_iFaceIndex_Haruka)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pFace_Haruka->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFace_HarukaAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(61)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
                }
            }
            if (true == m_bRender_Face_Gemma)
            {
                // BrainTalk_Face_Gamma
                if (FAILED(m_pTexture_Face_Gemma->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", m_iFaceIndex_Gemma)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pFace_Gemma->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFace_GemmaAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(61)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
                }
            }
            if (true == m_bRender_Face_Naomi)
            {
                // BrainTalk_Face_Naomi
                if (FAILED(m_pTexture_Face_Naomi->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse", m_iFaceIndex_Naomi)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pFace_Naomi->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFace_NaomiAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(61)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
                }
            }
        }

        if (true == m_bRender_BTL)
        {
            // BrainTalk_Light
            if (FAILED(m_pTexture_BrainTalk_Light->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrainTalk_Light->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc.m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndex", &m_tUIDesc.m_iIndex, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxX", &m_tUIDesc.m_iIndexMaxX, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxY", &m_tUIDesc.m_iIndexMaxY, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fSizeX", &m_tUIDesc.m_fSizeX, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fSizeY", &m_tUIDesc.m_fSizeY, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(4)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
        }

        if (true == m_bRender_BTF)
        {
            // BrainTalk_Frame
            if (FAILED(m_pTexture_BrainTalkFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBrainTalkFrame->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBTF, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(57)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Render", "Failed to CGameObject::Render");
            }

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strText1, _float2(100.f, 680.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.47f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strText2, _float2(100.f, 725.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.47f);

        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_BrainTalk_Mizuha::Ready_Components()
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

void CUI_Indicator_BrainTalk_Mizuha::Initialize_UI()
{
//================================================================================================ Transform
    m_pBrainTalkFrame                       = CTransform::Create(m_pDevice, m_pContext);
    m_pBrainTalkFrame_Noise                 = CTransform::Create(m_pDevice, m_pContext);
    m_pFaceFrame                            = CTransform::Create(m_pDevice, m_pContext);
    m_pFaceFrame_Noise                      = CTransform::Create(m_pDevice, m_pContext);
    m_pBrainTalk_LightFrame                 = CTransform::Create(m_pDevice, m_pContext);
    m_pBrainTalk_LightFrame_Noise           = CTransform::Create(m_pDevice, m_pContext);
    m_pBrainTalk_Light                      = CTransform::Create(m_pDevice, m_pContext);
    m_pFace_Kasane                          = CTransform::Create(m_pDevice, m_pContext);
    m_pFace_Siden                           = CTransform::Create(m_pDevice, m_pContext);
    m_pFace_Arasi                           = CTransform::Create(m_pDevice, m_pContext);
    m_pFace_Kyoka                           = CTransform::Create(m_pDevice, m_pContext);
    m_pFace_Kagero                          = CTransform::Create(m_pDevice, m_pContext);
    m_pFace_Haruka                          = CTransform::Create(m_pDevice, m_pContext);
    m_pFace_Gemma                           = CTransform::Create(m_pDevice, m_pContext);
    m_pFace_Naomi                           = CTransform::Create(m_pDevice, m_pContext);

//================================================================================================ Texture
    m_pTexture_BrainTalkFrame               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_Frame.png"));
    m_pTexture_BrainTalkFrame_Noise         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_FrameNoise.png"));
    m_pTexture_FaceFrame                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/FaceFrame.png"));
    m_pTexture_FaceFrame_Noise              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/FaceFrame_Noise.png"));
    m_pTexture_BrainTalk_LightFrame         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_LightFrame.png"));
    m_pTexture_BrainTalk_LightFrame_Noise   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_LightFrame_Noise.png"));
    m_pTexture_BrainTalk_Light              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_Light.png"));
    m_pTexture_Face_Kasane                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_ch0200_0%d.png"), 3);
    m_pTexture_Face_Siden                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_ch0800_0%d.png"), 3);
    m_pTexture_Face_Arasi                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_ch1000_0%d.png"), 3);
    m_pTexture_Face_Kyoka                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_ch0900_0%d.png"), 4);
    m_pTexture_Face_Kagero                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_ch0700_0%d.png"), 3);
    m_pTexture_Face_Haruka                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_np1300_0%d.png"), 3);
    m_pTexture_Face_Gemma                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_ch0400_0%d.png"), 3);
    m_pTexture_Face_Naomi                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_np1500_0%d.png"), 3);

//================================================================================================ _bool
    m_bBrainTalk_First                      = false;
    m_bBrainTalk_Second                     = false;
    m_bBrainTalk_Third                      = false;
    m_bBrainTalk_Fourth                     = false;
    m_bBrainTalk_Fifth                      = false;
    m_bBrainTalk_Sixth                      = false;
    m_bBrainTalk_Seventh                    = false;
    m_bBrainTalk_BossStage                  = false;
    m_bBrainTalk_BossStage_Half             = false;
    m_bBrainTalk_BossStage_End              = false;
    m_bBrainTalk_Crows                      = false;

    m_bRender_BTLFN                         = true;
    m_bRender_BTLF                          = false;
    m_bRender_Noise                         = false;
    m_bRender_BTF                           = false;
    m_bRender_BTL                           = false;
    m_bRender_Face                          = false;
    m_bRender_Face_Kasane                   = false;
    m_bRender_Face_Siden                    = false;
    m_bRender_Face_Arasi                    = false;
    m_bRender_Face_Kyoka                    = false;
    m_bRender_Face_Kagero                   = false;
    m_bRender_Face_Haruka                   = false;
    m_bRender_Face_Gemma                    = false;
    m_bRender_Face_Naomi                    = false;

    m_bDisappear                            = false;

    // Enum
    m_eBrainTalk_Conversation                       = BT_MAX;
    m_eBrainTalk_Specific_Situation                 = BT_SPEMAX;

    // For. Specific Situation
    m_bBrainTalk_Haruka_Player_Damaged_By_Electric  = false;
    m_bBrainTalk_Haruka_Player_Damaged_By_Water     = false;

    m_bBrainTalk_Kasane_LevelUp                     = false;
    m_bBrainTalk_Kasane_Get_Item                    = false;

    m_bBrainTalk_Arasi_Use_My_SAS                   = false;
    m_bBrainTalk_Siden_Use_My_SAS                   = false;
    m_bBrainTalk_Kyoka_Use_My_SAS                   = false;
    m_bBrainTalk_Kagero_Use_My_SAS                  = false;

    m_bBrainTalk_Siden_Monster_Get_Wet              = false;                                                 
                                                    
    m_bBrainTalk_Kagero_Monster_Lost_Us             = false;                                                  
                                                    
    m_bBrainTalk_Siden_Notice_Item                  = false;
    m_bBrainTalk_Arasi_Notice_Item                  = false;
    m_bBrainTalk_Kyoka_Notice_Item                  = false;
    m_bBrainTalk_Kagero_Notice_Item                 = false;

    m_bIsBrainTalkNow                               = false;
    m_bRender_Kasane_Get_Item                       = false;
//================================================================================================ _uint
    m_iRandomNumber                         = 0;
    m_iCheckSoundIndex                      = 0;

    m_iFaceIndex_Kasane                     = 0;
    m_iFaceIndex_Siden                      = 0;
    m_iFaceIndex_Arasi                      = 0;
    m_iFaceIndex_Kyoka                      = 0;
    m_iFaceIndex_Kagero                     = 0;
    m_iFaceIndex_Haruka                     = 0;
    m_iFaceIndex_Gemma                      = 0;
    m_iFaceIndex_Naomi                      = 0;

//================================================================================================ _float
    m_fBTLFN                                = 0.f;
    m_fBTLF                                 = 0.f;
    m_fNoiseAlpha                           = 0.5f;
    m_fFaceFrame                            = 1.f;
    m_fAccTime                              = 0.f;

    m_fFace_KasaneAlpha                     = 0.f;
    m_fFace_SidenAlpha                      = 0.f;
    m_fFace_ArasiAlpha                      = 0.f;
    m_fFace_KyokaAlpha                      = 0.f;
    m_fFace_KageroAlpha                     = 0.f;
    m_fFace_HarukaAlpha                     = 0.f;
    m_fFace_GemmaAlpha                      = 0.f;
    m_fFace_NaomiAlpha                      = 0.f;

    m_fAccTime_Face_Kasane                  = 0.f;
    m_fAccTime_Face_Siden                   = 0.f;
    m_fAccTime_Face_Arasi                   = 0.f;
    m_fAccTime_Face_Kyoka                   = 0.f;
    m_fAccTime_Face_Kagero                  = 0.f;
    m_fAccTime_Face_Haruka                  = 0.f;
    m_fAccTime_Face_Gemma                   = 0.f;
    m_fAccTime_Face_Naomi                   = 0.f;

    m_fCheckSoundTime                       = 0.f;
    m_fBTF                                  = 0.f;

//================================================================================================ // Texture_Sprite_Desc // BTL
    ZeroMemory(&m_tUIDesc, sizeof m_tUIDesc);

    m_tUIDesc.m_iIndex                      = 0;
    m_tUIDesc.m_iIndexMaxX                  = 1;
    m_tUIDesc.m_iIndexMaxY                  = 10;
    m_tUIDesc.m_fFramePerSec                = 0.05f;

    m_tUIDesc.m_fTime                       = 0.f;
    m_tUIDesc.m_fSizeX                      = 26.f;
    m_tUIDesc.m_fSizeY                      = 220.f;

//================================================================================================ _float3
    m_vScale_BrainTalkFrame                 = _float3(586.f, 119.f, 1.f);
    m_vScale_BrainTalkFrame_Noise           = _float3(586.f, 119.f, 1.f);
    m_vScale_FaceFrame                      = _float3(133.f, 141.f, 1.f);
    m_vScale_FaceFrame_Noise                = _float3(107.f, 113.f, 1.f);
    m_vScale_BrainTalk_LightFrame           = _float3(70.f, 70.f, 1.f);
    m_vScale_BrainTalk_LightFrame_Noise     = _float3(70.f, 70.f, 1.f);
    m_vScale_BrainTalk_Light                = _float3(26.f, 22.f, 1.f);
    m_vScale_Face_Kasane                    = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Siden                     = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Arasi                     = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Kyoka                     = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Kagero                    = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Haruka                    = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Gemma                     = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Naomi                     = _float3(128.f, 128.f, 1.f);

//================================================================================================ _float4
    m_vPosition_BrainTalkFrame              = _float4(-610.f, -172.f, 0.f, 1.f);
    m_vPosition_BrainTalkFrame_Noise        = _float4(-610.f, -172.f, 0.f, 1.f);
    m_vPosition_FaceFrame                   = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_FaceFrame_Noise             = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_BrainTalk_LightFrame        = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_BrainTalk_LightFrame_Noise  = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_BrainTalk_Light             = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_Face_Kasane                 = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Siden                  = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Arasi                  = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Kyoka                  = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Kagero                 = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Haruka                 = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Gemma                  = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Naomi                  = _float4(-830.f, -40.f, 0.f, 1.f);


//================================================================================================ Scale
    m_pBrainTalkFrame->Set_Scale(m_vScale_BrainTalkFrame);
    m_pBrainTalkFrame_Noise->Set_Scale(m_vScale_BrainTalkFrame_Noise);
    m_pFaceFrame->Set_Scale(m_vScale_FaceFrame);
    m_pFaceFrame_Noise->Set_Scale(m_vScale_FaceFrame_Noise);
    m_pBrainTalk_Light->Set_Scale(m_vScale_BrainTalk_Light);
    m_pBrainTalk_LightFrame->Set_Scale(m_vScale_BrainTalk_LightFrame);
    m_pBrainTalk_LightFrame_Noise->Set_Scale(m_vScale_BrainTalk_LightFrame_Noise);
    m_pFace_Kasane->Set_Scale(m_vScale_Face_Kasane);
    m_pFace_Siden->Set_Scale(m_vScale_Face_Siden);
    m_pFace_Arasi->Set_Scale(m_vScale_Face_Arasi);
    m_pFace_Kyoka->Set_Scale(m_vScale_Face_Kyoka);
    m_pFace_Kagero->Set_Scale(m_vScale_Face_Kagero);
    m_pFace_Haruka->Set_Scale(m_vScale_Face_Haruka);
    m_pFace_Gemma->Set_Scale(m_vScale_Face_Gemma);
    m_pFace_Naomi->Set_Scale(m_vScale_Face_Naomi);

//================================================================================================ Position
    m_pBrainTalkFrame->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalkFrame);
    m_pBrainTalkFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalkFrame_Noise);
    m_pFaceFrame->Set_State(TRANSFORM::POSITION, m_vPosition_FaceFrame);
    m_pFaceFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_FaceFrame_Noise);
    m_pBrainTalk_Light->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_Light);
    m_pBrainTalk_LightFrame->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_LightFrame);
    m_pBrainTalk_LightFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_LightFrame_Noise);
    m_pFace_Kasane->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Kasane);
    m_pFace_Siden->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Siden);
    m_pFace_Arasi->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Arasi);
    m_pFace_Kyoka->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Kyoka);
    m_pFace_Kagero->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Kagero);
    m_pFace_Haruka->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Haruka);
    m_pFace_Gemma->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Gemma);
    m_pFace_Naomi->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Naomi);

//================================================================================================ wstring
    m_strText1                              = L"";
    m_strText2                              = L"";

}

void CUI_Indicator_BrainTalk_Mizuha::Tick_UI(_float fTimeDelta)
{
    //Set_BrainTalk_Test();
    //Shuffle_RandomNumber();

    if (false == m_bDisappear)
    {
        // Control Frame
        OpenBrainTalk(fTimeDelta);
        Tick_SpriteTexture(fTimeDelta);

        // Conversation
        Conversation_First(fTimeDelta);
        Conversation_Second(fTimeDelta);
        if (false == m_bIsBrainTalkNow)
        {
            Conversation_Third(fTimeDelta);
        }
        Conversation_Fourth(fTimeDelta);
        Conversation_Fifth(fTimeDelta);
        Conversation_Sixth(fTimeDelta);
        Conversation_Seventh(fTimeDelta);
        Conversation_BossStage(fTimeDelta);
        Conversation_BossStage_Half(fTimeDelta);
        Conversation_BossStage_End(fTimeDelta);
        Conversation_Crows(fTimeDelta);

        // Specific Situation
        Situation_Haruka_Player_Damaged_By_Electric(fTimeDelta);
        Situation_Haruka_Player_Damaged_By_Water(fTimeDelta);
        Situation_Kasane_LevelUp(fTimeDelta);
        if (false == m_bRender_Kasane_Get_Item)
        {
            Situation_Kasane_Get_Item(fTimeDelta);
        }
        Situation_Arasi_Use_My_SAS(fTimeDelta);
        Situation_Siden_Use_My_SAS(fTimeDelta);
        Situation_Kyoka_Use_My_SAS(fTimeDelta);
        Situation_Kagero_Use_My_SAS(fTimeDelta);
        Situation_Siden_Monster_Get_Wet(fTimeDelta);
        Situation_Kagero_Monster_Lost_Us(fTimeDelta);

        if (false == m_bRender_Kasane_Get_Item)
        {
            Situation_Siden_Notice_Items(fTimeDelta);
            Situation_Arasi_Notice_Items(fTimeDelta);
            Situation_Kyoka_Notice_Items(fTimeDelta);
            Situation_Kagero_Notice_Items(fTimeDelta);
        }
    }
    else
    {
        BrainTalk_Reset();
    }
}

void CUI_Indicator_BrainTalk_Mizuha::Shuffle_RandomNumber()
{
    srand(_uint(time(NULL)));

    m_iRandomNumber = rand() % 11;
}

void CUI_Indicator_BrainTalk_Mizuha::Set_BrainTalk_Test()
{
    //if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    //{
    //    //Set_BrainTalk_Conversation(0);
    //    Set_BrainTalk_SpecialSituation(5);
    //}
}

// Default
void CUI_Indicator_BrainTalk_Mizuha::OpenBrainTalk(_float fTimeDelta)
{
    if (true == m_bBrainTalk_First || true == m_bBrainTalk_Second || true == m_bBrainTalk_Third || true == m_bBrainTalk_Fourth 
        || true == m_bBrainTalk_Fifth || true == m_bBrainTalk_Sixth || true == m_bBrainTalk_Seventh || true == m_bBrainTalk_Crows
        || true == m_bBrainTalk_BossStage || true == m_bBrainTalk_BossStage_Half || true == m_bBrainTalk_BossStage_End
        || true == m_bBrainTalk_Haruka_Player_Damaged_By_Electric || true == m_bBrainTalk_Haruka_Player_Damaged_By_Water
        || true == m_bBrainTalk_Kasane_LevelUp || true == m_bBrainTalk_Kasane_Get_Item
        || true == m_bBrainTalk_Siden_Monster_Get_Wet || true == m_bBrainTalk_Kagero_Monster_Lost_Us
        || true == m_bBrainTalk_Arasi_Use_My_SAS || true == m_bBrainTalk_Siden_Use_My_SAS || true == m_bBrainTalk_Kyoka_Use_My_SAS || true == m_bBrainTalk_Kagero_Use_My_SAS
        || true == m_bBrainTalk_Siden_Notice_Item || true == m_bBrainTalk_Arasi_Notice_Item || true == m_bBrainTalk_Kyoka_Notice_Item || true == m_bBrainTalk_Kagero_Notice_Item)
    {
        if (true == m_bRender_BTLFN && false == m_bRender_BTLF)
        {
            if (1.f <= m_fBTLFN)
            {
                m_fBTLFN = 0.f;
                m_bRender_BTLFN = false;
                m_bRender_BTLF = true;
            }
            else
            {
                m_fBTLFN += fTimeDelta * 2.f;
            }
        }

        if (true == m_bRender_BTLF && false == m_bRender_Noise)
        {
            m_fBTLFN += fTimeDelta * 5.f;

            if (0.f <= m_fBTLFN && 0.2f > m_fBTLFN)
            {
                m_fBTLF = 1.f;
            }
            else if (0.2f <= m_fBTLFN && 0.4f > m_fBTLFN)
            {
                m_fBTLF = 0.1f;
            }
            else if (0.4f <= m_fBTLFN && 0.6f > m_fBTLFN)
            {
                m_fBTLF = 1.f;
            }
            else if (0.6f <= m_fBTLFN && 0.8f > m_fBTLFN)
            {
                m_fBTLF = 0.1f;
            }
            else if (0.8f <= m_fBTLFN)
            {
                m_fBTLF = 1.f;
                m_bRender_BTL = true;
            }

            if (1.f <= m_fBTLFN)
            {
                m_fBTLFN = 1.f;
                m_bRender_Noise = true;
                m_bRender_Face = true;
            }
        }

        if (true == m_bRender_Noise && 0.f != m_fNoiseAlpha)
        {
            if (0.f >= m_fNoiseAlpha)
            {
                m_fNoiseAlpha = 0.f;
                m_bRender_Noise = false;
            }
            else
            {
                m_fNoiseAlpha -= fTimeDelta;
            }

            if (0.2 >= m_fNoiseAlpha)
            {
                m_bRender_FaceFrame = true;
            }
        }

        if (true == m_bRender_FaceFrame)
        {
            m_fAccTime += fTimeDelta * 2.f;

            if (0.f <= m_fAccTime && 0.2f > m_fAccTime)
            {
                m_fFaceFrame = 1.f;
            }
            else if (0.2f <= m_fAccTime && 0.4f > m_fAccTime)
            {
                m_fFaceFrame = 0.4f;
            }
            else if (0.4f <= m_fAccTime)
            {
                m_fFaceFrame = 1.f;
                m_bRender_BTF = true;
            }
        }
    }
}
void CUI_Indicator_BrainTalk_Mizuha::Tick_SpriteTexture(_float fTimeDelta)
{
    if (true == m_bRender_BTL)
    {
        m_tUIDesc.m_fTime += fTimeDelta;

        if (m_tUIDesc.m_fFramePerSec <= m_tUIDesc.m_fTime)
        {
            m_tUIDesc.m_iIndex += 1;
            if (m_tUIDesc.m_iIndex >= m_tUIDesc.m_iIndexMaxY * m_tUIDesc.m_iIndexMaxX)
            {
                m_tUIDesc.m_iIndex = 0;
            }
            m_tUIDesc.m_fTime = 0.f;
        }
    }
}
void CUI_Indicator_BrainTalk_Mizuha::Set_BrainTalk_Conversation(_uint iBrainTalkIndex)
{
    if (SCENE::MIZUHA == CGameInstance::Get_Instance()->Current_Scene())
    {
        m_eBrainTalk_Conversation = (BRAINTALK_CONVERSATION)iBrainTalkIndex;

        switch (m_eBrainTalk_Conversation)
        {
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_FIRST:
            m_bBrainTalk_First = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_SECOND:
            m_bBrainTalk_Second = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_THIRD:
            m_bBrainTalk_Third = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_FOURTH:
            m_bBrainTalk_Fourth = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_FIFTH:
            m_bBrainTalk_Fifth = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_SIXTH:
            m_bBrainTalk_Sixth = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_SEVENTH:
            m_bBrainTalk_Seventh = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_BOSSSTAGE:
            m_bBrainTalk_BossStage = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_BOSSSTAGE_HALF:
            m_bBrainTalk_BossStage_Half = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_BOSSSTAGE_END:
            m_bBrainTalk_BossStage_End = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_CROWS:
            m_bBrainTalk_Crows = true;
            break;

        default:
            break;
        }
    }
}
void CUI_Indicator_BrainTalk_Mizuha::Set_BrainTalk_SpecialSituation(_uint iBrainTalkIndex)
{
    if (SCENE::MIZUHA == CGameInstance::Get_Instance()->Current_Scene())
    {
        m_eBrainTalk_Specific_Situation = (BRAINTALK_SPECIFIC_SITUATION)iBrainTalkIndex;

        switch (m_eBrainTalk_Specific_Situation)
        {
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_HARUKA_PLAYER_DAMAGED_BY_ELECTRIC:
            m_bBrainTalk_Haruka_Player_Damaged_By_Electric = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_HARUKA_PLAYER_DAMAGED_BY_WATER:
            m_bBrainTalk_Haruka_Player_Damaged_By_Water = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_KASANE_LEVELUP:
            m_bBrainTalk_Kasane_LevelUp = true;
            break;
        
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_KASANE_GET_ITEM:
            m_bBrainTalk_Kasane_Get_Item = true;
            break;
        
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_SIDEN_MONSTER_GET_WET:
            m_bBrainTalk_Siden_Monster_Get_Wet = true;
            break;
        
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_KAGERO_MONSTER_LOST_US:
            m_bBrainTalk_Kagero_Monster_Lost_Us = true;
            break;

        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_USE_MY_SAS_ARASI:
            m_bBrainTalk_Arasi_Use_My_SAS = true;
            break;
        
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_USE_MY_SAS_SIDEN:
            m_bBrainTalk_Siden_Use_My_SAS = true;
            break;
        
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_USE_MY_SAS_KYOKA:
            m_bBrainTalk_Kyoka_Use_My_SAS = true;
            break;
        
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_USE_MY_SAS_KAGERO:
            m_bBrainTalk_Kagero_Use_My_SAS = true;
            break;
        
        case Client::CUI_Indicator_BrainTalk_Mizuha::BT_NOTICE_ITEM:
        {
            Shuffle_RandomNumber();

            if (0 <= m_iRandomNumber && 2 > m_iRandomNumber)
            {
                m_bBrainTalk_Siden_Notice_Item = true;
            }
            if (2 <= m_iRandomNumber && 6 > m_iRandomNumber)
            {
                m_bBrainTalk_Arasi_Notice_Item = true;
            }
            if (6 <= m_iRandomNumber && 9 > m_iRandomNumber)
            {
                m_bBrainTalk_Kyoka_Notice_Item = true;
            }
            if (10 == m_iRandomNumber)
            {
                m_bBrainTalk_Kagero_Notice_Item = true;
            }
        }
        break;

        default:
            break;
        }
    }
}
void CUI_Indicator_BrainTalk_Mizuha::BrainTalk_Reset()
{
//================================================================================================ _bool
    // For. DIalogue
    m_bBrainTalk_First                              = false;
    m_bBrainTalk_Second                             = false;
    m_bBrainTalk_Third                              = false;
    m_bBrainTalk_Fourth                             = false;
    m_bBrainTalk_Fifth                              = false;
    m_bBrainTalk_Sixth                              = false;
    m_bBrainTalk_Seventh                            = false;
    m_bBrainTalk_BossStage                          = false;
    m_bBrainTalk_BossStage_Half                     = false;
    m_bBrainTalk_BossStage_End                      = false;
    m_bBrainTalk_Crows                              = false;

    // For. Specific Situation
    m_bBrainTalk_Haruka_Player_Damaged_By_Electric  = false;
    m_bBrainTalk_Haruka_Player_Damaged_By_Water     = false;

    m_bBrainTalk_Kasane_LevelUp                     = false;
    m_bBrainTalk_Kasane_Get_Item                    = false;

    m_bBrainTalk_Arasi_Use_My_SAS                   = false;
    m_bBrainTalk_Siden_Use_My_SAS                   = false;
    m_bBrainTalk_Kyoka_Use_My_SAS                   = false;
    m_bBrainTalk_Kagero_Use_My_SAS                  = false;

    m_bBrainTalk_Siden_Monster_Get_Wet              = false;                                                 
                                                    
    m_bBrainTalk_Kagero_Monster_Lost_Us             = false;                                                  

    m_bBrainTalk_Siden_Notice_Item                  = false;
    m_bBrainTalk_Arasi_Notice_Item                  = false;
    m_bBrainTalk_Kyoka_Notice_Item                  = false;
    m_bBrainTalk_Kagero_Notice_Item                 = false;

    // For. Render
    m_bRender_BTLFN                                 = true;
    m_bRender_BTLF                                  = false;
    m_bRender_Noise                                 = false;
    m_bRender_BTF                                   = false;
    m_bRender_BTL                                   = false;
    m_bRender_Face                                  = false;
    m_bRender_Face_Kasane                           = false;
    m_bRender_Face_Siden                            = false;
    m_bRender_Face_Arasi                            = false;
    m_bRender_Face_Kyoka                            = false;
    m_bRender_Face_Kagero                           = false;
    m_bRender_Face_Haruka                           = false;
    m_bRender_Face_Gemma                            = false;
    m_bRender_Face_Naomi                            = false;

    m_bIsBrainTalkNow                               = false;
    m_bRender_Kasane_Get_Item                       = false;

    // Master - Key
    m_bDisappear                                    = false;

    // Enum
    m_eBrainTalk_Conversation                       = BT_MAX;
    m_eBrainTalk_Specific_Situation                 = BT_SPEMAX;

//================================================================================================ _uint
    m_iCheckSoundIndex                      = 0;

    m_iFaceIndex_Kasane                     = 0;
    m_iFaceIndex_Siden                      = 0;
    m_iFaceIndex_Arasi                      = 0;
    m_iFaceIndex_Kyoka                      = 0;
    m_iFaceIndex_Kagero                     = 0;
    m_iFaceIndex_Haruka                     = 0;
    m_iFaceIndex_Gemma                      = 0;
    m_iFaceIndex_Naomi                      = 0;

//================================================================================================ _float
    m_fBTLFN                                = 0.f;
    m_fBTLF                                 = 0.f;
    m_fNoiseAlpha                           = 0.5f;
    m_fFaceFrame                            = 1.f;
    m_fAccTime                              = 0.f;

    m_fFace_KasaneAlpha                     = 0.f;
    m_fFace_SidenAlpha                      = 0.f;
    m_fFace_ArasiAlpha                      = 0.f;
    m_fFace_KyokaAlpha                      = 0.f;
    m_fFace_KageroAlpha                     = 0.f;
    m_fFace_HarukaAlpha                     = 0.f;
    m_fFace_GemmaAlpha                      = 0.f;
    m_fFace_NaomiAlpha                      = 0.f;

    m_fAccTime_Face_Kasane                  = 0.f;
    m_fAccTime_Face_Siden                   = 0.f;
    m_fAccTime_Face_Arasi                   = 0.f;
    m_fAccTime_Face_Kyoka                   = 0.f;
    m_fAccTime_Face_Kagero                  = 0.f;
    m_fAccTime_Face_Haruka                  = 0.f;
    m_fAccTime_Face_Gemma                   = 0.f;
    m_fAccTime_Face_Naomi                   = 0.f;

    m_fCheckSoundTime                       = 0.f;
    m_fBTF                                  = 0.f;

//================================================================================================ // Texture_Sprite_Desc // BTL
    ZeroMemory(&m_tUIDesc, sizeof m_tUIDesc);

    m_tUIDesc.m_iIndex                      = 0;
    m_tUIDesc.m_iIndexMaxX                  = 1;
    m_tUIDesc.m_iIndexMaxY                  = 10;
    m_tUIDesc.m_fFramePerSec                = 0.05f;

    m_tUIDesc.m_fTime                       = 0.f;
    m_tUIDesc.m_fSizeX                      = 26.f;
    m_tUIDesc.m_fSizeY                      = 220.f;

//================================================================================================ _float3
    m_vScale_BrainTalkFrame                 = _float3(586.f, 119.f, 1.f);
    m_vScale_BrainTalkFrame_Noise           = _float3(586.f, 119.f, 1.f);
    m_vScale_FaceFrame                      = _float3(133.f, 141.f, 1.f);
    m_vScale_FaceFrame_Noise                = _float3(107.f, 113.f, 1.f);
    m_vScale_BrainTalk_LightFrame           = _float3(70.f, 70.f, 1.f);
    m_vScale_BrainTalk_LightFrame_Noise     = _float3(70.f, 70.f, 1.f);
    m_vScale_BrainTalk_Light                = _float3(26.f, 22.f, 1.f);
    m_vScale_Face_Kasane                    = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Siden                     = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Arasi                     = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Kyoka                     = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Kagero                    = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Haruka                    = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Gemma                     = _float3(128.f, 128.f, 1.f);
    m_vScale_Face_Naomi                     = _float3(128.f, 128.f, 1.f);

//================================================================================================ _float4
    m_vPosition_BrainTalkFrame              = _float4(-610.f, -172.f, 0.f, 1.f);
    m_vPosition_BrainTalkFrame_Noise        = _float4(-610.f, -172.f, 0.f, 1.f);
    m_vPosition_FaceFrame                   = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_FaceFrame_Noise             = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_BrainTalk_LightFrame        = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_BrainTalk_LightFrame_Noise  = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_BrainTalk_Light             = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_Face_Kasane                 = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Siden                  = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Arasi                  = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Kyoka                  = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Kagero                 = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Haruka                 = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Gemma                  = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_Face_Naomi                  = _float4(-830.f, -40.f, 0.f, 1.f);


//================================================================================================ Scale
    m_pBrainTalkFrame->Set_Scale(m_vScale_BrainTalkFrame);
    m_pBrainTalkFrame_Noise->Set_Scale(m_vScale_BrainTalkFrame_Noise);
    m_pFaceFrame->Set_Scale(m_vScale_FaceFrame);
    m_pFaceFrame_Noise->Set_Scale(m_vScale_FaceFrame_Noise);
    m_pBrainTalk_Light->Set_Scale(m_vScale_BrainTalk_Light);
    m_pBrainTalk_LightFrame->Set_Scale(m_vScale_BrainTalk_LightFrame);
    m_pBrainTalk_LightFrame_Noise->Set_Scale(m_vScale_BrainTalk_LightFrame_Noise);
    m_pFace_Kasane->Set_Scale(m_vScale_Face_Kasane);
    m_pFace_Siden->Set_Scale(m_vScale_Face_Siden);
    m_pFace_Arasi->Set_Scale(m_vScale_Face_Arasi);
    m_pFace_Kyoka->Set_Scale(m_vScale_Face_Kyoka);
    m_pFace_Kagero->Set_Scale(m_vScale_Face_Kagero);
    m_pFace_Haruka->Set_Scale(m_vScale_Face_Haruka);
    m_pFace_Gemma->Set_Scale(m_vScale_Face_Gemma);
    m_pFace_Naomi->Set_Scale(m_vScale_Face_Naomi);

//================================================================================================ Position
    m_pBrainTalkFrame->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalkFrame);
    m_pBrainTalkFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalkFrame_Noise);
    m_pFaceFrame->Set_State(TRANSFORM::POSITION, m_vPosition_FaceFrame);
    m_pFaceFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_FaceFrame_Noise);
    m_pBrainTalk_Light->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_Light);
    m_pBrainTalk_LightFrame->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_LightFrame);
    m_pBrainTalk_LightFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_LightFrame_Noise);
    m_pFace_Kasane->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Kasane);
    m_pFace_Siden->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Siden);
    m_pFace_Arasi->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Arasi);
    m_pFace_Kyoka->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Kyoka);
    m_pFace_Kagero->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Kagero);
    m_pFace_Haruka->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Haruka);
    m_pFace_Gemma->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Gemma);
    m_pFace_Naomi->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Naomi);

//================================================================================================ wstring
    m_strText1                              = L"";
    m_strText2                              = L"";
}
// Conversation
void CUI_Indicator_BrainTalk_Mizuha::Conversation_First(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_First)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 2.5f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"다들, 잘 들려?";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_10.Mizuha_Haruka00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (3.f <= m_fCheckSoundTime && 11.f > m_fCheckSoundTime)
        {
            m_strText1 = L"개별 행동 중에도 브레인 토크로 언제라도 모두와";
            m_strText2 = L"연락이 가능하니까. 무슨 일 있으면 바로 알려줘.";
        }
        else if (11.f <= m_fCheckSoundTime && 12.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (2.5f <= m_fCheckSoundTime && 3.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion


#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_FIRST == m_eBrainTalk_Conversation)
        {
            m_bRender_Face_Haruka = true;

            if (1.f >= m_fAccTime_Face_Haruka)
            {
                m_fAccTime_Face_Haruka += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Haruka && 0.5f > m_fAccTime_Face_Haruka)
            {
                m_fFace_HarukaAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Haruka && 0.6f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Haruka && 0.7f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Haruka && 0.8f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Haruka && 0.9f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Haruka && 1.f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_Second(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Second)
    {
        m_fCheckSoundTime += fTimeDelta;
        
        // Main Line
        if (0.f <= m_fCheckSoundTime && 6.5f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"얘들아. 까마귀의 중계가 방해되겠지만,";
            m_strText2 = L"신경 쓰지 마.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK,L"Conversation_11.Mizuha_Dialogue09.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (6.8f <= m_fCheckSoundTime && 12.7f > m_fCheckSoundTime)
        {
            m_iFaceIndex_Haruka = 1;

            m_strText1 = L"재밍당하고 있는데도 자기들이 초대받지 못한";
            m_strText2 = L"손님이라는 걸 이해하지 못하는 걸까, 까마귀들은.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_11.Mizuha_Dialogue10.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (13.f <= m_fCheckSoundTime && 21.2f > m_fCheckSoundTime)
        {
            m_strText1 = L"어쩔 수 없어. 매스컴은 토벌군의 활동을 중계할 권리가";
            m_strText2 = L"있다는 걸 방패 삼아서 제멋대로 행동하고 있으니까.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_11.Mizuha_Dialogue11.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (21.5f <= m_fCheckSoundTime && 25.f > m_fCheckSoundTime)
        {
            m_strText1 = L"귀찮지만, 뭐, 유명세 같은 거지.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_11.Mizuha_Dialogue12.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (25.f <= m_fCheckSoundTime && 25.5f > m_fCheckSoundTime) // End
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);

            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (6.5f <= m_fCheckSoundTime && 6.8f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
            }

            m_iCheckSoundIndex = 0;
        }
        if (12.7f <= m_fCheckSoundTime && 13.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
            }

            m_iCheckSoundIndex = 0;
        }
        if (21.2f <= m_fCheckSoundTime && 21.5f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_SECOND == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 6.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = true;

                if (1.f >= m_fAccTime_Face_Kyoka)
                {
                    m_fAccTime_Face_Kyoka += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Kyoka && 0.5f > m_fAccTime_Face_Kyoka)
                {
                    m_fFace_KyokaAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Kyoka && 0.6f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Kyoka && 0.7f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Kyoka && 0.8f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Kyoka && 0.9f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Kyoka && 1.f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
            }

            if (6.8f <= m_fCheckSoundTime && 12.7f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = false;
                m_bRender_Face_Haruka = true;
                m_fFace_HarukaAlpha = 1.f;
            }

            if (13.f <= m_fCheckSoundTime && 21.2f > m_fCheckSoundTime)
            {
                m_bRender_Face_Haruka = false;
                m_bRender_Face_Kyoka = true;
            }

            if (21.5f <= m_fCheckSoundTime && 25.f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = false;
                m_bRender_Face_Siden = true;
                m_fFace_SidenAlpha = 1.f;
            }
        }
    }
#pragma endregion

}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_Third(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Third)
    {
        m_fCheckSoundTime += fTimeDelta;

        m_bRender_Kasane_Get_Item = true;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 5.f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"미즈하강 상공은";
            m_strText2 = L"단절의 띠가 짙어진 장소였지.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK,L"Conversation_12.Mizuha_Dialogue00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (5.3f <= m_fCheckSoundTime && 11.3f > m_fCheckSoundTime)
        {
            m_strText1 = L"그래, 맞아. 도시 건설 중에 단절의 띠의 흐름이 바뀌어서";
            m_strText2 = L"괴이 발생이 많은 장소가 되어 버렸다.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_12.Mizuha_Dialogue01.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (11.8f <= m_fCheckSoundTime && 17.f > m_fCheckSoundTime)
        {
            m_strText1 = L"강력한 괴이가 많진 않지만";
            m_strText2 = L"다들, 긴장을 풀지 마.";
        }
        else if (17.f <= m_fCheckSoundTime && 17.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bRender_Kasane_Get_Item = false;
            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (5.f <= m_fCheckSoundTime && 5.3f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (11.3f <= m_fCheckSoundTime && 11.8f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_THIRD == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 5.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kasane = true;

                if (1.f >= m_fAccTime_Face_Kasane)
                {
                    m_fAccTime_Face_Kasane += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Kasane && 0.5f > m_fAccTime_Face_Kasane)
                {
                    m_fFace_KasaneAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Kasane && 0.6f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Kasane && 0.7f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Kasane && 0.8f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Kasane && 0.9f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Kasane && 1.f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
            }

            if (5.3f <= m_fCheckSoundTime && 17.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kasane = false;
                m_bRender_Face_Gemma = true;
                m_fFace_GemmaAlpha = 1.f;
            }
        }
    }
#pragma endregion

}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_Fourth(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Fourth)
    {
        m_fCheckSoundTime += fTimeDelta;
        m_bRender_Kasane_Get_Item = true;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 6.f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"나오미, 긴장하고 있나?";
            m_strText2 = L"조금 호흡이 얕아진 것 같은데.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_14.Mizuha_Dialogue00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (6.5f <= m_fCheckSoundTime && 12.5f > m_fCheckSoundTime)
        {
            m_strText1 = L"아, 네, 조금. 그래도 괜찮아요!";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_14.Mizuha_Dialogue01.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (13.f <= m_fCheckSoundTime && 19.5f > m_fCheckSoundTime)
        {
            m_strText1 = L"심호흡하면 괜찮을 거야.";
            m_strText2 = L"나오미는 카사네도 걱정하고 있는 거겠지.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_14.Mizuha_Dialogue02.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (20.f <= m_fCheckSoundTime && 25.5f > m_fCheckSoundTime)
        {
            m_iFaceIndex_Gemma = 2;

            m_strText1 = L"카사네도 나오미를 걱정하고 있었지.";
            m_strText2 = L"좋은 자매구나.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_14.Mizuha_Dialogue03.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (25.5f <= m_fCheckSoundTime && 26.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
            m_bRender_Kasane_Get_Item = false;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (6.f <= m_fCheckSoundTime && 6.5f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (12.5f <= m_fCheckSoundTime && 13.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (19.5f <= m_fCheckSoundTime && 20.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_FOURTH == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 5.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Gemma = true;

                if (1.f >= m_fAccTime_Face_Gemma)
                {
                    m_fAccTime_Face_Gemma += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Gemma && 0.5f > m_fAccTime_Face_Gemma)
                {
                    m_fFace_GemmaAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Gemma && 0.6f > m_fAccTime_Face_Gemma) // Blink
                {
                    m_fFace_GemmaAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Gemma && 0.7f > m_fAccTime_Face_Gemma) // Blink
                {
                    m_fFace_GemmaAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Gemma && 0.8f > m_fAccTime_Face_Gemma) // Blink
                {
                    m_fFace_GemmaAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Gemma && 0.9f > m_fAccTime_Face_Gemma) // Blink
                {
                    m_fFace_GemmaAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Gemma && 1.f > m_fAccTime_Face_Gemma) // Blink
                {
                    m_fFace_GemmaAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Gemma) // Blink
                {
                    m_fFace_GemmaAlpha = 1.f;
                }
            }

            if (6.5f <= m_fCheckSoundTime && 12.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Gemma = false;
                m_bRender_Face_Naomi = true;
                m_fFace_NaomiAlpha = 1.f;
            }
            if (13.f <= m_fCheckSoundTime && 19.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Naomi = false;
                m_bRender_Face_Kyoka = true;
                m_fFace_KyokaAlpha = 1.f;
            }
            if (20.f <= m_fCheckSoundTime && 25.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = false;
                m_bRender_Face_Gemma = true;
            }
        }
    }
#pragma endregion

}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_Fifth(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Fifth)
    {
        m_fCheckSoundTime += fTimeDelta;
        m_bRender_Kasane_Get_Item = true;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 7.8f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"스오와 세이란의 중간에 있는 미즈하강은";
            m_strText2 = L"주거지로 쓰기 위해 개척이 진행되던 장소야.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_10.Mizuha_Elevator.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (8.2f <= m_fCheckSoundTime && 17.8f > m_fCheckSoundTime)
        {
            m_strText1 = L"공사가 중단된 지금도 중장비투성이야. 그리고";
            m_strText2 = L"미즈하강에 걸린 다리는 아직 교역로로 쓰이고 있어.";
        }
        else if (17.8f <= m_fCheckSoundTime && 18.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
            m_bRender_Kasane_Get_Item = false;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (7.8f <= m_fCheckSoundTime && 8.2f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_FIFTH == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 18.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = true;

                if (1.f >= m_fAccTime_Face_Kyoka)
                {
                    m_fAccTime_Face_Kyoka += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Kyoka && 0.5f > m_fAccTime_Face_Kyoka)
                {
                    m_fFace_KyokaAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Kyoka && 0.6f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Kyoka && 0.7f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Kyoka && 0.8f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Kyoka && 0.9f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Kyoka && 1.f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
            }
        }
    }
#pragma endregion


}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_Sixth(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Sixth)
    {
        m_fCheckSoundTime += fTimeDelta;
        m_bRender_Kasane_Get_Item = true;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 7.2f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"시덴은 코다마 소대에서 이동해 온 거지?";
            m_strText2 = L"그렇게 코다마와 사이가 안 좋았던 거야?";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_15.Mizuha_Dialogue00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (7.5f <= m_fCheckSoundTime && 15.5f > m_fCheckSoundTime)
        {
            m_iFaceIndex_Siden = 1;

            m_strText1 = L"......그 여자, 평소엔 신이라도 난 듯이 행동하는데";
            m_strText2 = L"속에 무슨 꿍꿍이가 있는지 전혀 모르겠어.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_15.Mizuha_Dialogue01.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (16.f <= m_fCheckSoundTime && 21.f > m_fCheckSoundTime)
        {
            m_strText1 = L"그리고 그 녀석......";
            m_strText2 = L"나한테만 이상하게 깐깐했어.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_15.Mizuha_Dialogue02.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (21.f <= m_fCheckSoundTime && 21.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
            m_bRender_Kasane_Get_Item = false;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (7.2f <= m_fCheckSoundTime && 7.5f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (15.5f <= m_fCheckSoundTime && 16.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_SIXTH == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 7.f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kasane = true;

                if (1.f >= m_fAccTime_Face_Kasane)
                {
                    m_fAccTime_Face_Kasane += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Kasane && 0.5f > m_fAccTime_Face_Kasane)
                {
                    m_fFace_KasaneAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Kasane && 0.6f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Kasane && 0.7f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Kasane && 0.8f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Kasane && 0.9f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Kasane && 1.f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
            }

            if (7.5f <= m_fCheckSoundTime && 15.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kasane = false;
                m_bRender_Face_Siden = true;
                m_fFace_SidenAlpha = 1.f;
            }
        }
    }
#pragma endregion

}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_Seventh(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Seventh)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 5.2f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();
            m_bRender_Kasane_Get_Item = true;

            m_strText1 = L"카사네 반, 상황은 어때?";
            m_strText2 = L"시덴이랑 잘하고 있어?";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK,L"Conversation_16.Mizuha_Dialogue00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (5.5f <= m_fCheckSoundTime && 10.5f > m_fCheckSoundTime)
        {
            m_strText1 = L"시덴...... 아, 네.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK,L"Conversation_16.Mizuha_Dialogue01.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (11.f <= m_fCheckSoundTime && 16.f > m_fCheckSoundTime)
        {
            m_iFaceIndex_Siden = 1;

            m_strText1 = L"너, 지금 내가 있다는 걸 까먹고 있었지!?";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK,L"Conversation_16.Mizuha_Dialogue02.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (16.f <= m_fCheckSoundTime && 17.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
            m_bRender_Kasane_Get_Item = false;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (5.2f <= m_fCheckSoundTime && 5.5f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (10.5f <= m_fCheckSoundTime && 11.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_SEVENTH == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 5.f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = true;

                if (1.f >= m_fAccTime_Face_Kyoka)
                {
                    m_fAccTime_Face_Kyoka += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Kyoka && 0.5f > m_fAccTime_Face_Kyoka)
                {
                    m_fFace_KyokaAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Kyoka && 0.6f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Kyoka && 0.7f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Kyoka && 0.8f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Kyoka && 0.9f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Kyoka && 1.f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
            }

            if (5.5f <= m_fCheckSoundTime && 10.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = false;
                m_bRender_Face_Kasane = true;
                m_fFace_KasaneAlpha = 1.f;
            }
            if (11.f <= m_fCheckSoundTime && 16.f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kasane = false;
                m_bRender_Face_Siden = true;
                m_fFace_SidenAlpha = 1.f;
            }
        }
    }
#pragma endregion

}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_BossStage(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_BossStage)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 5.2f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_iFaceIndex_Kasane = 1;

            m_strText1 = L"토벌군을 죽인 괴이......";
            m_strText2 = L"무작정 공격하는 건 위험해.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_18.Mizuha_Dialogue00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (5.5f <= m_fCheckSoundTime && 13.5f > m_fCheckSoundTime)
        {
            m_strText1 = L"머리를 써. 예를 들면...... 저걸 이용한다든지 말이지.";
            m_strText2 = L"어디 염력 솜씨 좀 보자고.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_18.Mizuha_Dialogue01.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (14.f <= m_fCheckSoundTime && 22.f > m_fCheckSoundTime)
        {
            m_strText1 = L"이봐, 저 괴이의 등에 있는 수조,";
            m_strText2 = L"안에 든게 물이라면...... 활용할 수 있을지도 몰라.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_18.Mizuha_Dialogue02.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (22.f <= m_fCheckSoundTime && 23.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (5.2f <= m_fCheckSoundTime && 5.5f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (13.5f <= m_fCheckSoundTime && 14.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_BOSSSTAGE == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 5.2f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kasane = true;

                if (1.f >= m_fAccTime_Face_Kasane)
                {
                    m_fAccTime_Face_Kasane += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Kasane && 0.5f > m_fAccTime_Face_Kasane)
                {
                    m_fFace_KasaneAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Kasane && 0.6f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Kasane && 0.7f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Kasane && 0.8f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Kasane && 0.9f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Kasane && 1.f > m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Kasane) // Blink
                {
                    m_fFace_KasaneAlpha = 1.f;
                }
            }

            if (5.5f <= m_fCheckSoundTime && 23.f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kasane = false;
                m_bRender_Face_Siden = true;
                m_fFace_SidenAlpha = 1.f;
            }
        }
    }
#pragma endregion

}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_BossStage_Half(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_BossStage_Half)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 6.2f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_iFaceIndex_Naomi = 1;

            m_strText1 = L"카사네, 괜찮아?";
            m_strText2 = L"처음 임무에서 갑자기 중대 괴이라니......";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_19.Mizuha_Dialogue_00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (6.5f <= m_fCheckSoundTime && 9.2f > m_fCheckSoundTime)
        {
            m_strText1 = L"걱정하지마, 난 괜찮아.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_19.Mizuha_Dialogue_01.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (9.5f <= m_fCheckSoundTime && 16.7f > m_fCheckSoundTime)
        {
            m_iFaceIndex_Siden = 1;

            m_strText1 = L"나오미, 내가 있으니까 걱정할 필요는 없어.";
            m_strText2 = L"...... 하지만, 되도록이면 빨리 원호하러 와 줘.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_19.Mizuha_Dialogue_02.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (17.f <= m_fCheckSoundTime && 19.5f > m_fCheckSoundTime)
        {
            m_strText1 = L"네. 가능한 한 서두를게요.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_19.Mizuha_Dialogue_03.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (19.5f <= m_fCheckSoundTime && 20.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (6.2f <= m_fCheckSoundTime && 6.5f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (9.2f <= m_fCheckSoundTime && 9.5f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (16.7f <= m_fCheckSoundTime && 17.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_BOSSSTAGE_HALF == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 6.2f > m_fCheckSoundTime)
            {
                m_bRender_Face_Naomi = true;

                if (1.f >= m_fAccTime_Face_Naomi)
                {
                    m_fAccTime_Face_Naomi += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Naomi && 0.5f > m_fAccTime_Face_Naomi)
                {
                    m_fFace_NaomiAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Naomi && 0.6f > m_fAccTime_Face_Naomi) // Blink
                {
                    m_fFace_NaomiAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Naomi && 0.7f > m_fAccTime_Face_Naomi) // Blink
                {
                    m_fFace_NaomiAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Naomi && 0.8f > m_fAccTime_Face_Naomi) // Blink
                {
                    m_fFace_NaomiAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Naomi && 0.9f > m_fAccTime_Face_Naomi) // Blink
                {
                    m_fFace_NaomiAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Naomi && 1.f > m_fAccTime_Face_Naomi) // Blink
                {
                    m_fFace_NaomiAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Naomi) // Blink
                {
                    m_fFace_NaomiAlpha = 1.f;
                }
            }

            if (6.5f <= m_fCheckSoundTime && 9.2f > m_fCheckSoundTime)
            {
                m_bRender_Face_Naomi = false;
                m_bRender_Face_Kasane = true;
                m_fFace_KasaneAlpha = 1.f;
            }
            if (9.5f <= m_fCheckSoundTime && 16.7f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kasane = false;
                m_bRender_Face_Siden = true;
                m_fFace_SidenAlpha = 1.f;
            }
            if (17.f <= m_fCheckSoundTime && 19.5f > m_fCheckSoundTime)
            {
                m_bRender_Face_Siden = false;
                m_bRender_Face_Naomi = true;
            }
        }
    }
#pragma endregion

}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_BossStage_End(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_BossStage_End)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 4.7f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_iFaceIndex_Kyoka = 2;

            m_strText1 = L"카사네, 시덴, 잘했어.";
            m_strText2 = L"한 건 해결했네.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_21.Mizuha_BattleEnd00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (5.f <= m_fCheckSoundTime && 9.2f > m_fCheckSoundTime)
        {
            m_iFaceIndex_Naomi = 2;

            m_strText1 = L"카사네...... 정말 무사해서 다행이야.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_21.Mizuha_BattleEnd01.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (9.5f <= m_fCheckSoundTime && 13.f > m_fCheckSoundTime)
        {
            m_iFaceIndex_Gemma = 2;

            m_strText1 = L"수석 졸업했다는 실력은 진짜였군.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_21.Mizuha_BattleEnd02.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (13.f <= m_fCheckSoundTime && 13.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        // Next Step
        if (4.7f <= m_fCheckSoundTime && 5.f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
        if (9.2f <= m_fCheckSoundTime && 9.5f > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_BOSSSTAGE_END == m_eBrainTalk_Conversation)
        {
            if (0.f <= m_fCheckSoundTime && 4.7f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = true;

                if (1.f >= m_fAccTime_Face_Kyoka)
                {
                    m_fAccTime_Face_Kyoka += fTimeDelta * 3.f;
                }

                if (0.f <= m_fAccTime_Face_Kyoka && 0.5f > m_fAccTime_Face_Kyoka)
                {
                    m_fFace_KyokaAlpha += fTimeDelta * 3.f;
                }
                else if (0.5f <= m_fAccTime_Face_Kyoka && 0.6f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (0.6f <= m_fAccTime_Face_Kyoka && 0.7f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
                else if (0.7f <= m_fAccTime_Face_Kyoka && 0.8f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (0.8f <= m_fAccTime_Face_Kyoka && 0.9f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
                else if (0.9f <= m_fAccTime_Face_Kyoka && 1.f > m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 0.f;
                }
                else if (1.f <= m_fAccTime_Face_Kyoka) // Blink
                {
                    m_fFace_KyokaAlpha = 1.f;
                }
            }

            if (5.f <= m_fCheckSoundTime && 9.2f > m_fCheckSoundTime)
            {
                m_bRender_Face_Kyoka = false;
                m_bRender_Face_Naomi = true;
                m_fFace_NaomiAlpha = 1.f;
            }
            if (9.5f <= m_fCheckSoundTime && 13.f > m_fCheckSoundTime)
            {
                m_bRender_Face_Naomi = false;
                m_bRender_Face_Gemma = true;
                m_fFace_GemmaAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Conversation_Crows(_float fTimeDelta)
{
#pragma region Conversation

#pragma endregion

#pragma region Face

#pragma endregion

}
// Specific_Situaion
void CUI_Indicator_BrainTalk_Mizuha::Situation_Haruka_Player_Damaged_By_Electric(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Haruka_Player_Damaged_By_Electric)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 7.f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();
            m_bRender_Kasane_Get_Item = true;

            m_iFaceIndex_Haruka = 1;

            m_strText1 = L"적이 전기로 공격하고 있어. 맞으면 위험해.";
            m_strText2 = L"찌릿찌릿해서 움직일 수 없게 되니까.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_10.Mizuha_Haruka02.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (7.f <= m_fCheckSoundTime && 7.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
            m_bRender_Kasane_Get_Item = false;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_HARUKA_PLAYER_DAMAGED_BY_ELECTRIC == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Haruka = true;

            if (1.f >= m_fAccTime_Face_Haruka)
            {
                m_fAccTime_Face_Haruka += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Haruka && 0.5f > m_fAccTime_Face_Haruka)
            {
                m_fFace_HarukaAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Haruka && 0.6f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Haruka && 0.7f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Haruka && 0.8f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Haruka && 0.9f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Haruka && 1.f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Haruka_Player_Damaged_By_Water(_float fTimeDelta)
{
    if (true == m_bRender_BTF && true == m_bBrainTalk_Haruka_Player_Damaged_By_Water)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 7.f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();
            m_bRender_Kasane_Get_Item = true;

            m_iFaceIndex_Haruka = 1;

            m_strText1 = L"괴이의 물 공격을 확인했는데...... 괜찮아?";
            m_strText2 = L"물에 젖으면 움직이기 힘들지.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_10.Mizuha_Haruka01.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (7.f <= m_fCheckSoundTime && 7.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
            m_bRender_Kasane_Get_Item = false;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_HARUKA_PLAYER_DAMAGED_BY_WATER == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Haruka = true;

            if (1.f >= m_fAccTime_Face_Haruka)
            {
                m_fAccTime_Face_Haruka += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Haruka && 0.5f > m_fAccTime_Face_Haruka)
            {
                m_fFace_HarukaAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Haruka && 0.6f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Haruka && 0.7f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Haruka && 0.8f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Haruka && 0.9f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Haruka && 1.f > m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Haruka) // Blink
            {
                m_fFace_HarukaAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Kasane_LevelUp(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Kasane_LevelUp)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 3.f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_iFaceIndex_Kasane = 2;
            m_bRender_Kasane_Get_Item = true;

            m_strText1 = L"또 한 걸음. 나아간 걸까.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_99.Kasane_Level_Up.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (3.f <= m_fCheckSoundTime && 3.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
            m_bRender_Kasane_Get_Item = false;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_KASANE_LEVELUP == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Kasane = true;

            if (1.f >= m_fAccTime_Face_Kasane)
            {
                m_fAccTime_Face_Kasane += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Kasane && 0.5f > m_fAccTime_Face_Kasane)
            {
                m_fFace_KasaneAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Kasane && 0.6f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Kasane && 0.7f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Kasane && 0.8f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Kasane && 0.9f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Kasane && 1.f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Kasane_Get_Item(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Kasane_Get_Item)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 2.7f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();
            m_bIsBrainTalkNow = true;
            m_strText1 = L"그럭저럭 꽤 괜찮은게 손에 들어왔어.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_99.Kasane_Get_Items.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (2.7f <= m_fCheckSoundTime && 3.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
            m_bIsBrainTalkNow = false;
            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_KASANE_GET_ITEM == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Kasane = true;

            if (1.f >= m_fAccTime_Face_Kasane)
            {
                m_fAccTime_Face_Kasane += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Kasane && 0.5f > m_fAccTime_Face_Kasane)
            {
                m_fFace_KasaneAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Kasane && 0.6f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Kasane && 0.7f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Kasane && 0.8f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Kasane && 0.9f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Kasane && 1.f > m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Kasane) // Blink
            {
                m_fFace_KasaneAlpha = 1.f;
            }
        }
    }
#pragma endregion

}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Arasi_Use_My_SAS(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Arasi_Use_My_SAS)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 4.5f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"카사네, SAS로 초고속을 써보는 건 어때?";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_SFX_Kasane_Need_SAS_Acceleration.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (4.5f <= m_fCheckSoundTime && 5.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_USE_MY_SAS_ARASI == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Arasi = true;

            if (1.f >= m_fAccTime_Face_Arasi)
            {
                m_fAccTime_Face_Arasi += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Arasi && 0.5f > m_fAccTime_Face_Arasi)
            {
                m_fFace_ArasiAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Arasi && 0.6f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Arasi && 0.7f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Arasi && 0.8f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Arasi && 0.9f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Arasi && 1.f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Siden_Use_My_SAS(_float fTimeDelta)
{ 
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Siden_Use_My_SAS)
    {
        m_fCheckSoundTime += fTimeDelta;
    
        // Main Line
        if (0.f <= m_fCheckSoundTime && 2.7f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"내 초뇌능력을 써라, 카사네!";
            m_strText2 = L"";
    
            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Voice_Siden_Need_SAS.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (2.7f <= m_fCheckSoundTime && 3.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;
    
            m_bDisappear = true;
            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_USE_MY_SAS_SIDEN == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Siden = true;

            if (1.f >= m_fAccTime_Face_Siden)
            {
                m_fAccTime_Face_Siden += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Siden && 0.5f > m_fAccTime_Face_Siden)
            {
                m_fFace_SidenAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Siden && 0.6f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Siden && 0.7f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Siden && 0.8f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Siden && 0.9f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Siden && 1.f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Kyoka_Use_My_SAS(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Kyoka_Use_My_SAS)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 4.f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"카사네, 내 복제 능력을 쓰는 거야.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_SFX_Kasane_Need_SAS_Replication.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (4.f <= m_fCheckSoundTime && 4.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;

            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_USE_MY_SAS_KYOKA == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Kyoka = true;

            if (1.f >= m_fAccTime_Face_Kyoka)
            {
                m_fAccTime_Face_Kyoka += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Kyoka && 0.5f > m_fAccTime_Face_Kyoka)
            {
                m_fFace_KyokaAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Kyoka && 0.6f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Kyoka && 0.7f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Kyoka && 0.8f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Kyoka && 0.9f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Kyoka && 1.f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Kagero_Use_My_SAS(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Kagero_Use_My_SAS)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 3.5f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_strText1 = L"카사네, SAS로 투명화하는게 좋겠어!";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Voice_Kagero_Need_SAS.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (3.5f <= m_fCheckSoundTime && 4.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;

            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_USE_MY_SAS_KAGERO == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Kagero = true;

            if (1.f >= m_fAccTime_Face_Kagero)
            {
                m_fAccTime_Face_Kagero += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Kagero && 0.5f > m_fAccTime_Face_Kagero)
            {
                m_fFace_KageroAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Kagero && 0.6f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Kagero && 0.7f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Kagero && 0.8f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Kagero && 0.9f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Kagero && 1.f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Siden_Monster_Get_Wet(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Siden_Monster_Get_Wet)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 2.3f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_iFaceIndex_Siden = 1;

            m_strText1 = L"저 녀석은 이제 흠뻑 젖었어!";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Conversation_10.Mizuha_Siden06.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (2.7f <= m_fCheckSoundTime && 8.2f > m_fCheckSoundTime)
        {
            m_strText1 = L"지금이 바로 SAS로 내 초뇌능력을 쓸 때야.";
            m_strText2 = L"알고 있겠지?";
        }
        else if (8.2f <= m_fCheckSoundTime && 8.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;

            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }

        if (2.3f <= m_fCheckSoundTime && 2.7 > m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
            }

            m_iCheckSoundIndex = 0;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_SIDEN_MONSTER_GET_WET == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Siden = true;

            if (1.f >= m_fAccTime_Face_Siden)
            {
                m_fAccTime_Face_Siden += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Siden && 0.5f > m_fAccTime_Face_Siden)
            {
                m_fFace_SidenAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Siden && 0.6f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Siden && 0.7f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Siden && 0.8f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Siden && 0.9f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Siden && 1.f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Kagero_Monster_Lost_Us(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Kagero_Monster_Lost_Us)
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 3.2f > m_fCheckSoundTime)
        {
            CUI_Manager::Get_Instance()->Set_BrainTalk_True();

            m_iFaceIndex_Siden = 1;

            m_strText1 = L"좋아, 좋아. 우리를 놓친 것 같군.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Voice_Kagero_Used_SAS.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (3.2f <= m_fCheckSoundTime && 3.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;

            CUI_Manager::Get_Instance()->Set_BrainTalk_False();
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_KAGERO_MONSTER_LOST_US == m_eBrainTalk_Specific_Situation)
        {
            m_bRender_Face_Kagero = true;

            if (1.f >= m_fAccTime_Face_Kagero)
            {
                m_fAccTime_Face_Kagero += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Kagero && 0.5f > m_fAccTime_Face_Kagero)
            {
                m_fFace_KageroAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Kagero && 0.6f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Kagero && 0.7f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Kagero && 0.8f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Kagero && 0.9f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Kagero && 1.f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Siden_Notice_Items(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Siden_Notice_Item && false == CUI_Manager::Get_Instance()->Get_BrainTalk())
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 4.2f > m_fCheckSoundTime)
        {
            m_strText1 = L"주변에 물자 반응이 있다. 확인하는게 어때.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Voice_Siden_Notice_Item.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (4.2f <= m_fCheckSoundTime && 4.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_NOTICE_ITEM == m_eBrainTalk_Specific_Situation && 0 <= m_iRandomNumber && 2 > m_iRandomNumber && false == CUI_Manager::Get_Instance()->Get_BrainTalk())
        {
            m_bRender_Face_Siden = true;

            if (1.f >= m_fAccTime_Face_Siden)
            {
                m_fAccTime_Face_Siden += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Siden && 0.5f > m_fAccTime_Face_Siden)
            {
                m_fFace_SidenAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Siden && 0.6f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Siden && 0.7f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Siden && 0.8f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Siden && 0.9f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Siden && 1.f > m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Siden) // Blink
            {
                m_fFace_SidenAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Arasi_Notice_Items(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Arasi_Notice_Item && false == CUI_Manager::Get_Instance()->Get_BrainTalk())
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 5.5f > m_fCheckSoundTime)
        {
            m_strText1 = L"주변에 물자 반응이 있는 것 같다만.......";
            m_strText2 = L"가지러 가는건 맡길게.";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Voice_Arasi_NoticeItem.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (5.5f <= m_fCheckSoundTime && 6.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_NOTICE_ITEM == m_eBrainTalk_Specific_Situation && 2 <= m_iRandomNumber && 6 > m_iRandomNumber && false == CUI_Manager::Get_Instance()->Get_BrainTalk())
        {
            m_bRender_Face_Arasi = true;

            if (1.f >= m_fAccTime_Face_Arasi)
            {
                m_fAccTime_Face_Arasi += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Arasi && 0.5f > m_fAccTime_Face_Arasi)
            {
                m_fFace_ArasiAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Arasi && 0.6f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Arasi && 0.7f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Arasi && 0.8f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Arasi && 0.9f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Arasi && 1.f > m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Arasi) // Blink
            {
                m_fFace_ArasiAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Kyoka_Notice_Items(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Kyoka_Notice_Item && false == CUI_Manager::Get_Instance()->Get_BrainTalk())
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 3.f > m_fCheckSoundTime)
        {
            m_strText1 = L"주변에 물자 반응이 있는 것 같아 보이네.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Voice_Kyoka_NoticeItem.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (3.f <= m_fCheckSoundTime && 3.5f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
        }
    }
#pragma endregion

#pragma region Face
    if (true == m_bRender_Face)
    {
        if (BT_NOTICE_ITEM == m_eBrainTalk_Specific_Situation && 6 <= m_iRandomNumber && 9 > m_iRandomNumber && false == CUI_Manager::Get_Instance()->Get_BrainTalk())
        {
            m_bRender_Face_Kyoka = true;

            if (1.f >= m_fAccTime_Face_Kyoka)
            {
                m_fAccTime_Face_Kyoka += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Kyoka && 0.5f > m_fAccTime_Face_Kyoka)
            {
                m_fFace_KyokaAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Kyoka && 0.6f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Kyoka && 0.7f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Kyoka && 0.8f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Kyoka && 0.9f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Kyoka && 1.f > m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Kyoka) // Blink
            {
                m_fFace_KyokaAlpha = 1.f;
            }
        }
    }
#pragma endregion
}
void CUI_Indicator_BrainTalk_Mizuha::Situation_Kagero_Notice_Items(_float fTimeDelta)
{
#pragma region Conversation
    if (true == m_bRender_BTF && true == m_bBrainTalk_Kagero_Notice_Item && false == CUI_Manager::Get_Instance()->Get_BrainTalk())
    {
        m_fCheckSoundTime += fTimeDelta;

        // Main Line
        if (0.f <= m_fCheckSoundTime && 3.5f > m_fCheckSoundTime)
        {
            m_strText1 = L"주변에 물자 반응이 있어 보이는걸.";
            m_strText2 = L"";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySound_BrainTalk(CSound_Manager::SOUND_BRAINTALK, L"Voice_Kagero_NoticeItem.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        else if (3.5f <= m_fCheckSoundTime && 4.f > m_fCheckSoundTime)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
            m_iCheckSoundIndex = 0;

            m_bDisappear = true;
        }
    }
#pragma endregion

#pragma region Face6
    if (true == m_bRender_Face)
    {
        if (BT_NOTICE_ITEM == m_eBrainTalk_Specific_Situation && 10 == m_iRandomNumber && false == CUI_Manager::Get_Instance()->Get_BrainTalk())
        {
            m_bRender_Face_Kagero = true;

            if (1.f >= m_fAccTime_Face_Kagero)
            {
                m_fAccTime_Face_Kagero += fTimeDelta * 3.f;
            }

            if (0.f <= m_fAccTime_Face_Kagero && 0.5f > m_fAccTime_Face_Kagero)
            {
                m_fFace_KageroAlpha += fTimeDelta * 3.f;
            }
            else if (0.5f <= m_fAccTime_Face_Kagero && 0.6f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (0.6f <= m_fAccTime_Face_Kagero && 0.7f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
            else if (0.7f <= m_fAccTime_Face_Kagero && 0.8f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (0.8f <= m_fAccTime_Face_Kagero && 0.9f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
            else if (0.9f <= m_fAccTime_Face_Kagero && 1.f > m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 0.f;
            }
            else if (1.f <= m_fAccTime_Face_Kagero) // Blink
            {
                m_fFace_KageroAlpha = 1.f;
            }
        }
    }
#pragma endregion
}

shared_ptr<CUI_Indicator_BrainTalk_Mizuha> CUI_Indicator_BrainTalk_Mizuha::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_BrainTalk_Mizuha> pInstance = make_private_shared(CUI_Indicator_BrainTalk_Mizuha, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_BrainTalk_Mizuha::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
shared_ptr<CGameObject> CUI_Indicator_BrainTalk_Mizuha::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_BrainTalk_Mizuha> pInstance = make_private_shared_copy(CUI_Indicator_BrainTalk_Mizuha, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_BrainTalk_Mizuha::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

// 잊지말자 혹시 쓸 것 같은 남은 대사

// 카사네 저 녀석에게는 내 초고속이 효과적일 거야.
// 지금껀 깔끔했다. 나만큼은 아니지만.
// 카사네, 끝내!
// 카게로의 투명화가 도움이 될 것 같아.
// 굉장해. 깔끔한 솜씨였어.

// 브레인필드
// 1. 설마 나한테 반항 할 생각이야?
// 2. 우후후후후후...!
// 3. 쓰레기 벌레는 구제해야 하지!
// 4. 무슨짓을 해도 소용없어! \n 내 뜻대로 그냥 죽어!
