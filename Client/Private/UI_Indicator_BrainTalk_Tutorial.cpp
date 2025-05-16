#include "ClientPCH.h"
#include "UI_Indicator_BrainTalk_Tutorial.h"
#include "GameInstance.h" 
#include "UI_Manager.h"

CUI_Indicator_BrainTalk_Tutorial::CUI_Indicator_BrainTalk_Tutorial(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_BrainTalk_Tutorial::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_BrainTalk_Tutorial::Initialize(any _aDesc)
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

void CUI_Indicator_BrainTalk_Tutorial::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_BrainTalk_Tutorial::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_5);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_BrainTalk_Tutorial::Render()
{
    if (true == m_bBrainTalk_First || true == m_bBrainTalk_Second || true == m_bBrainTalk_Third || true == m_bBrainTalk_Fourth || true == m_bBrainTalk_Fifth)
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

        if (true == m_bRender_Face_Kasane)
        {
            // BrainTalk_Face_Kasane
            if (FAILED(m_pTexture_Face_Kasane->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
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

HRESULT CUI_Indicator_BrainTalk_Tutorial::Ready_Components()
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

void CUI_Indicator_BrainTalk_Tutorial::Initialize_UI()
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

//================================================================================================ Texture
    m_pTexture_BrainTalkFrame               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_Frame.png"));
    m_pTexture_BrainTalkFrame_Noise         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_FrameNoise.png"));
    m_pTexture_FaceFrame                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/FaceFrame.png"));
    m_pTexture_FaceFrame_Noise              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/FaceFrame_Noise.png"));
    m_pTexture_BrainTalk_LightFrame         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_LightFrame.png"));
    m_pTexture_BrainTalk_LightFrame_Noise   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_LightFrame_Noise.png"));
    m_pTexture_BrainTalk_Light              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Frames/BrainTalk_Light.png"));
    m_pTexture_Face_Kasane                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/BrainTalk/Face/T_ui_ch0200_01.png"));

//================================================================================================ _bool
    m_bBrainTalk_First                      = false;
    m_bBrainTalk_Second                     = false;
    m_bBrainTalk_Third                      = false;
    m_bBrainTalk_Fourth                     = false;
    m_bBrainTalk_Fifth                      = false;

    m_bRender_BTLFN                         = true;
    m_bRender_BTLF                          = false;
    m_bRender_Noise                         = false;
    m_bRender_BTF                           = false;
    m_bRender_Face_Kasane                   = false;
    m_bRender_BTL                           = false;

    m_bDisappear                            = false;

//================================================================================================ _uint
    m_iCheckSoundIndex                      = 0;

//================================================================================================ _float
    m_fBTLFN                                = 0.f;
    m_fBTLF                                 = 0.f;
    m_fNoiseAlpha                           = 0.5f;
    m_fFaceFrame                            = 1.f;
    m_fAccTime                              = 0.f;
    m_fFace_KasaneAlpha                     = 0.f;
    m_fAccTime_Face_Kasane                  = 0.f;
    m_fCheckSoundTime                       = 0.f;
    m_fBTF                                  = 0.f;

//================================================================================================ // Texture_Sprite_Desc
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

//================================================================================================ _float4
    m_vPosition_BrainTalkFrame              = _float4(-610.f, -172.f, 0.f, 1.f);
    m_vPosition_BrainTalkFrame_Noise        = _float4(-610.f, -172.f, 0.f, 1.f);
    m_vPosition_FaceFrame                   = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_FaceFrame_Noise             = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_BrainTalk_LightFrame        = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_BrainTalk_LightFrame_Noise  = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_BrainTalk_Light             = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_Face_Kasane                 = _float4(-830.f, -40.f, 0.f, 1.f);

//================================================================================================ Scale
    m_pBrainTalkFrame->Set_Scale(m_vScale_BrainTalkFrame);
    m_pBrainTalkFrame_Noise->Set_Scale(m_vScale_BrainTalkFrame_Noise);
    m_pFaceFrame->Set_Scale(m_vScale_FaceFrame);
    m_pFaceFrame_Noise->Set_Scale(m_vScale_FaceFrame_Noise);
    m_pBrainTalk_Light->Set_Scale(m_vScale_BrainTalk_Light);
    m_pBrainTalk_LightFrame->Set_Scale(m_vScale_BrainTalk_LightFrame);
    m_pBrainTalk_LightFrame_Noise->Set_Scale(m_vScale_BrainTalk_LightFrame_Noise);
    m_pFace_Kasane->Set_Scale(m_vScale_Face_Kasane);

//================================================================================================ Position
    m_pBrainTalkFrame->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalkFrame);
    m_pBrainTalkFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalkFrame_Noise);
    m_pFaceFrame->Set_State(TRANSFORM::POSITION, m_vPosition_FaceFrame);
    m_pFaceFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_FaceFrame_Noise);
    m_pBrainTalk_Light->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_Light);
    m_pBrainTalk_LightFrame->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_LightFrame);
    m_pBrainTalk_LightFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_LightFrame_Noise);
    m_pFace_Kasane->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Kasane);

