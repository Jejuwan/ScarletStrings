#pragma once
#include "Client_Define.h"
#include "EffectLayer.h"
#include "EffectRegular_Visual.h"
#include "EffectRegular_VisualCurve.h"
#include "EffectRegular_VisualDetailCurve.h"
#include "EffectRegular_VisualMesh.h"
#include "EffectRegular_MeshAnim.h"
#include "EffectRegular_Distortion.h"
#include "EffectInstance_ParticlePoint.h"
#include "EffectInstance_ParticlePointTarget.h"
#include "EffectInstance_TrailLineDiffuse.h"
#include "EffectInstance_TrailQuadDiffuse.h"
#include "EffectInstance_MeshAfterImage.h"
#include "EffectInstance_ParticleMesh.h"
#include "EffectInstance_ParticleRect.h"
#include "EffectInstance_ParticleLine.h"
#include "EffectInstance_ParticlePointCurve.h"
#include "EffectInstance_ParticlePointDynamicCurve.h"
#include "EffectInstance_ParticlePointVectorFieldCurve.h"

#define VFXMGR		CEffect_Manager::Get_Instance()

BEGIN(Engine)
class CObjectPool;
END

BEGIN(Client)

enum class VFX
{
	KINESIS_DIFFUSE,
	KINESIS_DISTORTION,
	KINESIS_PARTICLE,
	FADE_PARTICLE,
	PLAYER_WEAPON_DIFFUSE,
	PLAYER_WEAPON_DISTORTION,
	PLAYER_WEAPON_INITIALIZE,		// VisualCurve	1
	AFTERIMAGE,
//	TEST,
	PLAYER_WEAPON_DIFFUSE_ELEC,
	PLAYER_WEAPON_INITIALIZE_ELEC,	// VisualCurve	2
	PLAYER_WEAPON_PARTICLE_ELEC0,
	PLAYER_WEAPON_PARTICLE_ELEC1,
	PLAYER_WEAPON_PARTICLE_ELEC2,
	PLAYER_WEAPON_PARTICLE_ELEC3,
	PLAYER_WEAPON_PARTICLE_ELEC4,
	PLAYER_WEAPON_PARTICLE_ELEC5,
	PLAYER_WEAPON_PARTICLE0,
	PLAYER_SYM,						// DynamicCurve
//	PLAYER_EXECUTE_HAND_0,			// VisualMesh
//	PLAYER_EXECUTE_HAND_1,			// VisualMesh
	PLAYER_EXECUTE_PARTICLE_HAND,	// VectorField
	PLAYER_EXECUTE_PARTICLE_R,		// DynamicCurve
	PLAYER_EXECUTE_PARTICLE,		// VectorField
	PLAYER_EXECUTE_PARTICLE_EXP,	// DynamicCurve
//	PLAYER_EXECUTE_DISTORTION,		// VisualCurve
//	PLAYER_EXECUTE_LIG,				// VisualCurve
//	PLAYER_EXECUTE_LIG_EXP,			// VisualCurve
	PLAYER_EXECUTE_SMK_0,			// DetailCurve
	PLAYER_EXECUTE_SMK_1,			// DetailCurve
	PLAYER_EXECUTE_SMK_2,			// DetailCurve
	HIT_FLUIDMESH,					// MeshAnim
	HIT_REGULAR_EXP,				// VisualCurve
	HIT_REGULAR_IMP,				// VisualCurve
	HIT_REGULAR_LIG,				// VisualCurve
	HIT_REGULAR_PARTICLE,			// ParticlePointDynamicCurve
	SPAWN_DIFFUSE,
	SPAWN_DISTORTION,
	EM800_BEAM_DIFFUSE1,
	EM800_BEAM_DIFFUSE2,
	EM800_BEAM_DISTORTION,
	EM800_BEAM_LIG,					// VisualCurve	3
	EM800_BEAM_LIG_INITIAL,					// VisualCurve	3
	EM800_BEAM_LIG_LOOP,					// VisualCurve	3
	EM800_BEAM_LIG_INITIAL_DISTORTION,			// VisualCurve	5
	EM800_BEAM_LIG_DISTORTION,		// VisualCurve	4
	EM910_WATERBEAM,
	VF_00,
	VF_KINESIS_PARTICLE,
	VF_KINESIS_FLOAT_PARTICLE,//flying
	VF_KINESIS_HIT_PARTICLE,//flyhit
	AIR_KINESEIS_DIFFUSE0,
	AIR_KINESEIS_DIFFUSE1,
	AIR_KINESEIS_DISTORTION,
	NEW_KINESEIS_DIFFUSE0,
	NEW_KINESEIS_DIFFUSE1,
	NEW_KINESEIS_DISTORTION,
	DRIVE_READY,
	DRIVE_SPAWN_RING0,
	DRIVE_SPAWN_RING0_0,
	DRIVE_SPAWN_RING0_1,
	DRIVE_INITIALIZE_COG,
	DRIVE_INITIALIZE_COG_0,
	DRIVE_DEPLOY_0,
	DRIVE_DEPLOY_1,
	DRIVE_DEPLOY_2,
	DRIVE_DEPLOY_3,
	DRIVE_DEPLOY_4,
	DRIVE_DEPLOY_BLOOM,
	SAS_ACCELERATION,
	SAS_ELECTRIC,
	SAS_STEALTH,
	SAS_REPLICATION,
	SAS_COG,
	SAS_PARTICLE,
	SAS_SPLASH,
	ROCK_PARTICLE,
	KAREN_SHOCKWAVE,
	KAREN_AIR_0,
	KAREN_AIR_1,
	KAREN_AIR_2,
	KAREN_AIR_HORIZONTAL,
	KAREN_AIR_PARTICLE_0,
	KAREN_AIR_PARTICLE_1,
	KAREN_AIR_PARTICLE_2,
	KAREN_AIR_PARTICLE_3,
	KAREN_AIR_PARTICLE_4,
	KAREN_AIR_PARTICLE_5,
	KAREN_KICK1,
	KAREN_KICK2,
	KAREN_KICK3_0,
	KAREN_KICK3_1,
	KAREN_KICK3_2,
	EXECUTE_COMMON_EXP,
	EXECUTE_COMMON_LIG,
	EXECUTE_COMMON_IMP,
	EXECUTE_COMMON_DISTORTION,
	EXECUTE_COMMON_HAND_1,
	EXECUTE_COMMON_HAND_2,
	EXECUTE_COMMON_PARTICLE_1,
	EXECUTE_COMMON_PARTICLE_2,
	EXECUTE_COMMON_PARTICLE_3,
	EXECUTE_COMMON_PARTICLE_4,
	EXECUTE_COMMON_PARTICLE_REVERSE_0,
	EXECUTE_COMMON_PARTICLE_REVERSE_1,
	EXECUTE_COMMON_PARTICLE_HAND,
	EXECUTE_COMMON_SMOKE,
	EXECUTE_COMMON_SMOKE1,
	EXECUTE_COMMON_SMOKE2,
	STEALTHCHARGE_BOOM,
	STEALTHCHARGE_CHARGE,
	STEALTHCHARGE_IMP,
	STEALTHCHARGE_LIG,
	STEALTHCHARGE_DISTORTION,
	STEALTHCHARGE_PARTICLE,
	STEALTHCHARGE_PARTICLE_CHARGE,
	STEALTHCHARGE_PARTICLE_REVERSE,
	STEALTHCHARGE_CHARGING_A,
	STEALTHCHARGE_CHARGING_B,
	STEALTHCHARGE_TRAIL_DIFFUSE,
	STEALTHCHARGE_TRAIL_DISTORTION,
	ITEM_WAIT,						//Item
	ITEM_GET_0,						//Item
	ITEM_GET_1,						//Item
	ITEM_USE_0,						//Item
	ITEM_USE_1,						//Item
	ITEM_USE_2,						//Item
	ITEM_USE_3,						//Item
	ITEM_USE_4,						//Item
	ELEC_ATTACK_CIRCLE,				//First
	ELEC_ATTACK_MIDDLE,				//First
	ELEC_ATTACK_EXPLOSION,			//First
	ELEC_ATTACK_FIRST_EXPLOSION,	
	ELEC_ATTACK_EXTEND,
	ELEC_ATTACK_DISTORTION,
	ELEC_PSY_EXPLOSION,
	ELEC_PSY_EXPLOSION1,
	ELEC_PSY_MIDDLE0,
	ELEC_PSY_MIDDLE1,
	PSY_HIT_DISTORTION,
	PSY_START_DISTORTION,
	ELEC_SECOND_0,					//Second
	ELEC_SECOND_1,					//Second
	ELEC_SECOND_2,					//Second
	ELEC_SECOND_3,					//Second
	ELEC_SECOND_4,					//Second
	ELEC_SECOND_5,					//Second
	ELEC_ATTACK_SECOND_DISTORTION,	//Second
	ELEC_ATTACK_PARTICLE,			//Particle
	ELEC_PSY_PARTICLE1,				//Particle
	ELEC_PSY_PARTICLE0,				//Particle
	WATER_ATTACKED1,				//Water
	WATER_ATTACKED2,				//Water
	WATER_SHOOTING0,				//Water
	WATER_ATTACKED0,				//Water
	WATER_ATTACKED_PARTICLE,		//Water
	WATER_APPEAR0,		//Water
	KAREN_TELEPORT,					//Karen_Teleport
	KAREN_TELEPORT_DISTORTION,		//Karen_Teleport
	KAREN_TELEPORT_ATT0,			//Karen_Teleport
	KAREN_TELEPORT_ATT1,			//Karen_Teleport
	ATTACKED_BY_FLOWERCHEST,
	ATTACKED_BY_FLY0,
	ATTACKED_BY_FLY1,
	ATTACKED_BY_BUFFALO0,
	ATTACKED_BY_BUFFALO1,
	ATTACKED_BY_BUFFALO2,
	ATTACKED_BY_BUFFALO3,
	DISPENPERRY_SHOOTING0,			//DispenPerry
	DISPENPERRY_SHOOTING1,			//DispenPerry
	START_DRIVEMODE_0,				//Drive
	START_DRIVEMODE_1,				//Drive
	START_DRIVEMODE_2,				//Drive
	START_DRIVEMODE_PARTICLE,		//Drive
	START_DRIVEMODE_BRAINITER0,		//Drive
	START_DRIVEMODE_BRAINITER1,		//Drive
	START_DRIVEMODE_BRAINITER2,		//Drive
	START_DRIVEMODE_BRAINITER3,		//Drive
	ROCK_EXPLOSION,					//Special_Dynamic
	ROCK_EXPLOSION1,				//Special_Dynamic
	SPECIAL_ROCK_PARTICLE,			//Special_Dynamic
	SMOKE_0,						//Special_Dynamic
	SMOKE_1,						//Special_Dynamic
	BUS_EXPLOSION,					//Special_Dynamic
	BUS_SMOKE,						//Special_Dynamic
	BUS_EXP,
	BUS_EXP_LIG,
	BUS_EXP_DISTORTION,
	SPECIAL_BEAM0,					//Special_Dynamic
	SPECIAL_BEAM1,					//Special_Dynamic
	SPECIAL_BEAMPARTICLE,			//Special_Dynamic
	DUMP_PARTICLE,					//Special_Dynamic
	DUMP_PARTICLE1,					//Special_Dynamic
	DUMP_HIT0,						//Special_Dynamic
	DUMP_HIT1,						//Special_Dynamic
	BULLDOZER_SMOKE,				//Special_Dynamic
	BULLDOZER_HIT0,					//Special_Dynamic
	BULLDOZER_HIT1,					//Special_Dynamic
	SEALTH_PARTICLE,		//Stealth
	WATER_BEAM0,		//Stealth
	PSY_BRAINITER,
	BRAINFIELD_BRAINITER,
	DISPEN_SMOKE,				
	
