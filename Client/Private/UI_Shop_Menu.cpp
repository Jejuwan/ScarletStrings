#include "ClientPCH.h"
#include "UI_Shop_Menu.h"
#include "GameInstance.h"
#include "UI_Manager.h"      

CUI_Shop_Menu::CUI_Shop_Menu(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Shop_Menu::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_2, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Shop_Menu::Initialize(any _pPlayerEntityDesc)
{   
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category::Initialize", "Failed to CGameObject::Initialize");
    }


    m_pTexture_On = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOn.png"));
    m_pTexture_Off = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds"));

    m_pTexture_SubMenu = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/SkillTag.png"));

    m_pTexture_Line = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Line.png"));
    m_pTexture_Diagonal = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Shop_Line.png"));
    m_pTexture_Cart = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Shop_Cart.png"));

    m_pTexture_Arrow[0] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_L.png"));
    m_pTexture_Arrow[1] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_R.png"));

    m_pTexture_Highlight = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/HighLight.png"));

    m_pTexture_CheckBox = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Shop_CheckBox.png"));
    m_pTexture_CheckBox_Line = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Shop_CheckLine.png"));

    m_pTexture_Key_Icon[0] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/1.png"));
    m_pTexture_Key_Icon[1] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/3.png"));
    m_pTexture_Key_Icon[2] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Q.png"));
    m_pTexture_Key_Icon[3] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/E.png"));
    
    m_pTranform_Purchase = CTransform::Create(m_pDevice, m_pContext);
    m_pTranform_Sale = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_Everything = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_Item = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_Present = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_ThinLine = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_ThickLine = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_Diagonal = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_Cart = CTransform::Create(m_pDevice, m_pContext);

    m_pTransform_Highlight[0] = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_Highlight[1] = CTransform::Create(m_pDevice, m_pContext);

    m_pTransform_CheckBox_YellowLine = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_CheckBox = CTransform::Create(m_pDevice, m_pContext);
    m_pTransform_CheckBox_Line = CTransform::Create(m_pDevice, m_pContext);

    for (_int i = 0; i < 6; i++)
    {
        m_pTransform_Arrow[i] = CTransform::Create(m_pDevice, m_pContext);
    }

    for (_int i = 0; i < 4; i++)
    {
        m_pTransform_Key_Icon[i] = CTransform::Create(m_pDevice, m_pContext);
    }
    
    

    Input_Transform();

    return S_OK;
}

void CUI_Shop_Menu::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Shop_Menu::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_2);
}