//================================================================================================ wstring
    m_strText1                              = L"";
    m_strText2                              = L"";

}

void CUI_Indicator_BrainTalk_Tutorial::Tick_UI(_float fTimeDelta)
{
    //Set_BrainTalk_Test();

    if (false == m_bDisappear)
    {
        // Control Frame
        OpenBrainTalk(fTimeDelta);
        Tick_SpriteTexture(fTimeDelta);

        // Conversation
        Conversation_First(fTimeDelta);
        Conversation_Second(fTimeDelta);
        Conversation_Third(fTimeDelta);
        Conversation_Fourth(fTimeDelta);
        Conversation_Fifth(fTimeDelta);
    }
    else
    {
        BrainTalk_Reset();
    }
}

void CUI_Indicator_BrainTalk_Tutorial::Set_BrainTalk_Test()
{
    if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    {
        Set_BrainTalk(6);
    }
}

void CUI_Indicator_BrainTalk_Tutorial::Set_BrainTalk(_uint iBrainTalkIndex)
{
    m_eBrainTalk = (BRAINTALK)iBrainTalkIndex;

    switch (m_eBrainTalk)
    {
    case Client::CUI_Indicator_BrainTalk_Tutorial::BT_FIRST:
        m_bBrainTalk_First = true;
        break;

    case Client::CUI_Indicator_BrainTalk_Tutorial::BT_SECOND:
        m_bBrainTalk_Second = true;
        break;

    case Client::CUI_Indicator_BrainTalk_Tutorial::BT_THIRD:
        m_bBrainTalk_Third = true;
        break;

    case Client::CUI_Indicator_BrainTalk_Tutorial::BT_FOURTH:
        m_bBrainTalk_Fourth = true;
        break;

    case Client::CUI_Indicator_BrainTalk_Tutorial::BT_FIFTH:
        m_bBrainTalk_Fifth = true;
        break;

    default:
        break;
    }
}

