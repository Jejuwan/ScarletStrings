#include "ClientPCH.h"
#include "PlayerTentacle.h"
#include "Player.h"
#include "GameInstance.h"
#include "Bone.h"
#include "Animation.h"
#include "Monster_Karen.h"

CPlayerTentacle::CPlayerTentacle(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

CPlayerTentacle::CPlayerTentacle(const CPlayerTentacle& _rhs)
    :CGameObject(_rhs)
{
}

CPlayerTentacle::~CPlayerTentacle()
{
}

HRESULT CPlayerTentacle::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
    m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
    m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE, g_aNull);

    return S_OK;
}

HRESULT CPlayerTentacle::Initialize(any pArg)
{
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_PLAYER_TENTACLE, &m_tMaterialDesc);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CPlayerTentacle::Initialize", "Failed to CGameObject::Initialize");
	}

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (pArg.has_value())
		m_TentacleDesc = any_cast<TENTACLEDESC>(pArg);

	m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
	return S_OK;
}

void CPlayerTentacle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();


	if(!m_bKaren)
	{
		_matrix BoneMatrix = pGameInstance->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone(m_TentacleDesc.szBoneName.c_str())->Get_CombinedTransformation();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		_matrix PivotMatrix = pGameInstance->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Pivot();

		m_pTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f)
			* BoneMatrix
			* PivotMatrix
			* pGameInstance->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_Matrix()
		);

		if (m_TentacleDesc.szBoneName == "Spine1")
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(90.f));
		else
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(-90.f));

		if (m_bFly)
		{
			m_bFly = false;
			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_TentacleDesc.index - 1, 3.f, false, 0.f, false);
		}
		if (Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_TentacleDesc.index - 1) &&
			!Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::TENTACLE::COMMON))
		{
			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::TENTACLE::COMMON, 10.f, false, 0.f, true);
		}

		// 0 ~ 1.6
		//ImGui::SliderFloat("Gradient Factor", &m_fGradientFactor, 0.f, 1.6f);
		m_fGradientFactor += fTimeDelta * 0.1f;
	}
	else
	{
		shared_ptr<CObjectLayer> pMonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
		_float fMinDist = 300.f;
		shared_ptr<CMonster_Karen> pKaren;
		pMonLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				if (nullptr == dynamic_pointer_cast<CMonster_Karen>(_pObject))
					return true;

				pKaren = dynamic_pointer_cast<CMonster_Karen>(_pObject);

				return true;
			});

		_matrix BoneMatrix = pKaren->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone(m_TentacleDesc.szBoneName.c_str())->Get_CombinedTransformation();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		_matrix PivotMatrix = pKaren->Get_Component<CModel>(COMPONENT::MODEL)->Get_Pivot();

		m_pTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f)
			* BoneMatrix
			* PivotMatrix
			*pKaren->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_Matrix()
		);

	/*	if (m_TentacleDesc.szBoneName == "Spine1")
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(90.f));
		else
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(-90.f));*/

		if (m_bFly)
		{
			m_bFly = false;
			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_TentacleDesc.index - 1, 3.f, false, 0.f, false);
		}
		if (Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_TentacleDesc.index - 1) &&
			!Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::TENTACLE::COMMON))
		{
			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::TENTACLE::COMMON, 10.f, false, 0.f, true);
		}

		// 0 ~ 1.6
		//ImGui::SliderFloat("Gradient Factor", &m_fGradientFactor, 0.f, 1.6f);
		m_fGradientFactor += fTimeDelta * 0.1f;
	}
}

void CPlayerTentacle::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	
	if (m_bTransparency)
	{
		Add_RenderObject(RENDER_GROUP::BLEND);
	}
	else
	{
		Add_RenderObject(RENDER_GROUP::NONBLEND);
	}
}

HRESULT CPlayerTentacle::Render()
{
	if(m_bRender)
	{
		if (m_bTransparency)
		{
			if (FAILED(__super::Render(10)))
			{
				MSG_RETURN(E_FAIL, "CPlayerTentacle::Render", "Failed to CGameObject::Render");
			}
		}
		else
		{
			if (m_pPlayer->Get_PlayerDesc().bBrainField || m_bBrainField)
			{
				if (FAILED(m_pShader->Bind_RawValue("g_vGradientStartColor", &m_vRootColor, sizeof(_float3))))
				{
					MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Render", "Failed to Bind_RawValue: RootColor");
				}
				if (FAILED(m_pShader->Bind_RawValue("g_vGradientEndColor", &m_vTipColor, sizeof(_float3))))
				{
					MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Render", "Failed to Bind_RawValue: TipColor");
				}

				if (FAILED(m_pShader->Bind_Float("g_fGradientFactor", m_fZFactor)))
				{
					MSG_RETURN(E_FAIL, "CBrainFieldTentacle::Render", "Failed to Bind_Float: GradientFactor");
				}
				
				if (FAILED(__super::Render(27)))
				{
					MSG_RETURN(E_FAIL, "CPlayerTentacle::Render", "Failed to CGameObject::Render");
				}
			}
			else
			{
				if (FAILED(m_pShader->Bind_Float("g_fGradientFactor", m_fGradientFactor)))
				{
					MSG_RETURN(E_FAIL, "CPlayerTentacle::Render", "Failed to Bind Gradient Factor");
				}

				if (FAILED(__super::Render(21)))
				{
					MSG_RETURN(E_FAIL, "CPlayerTentacle::Render", "Failed to CGameObject::Render");
				}
			}
		}
	}

	return S_OK;
}

void CPlayerTentacle::Set_Transparency(_bool bTansparency)
{
	m_bTransparency = bTansparency;
}

HRESULT CPlayerTentacle::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

shared_ptr<CPlayerTentacle> CPlayerTentacle::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CPlayerTentacle> pInstance = make_private_shared(CPlayerTentacle, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CPlayerTentacle::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CPlayerTentacle::Clone(any pArg)
{
	shared_ptr<CPlayerTentacle> pInstance = make_private_shared_copy(CPlayerTentacle, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CPlayerTentacle::Clone", "Failed to Initialize");
	}

	return pInstance;
}

_bool CPlayerTentacle::Return()
{
	return m_bDead;
}
