#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CModel;
class CLight;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	explicit CPlayer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPlayer(const CPlayer&);
	virtual ~CPlayer();

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT						Render_OutLine() override;
	virtual HRESULT						Render_Neon() override;

private:
	virtual HRESULT						Ready_Components() override;
	
	HRESULT								Initialize_Weapon();
	HRESULT								Initialize_FootCollider();
	HRESULT								Initialize_Tentacle();

public:
	HRESULT								Enable_WeaponVFX(_bool bElec = true);
	void								Enable_ChargeVFX();
	void								Set_WeaponEmissive(_float4 vColor);
	void								SAS_Overdrive();

	void								Enable_BrainField(_bool bEnableBrainField);


public:
	void								Set_Gravity(_float fGravity);
	_float								Get_Gravity();
	
public:
	void								Set_Transparency(_bool bTansparency);
	void								Blink_Transparency(_float fTimeDelta);

public:
	void								Exit_BrainField();

public:
	void								Add_RimEffect(const RIMEFFECTDESC& tRimEffectDesc);
	void								Clear_RimEffect();

public:
	void								Enable_OutLine(_bool bOutLine, _float fTimeDelta = 0.f);

#ifdef _DEBUG
public:
	void								Free_Move();

public:
	void								Debug_Transparency_Parameters();
#endif

private:
	_bool								m_bDebug	= false;

//#endif
private:
	HRESULT								Render_Player();
	HRESULT								Render_RimPlayer();

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

private:
	vector<pair<_uint, _uint>>			m_vecMeshPassIndexPair;

	PxController* m_pController = { nullptr };
	PxControllerFilters m_ControllerFilters;
	shared_ptr<class PxQueryFilterCallback> m_ControllerFilterCallBack;
	shared_ptr<class PxControllerFilterCallback> m_CCTFilterCallBack;
	shared_ptr<class PxUserControllerHitReport> m_pPlayerControllerHitReport;

	shared_ptr<class CPhysXCollider> m_pLockOnCollider;
	shared_ptr<class CPhysXCollider> m_pFootCollider;

	shared_ptr<class CUI_SkillSlot>		m_pSkillSlot;
	shared_ptr<class CPlayerState> m_ePreState;
	shared_ptr<class CPlayerState> m_eCurState;
	shared_ptr<class CPlayerState> m_eForceChangeState = { nullptr };

	PLAYER_DIR m_eDir = PLAYER_DIR::F;

	_float	m_fRunStartSkipProportion = 0.f;

	vector<shared_ptr<class CPlayerWeapon>> m_Weapons;
	vector<shared_ptr<class CPlayerTentacle>> m_Tentacles;

	shared_ptr<class CCamera_Player> m_pCamera;

	ACTORDESC m_ActorDesc;
	PLAYERDESC m_PlayerDesc;

	shared_ptr<class CMonster>			m_pLockedOnMonster = { nullptr };
	shared_ptr<class CMonster>			 m_pFinishMonster = { nullptr };

private:
	_bool											m_bEnableBrainField = { false };
	vector<shared_ptr<class CBrainFieldTentacle>>	m_vecBrainFieldTentacles;

	vector<pair<_uint, _uint>>			m_vecDriveModeMeshPassIndexPair;
	list<RIMEFFECTDESC>					m_lstRimEffectDesc;

#pragma region shader
private:
	_float3								m_vOutlineColor = { 0.5f, 0.5f, 0.5f };
	shared_ptr<CTexture>				m_pTexDissolve;
	_float								m_fOutlineThin = { 0.01f };

	_bool								m_bEnableOutLine = { false };
	_bool								m_bDecreaseOutLine = { false };
	_float3								m_vOutLineColorMaxThreshold = { 0.749f, 0.623f, 0.87f };
	_float3								m_vOutLineColorMinThreshold = { 0.5f, 0.5f, 0.5f };

