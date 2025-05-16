#include "ClientPCH.h"
#include "UI_Shop_Info.h"
#include "GameInstance.h"
#include "UI_Manager.h"      

#include <sstream>

auto g_UIInstance = CUI_Manager::Get_Instance();
auto g_GameInstance = CGameInstance::Get_Instance();

CUI_Shop_Info::CUI_Shop_Info(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Shop_Info::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_2, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Shop_Info::Initialize(any _pPlayerEntityDesc)
{   
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category::Initialize", "Failed to CGameObject::Initialize");
    }
    

/* Texture */
    m_pTexture_Frame_Selected[0] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Shop_Selected.png"));
    m_pTexture_Frame_Selected[1] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Shop_No_Selected.png"));

    m_pTexture_Jelly[0] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Jelly.png"));
    m_pTexture_Jelly[1] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Jelly_All.png"));
    m_pTexture_Jelly[2] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Jelly_L.png"));
    m_pTexture_Jelly[3] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Jelly_All_L.png"));

    m_pTexture_Medium_Jelly[0] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Medium_Jelly.png"));
    m_pTexture_Medium_Jelly[1] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Medium_Jelly_All.png"));
    m_pTexture_Medium_Jelly[2] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Medium_Jelly_L.png"));
    m_pTexture_Medium_Jelly[3] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Medium_Jelly_All_L.png"));

    m_pTexture_Max_Jelly[0] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Max_Jelly.png"));
    m_pTexture_Max_Jelly[1] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Max_Jelly_All.png"));
    m_pTexture_Max_Jelly[2] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Max_Jelly_L.png"));
    m_pTexture_Max_Jelly[3] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Max_Jelly_All_L.png"));

    m_pTexture_Gift[0] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Gift.png"));
    m_pTexture_Gift[1] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Gift_L.png"));

    m_pTexture_Icon[0] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Jelly_L.png"));
    m_pTexture_Icon[1] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Jelly_All_L.png"));
    m_pTexture_Icon[2] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Medium_Jelly_L.png"));
    m_pTexture_Icon[3] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Medium_Jelly_All_L.png"));
    m_pTexture_Icon[4] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Max_Jelly_L.png"));
    m_pTexture_Icon[5] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Max_Jelly_All_L.png"));
    m_pTexture_Icon[6] = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Gift_L.png"));

    m_pTexture_Info = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Shop_Explanation.png"));
    m_pTexture_K_Info = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Shop/Shop_K_Info.png"));

/* Tranform*/
    m_pTranform_Icon = CTransform::Create(m_pDevice, m_pContext);
    m_pTranform_Info = CTransform::Create(m_pDevice, m_pContext);
    m_pTranform_K_Info = CTransform::Create(m_pDevice, m_pContext);


    for (_int i = 0; i < 9; i++)
    {
        m_pTranform_Catalog[i] = CTransform::Create(m_pDevice, m_pContext);
        m_pTranform_Catalog[i]->Set_Scale(_float3(900.f, 67.f, 1.f));
        m_pTranform_Catalog[i]->Set_State(TRANSFORM::POSITION, _float4(-440.f, (250.f - 75.f * i), 0.5f, 1.f));
        
        m_pTranform_Texture[i] = CTransform::Create(m_pDevice, m_pContext);
        m_pTranform_Texture[i]->Set_Scale(_float3(36.f, 40.f, 1.f));
        m_pTranform_Texture[i]->Set_State(TRANSFORM::POSITION, _float4(-857.5f, (250.f - 75.f * i), 0.5f, 1.f));

        m_NameFontPos[i] = _float2(140.f, 280.f + 75.f * i);

        m_PossessionFontPos[i] = _float2(680.f, 280.f + 75.f * i);
        m_NumberFontPos[i] = _float2(900.f, 280.f + 75.f * i);
    }
    for (_int i = 0; i < 9; i++)
    {
        m_AmountFontPos[i] = _float2(765.f, 280.f + 75.f * i);
    }

    m_pTranform_Icon->Set_Scale(_float3(75.f, 75.f, 1.f));
    m_pTranform_Icon->Set_State(TRANSFORM::POSITION, _float4(180.f, 260.f, 0.f, 1.f));

    m_pTranform_Info->Set_Scale(_float3(700.f, 350.f, 1.f));
    m_pTranform_Info->Set_State(TRANSFORM::POSITION, _float4(450.f, 150.f, 0.f, 1.f));

    m_pTranform_K_Info->Set_Scale(_float3(600.f, 150.f, 1.f));
    m_pTranform_K_Info->Set_State(TRANSFORM::POSITION, _float4(500.f, -225.f, 0.f, 1.f));

  

/* wstring */
    m_strGift[0] = TEXT("별갑 무늬 안경");
    m_strGift[1] = TEXT("즐거운 과자 모음");
    m_strGift[2] = TEXT("플러그인 병렬화 프로그램 V");

    m_strItem[0] = TEXT("체력(소) 젤리");
    m_strItem[1] = TEXT("전체 : 체력(소) 젤리");
    m_strItem[2] = TEXT("체력(중) 젤리");
    m_strItem[3] = TEXT("전체 : 체력(중) 젤리");
    m_strItem[4] = TEXT("체력(대) 젤리");
    m_strItem[5] = TEXT("전체 : 체력(대) 젤리");

    m_strName[0] = TEXT("체력(소) 젤리");
    m_strName[1] = TEXT("전체 : 체력(소) 젤리");
    m_strName[2] = TEXT("체력(중) 젤리");
    m_strName[3] = TEXT("전체 : 체력(중) 젤리");
    m_strName[4] = TEXT("체력(대) 젤리");
    m_strName[5] = TEXT("전체 : 체력(대) 젤리");
    m_strName[6] = TEXT("별갑 무늬 안경");
    m_strName[7] = TEXT("즐거운 과자 모음");
    m_strName[8] = TEXT("플러그인 병렬화 프로그램 V");

    m_strInfo[0] = TEXT("체력을 최대치의 30%만큼 회복한다. \n약간 달콤한 귤맛이 나는 음료형 젤리.");
    m_strInfo[1] = TEXT("아군 전체의 체력을 최대치의 30%만큼 회복한다.\n다 같이 나눠 먹는 패밀리 팩");
    m_strInfo[2] = TEXT("체력을 최대치의 50%만큼 회복한다. \n약간 달콤한 귤맛이 나는 음료형 젤리.");
    m_strInfo[3] = TEXT("아군 전체의 체력을 최대치의 50%만큼 회복한다.\n다 같이 나눠 먹는 패밀리 팩");
    m_strInfo[4] = TEXT("체력을 최대치의 70%만큼 회복한다. \n약간 달콤한 귤맛이 나는 음료형 젤리.");
    m_strInfo[5] = TEXT("아군 전체의 체력을 최대치의 70%만큼 회복한다.\n다 같이 나눠 먹는 패밀리 팩");
    m_strInfo[6] = LR"(귀갑테 중 인기인 <백자갑>을 본뜬 셀룰로이드 안경.
가죽 안경 받침대가 붙어 있다. 개성적인 패션 아이템이나 
도수 있는 렌즈를 넣어 실용적으로도 쓸 수 있다.
렌즈는 나중에 자기에게 맞는 것으로 교환 가능.)";
    m_strInfo[7] = LR"(달콤한 쿠키에 짭잘한 감자칩, 쫀득쫀득한 찹쌀떡 등
