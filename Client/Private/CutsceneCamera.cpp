#include "ClientPCH.h"
#include "CutsceneCamera.h"
#include "GameInstance.h"

CCutsceneCamera::CCutsceneCamera(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCinematicCamera(_pDevice, _pContext)
{
}

HRESULT CCutsceneCamera::Initialize(any aDesc)
{
	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);

	m_tCameraMainDesc.eType = CCamera::TYPE::PERSPECTIVE;
	m_tCameraMainDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	m_tCameraMainDesc.vAt = _float4(0.f, 0.f, 1.f, 0.f);

	m_tCameraMainDesc.fFovAngleY = CUTSCENE_CAMERA_FOV;
	m_tCameraMainDesc.fAspect = static_cast<_float>(g_iWinCX) / static_cast<_float>(g_iWinCY);
	m_tCameraMainDesc.fNear = CUTSCENE_CAMERA_NEAR;

	m_tCameraMainDesc.fFar = CUTSCENE_CAMERA_FAR;

	m_tCameraMainDesc.fWidth = g_iWinCX;
	m_tCameraMainDesc.fHeight = g_iWinCY;

	if (FAILED(__super::Initialize(m_tCameraMainDesc)))
	{
		MSG_RETURN(E_FAIL, "CCutsceneCamera::Initialize", "Failed to CCamera::Initialize");
	}

	return S_OK;
}

void CCutsceneCamera::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CCutsceneCamera::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CCutsceneCamera::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneCamera::Render", "Failed to CCamera::Render");
	}

	return S_OK;
}

shared_ptr<CCutsceneCamera> CCutsceneCamera::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCutsceneCamera> pInstance = make_private_shared(CCutsceneCamera, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CCutsceneCamera::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCutsceneCamera::Clone(any)
{
	return shared_from_gameobject();
}
