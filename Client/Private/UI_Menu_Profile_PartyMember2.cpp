#include "ClientPCH.h"
#include "UI_Menu_Profile_PartyMember2.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "MenuCharacter.h"

CUI_Menu_Profile_PartyMember2::CUI_Menu_Profile_PartyMember2(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Profile_PartyMember2::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Profile_PartyMember2::Initialize(any _pPlayerEntityDesc)
{   
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category::Initialize", "Failed to CGameObject::Initialize");
    }
    
    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    

    /* Profile */
    m_vScaleOn                  = _float3(502.f, 143.f, 1.f);
    m_vScaleOff                 = _float3(500.f, 140.f, 1.f);
    m_vPositionOn               = _float4(-670.f, 19.f, 0.5f, 1.f);
    m_vPositionOff              = _float4(-670.f, 20.f, 0.5f, 1.f);


    /* Profile Frame */
    m_vScale_ProfileFrameOn     = _float3(315.f, 90.f, 1.f);
    m_vScale_ProfileFrameOff    = _float3(315.f, 90.f, 1.f);
    m_vPosition_ProfileFrameOn  = _float4(-680.f, 20.f, 0.4f, 1.f);
    m_vPosition_ProfileFrameOff = _float4(-680.f, 20.f, 0.4f, 1.f);


    /* Profile Index */
    m_vScale_ProfileIndexOn     = _float3(34.f, 34.f, 1.f);
    m_vScale_ProfileIndexOff    = _float3(34.f, 34.f, 1.f);
    m_vPosition_ProfileIndexOn  = _float4(-874.f, 54.f, 0.4f, 1.f);
    m_vPosition_ProfileIndexOff = _float4(-874.f, 54.f, 0.4f, 1.f);


    /* MEMBER */
    m_vScale_MEMBEROn           = _float3(40.f, 7.f, 1.f);
    m_vScale_MEMBEROff          = _float3(40.f, 7.f, 1.f);
    m_vPosition_MEMBEROn        = _float4(-873.f, 22.f, 0.4f, 1.f);
    m_vPosition_MEMBEROff       = _float4(-873.f, 22.f, 0.4f, 1.f);



    /* Profile */
    m_vScale_ProfileOn          = _float3(82.f, 82.f, 1.f);
    m_vScale_ProfileOff         = _float3(82.f, 82.f, 1.f);
    m_vPosition_ProfileOn       = _float4(-793.f, 22.f, 0.4f, 1.f);
    m_vPosition_ProfileOff      = _float4(-793.f, 22.f, 0.4f, 1.f);


    /* SkillIconFrame */
    m_vScale_SkillIconFrameOn       = _float3(64.f, 64.f, 1.f);
    m_vScale_SkillIconFrameOff      = _float3(64.f, 64.f, 1.f);
    m_vPosition_SkillIconFrameOn    = _float4(-470.f, 36.f, 0.4f, 1.f);
    m_vPosition_SkillIconFrameOff   = _float4(-470.f, 36.f, 0.4f, 1.f);


    /* SkillIcon */
    m_vScale_SkillIconOn            = _float3(52.f, 52.f, 1.f);
    m_vScale_SkillIconOff           = _float3(52.f, 52.f, 1.f);
    m_vPosition_SkillIconOn         = _float4(-470.f, 36.f, 0.4f, 1.f);
    m_vPosition_SkillIconOff        = _float4(-470.f, 36.f, 0.4f, 1.f);


    /* SkillTag */
    m_vScale_SkillTagOn             = _float3(95.f, 20.f, 1.f);
    m_vScale_SkillTagOff            = _float3(95.f, 18.f, 1.f);
    m_vPosition_SkillTagOn          = _float4(-474.f, -20.f, 0.4f, 1.f);
    m_vPosition_SkillTagOff         = _float4(-474.f, -20.f, 0.4f, 1.f);


    /* LoveIcon */
    m_vScale_LoveIconOn             = _float3(64.f, 28.f, 1.f);
    m_vScale_LoveIconOff            = _float3(64.f, 28.f, 1.f);
    m_vPosition_LoveIconOn          = _float4(-553.f, 23.f, 0.4f, 1.f);
    m_vPosition_LoveIconOff         = _float4(-553.f, 23.f, 0.4f, 1.f);


    /* LoveIndex */
    m_vScale_LoveIndexOn            = _float3(7.f, 9.f, 1.f);
    m_vScale_LoveIndexOff           = _float3(7.f, 9.f, 1.f);
    m_vPosition_LoveIndexOn         = _float4(-537.f, 22.f, 0.4f, 1.f);
    m_vPosition_LoveIndexOff        = _float4(-537.f, 22.f, 0.4f, 1.f);


    /* ProfileHp */
    m_vScale_ProfileHpOn            = _float3(220.f, 7.f, 1.f);
    m_vScale_ProfileHpOff           = _float3(220.f, 7.f, 1.f);
    m_vPosition_ProfileHpOn         = _float4(-635.f, -20.f, 0.4f, 1.f);
    m_vPosition_ProfileHpOff        = _float4(-635.f, -20.f, 0.4f, 1.f);


    /* Transform */
    m_pBackground                   = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileFrame                 = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileIndex                 = CTransform::Create(m_pDevice, m_pContext);
    m_pMEMBER                       = CTransform::Create(m_pDevice, m_pContext);
    m_pProfile                      = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillIconFrame               = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillIcon                    = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillTag                     = CTransform::Create(m_pDevice, m_pContext);
    m_pLoveIcon                     = CTransform::Create(m_pDevice, m_pContext);
    m_pLoveIndex                    = CTransform::Create(m_pDevice, m_pContext);
    m_pProfileHp                    = CTransform::Create(m_pDevice, m_pContext);


    /* Texture */
    m_pTexture_On                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOn.png"));
    m_pTexture_Off                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileOff.png"));
    m_pTexture_ProfileFrame         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Profile_Frame.dds"));
    m_pTexture_ProfileIndex         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/03.png"));
    m_pTexture_MEMBER               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MEMBER.png"));
    m_pTexture_Profile              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arasi.png"));
    m_pTexture_SkillIconFrame       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/SkillIconFrame.png"));
    m_pTexture_SkillIcon            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Icon_Acceleration.png"));
    m_pTexture_SkillTag             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/SkillTag.png"));
    m_pTexture_LoveIcon             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/LoveIcon.png"));
    m_pTexture_LoveIndex            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/2.png"));
    m_pTexture_ProfileHp            = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/ProfileHp.png"));


    /* Scale */
    m_pBackground->Set_Scale(m_vScaleOn);
    m_pProfileFrame->Set_Scale(m_vScale_ProfileFrameOn);
    m_pProfileIndex->Set_Scale(m_vScale_ProfileIndexOff);
    m_pMEMBER->Set_Scale(m_vScale_MEMBEROff);
    m_pProfile->Set_Scale(m_vScale_ProfileOff);
    m_pSkillIconFrame->Set_Scale(m_vScale_SkillIconFrameOff);
    m_pSkillIcon->Set_Scale(m_vScale_SkillIconOff);
    m_pSkillTag->Set_Scale(m_vScale_SkillTagOff);
    m_pLoveIcon->Set_Scale(m_vScale_LoveIconOff);
    m_pLoveIndex->Set_Scale(m_vScale_LoveIndexOff);
    m_pProfileHp->Set_Scale(m_vScale_ProfileHpOn);


    /* Position */
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPositionOff);
    m_pProfileFrame->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameOff);
    m_pProfileIndex->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileIndexOff);
    m_pMEMBER->Set_State(TRANSFORM::POSITION, m_vPosition_MEMBEROff);
    m_pProfile->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileOff);
    m_pSkillIconFrame->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIconFrameOff);
    m_pSkillIcon->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIconOff);
    m_pSkillTag->Set_State(TRANSFORM::POSITION, m_vPosition_SkillTagOff);
    m_pLoveIcon->Set_State(TRANSFORM::POSITION, m_vPosition_LoveIconOff);
    m_pLoveIndex->Set_State(TRANSFORM::POSITION, m_vPosition_LoveIndexOff);
    m_pProfileHp->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileHpOff);


    m_strHp.resize(MAX_PATH);
    m_strHpMax.resize(MAX_PATH);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    auto pGameInstance = CGameInstance::Get_Instance();
    auto p3DUILayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_UI_3D);
    p3DUILayer->Iterate_Objects([&](shared_ptr<CGameObject> pObject)
        {
            shared_ptr<CMenuCharacter> pMenuCharacter = dynamic_pointer_cast<CMenuCharacter>(pObject);
            if (pMenuCharacter->Get_ModelTag() == PROTOTYPE_COMPONENT_MODEL_ARASHI)
            {
                m_pMenuCharacter = pMenuCharacter;
                return false;
            }

            return true;
        });

    return S_OK;
}