모두가 좋아하는 다양한 과자 모음.
무엇이 들어있는지 열어보는 것도 하나의 즐거움이다.
가족이나 침구와 함께, 아니면 혼자 먹어버리는 것도...?)";
    m_strInfo[8] = LR"(<플러그인>은 뇌에 설치에 각종 확장기능을 얻는 것으로
보통 한 사람에게 하나밖에 적용할 수 없다.
이 프로그램을 도입하면 플러그인 동시 적용 수를
1개 늘릴 수 있다. 같은 계열의 프로그램 H와 성능은 동일.)";

    m_str_K_Info[0] = TEXT("소지금");
    m_str_K_Info[1] = TEXT("구입 후 소지금");
    m_str_K_Info[2] = TEXT("구입 금액");


    for (_int i = 0; i < 3; i++)
    {
        m_tCatalog = { 0, 2000 * (i + 1)};
        m_umapCatalog.insert(make_pair(m_strGift[i], m_tCatalog));
    }

    for (_int i = 0; i < 6; i++)
    {
        if (i % 2 == 0)
            m_tCatalog = { 0, 1000 * (i + 1)};
        else if(i % 2 == 1)
            m_tCatalog = { 0, 1600 * (i + 1)};

        m_umapCatalog.insert(make_pair(m_strItem[i], m_tCatalog));
    }

    return S_OK;
}