void CUI_Indicator_BrainTalk_Tutorial::OpenBrainTalk(_float fTimeDelta)
{
    if (true == m_bBrainTalk_First || true == m_bBrainTalk_Second || true == m_bBrainTalk_Third || true == m_bBrainTalk_Fourth || true == m_bBrainTalk_Fifth)
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
                m_bRender_Face_Kasane = true;
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
        
        if (true == m_bRender_Face_Kasane)
        {
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
}

void CUI_Indicator_BrainTalk_Tutorial::BrainTalk_Reset()
{
    //================================================================================================ _bool
    m_bBrainTalk_First                      = false;
    m_bBrainTalk_Second                     = false;
    m_bBrainTalk_Third                      = false;
    m_bBrainTalk_Fourth                     = false;
    m_bBrainTalk_Fifth                      = false;

    m_bRender_BTLFN                         = true;
    m_bRender_BTLF                          = false;
    m_bRender_Noise                         = false;
    m_bRender_BTF                           = false;
    m_bRender_Face_Kasane                   = false;
    m_bRender_BTL                           = false;

    m_bDisappear                            = false;

//================================================================================================ _uint
    m_iCheckSoundIndex = 0;

//================================================================================================ _float
    m_fBTLFN                                = 0.f;
    m_fBTLF                                 = 0.f;
    m_fNoiseAlpha                           = 0.5f;
    m_fFaceFrame                            = 1.f;
    m_fAccTime                              = 0.f;
    m_fFace_KasaneAlpha                     = 0.f;
    m_fAccTime_Face_Kasane                  = 0.f;
    m_fCheckSoundTime                       = 0.f;
    m_fBTF                                  = 0.f;

//================================================================================================ // Texture_Sprite_Desc
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

//================================================================================================ _float4
    m_vPosition_BrainTalkFrame              = _float4(-610.f, -172.f, 0.f, 1.f);
    m_vPosition_BrainTalkFrame_Noise        = _float4(-610.f, -172.f, 0.f, 1.f);
    m_vPosition_FaceFrame                   = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_FaceFrame_Noise             = _float4(-830.f, -40.f, 0.f, 1.f);
    m_vPosition_BrainTalk_LightFrame        = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_BrainTalk_LightFrame_Noise  = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_BrainTalk_Light             = _float4(-905.f, -115.f, 0.f, 1.f);
    m_vPosition_Face_Kasane                 = _float4(-830.f, -40.f, 0.f, 1.f);

//================================================================================================ Scale
    m_pBrainTalkFrame->Set_Scale(m_vScale_BrainTalkFrame);
    m_pBrainTalkFrame_Noise->Set_Scale(m_vScale_BrainTalkFrame_Noise);
    m_pFaceFrame->Set_Scale(m_vScale_FaceFrame);
    m_pFaceFrame_Noise->Set_Scale(m_vScale_FaceFrame_Noise);
    m_pBrainTalk_Light->Set_Scale(m_vScale_BrainTalk_Light);
    m_pBrainTalk_LightFrame->Set_Scale(m_vScale_BrainTalk_LightFrame);
    m_pBrainTalk_LightFrame_Noise->Set_Scale(m_vScale_BrainTalk_LightFrame_Noise);
    m_pFace_Kasane->Set_Scale(m_vScale_Face_Kasane);

//================================================================================================ Position
    m_pBrainTalkFrame->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalkFrame);
    m_pBrainTalkFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalkFrame_Noise);
    m_pFaceFrame->Set_State(TRANSFORM::POSITION, m_vPosition_FaceFrame);
    m_pFaceFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_FaceFrame_Noise);
    m_pBrainTalk_Light->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_Light);
    m_pBrainTalk_LightFrame->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_LightFrame);
    m_pBrainTalk_LightFrame_Noise->Set_State(TRANSFORM::POSITION, m_vPosition_BrainTalk_LightFrame_Noise);
    m_pFace_Kasane->Set_State(TRANSFORM::POSITION, m_vPosition_Face_Kasane);

//================================================================================================ wstring
    m_strText1                              = L"";
    m_strText2                              = L"";
}

void CUI_Indicator_BrainTalk_Tutorial::Tick_SpriteTexture(_float fTimeDelta)
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

void CUI_Indicator_BrainTalk_Tutorial::Conversation_First(_float fTimeDelta) // GameStart
{
    if (true == m_bRender_BTF && true == m_bBrainTalk_First)
    {
        m_fCheckSoundTime += fTimeDelta;

        if (0.f <= m_fCheckSoundTime && 6.5f > m_fCheckSoundTime)
        {
            m_strText1 = L"(이 시험으로 배속되는 곳이 결정되는 거구나.";
            m_strText2 = L"언니와 같은 소대가 되면 좋겠는데......)";

            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySoundW(L"Conversation_02.Tutorial_Kasane_Monologue00.wav", 1.f);
                ++m_iCheckSoundIndex;
            }
        }
        //else if (6.8f <= m_fCheckSoundTime && 13.2f > m_fCheckSoundTime)
        //{
        //    m_strText1 = L"(아니, 지금 생각해도 소용없어.";
        //    m_strText2 = L"지금은 집중해야지.)";

        //    if (0 == m_iCheckSoundIndex)
        //    {
        //        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
        //        CGameInstance::Get_Instance()->PlaySoundW(L"Conversation_02.Tutorial_Kasane_Monologue01.wav", 1.f);
        //        ++m_iCheckSoundIndex;
        //    }
        //}
        //else if (13.5f <= m_fCheckSoundTime && 21.f > m_fCheckSoundTime)
        //{
        //    m_strText1 = L"(내 실력이라면, 과제를 완벽하게 클리어할 수";
        //    m_strText2 = L"있을거야. 침착하게 대응만 한다면......)";

        //    if (0 == m_iCheckSoundIndex)
        //    {
        //        CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.BrainTalk_Again.wav", 1.f);
        //        CGameInstance::Get_Instance()->PlaySoundW(L"Conversation_02.Tutorial_Kasane_Monologue02.wav", 1.f);
        //        ++m_iCheckSoundIndex;
        //    }
        //}
        else if (6.5f <= m_fCheckSoundTime && 7.f > m_fCheckSoundTime)/*(21.f <= m_fCheckSoundTime && 21.5f > m_fCheckSoundTime)*/
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
                m_iCheckSoundIndex = 0;
            }
            m_bDisappear = true;
        }

        //if (6.5f <= m_fCheckSoundTime && 6.8f > m_fCheckSoundTime)
        //{
        //    m_iCheckSoundIndex = 0;
        //}
        //else if (13.2f <= m_fCheckSoundTime && 13.5f > m_fCheckSoundTime)
        //{
        //    m_iCheckSoundIndex = 0;
        //}
    }
}