void CUI_Menu_Profile_PartyMember2::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (CUI_Manager::Get_Instance()->Get_MenuIndex() != 0)
    {
        m_pMenuCharacter->Set_Enable(false);
    }
}

void CUI_Menu_Profile_PartyMember2::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_2);

    KeyWorks();
  
}

HRESULT CUI_Menu_Profile_PartyMember2::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (0 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            /* String */
            m_strName       = TEXT("아라시 스프링");
            m_strLevelTag   = TEXT("레벨");
            m_iLevel        = 4;
            m_strLevelIndex = std::to_wstring(m_iLevel);
            m_strHpTag      = TEXT("체력");
            m_iHp           = 723;
            m_strHp         = std::to_wstring(m_iHp);
            m_strHpSeperate = TEXT("/");
            m_iHpMax        = 723;
            m_strHpMax      = std::to_wstring(m_iHpMax);
            m_strPsyco      = TEXT("초고속");


            if (2 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                // Render Enable Character
                if (false == m_pMenuCharacter->Get_Enable())
                {
                    m_pMenuCharacter->Set_Enable(true);
                    m_pMenuCharacter->Fade_In();
                }

                /* Frame */
                if (FAILED(m_pTexture_On->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* ProfileFrame */
                if (FAILED(m_pTexture_ProfileFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileFrame->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }

                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }


                /* ProfileIndex */
                if (FAILED(m_pTexture_ProfileIndex->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileIndex->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* MEMBER */
                if (FAILED(m_pTexture_MEMBER->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pMEMBER->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* Profile */
                if (FAILED(m_pTexture_Profile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfile->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* SkillIconFrame */
                if (FAILED(m_pTexture_SkillIconFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIconFrame->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* SkillIcon */
                if (FAILED(m_pTexture_SkillIcon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcon->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* SkillTag */
                if (FAILED(m_pTexture_SkillTag->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillTag->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* LoveIcon */
                if (FAILED(m_pTexture_LoveIcon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIcon->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* LoveIndex */
                if (FAILED(m_pTexture_LoveIndex->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIndex->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* ProfileHp */
                if (FAILED(m_pTexture_ProfileHp->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileHp->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strName,          _float2(214.f, 478.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.47f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelTag,      _float2(216.f, 514.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelIndex,    _float2(250.f, 506.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.53f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpTag,         _float2(216.f, 538.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHp,            _float2(366.f, 538.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpSeperate,    _float2(396.f, 538.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpMax,         _float2(406.f, 538.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPsyco,         _float2(462.f, 551.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.4f);




            }
            else
            {
                // Render Disable Character
                if (m_pMenuCharacter->Get_Enable())
                {
                    m_pMenuCharacter->Set_Enable(false);
                }

                if (FAILED(m_pTexture_Off->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(11)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }

                /* ProfileFrame */
                if (FAILED(m_pTexture_ProfileFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileFrame->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Render", "Failed to CGameObject::Render");
                }


                /* ProfileIndex */
                if (FAILED(m_pTexture_ProfileIndex->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileIndex->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                /* LEADER */
                if (FAILED(m_pTexture_MEMBER->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pMEMBER->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                 /* Profile */
                if (FAILED(m_pTexture_Profile->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfile->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                /* SkillIconFrame */
                if (FAILED(m_pTexture_SkillIconFrame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIconFrame->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                /* SkillIcon */
                if (FAILED(m_pTexture_SkillIcon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillIcon->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                /* SkillTag */
                if (FAILED(m_pTexture_SkillTag->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkillTag->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                /* LoveIcon */
                if (FAILED(m_pTexture_LoveIcon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIcon->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(8)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                /* LoveIndex */
                if (FAILED(m_pTexture_LoveIndex->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pLoveIndex->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                /* ProfileHp */
                if (FAILED(m_pTexture_ProfileHp->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pProfileHp->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember1::Render", "Failed to CGameObject::Render");
                }

                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strName,          _float2(214.f, 478.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.47f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelTag,      _float2(216.f, 514.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strLevelIndex,    _float2(250.f, 506.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.53f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpTag,         _float2(216.f, 538.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHp,            _float2(366.f, 538.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpSeperate,    _float2(396.f, 538.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strHpMax,         _float2(406.f, 538.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.3f);
                CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPsyco,         _float2(462.f, 551.f), XMVectorSet(0.f, 0.f, 0.f, 1.0f), 0.4f);

            }
        }
    }

    return S_OK;
}

HRESULT CUI_Menu_Profile_PartyMember2::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Profile_PartyMember2::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Menu_Profile_PartyMember2::KeyWorks()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (0 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {

            if (2 == CUI_Manager::Get_Instance()->Get_SelectMemberIndex())
            {
                m_pBackground->Set_Scale(m_vScaleOn);
                m_pBackground->Set_State(TRANSFORM::POSITION, m_vPositionOn);

                m_pProfileFrame->Set_Scale(m_vScale_ProfileFrameOn);
                m_pProfileFrame->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameOn);

                m_pProfileIndex->Set_Scale(m_vScale_ProfileIndexOn);
                m_pProfileIndex->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileIndexOn);

                m_pMEMBER->Set_Scale(m_vScale_MEMBEROn);
                m_pMEMBER->Set_State(TRANSFORM::POSITION, m_vPosition_MEMBEROn);

                m_pProfile->Set_Scale(m_vScale_ProfileOn);
                m_pProfile->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileOn);

                m_pSkillIcon->Set_Scale(m_vScale_SkillIconOn);
                m_pSkillIcon->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIconOn);

                m_pSkillTag->Set_Scale(m_vScale_SkillTagOn);
                m_pSkillTag->Set_State(TRANSFORM::POSITION, m_vPosition_SkillTagOn);
            }
            else
            {
                m_pBackground->Set_Scale(m_vScaleOff);
                m_pBackground->Set_State(TRANSFORM::POSITION, m_vPositionOff);

                m_pProfileFrame->Set_Scale(m_vScale_ProfileFrameOff);
                m_pProfileFrame->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileFrameOff);

                m_pProfileIndex->Set_Scale(m_vScale_ProfileIndexOff);
                m_pProfileIndex->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileIndexOff);

                m_pMEMBER->Set_Scale(m_vScale_MEMBEROff);
                m_pMEMBER->Set_State(TRANSFORM::POSITION, m_vPosition_MEMBEROff);

                m_pProfile->Set_Scale(m_vScale_ProfileOff);
                m_pProfile->Set_State(TRANSFORM::POSITION, m_vPosition_ProfileOff);

                m_pSkillIcon->Set_Scale(m_vScale_SkillIconOff);
                m_pSkillIcon->Set_State(TRANSFORM::POSITION, m_vPosition_SkillIconOff);

                m_pSkillTag->Set_Scale(m_vScale_SkillTagOff);
                m_pSkillTag->Set_State(TRANSFORM::POSITION, m_vPosition_SkillTagOff);
            }
        }
    }
}

shared_ptr<CUI_Menu_Profile_PartyMember2> CUI_Menu_Profile_PartyMember2::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Profile_PartyMember2> pInstance = make_private_shared(CUI_Menu_Profile_PartyMember2, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Profile_PartyMember2::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Profile_PartyMember2::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Profile_PartyMember2> pInstance = make_private_shared_copy(CUI_Menu_Profile_PartyMember2, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Profile_PartyMember2::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