void CUI_Shop_Info::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    if (true == g_UIInstance->Get_Enter_ShopUI())
    {
        _int iCatalogIndex = g_UIInstance->Get_Shop_Catalog_Index();
        _int iSubMenuIndex = g_UIInstance->Get_ShopSubMenu_Index();
    }
}

void CUI_Shop_Info::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_2);
}

HRESULT CUI_Shop_Info::Render()
{
    if (true == g_UIInstance->Get_Enter_ShopUI())
    {
        _int iMenuIndex = CUI_Manager::Get_Instance()->Get_ShopMenu_Index();
        _int iCatalogIndex = g_UIInstance->Get_Shop_Catalog_Index();
        _int iSubMenuIndex = g_UIInstance->Get_ShopSubMenu_Index();
        _int iCatalogIndex_2 = iCatalogIndex;


        if (0 == iSubMenuIndex)
        {
            for (_int i = 0; i < 9; i++)
            {
                Bind_Shader(i == iCatalogIndex ? m_pTexture_Frame_Selected[0] : m_pTexture_Frame_Selected[1],
                    m_pTranform_Catalog[i], 5, i == iCatalogIndex ? 1.f : 0.5f);
            }

            Bind_Shader(m_pTexture_Jelly[0], m_pTranform_Texture[0], 5, 0 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Jelly[1], m_pTranform_Texture[1], 5, 1 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Medium_Jelly[0], m_pTranform_Texture[2], 5, 2 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Medium_Jelly[1], m_pTranform_Texture[3], 5, 3 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Max_Jelly[0], m_pTranform_Texture[4], 5, 4 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Max_Jelly[1], m_pTranform_Texture[5], 5, 5 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Gift[0], m_pTranform_Texture[6], 5, 6 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Gift[0], m_pTranform_Texture[7], 5, 7 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Gift[0], m_pTranform_Texture[8], 5, 8 == iCatalogIndex ? 1.f : 0.5f);
        }
        else if (1 == iSubMenuIndex)
        {
            for (_int i = 0; i < 6; i++)
            {
                Bind_Shader(i == iCatalogIndex ? m_pTexture_Frame_Selected[0] : m_pTexture_Frame_Selected[1],
                    m_pTranform_Catalog[i], 5, i == iCatalogIndex ? 1.f : 0.5f);
            }
            Bind_Shader(m_pTexture_Jelly[0], m_pTranform_Texture[0], 5, 0 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Jelly[1], m_pTranform_Texture[1], 5, 1 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Medium_Jelly[0], m_pTranform_Texture[2], 5, 2 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Medium_Jelly[1], m_pTranform_Texture[3], 5, 3 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Max_Jelly[0], m_pTranform_Texture[4], 5, 4 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Max_Jelly[1], m_pTranform_Texture[5], 5, 5 == iCatalogIndex ? 1.f : 0.5f);
        }
        else if (2 == iSubMenuIndex)
        {
            for (_int i = 0; i < 3; i++)
            {
                Bind_Shader(i == iCatalogIndex ? m_pTexture_Frame_Selected[0] : m_pTexture_Frame_Selected[1],
                    m_pTranform_Catalog[i], 5, i == iCatalogIndex ? 1.f : 0.5f);
            }
            Bind_Shader(m_pTexture_Gift[0], m_pTranform_Texture[0], 5, 0 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Gift[0], m_pTranform_Texture[1], 5, 1 == iCatalogIndex ? 1.f : 0.5f);
            Bind_Shader(m_pTexture_Gift[0], m_pTranform_Texture[2], 5, 2 == iCatalogIndex ? 1.f : 0.5f);
        }


        if (iCatalogIndex_2 > 6 || iSubMenuIndex == 2)
        {
            iCatalogIndex_2 = 6;
        }

        Bind_Shader(m_pTexture_Icon[iCatalogIndex_2], m_pTranform_Icon, 4);
        Bind_Shader(m_pTexture_Info, m_pTranform_Info, 4);
        Bind_Shader(m_pTexture_K_Info, m_pTranform_K_Info, 7);
    }

    return S_OK;
}

