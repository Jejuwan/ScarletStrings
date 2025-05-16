#include "EnginePCH.h"
#include "CinematicCamera.h"

#include "PipeLine.h"
#include "Transform.h"

#include "Keyframe.h"

CCinematicCamera::CCinematicCamera(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCamera(_pDevice, _pContext)
{
}

HRESULT CCinematicCamera::Initialize(any _arg)
{
	m_bitComponent |= BIT(COMPONENT::KEYFRAME);
	m_umapComponentArg[COMPONENT::KEYFRAME] = make_pair(PROTOTYPE_COMPONENT_KEYFRAME, g_aNull);

	if (FAILED(__super::Initialize(_arg)))
	{
		MSG_RETURN(E_FAIL, "CCinematicCamera::Initialize", "Failed to CCamera::Initialize");
	}

	return S_OK;
}

void CCinematicCamera::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pKeyframe->Tick(m_pTransform, fTimeDelta);
}

void CCinematicCamera::Append_Cinematic(const wstring& wstrFilePath, const wstring& wstrCinCameraTag)
{
	m_pKeyframe->Append_Keyframe(wstrFilePath, wstrCinCameraTag);
}

void CCinematicCamera::Clear_Cinematic()
{
	m_pKeyframe->Clear_Keyframe();
}

void CCinematicCamera::Play_CinematicCamera(const wstring& wstrCinematicTag, const _float4x4& matPivot)
{
	m_pKeyframe->Play_Animation(wstrCinematicTag, matPivot);
}

void CCinematicCamera::Pause_CinematicCamera()
{
	m_pKeyframe->Pause_Animation();
}

void CCinematicCamera::Continue_CinematicCamera()
{
	m_pKeyframe->Continue_Animation();
}

HRESULT CCinematicCamera::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCinematicCamera::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pKeyframe = Get_Component<CKeyframe>(COMPONENT::KEYFRAME);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCinematicCamera::Ready_Components", "Failed to Get_Component: KEYFRAME");
	}

	return S_OK;
}

void CCinematicCamera::Subscribe_Cinematic_Finish_Callback(const std::function<void(const wstring&)>& fpCallback)
{
	m_pKeyframe->Subscribe_Animation_Finish_Callback(fpCallback);
}