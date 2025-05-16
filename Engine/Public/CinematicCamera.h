#pragma once
#include "Engine_Define.h"
#include "Camera.h"

BEGIN(Engine)

class ENGINE_DLL CCinematicCamera abstract : public CCamera
{
protected:
	explicit CCinematicCamera(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCinematicCamera() DEFAULT;

public:
	virtual HRESULT									Initialize(any) override;
	virtual void									Tick(_float fTimeDelta) override;

public:
	void											Append_Cinematic(const wstring& wstrFilePath, const wstring& wstrCinCameraTag);
	void											Clear_Cinematic();

public:
	void											Play_CinematicCamera(const wstring& wstrCinematicTag, const _float4x4& matPivot = g_mUnit);
	void											Pause_CinematicCamera();
	void											Continue_CinematicCamera();

protected:
	virtual HRESULT									Ready_Components() override;

public:
	void											Subscribe_Cinematic_Finish_Callback(const std::function<void(const wstring&)>& fpCallback);

	shared_ptr<class CKeyframe>						m_pKeyframe;

public:
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override	PURE;
};

END