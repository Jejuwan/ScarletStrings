#include "ClientPCH.h"
#include "UI_Drive_Background.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define BACKGROUND_SCALE        _float3(70.f, 80.f, 0.f)
#define BACKGROUND_POSITION     _float4(241.f, -410.f, 0.f, 1.f)

CUI_Drive_Background::CUI_Drive_Background(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Drive_Background::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Drive_Background::Initialize(any _pPlayerEntityDesc)
{    
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Drive_Background::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    
    m_pBackground       = CTransform::Create(m_pDevice, m_pContext);
    //m_pHP               = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_Background.png"));

    m_pBackground->Set_Scale(BACKGROUND_SCALE);
    m_pBackground->Set_State(TRANSFORM::POSITION, BACKGROUND_POSITION);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Drive_Background::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Drive_Background::Tick(_float fTimeDelta)
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

void CUI_Drive_Background::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CUI_Drive_Background::Render()
{
    if (SCENE::TEST != CGameInstance::Get_Instance()->Current_Scene())
    {
        if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
            && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
            && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
            && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
            && SCENE::HIDEOUT != CGameInstance::Get_Instance()->Current_Scene()
            && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene())
        {
            if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_Background::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_Background::Render", "Failed to CTexture::Bind_OnShader");
            }

            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_Background::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

HRESULT CUI_Drive_Background::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Drive_Background::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Drive_Background::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Drive_Background> CUI_Drive_Background::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Drive_Background> pInstance = make_private_shared(CUI_Drive_Background, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Drive_Background::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Drive_Background::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Drive_Background> pInstance = make_private_shared_copy(CUI_Drive_Background, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Drive_Background::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