HRESULT CUI_Shop_Info::Bind_Shader(shared_ptr<CTexture> ptrTexture, shared_ptr<CTransform> ptrTransform, _int iPassIndex, _float fValue)
{
    if (FAILED(ptrTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Info::Render", "Failed to CTexture::Bind_ShaderResourceView");
    }

    if (FAILED(ptrTransform->Bind_OnShader(m_pShader)))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Info::Render", "Failed to CTexture::Bind_OnShader");
    }

    if (g_UIInstance->Get_Buy_CheckBox())
        fValue = 0.1f;

    if(g_UIInstance->Get_Buy_CheckBox())
        Make_Font_Catalog();

    if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &fValue, sizeof(_float))))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_SubMenu::Render", "Failed to CTexture::Bind_RawValue");
    }

    if (FAILED(__super::Render(iPassIndex)))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Info::Render", "Failed to CGameObject::Render");
    }

    if (!g_UIInstance->Get_Buy_CheckBox())
        Make_Font_Catalog();


    Make_Font_Info();

    return S_OK;
}

void CUI_Shop_Info::Make_Font_Catalog()
{
    _int iSubMenu_Index = g_UIInstance->Get_ShopSubMenu_Index();
    _int iMenuIndex = g_UIInstance->Get_ShopMenu_Index();
    _int iCatalog_Index = g_UIInstance->Get_Shop_Catalog_Index();

    _vector BlackColor = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    _vector GrayColor = XMVectorSet(0.1f, 0.1f, 0.1f, 0.1f);
    _vector WhiteColor = XMVectorSet(0.1f, 0.1f, 0.1f, 0.f);
    _vector OtherColor = XMVectorSet(0.96f, 0.81f, 0.74f, 0.f);


    if (g_UIInstance->Get_Buy_CheckBox())
    {
        BlackColor = GrayColor = WhiteColor = OtherColor = XMVectorSet(0.005f, 0.005f, 0.005f, 0.f);
    }

    g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"), TEXT("소지 수"),
        _float2((g_iWinCX * 0.5f) + 500.f, 300.f), OtherColor, 0.5f);
    g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),TEXT(" / 99"),
        _float2((g_iWinCX * 0.5f) + 625.f, 300.f), OtherColor, 0.5f);

