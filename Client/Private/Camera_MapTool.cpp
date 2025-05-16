#include "ClientPCH.h"
#include "Camera_MapTool.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Map_ToolManager.h"

CCamera_MapTool::CCamera_MapTool(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCinematicCamera(_pDevice, _pContext)
{
}

HRESULT CCamera_MapTool::Initialize(any _aDesc)
{
	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);

	if (_aDesc.has_value())
	{
		m_tCameraMainDesc = any_cast<CAMERA_MAIN_DESC>(_aDesc);
	}
	else
	{
		m_tCameraMainDesc.tCameraDesc.eType			= CCamera::TYPE::PERSPECTIVE;

		m_tCameraMainDesc.tCameraDesc.vEye			= _float4(0.f, 0.f, 0.f, 1.f);
		m_tCameraMainDesc.tCameraDesc.vAt			= _float4(0.f, 0.f, 1.f, 0.f);

		m_tCameraMainDesc.tCameraDesc.fFovAngleY	= MAIN_CAMERA_FOV;
		m_tCameraMainDesc.tCameraDesc.fAspect		= static_cast<_float>(g_iWinCX) / static_cast<_float>(g_iWinCY);

		m_tCameraMainDesc.tCameraDesc.fNear			= MAIN_CAMERA_NEAR;
		m_tCameraMainDesc.tCameraDesc.fFar			= MAIN_CAMERA_FAR;
	}

	if (FAILED(__super::Initialize(m_tCameraMainDesc.tCameraDesc)))
	{
		MSG_RETURN(E_FAIL, "CCamera_MapTool::Initialize", "Failed to CCamera::Initialize");
	}

	CGameInstance::Get_Instance()->Fix_Cursor();
	CPipeLine::Get_Instance()->Set_Camera(static_pointer_cast<CCamera>(shared_from_this()));

	return S_OK;
}

void CCamera_MapTool::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

#ifdef _DEBUG
	if (false == m_bDebugMode)
	{
#endif
		Handle_MouseInput(_fTimeDelta);
#ifdef _DEBUG
	}
	else
	{
		Handle_MouseInput_Debug(_fTimeDelta);
	}
#endif

	CMap_ToolManager::Get_Instance()->Set_CameraPos(m_pTransform->Get_State(TRANSFORM::POSITION));

	if (CGameInstance::Get_Instance()->Key_Down('G'))
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, CMap_ToolManager::Get_Instance()->Get_PickingPos());
	}
}

void CCamera_MapTool::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

#if ACTIVATE_IMGUI
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		if (ImGui::BeginMainMenuBar())
		{
		if (ImGui::BeginMenu("MainCam"))
		{
		ImGui::Text("Position: ");
		ImGui::Text("X: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).x);
		ImGui::SameLine();
		ImGui::Text("Y: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).y);
		ImGui::SameLine();
		ImGui::Text("Z: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).z);

		ImGui::Checkbox("Debug", &m_bDebugMode);
		if (ImGui::Button("Reset Position"))
		{
			m_pTransform->Set_Matrix(g_mUnit);
		}

		ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
		}
	}
#endif
}

HRESULT CCamera_MapTool::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CCamera_MapTool::Render", "Failed to CCamera::Render");
	}

	return S_OK;
}

void CCamera_MapTool::Handle_MouseInput(_float _fTimeDelta)
{
	POINT ptCursorMove{};

	if (!CGameInstance::Get_Instance()->Is_CursorOn())
	{
		ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();
	}

	if (ptCursorMove.x)
	{
		m_pTransform->Rotate(_float3(0.f, 1.f, 0.f), m_vSensitivty.x * ptCursorMove.x * _fTimeDelta);
	}
	if (ptCursorMove.y)
	{
		_float3	vLook		= m_pTransform->Get_State(TRANSFORM::LOOK);
		_float	fCurPitch	= atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
		_float	fChgPitch	= m_vSensitivty.y * ptCursorMove.y * _fTimeDelta;
		_float	fNewPitch	= std::clamp(fCurPitch + fChgPitch, XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX));
		_float	fFinal		= fNewPitch - fCurPitch;

		m_pTransform->Rotate(TRANSFORM::RIGHT, fFinal);
	}
}

#ifdef _DEBUG
void CCamera_MapTool::Handle_MouseInput_Debug(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Hold(VK_RBUTTON))
	{
		POINT ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();

		if (ptCursorMove.x)
		{
			m_pTransform->Rotate(_float3(0.f, 1.f, 0.f), m_vSensitivty.x * ptCursorMove.x * _fTimeDelta);
		}
		if (ptCursorMove.y)
		{
			_float3	vLook		= m_pTransform->Get_State(TRANSFORM::LOOK);
			_float	fCurPitch	= atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
			_float	fChgPitch	= m_vSensitivty.y * ptCursorMove.y * _fTimeDelta;
			_float	fNewPitch	= std::clamp(fCurPitch + fChgPitch, XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX));
			_float	fFinal		= fNewPitch - fCurPitch;

			m_pTransform->Rotate(TRANSFORM::RIGHT, fFinal);
		}

		if (_short sVerticalScroll = CGameInstance::Get_Instance()->Get_CursorScroll().y)
		{
			m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * sVerticalScroll * 0.01f);
		}

		// Debug Movement
		if (CGameInstance::Get_Instance()->Key_Hold('W'))
		{
			_float3 vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
			m_pTransform->Translate(vLook * MAINCAM_DEBUG_MOVEMENT_SPEED * _fTimeDelta);
		}
		if (CGameInstance::Get_Instance()->Key_Hold('S'))
		{
			_float3 vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
			m_pTransform->Translate(vLook * -MAINCAM_DEBUG_MOVEMENT_SPEED * _fTimeDelta);
		}
		if (CGameInstance::Get_Instance()->Key_Hold('A'))
		{
			_float3 vLook = m_pTransform->Get_State(TRANSFORM::RIGHT);
			m_pTransform->Translate(vLook * -MAINCAM_DEBUG_MOVEMENT_SPEED * _fTimeDelta);
		}
		if (CGameInstance::Get_Instance()->Key_Hold('D'))
		{
			_float3 vLook = m_pTransform->Get_State(TRANSFORM::RIGHT);
			m_pTransform->Translate(vLook * MAINCAM_DEBUG_MOVEMENT_SPEED * _fTimeDelta);
		}

	}
	if (CGameInstance::Get_Instance()->Key_Hold(VK_MBUTTON))
	{
		POINT ptMove = CGameInstance::Get_Instance()->Get_CursorMove();
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::RIGHT)	* m_vSensitivty.x * static_cast<_float>(-ptMove.x));
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::UP)		* m_vSensitivty.y * static_cast<_float>(-ptMove.y));
	}
}
#endif

shared_ptr<CCamera_MapTool> CCamera_MapTool::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCamera_MapTool> pInstance = make_private_shared(CCamera_MapTool, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CCamera_MapTool::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCamera_MapTool::Clone(any)
{
	return shared_from_gameobject();
}
