#include "ClientPCH.h"
#include "UI_ItemTag.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define ITEMTAG_SCALE        _float3(260.f, 50.f, 0.f)
#define ITEMTAG_POSITION     _float4(-710.f, -400.f, 0.f, 1.f)

CUI_ItemTag::CUI_ItemTag(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_ItemTag::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_ItemTag::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_ItemTag::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    m_vScale_ItemCount_Ones         = _float3(9.5f, 14.f ,1.f);
    m_vScale_ItemCount_Tens         = _float3(9.5f, 14.f ,1.f);
    m_vPosition_ItemCount_Ones      = _float4(-685.f, -360.f, 0.f, 1.f);
    m_vPosition_ItemCount_Tens      = _float4(-694.f, -360.f, 0.f, 1.f);


    m_pItemTag          = CTransform::Create(m_pDevice, m_pContext);
    m_pOnes             = CTransform::Create(m_pDevice, m_pContext);
    m_pTens             = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemTag.png"));
    m_pTexture_Zero     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/0.png"));
    m_pTexture_One      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/1.png"));
    m_pTexture_Two      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/2.png"));
    m_pTexture_Three    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/3.png"));
    m_pTexture_Four     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/4.png"));
    m_pTexture_Five     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/5.png"));
    m_pTexture_Six      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/6.png"));
    m_pTexture_Seven    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/7.png"));
    m_pTexture_Eight    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/8.png"));
    m_pTexture_Nine     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/9.png"));

    m_pItemTag->Set_Scale(ITEMTAG_SCALE);
    m_pItemTag->Set_State(TRANSFORM::POSITION, ITEMTAG_POSITION);

    m_pOnes->Set_Scale(m_vScale_ItemCount_Ones);
    m_pTens->Set_Scale(m_vScale_ItemCount_Tens);
    m_pOnes->Set_State(TRANSFORM::POSITION, m_vPosition_ItemCount_Ones);
    m_pTens->Set_State(TRANSFORM::POSITION, m_vPosition_ItemCount_Tens);


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_ItemTag::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_ItemTag::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_ItemTag::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_1);

    Split_ItemCount();
}

HRESULT CUI_ItemTag::Render()
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
                MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pItemTag->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CGameObject::Render");
            }

            if (0 == CUI_Manager::Get_Instance()->Get_ItemIndex())
            {
                m_strItemTag = L"회복(소) 젤리";
                CGameInstance::Get_Instance()->Render_Font(L"Font_ScarletStrings", m_strItemTag, _float2(205.f, 931.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.42f, -0.05f);
            
#pragma region Tens
                switch (m_iTens)
                {
                case 0:
                {
                    if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 1:
                {
                    if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                default:
                    break;
                }
                if (FAILED(m_pTens->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CGameObject::Render");
                }
#pragma endregion

#pragma region Ones
                switch (m_iOnes)
                {
                case 0:
                {
                    if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 1:
                {
                    if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 2:
                {
                    if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 3:
                {
                    if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 4:
                {
                    if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 5:
                {
                    if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 6:
                {
                    if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 7:
                {
                    if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 8:
                {
                    if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 9:
                {
                    if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                default:
                    break;
                }
                if (FAILED(m_pOnes->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_ItemTag::Render", "Failed to CGameObject::Render");
                }
#pragma endregion            
            }
            if (1 == CUI_Manager::Get_Instance()->Get_ItemIndex())
            {
                m_strItemTag = L"회복(대) 젤리";
                CGameInstance::Get_Instance()->Render_Font(L"Font_ScarletStrings", m_strItemTag, _float2(205.f, 931.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.42f, -0.05f);
            }
            if (2 == CUI_Manager::Get_Instance()->Get_ItemIndex())
            {
                m_strItemTag = L"SAS 보충수";
                CGameInstance::Get_Instance()->Render_Font(L"Font_ScarletStrings", m_strItemTag, _float2(205.f, 931.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.42f, -0.05f);
            }
        }
    }

    return S_OK;
}

void CUI_ItemTag::Split_ItemCount()
{
    // Renewal
    m_iItemCount = CUI_Manager::Get_Instance()->Get_ItemCount();

    // Split - Tens
    m_iTens = (m_iItemCount / 10) & 10;
    
    // Split - Ones
    m_iOnes = m_iItemCount % 10;
}

HRESULT CUI_ItemTag::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_ItemTag::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_ItemTag::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_ItemTag> CUI_ItemTag::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_ItemTag> pInstance = make_private_shared(CUI_ItemTag, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_ItemTag::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_ItemTag::Clone(any _aDesc)
{
    shared_ptr<CUI_ItemTag> pInstance = make_private_shared_copy(CUI_ItemTag, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_ItemTag::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