/* 카탈로그 폰트 */
    if (0 == iSubMenu_Index)
    {
        for (_int i = 0; i < 6; i++)
        {
            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                m_strItem[i], m_NameFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);

            if (i == 0)
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, i), g_UIInstance->Get_Possession()).iPossession),
                    m_PossessionFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
            }
            else
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, i)).iPossession),
                    m_PossessionFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
            }
            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, i)).iAmount),
                m_AmountFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);

            _int iNum = g_UIInstance->Get_Shop_Number_Index();

            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                i == iCatalog_Index ? Int_Change_to_wstring(iNum) : Int_Change_to_wstring(0),
                m_NumberFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
        }
        for (_int i = 6; i < 9; i++)
        {
            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                m_strGift[i - 6], m_NameFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);

            if (i != 7)
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, i)).iPossession),
                    m_PossessionFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
            }
            else
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, i), g_UIInstance->Get_GiftPossession()).iPossession),
                    m_PossessionFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
            }

            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, i)).iAmount),
                m_AmountFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);

            _int iNum = g_UIInstance->Get_Shop_Number_Index();

            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                i == iCatalog_Index ? Int_Change_to_wstring(iNum) : Int_Change_to_wstring(0),
                m_NumberFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
        }
    }
    if (1 == iSubMenu_Index)
    {
        for (_int i = 0; i < 6; i++)
        {
            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                m_strItem[i], m_NameFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);

            if (i == 0)
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(1, i), g_UIInstance->Get_Possession()).iPossession),
                    m_PossessionFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
            }
            else
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(1, i)).iPossession),
                    m_PossessionFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
            }
            

            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(1, i)).iAmount),
                m_AmountFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);

            _int iNum = g_UIInstance->Get_Shop_Number_Index();

            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                i == iCatalog_Index ? Int_Change_to_wstring(iNum) : Int_Change_to_wstring(0), 
                m_NumberFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
        }
    }

    else if (2 == iSubMenu_Index)
    {
        for (_int i = 0; i < 3; i++)
        {
            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                m_strGift[i], m_NameFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);

            if (i == 1)
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(2, i), g_UIInstance->Get_GiftPossession()).iPossession),
                    m_PossessionFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
            }
            else
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(2, i)).iPossession),
                    m_PossessionFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
            }
            

            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(2, i)).iAmount),
                m_AmountFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);

            _int iNum = g_UIInstance->Get_Shop_Number_Index();

            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                i == iCatalog_Index ? Int_Change_to_wstring(iNum) : Int_Change_to_wstring(0),
                m_NumberFontPos[i], iCatalog_Index == i ? OtherColor : GrayColor, 0.5f);
        }
    }
}

void CUI_Shop_Info::Make_Font_Info()
{
    _int iMenuIndex = g_UIInstance->Get_ShopMenu_Index();
    _int iSubMenu_Index = g_UIInstance->Get_ShopSubMenu_Index();
    _int iCatalogIndex = g_UIInstance->Get_Shop_Catalog_Index();
    _int iNameIndex = iCatalogIndex;

    _vector BlackColor = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    _vector GrayColor = XMVectorSet(0.1f, 0.1f, 0.1f, 0.f);
    _vector WhiteColor = XMVectorSet(0.1f, 0.1f, 0.1f, 0.f);
    _vector OtherColor = XMVectorSet(0.96f, 0.81f, 0.74f, 0.f);

    if (g_UIInstance->Get_Buy_CheckBox())
    {
        BlackColor = GrayColor = WhiteColor = OtherColor = XMVectorSet(0.005f, 0.005f, 0.005f, 0.f);
    }

    if (iSubMenu_Index == 2)
    {
        iNameIndex += 6;
    }

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        m_strName[iNameIndex], _float2((g_iWinCX * 0.5f) + 230.f, 240.f), OtherColor, 0.5f);

    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
        m_strInfo[iNameIndex], _float2((g_iWinCX * 0.5f) + 140.f, 350.f), OtherColor, 0.5f);

    for (_int i = 0; i < 3; i++)
    {
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
            m_str_K_Info[i], _float2((g_iWinCX * 0.5f) + 225.f, (g_iWinCY * 0.5f) + 180.f + (40.f * i)), OtherColor, 0.5f);
    }

/* 소지금 계산 */
    m_str_K[0] = Int_Change_to_wstring(g_UIInstance->Get_K());

    _int iCalculataion = Find_Catalog_Value(Catalog_Name_For_K(iSubMenu_Index, iCatalogIndex)).iAmount *
        g_UIInstance->Get_Shop_Number_Index();

    m_str_K[1] = Int_Change_to_wstring(g_UIInstance->Get_K() - iCalculataion);

    m_str_K[2] = Int_Change_to_wstring(iCalculataion);

    if (g_UIInstance->Get_K() - iCalculataion > 0)
    {
        if (g_UIInstance->Get_Buy())
        {
            g_UIInstance->Set_UI_K(g_UIInstance->Get_K() - iCalculataion);

            if (iSubMenu_Index == 0 || iSubMenu_Index == 1)
            {
                if (iCatalogIndex == 0)
                    g_UIInstance->Set_UI_Possession_Plus(g_UIInstance->Get_Shop_Number_Index());
                else if (iCatalogIndex == 7)
                    g_UIInstance->Set_UI_Gift_Possession_Plus(g_UIInstance->Get_Shop_Number_Index());
            }
            else if (iSubMenu_Index == 2)
            {
                if (iCatalogIndex == 1)
                    g_UIInstance->Set_UI_Gift_Possession_Plus(g_UIInstance->Get_Shop_Number_Index());
            }

            g_UIInstance->Set_Buy(false);
        }
    }

    for (_int i = 0; i < 3; i++)
    {
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"),
            m_str_K[i], _float2((g_iWinCX * 0.5f) + 700.f, (g_iWinCY * 0.5f) + 180.f + (40.f * i)), OtherColor, 0.5f);
    }

