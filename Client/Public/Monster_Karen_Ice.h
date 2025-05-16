#pragma once
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CTransform;
END

BEGIN(Client)

class CMonster_Karen_Ice final : public CGameObject
{
public :
	
private:
	explicit CMonster_Karen_Ice(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Karen_Ice(const CMonster_Karen_Ice&);
	virtual ~CMonster_Karen_Ice() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Fetch(any Any) override;
	virtual HRESULT						Fetch_Decal(any Any);

public :
	void Start();
private:
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

private :
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;
	shared_ptr<CAnimator>				m_pAnimator;
	_bool								m_bUpword = { false };
	shared_ptr<class CPhysXCollider>	m_pAttackCollider;
	_float4								m_vPosition = {};

private:
	shared_ptr<class CIceDecal>			m_pIceDecal;

public:
	static shared_ptr<CMonster_Karen_Ice>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	


};

END
