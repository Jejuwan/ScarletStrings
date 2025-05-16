#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "physx/PxPhysicsAPI.h"
#include "FSM.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CAnimator;
class CPhysics;
class CTexture;
END

BEGIN(Client)

class CMonster : public CGameObject
{
public :
	enum MONSTERTYPE {
		FLOWERCHEST,
		FLOWERHEAD,
		FLY,
		SNAIL,
		MIMIC,
		BUFFALO,
		TUTOBOSS,
		DISPENPERRY,
		STATUE,
		Karen,
		WATER_PROJECTILE,
		TYPE_END
	};
protected :
	explicit CMonster(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster(const CMonster&);
	virtual ~CMonster() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_OutLine() override;
	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;

	virtual HRESULT						Fetch(any Any = g_aNull) override;
	virtual _bool						Return() override;
	string GetWeakBoneName()			{ return m_strWeakBoneName; };
	//virtual void						SetCoolTime(_float CoolTime) { //m_tMonsterDesc.CoolTime = CoolTime; }
	
	// For WeakRender
	void								SetWeakRender(_bool Weak) { m_bIsWeak = Weak; };

#pragma region For RootMotion
	virtual void						InitRootMotion();		
	virtual void						Update_Motion(_float fTimeDelta);
	virtual void						AirBone(_float fTimeDelta);
	virtual void						SetAirBoneTimeZero() { m_fAirBoneTime = 0.f; };
	virtual void						No_RootMotion(_float fTimeDelta);		
	virtual void						RootMotion(_float fTimeDelta, _bool OneFrame = true);
	virtual void						MotionReset();
	void								ReleaseController();
	PxControllerCollisionFlags			GetCollisionFlags() { return m_CollisionFlag; };
	_bool								Is_PxColUp()		{ return m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_UP; }
	_bool								Is_PxColSides()		{ return m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_SIDES; }
	_bool								Is_PxColDown()		{ return m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN; }
	PxControllerCollisionFlags			GetCollisionFalgs() { return m_CollisionFlag; };
	virtual void						ShootBulletToPlayer();
	virtual void Dissolve_Weak();

	_float								Height();
	void Set_HP(_float fHp) { m_tEntityDesc.fHP = fHp; m_pNormalActorDesc->bHit = true; };

	virtual void						SetWeaponeActiveOn() { m_pAttackActorDesc->bActive = true; };
	virtual void						SetWeaponeActiveOff() { m_pAttackActorDesc->bActive = false; };
	ACTORDESC&							GetAttackActorDesc(){ return *m_pAttackActorDesc; }
#pragma endregion
	void								Update_Physics_PhysX(_float fTimeDelta);
	MONSTERTYPE							Get_Type() { return m_eMonsterType; }
	virtual wstring						Get_TypeName() = 0;
	void								SetDead() { SetWeaponeActiveOff(); SetWeakRender(false); m_bDead = true; }
	_bool								GetDead() { return m_bDead; }
	_bool								Get_LockOnAble() { return m_bLockOnAble; }
	void								Set_LockOnAble(_bool b) { m_bLockOnAble = b; }
	_bool								Get_BcChance() { return m_bBcChance; }
	void								Set_BcChance(_bool b) { m_bBcChance = b; }
	_float4								Get_PhysXPivot() {
		return _float4(m_vControllerPivot.x, m_vControllerPivot.y, m_vControllerPivot.z, 1.f);
	}
	ENTITYDESC							Get_EntityDesc() { return m_tEntityDesc; }
	ENTITYDESC&							Get_EntityDesc_Ref() { return m_tEntityDesc; }
#pragma region physx
#pragma endregion

#pragma region For Dissolve
	virtual void						Dissolve();
	_bool								Is_Dissolved() { return 1.f >= m_fDissolveThreshold; }

#pragma endregion

	void								DeadSlow();
protected:
	virtual HRESULT						Render(_uint iPassIndex);

	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

public:
	shared_ptr<class CUI_Indicator_Monster_Hp> Get_UI_HP()	{ return m_pUI_Hp; }

protected:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;
	shared_ptr<CAnimator>				m_pAnimator;
	shared_ptr<CPhysics>				m_pPhysics;

	const float4x4*						m_mRootBoneMat = { nullptr };
	float4x4							m_mRootMat1FrameAgo = g_mUnit;

	_uint								m_iCurAnimIndex = { 0 };

	PxController*						m_pController = { nullptr };
	PxControllerFilters					m_ControllerFilters;
	shared_ptr<class PxQueryFilterCallback> m_ControllerFilterCallBack;
	shared_ptr<class PxControllerFilterCallback> m_CCTFilterCallBack;
	shared_ptr<class PxUserControllerHitReport> m_pMonsterControllerHitReport;

	shared_ptr<class CPhysXCollider>	m_pCollider;
	PxExtendedVec3						m_vControllerPivot = { 0.f,0.f,0.f };

	shared_ptr<class CPhysXCollider>	m_pAttackCollider;
	shared_ptr<class CPhysXCollider>    m_pBodyCollider;
	shared_ptr<class CPhysXCollider>	m_pBigAttackCollider;

	ACTORDESC*							m_pNormalActorDesc = {nullptr};
	ACTORDESC*							m_pBigActorDesc = {nullptr};
	ACTORDESC*							m_pAttackActorDesc = {nullptr};
	ACTORDESC*							m_pBigAttackActorDesc = { nullptr };
	ACTORDESC							m_ActorDesc;

	_float								m_fTime = { 0.f };
	_float								m_fAirBoneTime = { 0.f };
	PxControllerCollisionFlags			m_CollisionFlag;
	_vector								m_vPrePos;
	//_vector								m_vInitPos;
	_float4								m_vPreRootPos;
	_float4								m_vResetPos;
	shared_ptr<CFSM>					m_pFSM;
	shared_ptr<CTexture>				m_pTexDissolve;

	string								m_strAttackBoneName;
	string								m_strCenterBoneName = "Hips";
	string								m_strWeakBoneName = "Target";
	shared_ptr<CGameObject>				m_pProjectile;
	_bool								m_bIsWeak = { false };
	shared_ptr<CGameObject>				m_pWeak;

	MONSTERTYPE							m_eMonsterType = { TYPE_END };
	_bool								m_bDead = { false };
	_bool								m_bLockOnAble = { true };
	_bool								m_bBcChance = { false };
	_bool								m_bBulldozerHit = { false };

protected :
	_bool								m_bDissolve = false;
	_float								m_fDissolveThreshold = 0.f;
	_float								m_fDissolveDuration = 1.f;

private:
	_bool								m_bDeadSlow = { false };
	_float								m_fDeadTime = 0.5f;
protected:
	_float								m_fOutLineSize = { 0.01f };
	_float3								m_vOutLineColor = { 0.f, 0.f, 0.f };
	_matrix								m_mAttackColliderPivot = g_mUnit;

#pragma region UI
	shared_ptr<class CCamera_Player>			m_pCamera_Player;
	shared_ptr<class CUI_Indicator_Monster_Hp>	m_pUI_Hp;
#pragma endregion

public:
	PxController* Get_Controller() { return m_pController; }
	shared_ptr<class CPhysXCollider> GetAttackCollider() { return m_pAttackCollider; }
	shared_ptr<class CPhysXCollider> GetBodyCollider() { return m_pBodyCollider; }
	PxControllerFilters				 GetControllerFilters() { return m_ControllerFilters; }
	void							 SetBulldozerHit(_bool b) { m_bBulldozerHit = b; }
	_bool							 GetBulldozerHit() { return m_bBulldozerHit; }
	ACTORDESC* Get_NormalActorDesc() { return m_pNormalActorDesc; }
	shared_ptr<class CPhysXCollider> GetBigAttackCollider() { return m_pBigAttackCollider; }
	ACTORDESC* Get_BigAttackActorDesc() { return m_pBigAttackActorDesc; }
	public:
		void FSM_GotoFinish();
	
};

END
