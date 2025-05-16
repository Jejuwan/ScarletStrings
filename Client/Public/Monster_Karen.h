#pragma once
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMonster_Karen final : public CMonster
{
private:
	explicit CMonster_Karen(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Karen(const CMonster_Karen&);
	virtual ~CMonster_Karen() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;
	virtual wstring						Get_TypeName() { return L"레벨 ??? 카렌 트레버스"; }; 
	void SetElectricAttackTrue(_float3 Position);
	void SetElectricAttackFalse();

public :
	void IceCorn(any Any);
	void IceCornDecal(any Any);

public:
	void								VFX_Kick1();
	void								VFX_Kick2();
	void								VFX_Kick3();

private:
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

private :
	_bool								m_bCloned = { false };
	_bool								m_bRealCloned = { false };
	shared_ptr<class CMonster_Karen_Cloned> m_pCloned[2];
	vector<pair<_uint, _uint>>			m_vecMeshPassIndexPair;
	shared_ptr<CGameObject>				m_pIceCorn;
	shared_ptr<class CPhysXCollider>	m_pElectricCollider;
	ACTORDESC*							m_pElectricActorDesc = {nullptr};
	_bool								m_bElectricColliderActive = { false };
	vector<shared_ptr<class CPlayerTentacle>> m_Tentacles;

private:
	shared_ptr<class CUI_Indicator_Boss_Hp>		m_pUI_BossHp;


public :
	void								SetCloned() { m_bCloned = !m_bCloned; };
	void								SetRealCloned(any Any = g_aNull);

public:
	static shared_ptr<CMonster_Karen>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	vector<shared_ptr<class CPlayerTentacle>>& Get_Tentacle() { return m_Tentacles; }
	void Tentacle_Fly();
};

END
