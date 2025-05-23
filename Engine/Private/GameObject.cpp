#include "EnginePCH.h"
#include "GameObject.h"
#include "System.h"
#include "Component_Manager.h"
#include "GameInstance.h"
#include "Scene_Manager.h"
#include "Object_Manager.h"
#include "PipeLine.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Model.h"
#include "Keyframe.h"
#include "Light.h"
#include "Camera.h"
#include "Light_Manager.h"

CGameObject::CGameObject(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: m_pDevice	(_pDevice)
	, m_pContext(_pContext)
{
	m_bitComponent.reset();
	m_bitBehavior.reset();

	m_umapComponentArg.reserve(IDX(COMPONENT::MAX));
	m_umapBehaviorArg.reserve(IDX(BEHAVIOR::MAX));

	for (_uint i = 0; i < IDX(COMPONENT::MAX); ++i)
	{
		m_umapComponentArg[static_cast<COMPONENT>(i)] = make_pair(wstring(), g_aNull);
	}
	for (_uint i = 0; i < IDX(BEHAVIOR::MAX); ++i)
	{
		m_umapBehaviorArg[static_cast<BEHAVIOR>(i)] = make_pair(wstring(), g_aNull);
	}
}

CGameObject::CGameObject(const CGameObject& _rhs)
	: m_pDevice			(_rhs.m_pDevice)
	, m_pContext		(_rhs.m_pContext)
	, m_tEntityDesc		(_rhs.m_tEntityDesc)
	, m_tMaterialDesc	(_rhs.m_tMaterialDesc)
	, m_bitComponent	(_rhs.m_bitComponent)
	, m_bitBehavior		(_rhs.m_bitBehavior)
	, m_umapComponentArg(_rhs.m_umapComponentArg)
	, m_umapBehaviorArg	(_rhs.m_umapBehaviorArg)
{
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(any _arg)
{
	if (FAILED(Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Initialize", "Failed to Ready_Components");
	}

	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Initialize", "Failed to Ready_Behaviors");
	}

	return S_OK;
}

void CGameObject::Tick(_float _fTimeDelta)
{
	for (auto& iter : m_umapBehavior)
	{
		iter.second->Tick(_fTimeDelta);
	}
}

void CGameObject::Late_Tick(_float _fTimeDelta)
{
	for (auto& iter : m_umapBehavior)
	{
		iter.second->Late_Tick(_fTimeDelta);
	}
}

HRESULT CGameObject::Render_Screen_Effect()
{
	return S_OK;
}

HRESULT CGameObject::Fetch(any)
{
	m_bIsCheckOut = true;

	return S_OK;
}

_bool CGameObject::Return()
{
	return false;
}

HRESULT CGameObject::Release()
{
	m_bIsCheckOut = false;

	return S_OK;
}

HRESULT CGameObject::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (shared_ptr<CShader> pShader = m_pWeakShader.lock())
	{
		if (shared_ptr<CTransform> pTransform = m_pWeakTransform.lock())
		{
			if (FAILED(pTransform->Bind_OnShader(pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render_ShadowDepth", "Failed to CTransform::Bind_OnShader");
			}

			if (FAILED(_pShadow->Bind_ShadowMatrix(pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render_ShadowDepth", "Failed to CLight::Bind_ShadowMatrix");
			}
		}
	}

	return S_OK;
}

HRESULT CGameObject::Render_OutLine()
{
	return S_OK;
}

HRESULT CGameObject::Render_Bloom()
{
	return S_OK;
}

HRESULT CGameObject::Render_Neon()
{
	return S_OK;
}

void CGameObject::OnCollisionEnter(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta)
{
}
void CGameObject::OnCollision(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta)
{
}
void CGameObject::OnCollisionExit(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta)
{
}

HRESULT CGameObject::Render(_uint _iPassIndex, _bool bOrthographic)
{
 	if (shared_ptr<CShader> pShader = m_pWeakShader.lock())
	{
		Bind_Matarial(pShader);
		if (bOrthographic)
			Bind_Transform_Orthographic(pShader);
		else
			Bind_Transform(pShader);
		Bind_Texture(pShader);
		Bind_VIBuffer(pShader, _iPassIndex);
		Bind_Model(pShader, _iPassIndex);
	}

	return S_OK;
}

shared_ptr<CComponent> CGameObject::Get_Component(const COMPONENT _eComponent)
{
	if (m_umapComponent.end() != m_umapComponent.find(_eComponent))
	{
		return m_umapComponent[_eComponent];
	}

	return nullptr;
}

shared_ptr<CBehavior> CGameObject::Get_Behavior(const BEHAVIOR _eBehavior)
{
	if (m_umapBehavior.end() != m_umapBehavior.find(_eBehavior))
	{
		return m_umapBehavior[_eBehavior];
	}

	return nullptr;
}

_bool CGameObject::Has_Component(const COMPONENT _eComponent)
{
	if (m_umapComponent.end() != m_umapComponent.find(_eComponent))
	{
		return true;
	}

	return false;
}


HRESULT CGameObject::Ready_Components()
{
	HRESULT hr = S_OK;

	for (size_t i = 0; i < IDX(COMPONENT::MAX); ++i)
	{
		if (m_bitComponent.test(i))
		{
			if (FAILED(Add_Component(static_cast<COMPONENT>(i))))
			{
				hr = E_FAIL;
			}
		}
	}

	if (FAILED(hr))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Ready_Components", "Failed to Add_Component");
	}

	return S_OK;
}

HRESULT CGameObject::Ready_Behaviors()
{
	HRESULT hr = S_OK;

	for (size_t i = 0; i < IDX(BEHAVIOR::MAX); ++i)
	{
		if (m_bitBehavior.test(i))
		{
			if (FAILED(Add_Behavior(static_cast<BEHAVIOR>(i))))
			{
				hr = E_FAIL;
			}
		}
	}

	if (FAILED(hr))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Ready_Behaviors", "Failed to Add_Behavior");
	}

	return S_OK;
}

HRESULT CGameObject::Add_Component(const COMPONENT _eComponent, shared_ptr<CComponent> _pComponent)
{
#pragma region Add
	switch (_eComponent)
	{
	case COMPONENT::TRANSFORM:
		m_umapComponent.emplace(_eComponent, _pComponent ? _pComponent : CTransform::Create(m_pDevice, m_pContext));
		break;

	case COMPONENT::RENDERER:
	case COMPONENT::SHADER:
	case COMPONENT::VIBUFFER_RECT:
	case COMPONENT::VIBUFFER_CUBE:
	case COMPONENT::VIBUFFER_SPHERE:
	case COMPONENT::VIBUFFER_INSTANCE_MESH:
	case COMPONENT::VIBUFFER_INSTANCE_MODEL:
	case COMPONENT::VIBUFFER_INSTANCE_POINT:
	case COMPONENT::VIBUFFER_INSTANCE_LINE:
	case COMPONENT::VIBUFFER_INSTANCE_RECT:
	case COMPONENT::KEYFRAME:
	case COMPONENT::MODEL_RENDERER:
		m_umapComponent.emplace(_eComponent, _pComponent ? _pComponent :
			CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Static_Scene(),
			m_umapComponentArg[_eComponent].first, m_umapComponentArg[_eComponent].second));
		break;

	case COMPONENT::TEXTURE:
	case COMPONENT::MODEL:
	case COMPONENT::MESH:
		m_umapComponent.emplace(_eComponent, _pComponent ? _pComponent :
			CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Current_Scene(),
			m_umapComponentArg[_eComponent].first, m_umapComponentArg[_eComponent].second));
		break;

	case COMPONENT::COLLIDER:
		m_umapComponent.emplace(_eComponent, _pComponent ? _pComponent :
			CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Static_Scene(),
				m_umapComponentArg[_eComponent].first, make_pair(shared_from_gameobject(), any_cast<COLLIDERDESC>(m_umapComponentArg[_eComponent].second))));
		break;

	case COMPONENT::VIBUFFER:
		break;

	default:
		MSG_RETURN(E_FAIL, "CGameObject::Add_Component", "Invalid Range");
	}
#pragma endregion
#pragma region VIBuffer
	switch (_eComponent)
	{
	case COMPONENT::MESH:
	case COMPONENT::VIBUFFER_RECT:
	case COMPONENT::VIBUFFER_CUBE:
	case COMPONENT::VIBUFFER_SPHERE:
	case COMPONENT::VIBUFFER_INSTANCE_MESH:
	case COMPONENT::VIBUFFER_INSTANCE_MODEL:
	case COMPONENT::VIBUFFER_INSTANCE_POINT:
	case COMPONENT::VIBUFFER_INSTANCE_LINE:
	case COMPONENT::VIBUFFER_INSTANCE_RECT:
		m_umapComponent.emplace(COMPONENT::VIBUFFER, m_umapComponent[_eComponent]);
		m_bitComponent.set(IDX(COMPONENT::VIBUFFER), true);
		break;
	}
#pragma endregion
#pragma region Weak Pointer
	switch (_eComponent)
	{
	case COMPONENT::RENDERER:
		m_pWeakRenderer		= Get_Component<CRenderer>(_eComponent);
		break;
	case COMPONENT::TRANSFORM:
		m_pWeakTransform	= Get_Component<CTransform>(_eComponent);
		break;
	case COMPONENT::SHADER:
		m_pWeakShader		= Get_Component<CShader>(_eComponent);
		break;
	case COMPONENT::TEXTURE:
		m_pWeakTexture		= Get_Component<CTexture>(_eComponent);
		break;
	case COMPONENT::COLLIDER:
		m_pWeakCollider		= Get_Component<CCollider>(_eComponent);
		break;
	case COMPONENT::KEYFRAME:
		m_pWeakKeyframe		= Get_Component<CKeyframe>(_eComponent);
		break;
	case COMPONENT::MESH:
	case COMPONENT::VIBUFFER_RECT:
	case COMPONENT::VIBUFFER_CUBE:
	case COMPONENT::VIBUFFER_SPHERE:
	case COMPONENT::VIBUFFER_INSTANCE_MESH:
	case COMPONENT::VIBUFFER_INSTANCE_MODEL:
	case COMPONENT::VIBUFFER_INSTANCE_POINT:
	case COMPONENT::VIBUFFER_INSTANCE_LINE:
	case COMPONENT::VIBUFFER_INSTANCE_RECT:
		m_pWeakVIBuffer		= Get_Component<CVIBuffer>(_eComponent);
		break;
	case COMPONENT::MODEL:
		m_pWeakModel		= Get_Component<CModel>(_eComponent);
		break;
	}
#pragma endregion

	m_bitComponent.set(IDX(_eComponent), true);

	return S_OK;
}