/* 소지 수 */
    if (iSubMenu_Index == 0)
    {
        if (iCatalogIndex == 0)
        {
            g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, 0), g_UIInstance->Get_Possession()).iPossession),
                _float2((g_iWinCX * 0.5f) + 600.f, 300.f), OtherColor, 0.5f);
        }
        for (_int i = 1; i < 9; i++)
        {
            if (iCatalogIndex == i)
            {
                if (i != 8)
                {
                    g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                        Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, i)).iPossession),
                        _float2((g_iWinCX * 0.5f) + 600.f, 300.f), OtherColor, 0.5f);
                }
                else 
                {
                    g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                        Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(0, i), g_UIInstance->Get_GiftPossession()).iPossession),
                        _float2((g_iWinCX * 0.5f) + 600.f, 300.f), OtherColor, 0.5f);
                }
                
            }
        }
    }
    else if (iSubMenu_Index == 1)
    {
        for (_int i = 0; i < 6; i++)
        {
            if (iCatalogIndex == 0)
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(1, i), g_UIInstance->Get_Possession()).iPossession),
                    _float2((g_iWinCX * 0.5f) + 600.f, 300.f), OtherColor, 0.5f);
            }
            else if (iCatalogIndex != 0)
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(1, i)).iPossession),
                    _float2((g_iWinCX * 0.5f) + 600.f, 300.f), OtherColor, 0.5f);
            }
        }
    }
    else if (iSubMenu_Index == 2)
    {
        for (_int i = 0; i < 3; i++)
        {
            if (iCatalogIndex == 1)
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(2, i), g_UIInstance->Get_GiftPossession()).iPossession),
                    _float2((g_iWinCX * 0.5f) + 600.f, 300.f), OtherColor, 0.5f);
            }
            else
            {
                g_GameInstance->Render_Font(TEXT("Font_ScarletStrings"),
                    Int_Change_to_wstring(Find_Catalog_Value(Catalog_Name(2, i)).iPossession),
                    _float2((g_iWinCX * 0.5f) + 600.f, 300.f), OtherColor, 0.5f);
            }
        }
    }
}

CUI_Shop_Info::CATALOGDESC CUI_Shop_Info::Find_Catalog_Value(wstring strName, _int iChangePossession)
{
    auto it = m_umapCatalog.find(strName);

    if (it != m_umapCatalog.end()) {
        CATALOGDESC& foundCatalog = it->second;

        foundCatalog.iPossession = iChangePossession;

        return foundCatalog;
    }
    else
    {
        return CATALOGDESC();
    }
}

wstring CUI_Shop_Info::Int_Change_to_wstring(_int iValue)
{
    wstring strValue = TEXT("");

    std::wstringstream wss;
    wss << iValue;

    strValue = wss.str();

    return strValue;
}

