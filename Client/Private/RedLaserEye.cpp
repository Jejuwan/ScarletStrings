#include "ClientPCH.h"
#include "RedLaserEye.h"

#include "GameInstance.h"
#include "Bone.h"

CRedLaserEye::CRedLaserEye(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CRedLaserEye::CRedLaserEye(const CRedLaserEye& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CRedLaserEye::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	return S_OK;
}

HRESULT CRedLaserEye::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRedLayerEye::Initialize", "Failed to CGameObject::Initialize");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayer = pGameInstance->Get_Player();
	auto pPlayerModel = pPlayer->Get_Component<CModel>(COMPONENT::MODEL);
	m_pPlayerTransform = pPlayer->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	m_pLeftEyeCombineBone = pPlayerModel->Get_Bone("LeftEye")->Get_CombinedTransformationPointer();

	return S_OK;
}

void CRedLaserEye::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix matLeftEye = XMLoadFloat4x4(m_pLeftEyeCombineBone) * XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * m_pPlayerTransform->Get_Matrix();
	m_pTransform->Set_Matrix(matLeftEye);

	if (m_eLaserType == LASER_TYPE::SMALL)
	{
		m_tMaterialDesc.vDiffuse.w = Function::Random({ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f });
		m_pTransform->Set_Scale(XMVectorSet(2.f, Function::RandomFloat(0.01f, 1.f), 1.f, 0.f));
	}
	else if (m_eLaserType == LASER_TYPE::BIG)
	{
		if (m_queLaserKeyframe.empty() == false)
		{
			const LASERKEYFRAME& tLaserKeyframe = m_queLaserKeyframe.front();

			m_fAccTime += fTimeDelta;
			if (m_fAccTime >= tLaserKeyframe.fDuration)
			{
				m_fAccTime = 0.f;
				m_queLaserKeyframe.pop();

				m_vPrevColor = tLaserKeyframe.vLaserColor;
				m_vPrevScale = tLaserKeyframe.vLaserScale;
			}
			
			_vector vInterpolatedColor = XMVectorLerp(m_vPrevColor, tLaserKeyframe.vLaserColor, m_fAccTime / tLaserKeyframe.fDuration);
			XMStoreFloat4(&m_tMaterialDesc.vDiffuse, vInterpolatedColor);
			_vector vInterpolatedScale = XMVectorLerp(m_vPrevScale, tLaserKeyframe.vLaserScale, m_fAccTime / tLaserKeyframe.fDuration);
			m_pTransform->Set_Scale(vInterpolatedScale);
		}
		else
		{
			m_tMaterialDesc.vDiffuse = _float4(1.5f, 0.f, 0.f, 1.f);
			m_pTransform->Set_Scale({ 4.f, 0.05f, 1.f });
		}

#ifdef _DEBUG
		ImGui::Begin("Laser Settings");
		_float3 vScale = m_pTransform->Get_Scale();
		if (ImGui::SliderFloat3("Laser Scale", &vScale.x, 0.f, 10.f))
		{
			m_pTransform->Set_Scale(vScale);
		}

		ImGui::SliderFloat4("Laser Diffuse Color", &m_tMaterialDesc.vDiffuse.x, 0.f, 5.f);

		ImGui::End();
#endif
	}
}

void CRedLaserEye::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_tMaterialDesc.vDiffuse.w > 0.f)
		Add_RenderObject(RENDER_GROUP::NEON);
}

HRESULT CRedLaserEye::Render()
{
	return S_OK;
}

HRESULT CRedLaserEye::Render_Neon()
{
	if (m_eLaserType == LASER_TYPE::SMALL)
	{
		if (FAILED(m_pSmallLaser->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to CRedLaserEye::Bind_ShaderResourceView");
		}
		if (FAILED(__super::Render(6)))
		{
			MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to CGameObject::Render");
		}
	}
	else if (m_eLaserType == LASER_TYPE::BIG)
	{
		if (FAILED(m_pBigLaser->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to CRedLaserEye::Bind_ShaderResourceView");
		}
		if (FAILED(__super::Render(7)))
		{
			MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to CGameObject::Render");
		}
	}

	return S_OK;
}

void CRedLaserEye::Set_Enable(_bool bEnable)
{
	__super::Set_Enable(bEnable);
}

void CRedLaserEye::Set_Laser_Type(LASER_TYPE eLaserType)
{
	m_eLaserType = eLaserType;
	if (m_eLaserType == LASER_TYPE::SMALL)
	{
		m_tMaterialDesc.vDiffuse = float4(1.f, 0.f, 0.f, 1.f);
	}
	else if (m_eLaserType == LASER_TYPE::BIG)
	{
		m_tMaterialDesc.vDiffuse = float4(1.5f, 0.5f, 0.5f, 1.f);
	}

	m_tMaterialDesc.vDiffuse.w = 1.f;
	m_fAccTime = 0.f;
}

void CRedLaserEye::Push_Keyframe(const LASERKEYFRAME& tLaserKeyframe)
{
	m_queLaserKeyframe.push(tLaserKeyframe);
}

HRESULT CRedLaserEye::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to Get_Component: VIBUFFER_RECT");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pSmallLaser = pGameInstance->Clone_Component<CTexture>(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_DECAL_EV_ONE_CIR_001);
	if (nullptr == m_pSmallLaser)
	{
		MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to Clone_Component: SmallLaser");
	}
	
	m_pBigLaser = pGameInstance->Clone_Component<CTexture>(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_MASK_LIN_010);
	if (nullptr == m_pBigLaser)
	{
		MSG_RETURN(E_FAIL, "CRedLaserEye::Ready_Components", "Failed to Clone_Component: SmallLaser");
	}

	return S_OK;
}

shared_ptr<CRedLaserEye> CRedLaserEye::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRedLaserEye> pInstance = make_private_shared(CRedLaserEye, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CRedLayerEye::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CRedLaserEye::Clone(any)
{
	shared_ptr<CRedLaserEye> pInstance = make_private_shared_copy(CRedLaserEye, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CRedLayerEye::Clone", "Failed to Initialize");
	}

	return pInstance;
}