HRESULT CUI_Shop_Menu::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_Enter_ShopUI())
    {
        _int iIndex = CUI_Manager::Get_Instance()->Get_ShopMenu_Index();
        _int iSubMenu_Index = CUI_Manager::Get_Instance()->Get_ShopSubMenu_Index();
        _int iCatalog_Index = CUI_Manager::Get_Instance()->Get_Shop_Catalog_Index();

        Bind_Shader(m_pTexture_Line, m_pTransform_ThinLine, 2);
        Bind_Shader(m_pTexture_Diagonal, m_pTransform_Diagonal, 3);
        Bind_Shader(m_pTexture_Cart, m_pTransform_Cart, 3);
        Bind_Shader(m_pTexture_Line, m_pTransform_ThickLine, 2);

        Bind_Shader(0 == iIndex ? m_pTexture_On : m_pTexture_Off, m_pTranform_Purchase, 4);
        Bind_Shader(1 == iIndex ? m_pTexture_On : m_pTexture_Off, m_pTranform_Sale, 4);

        Bind_Shader(m_pTexture_SubMenu, m_pTransform_Everything, 5, 0 == iSubMenu_Index ? 1.f : 0.5f);
        Bind_Shader(m_pTexture_SubMenu, m_pTransform_Item, 5, 1 == iSubMenu_Index ? 1.f : 0.5f);
        Bind_Shader(m_pTexture_SubMenu, m_pTransform_Present, 5, 2 == iSubMenu_Index ? 1.f : 0.5f);


        for (_int i = 0; i < 4; i++)
        {
            Bind_Shader(m_pTexture_Key_Icon[i], m_pTransform_Key_Icon[i], 8);

            _int iTexture = 0;

            if (i % 2 == 0)
                iTexture = 0;
            else
                iTexture = 1;

            Bind_Shader(m_pTexture_Arrow[iTexture], m_pTransform_Arrow[i], 8);
        }


        m_pTransform_Arrow[4]->Set_State(TRANSFORM::POSITION, _float4(-95.f, 250.f - (75.f * iCatalog_Index), 0.f, 1.f));
        m_pTransform_Arrow[5]->Set_State(TRANSFORM::POSITION, _float4(-8.f, 250.f - (75.f * iCatalog_Index), 0.f, 1.f));

        Bind_Shader(m_pTexture_Arrow[0], m_pTransform_Arrow[4], 8);
        Bind_Shader(m_pTexture_Arrow[1], m_pTransform_Arrow[5], 8);


        Bind_Shader(m_pTexture_Highlight, iIndex == 0 ? m_pTransform_Highlight[0] : m_pTransform_Highlight[1], 8);
        
        if (CUI_Manager::Get_Instance()->Get_Buy_CheckBox())
        {
            Bind_Shader(m_pTexture_CheckBox, m_pTransform_CheckBox, 9, 1.f, true);
            Bind_Shader(m_pTexture_Line, m_pTransform_CheckBox_YellowLine, 2, 1.f, true);

            m_pTransform_CheckBox_Line->Set_Scale(_float3(250.f, 75.f, 1.f));

            if (CUI_Manager::Get_Instance()->Get_Buy_Check() == 0)
            {
                m_pTransform_CheckBox_Line->Set_State(TRANSFORM::POSITION, _float4(25.f, -15.f, 0.f, 1.f));
            }
            else if (CUI_Manager::Get_Instance()->Get_Buy_Check() == 1)
            {
                m_pTransform_CheckBox_Line->Set_State(TRANSFORM::POSITION, _float4(25.f, -115.f, 0.f, 1.f));
            }

            Bind_Shader(m_pTexture_CheckBox_Line, m_pTransform_CheckBox_Line, 8, 0.25f, true);
        }

    }

    return S_OK;
}

HRESULT CUI_Shop_Menu::Bind_Shader(shared_ptr<CTexture> ptrTexture, shared_ptr<CTransform> ptrTransform, _int iPassIndex, _float fValue, _bool IsCheckbox)
{
    if (FAILED(ptrTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Menu::Render", "Failed to CTexture::Bind_ShaderResourceView");
    }
    if (FAILED(ptrTransform->Bind_OnShader(m_pShader)))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Menu::Render", "Failed to CTexture::Bind_OnShader");
    }

    if (!IsCheckbox)
    {
        if (CUI_Manager::Get_Instance()->Get_Buy_CheckBox())
            fValue = 0.1f;
    }


    if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &fValue, sizeof(_float))))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_SubMenu::Render", "Failed to CTexture::Bind_RawValue");
    }

    if (FAILED(__super::Render(iPassIndex)))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Menu::Render", "Failed to CGameObject::Render");
    }

    Make_Font();

    return S_OK;
}

