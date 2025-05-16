#pragma once
#include "Client_Define.h"
#include "Map_Special_Dynamic.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
class CModelRenderer;
END

BEGIN(Client)

class CMonster_FallenObject final : public CMap_Special_Dynamic
{
public:
	struct FALLENDESC {
		_float4 InitPos;
		_bool Randomize = false;
	};
private:
	explicit CMonster_FallenObject(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_FallenObject(const CMonster_FallenObject&);
	virtual ~CMonster_FallenObject();

public:
	virtual HRESULT						Initialize_Prototype(_uint Type);
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;
	virtual HRESULT						Fetch(any) override;
	virtual _bool						Return() override;
private:
	virtual HRESULT						Ready_Components() override;

private:
	vector<shared_ptr<CMap_Special_Dynamic>> m_DuplicatedBeam;
	vector<shared_ptr<CMap_Special_Dynamic>> m_Debris;

	//PxController*						m_pController = { nullptr };
	ACTORDESC* m_pActorDesc = { nullptr };

	TESTOBJDESC							m_ObjDesc;
	_float								m_fTime = 0.f;
	PxTransform							m_PxTransform;
	_uint								m_Type = 3;
	_bool								m_bRandomize = false;
	_bool								m_bFetched = false;
	_float								m_fFetchedTime = 0.f;

	_float3							m_vHitDir[5];
	_float3							m_vRockHitDir[7];
	_float3							m_vDebrisHitDir[100];
	_float							m_fDebrisFallTime = 0.f;
	_int							m_iHitCount = 0;
	_bool							m_bHitSuccess = { false };

	_float3							m_vHeavyPrePos;

	_float							m_fBeamHitTime = 0.4f;
	_float							m_fBeamRotTime = 0.5f;
	_bool							m_bBeamThrowHit = false;

	_bool							m_bSlowStart = false;
	_bool							m_bSlowEnable = false;
	_float							m_fSlowTime = 0.1f;


	_float3 m_vLongPos1[10];

	//class CPlayer* m_pPlayer;
	_bool m_bInteractive = { false };

private:
	shared_ptr<CModelRenderer>			m_pModelRenderer;
	wstring								m_wstrModelTag;

private:
	INSTMODELDESC						m_tInstModelDesc;
	INSTMODELMESHDESC					m_tInstModelMeshDesc;

private:
	shared_ptr<class CUI_Indicator_SpcObjectAttack_Rotation_Beam>	m_pSpcObject_Rotation_Beam;
	shared_ptr<class CUI_Indicator_SpcObjectAttack_Rock>			m_pSpcObject_Rock;

public:
	static shared_ptr<CMonster_FallenObject>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _uint);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	void Free();

	void Set_Target_Statue();

	void Tick_SawTooth(_float fTimeDelta);
	void Tick_Rock(_float fTimeDelta);
	void Tick_Long(_float fTimeDelta);
	

	virtual void Throw() override;
	virtual void ThrowHit() override;
	_uint GetFallenType() { return m_Type; }
	//PxController* Get_Controller() { return m_pController; }
	//shared_ptr<class CPhysXCollider> GetCollider() { return m_pCollider; }

};

END
