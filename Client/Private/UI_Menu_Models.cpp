#include "ClientPCH.h"
#include "UI_Menu_Models.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerState.h"
#include "UI_Manager.h"
#include "Camera_Player.h"

CUI_Menu_Models::CUI_Menu_Models(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Models::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]   = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL]    = make_pair(PROTOTYPE_COMPONENT_MODEL_MENU, g_aNull);

	return S_OK;
}

HRESULT CUI_Menu_Models::Initialize(any _aDesc)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CUI_Menu_Models::Initialize", "Failed to CGameObject::Initialize");
	}

	//m_pPlayer = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	_matrix pivotMatrix = XMMatrixIdentity();
	pivotMatrix			= XMMatrixScaling(0.5f, 0.5f, 0.5f);

	m_pTransform->Set_Matrix(pivotMatrix);

	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_CAMERA);
	m_pCamera = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));

	m_vCameraPosition = static_pointer_cast<CTransform>(m_pCamera->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);


	m_pTransform->Set_State(TRANSFORM::POSITION, m_vCameraPosition);

	return S_OK;
}

void CUI_Menu_Models::Tick(_float fTimeDelta) 
{
	__super::Tick(fTimeDelta);
}

void CUI_Menu_Models::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::UI_3D);

	m_pTransform->Rotate(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(0.2f));

	//_float3 vPlayerPos = static_pointer_cast<CTransform>(m_pPlayer->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
	//m_pTransform->Set_State(TRANSFORM::POSITION, vPlayerPos);

	//shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(SCENE::TEST, LAYER_CAMERA);
	//m_pCamera = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));
	//
	//m_vCameraPosition = static_pointer_cast<CTransform>(m_pCamera->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
	//
	//m_vSofaPosition = m_vCameraPosition;
	//
	//m_vSofaPosition.z += 3.f;
	//
	//m_pTransform->Set_State(TRANSFORM::POSITION, m_vSofaPosition);

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, -0.5f, 3.f, 1.f));
}

HRESULT CUI_Menu_Models::Render()
{
	//if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
	//{
		//if (FAILED(__super::Render(0)))
		//{
		//	MSG_RETURN(E_FAIL, "CUI_Menu_Models::Render", "Failed to CGameObject::Render");
		//}
	//}


	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CUI_Menu_Models::Render()", "Fail to Bind Matrix");
	}

	auto pUIManager = CUI_Manager::Get_Instance();
	if (FAILED(pUIManager->Bind_Matixies(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CUI_Menu_Models::Render()", "Fail to Bind_Matixies");
	}

	if (FAILED(m_pModel->Render(m_pShader, 13)))
	{
		MSG_RETURN(E_FAIL, "CUI_Menu_Models::Render()", "Fail to Render Model");
	}

	return S_OK;
}

HRESULT CUI_Menu_Models::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CUI_Menu_Models::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CUI_Menu_Models::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CUI_Menu_Models::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CUI_Menu_Models::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

shared_ptr<CUI_Menu_Models> CUI_Menu_Models::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CUI_Menu_Models> pInstance = make_private_shared(CUI_Menu_Models, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CUI_Menu_Models::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Models::Clone(any)
{
	shared_ptr<CUI_Menu_Models> pInstance = make_private_shared_copy(CUI_Menu_Models, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CUI_Menu_Models::Clone", "Failed to Initialize");
	}

	return pInstance;
}
