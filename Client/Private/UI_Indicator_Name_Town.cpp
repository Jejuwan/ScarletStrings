#include "ClientPCH.h"
#include "UI_Indicator_Name_Town.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Indicator_Name_Town::CUI_Indicator_Name_Town(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Name_Town::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Name_Town::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Initialize", "Failed to CGameObject::Initialize");
    }

    Initialize_UI();

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Name_Town::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Name_Town::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_4);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Name_Town::Render()
{
    if (true == m_bBackground2_Start)
    {
        if (FAILED(m_pTexture_Background_2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground_2->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime_Background_2, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }
        if (FAILED(__super::Render(34)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }
    }

    if (true == m_bBackground1_Start)
    {
        if (FAILED(m_pTexture_Background_1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground_1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fTime_Background_1, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }
        if (FAILED(__super::Render(33)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }
            
    }

    if (0.5f <= m_fTime_Background_1)
    {
        if (FAILED(m_pTexture_Line_M->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pLine_M->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(35)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }
    }

    if (0.45f <= m_fTime_Background_2)
    {
        if (FAILED(m_pTexture_Line_L1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pLine_L1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(36)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }

        if (FAILED(m_pTexture_Line_R1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pLine_R1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(36)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }

        if (FAILED(m_pTexture_Line_L2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pLine_L2->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(36)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }

        if (FAILED(m_pTexture_Line_R2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pLine_R2->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(36)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_ObjectAttack::Render", "Failed to CUI_Indicator_ObjectAttack::Render");
        }
    }

    if (0.f < m_fTime_RenderFontAlpha_OutLine)
    {
        if (SCENE::SUOH == CGameInstance::Get_Instance()->Current_Scene())
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_OutLine"), m_strNameTitle_OutLine_Suoh, _float2(g_iWinCX * 0.5f - 210.f, g_iWinCY * 0.5f - 120.f), XMVectorSet(0.f / 255.f, 0.f / 255.f, 0.f / 255.f, m_fTime_RenderFontAlpha_OutLine), 1.5f);
        }
        else
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_OutLine"), m_strNameTitle_OutLine, _float2(g_iWinCX * 0.5f - 300.f, g_iWinCY * 0.5f - 120.f), XMVectorSet(0.f / 255.f, 0.f / 255.f, 0.f / 255.f, m_fTime_RenderFontAlpha_OutLine), 1.5f);
        //CGameInstance::Get_Instance()->Render_Font(TEXT("Font_OutLine"), m_strNameSubTitle,      _float2(g_iWinCX * 0.5f - 120.f, g_iWinCY * 0.5f - 50.f), XMVectorSet(0.f / 255.f, 0.f / 255.f, 0.f / 255.f, m_fTime_RenderFontAlpha_OutLine), 0.8f);
    }

	return S_OK;
}

HRESULT CUI_Indicator_Name_Town::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Ready_Components", "Failed to Get_Component: SHADER");
    }
    
    return S_OK;
}

void CUI_Indicator_Name_Town::Initialize_UI()
{
//-------------------------------------------------------------------Transform
    m_pBackground_1                 = CTransform::Create(m_pDevice, m_pContext);
    m_pBackground_2                 = CTransform::Create(m_pDevice, m_pContext);
    m_pLine_L1                      = CTransform::Create(m_pDevice, m_pContext);
    m_pLine_L2                      = CTransform::Create(m_pDevice, m_pContext);
    m_pLine_M                       = CTransform::Create(m_pDevice, m_pContext);
    m_pLine_R1                      = CTransform::Create(m_pDevice, m_pContext);
    m_pLine_R2                      = CTransform::Create(m_pDevice, m_pContext);

//-------------------------------------------------------------------Texture
    m_pTexture_Background_1         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Background1.dds"));
    m_pTexture_Background_2         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Background2.dds"));
    m_pTexture_Line_L1              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Line1.dds"));
    m_pTexture_Line_L2              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Line1.dds"));
    m_pTexture_Line_M               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Line2.dds"));
    m_pTexture_Line_R1              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Line1.dds"));
    m_pTexture_Line_R2              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Town/TownName_Line1.dds"));

//-------------------------------------------------------------------_bool
    m_bBackground1_Start            = false;
    m_bBackground2_Start            = false;
    m_bRenderFont_Start             = false;
    m_bDisappear_Start              = false;

//-------------------------------------------------------------------_uint
    m_iAnimCount                    = 0;
    m_iBlinkIndex                   = 0;

//-------------------------------------------------------------------_float
    m_fAppearTime                   = 0.f;
    m_fAccTime                      = 0.f;
    m_fTime_Background_1            = 0.f;
    m_fTime_Background_2            = 0.f;
    m_fTime_Line_L1                 = 0.f;
    m_fTime_Line_L2                 = 0.f;
    m_fTime_Line_M                  = 0.f;
    m_fTime_Line_R1                 = 0.f;
    m_fTime_Line_R2                 = 0.f;
    m_fTime_RenderFontAlpha_OutLine = 0.f;

//-------------------------------------------------------------------_float3
    m_vScale_Background_1           = _float3(850.f, 200.f, 1.f);
    m_vScale_Background_2           = _float3(950.f, 200.f, 1.f);
    m_vScale_Line_L1                = _float3(50.f, 5.f, 1.f);
    m_vScale_Line_L2                = _float3(50.f, 5.f, 1.f);
    m_vScale_Line_M                 = _float3(50.f, 30.f, 1.f);
    m_vScale_Line_R1                = _float3(50.f, 5.f, 1.f);
    m_vScale_Line_R2                = _float3(50.f, 5.f, 1.f);

//-------------------------------------------------------------------_float4
    m_vPosition_Background_1        = _float4(0.f, 100.f, 0.f, 1.f);
    m_vPosition_Background_2        = _float4(0.f, 100.f, 0.f, 1.f);
    m_vPosition_Line_L1             = _float4(-120.f, 57.f, 0.f, 1.f);
    m_vPosition_Line_L2             = _float4(-350.f, 57.f, 0.f, 1.f);
    m_vPosition_Line_M              = _float4(0.f, 58.f, 0.f, 1.f);
    m_vPosition_Line_R1             = _float4(120.f, 57.f, 0.f, 1.f);
    m_vPosition_Line_R2             = _float4(350.f, 57.f, 0.f, 1.f);
    
//-------------------------------------------------------------------wstring
    m_strNameTitle_OutLine          = /*L"구로 디지털 단지"*/L"미즈하강 신도시 개발 특구";
    m_strNameSubTitle               = /*L"환장의 도시계획"*/L"환상의 도시계획";
    m_strNameTitle_OutLine_Suoh     = L"    스오 - 류신구";
//-------------------------------------------------------------------Set_Scale
    m_pBackground_1->Set_Scale(m_vScale_Background_1);
    m_pBackground_2->Set_Scale(m_vScale_Background_2);
    m_pLine_L1->Set_Scale(m_vScale_Line_L1);
    m_pLine_L2->Set_Scale(m_vScale_Line_L2);
    m_pLine_M->Set_Scale(m_vScale_Line_M);
    m_pLine_R1->Set_Scale(m_vScale_Line_R1);
    m_pLine_R2->Set_Scale(m_vScale_Line_R2);

//-------------------------------------------------------------------Set_Position
    m_pBackground_1->Set_State(TRANSFORM::POSITION, m_vPosition_Background_1);
    m_pBackground_2->Set_State(TRANSFORM::POSITION, m_vPosition_Background_2);
    m_pLine_L1->Set_State(TRANSFORM::POSITION, m_vPosition_Line_L1);
    m_pLine_L2->Set_State(TRANSFORM::POSITION, m_vPosition_Line_L2);
    m_pLine_M->Set_State(TRANSFORM::POSITION, m_vPosition_Line_M);
    m_pLine_R1->Set_State(TRANSFORM::POSITION, m_vPosition_Line_R1);
    m_pLine_R2->Set_State(TRANSFORM::POSITION, m_vPosition_Line_R2);

}

void CUI_Indicator_Name_Town::Tick_UI(_float fTimeDelta)
{
    if (SCENE::CUTSCENE_MIZUHA == CGameInstance::Get_Instance()->Current_Scene())
    {
        if (4.f <= m_fAppearTime)
        {
            m_fAppearTime = 4.f;
        }
        else
        {
            m_fAppearTime += fTimeDelta;
        }
    }

    Start_Title_Animation_Trigger();

    if (true == m_bDisappear_Start)
    {
        End_Animation(fTimeDelta);
    }
    else
    {
        Start_Animation(fTimeDelta);
        Start_Line_M_Become_Longer(fTimeDelta);
        Start_Line_Movement(fTimeDelta);
    }
}

void CUI_Indicator_Name_Town::Start_Title_Animation_Trigger()
{
    if (0 == m_iAnimCount && 2.5f <= m_fAppearTime)
    {
        m_bBackground1_Start = true;
    }
}

void CUI_Indicator_Name_Town::Start_Animation(_float fTimeDelta)
{
    if (true == m_bBackground1_Start)
    {
        m_fTime_Background_1 += fTimeDelta * 0.5f;

        if (0.3f <= m_fTime_Background_1)
        {
            m_bBackground2_Start = true;
        }
        if (0.4f <= m_fTime_Background_2)
        {
            m_bRenderFont_Start = true;
        }
        if (1.f <= m_fTime_Background_1)
        {
            m_fTime_Background_1 = 1.f;
        }

    }
    
    if (true == m_bBackground2_Start)
    {
        _uint iIndex = m_iBlinkIndex % 2;
     
        if (0 == iIndex)
        {
            m_fTime_Background_2 += fTimeDelta * 0.5f;
            
            if (2.f <= m_fTime_Background_2)
            {
                m_fTime_Background_2 = 2.f;
                ++m_iBlinkIndex;
            }
        }
        if (1 == iIndex)
        {
            m_fTime_Background_2 -= fTimeDelta * 0.5f;

            if (1.4f >= m_fTime_Background_2)
            {
                m_fTime_Background_2 = 1.4f;
                ++m_iBlinkIndex;
            }
        }
    }

    if (true == m_bRenderFont_Start)
    {
        m_fTime_RenderFontAlpha_OutLine += fTimeDelta * 0.5f;

        if (1.f <= m_fTime_RenderFontAlpha_OutLine)
        {
            m_fTime_RenderFontAlpha_OutLine = 1.f;
        }
    }

}

void CUI_Indicator_Name_Town::Start_Line_M_Become_Longer(_float fTimeDelta)
{
    if (true == m_bBackground1_Start && 0.2f <= m_fTime_Background_1)
    {
        m_fTime_Line_M += fTimeDelta * 60.f;

        if (900.f <= m_vScale_Line_M.x)
        {
            m_vScale_Line_M.x = 900.f;
        }
        else
        {
            m_vScale_Line_M.x += m_fTime_Line_M;

            m_pLine_M->Set_Scale(m_vScale_Line_M);
        }
    }
}

void CUI_Indicator_Name_Town::Start_Line_Movement(_float fTimeDelta)
{
    if (true == m_bBackground1_Start && 0.45f <= m_fTime_Background_2)
    {
        m_fTime_Line_L1 += fTimeDelta * 40.f;
        m_fTime_Line_R1 += fTimeDelta * 40.f;

        if (230.f <= m_vScale_Line_L1.x)
        {
            m_vScale_Line_L1.x = 230.f;
            m_vScale_Line_R1.x = 230.f;
        }
        else
        {
            m_vScale_Line_L1.x += m_fTime_Line_L1;
            m_vScale_Line_R1.x += m_fTime_Line_R1;

            m_pLine_L1->Set_Scale(m_vScale_Line_L1);
            m_pLine_R1->Set_Scale(m_vScale_Line_R1);
        }
    }

    if (true == m_bBackground1_Start && 0.5f <= m_fTime_Background_2)
    {
        m_fTime_Line_L2 += fTimeDelta * 40.f;
        m_fTime_Line_R2 += fTimeDelta * 40.f;

        if (230.f <= m_vScale_Line_L2.x)
        {
            m_vScale_Line_L2.x = 230.f;
            m_vScale_Line_R2.x = 230.f;

            m_fAccTime += fTimeDelta;

            if (5.f <= m_fAccTime)
            {
                m_bDisappear_Start = true;

                m_fTime_Line_L1 = 0.f;
                m_fTime_Line_R1 = 0.f;
                m_fTime_Line_L2 = 0.f; 
                m_fTime_Line_R2 = 0.f; 
            }
        }
        else
        {
            m_vScale_Line_L2.x += m_fTime_Line_L2;
            m_vScale_Line_R2.x += m_fTime_Line_R2;

            m_pLine_L2->Set_Scale(m_vScale_Line_L2);
            m_pLine_R2->Set_Scale(m_vScale_Line_R2);
        }
    }
}

void CUI_Indicator_Name_Town::End_Animation(_float fTimeDelta)
{
    m_bRenderFont_Start     = false;


    m_fTime_Background_1            -= fTimeDelta * 0.55f;
    if (0.f >= m_fTime_Background_1)
    {
        m_fTime_Background_1 = 0.f;
        m_bBackground1_Start = false;
    }


    m_fTime_Background_2            -= fTimeDelta * 0.55f;
    if (0.f >= m_fTime_Background_2)
    {
        m_fTime_Background_2 = 0.f;
        m_bBackground2_Start = false;

    }


    m_fTime_RenderFontAlpha_OutLine -= fTimeDelta * 0.6f;
    if (0.f >= m_fTime_RenderFontAlpha_OutLine)
    {
        m_fTime_RenderFontAlpha_OutLine = 0.f;
    }



     if (50.f >= m_vScale_Line_M.x)
     {
         m_vScale_Line_M.x = 50.f;
     }
     else
     {
         m_fTime_Line_M     += fTimeDelta * 60.f;
         m_vScale_Line_M.x  -= m_fTime_Line_M;
         m_pLine_M->Set_Scale(m_vScale_Line_M);
     }


     if (50.f >= m_vScale_Line_L1.x || 50.f >= m_vScale_Line_L2.x || 50.f >= m_vScale_Line_R1.x || 50.f >= m_vScale_Line_R2.x)
     {
         m_vScale_Line_L1.x = 50.f;
         m_vScale_Line_R1.x = 50.f;
         m_vScale_Line_L2.x = 50.f;
         m_vScale_Line_R2.x = 50.f;
     }
     else
     {
         m_fTime_Line_L1 += fTimeDelta * 40.f;
         m_fTime_Line_R1 += fTimeDelta * 40.f;
         m_fTime_Line_L2 += fTimeDelta * 40.f;
         m_fTime_Line_R2 += fTimeDelta * 40.f;

         m_vScale_Line_L1.x -= m_fTime_Line_L1;
         m_vScale_Line_L2.x -= m_fTime_Line_L2;
         m_vScale_Line_R1.x -= m_fTime_Line_R1;
         m_vScale_Line_R2.x -= m_fTime_Line_R2;

         m_pLine_L1->Set_Scale(m_vScale_Line_L1);
         m_pLine_L2->Set_Scale(m_vScale_Line_L2);
         m_pLine_R1->Set_Scale(m_vScale_Line_R1);
         m_pLine_R2->Set_Scale(m_vScale_Line_R2);
     }


    if (0.f >= m_fTime_Background_1 && 0.f >= m_fTime_Background_2 && 0.f >= m_fTime_RenderFontAlpha_OutLine)
    {
//-------------------------------------------------------------------_bool
        m_bBackground1_Start            = false;
        m_bBackground2_Start            = false;
        m_bRenderFont_Start             = false;
        m_bDisappear_Start              = false;

//-------------------------------------------------------------------_uint
        m_iBlinkIndex                   = 0;

//-------------------------------------------------------------------_float
        m_fAccTime                      = 0.f;
        m_fTime_Background_1            = 0.f;
        m_fTime_Background_2            = 0.f;
        m_fTime_Line_L1                 = 0.f;
        m_fTime_Line_L2                 = 0.f;
        m_fTime_Line_M                  = 0.f;
        m_fTime_Line_R1                 = 0.f;
        m_fTime_Line_R2                 = 0.f;
        m_fTime_RenderFontAlpha_OutLine = 0.f;

//-------------------------------------------------------------------_float3
        m_vScale_Background_1           = _float3(850.f, 200.f, 1.f);
        m_vScale_Background_2           = _float3(950.f, 200.f, 1.f);
        m_vScale_Line_L1                = _float3(50.f, 5.f, 1.f);
        m_vScale_Line_L2                = _float3(50.f, 5.f, 1.f);
        m_vScale_Line_M                 = _float3(50.f, 30.f, 1.f);
        m_vScale_Line_R1                = _float3(50.f, 5.f, 1.f);
        m_vScale_Line_R2                = _float3(50.f, 5.f, 1.f);
    
//-------------------------------------------------------------------Set_Scale
        m_pBackground_1->Set_Scale(m_vScale_Background_1);
        m_pBackground_2->Set_Scale(m_vScale_Background_2);
        m_pLine_L1->Set_Scale(m_vScale_Line_L1);
        m_pLine_L2->Set_Scale(m_vScale_Line_L2);
        m_pLine_M->Set_Scale(m_vScale_Line_M);
        m_pLine_R1->Set_Scale(m_vScale_Line_R1);
        m_pLine_R2->Set_Scale(m_vScale_Line_R2);

        m_bDisappear_Start = false;
        m_iAnimCount += 1;
    }
}

shared_ptr<CUI_Indicator_Name_Town> CUI_Indicator_Name_Town::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Name_Town> pInstance = make_private_shared(CUI_Indicator_Name_Town, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Name_Town::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Name_Town::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_Name_Town> pInstance = make_private_shared_copy(CUI_Indicator_Name_Town, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Name_Town::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