void CUI_Shop_Menu::Make_Font()
{
    _int iSubMenu_Index = CUI_Manager::Get_Instance()->Get_ShopSubMenu_Index();
    _int iIndex = CUI_Manager::Get_Instance()->Get_ShopMenu_Index();

    _vector BlackColor = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    _vector GrayColor = XMVectorSet(0.01f, 0.01f, 0.01f, 0.f);
    _vector WhiteColor = XMVectorSet(0.1f, 0.1f, 0.1f, 0.f);
    _vector OtherColor = XMVectorSet(0.96f, 0.81f, 0.74f, 0.f);

    wstring strInfo_Purchase = TEXT("구입할 상품을 선택하세요.");
    wstring strInfo_Sale = TEXT("구입할 상품을 선택하세요.");

    wstring strPurchase = TEXT("구매");
    wstring strSale = TEXT("매각");

    wstring strEverything = TEXT("모두");
    wstring strItem = TEXT("배틀 아이템");
    wstring strGift = TEXT("선물");

    if (CUI_Manager::Get_Instance()->Get_Buy_CheckBox())
    {
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
            TEXT("최종 확인"), _float2((g_iWinCX * 0.5f) - 335.f, (g_iWinCY * 0.5f) - 220.f), BlackColor, 0.5f);

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
            TEXT("구입하시겠습니까?"), _float2((g_iWinCX * 0.5f) - 125.f, (g_iWinCY * 0.5f) - 100.f), OtherColor, 0.75f);

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
            TEXT("예"), _float2((g_iWinCX * 0.5f) - 25.f, (g_iWinCY * 0.5f)), OtherColor, 0.75f);

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
            TEXT("아니오"), _float2((g_iWinCX * 0.5f) - 25.f, (g_iWinCY * 0.5f) + 100.f), OtherColor, 0.75f);
    }


    if (CUI_Manager::Get_Instance()->Get_Buy_CheckBox())
    {
        BlackColor = GrayColor = WhiteColor = OtherColor = XMVectorSet(0.005f, 0.005f, 0.005f, 0.f);
    }

    if (0 == iIndex)
    {
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
            strInfo_Purchase, _float2(80.f, 960.f), OtherColor, 0.52f);
    }

    if (1 == iIndex)
    {
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
            strInfo_Sale, _float2(80.f, 960.f), OtherColor, 0.52f);
    }

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        strPurchase, _float2(187.f, 89.f), 0 == iIndex ? OtherColor : WhiteColor, 0.5f);

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        strSale, _float2(400.f, 89.f), 1 == iIndex ? OtherColor : WhiteColor, 0.5f);
    

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        strEverything, _float2(190.f, 170.f), 0 == iSubMenu_Index ? BlackColor : GrayColor, 0.5f);

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        strItem, _float2(378.f, 170.f), 1 == iSubMenu_Index ? BlackColor : GrayColor, 0.5f);

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        strGift, _float2(628.f, 170.f), 2 == iSubMenu_Index ? BlackColor : GrayColor, 0.5f);


    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        TEXT("아이템 상점"), _float2(1500.f, 85.f), WhiteColor, 0.65f);

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        TEXT("상품"), _float2(80.f, 220.f), BlackColor, 0.5f);

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        TEXT("소지 수"), _float2(657.5f, 220.f), BlackColor, 0.5f);

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        TEXT("금액"), _float2(775.f, 220.f), BlackColor, 0.5f);

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        TEXT("개수"), _float2(885.f, 220.f), BlackColor, 0.5f);
}