HRESULT CGameObject::Add_Behavior(const BEHAVIOR _eBehavior, shared_ptr<CBehavior> _pBehavior)
{
	switch (_eBehavior)
	{
	case BEHAVIOR::PHYSICS:
		m_umapBehavior.emplace(_eBehavior, _pBehavior ? _pBehavior : CPhysics::Create(shared_from_gameobject(), &m_tEntityDesc));
		break;

	case BEHAVIOR::ANIMATOR:
		m_umapBehavior.emplace(_eBehavior, _pBehavior ? _pBehavior : CAnimator::Create(shared_from_gameobject()));
		break;

	case BEHAVIOR::CONTROL:
	case BEHAVIOR::CUSTOM:
		if (!_pBehavior)
		{
			MSG_RETURN(E_FAIL, "CGameObject::Add_Behavior", "Invalid CBehavior");
		}
		m_umapBehavior.emplace(_eBehavior, _pBehavior);
		break;

	default:
		MSG_RETURN(E_FAIL, "CGameObject::Add_Behavior", "Invalid Range");
	}

	m_bitBehavior.set(IDX(_eBehavior), true);

 	return S_OK;
}

HRESULT CGameObject::Delete_Component(const COMPONENT _eComponent)
{
	auto iter = m_umapComponent.find(_eComponent);
	if (iter == m_umapComponent.end())
	{
		return S_FALSE;
	}

	switch (_eComponent)
	{
	case COMPONENT::MESH:
	case COMPONENT::VIBUFFER_RECT:
	case COMPONENT::VIBUFFER_CUBE:
	case COMPONENT::VIBUFFER_SPHERE:
	case COMPONENT::VIBUFFER_INSTANCE_MESH:
	case COMPONENT::VIBUFFER_INSTANCE_MODEL:
	case COMPONENT::VIBUFFER_INSTANCE_POINT:
	case COMPONENT::VIBUFFER_INSTANCE_LINE:
	case COMPONENT::VIBUFFER_INSTANCE_RECT:
		m_umapComponent.erase(m_umapComponent.find(COMPONENT::VIBUFFER));
		m_bitComponent.set(IDX(COMPONENT::VIBUFFER), false);

	default:
		m_umapComponent.erase(iter);
		m_bitComponent.set(IDX(_eComponent), false);
		break;
	}

	return S_OK;
}

