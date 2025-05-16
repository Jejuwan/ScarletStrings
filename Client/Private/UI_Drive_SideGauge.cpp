#include "ClientPCH.h"
#include "UI_Drive_SideGauge.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define HP_SCALE        _float3(3.f, 68.7f, 0.f)
#define HP_POSITION     _float4(208.5f, -409.6f, 0.f, 1.f)
//#define HP_POSITION     _float4(228.5f, -390.f, 0.0f, 1.f)


CUI_Drive_SideGauge::CUI_Drive_SideGauge(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Drive_SideGauge::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Drive_SideGauge::Initialize(any _pPlayerEntityDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    
    m_pDrive_SideGauge  = CTransform::Create(m_pDevice, m_pContext);
    m_pTexture          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Drive/Drive_SideGauge.png"));


    Initialize_UI();

    m_pDrive_SideGauge->Set_Scale(HP_SCALE);
    m_pDrive_SideGauge->Set_State(TRANSFORM::POSITION, HP_POSITION);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Drive_SideGauge::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_tUIDesc.m_fTime += fTimeDelta;
    
    if (m_tUIDesc.m_fFramePerSec <= m_tUIDesc.m_fTime)
    {
        m_tUIDesc.m_iIndex += 1;
        if (m_tUIDesc.m_iIndex >= m_tUIDesc.m_iIndexMaxY * m_tUIDesc.m_iIndexMaxX)
        {
            m_tUIDesc.m_iIndex = 0;
        }
        m_tUIDesc.m_fTime = 0.f;
    }
}

void CUI_Drive_SideGauge::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_1);
}

HRESULT CUI_Drive_SideGauge::Render()
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
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pDrive_SideGauge->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc.m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndex", &m_tUIDesc.m_iIndex, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxX", &m_tUIDesc.m_iIndexMaxX, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxY", &m_tUIDesc.m_iIndexMaxY, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fSizeX", &m_tUIDesc.m_fSizeX, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fSizeY", &m_tUIDesc.m_fSizeY, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(4)))
            {
                MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

void CUI_Drive_SideGauge::Initialize_UI()
{
    ZeroMemory(&m_tUIDesc, sizeof m_tUIDesc);

    m_tUIDesc.m_iIndex          = 0;
    m_tUIDesc.m_iIndexMaxX      = 1;
    m_tUIDesc.m_iIndexMaxY      = 13;
    m_tUIDesc.m_fFramePerSec    = 0.05f;

    m_tUIDesc.m_fTime           = 0.f;
    m_tUIDesc.m_fSizeX          = 3.f;
    m_tUIDesc.m_fSizeY          = 754.f;
}

HRESULT CUI_Drive_SideGauge::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Drive_SideGauge::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Drive_SideGauge> CUI_Drive_SideGauge::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Drive_SideGauge> pInstance = make_private_shared(CUI_Drive_SideGauge, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Drive_SideGauge::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Drive_SideGauge::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Drive_SideGauge> pInstance = make_private_shared_copy(CUI_Drive_SideGauge, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Drive_SideGauge::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
