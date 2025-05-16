#include "ClientPCH.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "ImGui_Manager.h"
#include "Transform.h"
#include "Event_Handler.h"
#include "Player.h"
#include "UI_Manager.h"
#include "Monster.h"
#include "ScreenEffectManager.h"

CCamera_Player::CCamera_Player(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCinematicCamera(_pDevice, _pContext)
{
}

HRESULT CCamera_Player::Initialize(any _aDesc)
{
	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);

	if (_aDesc.has_value())
	{
		m_tCameraMainDesc = any_cast<CAMERA_MAIN_DESC>(_aDesc);
	}
	else
	{
		m_tCameraMainDesc.tCameraDesc.eType = CCamera::TYPE::PERSPECTIVE;

		m_tCameraMainDesc.tCameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
		m_tCameraMainDesc.tCameraDesc.vAt = _float4(0.f, 0.f, 1.f, 0.f);

		m_tCameraMainDesc.tCameraDesc.fFovAngleY = MAIN_CAMERA_FOV;
		m_tCameraMainDesc.tCameraDesc.fAspect = static_cast<_float>(g_iWinCX) / static_cast<_float>(g_iWinCY);

		m_tCameraMainDesc.tCameraDesc.fNear = MAIN_CAMERA_NEAR;
		m_tCameraMainDesc.tCameraDesc.fFar = MAIN_CAMERA_FAR;

		m_tCameraMainDesc.tCameraDesc.fWidth = _float(g_iWinCX);
		m_tCameraMainDesc.tCameraDesc.fHeight = _float(g_iWinCY);
	}

	if (FAILED(__super::Initialize(m_tCameraMainDesc.tCameraDesc)))
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Initialize", "Failed to CCamera::Initialize");
	}

	CGameInstance::Get_Instance()->CheckFocus_OnMouse();
	CPipeLine::Get_Instance()->Set_Camera(static_pointer_cast<CCamera>(shared_from_this()));

	m_pTransform->Set_State(TRANSFORM::POSITION, _float3(0, 5, 10));

	return S_OK;
}

void CCamera_Player::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
	
#ifdef _DEBUG
	if (m_bDebugMode)
	{
		return;
	}
#endif // _DEBUG

	if (m_bLockMovement)
		return;

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	if (!m_bExecuted[0])
	{
		m_bExecuted[0] = true;
		m_pPlayerTransform = (static_pointer_cast<CTransform>(pGameInstance->Get_Player()->Get_Component(COMPONENT::TRANSFORM)));
	}

	if(m_bHandle)
		Move(_fTimeDelta);
	Shake(_fTimeDelta);

}

void CCamera_Player::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	if(nullptr!= m_pPlayerTransform && false == m_bLockMovement)
		Physics(_fTimeDelta);

	if(CGameInstance::Get_Instance()->Key_Down('K'))
	{
		if (nullptr != m_pLockedMonster.lock())
			m_pLockedMonster.lock()->Set_HP(-1.f);
	}

#if ACTIVATE_IMGUI
	if (CGameInstance::Get_Instance()->Key_Down(VK_HOME))
	{
		m_bDebugMode = !m_bDebugMode;
		if (m_bDebugMode)
		{
			CGameInstance::Get_Instance()->Fix_Cursor(false);
			CGameInstance::Get_Instance()->Show_Cursor(true);
		}
		else
		{
			CGameInstance::Get_Instance()->Fix_Cursor(true);
			CGameInstance::Get_Instance()->Show_Cursor(false);
		}
	}
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

		if (ImGui::Checkbox("Debug", &m_bDebugMode))
		{
			if (m_bDebugMode)
			{
				CGameInstance::Get_Instance()->Fix_Cursor(false);
				CGameInstance::Get_Instance()->Show_Cursor(true);
			}
			else
			{
				CGameInstance::Get_Instance()->Fix_Cursor(true);
				CGameInstance::Get_Instance()->Show_Cursor(false);
			}
		}

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

HRESULT CCamera_Player::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Render", "Failed to CCamera::Render");
	}

	return S_OK;
}

void CCamera_Player::Handle_MouseInput(_float _fTimeDelta)
{
	POINT ptCursorMove{};

	if (!CGameInstance::Get_Instance()->Is_CursorOn())
	{
		ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();
	}

	if (ptCursorMove.x)
	{
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), m_vSensitivty.x * ptCursorMove.x * _fTimeDelta);
	}
	if (ptCursorMove.y)
	{
		_float3	vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
		_float	fCurPitch = atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
		_float	fChgPitch = m_vSensitivty.y * ptCursorMove.y * _fTimeDelta;
		_float	fNewPitch = std::clamp(fCurPitch + fChgPitch, XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX));
		_float	fFinal = fNewPitch - fCurPitch;

		m_pTransform->Rotate(TRANSFORM::RIGHT, fFinal);
	}
}