HRESULT CGameObject::Delete_Behavior(const BEHAVIOR _eBehavior)
{
	auto iter = m_umapBehavior.find(_eBehavior);
	if (iter == m_umapBehavior.end())
	{
		return S_FALSE;
	}

	m_umapBehavior.erase(iter);
	m_bitBehavior.set(IDX(_eBehavior), false);

	return S_OK;
}

HRESULT CGameObject::Add_RenderObject(const RENDER_GROUP _eRenderGroup)
{
	if (shared_ptr<CRenderer> pRenderer = m_pWeakRenderer.lock())
	{
		if (SUCCEEDED(pRenderer->Add_RenderObject(_eRenderGroup, shared_from_this())))
		{
#ifdef _DEBUG
			if (RENDER_GROUP::SHADOW != _eRenderGroup)
			{
				if (m_bitComponent.test(IDX(COMPONENT::COLLIDER)))
				{
					if (FAILED(pRenderer->Add_RenderObject(RENDER_GROUP::DEBUG, m_umapComponent[COMPONENT::COLLIDER])))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Add_RenderObject", "Failed to CRenderer::Add_RenderObject");
					}
				}
			}
#endif
			return S_OK;
		}
		else
		{
			MSG_RETURN(E_FAIL, "CGameObject::Add_RenderObject", "Failed to CRenderer::Add_RenderObject");
		}
	}

	return S_FALSE;
}

