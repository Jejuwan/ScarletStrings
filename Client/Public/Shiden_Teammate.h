#pragma once
#include "Client_Define.h"
#include "Teammate.h"

BEGIN(Engine)
class CFSM;
class CBone;
END

BEGIN(Client)

class CShiden_Teammate final : public CTeammate
{
private:
	explicit CShiden_Teammate(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CShiden_Teammate(const CShiden_Teammate&);
	virtual ~CShiden_Teammate() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	virtual void						Active_Weapon(_bool bActive);

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<class CShiden_Weapon>	m_pLeftWeapon;
	shared_ptr<CTransform>				m_pLeftWeaponTransform;
	shared_ptr<CBone>					m_pLeftWeaponCombineBone;

	shared_ptr<class CShiden_Weapon>	m_pRightWeapon;
	shared_ptr<CTransform>				m_pRightWeaponTransform;
	shared_ptr<CBone>					m_pRightWeaponCombineBone;

	_float4x4							m_matWeaponPivot;

private:
	vector<pair<_uint, _uint>>			m_vecMeshPassIndexPair;

public:
	static shared_ptr<CShiden_Teammate>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END