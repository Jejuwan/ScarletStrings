#include "ClientPCH.h"
#include "UI_SkillChangerKey_L.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define SKILLSLOT_SCALE                 _float3(80.f, 68.f, 1.f)
#define SKILLSLOT_POSITION              _float4(603.f, -230.f, 0.f, 1.f)

#define SKILLCHANGER_KEYICON_SCALE      _float3(27.f, 19.f, 1.f)
#define SKILLCHANGER_KEYICON_POSITION   _float4(601.f, -228.f, 0.f, 1.f)

CUI_SkillChangerKey_L::CUI_SkillChangerKey_L(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_SkillChangerKey_L::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_SkillChangerKey_L::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }


    _matrix PivotMatrix = XMMatrixIdentity();
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(45.f));

    m_pSkillChangerKey_L    = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillChangerKey_ICon = CTransform::Create(m_pDevice, m_pContext);


    m_pTexture              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_L.png"));
    m_pTexture_Icon         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/CTRL.png"));


    m_pSkillChangerKey_L->Set_Matrix(PivotMatrix);
    m_pSkillChangerKey_L->Set_Scale(SKILLSLOT_SCALE);
    m_pSkillChangerKey_L->Set_State(TRANSFORM::POSITION, SKILLSLOT_POSITION);
    m_pSkillChangerKey_ICon->Set_Scale(SKILLCHANGER_KEYICON_SCALE);
    m_pSkillChangerKey_ICon->Set_State(TRANSFORM::POSITION, SKILLCHANGER_KEYICON_POSITION);


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_SkillChangerKey_L::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_SkillChangerKey_L::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CUI_SkillChangerKey_L::Render()
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
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillChangerKey_L->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Render", "Failed to CGameObject::Render");
            }

            if (FAILED(m_pTexture_Icon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillChangerKey_ICon->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Render", "Failed to CGameObject::Render");
            }
        }
    }
 

    return S_OK;
}

HRESULT CUI_SkillChangerKey_L::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_L::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_SkillChangerKey_L> CUI_SkillChangerKey_L::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_SkillChangerKey_L> pInstance = make_private_shared(CUI_SkillChangerKey_L, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_SkillChangerKey_L::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_SkillChangerKey_L::Clone(any _aDesc)
{
    shared_ptr<CUI_SkillChangerKey_L> pInstance = make_private_shared_copy(CUI_SkillChangerKey_L, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_SkillChangerKey_L::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
