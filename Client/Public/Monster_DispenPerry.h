#pragma once
#include "Client_Define.h"
#include "Monster.h"

#define DISPENPERRY_MDL_PATH	TEXT("Bin/Resources/Model/DispenPerry.mdl")

#define WATERTANK_MAXHP			100.f
#define WATERTANK_HEALSPD		5.f

#define DP_FIELDCENTER			_float3(-260.f, -30.f, 0.f)
#define DP_FIELDRANGE			40.f

BEGIN(Client)

class CMonster_DispenPerry final : public CMonster
{
private:
	explicit CMonster_DispenPerry(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_DispenPerry(const CMonster_DispenPerry&);
	virtual ~CMonster_DispenPerry() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any = g_aNull) override;
	_bool										Detect_Ground(_float4& vGroundPosition);
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

	//virtual HRESULT								Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT								Render_OutLine() override;
	virtual wstring								Get_TypeName() { return L"레벨 20 디스펜 페리"; }

	virtual void								SetWeapone1ActiveOn()	{ m_pAttack1ActorDesc[0]->bActive = true; };
	virtual void								SetWeapone1ActiveOff()	{ m_pAttack1ActorDesc[0]->bActive = false; }
	virtual void								SetWeapone2ActiveOn()	{ m_pAttack1ActorDesc[1]->bActive = true; };
	virtual void								SetWeapone2ActiveOff()	{ m_pAttack1ActorDesc[1]->bActive = false; };
	virtual void								SetWeapone3ActiveOn()	{ m_pAttack1ActorDesc[2]->bActive = true; };
	virtual void								SetWeapone3ActiveOff()	{ m_pAttack1ActorDesc[2]->bActive = false; };
	virtual void								SetWeapone4ActiveOn()	{ m_pAttack1ActorDesc[3]->bActive = true; };
	virtual void								SetWeapone4ActiveOff()	{ m_pAttack1ActorDesc[3]->bActive = false; };
	virtual void								SetWeapone5ActiveOn()	{ m_pAttack1ActorDesc[4]->bActive = true; };
	virtual void								SetWeapone5ActiveOff()	{ m_pAttack1ActorDesc[4]->bActive = false; };
	virtual void								SetWeapone6ActiveOn()	{ m_pAttack1ActorDesc[5]->bActive = true; };
	virtual void								SetWeapone6ActiveOff()	{ m_pAttack1ActorDesc[5]->bActive = false; };
	virtual void								SetWeaponeAllActiveOn() ;
	virtual void								SetWeaponeAllActiveOff();
private:
	virtual HRESULT								Ready_Components() override;
	virtual HRESULT								Ready_Behaviors() override;

public:
	virtual void								RootMotion(_float fTimeDelta, _bool bOneFrame = true) override;

public:
	shared_ptr<class CWaterTank>				Get_WaterTank() const		{ return m_pWaterTank; }
	DP_STATE									Get_State() const			{ return m_eState; }
	DP_STATE*									Get_StatePtr()				{ return &m_eState; }
	void										Set_State(DP_STATE eState)	{ m_eState = eState; }

	pair<DP_PARTS, DP_STATE>					Get_RecentEvent() const		{ return m_pairPartState; }
	const _float								Get_HP(DP_PARTS) const;
	const DP_STATE								Damage(DP_PARTS, _float fAtk = 0.f);

	void										WaterShot(_bool bMultiWaterShot = false);

private:
	void										Set_DamageLevel(_float fHp, DP_STATE);
	const DP_STATE								Damage(_float fAtk = 0.f);
	void										Check_Hit();

private:
	shared_ptr<class CWaterTank>				m_pWaterTank;

	map<_float, DP_STATE>						m_mapDamageLevel;
	map<_float, DP_STATE>::iterator				m_itDamageLevel;

	DP_STATE									m_eState		= DP_STATE::NOEVENT;

	pair<DP_PARTS, DP_STATE>					m_pairPartState;

#pragma region UI
	shared_ptr<class CCamera_Player>			m_pCamera_Player;
	shared_ptr<class CUI_Indicator_Boss_Hp>		m_pUI_Hp;
	shared_ptr<class CUI_Indicator_Monster_Hp>	m_pUI_Hp_Small;
	shared_ptr<class CUI_Indicator_Name_Boss>	m_pUI_Name;
	_bool										m_bOnce1;
	_bool										m_bOnce2;
#pragma endregion
	shared_ptr<class CPhysXCollider>			m_pAttack1Collider[6];
	string										m_strAttack1BoneName[6];
	ACTORDESC*									m_pAttack1ActorDesc[6] = { nullptr };

private:
	_bool										m_bCutscene = { true };
	_bool										m_bLanding = { false };

	_float										m_fCutsceneAccTime = { 0.f };
	_float										m_fCutsceneTimeThreshold = { 6.5f };

	_bool										m_bOriginDOFEnable = { false };
	_float										m_fOriginDOFDepthStart = { 0.f };
	_float										m_fOriginDOFDepthRange = { 0.f };

	_float4x4									m_mRootPrev;
	_float4x4									m_mRootCur;

public:
	static shared_ptr<CMonster_DispenPerry>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END