HRESULT CGameObject::Add_RenderObject(const UI_LAYER _eUILayer)
{
	if (shared_ptr<CRenderer> pRenderer = m_pWeakRenderer.lock())
	{
		if (SUCCEEDED(pRenderer->Add_RenderObject(_eUILayer, shared_from_this())))
		{
			return S_OK;
		}
	}

	MSG_RETURN(E_FAIL, "CGameObject::Add_RenderObject(UI)", "Failed to CRenderer::Add_RenderObject(UI)");
}

shared_ptr<CGameObject> CGameObject::shared_from_gameobject()
{
	return static_pointer_cast<CGameObject>(ISystem::shared_from_this());
}

HRESULT CGameObject::Bind_Matarial(shared_ptr<CShader> pShader)
{
	if (FAILED(pShader->Bind_Vector(SHADER_MTRLDIF, m_tMaterialDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(pShader->Bind_Vector(SHADER_MTRLAMB, m_tMaterialDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(pShader->Bind_Vector(SHADER_MTRLSPC, m_tMaterialDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(pShader->Bind_Vector(SHADER_MTRLEMS, m_tMaterialDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(pShader->Bind_Float(SHADER_MTRLSHN, m_tMaterialDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	_float fSpecularExist = m_tMaterialDesc.vSpecular.x + m_tMaterialDesc.vSpecular.y + m_tMaterialDesc.vSpecular.z;
	if (fSpecularExist > 0.f)
	{
		if (FAILED(CLight_Manager::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
		}
	}

	return S_OK;
}

HRESULT CGameObject::Bind_Transform(shared_ptr<CShader> pShader)
{
	if (shared_ptr<CTransform> pTransform = m_pWeakTransform.lock())
	{
		if (FAILED(pTransform->Bind_OnShader(pShader)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CTransform::Bind_OnShader");
		}
	}

	return S_OK;
}

HRESULT CGameObject::Bind_Transform_Orthographic(shared_ptr<CShader> pShader)
{
	if (shared_ptr<CTransform> pTransform = m_pWeakTransform.lock())
	{
		if (FAILED(pShader->Bind_Matrix(SHADER_MATWORLD, pTransform->Get_Matrix())))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CTransform::Bind_OnShader");
		}

		//auto pPipeLine = CPipeLine::Get_Instance();
		//_vector vCamPosition = pPipeLine->Get_Camera()->Get_State(TRANSFORM::POSITION);
		//XMMatrixLookToLH(
		//	vCamPosition,
		//	_float4(pTransform->Get_State(TRANSFORM::POSITION) - vCamPosition, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f))

		if (FAILED(pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CTransform::Bind_OnShader");
		}
		if (FAILED(pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_OrthoProjection())))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CTransform::Bind_OnShader");
		}
	}

	return S_OK;
}

HRESULT CGameObject::Bind_Texture(shared_ptr<CShader> pShader)
{
	if (shared_ptr<CTexture> pTexture = m_pWeakTexture.lock())
	{
		if (FAILED(pTexture->Bind_ShaderResourceViews(pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CTexture::Bind_ShaderResourceView");
		}
	}

	return S_OK;
}

HRESULT CGameObject::Bind_VIBuffer(shared_ptr<CShader> pShader, _uint _iPassIndex)
{
	if (shared_ptr<CVIBuffer> pVIBuffer = m_pWeakVIBuffer.lock())
	{
		if (FAILED(pVIBuffer->Render(pShader, _iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CVIBuffer::Render");
		}
	}

	return S_OK;
}

HRESULT CGameObject::Bind_Model(shared_ptr<CShader> pShader, _uint _iPassIndex)
{
	if (shared_ptr<CModel> pModel = m_pWeakModel.lock())
	{
		if (FAILED(pModel->Render(pShader, _iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Model::Render");
		}
	}

	return S_OK;
}
