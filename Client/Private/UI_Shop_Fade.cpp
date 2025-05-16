#include "ClientPCH.h"
#include "UI_Shop_Fade.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "PhysXEngine.h"

CUI_Shop_Fade::CUI_Shop_Fade(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Shop_Fade::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Shop_Fade::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Initialize", "Failed to CGameObject::Initialize");
    }

    m_pFade                 = CTransform::Create(m_pDevice, m_pContext);
    m_pTexture_Fade         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Black1px.png"));

    Initialize_UI();

    m_pFade->Set_Scale(m_vScale_Fade);
    m_pFade->Set_State(TRANSFORM::POSITION, m_vPosition_Fade);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Fade::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Shop_Fade::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Shop_Fade::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_4);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Shop_Fade::Render()
{
    /* Fade */
    if (FAILED(m_pTexture_Fade->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Fade::Render", "Failed to CTexture::Bind_ShaderResourceView");
    }
    if (FAILED(m_pFade->Bind_OnShader(m_pShader)))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Fade::Render", "Failed to CTexture::Bind_OnShader");
    }
    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFadeAlpha, sizeof(_float))))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Fade::Render", "Failed to CTexture::Bind_RawValue");
    }
    if (FAILED(__super::Render(25)))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Fade::Render", "Failed to CGameObject::Render");
    }


    return S_OK;
}

HRESULT CUI_Shop_Fade::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Fade::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Shop_Fade::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK; 
}

void CUI_Shop_Fade::Initialize_UI()
{
    m_vScale_Fade       = _float3(g_iWinCX, g_iWinCY, 1.f);
    m_vPosition_Fade    = _float4(0.f, 0.f, 0.f, 1.f);

    m_iSoundIndex       = 0;

    m_fFadeAlpha        = 0.f;
    m_fSoundDelay       = 0.f;
}

void CUI_Shop_Fade::Tick_UI(_float fTimeDelta)
{
    Fade_InOut(fTimeDelta);
#ifdef _DEBUG
    if (CGameInstance::Get_Instance()->Get_PhysXEngine()->GetDrawStatus())
        m_vPosition_Fade = _float4(0.f, 0.f, 1.f, 1.f);
    else
#endif // _DEBUG
        m_vPosition_Fade = _float4(0.f, 0.f, 0.f, 1.f);
    m_pFade->Set_State(TRANSFORM::POSITION, m_vPosition_Fade);
}

void CUI_Shop_Fade::Fade_InOut(_float fTimeDelta)
{
    if (true == CUI_Manager::Get_Instance()->Get_IsShop()) /* f5를 누르고 상정 UI 들어가기*/
    {
        if (!m_bEnter_ShopUI) /* 상점 UI로 Fadeout 되기 전*/
        {
            if (0 == m_iSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Open.wav", 1.5f);
                ++m_iSoundIndex;
            }

            m_fFadeAlpha += fTimeDelta;

            if (1.f <= m_fFadeAlpha) /* 상점 UI 화면 나오는 시점 */
            {
                m_fFadeAlpha = 1.f;
                CUI_Manager::Get_Instance()->Set_MenuSwitchOn();
                CUI_Manager::Get_Instance()->Set_Enter_ShopUI(true);

                m_bEnter_ShopUI = true;
            }
        }
        else
        {
            m_fFadeAlpha -= fTimeDelta;

            if (0.f >= m_fFadeAlpha)
            {
                m_fFadeAlpha = 0.f;
            }
        }
    }
    else /* f5를 누르고 상점 UI를 나가기*/
    {
        if (m_bEnter_ShopUI) /* 상점 UI가 켜져 있었을 때 */
        {
            if (0 != m_iSoundIndex)
            {
                CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Close.wav", 1.f);

                m_iSoundIndex = 0;
            }
            m_fFadeAlpha += fTimeDelta;

            if (1.f <= m_fFadeAlpha) /* 이전 화면이 Fadein 되는 시점 */
            {
                m_fFadeAlpha = 0.f;
                CUI_Manager::Get_Instance()->Set_MenuSwitchOff();
                CUI_Manager::Get_Instance()->Set_Enter_ShopUI(false);
                m_bEnter_ShopUI = false;

            }
        }
        else 
        {
            m_fFadeAlpha -= fTimeDelta;
            if (0.f >= m_fFadeAlpha)
            {
                m_fFadeAlpha = 0.f;
                //CGameInstance::Get_Instance()->Fix_Cursor(false);
            }
        }
    }
}

shared_ptr<CUI_Shop_Fade> CUI_Shop_Fade::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Shop_Fade> pInstance = make_private_shared(CUI_Shop_Fade, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Shop_Fade::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Shop_Fade::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Shop_Fade> pInstance = make_private_shared_copy(CUI_Shop_Fade, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Shop_Fade::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
