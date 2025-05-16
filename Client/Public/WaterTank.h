#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Engine)
class CObjectPool;
END

BEGIN(Client)

class CWaterTank final : public ISystem
{
private:
	explicit CWaterTank(_float, _float);
	virtual ~CWaterTank() DEFAULT;

public:
	HRESULT									Initialize(shared_ptr<class CMonster_DispenPerry>);
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;

public:
	const _float							Get_HP(_bool bMaxHp = false) const										{ return bMaxHp ? m_tEntityDesc.fMaxHP : m_tEntityDesc.fHP; }
	const DP_STATE							Get_DamageState() const													{ return m_eDamageState; }

	void									Set_DamageLevel(_float fHp, DP_STATE);
	DP_STATE								Damage(_float fAtk);

	void									Fire_WaterDrop(_bool bMultiWaterShot = false);

private:
	weak_ptr<class CMonster_DispenPerry>	m_pOwner;
	ENTITYDESC								m_tEntityDesc{};

	DP_STATE								m_eDamageState		= DP_STATE::NOEVENT;

	PxController*							m_pController = { nullptr };
	ACTORDESC*								m_pActorDesc = { nullptr };
	map<_float, DP_STATE>					m_mapDamageLevel;
	map<_float, DP_STATE>::iterator			m_itDamageLevel;

	_bool									m_bBroken			= false;

	const _float							m_fHealSpeed		= 0.f;

	shared_ptr<CObjectPool>					m_pWaterPool;

	shared_ptr<CTransform>					m_pParentTransform	= nullptr;
	shared_ptr<CTransform>					m_pTargetTransform	= nullptr;
	shared_ptr<CTransform>					m_pDumpTransform	= nullptr;
	_float4x4								m_mPivot			= g_mUnit;
	const _float4x4*						m_mBone				= nullptr;

	_float4x4								m_mShellRadians		= g_mUnit;
	pair<_float4x4, _float4x4>				m_pairRotationRadians;

public:
	static shared_ptr<CWaterTank>			Create(shared_ptr<class CMonster_DispenPerry>, _float fMaxHP, _float fHeal);
};

END
