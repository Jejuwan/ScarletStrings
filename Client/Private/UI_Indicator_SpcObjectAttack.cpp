#include "ClientPCH.h"
#include "UI_Indicator_SpcObjectAttack.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "InteractiveObject.h"

CUI_Indicator_SpcObjectAttack::CUI_Indicator_SpcObjectAttack(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_SpcObjectAttack::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_SpcObjectAttack::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    else
    {
        //m_fGauge    = 0.f;
        //m_vPosition = _float4(-700.f, 0.f, 0.f, 1.f);
    }

    m_bRenderDegree = false;
    m_bRenderSwitch = false;

    m_pIndicator_SpecialObjectAttack = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Indicator_G.png"));

    m_pIndicator_SpecialObjectAttack->Set_Scale(CUI_Manager::Get_Instance()->Get_IndicatorScale());
    m_pIndicator_SpecialObjectAttack->Set_State(TRANSFORM::POSITION, m_vPosition);

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_SpcObjectAttack::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_SpcObjectAttack::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);

    //Set_IndicatorPosition();
}

HRESULT CUI_Indicator_SpcObjectAttack::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene())
    {
        if (true == m_bRenderSwitch && true == m_bRenderDegree)
        {
            if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pIndicator_SpecialObjectAttack->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack::Render", "Failed to CTexture::Bind_OnShader");
            }

            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_SpcObjectAttack::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_SpcObjectAttack::Ready_PsycoAttack(shared_ptr<CInteractiveObject> pFocusedObject, _float fTimeDelta)
{
    if (pFocusedObject)
    {
        Set_IndicatorPosition(pFocusedObject->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION),
            pFocusedObject->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_Matrix());
    }
}

void CUI_Indicator_SpcObjectAttack::Set_IndicatorPosition(_vectorf vWorldPosition, _matrixf _WorldMatrix)
{
    _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);

    _vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

    /* World */
    vPosition = XMVector3TransformCoord(vPosition, _WorldMatrix);

    _vector vLookCam = MatCam.r[3] - vPosition ;

    _vector vCrossLookCam = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));

    vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);

    _float fScreenX = XMVectorGetX(vPosition);
    _float fScreenY = XMVectorGetY(vPosition);

    m_vPosition.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition.y = (fScreenY) * 0.5f * g_iWinCY;


    m_pIndicator_SpecialObjectAttack->Set_State(TRANSFORM::POSITION, m_vPosition);


    m_bRenderSwitch = true;

//=========================================================================================================================== Culling
    // Look_Vector (Monster ---> Camera)
    _vector vLookCamera_FromMonster = MatCam.r[3] - _WorldMatrix.r[3];
    _vector vLookMonster_FromCamera = _WorldMatrix.r[3] - MatCam.r[3];

    _float fDotProduct = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLookCamera_FromMonster), XMVector3Normalize(MatCam.r[2])))));

    if (90.f <= fDotProduct && 270.f >= fDotProduct)
    {
        m_bRenderDegree = true;
    }
    else
    {
        m_bRenderDegree = false;
    }
}

void CUI_Indicator_SpcObjectAttack::Set_RenderOff()
{
    m_bRenderSwitch = false;
}

shared_ptr<CUI_Indicator_SpcObjectAttack> CUI_Indicator_SpcObjectAttack::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_SpcObjectAttack> pInstance = make_private_shared(CUI_Indicator_SpcObjectAttack, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_SpcObjectAttack::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_SpcObjectAttack::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_SpcObjectAttack> pInstance = make_private_shared_copy(CUI_Indicator_SpcObjectAttack, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_SpcObjectAttack::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
