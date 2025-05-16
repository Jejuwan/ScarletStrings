#pragma once

#include "Client_Define.h"
#include "Map_Special_Dynamic.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
END

BEGIN(Client)

class CMap_Stage1_SpecialDynamic : public CMap_Special_Dynamic
{
protected:
	explicit CMap_Stage1_SpecialDynamic(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Stage1_SpecialDynamic(const CMap_Stage1_SpecialDynamic&);
	virtual ~CMap_Stage1_SpecialDynamic() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	void	Add_ModelCom();
	void	Add_SpecialType();
	void	Throw_Ready_Heavy_Equipment(_float fTimeDelta);
	void	Throw_Ready_Beam(_float fTimeDelta);
	void	Throw_Ready_Rock(_float fTimeDelta);
	void	Throw_Ready_Bulldozer(_float fTimeDelta);

private:
	vector<shared_ptr<CMap_Stage1_SpecialDynamic>> m_DuplicatedBeam;
	vector<shared_ptr<CMap_Stage1_SpecialDynamic>> m_Debris;
	_bool							m_bBeamHit = { false };

	_float3							m_vHitDir[5];
	_float3							m_vRockHitDir[7];
	_float3							m_vDebrisHitDir[100];
	_float							m_fDebrisFallTime = 0.f;
	_int							m_iHitCount = 0;
	_bool							m_bHitSuccess = { false };

	_float3							m_vHeavyPrePos;

	shared_ptr<class CPhysXCollider> m_pCollider;
	shared_ptr<class CUI_Indicator_SpcObjectAttack_Rotation_Beam>	m_pSpcObject_Rotation_Beam;
	shared_ptr<class CUI_Indicator_SpcObjectAttack_Rock>			m_pSpcObject_Rock;
	_float							m_fBeamHitTime = 0.4f;
	_float							m_fBeamRotTime = 0.5f;
	_bool							m_bBeamThrowHit = false;

	_bool							m_bSlowStart = false;
	_bool							m_bSlowEnable = false;
	_float							m_fSlowTime = 0.1f;
private:
	void							Tick_HeavyEquipment();
	void							Tick_Rock(_float fTimeDelta);
	void							Tick_Bulldozer(_float fTimeDelta);
public:
	static shared_ptr<CMap_Stage1_SpecialDynamic>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	shared_ptr<class CPhysXCollider> Get_Collider() { return m_pCollider; }
public:
	virtual void Throw() override;
	virtual void ThrowHit() override;
	virtual _bool	Return() override;
};

END
