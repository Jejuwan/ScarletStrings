#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CMonster_Projectile final : public CGameObject
{
private:
	explicit CMonster_Projectile(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Projectile(const CMonster_Projectile&);
	virtual ~CMonster_Projectile();

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual HRESULT						ShootAgain(any = g_aNull);
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

	PxController*						m_pController = { nullptr };
	shared_ptr<class CPhysXCollider>	m_pCollider;
	shared_ptr<class CPhysXCollider>	m_pBigAttackCollider;

	TESTOBJDESC							m_ObjDesc;
	_float								m_fTime = 0.f;

	class CPlayer* m_pPlayer;

public:
	static shared_ptr<CMonster_Projectile>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	void Free();

	PxController* Get_Controller() { return m_pController; }
	shared_ptr<class CPhysXCollider> GetCollider() { return m_pCollider; }

};

END
