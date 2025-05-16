#pragma once
#include "Client_Define.h"
#include "CinematicCamera.h"

#define MAIN_CAMERA_FOV				XMConvertToRadians(60.f)
#define MAIN_CAMERA_NEAR			0.1f
#define MAIN_CAMERA_FAR				10000.f
#define MAINCAM_PITCH_MIN			-89.f
#define MAINCAM_PITCH_MAX			+89.f

#define MAINCAM_SENSITIVITY_YAW		1.f
#define MAINCAM_SENSITIVITY_PITCH	1.f

#define MAINCAM_DEBUG_MOVEMENT_SPEED 5.f

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Main final : public CCinematicCamera
{
public:
	typedef struct tagCameraMainDesc
	{
		CAMERA_DESC	tCameraDesc;
	} CAMERA_MAIN_DESC;

private:
	explicit CCamera_Main(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCamera_Main() DEFAULT;

public:
	virtual HRESULT						Initialize(any aDesc = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Lock_MouseInput(_bool bLock) { m_bLockMouseInput = bLock; }

public:
#ifdef _DEBUG
	const _bool							Is_Debug() const						{ return m_bDebugMode; }

	void								Set_DebugMode(_bool bDebugMode = true)	{ m_bDebugMode = bDebugMode; }
#endif

private:
	void								Handle_MouseInput(_float fTimeDelta);
#ifdef _DEBUG
	void								Handle_MouseInput_Debug(_float fTimeDelta);
#endif

private:
	CAMERA_MAIN_DESC					m_tCameraMainDesc{};

	_float2								m_vSensitivty	= _float2(MAINCAM_SENSITIVITY_YAW, MAINCAM_SENSITIVITY_PITCH);
#ifdef _DEBUG
	_bool								m_bDebugMode	= false;
#endif

private:
	_bool								m_bLockMouseInput = { false };

public:
	static shared_ptr<CCamera_Main>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
