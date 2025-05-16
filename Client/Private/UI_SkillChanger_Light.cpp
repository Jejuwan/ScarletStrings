#include "ClientPCH.h"
#include "UI_SkillChanger_Light.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define SKILLSLOT_SCALE        _float3(35.f, 20.f, 1.f)
#define SKILLSLOT_POSITION_L   _float4(663.f, -154.2f, 0.f, 1.f)
#define SKILLSLOT_POSITION_R   _float4(762.f, -154.2f, 0.f, 1.f)

CUI_SkillChanger_Light::CUI_SkillChanger_Light(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_SkillChanger_Light::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_SkillChanger_Light::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChanger_Light::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    m_pSkillChanger_Light = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChanger_Light.png"));

    m_pSkillChanger_Light->Set_Scale(SKILLSLOT_SCALE);
    //m_pSkillChanger_Light->Set_State(TRANSFORM::POSITION, SKILLSLOT_POSITION_L);

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChanger_Light::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_SkillChanger_Light::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_SkillChanger_Light::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);

    Set_LightLR();
}

HRESULT CUI_SkillChanger_Light::Render()
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
                MSG_RETURN(E_FAIL, "CUI_SkillChanger_Light::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillChanger_Light->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChanger_Light::Render", "Failed to CTexture::Bind_OnShader");
            }

            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChanger_Light::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

HRESULT CUI_SkillChanger_Light::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChanger_Light::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChanger_Light::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_SkillChanger_Light::Set_LightLR()
{
#pragma region Key_Input
    if (CGameInstance::Get_Instance()->Key_Down(VK_LCONTROL))
        m_iIsLeft = true;
    if (CGameInstance::Get_Instance()->Key_Down(VK_MENU))
        m_iIsLeft = false;
#pragma endregion

    if (true == m_iIsLeft)
        m_pSkillChanger_Light->Set_State(TRANSFORM::POSITION, SKILLSLOT_POSITION_L);
    else
        m_pSkillChanger_Light->Set_State(TRANSFORM::POSITION, SKILLSLOT_POSITION_R);
}

shared_ptr<CUI_SkillChanger_Light> CUI_SkillChanger_Light::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_SkillChanger_Light> pInstance = make_private_shared(CUI_SkillChanger_Light, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_SkillChanger_Light::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_SkillChanger_Light::Clone(any _aDesc)
{
    shared_ptr<CUI_SkillChanger_Light> pInstance = make_private_shared_copy(CUI_SkillChanger_Light, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_SkillChanger_Light::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