#ifdef _DEBUG
void CCamera_Player::Handle_MouseInput_Debug(_float _fTimeDelta)
{
		POINT ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();

		if (ptCursorMove.x)
		{
			m_pTransform->Turn({ 0,1,0,0 }, m_vSensitivty.x * ptCursorMove.x * _fTimeDelta);
		}
		if (ptCursorMove.y)
		{
		/*	_float3	vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
			_float	fCurPitch = atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
			_float	fChgPitch = m_vSensitivty.y * ptCursorMove.y * _fTimeDelta;
			_float	fNewPitch = std::clamp(fCurPitch + fChgPitch, XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX));
			_float	fFinal = fNewPitch - fCurPitch;

			m_pTransform->Rotate(TRANSFORM::RIGHT, fFinal);*/
		}

		if (_short sVerticalScroll = CGameInstance::Get_Instance()->Get_CursorScroll().y)
		{
			//m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * sVerticalScroll * 0.01f);
		}
		m_pTransform->Set_State(TRANSFORM::POSITION, m_pPlayerTransform->Get_State(TRANSFORM::POSITION) - m_pPlayerTransform->Get_State(TRANSFORM::LOOK)*5.f + _float4(0, 2, 0, 1));
		// Debug Movement
	//	if (CGameInstance::Get_Instance()->Key_Hold('W'))
	//	{
	//		_float3 vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	//		m_pTransform->Translate(vLook * MAINCAM_DEBUG_MOVEMENT_SPEED * _fTimeDelta);
	//	}
	//	if (CGameInstance::Get_Instance()->Key_Hold('S'))
	//	{
	//		_float3 vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	//		m_pTransform->Translate(vLook * -MAINCAM_DEBUG_MOVEMENT_SPEED * _fTimeDelta);
	//	}
	//	if (CGameInstance::Get_Instance()->Key_Hold('A'))
	//	{
	//		_float3 vLook = m_pTransform->Get_State(TRANSFORM::RIGHT);
	//		m_pTransform->Translate(vLook * -MAINCAM_DEBUG_MOVEMENT_SPEED * _fTimeDelta);
	//	}
	//	if (CGameInstance::Get_Instance()->Key_Hold('D'))
	//	{
	//		_float3 vLook = m_pTransform->Get_State(TRANSFORM::RIGHT);
	//		m_pTransform->Translate(vLook * MAINCAM_DEBUG_MOVEMENT_SPEED * _fTimeDelta);
	//	}

	//if (CGameInstance::Get_Instance()->Key_Hold(VK_MBUTTON))
	//{
	//	POINT ptMove = CGameInstance::Get_Instance()->Get_CursorMove();
	//	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::RIGHT) * m_vSensitivty.x * static_cast<_float>(-ptMove.x));
	//	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::UP) * m_vSensitivty.y * static_cast<_float>(-ptMove.y));
	//}
}
#endif
void CCamera_Player::Move(_float _fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	if (false == m_bFixed)
	{

		_vector vPrePos = m_pTransform->Get_State(TRANSFORM::POSITION);
		_vector vCurPos = m_pPlayerTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 5.f + _float4(0, 2, 0, 1);

		if (false == m_bLockOn)
		{

			POINT ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();

			if (ptCursorMove.x)
			{
				m_pTransform->Rotate({ 0,1,0,0 }, m_vSensitivty.x * ptCursorMove.x * _fTimeDelta);
			}
			if (ptCursorMove.y)
			{
				m_pTransform->Rotate(m_pTransform->Get_State(TRANSFORM::RIGHT), m_vSensitivty.y * ptCursorMove.y * _fTimeDelta);
			}

			if (_short sVerticalScroll = CGameInstance::Get_Instance()->Get_CursorScroll().y)
			{
				//m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * sVerticalScroll * 0.01f);
			}


			/*if (!ptCursorMove.x && !ptCursorMove.y)
			{
				m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(vPrePos, vCurPos, 0.05f));
			}
			else
			{
				m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(vPrePos, vCurPos, 0.05f));
			}*/

			if (0 <= CUI_Manager::Get_Instance()->Get_MenuSwitch())
			{
				//pGameInstance->Show_Cursor(true);
			}
			else
			{
				pGameInstance->Show_Cursor(false);
			}
		}
		else if (m_pTargetTransform)
		{
			_float3 vMonPos = m_pTargetTransform->Get_State(TRANSFORM::POSITION);
			m_pTransform->LookTo(XMVectorLerp(m_pTransform->Get_State(TRANSFORM::LOOK), vMonPos - m_pTransform->Get_State(TRANSFORM::POSITION), 0.03f), true, false);
			m_pTransform->Rotate(TRANSFORM::RIGHT, m_fTheta);
		}
		else
		{
			if (nullptr != m_pMonsterTransform)
			{
				_float3 vMonPos = m_pMonsterTransform->Get_State(TRANSFORM::POSITION);
				m_pTransform->LookTo(XMVectorLerp(m_pTransform->Get_State(TRANSFORM::LOOK), vMonPos - m_pTransform->Get_State(TRANSFORM::POSITION), 0.03f), true, false);
				//m_pTransform->Rotate(TRANSFORM::RIGHT,XMConvertToRadians(-vMonPos.y*3.f));c
			}
			else
				m_bLockOn = false;
		}

		if(m_bLerpMove)
			m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(vPrePos, vCurPos, m_fLerpTime));

		_float3 vLook = m_pTransform->Get_HorizontalDir(TRANSFORM::LOOK);
		//cout << vLook.x << '\t' << vLook.y << '\t' << vLook.z << endl;
	}
}
void CCamera_Player::Physics(_float _fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	auto pScene = pGameInstance->Get_PhysXEngine()->GetScene();
	_vector Campos = m_pTransform->Get_State(TRANSFORM::POSITION);
	_vector CamLook = XMVector3Normalize(XMVectorSetY(m_pPlayerTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pPlayerTransform->Get_State(TRANSFORM::POSITION)) + 1.f) - Campos);
	PxQueryFilterData FilterData;
	FilterData.flags = PxQueryFlag::Enum::eSTATIC;
	
	PxRaycastBuffer Callback;
	_float Dist = Function::Distance(m_pTransform, m_pPlayerTransform);

	while (pScene->raycast(PxVec3(XMVectorGetX(Campos), XMVectorGetY(Campos), XMVectorGetZ(Campos)),
		PxVec3(XMVectorGetX(CamLook), XMVectorGetY(CamLook), XMVectorGetZ(CamLook)),
		Dist,
		Callback,
		PxHitFlag::Enum::eMESH_BOTH_SIDES,
		FilterData))
	{
		if (Callback.block.actor->userData == nullptr)
		{
			m_pTransform->Translate(CamLook * _fTimeDelta);
			Campos = m_pTransform->Get_State(TRANSFORM::POSITION);
			CamLook = XMVector3Normalize(XMVectorSetY(m_pPlayerTransform->Get_State(TRANSFORM::POSITION), XMVectorGetY(m_pPlayerTransform->Get_State(TRANSFORM::POSITION)) + 1.f) - Campos);
			Dist = Function::Distance(m_pTransform, m_pPlayerTransform);
			if (Dist >= 10.f)
				break;
			if (Dist <= 1.f)
				break;
		}
		else
			break;
	}
}