	TEST,TEST1,
	MAX
};

enum class MULTIVFX
{
	EM800_BEAM,
	HIT_REGULAR0,
	HIT_REGULAR1,
	//	SAS_ACCELERATION,
	//	SAS_ELECTRIC,
	//	SAS_STEALTH,
	//	SAS_REPLICATION,
	SPAWN,
	DRIVE,
	EXECUTE_COMMON,
	STEALTHCHARGE,
	HIT_ELEC,
	HIT_SECOND_ELEC,
	HIT_THIRD_ELEC_1,
	HIT_THIRD_ELEC_2,
	HIT_FORTH_ELEC,
	HIT_ELEC_DASH,
	HIT_ELEC_BACKSTEP,
	PSY_ELEC,
	ITEM_USE,
	KAREN_TELPORT_ATT,
	START_DRIVEMODE0,
	START_DRIVEMODE1,
	START_DRIVEMODE2,
	START_DRIVEMODE_BRAINITER,
	START_DRIVEMODE_BRAINITER1,
	SPERICAL_ROCK0,
	SPERICAL_ROCK1,
	SPERICAL_BEAM,
	SPERICAL_DUMP,
	BUS,
	MAX
};

class CEffect_Manager final : public CSingleton<CEffect_Manager>
{
private:
#if ACTIVATE_TOOL
	typedef map<shared_ptr<CEffect>, shared_ptr<CObjectPool>, std::owner_less<>>	TOOLVFX;
	typedef multimap<EFFECT, wstring>::iterator										LOCATION_ITERATOR;
#endif

private:
	explicit CEffect_Manager() DEFAULT;
	virtual ~CEffect_Manager() DEFAULT;

public:
	HRESULT													Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	void													Tick(_float fTimeDelta);
	void													Late_Tick(_float fTimeDelta);

public:
	shared_ptr<CGameObject>									Fetch(VFX, any arg);
	shared_ptr<CGameObject>									Fetch(MULTIVFX, any ilArgs);
#if ACTIVATE_TOOL
	void													Fetch(shared_ptr<CEffect>, any arg, _bool bReset = false);