void CUI_Shop_Menu::Input_Transform()
{
/* 메인 메뉴*/
    m_pTranform_Purchase->Set_Scale(_float3(198.f, 67.f, 1.f));
    m_pTranform_Purchase->Set_State(TRANSFORM::POSITION, _float4(-750.f, 440.f, 0.5f, 1.f));

    m_pTranform_Sale->Set_Scale(_float3(198.f, 67.f, 1.f));
    m_pTranform_Sale->Set_State(TRANSFORM::POSITION, _float4(-540.f, 440.f, 0.5f, 1.f));

/* 서브 메뉴*/
    m_pTransform_Everything->Set_Scale(_float3(200.f, 30.f, 1.f));
    m_pTransform_Everything->Set_State(TRANSFORM::POSITION, _float4(-750.f, 360.f, 0.f, 1.f));

    m_pTransform_Item->Set_Scale(_float3(200.f, 30.f, 1.f));
    m_pTransform_Item->Set_State(TRANSFORM::POSITION, _float4(-530.f, 360.f, 0.f, 1.f));

    m_pTransform_Present->Set_Scale(_float3(200.f, 30.f, 1.f));
    m_pTransform_Present->Set_State(TRANSFORM::POSITION, _float4(-310.f, 360.f, 0.f, 1.f));



    m_pTransform_ThinLine->Set_Scale(_float3(g_iWinCX, 2.5f, 1.f));
    m_pTransform_ThinLine->Set_State(TRANSFORM::POSITION, _float4(0.f, 395.f, 0.f, 1.f));

    m_pTransform_Diagonal->Set_Scale(_float3(150.f, 300.f, 1.f));
    m_pTransform_Diagonal->Set_State(TRANSFORM::POSITION, _float4(500.f, (g_iWinCY * 0.5f), 0.f, 1.f));

    m_pTransform_Cart->Set_Scale(_float3(60.f, 45.f, 1.f));
    m_pTransform_Cart->Set_State(TRANSFORM::POSITION, _float4(750.f, (g_iWinCY * 0.5f) - 100.f, 0.f, 1.f));

    m_pTransform_ThickLine->Set_Scale(_float3(900.f, 30.f, 1.f));
    m_pTransform_ThickLine->Set_State(TRANSFORM::POSITION, _float4(-440.f, (g_iWinCY * 0.5f) - 230.f, 0.f, 1.f));


    for (_int i = 0; i < 6; i++)
    {
        m_pTransform_Arrow[i]->Set_Scale(_float3(13.f, 17.f, 1.f));
    }

    m_pTransform_Arrow[0]->Set_State(TRANSFORM::POSITION, _float4(-903.f, 430.f, 0.f, 1.f));
    m_pTransform_Arrow[1]->Set_State(TRANSFORM::POSITION, _float4(-375.f, 430.f, 0.f, 1.f));
    m_pTransform_Arrow[2]->Set_State(TRANSFORM::POSITION, _float4(-903.f, 360.f, 0.f, 1.f));
    m_pTransform_Arrow[3]->Set_State(TRANSFORM::POSITION, _float4(-150.f, 360.f, 0.f, 1.f));

    m_pTransform_Highlight[0]->Set_Scale(_float3(152.f, 10.f, 1.f));
    m_pTransform_Highlight[0]->Set_State(TRANSFORM::POSITION, _float4(-749.f, 425.f, 0.6f, 1.f));

    m_pTransform_Highlight[1]->Set_Scale(_float3(152.f, 10.f, 1.f));
    m_pTransform_Highlight[1]->Set_State(TRANSFORM::POSITION, _float4(-539.f, 425.f, 0.8f, 1.f));

    m_pTransform_CheckBox->Set_Scale(_float3(700.f, 350.f, 1.f));
    m_pTransform_CheckBox->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));  

    m_pTransform_CheckBox_YellowLine->Set_Scale(_float3(700.f, 30.f, 1.f));
    m_pTransform_CheckBox_YellowLine->Set_State(TRANSFORM::POSITION, _float4(0.f, 210.f, 0.f, 1.f));

    for (_int i = 0; i < 4; i++)
    {
        m_pTransform_Key_Icon[i]->Set_Scale(_float3(30.f, 30.f, 1.f));
    }

    m_pTransform_Key_Icon[0]->Set_State(TRANSFORM::POSITION, _float4(-875.f, 430.f, 0.f, 1.f));
    m_pTransform_Key_Icon[1]->Set_State(TRANSFORM::POSITION, _float4(-410.f, 430.f, 0.f, 1.f));
    m_pTransform_Key_Icon[2]->Set_State(TRANSFORM::POSITION, _float4(-875.f, 360.f, 0.f, 1.f));
    m_pTransform_Key_Icon[3]->Set_State(TRANSFORM::POSITION, _float4(-180.f, 360.f, 0.f, 1.f));


}

HRESULT CUI_Shop_Menu::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Menu::Ready_Components", "Failed to CGameObject::Ready_Components");
    }

    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);

    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Menu::Ready_Components", "Failed to Get_Component: SHADER");
    }

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);

    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Menu::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }
    return S_OK;
}

shared_ptr<CUI_Shop_Menu> CUI_Shop_Menu::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Shop_Menu> pInstance = make_private_shared(CUI_Shop_Menu, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Shop_Menu::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Shop_Menu::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Shop_Menu> pInstance = make_private_shared_copy(CUI_Shop_Menu, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Shop_Menu::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