wstring CUI_Shop_Info::Catalog_Name(_int iSubMenuIndex, _int iRoop)
{
    wstring strTmp = TEXT("");

    if (iSubMenuIndex == 0)
    {
        if (iRoop == 0)
            strTmp = TEXT("체력(소) 젤리");
        else if (iRoop == 1)
            strTmp = TEXT("전체 : 체력(소) 젤리");
        else if (iRoop == 2)
            strTmp = TEXT("체력(중) 젤리");
        else if (iRoop == 3)
            strTmp = TEXT("전체 : 체력(중) 젤리");
        else if (iRoop == 4)
            strTmp = TEXT("체력(대) 젤리");
        else if (iRoop == 5)
            strTmp = TEXT("전체 : 체력(대) 젤리");
        else if (iRoop == 6)
            strTmp = TEXT("별갑 무늬 안경");
        else if (iRoop == 7)
            strTmp = TEXT("즐거운 과자 모음");
        else if (iRoop == 8)
            strTmp = TEXT("플러그인 병렬화 프로그램 V");
    }
    else if (iSubMenuIndex == 1)
    {
        if (iRoop == 0)
            strTmp = TEXT("체력(소) 젤리");
        else if (iRoop == 1)
            strTmp = TEXT("전체 : 체력(소) 젤리");
        else if (iRoop == 2)
            strTmp = TEXT("체력(중) 젤리");
        else if (iRoop == 3)
            strTmp = TEXT("전체 : 체력(중) 젤리");
        else if (iRoop == 4)
            strTmp = TEXT("체력(대) 젤리");
        else if (iRoop == 5)
            strTmp = TEXT("전체 : 체력(대) 젤리");
    }
    else if (iSubMenuIndex == 2)
    {
        if (iRoop == 0)
            strTmp = TEXT("별갑 무늬 안경");
        else if (iRoop == 1)
            strTmp = TEXT("즐거운 과자 모음");
        else if (iRoop == 2)
            strTmp = TEXT("플러그인 병렬화 프로그램 V");
    }

    return strTmp;
}

wstring CUI_Shop_Info::Catalog_Name_For_K(_int iSubMenuIndex, _int iCatalogIndex)
{
    wstring strTmp = TEXT("");

    if (iSubMenuIndex == 0)
    {
        if (iCatalogIndex == 0)
            strTmp = TEXT("체력(소) 젤리");
        else if (iCatalogIndex == 1)
            strTmp = TEXT("전체 : 체력(소) 젤리");
        else if (iCatalogIndex == 2)
            strTmp = TEXT("체력(중) 젤리");
        else if (iCatalogIndex == 3)
            strTmp = TEXT("전체 : 체력(중) 젤리");
        else if (iCatalogIndex == 4)
            strTmp = TEXT("체력(대) 젤리");
        else if (iCatalogIndex == 5)
            strTmp = TEXT("전체 : 체력(대) 젤리");
        else if (iCatalogIndex == 6)
            strTmp = TEXT("별갑 무늬 안경");
        else if (iCatalogIndex == 7)
            strTmp = TEXT("즐거운 과자 모음");
        else if (iCatalogIndex == 8)
            strTmp = TEXT("플러그인 병렬화 프로그램 V");
    }
    else if (iSubMenuIndex == 1)
    {
        if (iCatalogIndex == 0)
            strTmp = TEXT("체력(소) 젤리");
        else if (iCatalogIndex == 1)
            strTmp = TEXT("전체 : 체력(소) 젤리");
        else if (iCatalogIndex == 2)
            strTmp = TEXT("체력(중) 젤리");
        else if (iCatalogIndex == 3)
            strTmp = TEXT("전체 : 체력(중) 젤리");
        else if (iCatalogIndex == 4)
            strTmp = TEXT("체력(대) 젤리");
        else if (iCatalogIndex == 5)
            strTmp = TEXT("전체 : 체력(대) 젤리");
    }
    else if (iSubMenuIndex == 2)
    {
        if (iCatalogIndex == 0)
            strTmp = TEXT("별갑 무늬 안경");
        else if (iCatalogIndex == 1)
            strTmp = TEXT("즐거운 과자 모음");
        else if (iCatalogIndex == 2)
            strTmp = TEXT("플러그인 병렬화 프로그램 V");
    }

    return strTmp;
}

HRESULT CUI_Shop_Info::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Info::Ready_Components", "Failed to CGameObject::Ready_Components");
    }

    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);

    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Info::Ready_Components", "Failed to Get_Component: SHADER");
    }

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);

    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Info::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }
    return S_OK;
}

shared_ptr<CUI_Shop_Info> CUI_Shop_Info::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Shop_Info> pInstance = make_private_shared(CUI_Shop_Info, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Shop_Info::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Shop_Info::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Shop_Info> pInstance = make_private_shared_copy(CUI_Shop_Info, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Shop_Info::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
