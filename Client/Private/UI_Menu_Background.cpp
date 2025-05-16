#include "ClientPCH.h"
#include "UI_Menu_Background.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "RenderTarget_Manager.h"

#define SCALE_BACK                  _float3(g_iWinCX, g_iWinCY, 1.f)
#define POSITION_BACK               _float4(0.f, 0.f, 0.9f, 1.f)

#define BACKGROUND_FRAME_SCALE      _float3(g_iWinCX, g_iWinCY, 1.f)
#define BACKGROUND_FRAME_POSITION   _float4(0.f, 0.f, 0.8f, 1.f)

#define SEPERATELINE_SCALE          _float3(g_iWinCX, 2.f, 1.f)
#define SEPERATELINE_POSITION       _float4(0.f, 400.f, 0.0f, 1.f)

#define SCALE_INFO_L                _float3(1250.f, 60.f, 1.f)
#define POSITION_INFO_L             _float4(-294.f, -430.f, 0.6f, 1.f)

#define SCALE_INFO_S1               _float3(150.f, 60.f, 1.f)
#define POSITION_INFO_S1            _float4(418.f, -430.f, 0.6f, 1.f)

#define SCALE_INFO_M                _float3(200.f, 60.f, 1.f)
#define POSITION_INFO_M             _float4(606.f, -430.f, 0.6f, 1.f)

#define SCALE_INFO_S2               _float3(180.f, 60.f, 1.f)
#define POSITION_INFO_S2            _float4(808.f, -430.f, 0.6f, 1.f)

#define SCALE_BP                    _float3(27.f, 17.f, 1.f)
#define POSITION_BP                 _float4(385.f, -430.f, 0.5f, 1.f)

#define SCALE_K                     _float3(22.f, 22.f, 1.f)
#define POSITION_K                  _float4(550.f, -430.f, 0.5f, 1.f)

CUI_Menu_Background::CUI_Menu_Background(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Background::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Background::Initialize(any _pPlayerEntityDesc)
{    
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Initialize", "Failed to CGameObject::Initialize");
    }

    m_pBackground           = CTransform::Create(m_pDevice, m_pContext);
    m_pBackground_Frame     = CTransform::Create(m_pDevice, m_pContext);
    m_pSeperate_Line        = CTransform::Create(m_pDevice, m_pContext);
    m_pInfo_L               = CTransform::Create(m_pDevice, m_pContext);
    m_pInfo_S1              = CTransform::Create(m_pDevice, m_pContext);
    m_pInfo_M               = CTransform::Create(m_pDevice, m_pContext);
    m_pInfo_S2              = CTransform::Create(m_pDevice, m_pContext);
    m_pBP                   = CTransform::Create(m_pDevice, m_pContext);
    m_pK                    = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture_Background   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Background.png"));
    m_pTexture_Frame        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/UI_MenuBackground.dds"));
    m_pTexture_Sperate_Line = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Line.png"));
    m_pTexture_Info_L       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Info_L.dds"));
    m_pTexture_Info_S1      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Info_S.png"));
    m_pTexture_Info_M       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Info_M.png"));
    m_pTexture_Info_S2      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Info_S.png"));
    m_pTexture_BP           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/BP.png"));
    m_pTexture_K_Info       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/7k.png"));

    m_pBackground->Set_Scale(SCALE_BACK);
    m_pBackground_Frame->Set_Scale(BACKGROUND_FRAME_SCALE);
    m_pSeperate_Line->Set_Scale(SEPERATELINE_SCALE);
    m_pInfo_L->Set_Scale(SCALE_INFO_L);
    m_pInfo_S1->Set_Scale(SCALE_INFO_S1);
    m_pInfo_M->Set_Scale(SCALE_INFO_M);
    m_pInfo_S2->Set_Scale(SCALE_INFO_S2);
    m_pBP->Set_Scale(SCALE_BP);
    m_pK->Set_Scale(SCALE_K);

    m_pBackground->Set_State(TRANSFORM::POSITION, POSITION_BACK);
    m_pBackground_Frame->Set_State(TRANSFORM::POSITION, BACKGROUND_FRAME_POSITION);
    m_pSeperate_Line->Set_State(TRANSFORM::POSITION, SEPERATELINE_POSITION);
    m_pInfo_L->Set_State(TRANSFORM::POSITION, POSITION_INFO_L);
    m_pInfo_S1->Set_State(TRANSFORM::POSITION, POSITION_INFO_S1);
    m_pInfo_M->Set_State(TRANSFORM::POSITION, POSITION_INFO_M);
    m_pInfo_S2->Set_State(TRANSFORM::POSITION, POSITION_INFO_S2);
    m_pBP->Set_State(TRANSFORM::POSITION, POSITION_BP);
    m_pK->Set_State(TRANSFORM::POSITION, POSITION_K);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Background::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Menu_Background::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);

    TestKey();

    Calculate_Time(fTimeDelta);
    KeyControl();
}

