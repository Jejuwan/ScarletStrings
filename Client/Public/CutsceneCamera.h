#pragma once
#include "Client_Define.h"
#include "CinematicCamera.h"

#define CUTSCENE_CAMERA_FOV				XMConvertToRadians(60.f)
#define CUTSCENE_CAMERA_NEAR			0.1f
#define CUTSCENE_CAMERA_FAR				10000.f

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCutsceneCamera final : public CCinematicCamera
{
private:
	explicit CCutsceneCamera(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCutsceneCamera() DEFAULT;

public:
	virtual HRESULT						Initialize(any aDesc = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	CAMERA_DESC							m_tCameraMainDesc{};

public:
	static shared_ptr<CCutsceneCamera>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END