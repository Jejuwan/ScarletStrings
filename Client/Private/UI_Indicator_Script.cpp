#include "ClientPCH.h"
#include "UI_Indicator_Script.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Indicator_Script::CUI_Indicator_Script(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Script::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Script::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_aDesc);
    }

    Initialize_UI();

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Script::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Script::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Script::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
        && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
        && true == CUI_Manager::Get_Instance()->Get_TutorialRender()
        && SCENE::TEST == CGameInstance::Get_Instance()->Current_Scene())
    {
        // Background
        if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Category_Item::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Category_Item::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Category_Item::Render", "Failed to CGameObject::Render");
        }

        // NameTag
        if (FAILED(m_pTexture_NameTag->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Category_Item::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pNameTag->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Category_Item::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Category_Item::Render", "Failed to CGameObject::Render");
        }

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strNameTag,   _float2(598.f, 789.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.6f);
        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strText,      _float2(598.f, 879.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.6f);
    }

    return S_OK;
}

void CUI_Indicator_Script::Initialize_UI()
{
//=============================================================== CTransform
    m_pBackground                   = CTransform::Create(m_pDevice, m_pContext);
    m_pNameTag                      = CTransform::Create(m_pDevice, m_pContext);

//=============================================================== CTexture
    m_pTexture_Background           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Script/Background.png"));
    m_pTexture_NameTag              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Script/NameTag.png"));

//=============================================================== _bool

//=============================================================== _int
    
//=============================================================== _float
    m_fRenderTime                   = 0.f;

//=============================================================== _float3
    m_vScale_Background             = _float3(1428.f, 256.f, 1.f);
    m_vScale_NameTag                = _float3(256.f, 28.f, 1.f);

//=============================================================== _float4
    m_vPosition_Background          = _float4(0.f, -350.f, 0.f, 1.f);
    m_vPosition_NameTag             = _float4(-320.f, -280.f, 0.f, 1.f);

//=============================================================== Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pNameTag->Set_Scale(m_vScale_NameTag);

//=============================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pNameTag->Set_State(TRANSFORM::POSITION, m_vPosition_NameTag);

//=============================================================== Set_Position
    m_strNameTag                    = TEXT("오퍼레이터");
    m_strText                       = TEXT("임무 달성. 이것으로 입대 적성시험을 종료한다.");

}

void CUI_Indicator_Script::Tick_UI(_float fTimeDelta)
{

}

void CUI_Indicator_Script::Set_ScriptRenderStart()
{
    CUI_Manager::Get_Instance()->Set_TutorialRender(true);
}

HRESULT CUI_Indicator_Script::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Indicator_Script> CUI_Indicator_Script::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Script> pInstance = make_private_shared(CUI_Indicator_Script, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Script::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Script::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_Script> pInstance = make_private_shared_copy(CUI_Indicator_Script, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Script::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
