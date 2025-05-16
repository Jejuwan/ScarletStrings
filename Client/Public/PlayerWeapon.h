#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CPlayerWeapon final : public CGameObject
{
private:
	enum class VFX_STATE { STATIC, FADEIN, TRAIL, FADEOUT, MAX };

private:
	explicit CPlayerWeapon(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPlayerWeapon(const CPlayerWeapon&);
	virtual ~CPlayerWeapon();

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;
	virtual HRESULT						Render_Bloom() override;

private:
	virtual HRESULT						Ready_Components() override;

public:
	void								Apply_Effect(_bool bElec = false);

	void								Set_Emissive(_float4 vColor);

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

	PxController*						m_pController		= { nullptr };
	ACTORDESC							m_ActorDesc;
	shared_ptr<class CPhysXCollider>	m_pCollider;
	shared_ptr<class CPhysXCollider>	m_pBigCollider;

	unique_ptr<class CPlayerState>		m_eCurState;

	WEAPONDESC							m_WeaponDesc;

	class CPlayer*						m_pPlayer;

	VFX_STATE							m_eVFXState			= VFX_STATE::STATIC;
	_float								m_fVFXRatio			= 0.f;
	_float								m_fVFXDurationFade	= 1.f;
	array<_float, 4>					m_fVFXFadeScope;

public:
	static shared_ptr<CPlayerWeapon>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	void Free();

	PxController* Get_Controller() { return m_pController; }
	shared_ptr<class CPhysXCollider> GetCollider() { return m_pCollider; }
	shared_ptr<class CPhysXCollider> GetBigCollider() { return m_pBigCollider; }

};

END
