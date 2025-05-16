#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CFSM;
END

BEGIN(Client)

class CTeammate abstract : public CGameObject
{
protected:
	explicit CTeammate(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTeammate(const CTeammate&);
	virtual ~CTeammate() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() PURE;
	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;

	virtual HRESULT						Ready_Components() override;

public:
	virtual void						Set_Animation(_uint iAnimationIndex, _float fPlaySpeed = 1.f, _bool bReverse = false, _float fInterpolationDuration = g_fDefaultInterpolationDuration, _bool bLoop = true);
	_uint								Get_CurrentAnimation();

	_float								Get_GravityVelocity() { return m_fGravityVelocity; }

public:
	void								Enable_Controller(_bool bEnable);
	void								Enable_FSM(_bool bEnable) { m_bEnableFSM = bEnable; }

public:
	virtual void						Enable_Weapon(_bool bEnableWeapon) { m_bEnableWeapon = bEnableWeapon; }
	virtual void						Active_Weapon(_bool bActive);

public:
	_bool								Get_Transparency() { return m_bTransparency; }

public:
	_bool								Detect_Obstacle(PxRaycastBuffer& hit, _float fRayDistance = 1.f);
	_bool								Detect_Ground(_float4& vGroundPosition);

public:
	void								Init_RootMotion();

public:
	void								Update_Motion(_float fTimeDelta);

	void								RootMotion(_float fTimeDelta, _bool bCheckGround = true);
	void								Motion(_float fTimeDelta);

	void								Reset_Motion();
	PxControllerCollisionFlags			Get_CurrentCollisionFlag() { return m_CollisionFlag; }

public:
	void								Enable_Gravity(_bool bGravity) { m_bGravityEnable = bGravity; }

public:
	PxController*						Get_Controller() { return m_pController; }

protected:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

protected:
	_bool								m_bTransparency = { false };

	float3								m_vRimColor = float3(1.f, 1.f, 1.f);
	_float								m_fRimStrength = { 0.1f };
	_float								m_fRimPower = { 7.f };

protected:
	ACTORDESC							m_ActorDesc;

	PxController*						m_pController = { nullptr };
	PxControllerFilters					m_ControllerFilters;

	shared_ptr<class PxQueryFilterCallback> m_ControllerFilterCallBack;
	shared_ptr<class PxControllerFilterCallback> m_CCTFilterCallBack;
	shared_ptr<class PxUserControllerHitReport> m_pPlayerControllerHitReport;

protected:
	_float								m_fGravityVelocity = { 0.f };
	_bool								m_bGravityEnable = { true };
	PxControllerCollisionFlags			m_CollisionFlag;

	shared_ptr<class CPhysXCollider>	m_pCollider;
	PxExtendedVec3						m_vControllerPivot = { 0.f,0.f,0.f };

	_float4								m_vPrePos;
	_float4								m_vPreRootPos;
	_float4								m_vResetPos;

protected:
	shared_ptr<CFSM>					m_pFSM;
	_bool								m_bEnableFSM = true;

protected:
	_bool								m_bEnableWeapon = { false };

public:
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) PURE;
};

END