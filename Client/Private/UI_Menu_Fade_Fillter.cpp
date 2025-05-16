#include "ClientPCH.h"
#include "UI_Menu_Fade_Fillter.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "PhysXEngine.h"
#include "MenuCharacter.h"

CUI_Menu_Fade_Fillter::CUI_Menu_Fade_Fillter(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Fade_Fillter::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Fade_Fillter::Initialize(any _aDesc)
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
        MSG_RETURN(E_FAIL, "CUI_Menu_Fade_Fillter::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Fade_Fillter::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Menu_Fade_Fillter::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_4);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Menu_Fade_Fillter::Render()
{
    /* Fade */
    if (FAILED(m_pTexture_Fade->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fade_Fillter::Render", "Failed to CTexture::Bind_ShaderResourceView");
    }
    if (FAILED(m_pFade->Bind_OnShader(m_pShader)))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fade_Fillter::Render", "Failed to CTexture::Bind_OnShader");
    }
    if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFadeAlpha, sizeof(_float))))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fade_Fillter::Render", "Failed to CTexture::Bind_RawValue");
    }
    if (FAILED(__super::Render(25)))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fade_Fillter::Render", "Failed to CGameObject::Render");
    }


    return S_OK;
}

HRESULT CUI_Menu_Fade_Fillter::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fade_Fillter::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fade_Fillter::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK; 
}

void CUI_Menu_Fade_Fillter::Initialize_UI()
{
    m_vScale_Fade       = _float3(g_iWinCX, g_iWinCY, 1.f);
    m_vPosition_Fade    = _float4(0.f, 0.f, 0.f, 1.f);

    m_iSoundIndex       = 0;

    m_fFadeAlpha        = 0.f;
    m_fSoundDelay       = 0.f;
}

void CUI_Menu_Fade_Fillter::Tick_UI(_float fTimeDelta)
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

void CUI_Menu_Fade_Fillter::Fade_InOut(_float fTimeDelta)
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (0 <= CUI_Manager::Get_Instance()->Get_FadeInOut())
        {
            if (true == CUI_Manager::Get_Instance()->Get_UI_CanFade())
            {
                if (0 == m_iSoundIndex)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Open.wav", 1.5f);
                    ++m_iSoundIndex;
                }

                m_fFadeAlpha += fTimeDelta;

                if (1.f <= m_fFadeAlpha)
                {
                    m_fFadeAlpha = 1.f;
                    CUI_Manager::Get_Instance()->Set_OnOffSwitch_RenderUI(true);

                    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
                    {
                        CUI_Manager::Get_Instance()->Set_MenuSwitchOn();


                        auto pGameInstance = CGameInstance::Get_Instance();
                        pGameInstance->Enable_Lensflare(false);
                        pGameInstance->Iterate_Layers(CGameInstance::Get_Instance()->Current_Scene(), [](pair<wstring, shared_ptr<CObjectLayer>> tLayerDesc)
                            {
                                if (tLayerDesc.first == LAYER_UI)
                                    return true;

                                if (tLayerDesc.first == LAYER_INTERACTIVE_LATE)
                                    return true;

                                if (tLayerDesc.first == LAYER_UI_3D)
                                {
                                   /* tLayerDesc.second->Iterate_Objects([](shared_ptr<CGameObject> pObject)
                                    {
                                        pObject->Set_Enable(true);
                                        return true;
                                    });*/

                                    return true;
                                }

                                tLayerDesc.second->Set_Enable(false);

                                return true;
                            });
                    }
                    //CGameInstance::Get_Instance()->Fix_Cursor(false);
                    //CGameInstance::Get_Instance()->Show_Cursor(true);
                    CUI_Manager::Get_Instance()->Set_UI_CanFadeFalse();
                    //CGameInstance::Get_Instance()->Set_BloomStrength(0.8f);
                    //CGameInstance::Get_Instance()->Set_Exposure(2.f);   // Default = 2.f;
                    //CGameInstance::Get_Instance()->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.9f);     // Default = 2.2f;
                }
            }
            else
            {
                m_fFadeAlpha -= fTimeDelta;

                if (0.f >= m_fFadeAlpha)
                {
                    m_fFadeAlpha = 0.f;
                    CUI_Manager::Get_Instance()->Set_UI_CanFadeTrue();
                    CUI_Manager::Get_Instance()->Set_UI_FadeInOutDefault();
                }
            }
        }
    }
    else
    {
        if (0 <= CUI_Manager::Get_Instance()->Get_FadeInOut())
        {
            if (true == CUI_Manager::Get_Instance()->Get_UI_CanFade())
            {
                if (0 != m_iSoundIndex)
                {
                    CGameInstance::Get_Instance()->PlaySoundW(L"UI_04.Menu_Close.wav", 1.f);

                    m_iSoundIndex = 0;

                    // FadeOut Character UI
                    auto pGameInstance = CGameInstance::Get_Instance();
                    auto pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_UI_3D);
                    pLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
                        {
                            auto pMenuCharacter = dynamic_pointer_cast<CMenuCharacter>(pObject);
                            pMenuCharacter->Fade_Out();

                            return true;
                        });
                }
                m_fFadeAlpha += fTimeDelta;

                if (1.f <= m_fFadeAlpha)
                {
                    m_fFadeAlpha = 1.f;
                    CUI_Manager::Get_Instance()->Set_OnOffSwitch_RenderUI(false);

                    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
                    {
                        CUI_Manager::Get_Instance()->Set_MenuSwitchOff();


                        auto pGameInstance = CGameInstance::Get_Instance();
                        pGameInstance->Enable_Lensflare(true);
                        pGameInstance->Iterate_Layers(CGameInstance::Get_Instance()->Current_Scene(), [](pair<wstring, shared_ptr<CObjectLayer>> tLayerDesc)
                            {
                                if (tLayerDesc.first == LAYER_UI)
                                    return true;

                                if (tLayerDesc.first == LAYER_INTERACTIVE_LATE)
                                    return true;

                                if (tLayerDesc.first == LAYER_UI_3D)
                                {
                                    tLayerDesc.second->Iterate_Objects([](shared_ptr<CGameObject> pObject)
                                        {
                                            pObject->Set_Enable(false);
                                            return true;
                                        });

                                    return true;
                                }

                                tLayerDesc.second->Set_Enable(true);

                                return true;
                            });
                    }

                    //CGameInstance::Get_Instance()->Fix_Cursor(true);
                    CUI_Manager::Get_Instance()->Set_UI_CanFadeFalse();
                }
            }
            else
            {
                m_fFadeAlpha -= fTimeDelta;

                if (0.f >= m_fFadeAlpha)
                {
                    m_fFadeAlpha = 0.f;
                    CGameInstance::Get_Instance()->Fix_Cursor(false);

                    CUI_Manager::Get_Instance()->Set_UI_CanFadeTrue();
                    CUI_Manager::Get_Instance()->Set_UI_FadeInOutDefault();
                }
            }
        }
    }
}

shared_ptr<CUI_Menu_Fade_Fillter> CUI_Menu_Fade_Fillter::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Fade_Fillter> pInstance = make_private_shared(CUI_Menu_Fade_Fillter, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Fade_Fillter::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Fade_Fillter::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Fade_Fillter> pInstance = make_private_shared_copy(CUI_Menu_Fade_Fillter, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Fade_Fillter::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
