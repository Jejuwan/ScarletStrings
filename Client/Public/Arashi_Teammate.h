#pragma once
#include "Client_Define.h"
#include "Teammate.h"

BEGIN(Engine)
class CFSM;
class CBone;
END

BEGIN(Client)

class CArashi_Teammate final : public CTeammate
{
private:
	explicit CArashi_Teammate(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CArashi_Teammate(const CArashi_Teammate&);
	virtual ~CArashi_Teammate() DEFAULT;

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
	shared_ptr<class CArashi_Weapon>	m_pWeapon;
	shared_ptr<CTransform>				m_pWeaponTransform;
	shared_ptr<CBone>					m_pWeaponCombineBone;

private:
	vector<pair<_uint, _uint>>			m_vecMeshPassIndexPair;

public:
	static shared_ptr<CArashi_Teammate>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END