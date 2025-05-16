#include "ClientPCH.h"
#include "UI_HP_Background.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define BACKGROUND_SCALE        _float3(450.f, 80.f, 1.f)
#define BACKGROUND_POSITION     _float4(-20.f, -400.f, 0.f, 1.f)

CUI_HP_Background::CUI_HP_Background(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_HP_Background::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_HP_Background::Initialize(any _pPlayerEntityDesc)
{    
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_HP_Background::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    
    m_strPlayerName     = L"카사네 랜들";

    m_pBackground       = CTransform::Create(m_pDevice, m_pContext);
    //m_pHP               = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Background2.png"));

    m_pBackground->Set_Scale(BACKGROUND_SCALE);
    m_pBackground->Set_State(TRANSFORM::POSITION, BACKGROUND_POSITION);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_HP_Background::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_HP_Background::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    //_float  fHPFactor(m_pEntityDesc->fHP / m_pEntityDesc->fMaxHP);
    //_float3 vHealthScale(HEALTH_SCALE), vHealthPosition(BACKGROUND_POSITION);

    //vHealthScale.x += fHPFactor;
    //vHealthPosition.x = HEALTH_SCALE.x * Function::Lerp(-0.5f, 0.f, std::clamp(fHPFactor, 0.f, 1.f));

    //_float3 vHealthScale(HEALTH_SCALE), vHealthPosition(BACKGROUND_POSITION);


    //m_pHealth->Set_Scale(vHealthScale);
    //m_pHealth->Set_State(TRANSFORM::POSITION, vHealthPosition);
}

void CUI_HP_Background::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CUI_HP_Background::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI() 
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
        && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
        && false == CUI_Manager::Get_Instance()->Get_TutorialRender()
        && SCENE::HIDEOUT != CGameInstance::Get_Instance()->Current_Scene()
        && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene())
    {
        if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_HP_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP_Background::Render", "Failed to CTexture::Bind_OnShader");
        }

        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP_Background::Render", "Failed to CGameObject::Render");
        }

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strPlayerName, _float2(g_iWinCX * 0.5f - 250.f, g_iWinCY * 0.5f + 451.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.4f);
    }

    return S_OK;
}

HRESULT CUI_HP_Background::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_HP_Background::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_HP_Background::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_HP_Background> CUI_HP_Background::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_HP_Background> pInstance = make_private_shared(CUI_HP_Background, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_HP_Background::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_HP_Background::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_HP_Background> pInstance = make_private_shared_copy(CUI_HP_Background, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_HP_Background::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
