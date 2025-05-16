#include "ClientPCH.h"
#include "UI_SkillChangerKey_R.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define SKILLSLOT_SCALE                 _float3(95.f, 78.f, 1.f)
#define SKILLSLOT_POSITION              _float4(836.f, -232.f, 0.f, 1.f)

#define SKILLCHANGER_KEYICON_SCALE      _float3(28.f, 20.f, 1.f)
#define SKILLCHANGER_KEYICON_POSITION   _float4(833.f, -232.f, 0.f, 1.f)

CUI_SkillChangerKey_R::CUI_SkillChangerKey_R(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_SkillChangerKey_R::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_SkillChangerKey_R::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    _matrix PivotMatrix = XMMatrixIdentity();
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(-45.f));

    m_pSkillChangerKey_R    = CTransform::Create(m_pDevice, m_pContext);
    m_pSkillChangerKey_Icon = CTransform::Create(m_pDevice, m_pContext);


    m_pTexture              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillChangerKey_R.png"));
    m_pTexture_Icon         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/ALT.png"));


    m_pSkillChangerKey_R->Set_Matrix(PivotMatrix);
    m_pSkillChangerKey_R->Set_Scale(SKILLSLOT_SCALE);
    m_pSkillChangerKey_R->Set_State(TRANSFORM::POSITION, SKILLSLOT_POSITION);
    m_pSkillChangerKey_Icon->Set_Scale(SKILLCHANGER_KEYICON_SCALE);
    m_pSkillChangerKey_Icon->Set_State(TRANSFORM::POSITION, SKILLCHANGER_KEYICON_POSITION);


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_SkillChangerKey_R::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_SkillChangerKey_R::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CUI_SkillChangerKey_R::Render()
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
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillChangerKey_R->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Render", "Failed to CGameObject::Render");
            }

            if (FAILED(m_pTexture_Icon->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillChangerKey_Icon->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

HRESULT CUI_SkillChangerKey_R::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_SkillChangerKey_R::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_SkillChangerKey_R> CUI_SkillChangerKey_R::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_SkillChangerKey_R> pInstance = make_private_shared(CUI_SkillChangerKey_R, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_SkillChangerKey_R::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_SkillChangerKey_R::Clone(any _aDesc)
{
    shared_ptr<CUI_SkillChangerKey_R> pInstance = make_private_shared_copy(CUI_SkillChangerKey_R, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_SkillChangerKey_R::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