	pair<LOCATION_ITERATOR, LOCATION_ITERATOR>				Load_Effects(EFFECT);
#endif

private:
	HRESULT													Load_Effect(VFX, const wstring& wstrPath);
	HRESULT													Load_Pool(VFX, shared_ptr<CEffect> pPrototype);
	HRESULT													Load_Pool(MULTIVFX, shared_ptr<CEffectLayer> pPrototype);

private:
	unordered_map<VFX, shared_ptr<CEffect>>					m_umapEffectPrototypes;
	unordered_map<VFX, shared_ptr<CObjectPool>>				m_umapEffectPools;
	unordered_map<MULTIVFX, shared_ptr<CEffectLayer>>		m_umapMultiEffectPrototypes;
	unordered_map<MULTIVFX, shared_ptr<CObjectPool>>		m_umapMultiEffectPools;

#if ACTIVATE_TOOL
	TOOLVFX													m_umapToolEffectPools;
	multimap<EFFECT, wstring>								m_mmapFileLocation;
#endif

	ComPtr<ID3D11Device>									m_pDevice;
	ComPtr<ID3D11DeviceContext>								m_pContext;

	_float													m_fTimeScale = 1.f;

public:
	void													Set_TimeScale(_float f) { m_fTimeScale = f; }

	friend CSingleton<CEffect_Manager>;
};

END
