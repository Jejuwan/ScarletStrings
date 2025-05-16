#include "ClientPCH.h"
#include "GlobalGizmo.h"
#include "GameInstance.h"

#if ACTIVATE_TOOL

CGlobalGizmo::CGlobalGizmo(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CGlobalGizmo::CGlobalGizmo(const CGlobalGizmo& _rhs)
	: CGameObject(_rhs)
{
}

//	GameObject Prototype Function
HRESULT CGlobalGizmo::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_RECT);

	//	If Components require Clone Arguments, Replace g_aNull
	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT, g_aNull);

	m_tMaterialDesc.vDiffuse = _color(1.f, 1.f, 1.f, 0.5f);

	return S_OK;
}

HRESULT CGlobalGizmo::Initialize(any)
{
	//	Super Initialize: Initialize Components, Behaviors, EntityDesc, MaterialDesc from Prototype
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CGlobalGizmo::Initialize", "Failed to CGameObject::Initialize");
	}

	//	Get_Component: Template Function
	shared_ptr<CTransform> pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	pTransform->LookTo(_float3(0.f, 1.f, 0.f), false);
	Initialize_Gizmo(_int2(10, 10));

	if (FAILED(Add_Component(COMPONENT::TEXTURE, CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Gizmo.png")))))
	{
		MSG_RETURN(E_FAIL, "CGlobalGizmo::Initialize", "Failed to Add_Component");
	}

	return S_OK;
}

void CGlobalGizmo::Tick(_float _fTimeDelta)
{
	//	Must Call Super Function
	__super::Tick(_fTimeDelta);
}

void CGlobalGizmo::Late_Tick(_float _fTimeDelta)
{
	//	Must Call Super Function
	__super::Late_Tick(_fTimeDelta);

	if (m_bShow)
	{
		//	Render Function from Super Class
		Add_RenderObject(m_eGroup);
	}
}

HRESULT CGlobalGizmo::Render()
{
	//	Parameters: Shader Pass
	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CGlobalGizmo::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CGlobalGizmo::Initialize_Gizmo(_int2 _vGridSize)
{
	shared_ptr<CVIBufferInstance_Rect> pVIBuffer = Get_Component<CVIBufferInstance_Rect>(COMPONENT::VIBUFFER_INSTANCE_RECT);
	pVIBuffer->Update(
		[&](void* _pData, _uint iNumInstance)
		{
			VTXPOSTEXINSTTRANSARG2* pData = reinterpret_cast<VTXPOSTEXINSTTRANSARG2*>(_pData);
			_int2 vGridSize = _int2(std::clamp(_vGridSize.x, 1, 100), std::clamp(_vGridSize.y, 1, 100));
			size_t iInstance = 0;
			ZeroMemory(pData, sizeof(VTXPOSTEXINSTTRANSARG2) * iNumInstance);
			for (size_t x = 0; x < vGridSize.x; ++x)
			{
				for (size_t y = 0; y < vGridSize.y; ++y)
				{
					_float4x4 mTransformation = g_mUnit;
					mTransformation._41 = (1.f - vGridSize.x) * 0.5f + static_cast<_float>(x);
					mTransformation._42 = (vGridSize.y - 1.f) * 0.5f - static_cast<_float>(y);
					memcpy(&pData[iInstance++], &mTransformation, sizeof(_float4x4));
				}
			}
		}
	);
}

void CGlobalGizmo::Set_Color(_color _vColor)
{
	m_tMaterialDesc.vDiffuse = _vColor;
}

void CGlobalGizmo::Set_RenderGroup(RENDER_GROUP _eGroup)
{
	m_eGroup = _eGroup;
}

shared_ptr<CGlobalGizmo> CGlobalGizmo::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{	
    shared_ptr<CGlobalGizmo> pInstance = make_private_shared(CGlobalGizmo, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CGlobalGizmo::Create", "Failed to Initialze_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CGlobalGizmo::Clone(any)
{
	shared_ptr<CGlobalGizmo> pInstance = make_private_shared_copy(CGlobalGizmo, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CGlobalGizmo::Clone", "Failed to Initialze");
	}

	return pInstance;
}

#endif