void CUI_Indicator_BrainTalk_Tutorial::Conversation_Second(_float fTimeDelta) // First Meet Monster
{
    if (true == m_bRender_BTF && true == m_bBrainTalk_Second)
    {
        m_fCheckSoundTime += fTimeDelta;

        if (0.f <= m_fCheckSoundTime && 9.f > m_fCheckSoundTime)
        {
            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySoundW(L"Conversation_02.Tutorial_Kasane_Monologue03.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_strText1 = L"(내 초뇌능력, 염력이라면 괴이를 섬멸할 수 있어.";
            m_strText2 = L"전투에 어울린다고 언니도 칭찬해 줬고.)";
        }
        if (9.f <= m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
                m_bDisappear = true;
            }
        }
    }
}

void CUI_Indicator_BrainTalk_Tutorial::Conversation_Third(_float fTimeDelta) // Meet Flies
{
    if (true == m_bRender_BTF && true == m_bBrainTalk_Third)
    {
        m_fCheckSoundTime += fTimeDelta;

        if (0.f <= m_fCheckSoundTime && 8.f > m_fCheckSoundTime)
        {
            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySoundW(L"Conversation_02.Tutorial_Kasane_Monologue04.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_strText1 = L"(언니는 잘하고 있을까.";
            m_strText2 = L"......분명 괜찮을 거야. 언니는 우수하니까.)";
        }
        if (8.f <= m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
                m_bDisappear = true;
            }
        }
    }
}

void CUI_Indicator_BrainTalk_Tutorial::Conversation_Fourth(_float fTimeDelta) // Fly wave Done
{
    if (true == m_bRender_BTF && true == m_bBrainTalk_Fourth)
    {
        m_fCheckSoundTime += fTimeDelta;

        if (0.f <= m_fCheckSoundTime && 8.f > m_fCheckSoundTime)
        {
            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySoundW(L"Conversation_02.Tutorial_Kasane_Monologue05.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_strText1 = L"(시험이 막바지에 접어든 것 같네.";
            m_strText2 = L"괜찮게 해온 것 같지만, 방심은 금물이야.)";
        }
        if (8.f <= m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
                m_bDisappear = true;
            }
        }
    }
}

void CUI_Indicator_BrainTalk_Tutorial::Conversation_Fifth(_float fTimeDelta) // meet TutoBoss
{
    if (true == m_bRender_BTF && true == m_bBrainTalk_Fifth)
    {
        m_fCheckSoundTime += fTimeDelta;

        if (0.f <= m_fCheckSoundTime && 9.5f > m_fCheckSoundTime)
        {
            if (0 == m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_02.Open_BrainTalk.wav", 1.f);
                CGameInstance::Get_Instance()->PlaySoundW(L"Conversation_02.Tutorial_Kasane_Monologue06.wav", 1.f);
                ++m_iCheckSoundIndex;
            }

            m_strText1 = L"(어떤 상대라도, 여태까지의 훈련으로 습득해온 걸";
            m_strText2 = L"다 발휘할 뿐이야. 나라면......이길 수 있어!)";
        }
        if (9.5f <= m_fCheckSoundTime)
        {
            if (0 != m_iCheckSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_01.CloseBrainTalk.wav", 1.f);
                m_bDisappear = true;
            }
        }
    }
}

shared_ptr<CUI_Indicator_BrainTalk_Tutorial> CUI_Indicator_BrainTalk_Tutorial::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_BrainTalk_Tutorial> pInstance = make_private_shared(CUI_Indicator_BrainTalk_Tutorial, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_BrainTalk_Tutorial::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_BrainTalk_Tutorial::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_BrainTalk_Tutorial> pInstance = make_private_shared_copy(CUI_Indicator_BrainTalk_Tutorial, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_BrainTalk_Tutorial::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
