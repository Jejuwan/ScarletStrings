#include "ClientPCH.h"
#include "Monster_Weak.h"
#include "Player.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Model.h"

CMonster_Weak::CMonster_Weak(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMonster_Weak::CMonster_Weak(const CMonster_Weak& _rhs)
	:CGameObject(_rhs)
{
}

CMonster_Weak::~CMonster_Weak()

{
}

HRESULT CMonster_Weak::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
    //m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
    m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_WEAK, g_aNull);

    return S_OK;
}

HRESULT CMonster_Weak::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Weak::Initialize", "Failed to CGameObject::Initialize");
	}

	WEAKDESC Desc = any_cast<WEAKDESC>(pArg);

	m_PivotMatrix = Desc.PivotMatrix;
	m_pParentMatrix = Desc.pParentMatrix;
	m_pWeakBoneMatrix = Desc.pWeakBoneMatrix;
	m_ControllerPivot = Desc.ControllerPivot;
		
	return S_OK;
}

void CMonster_Weak::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix BoneMatrix = XMLoadFloat4x4(m_pWeakBoneMatrix);
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
	
	_matrix ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
	
	m_pTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* m_PivotMatrix
		* ParentMatrix
	);

}

void CMonster_Weak::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::NEON);
	//Add_RenderObject(RENDER_GROUP::OUTLINE);
}

HRESULT CMonster_Weak::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Weak::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMonster_Weak::Render_Neon()
{
	m_pShader->Bind_Float("g_fAccTime", 0.f);
	if (FAILED(__super::Render(14)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Weak::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMonster_Weak::Render_OutLine()
{
	return S_OK;
}

HRESULT CMonster_Weak::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Weak::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Weak::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Weak::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Weak::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

shared_ptr<CMonster_Weak> CMonster_Weak::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Weak> pInstance = make_private_shared(CMonster_Weak, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Weak::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Weak::Clone(any pArg)
{
	shared_ptr<CMonster_Weak> pInstance = make_private_shared_copy(CMonster_Weak, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Weak::Clone", "Failed to Initialize");
	}

	return pInstance;
}