HRESULT CUI_Menu_Background::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        m_strBP         = std::to_wstring(CUI_Manager::Get_Instance()->Get_BP());
        m_strK          = std::to_wstring(CUI_Manager::Get_Instance()->Get_K());
        m_strZero       = TEXT("0");
        m_strTimeSec    = std::to_wstring((_uint)m_fTimeAccSec);
        m_strTimeMin    = std::to_wstring(m_iTimeAccMin);
        m_strTimeHour   = std::to_wstring(m_iTimeAccHour);
        m_strSepertate  = TEXT(":");

        /* Background */
        if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }

        auto pUI_3DTexture = CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_UI_3D);
        
        if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXTARGET_UI_3D, pUI_3DTexture)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }

        if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(26)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* Background_Frame */
        if (FAILED(m_pTexture_Frame->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground_Frame->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(7)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* Seperate_Line */
        if (FAILED(m_pTexture_Sperate_Line->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pSeperate_Line->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(9)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* Info_L */
        if (FAILED(m_pTexture_Info_L->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pInfo_L->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(8)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* Info_S1 */
        if (FAILED(m_pTexture_Info_S1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pInfo_S1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(8)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* Info_M */
        if (FAILED(m_pTexture_Info_M->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pInfo_M->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(8)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* Info_S2 */
        if (FAILED(m_pTexture_Info_S2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pInfo_S2->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(8)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* BP */
        if (FAILED(m_pTexture_BP->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBP->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* K */
        if (FAILED(m_pTexture_K_Info->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pK->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Background::Render", "Failed to CGameObject::Render");
        }

        /* Render Always*/
        if (0 <= CUI_Manager::Get_Instance()->Get_BP() && 10 > CUI_Manager::Get_Instance()->Get_BP())
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strBP, _float2(1395.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else if (10 <= CUI_Manager::Get_Instance()->Get_BP() && 100 > CUI_Manager::Get_Instance()->Get_BP())
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strBP, _float2(1383.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strBP, _float2(1371.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }

        if (0 <= CUI_Manager::Get_Instance()->Get_K() && 10 > CUI_Manager::Get_Instance()->Get_K())
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strK, _float2(1624.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else if (10 <= CUI_Manager::Get_Instance()->Get_K() && 100 > CUI_Manager::Get_Instance()->Get_K())
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strK, _float2(1612.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else if (100 <= CUI_Manager::Get_Instance()->Get_K() && 1000 > CUI_Manager::Get_Instance()->Get_K())
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strK, _float2(1600.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else if (1000 <= CUI_Manager::Get_Instance()->Get_K() && 10000 > CUI_Manager::Get_Instance()->Get_K())
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strK, _float2(1588.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else if (10000 <= CUI_Manager::Get_Instance()->Get_K() && 100000 > CUI_Manager::Get_Instance()->Get_K())
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strK, _float2(1576.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strSepertate, _float2(1790.f, 962.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.45f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strSepertate, _float2(1747.f, 962.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.45f);


        /* Info Script */
        if (0 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_strInfo = TEXT("파티 멤버를 변경합니다.");

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strInfo, _float2(80.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (1 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_strInfo = TEXT("BP를 소비해서 스킬을 습득합니다."); 

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strInfo, _float2(80.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (2 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_strInfo = TEXT("장비를 변경합니다.");

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strInfo, _float2(80.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (3 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_strInfo = TEXT("소지 아이템을 확인합니다.");

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strInfo, _float2(80.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (4 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_strInfo = TEXT("SAS의 버튼 할당을 변경합니다.");

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strInfo, _float2(80.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (5 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_strInfo = TEXT("지금까지의 메인스토리를 확인합니다.");

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strInfo, _float2(80.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (6 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_strInfo = TEXT("브레인 메시지를 확인합니다.");

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strInfo, _float2(80.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (7 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_strInfo = TEXT("옵션을 변경합니다.");

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strInfo, _float2(80.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }

        /* Game PlayTime */
        if (0 <= m_fTimeAccSec && 10 > m_fTimeAccSec)
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strZero,      _float2(1797.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTimeSec,   _float2(1813.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTimeSec,   _float2(1801.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (0 <= m_iTimeAccMin && 10 > m_iTimeAccMin)
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strZero,      _float2(1754.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTimeMin,   _float2(1770.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTimeMin,   _float2(1758.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        if (0 <= m_iTimeAccHour && 10 > m_iTimeAccHour)
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strZero,      _float2(1711.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTimeHour,  _float2(1727.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
        else
        {
            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strTimeHour, _float2(1715.f, 960.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.52f);
        }
    }


    return S_OK;
}

HRESULT CUI_Menu_Background::Ready_Components()
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

void CUI_Menu_Background::KeyControl()
{
    if (CGameInstance::Get_Instance()->Key_Down(VK_UP))
    {
        CUI_Manager::Get_Instance()->Set_Select_MemberIndexMlnus();
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_Menu_PartyMember_Shift.wav", 1.f);
    }
    if (CGameInstance::Get_Instance()->Key_Down(VK_DOWN))
    {
        CUI_Manager::Get_Instance()->Set_Select_MemberIndexPlus();
        CGameInstance::Get_Instance()->PlaySoundW(L"UI_Menu_PartyMember_Shift.wav", 1.f);
    }
}

void CUI_Menu_Background::Calculate_Time(_float fTimeDelta)
{
    m_fTimeAccSec += fTimeDelta;

    if (60.f <= m_fTimeAccSec)
    {
        m_fTimeAccSec = 0.f;
        m_iTimeAccMin += 1;
    }
    if (60 <= m_iTimeAccMin)
    {
        m_iTimeAccMin = 0;
        m_iTimeAccHour += 1;
    }
}

void CUI_Menu_Background::Get_BP()
{
    CUI_Manager::Get_Instance()->Set_UI_BPPlus();
}

void CUI_Menu_Background::Use_BP()
{
    CUI_Manager::Get_Instance()->Set_UI_BPMinus();
}

void CUI_Menu_Background::Get_K()
{
    CUI_Manager::Get_Instance()->Set_UI_KPlus();
}

void CUI_Menu_Background::Use_K()
{
    CUI_Manager::Get_Instance()->Set_UI_KMinus();
}

void CUI_Menu_Background::TestKey()
{
    if (CGameInstance::Get_Instance()->Key_Down(VK_F5))
    {
        Get_BP();
    }
    if (CGameInstance::Get_Instance()->Key_Down(VK_F6))
    {
        Use_BP();
    }
    if (CGameInstance::Get_Instance()->Key_Down(VK_F7))
    {
        Get_K();
    }
    if (CGameInstance::Get_Instance()->Key_Down(VK_F8))
    {
        Use_K();
    }
}

shared_ptr<CUI_Menu_Background> CUI_Menu_Background::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Background> pInstance = make_private_shared(CUI_Menu_Background, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Background::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Background::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Background> pInstance = make_private_shared_copy(CUI_Menu_Background, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Background::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
