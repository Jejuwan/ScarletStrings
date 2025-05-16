#pragma once
#include "Client_Define.h"
#include "CinematicCamera.h"

#define MAIN_CAMERA_FOV				XMConvertToRadians(60.f)
#define MAIN_CAMERA_NEAR			0.01f
#define MAIN_CAMERA_FAR				10000.f
#define MAINCAM_PITCH_MIN			-89.f
#define MAINCAM_PITCH_MAX			+89.f

#define MAINCAM_SENSITIVITY_YAW		.1f
#define MAINCAM_SENSITIVITY_PITCH	.05f

#define MAINCAM_DEBUG_MOVEMENT_SPEED 5.f

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Player final : public CCinematicCamera
{
public:
	typedef struct tagCameraMainDesc
	{
		CAMERA_DESC	tCameraDesc;
	} CAMERA_MAIN_DESC;

private:
	explicit CCamera_Player(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCamera_Player() DEFAULT;

public:
	virtual HRESULT						Initialize(any aDesc = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
#ifdef _DEBUG
	const _bool							Is_Debug() const { return m_bDebugMode; }

	void								Set_DebugMode(_bool bDebugMode = true) { m_bDebugMode = bDebugMode; }
#endif

private:
	void								Handle_MouseInput(_float fTimeDelta);
#ifdef _DEBUG
	void								Handle_MouseInput_Debug(_float fTimeDelta);
#endif

	void								Move(_float fTimeDelta);
	void								Physics(_float fTimeDelta);
	void								Shake(_float fTimeDelta);
private:
	CAMERA_MAIN_DESC					m_tCameraMainDesc{};

	_float2								m_vSensitivty = _float2(MAINCAM_SENSITIVITY_YAW, MAINCAM_SENSITIVITY_PITCH);
#ifdef _DEBUG
	_bool								m_bDebugMode = false;
#endif

	_bool								m_bExecuted[10] = { false };
	_bool								m_bLockOn = { false };
	_bool								m_bFixed = { false };
	_bool								m_bHandle = { true };
	shared_ptr<class CTransform>		m_pPlayerTransform = {nullptr};
	shared_ptr<class CTransform>		m_pMonsterTransform = { nullptr };
	shared_ptr<class CTransform>		m_pTargetTransform;
	weak_ptr<class CMonster>			m_pLockedMonster;

	_bool								m_bShake = false;
	_bool								m_bShakeNonDelta = false;
	_float								m_fShakeTime = 0.3f;
	_float								m_fCurrentShakeTime = m_fShakeTime;
	_float								m_fShakeX = 0, m_fShakeY = 0;
	_float								m_fShakeAmount = 1.f;
	_float								m_fLerpTime = 0.075f;
	_bool								m_bLerpMove = { true };
	_float								m_fTheta = 0.f;
	_float3								m_vPreShakeCamPos;

public:
	static shared_ptr<CCamera_Player>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
public:
	
	void								Set_LockOn(_bool b) { m_bLockOn = b; }
	_bool								Get_LockOn() { return m_bLockOn; }
	void								Set_LockedMonster(shared_ptr<class CMonster> Monster) { m_pLockedMonster = Monster; }
	weak_ptr<class CMonster>			Get_LockedMonster() { return m_pLockedMonster; }
	void								Set_LockOnTransform(shared_ptr<class CTransform> vPos) { m_pMonsterTransform = vPos;}
	shared_ptr<class CTransform>		Get_LockOnTransform() { return m_pMonsterTransform; }
	void								Set_Fixed(_bool b) { m_bFixed = b; }
	_bool								Get_Shake() { return m_bShake; }
	void								Set_Shake(_float time = 0.3f, _float amount = 1.f);
	void								Set_Shake_NonDelta(_float time = 0.3f, _float amount = 1.f);
	void								Set_Target(shared_ptr<CTransform> pTransform = nullptr, _float fTheta = 0.f) { m_pTargetTransform = pTransform; m_fTheta = fTheta; }

	_bool Get_Handle() { return m_bHandle; }
	void Set_Handle(_bool b) { m_bHandle = b; }
	void Set_LerpTime(_float f) { m_fLerpTime = f; }
	_bool Get_LerpMove() { return m_bLerpMove; }
	void Set_LerpMove(_bool b) { m_bLerpMove = b; }
};

END
