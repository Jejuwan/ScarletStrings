                                                                                                                                                                                  #include "ClientPCH.h"
#include "UI_Menu_Profile_ID_Card.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Player.h"

CUI_Menu_Profile_ID_Card::CUI_Menu_Profile_ID_Card(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Profile_ID_Card::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Profile_ID_Card::Initialize(any _pPlayerEntityDesc)
{   
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category::Initialize", "Failed to CGameObject::Initialize");
    }
    
    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    /* _float */
    m_fHp1      = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
    m_fHpMax1   = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
    m_fHp2      = 860.f;
    m_fHpMax2   = 860.f;
    m_fHp3      = 723.f;
    m_fHpMax3   = 723.f;
    m_fHp4      = 886.f;
    m_fHpMax4   = 886.f;
    m_fHp5      = 943.f;
    m_fHpMax5   = 943.f;


    
    /* Frame */
    m_vScale_Background             = _float3(550.f, 320.f, 1.f);
    m_vPosition_Background          = _float4(570.f, -220.f, 0.5f, 1.f);

    /* FrameUp */
    m_vScale_Frame_Up               = _float3(558.f, 20.f, 1.f);
    m_vPosition_Frame_Up            = _float4(570.f, -66.f, 0.5f, 1.f);

    /* Frame_Dw*/
    m_vScale_Frame_Dw               = _float3(558.f, 20.f, 1.f);
    m_vPosition_Frame_Dw            = _float4(570.f, -373.f, 0.5f, 1.f);

    /* ID_Frame */
    m_vScale_ID_Frame               = _float3(530.f, 300.f, 1.f);
    m_vPosition_ID_Frame            = _float4(570.f, -220.f, 0.5f, 1.f);

    /* Profile Picture */
    m_vScale_Profile1               = _float3(98.f, 98.f, 1.f);
    m_vScale_Profile2               = _float3(98.f, 98.f, 1.f);
    m_vScale_Profile3               = _float3(98.f, 98.f, 1.f);
    m_vScale_Profile4               = _float3(98.f, 98.f, 1.f);
    m_vScale_Profile5               = _float3(98.f, 98.f, 1.f);
    m_vPosition_Profile1            = _float4(360.f, -226.f, 0.4f, 1.f);
    m_vPosition_Profile2            = _float4(360.f, -226.f, 0.4f, 1.f);
    m_vPosition_Profile3            = _float4(360.f, -226.f, 0.4f, 1.f);
    m_vPosition_Profile4            = _float4(360.f, -226.f, 0.4f, 1.f);
    m_vPosition_Profile5            = _float4(360.f, -226.f, 0.4f, 1.f);

    /* SkillIcon */
    m_vScale_SkillIcon1             = _float3(90.f, 90.f, 1.f);
    m_vScale_SkillIcon2             = _float3(90.f, 90.f, 1.f);
    m_vScale_SkillIcon3             = _float3(90.f, 90.f, 1.f);
    m_vScale_SkillIcon4             = _float3(90.f, 90.f, 1.f);
    m_vScale_SkillIcon5             = _float3(90.f, 90.f, 1.f);
    m_vPosition_SkillIcon1          = _float4(759.5f, -182.5f, 0.4f, 1.f);
    m_vPosition_SkillIcon2          = _float4(759.5f, -182.5f, 0.4f, 1.f);
    m_vPosition_SkillIcon3          = _float4(759.5f, -182.5f, 0.4f, 1.f);
    m_vPosition_SkillIcon4          = _float4(759.5f, -182.5f, 0.4f, 1.f);
    m_vPosition_SkillIcon5          = _float4(759.5f, -182.5f, 0.4f, 1.f);

    /* SkillTag */
    m_vScale_SkillTag               = _float3(155.f, 25.f, 1.f);
    m_vPosition_SkillTag            = _float4(758.f, -260.f, 0.4f, 1.f);

    /* LoveIcon */
    m_vScale_LoveIcon               = _float3(70.f, 42.f, 1.f);
    m_vPosition_LoveIcon            = _float4(635.f, -132.f, 0.4f, 1.f);

    /* LoveIndex */
    m_vScale_LoveIndex1             = _float3(12.f, 5.f, 1.f);
    m_vScale_LoveIndex2             = _float3(11.f, 8.f, 1.f);
    m_vScale_LoveIndex3             = _float3(11.f, 8.f, 1.f);
    m_vScale_LoveIndex4             = _float3(11.f, 8.f, 1.f);
    m_vScale_LoveIndex5             = _float3(11.f, 8.f, 1.f);
    m_vPosition_LoveIndex1          = _float4(655.f, -132.f, 0.4f, 1.f);
    m_vPosition_LoveIndex2          = _float4(655.f, -132.f, 0.4f, 1.f);
    m_vPosition_LoveIndex3          = _float4(655.f, -132.f, 0.4f, 1.f);
    m_vPosition_LoveIndex4          = _float4(655.f, -132.f, 0.4f, 1.f);
    m_vPosition_LoveIndex5          = _float4(655.f, -132.f, 0.4f, 1.f);

    /* EXP */
    m_vScale_EXP                    = _float3(280.f, 7.f, 1.f);
    m_vPosition_EXP                 = _float4(448.f, -150.f, 0.4f, 1.f);


    /* ProfileHp */
    m_vScale_ProfileHpBackground    = _float3(218.f, 10.f, 1.f);
    m_vPosition_ProfileHpBackground = _float4(564.f, -190.f, 0.4f, 1.f);
    m_vScale_ProfileHp1             = _float3(218.f, 10.f, 1.f);
    m_vPosition_ProfileHp1          = _float4(564.f, -190.f, 0.4f, 1.f);
    m_vScale_ProfileHp2             = _float3(218.f, 10.f, 1.f);
    m_vPosition_ProfileHp2          = _float4(564.f, -190.f, 0.4f, 1.f);
    m_vScale_ProfileHp3             = _float3(218.f, 10.f, 1.f);
    m_vPosition_ProfileHp3          = _float4(564.f, -190.f, 0.4f, 1.f);
    m_vScale_ProfileHp4             = _float3(218.f, 10.f, 1.f);
    m_vPosition_ProfileHp4          = _float4(564.f, -190.f, 0.4f, 1.f);
    m_vScale_ProfileHp5             = _float3(218.f, 10.f, 1.f);
    m_vPosition_ProfileHp5          = _float4(564.f, -190.f, 0.4f, 1.f);

    /* TransformCom */
    m_pBackground                   = CTransform::Create(m_pDevice, m_pContext);
    m_pFrameUp                      = CTransform::Create(m_pDevice, m_pContext);
    m_pFrameDw                      = CTransform::Create(m_pDevice, m_pContext);
    m_pID_Frame                     = CTransform::Create(m_pDevice, m_pContext);
    m_pProfile1                     = CTransform::Create(m_pDevice, m_pContext);
    m_pProfile2                     = CTransform::Create(m_pDevice, m_pContext);
    m_pProfile3                     = CTransform::Create(m_pDevice, m_pContext);
    m_pProfile4                     = CTransform::Create(m_pDevice, m_pContext);
    m_pProfile5                     = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillIcon1                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillIcon2                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillIcon3                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillIcon4                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillIcon5                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillTag                     = CTransform::Create(m_pDevice, m_pContext);
    m_pLoveIcon                     = CTransform::Create(m_pDevice, m_pContext);
    m_pLoveIndex1                   = CTransform::Create(m_pDevice, m_pContext);
    m_pLoveIndex2                   = CTransform::Create(m_pDevice, m_pContext);
    m_pLoveIndex3                   = CTransform::Create(m_pDevice, m_pContext);
    m_pLoveIndex4                   = CTransform::Create(m_pDevice, m_pContext);
    m_pLoveIndex5                   = CTransform::Create(m_pDevice, m_pContext);
    m_pEXP                          = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileHp1                   = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileHp2                   = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileHp3                   = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileHp4                   = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileHp5                   = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileHpBackground          = CTransform::Create(m_pDevice, m_pContext);



    /* Texture */
    m_pTexture_Background           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png"));
    m_pTexture_FrameUp              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/FrameUp.png"));
    m_pTexture_FrameDw              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/FrameDw.png"));
    m_pTexture_ID_Frame             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ID.dds"));
    m_pTexture_Profile1             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Kasane.png"));
    m_pTexture_Profile2             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Siden.png"));
    m_pTexture_Profile3             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arasi.png"));
    m_pTexture_Profile4             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Kyoca.png"));
    m_pTexture_Profile5             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Kagero.png"));
    m_pTexture_SkillIcon1           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Icon_Psyco.png"));
    m_pTexture_SkillIcon2           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Icon_Electric.png"));
    m_pTexture_SkillIcon3           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Icon_Acceleration.png"));
    m_pTexture_SkillIcon4           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Icon_Clone.png"));
    m_pTexture_SkillIcon5           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Icon_Stealth.png"));
    m_pTexture_SkillTag             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/SkillTag.png"));
    m_pTexture_LoveIcon             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/LoveIcon.png"));
    m_pTexture_LoveIndex1           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/None.png"));
    m_pTexture_LoveIndex2           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/11.png"));
    m_pTexture_LoveIndex3           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/2.png"));
    m_pTexture_LoveIndex4           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/33.png"));
    m_pTexture_LoveIndex5           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/6.png"));
    m_pTexture_EXP                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/EXP.png"));
    m_pTexture_ProfileHp1           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/HP.png"));
    m_pTexture_ProfileHp2           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/HP.png"));
    m_pTexture_ProfileHp3           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/HP.png"));
    m_pTexture_ProfileHp4           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/HP.png"));
    m_pTexture_ProfileHp5           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/HP.png"));
    m_pTexture_ProfileHpBackground  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Monster_HpBackground.png"));

    /* Set_Scale */
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pFrameUp->Set_Scale(m_vScale_Frame_Up);
    m_pFrameDw->Set_Scale(m_vScale_Frame_Dw);
    m_pID_Frame->Set_Scale(m_vScale_ID_Frame);
    m_pProfile1->Set_Scale(m_vScale_Profile1);
    m_pProfile2->Set_Scale(m_vScale_Profile2);
    m_pProfile3->Set_Scale(m_vScale_Profile3);
    m_pProfile4->Set_Scale(m_vScale_Profile4);
    m_pProfile5->Set_Scale(m_vScale_Profile5);
    m_pSkillIcon1->Set_Scale(m_vScale_SkillIcon1);
    m_pSkillIcon2->Set_Scale(m_vScale_SkillIcon2);
    m_pSkillIcon3->Set_Scale(m_vScale_SkillIcon3);
    m_pSkillIcon4->Set_Scale(m_vScale_SkillIcon4);
    m_pSkillIcon5->Set_Scale(m_vScale_SkillIcon5);
    m_pSkillTag->Set_Scale(m_vScale_SkillTag);
    m_pLoveIcon->Set_Scale(m_vScale_LoveIcon);
    m_pLoveIndex1->Set_Scale(m_vScale_LoveIndex1);
    m_pLoveIndex2->Set_Scale(m_vScale_LoveIndex2);
    m_pLoveIndex3->Set_Scale(m_vScale_LoveIndex3);
    m_pLoveIndex4->Set_Scale(m_vScale_LoveIndex4);
    m_pLoveIndex5->Set_Scale(m_vScale_LoveIndex5);
    m_pEXP->Set_Scale(m_vScale_EXP);
    m_pProfileHp1->Set_Scale(m_vScale_ProfileHp1);
    m_pProfileHp2->Set_Scale(m_vScale_ProfileHp2);
    m_pProfileHp3->Set_Scale(m_vScale_ProfileHp3);
    m_pProfileHp4->Set_Scale(m_vScale_ProfileHp4);
    m_pProfileHp5->Set_Scale(m_vScale_ProfileHp5);
    m_pProfileHpBackground->Set_Scale(m_vScale_ProfileHpBackground);


    /* Set_Position */
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pFrameUp->Set_State(TRANSFORM::POSITION, m_vPosition_Frame_Up);
    m_pFrameDw->Set_State(TRANSFORM::POSITION, m_vPosition_Frame_Dw);
    m_pID_Frame->Set_State(TRANSFORM::POSITION, m_vPosition_ID_Frame);
    m_pProfile1->Set_State(TRANSFORM::POSITION, m_vPosition_Profile1);
    m_pProfile2->Set_State(TRANSFORM::POSITION, m_vPosition_Profile2);
    m_pProfile3->Set_State(TRANSFORM::POSITION, m_vPosition_Profile3);
    m_pProfile4->Set_State(TRANSFORM::POSITION, m_vPosition_Profile4);
    m_pProfile5->Set_State(TRANSFORM::POSITION, m_vPosition_Profile5);
    m_pSkillIcon1->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIcon1);
    m_pSkillIcon2->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIcon2);
    m_pSkillIcon3->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIcon3);
    m_pSkillIcon4->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIcon4);
    m_pSkillIcon5->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIcon5);
    m_pSkillTag->Set_State(TRANSFORM::POSITION, m_vPosition_SkillTag);
    m_pLoveIcon->Set_State(TRANSFORM::POSITION, m_vPosition_LoveIcon);
    m_pLoveIndex1->Set_State(TRANSFORM::POSITION, m_vPosition_LoveIndex1);
    m_pLoveIndex2->Set_State(TRANSFORM::POSITION, m_vPosition_LoveIndex2);
    m_pLoveIndex3->Set_State(TRANSFORM::POSITION, m_vPosition_LoveIndex3);
    m_pLoveIndex4->Set_State(TRANSFORM::POSITION, m_vPosition_LoveIndex4);
    m_pLoveIndex5->Set_State(TRANSFORM::POSITION, m_vPosition_LoveIndex5);
    m_pEXP->Set_State(TRANSFORM::POSITION, m_vPosition_EXP);
    m_pProfileHp1->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp1);
    m_pProfileHp2->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp2);
    m_pProfileHp3->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp3);
    m_pProfileHp4->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp4);
    m_pProfileHp5->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp5);
    m_pProfileHpBackground->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHpBackground);

    m_strHp.resize(MAX_PATH);
    m_strHpMax.resize(MAX_PATH);


    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Profile_ID_Card::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


}