private:
	_bool								m_bTransparency = { false };

	float3								m_vRimColor = float3(1.f, 1.f, 1.f);
	_float								m_fRimStrength = { 0.5f };
	_float								m_fRimPower = { 5.f };
	_float								m_fBlinkPowerMaxThreshold = { 1.f };
	_float								m_fBlinkPowerMinThreshold = { 5.f };

private:
	shared_ptr<CTexture>				m_pCircularMask;
	_bool								m_bElecEffectEnable = { false };
#pragma endregion
private:
	shared_ptr<class CPsychokinesis>	m_pPsychokinesis;
	shared_ptr<class CLight>			m_pPlayerShadow;
private:
	_float m_fEffect_Timer = 0.75f;

private:
	_bool                               m_bBrainFieldExitStart = { false };

	_float                              m_fBrainFieldExitAccTime = { 0.f };
	_float                              m_fBrainFieldExitDelay = { 15.f };
	_float                              m_fBrainFieldScreenEffect = { 10.f };

private:
	shared_ptr<class CLight>			m_pPurpleLight;

public:
	static shared_ptr<CPlayer>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any eScene = g_aNull) override;
	void LockOn();
	void SAS_Entry(PLAYER_SAS _sas);
	void SAS_Tick(_float fTimeDelta);
	void SAS_Exit();
	void Drive_Entry();
	void Drive_Tick(_float fTimeDelta);
	void Drive_Exit();
	void Just_Dodge_Tick(_float fTimeDelta);
	void Free();

	PxController* Get_Controller() { return m_pController; }
	PxControllerFilters Get_ControllerFilters() { return m_ControllerFilters; }
	shared_ptr<class CPhysXCollider> GetCollider() { return m_pLockOnCollider; }
	PLAYER_DIR Get_Dir() { return m_eDir; }
	void Set_Dir(PLAYER_DIR _eDir) { m_eDir = _eDir; }
	shared_ptr<class CPlayerState> Get_PlayerPreState() { return m_ePreState; }
	shared_ptr<class CPlayerState> Get_PlayerState() { return m_eCurState; }
	vector<shared_ptr<class CPlayerWeapon>> Get_Weapons() { return m_Weapons; }
	vector<shared_ptr<class CPlayerTentacle>> Get_Tentacles() { return m_Tentacles; }
	shared_ptr<class CCamera_Player> Get_Camera() { return m_pCamera; }
	void Set_Camera(shared_ptr<class CCamera_Player> _camera) { m_pCamera = _camera; }
	void Set_ChangeState(shared_ptr<class CPlayerState> _state) { m_eForceChangeState = _state; }
	shared_ptr<class CPsychokinesis> Get_Kinesis() { return m_pPsychokinesis; }
	PLAYERDESC& Get_PlayerDesc() { return m_PlayerDesc; }
	shared_ptr<class CMonster> Get_FinishMonster() { return m_pFinishMonster; }
	void Set_FinishMonster(shared_ptr<class CMonster> _mon) { m_pFinishMonster = _mon; }
	shared_ptr<class CMonster>			Get_LockedOnMonster() { return m_pLockedOnMonster; }
	void								Set_LockedOnMonster(shared_ptr<class CMonster> _mon) { m_pLockedOnMonster = _mon; }
	ACTORDESC&							Get_ActorDesc() { return m_ActorDesc; }

	_bool								Get_ElecEffectEnable() { return m_bElecEffectEnable; };
	void								Set_ElecEffectEnable(_bool bEnable) { m_bElecEffectEnable = bEnable; };
	void								Set_WeaponsActive(_bool b);
	void								Set_BigWeaponsActive(_bool b);
};

class PlayerCCTFilterCallBack : public PxControllerFilterCallback
{
public:
	PlayerCCTFilterCallBack() {};
	virtual ~PlayerCCTFilterCallBack() {}

	/**
	\brief Filtering method for CCT-vs-CCT.

	\param[in] a	First CCT
	\param[in] b	Second CCT
	\return true to keep the pair, false to filter it out
	*/
	virtual bool filter(const PxController& a, const PxController& b);
};

END