shared_ptr<CCamera_Player> CCamera_Player::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCamera_Player> pInstance = make_private_shared(CCamera_Player, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CCamera_Player::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCamera_Player::Clone(any)
{
	return shared_from_gameobject();
}

void CCamera_Player::Set_Shake(_float time, _float amount)
{
	m_bShake = true;  m_fShakeTime = time;  m_fCurrentShakeTime = time;  m_fShakeAmount = amount;
	m_vPreShakeCamPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);
}

void CCamera_Player::Set_Shake_NonDelta(_float time, _float amount)
{
	m_bShakeNonDelta = true;  m_fShakeTime = time;  m_fCurrentShakeTime = time;  m_fShakeAmount = amount;
	m_vPreShakeCamPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);
}

void CCamera_Player::Shake(_float fTimeDelta)
{
	if (m_bShake)
	{
		m_fCurrentShakeTime -= fTimeDelta;
		//_float fX, fY;
		if (m_fShakeTime - m_fCurrentShakeTime > 0.05f)
		{
			m_fShakeX = Function::RandomFloat(-m_fShakeAmount, m_fShakeAmount);
			m_fShakeY = Function::RandomFloat(-m_fShakeAmount, m_fShakeAmount);
			m_fShakeTime = m_fCurrentShakeTime;
		}
		m_pTransform->Translate(m_pPlayerTransform->Get_State(TRANSFORM::RIGHT)* m_fShakeX *fTimeDelta);
		m_pTransform->Translate(m_pPlayerTransform->Get_State(TRANSFORM::UP) * m_fShakeY * fTimeDelta);

		if (m_fCurrentShakeTime < 0.f)
		{
			m_bShake = false;
			m_fShakeTime = 0.3f;
			m_fCurrentShakeTime = 0.3f;
			m_fShakeAmount = 0.01f;

			CScreenEffectManager::Get_Instance()->Stop_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);
		}
	}

	if (m_bShakeNonDelta)
	{
		m_fCurrentShakeTime -= 0.016f;
		//_float fX, fY;
		if (m_fShakeTime - m_fCurrentShakeTime > 0.05f)
		{
			m_fShakeX = Function::RandomFloat(-m_fShakeAmount, m_fShakeAmount);
			m_fShakeY = Function::RandomFloat(-m_fShakeAmount, m_fShakeAmount);
			m_fShakeTime = m_fCurrentShakeTime;
		}
		m_pTransform->Translate(m_pPlayerTransform->Get_State(TRANSFORM::RIGHT) * m_fShakeX * 0.016f);
		m_pTransform->Translate(m_pPlayerTransform->Get_State(TRANSFORM::UP) * m_fShakeY * 0.016f);

		if (m_fCurrentShakeTime < 0.f)
		{
			m_bShakeNonDelta = false;
			m_fShakeTime = 0.3f;
			m_fCurrentShakeTime = 0.3f;
			m_fShakeAmount = 0.01f;

			CScreenEffectManager::Get_Instance()->Stop_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);
			//m_pTransform->Set_State(TRANSFORM::POSITION, m_vPreShakeCamPos);
		}
	}
}
