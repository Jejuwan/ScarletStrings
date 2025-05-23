#pragma once
#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	enum class	TYPE	{ PERSPECTIVE, ORTHOGONAL, MAX };

	typedef struct tagCameraDesc
	{
		TYPE			eType	= TYPE::MAX;
		_float4			vEye{}, vAt{}, vUp{};
		_float			fFovAngleY{}, fAspect{};
		_float			fWidth{}, fHeight{};
		_float			fNear{}, fFar{};
	} CAMERA_DESC;

protected:
	explicit CCamera(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCamera() DEFAULT;

public:
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Set_State(TRANSFORM, _vector);
	_vector								Get_State(TRANSFORM);

public:
	void								Lock(_bool bLock);
	void								Adjust_FOV(_float fTargetRadian, _float fDuration = 0.f, _float fWeight = 1.f);
protected:
	virtual HRESULT						Ready_Components() override;

	void								Release_FOV(_float fDuration = 0.f, _float fWeight = 1.f);

public:
	const CAMERA_DESC&					Get_Desc() const	{ return m_tCameraDesc; }

protected:
	shared_ptr<class CRenderer>			m_pRenderer;
	shared_ptr<class CTransform>		m_pTransform;
	shared_ptr<class CShader>			m_pShader;

	_float4x4							m_mView;
	_float4x4							m_mProjection;

private:
	CAMERA_DESC							m_tCameraDesc;

	_float								m_fTargetFovAngleY	= 0.f;

protected:
	_bool								m_bLockMovement = { false };

public:
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override	PURE;
};

END