void CUI_Menu_Profile_ID_Card::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_3);

    Tick_UI();
}

HRESULT CUI_Menu_Profile_ID_Card::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (0 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            /* Background */
            if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
            }

            /* FrameUp */
            if (FAILED(m_pTexture_FrameUp->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pFrameUp->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
            }

            /* FrameDw */
            if (FAILED(m_pTexture_FrameDw->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pFrameDw->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
            }

            /* ID_Frame */
            if (FAILED(m_pTexture_ID_Frame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pID_Frame->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
            }

            /* Skill_Tag */
            if (FAILED(m_pTexture_SkillTag->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillTag->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
            }

            /* Love_Icon */
            if (FAILED(m_pTexture_LoveIcon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pLoveIcon->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
            }

            /* EXP */
            if (FAILED(m_pTexture_EXP->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pEXP->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
            }

            /* HP - Background */
            if (FAILED(m_pTexture_ProfileHpBackground->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pProfileHpBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
            }


            if (0 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex() && m_fHp1 > 0.f)
            {
                /* HP */
                if (FAILED(m_pTexture_ProfileHp1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileHp1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }
            }
            if (1 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* HP */
                if (FAILED(m_pTexture_ProfileHp2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileHp2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }
            }
            if (2 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* HP */
                if (FAILED(m_pTexture_ProfileHp2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileHp2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }
            }
            if (3 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* HP */
                if (FAILED(m_pTexture_ProfileHp3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileHp3->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }
            }
            if (4 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* HP */
                if (FAILED(m_pTexture_ProfileHp4->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileHp4->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }
            }
            if (5 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* HP */
                if (FAILED(m_pTexture_ProfileHp5->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileHp5->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }
            }

            m_strLevelTag       = TEXT("레벨");
            m_strFromNowOnTag   = TEXT("앞으로");
            m_strHpTag          = TEXT("체력");
            m_strHpSeperate     = TEXT("/");
            m_strAPTag          = TEXT("초뇌능력");
            m_strAtkTag         = TEXT("공격력");
            m_strDefTag         = TEXT("방어력");
            m_strOperation      = TEXT("작전");
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelTag,      _float2(1270.f, 665.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFromNowOnTag,  _float2(1407.f, 665.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpTag,         _float2(1413.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpSeperate,    _float2(1583.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAPTag,         _float2(1413.f, 751.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAtkTag,        _float2(1413.f, 781.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strDefTag,        _float2(1413.f, 811.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strOperation,     _float2(1270.f, 844.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);


            if (0 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* String */
                m_strName           = TEXT("카사네 랜들");
                m_iLevel            = 4;
                m_strLevelIndex     = std::to_wstring(m_iLevel);
                m_iFromNowOn        = 2;
                m_strFromNowOnIndex = std::to_wstring(m_iFromNowOn);
                m_fHp1               = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
                if (m_fHp1 <= 0.f)
                    m_fHp1 = 0.f;
                m_strHp             = std::to_wstring((_uint)m_fHp1);
                /*m_fHpMax            = 800;*/
                m_strHpMax          = std::to_wstring((_uint)m_fHpMax1);
                m_iAp               = 76;
                m_strAp             = std::to_wstring(m_iAp);
                m_iAtk              = 71;
                m_strAtk            = std::to_wstring(m_iAtk);
                m_iDef              = 29;
                m_strDef            = std::to_wstring(m_iDef);
                m_strPsycoTag       = TEXT("염력");
                m_strOperations = TEXT("----");
                m_strIDNumber = TEXT("LRK567301");

                
                /* Profile Picture */
                if (FAILED(m_pTexture_Profile1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfile1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                /* SkillIcon */
                if (FAILED(m_pTexture_SkillIcon1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcon1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                /* Love_Index */
                if (FAILED(m_pTexture_LoveIndex1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIndex1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strName,          _float2(1270.f, 612.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelIndex,    _float2(1315.f, 660.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.58f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFromNowOnIndex,_float2(1530.f, 667.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHp,            _float2(1547.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpMax,         _float2(1600.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAp,            _float2(1608.f, 751.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAtk,           _float2(1608.f, 781.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strDef,           _float2(1608.f, 811.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPsycoTag,      _float2(1697.f, 791.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.45f);

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strOperations, _float2(1522.f, 878.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strIDNumber, _float2(1704.f, 614.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);

            }
            if (1 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* String */
                m_strName           = TEXT("시덴 리터");
                m_iLevel            = 4;
                m_strLevelIndex     = std::to_wstring(m_iLevel);
                m_iFromNowOn        = 2;
                m_strFromNowOnIndex = std::to_wstring(m_iFromNowOn);
                m_fHp2              = 860;
                if (m_fHp2 <= 0.f)
                    m_fHp2 = 0.f;
                m_strHp             = std::to_wstring((_uint)m_fHp2);
                m_fHpMax2           = 860;
                m_strHpMax          = std::to_wstring((_uint)m_fHpMax2);
                m_iAp               = 76;
                m_strAp             = std::to_wstring(m_iAp);
                m_iAtk              = 71;
                m_strAtk            = std::to_wstring(m_iAtk);
                m_iDef              = 29;
                m_strDef            = std::to_wstring(m_iDef);
                m_strPsycoTag       = TEXT("방전");
                m_strIDNumber = TEXT("SRS553101");
                m_strOperations = TEXT("자유롭게 싸워라");

                /* Profile Picture */
                if (FAILED(m_pTexture_Profile2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfile2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }


                /* SkillIcon */
                if (FAILED(m_pTexture_SkillIcon2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcon2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                /* Love_Index */
                if (FAILED(m_pTexture_LoveIndex2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIndex2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strName,          _float2(1270.f, 612.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelIndex,    _float2(1315.f, 660.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.58f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFromNowOnIndex,_float2(1530.f, 667.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHp,            _float2(1547.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpMax,         _float2(1600.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAp,            _float2(1608.f, 751.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAtk,           _float2(1608.f, 781.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strDef,           _float2(1608.f, 811.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPsycoTag,      _float2(1697.f, 791.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.45f);

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strOperations, _float2(1467.f, 878.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strIDNumber, _float2(1704.f, 614.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);

            }
            if (2 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* String */
                m_strName           = TEXT("아라시 스프링");
                m_iLevel            = 4;
                m_strLevelIndex     = std::to_wstring(m_iLevel);
                m_iFromNowOn        = 2;
                m_strFromNowOnIndex = std::to_wstring(m_iFromNowOn);
                m_fHp3               = 723;
                if (m_fHp3 <= 0.f)
                    m_fHp3 = 0.f;
                m_strHp             = std::to_wstring((_uint)m_fHp3);
                m_fHpMax3            = 723;
                m_strHpMax          = std::to_wstring((_uint)m_fHpMax3);
                m_iAp               = 76;
                m_strAp             = std::to_wstring(m_iAp);
                m_iAtk              = 71;
                m_strAtk            = std::to_wstring(m_iAtk);
                m_iDef              = 29;
                m_strDef            = std::to_wstring(m_iDef);
                m_strPsycoTag       = TEXT("초고속");
                m_strIDNumber = TEXT("NSA538201");
                m_strOperations = TEXT("무리하지 마라");

                /* Profile Picture */
                if (FAILED(m_pTexture_Profile3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfile3->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }


                /* SkillIcon */
                if (FAILED(m_pTexture_SkillIcon3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcon3->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                /* Love_Index */
                if (FAILED(m_pTexture_LoveIndex3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIndex3->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strName,          _float2(1270.f, 612.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelIndex,    _float2(1315.f, 660.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.58f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFromNowOnIndex,_float2(1530.f, 667.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHp,            _float2(1547.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpMax,         _float2(1600.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAp,            _float2(1608.f, 751.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAtk,           _float2(1608.f, 781.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strDef,           _float2(1608.f, 811.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPsycoTag,      _float2(1687.f, 791.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.45f);

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strOperations, _float2(1467.f, 878.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strIDNumber, _float2(1704.f, 614.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);

            }
            if (3 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* String */
                m_strName           = TEXT("쿄카 에덴");
                m_iLevel            = 4;
                m_strLevelIndex     = std::to_wstring(m_iLevel);
                m_iFromNowOn        = 2;
                m_strFromNowOnIndex = std::to_wstring(m_iFromNowOn);
                m_fHp4               = 886;
                if (m_fHp4 <= 0.f)
                    m_fHp4 = 0.f;
                m_strHp             = std::to_wstring((_uint)m_fHp4);
                m_fHpMax4            = 886;
                m_strHpMax          = std::to_wstring((_uint)m_fHpMax4);
                m_iAp               = 76;
                m_strAp             = std::to_wstring(m_iAp);
                m_iAtk              = 71;
                m_strAtk            = std::to_wstring(m_iAtk);
                m_iDef              = 29;
                m_strDef            = std::to_wstring(m_iDef);
                m_strPsycoTag       = TEXT("복제");
                m_strIDNumber = TEXT("KEK538231");
                m_strOperations = TEXT("무리하지 마라");

                /* Profile Picture */
                if (FAILED(m_pTexture_Profile4->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfile4->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }


                /* SkillIcon */
                if (FAILED(m_pTexture_SkillIcon4->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcon4->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                /* Love_Index */
                if (FAILED(m_pTexture_LoveIndex4->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIndex4->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }


                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strName,          _float2(1270.f, 612.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelIndex,    _float2(1315.f, 660.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.58f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFromNowOnIndex,_float2(1530.f, 667.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHp,            _float2(1547.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpMax,         _float2(1600.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAp,            _float2(1608.f, 751.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAtk,           _float2(1608.f, 781.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strDef,           _float2(1608.f, 811.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPsycoTag,      _float2(1697.f, 791.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.45f);


                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strOperations, _float2(1467.f, 878.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strIDNumber, _float2(1704.f, 614.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            }
            if (4 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                /* String */
                m_strName           = TEXT("카게로 던");
                m_iLevel            = 4;
                m_strLevelIndex     = std::to_wstring(m_iLevel);
                m_iFromNowOn        = 2;
                m_strFromNowOnIndex = std::to_wstring(m_iFromNowOn);
                m_fHp5               = 943;
                if (m_fHp5 <= 0.f)
                    m_fHp5 = 0.f;
                m_strHp             = std::to_wstring((_uint)m_fHp5);
                m_fHpMax5            = 943;
                m_strHpMax          = std::to_wstring((_uint)m_fHpMax5);
                m_iAp               = 76;
                m_strAp             = std::to_wstring(m_iAp);
                m_iAtk              = 71;
                m_strAtk            = std::to_wstring(m_iAtk);
                m_iDef              = 29;
                m_strDef            = std::to_wstring(m_iDef);
                m_strPsycoTag       = TEXT("투명화");

                m_strIDNumber = TEXT("IDK555265");
                m_strOperations = TEXT("한계까지 싸워라");

                /* Profile Picture */
                if (FAILED(m_pTexture_Profile5->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfile5->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }


                /* SkillIcon */
                if (FAILED(m_pTexture_SkillIcon5->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcon5->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                /* Love_Index */
                if (FAILED(m_pTexture_LoveIndex5->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIndex5->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strName,          _float2(1270.f, 612.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelIndex,    _float2(1315.f, 660.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.58f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFromNowOnIndex,_float2(1530.f, 667.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHp,            _float2(1547.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpMax,         _float2(1600.f, 705.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAp,            _float2(1608.f, 751.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strAtk,           _float2(1608.f, 781.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strDef,           _float2(1608.f, 811.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPsycoTag,      _float2(1687.f, 791.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strOperations,     _float2(1535.f, 991.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.45f);


                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strOperations, _float2(1467.f, 878.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.45f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strIDNumber, _float2(1704.f, 614.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.35f);
            }

        }
    }

    return S_OK;
}

HRESULT CUI_Menu_Profile_ID_Card::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Profile_ID_Card::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Menu_Profile_ID_Card::KeyWorks()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (0 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            //if (0 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            //{
            //    m_pBackground->Set_Scale(m_vScaleOn);
            //    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPositionOn);

            //    m_pProfileFrame->Set_Scale(m_vScale_ProfileFrameOn);
            //    m_pProfileFrame->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameOn);

            //    m_pProfileIndex->Set_Scale(m_vScale_ProfileIndexOn);
            //    m_pProfileIndex->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileIndexOn);

            //    m_pLEADER->Set_Scale(m_vScale_LEADEROn);
            //    m_pLEADER->Set_State(TRANSFORM::POSITION, m_vPosition_LEADEROn);

            //    m_pProfile->Set_Scale(m_vScale_ProfileOn);
            //    m_pProfile->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileOn);

            //    m_pSkillIcon->Set_Scale(m_vScale_SkillIconOn);
            //    m_pSkillIcon->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIconOn);

            //    m_pSkillTag->Set_Scale(m_vScale_SkillTagOn);
            //    m_pSkillTag->Set_State(TRANSFORM::POSITION, m_vPosition_SkillTagOn);
            //}
            //else
            //{
            //    m_pBackground->Set_Scale(m_vScaleOff);
            //    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPositionOff);

            //    m_pProfileFrame->Set_Scale(m_vScale_ProfileFrameOff);
            //    m_pProfileFrame->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameOff);
            //
            //    m_pProfileIndex->Set_Scale(m_vScale_ProfileIndexOff);
            //    m_pProfileIndex->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileIndexOff);

            //    m_pLEADER->Set_Scale(m_vScale_LEADEROff);
            //    m_pLEADER->Set_State(TRANSFORM::POSITION, m_vPosition_LEADEROff);

            //    m_pProfile->Set_Scale(m_vScale_ProfileOff);
            //    m_pProfile->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileOff);

            //    m_pSkillIcon->Set_Scale(m_vScale_SkillIconOff);
            //    m_pSkillIcon->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIconOff);

            //    m_pSkillTag->Set_Scale(m_vScale_SkillTagOff);
            //    m_pSkillTag->Set_State(TRANSFORM::POSITION, m_vPosition_SkillTagOff);
            //}
        }
    }
}

void CUI_Menu_Profile_ID_Card::Tick_UI()
{
    Set_HPPosition();
}

void CUI_Menu_Profile_ID_Card::Set_HPPosition()
{
    if (0 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
    {
        m_fHp1 = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;

        if (m_fHp1 <= 0.f)
            m_fHp1 = 0.f;

        m_fRatio1 = (_uint)m_fHp1 / m_fHpMax1;
        m_fPercentage1 = m_fRatio1 * 218.f;

        if (0 < m_fHp1)
        {
            /* Situation :: When Player Damaged */

            m_vScale_ProfileHp1.x = m_fPercentage1; // Set CurrentHP_Scale
            m_pProfileHp1->Set_Scale(m_vScale_ProfileHp1);

            m_vPosition_ProfileHp1.x = 564.f - 218.f * (1.f - m_fRatio1) * 0.5f; // Set CurrentHP_Position
            m_pProfileHp1->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp1);
        }
    }
    //if (1 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
    //{
    //    m_fHp2 = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;

    //    m_fRatio2 = m_fHp2 / m_fHpMax2;
    //    m_fPercentage2 = m_fRatio2 * 218.f;

    //    if (0 < m_fHp2)
    //    {
    //        /* Situation :: When Player Damaged */

    //        m_vScale_ProfileHp2.x = m_fPercentage2; // Set CurrentHP_Scale
    //        m_pProfileHp2->Set_Scale(m_vScale_ProfileHp2);

    //        m_vPosition_ProfileHp2.x = 564.f - 218.f * (1.f - m_fRatio2) * 0.5f; // Set CurrentHP_Position
    //        m_pProfileHp2->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp2);
    //    }
    //}
    //if (2 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
    //{
    //    m_fHp3 = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;

    //    m_fRatio3 = m_fHp3 / m_fHpMax3;
    //    m_fPercentage3 = m_fRatio3 * 218.f;

    //    if (0 < m_fHp3)
    //    {
    //        /* Situation :: When Player Damaged */

    //        m_vScale_ProfileHp3.x = m_fPercentage3; // Set CurrentHP_Scale
    //        m_pProfileHp3->Set_Scale(m_vScale_ProfileHp3);

    //        m_vPosition_ProfileHp3.x = 564.f - 218.f * (1.f - m_fRatio3) * 0.5f; // Set CurrentHP_Position
    //        m_pProfileHp3->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp3);
    //    }
    //}
    //if (3 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
    //{
    //    m_fHp4 = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;

    //    m_fRatio4 = m_fHp4 / m_fHpMax4;
    //    m_fPercentage4 = m_fRatio4 * 218.f;

    //    if (0 < m_fHp4)
    //    {
    //        /* Situation :: When Player Damaged */

    //        m_vScale_ProfileHp4.x = m_fPercentage4; // Set CurrentHP_Scale
    //        m_pProfileHp3->Set_Scale(m_vScale_ProfileHp4);

    //        m_vPosition_ProfileHp4.x = 564.f - 218.f * (1.f - m_fRatio4) * 0.5f; // Set CurrentHP_Position
    //        m_pProfileHp4->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp4);
    //    }
    //}
    //if (4 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
    //{
    //    m_fHp5 = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;

    //    m_fRatio5 = m_fHp5 / m_fHpMax5;
    //    m_fPercentage5 = m_fRatio5 * 218.f;

    //    if (0 < m_fHp5)
    //    {
    //        /* Situation :: When Player Damaged */

    //        m_vScale_ProfileHp5.x = m_fPercentage5; // Set CurrentHP_Scale
    //        m_pProfileHp3->Set_Scale(m_vScale_ProfileHp5);

    //        m_vPosition_ProfileHp5.x = 564.f - 218.f * (1.f - m_fRatio5) * 0.5f; // Set CurrentHP_Position
    //        m_pProfileHp5->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHp5);
    //    }
    //}
}

shared_ptr<CUI_Menu_Profile_ID_Card> CUI_Menu_Profile_ID_Card::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Profile_ID_Card> pInstance = make_private_shared(CUI_Menu_Profile_ID_Card, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Profile_ID_Card::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Profile_ID_Card::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Profile_ID_Card> pInstance = make_private_shared_copy(CUI_Menu_Profile_ID_Card, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Profile_ID_Card::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
