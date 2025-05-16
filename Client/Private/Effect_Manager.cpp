#include "ClientPCH.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#ifdef _DEBUG
#include "Player.h"
#include "Camera_Player.h"
#endif

#define LOAD_VFX(ENUM, PATH)	if (FAILED(Load_Effect(VFX::ENUM, TEXT(PATH))))	MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", #ENUM)

HRESULT CEffect_Manager::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	m_pDevice	= _pDevice;
	m_pContext	= _pContext;

	LOAD_VFX(KINESIS_DIFFUSE,					"Bin/Resources/Effect/Player/vfx_kinesis_diffuse.vfx");
	LOAD_VFX(KINESIS_DISTORTION,				"Bin/Resources/Effect/Player/vfx_kinesis_distortion.vfx");
	LOAD_VFX(KINESIS_PARTICLE,					"Bin/Resources/Effect/Player/vfx_kinesis_particle.vfx");
	LOAD_VFX(FADE_PARTICLE,						"Bin/Resources/Effect/Monster/vfx_mesh_fade_particle.vfx");
	LOAD_VFX(PLAYER_WEAPON_DIFFUSE,				"Bin/Resources/Effect/Player/vfx_weapon_diffuse.vfx");
	LOAD_VFX(PLAYER_WEAPON_DISTORTION,			"Bin/Resources/Effect/Player/vfx_weapon_distortion.vfx");
	LOAD_VFX(PLAYER_WEAPON_INITIALIZE,			"Bin/Resources/Effect/Player/vfx_weapon_initialize.vfx");
	LOAD_VFX(AFTERIMAGE,						"Bin/Resources/Effect/Player/vfx_afterimage.vfx");
	LOAD_VFX(PLAYER_WEAPON_DIFFUSE_ELEC,		"Bin/Resources/Effect/Player/vfx_weapon_diffuse_elec.vfx");
	LOAD_VFX(PLAYER_WEAPON_INITIALIZE_ELEC,		"Bin/Resources/Effect/Player/vfx_weapon_initialize_elec.vfx");
	LOAD_VFX(PLAYER_WEAPON_PARTICLE_ELEC0,		"Bin/Resources/Effect/Player/vfx_weapon_particle0_elec.vfx");
	LOAD_VFX(PLAYER_WEAPON_PARTICLE_ELEC1,		"Bin/Resources/Effect/Player/vfx_weapon_particle1_elec.vfx");
	LOAD_VFX(PLAYER_WEAPON_PARTICLE_ELEC2,		"Bin/Resources/Effect/Player/vfx_weapon_particle2_elec.vfx");
	LOAD_VFX(PLAYER_WEAPON_PARTICLE_ELEC3,		"Bin/Resources/Effect/Player/vfx_weapon_particle3_elec.vfx");
	LOAD_VFX(PLAYER_WEAPON_PARTICLE_ELEC4,		"Bin/Resources/Effect/Player/vfx_weapon_particle4_elec.vfx");
	LOAD_VFX(PLAYER_WEAPON_PARTICLE_ELEC5,		"Bin/Resources/Effect/Player/vfx_weapon_particle5_elec.vfx");
	LOAD_VFX(PLAYER_WEAPON_PARTICLE0,			"Bin/Resources/Effect/Player/vfx_weapon_particle0.vfx");
	LOAD_VFX(PLAYER_SYM,						"Bin/Resources/Effect/Player/vfx_sym.vfx");
//	LOAD_VFX(PLAYER_EXECUTE_HAND_0,				"Bin/Resources/Effect/Execute/vfx_exc_hand_00.vfx");
//	LOAD_VFX(PLAYER_EXECUTE_HAND_1,				"Bin/Resources/Effect/Execute/vfx_exc_hand_01.vfx");
	LOAD_VFX(PLAYER_EXECUTE_PARTICLE_HAND,		"Bin/Resources/Effect/Execute/vfx_exc_particle_01.vfx");
	LOAD_VFX(PLAYER_EXECUTE_PARTICLE_R,			"Bin/Resources/Effect/Execute/vfx_exc_particle_r_00.vfx");
	LOAD_VFX(PLAYER_EXECUTE_PARTICLE,			"Bin/Resources/Effect/Execute/vfx_exc_particle_02.vfx");
	LOAD_VFX(PLAYER_EXECUTE_PARTICLE_EXP,		"Bin/Resources/Effect/Execute/vfx_exc_particle_03.vfx");
//	LOAD_VFX(PLAYER_EXECUTE_DISTORTION,			"Bin/Resources/Effect/Execute/vfx_exc_distortion_b(3).vfx");
//	LOAD_VFX(PLAYER_EXECUTE_LIG,				"Bin/Resources/Effect/Execute/vfx_exc_lig.vfx");
//	LOAD_VFX(PLAYER_EXECUTE_LIG_EXP,			"Bin/Resources/Effect/Execute/vfx_exc_lig_exp.vfx");
	LOAD_VFX(PLAYER_EXECUTE_SMK_0,				"Bin/Resources/Effect/Common/vfx_exc_smk_00.vfx");//
	LOAD_VFX(PLAYER_EXECUTE_SMK_1,				"Bin/Resources/Effect/Common/vfx_exc_smk_01.vfx");//
	LOAD_VFX(PLAYER_EXECUTE_SMK_2,				"Bin/Resources/Effect/Common/vfx_exc_smk_02.vfx");//
	LOAD_VFX(VF_KINESIS_PARTICLE,				"Bin/Resources/Effect/VectorField/vf_00_original.vfx");
	LOAD_VFX(VF_KINESIS_FLOAT_PARTICLE,			"Bin/Resources/Effect/VectorField/vf_00_w_kinesis_float.vfx");
	LOAD_VFX(VF_KINESIS_HIT_PARTICLE,			"Bin/Resources/Effect/VectorField/vf_00_original.vfx");
	LOAD_VFX(AIR_KINESEIS_DIFFUSE0,				"Bin/Resources/Effect/Player/vfx_air_kinesis_diffuse0.vfx");
	LOAD_VFX(AIR_KINESEIS_DIFFUSE1,				"Bin/Resources/Effect/Player/vfx_air_kinesis_diffuse1.vfx");
	LOAD_VFX(AIR_KINESEIS_DISTORTION,			"Bin/Resources/Effect/Player/vfx_air_kinesis_distortion.vfx");
	LOAD_VFX(NEW_KINESEIS_DIFFUSE0,				"Bin/Resources/Effect/Player/vfx_new_kinesis_diffuse0.vfx");
	LOAD_VFX(NEW_KINESEIS_DIFFUSE1,				"Bin/Resources/Effect/Player/vfx_new_kinesis_diffuse1.vfx");
	LOAD_VFX(NEW_KINESEIS_DISTORTION,			"Bin/Resources/Effect/Player/vfx_new_kinesis_distortion.vfx");
	LOAD_VFX(SAS_ACCELERATION,					"Bin/Resources/Effect/SAS/vfx_sas_1.vfx");
	LOAD_VFX(SAS_ELECTRIC,						"Bin/Resources/Effect/SAS/vfx_sas_2.vfx");
	LOAD_VFX(SAS_STEALTH,						"Bin/Resources/Effect/SAS/vfx_sas_3.vfx");
	LOAD_VFX(SAS_REPLICATION,					"Bin/Resources/Effect/SAS/vfx_sas_4.vfx");
	LOAD_VFX(SAS_COG,							"Bin/Resources/Effect/SAS/vfx_sas_cog.vfx");
	LOAD_VFX(SAS_PARTICLE,						"Bin/Resources/Effect/SAS/vfx_sas_particle.vfx");
	LOAD_VFX(SAS_SPLASH,						"Bin/Resources/Effect/SAS/vfx_mesh_splash.vfx");
//==============================================================
	LOAD_VFX(SPAWN_DIFFUSE,						"Bin/Resources/Effect/Monster/vfx_spawn_diffuse.vfx");
	LOAD_VFX(SPAWN_DISTORTION,					"Bin/Resources/Effect/Monster/vfx_spawn_distortion.vfx");
//==============================================================
	LOAD_VFX(HIT_FLUIDMESH,						"Bin/Resources/Effect/Hit/vfx_fluid_mesh.vfx");
	LOAD_VFX(HIT_REGULAR_EXP,					"Bin/Resources/Effect/Hit/vfx_hit_exp.vfx");
	LOAD_VFX(HIT_REGULAR_IMP,					"Bin/Resources/Effect/Hit/vfx_hit_imp.vfx");
	LOAD_VFX(HIT_REGULAR_LIG,					"Bin/Resources/Effect/Hit/vfx_hit_lig.vfx");
	LOAD_VFX(HIT_REGULAR_PARTICLE,				"Bin/Resources/Effect/Hit/vfx_hit_particle.vfx");
//==============================================================
	LOAD_VFX(EM800_BEAM_DIFFUSE1,				"Bin/Resources/Effect/Monster/vfx_em800_beam_diffuse1.vfx");
	LOAD_VFX(EM800_BEAM_DIFFUSE2,				"Bin/Resources/Effect/Monster/vfx_em800_beam_diffuse2.vfx");
	LOAD_VFX(EM800_BEAM_DISTORTION,				"Bin/Resources/Effect/Monster/vfx_em800_beam_distortion.vfx");
	LOAD_VFX(EM800_BEAM_LIG,					"Bin/Resources/Effect/Monster/vfx_em800_beam_lig.vfx");
	LOAD_VFX(EM800_BEAM_LIG_INITIAL,			"Bin/Resources/Effect/Monster/vfx_em800_beam_lig_initial.vfx");
	LOAD_VFX(EM800_BEAM_LIG_LOOP,				"Bin/Resources/Effect/Monster/vfx_em800_beam_lig_loop.vfx");
	LOAD_VFX(EM800_BEAM_LIG_INITIAL_DISTORTION,	"Bin/Resources/Effect/Monster/vfx_em800_beam_lig_initial_distortion.vfx");
	LOAD_VFX(EM800_BEAM_LIG_DISTORTION,			"Bin/Resources/Effect/Monster/vfx_em800_beam_lig_distortion.vfx");
	LOAD_VFX(EM910_WATERBEAM,					"Bin/Resources/Effect/Monster/vfx_em910_beam.vfx");
//==============================================================
	LOAD_VFX(DRIVE_READY,						"Bin/Resources/Effect/Drive/vfx_drive_ready_ring.vfx");
	LOAD_VFX(DRIVE_SPAWN_RING0,					"Bin/Resources/Effect/Drive/vfx_drive_spawn_ring0.vfx");
	LOAD_VFX(DRIVE_SPAWN_RING0_0,				"Bin/Resources/Effect/Drive/vfx_drive_spawn_ring0_0.vfx");
	LOAD_VFX(DRIVE_SPAWN_RING0_1,				"Bin/Resources/Effect/Drive/vfx_drive_spawn_ring0_1.vfx");
	LOAD_VFX(DRIVE_INITIALIZE_COG,				"Bin/Resources/Effect/Drive/vfx_drive_initialize_cog.vfx");
	LOAD_VFX(DRIVE_INITIALIZE_COG_0,			"Bin/Resources/Effect/Drive/vfx_drive_initialize_cog_ignite.vfx");
	LOAD_VFX(DRIVE_DEPLOY_BLOOM,				"Bin/Resources/Effect/Drive/vfx_drive_deploy_bloom.vfx");
	LOAD_VFX(DRIVE_DEPLOY_0,					"Bin/Resources/Effect/Drive/vfx_drive_deploy_ring_0.vfx");
	LOAD_VFX(DRIVE_DEPLOY_1,					"Bin/Resources/Effect/Drive/vfx_drive_deploy_ring_1.vfx");
	LOAD_VFX(DRIVE_DEPLOY_2,					"Bin/Resources/Effect/Drive/vfx_drive_deploy_ring_2.vfx");
	LOAD_VFX(DRIVE_DEPLOY_3,					"Bin/Resources/Effect/Drive/vfx_drive_deploy_ring_3.vfx");
	LOAD_VFX(DRIVE_DEPLOY_4,					"Bin/Resources/Effect/Drive/vfx_drive_deploy_ring_4.vfx");
//==============================================================
	LOAD_VFX(KAREN_SHOCKWAVE,					"Bin/Resources/Effect/Karen/vfx_shockwave.vfx");
	LOAD_VFX(KAREN_AIR_0,						"Bin/Resources/Effect/Karen/vfx_air_0.vfx");
	LOAD_VFX(KAREN_AIR_1,						"Bin/Resources/Effect/Karen/vfx_air_1.vfx");
	LOAD_VFX(KAREN_AIR_2,						"Bin/Resources/Effect/Karen/vfx_air_2.vfx");
	LOAD_VFX(KAREN_AIR_HORIZONTAL,				"Bin/Resources/Effect/Karen/vfx_air_horizontal.vfx");
	LOAD_VFX(KAREN_AIR_PARTICLE_0,				"Bin/Resources/Effect/Karen/vfx_air_particle_0.vfx");
	LOAD_VFX(KAREN_AIR_PARTICLE_1,				"Bin/Resources/Effect/Karen/vfx_air_particle_1.vfx");
	LOAD_VFX(KAREN_AIR_PARTICLE_2,				"Bin/Resources/Effect/Karen/vfx_air_particle_2.vfx");
	LOAD_VFX(KAREN_AIR_PARTICLE_3,				"Bin/Resources/Effect/Karen/vfx_air_particle_3.vfx");
	LOAD_VFX(KAREN_AIR_PARTICLE_4,				"Bin/Resources/Effect/Karen/vfx_air_particle_4.vfx");
	LOAD_VFX(KAREN_AIR_PARTICLE_5,				"Bin/Resources/Effect/Karen/vfx_air_particle_5.vfx");
	LOAD_VFX(KAREN_KICK1,						"Bin/Resources/Effect/Karen/vfx_kick1_0.vfx");
	LOAD_VFX(KAREN_KICK2,						"Bin/Resources/Effect/Karen/vfx_kick2_0.vfx");
	LOAD_VFX(KAREN_KICK3_0,						"Bin/Resources/Effect/Karen/vfx_kick3_0.vfx");
	LOAD_VFX(KAREN_KICK3_1,						"Bin/Resources/Effect/Karen/vfx_kick3_1.vfx");
	LOAD_VFX(KAREN_KICK3_2,						"Bin/Resources/Effect/Karen/vfx_kick3_2.vfx");
	LOAD_VFX(ROCK_PARTICLE,						"Bin/Resources/Effect/Karen/vfx_rock_particle.vfx");
//==============================================================
	LOAD_VFX(EXECUTE_COMMON_EXP,				"Bin/Resources/Effect/Execute/vfx_exc_lig_exp.vfx");
	LOAD_VFX(EXECUTE_COMMON_LIG,				"Bin/Resources/Effect/Execute/vfx_exc_lig.vfx");
	LOAD_VFX(EXECUTE_COMMON_DISTORTION,			"Bin/Resources/Effect/Execute/vfx_exc_distortion_b(3).vfx");
	LOAD_VFX(EXECUTE_COMMON_HAND_1,				"Bin/Resources/Effect/Execute/vfx_exc_hand_00.vfx");
	LOAD_VFX(EXECUTE_COMMON_HAND_2,				"Bin/Resources/Effect/Execute/vfx_exc_hand_01.vfx");
	LOAD_VFX(EXECUTE_COMMON_PARTICLE_1,			"Bin/Resources/Effect/Execute/vfx_exc_particle_01.vfx");
	LOAD_VFX(EXECUTE_COMMON_PARTICLE_2,			"Bin/Resources/Effect/Execute/vfx_exc_particle_02.vfx");
	LOAD_VFX(EXECUTE_COMMON_PARTICLE_3,			"Bin/Resources/Effect/Execute/vfx_exc_particle_03.vfx");
	LOAD_VFX(EXECUTE_COMMON_PARTICLE_4,			"Bin/Resources/Effect/Execute/vfx_exc_particle_04.vfx");
	LOAD_VFX(EXECUTE_COMMON_PARTICLE_REVERSE_0,	"Bin/Resources/Effect/Execute/vfx_exc_particle_r_00.vfx");
	LOAD_VFX(EXECUTE_COMMON_PARTICLE_REVERSE_1,	"Bin/Resources/Effect/Execute/vfx_exc_particle_r_01.vfx");
	LOAD_VFX(EXECUTE_COMMON_PARTICLE_HAND,		"Bin/Resources/Effect/Execute/vfx_exc_kinesis_hand.vfx");
	LOAD_VFX(EXECUTE_COMMON_SMOKE,				"Bin/Resources/Effect/Execute/vfx_exc_smk.vfx");
	LOAD_VFX(EXECUTE_COMMON_SMOKE1,				"Bin/Resources/Effect/Execute/vfx_exc_smk_01.vfx");
	LOAD_VFX(EXECUTE_COMMON_SMOKE2,				"Bin/Resources/Effect/Execute/vfx_exc_smk_02.vfx");
//==============================================================
	LOAD_VFX(STEALTHCHARGE_BOOM,				"Bin/Resources/Effect/StealthCharge/vfx_boom.vfx");
	LOAD_VFX(STEALTHCHARGE_CHARGE,				"Bin/Resources/Effect/StealthCharge/vfx_charge.vfx");
	LOAD_VFX(STEALTHCHARGE_IMP,					"Bin/Resources/Effect/StealthCharge/vfx_imp.vfx");
	LOAD_VFX(STEALTHCHARGE_LIG,					"Bin/Resources/Effect/StealthCharge/vfx_lig.vfx");
	LOAD_VFX(STEALTHCHARGE_DISTORTION,			"Bin/Resources/Effect/StealthCharge/vfx_distortion.vfx");
	LOAD_VFX(STEALTHCHARGE_PARTICLE,			"Bin/Resources/Effect/StealthCharge/vfx_particle.vfx");
	LOAD_VFX(STEALTHCHARGE_PARTICLE_CHARGE,		"Bin/Resources/Effect/StealthCharge/vfx_particle_charge.vfx");
	LOAD_VFX(STEALTHCHARGE_PARTICLE_REVERSE,	"Bin/Resources/Effect/StealthCharge/vfx_r.vfx");
	LOAD_VFX(STEALTHCHARGE_CHARGING_A,			"Bin/Resources/Effect/StealthCharge/vfx_rot_a.vfx");
	LOAD_VFX(STEALTHCHARGE_CHARGING_B,			"Bin/Resources/Effect/StealthCharge/vfx_rot_b.vfx");
	LOAD_VFX(STEALTHCHARGE_TRAIL_DIFFUSE,		"Bin/Resources/Effect/StealthCharge/vfx_trail_diffuse.vfx");
	LOAD_VFX(STEALTHCHARGE_TRAIL_DISTORTION,	"Bin/Resources/Effect/StealthCharge/vfx_trail_distortion.vfx");
//============================================================== Bus
	LOAD_VFX(BUS_EXP,							"Bin/Resources/Effect/Bus/vfx_bus_exp.vfx");//
	LOAD_VFX(BUS_EXP_LIG,						"Bin/Resources/Effect/Bus/vfx_bus_exp_lig.vfx");
	LOAD_VFX(BUS_EXP_DISTORTION,				"Bin/Resources/Effect/Bus/vfx_bus_exp_distortion.vfx");
//============================================================== Item.................................................................................................................................
	LOAD_VFX(ITEM_WAIT,							"Bin/Resources/Effect/Item/Item_Wait.vfx");//
	LOAD_VFX(ITEM_GET_0,						"Bin/Resources/Effect/Item/Item_Get_0.vfx");//
	LOAD_VFX(ITEM_GET_1,						"Bin/Resources/Effect/Item/Item_Get_1.vfx");
	LOAD_VFX(ITEM_USE_0,						"Bin/Resources/Effect/Item/Item_Use0.vfx");
	LOAD_VFX(ITEM_USE_1,						"Bin/Resources/Effect/Item/Item_Use1.vfx");//
	LOAD_VFX(ITEM_USE_2,						"Bin/Resources/Effect/Item/Item_Use2.vfx");//
	LOAD_VFX(ITEM_USE_3,						"Bin/Resources/Effect/Item/Item_Use3.vfx");//
	LOAD_VFX(ITEM_USE_4,						"Bin/Resources/Effect/Item/Item_Use4.vfx");//
//============================================================== Elec_Attack
	LOAD_VFX(ELEC_ATTACK_CIRCLE,				"Bin/Resources/Effect/Elec/Elec_Attack_Circle.vfx");
	LOAD_VFX(ELEC_ATTACK_MIDDLE,				"Bin/Resources/Effect/Elec/Elec_Attack_Middle.vfx");
	LOAD_VFX(ELEC_ATTACK_EXPLOSION,				"Bin/Resources/Effect/Elec/Elec_Attack_Explosion.vfx");
	LOAD_VFX(ELEC_ATTACK_EXTEND,				"Bin/Resources/Effect/Elec/Elec_Attack_Extend.vfx");
	LOAD_VFX(ELEC_ATTACK_FIRST_EXPLOSION,		"Bin/Resources/Effect/Elec/Elec_Attack_First_Explosion.vfx");
	LOAD_VFX(ELEC_ATTACK_DISTORTION,			"Bin/Resources/Effect/Distortion/Circle_Distortion.vfx");
	LOAD_VFX(ELEC_SECOND_0,						"Bin/Resources/Effect/Elec/Elec_Second_0.vfx");
	LOAD_VFX(ELEC_SECOND_1,						"Bin/Resources/Effect/Elec/Elec_Second_1.vfx");
	LOAD_VFX(ELEC_SECOND_2,						"Bin/Resources/Effect/Elec/Elec_Second_2.vfx");//
	LOAD_VFX(ELEC_SECOND_3,						"Bin/Resources/Effect/Elec/Elec_Second_3.vfx");//
	LOAD_VFX(ELEC_SECOND_4,						"Bin/Resources/Effect/Elec/Elec_Second_4.vfx");//
	LOAD_VFX(ELEC_SECOND_5,						"Bin/Resources/Effect/Elec/Elec_Second_5.vfx");
	LOAD_VFX(ELEC_ATTACK_SECOND_DISTORTION,		"Bin/Resources/Effect/Distortion/Circle_Distortion_MeshTest.vfx");
	LOAD_VFX(ELEC_ATTACK_PARTICLE,				"Bin/Resources/Effect/Elec/Elec_Attack_Particle.vfx");
//============================================================== Stealth
	LOAD_VFX(SEALTH_PARTICLE,					"Bin/Resources/Effect/Stealth/Stealth_Particle.vfx");
//============================================================== Psy
	LOAD_VFX(PSY_HIT_DISTORTION,				"Bin/Resources/Effect/Distortion/Psy_Hit_Distortion.vfx");
	LOAD_VFX(PSY_START_DISTORTION,				"Bin/Resources/Effect/Distortion/Psy_Start_Distortion.vfx");
	LOAD_VFX(ELEC_PSY_EXPLOSION,				"Bin/Resources/Effect/Elec/Elec_Psy_Explosion.vfx");//
	LOAD_VFX(ELEC_PSY_EXPLOSION1,				"Bin/Resources/Effect/Elec/Elec_Psy_Explosion1.vfx");//
	LOAD_VFX(ELEC_PSY_MIDDLE0,					"Bin/Resources/Effect/Elec/Elec_Psy_Middle0.vfx");
	LOAD_VFX(ELEC_PSY_MIDDLE1,					"Bin/Resources/Effect/Elec/Elec_Psy_Middle1.vfx");
	LOAD_VFX(ELEC_PSY_PARTICLE0,				"Bin/Resources/Effect/Elec/Elec_Psy_Particle0.vfx");
	LOAD_VFX(ELEC_PSY_PARTICLE1,				"Bin/Resources/Effect/Elec/Elec_Psy_Particle1.vfx");
	LOAD_VFX(PSY_BRAINITER,						"Bin/Resources/Effect/Player/Psy_Brainiter.vfx");
	LOAD_VFX(BRAINFIELD_BRAINITER,				"Bin/Resources/Effect/Player/BrainField_Brainiter.vfx");
//============================================================== Monster
	LOAD_VFX(WATER_SHOOTING0,					"Bin/Resources/Effect/Water/Water_Shooting0.vfx");//
	LOAD_VFX(WATER_ATTACKED0,					"Bin/Resources/Effect/Water/Water_Attacked0.vfx");//
	LOAD_VFX(WATER_ATTACKED1,					"Bin/Resources/Effect/Water/Water_Attacked1.vfx");//
	LOAD_VFX(WATER_ATTACKED2,					"Bin/Resources/Effect/Water/Water_Attacked2.vfx");//
	LOAD_VFX(WATER_ATTACKED_PARTICLE,			"Bin/Resources/Effect/Water/Water_Attacked_Particle.vfx");
	LOAD_VFX(WATER_APPEAR0,						"Bin/Resources/Effect/Water/Water_Appear0.vfx");
	LOAD_VFX(ATTACKED_BY_FLOWERCHEST,			"Bin/Resources/Effect/Player/Attacked_By_FlowerChest.vfx");
	LOAD_VFX(ATTACKED_BY_FLY0,					"Bin/Resources/Effect/Player/Attacked_By_Fly0.vfx");
	LOAD_VFX(ATTACKED_BY_FLY1,					"Bin/Resources/Effect/Player/Attacked_By_Fly1.vfx");//
	LOAD_VFX(ATTACKED_BY_BUFFALO0,				"Bin/Resources/Effect/Player/Attacked_By_Buffalo0.vfx");
	LOAD_VFX(ATTACKED_BY_BUFFALO1,				"Bin/Resources/Effect/Player/Attacked_By_Buffalo1.vfx");//
	LOAD_VFX(ATTACKED_BY_BUFFALO2,				"Bin/Resources/Effect/Player/Attacked_By_Buffalo2.vfx");
	LOAD_VFX(ATTACKED_BY_BUFFALO3,				"Bin/Resources/Effect/Player/Attacked_By_Buffalo3.vfx");//
	LOAD_VFX(WATER_BEAM0,						"Bin/Resources/Effect/Monster/Water_Beam0.vfx");
//============================================================== Karen
	LOAD_VFX(KAREN_TELEPORT,					"Bin/Resources/Effect/Monster/Karen_Teleport.vfx");
	LOAD_VFX(KAREN_TELEPORT_DISTORTION,			"Bin/Resources/Effect/Monster/Karen_Teleport_Distortion.vfx");
	LOAD_VFX(KAREN_TELEPORT_ATT0,				"Bin/Resources/Effect/Monster/Karen_Teleport_Att0.vfx");
	LOAD_VFX(KAREN_TELEPORT_ATT1,				"Bin/Resources/Effect/Monster/Karen_Teleport_Att1.vfx");

//============================================================== DISPENPERRY
	LOAD_VFX(DISPENPERRY_SHOOTING0,				"Bin/Resources/Effect/Water/DispenPerry_Shooting0.vfx");//
	LOAD_VFX(DISPENPERRY_SHOOTING1,				"Bin/Resources/Effect/Water/DispenPerry_Shooting1.vfx");//
	LOAD_VFX(DISPEN_SMOKE,						"Bin/Resources/Effect/Monster/Dispen_Smoke.vfx");

//============================================================== Drive

	LOAD_VFX(START_DRIVEMODE_0,					"Bin/Resources/Effect/Drive/Start_DriveMode_0.vfx");
	LOAD_VFX(START_DRIVEMODE_1,					"Bin/Resources/Effect/Drive/Start_DriveMode_1.vfx");
	LOAD_VFX(START_DRIVEMODE_2,					"Bin/Resources/Effect/Drive/Start_DriveMode_2.vfx");
	LOAD_VFX(START_DRIVEMODE_PARTICLE,			"Bin/Resources/Effect/Drive/Start_DriveMode_Particle.vfx");
	LOAD_VFX(START_DRIVEMODE_BRAINITER0,		"Bin/Resources/Effect/Drive/Start_DriveMode_Brainiter0.vfx");
	LOAD_VFX(START_DRIVEMODE_BRAINITER1,		"Bin/Resources/Effect/Drive/Start_DriveMode_Brainiter1.vfx");
	LOAD_VFX(START_DRIVEMODE_BRAINITER2,		"Bin/Resources/Effect/Drive/Start_DriveMode_Brainiter2.vfx");
	LOAD_VFX(START_DRIVEMODE_BRAINITER3,		"Bin/Resources/Effect/Drive/Start_DriveMode_Brainiter3.vfx");

//============================================================== Special_Dynamic
	LOAD_VFX(ROCK_EXPLOSION,					"Bin/Resources/Effect/Special_Dynamic/Rock_Explosion.vfx");
	LOAD_VFX(ROCK_EXPLOSION1,					"Bin/Resources/Effect/Special_Dynamic/Rock_Explosion1.vfx");//
	LOAD_VFX(SPECIAL_ROCK_PARTICLE,				"Bin/Resources/Effect/Special_Dynamic/Rock_Particle.vfx");
	LOAD_VFX(SMOKE_0,							"Bin/Resources/Effect/Special_Dynamic/Smoke_0.vfx");
	LOAD_VFX(SMOKE_1,							"Bin/Resources/Effect/Special_Dynamic/Smoke_1.vfx");
	LOAD_VFX(BUS_EXPLOSION,						"Bin/Resources/Effect/Special_Dynamic/Bus_Explosion.vfx");//
	LOAD_VFX(BUS_SMOKE,							"Bin/Resources/Effect/Special_Dynamic/Bus_Smoke.vfx");//
	LOAD_VFX(SPECIAL_BEAM0,						"Bin/Resources/Effect/Special_Dynamic/Special_Beam0.vfx");
	LOAD_VFX(SPECIAL_BEAM1,						"Bin/Resources/Effect/Special_Dynamic/Special_Beam1.vfx");//
	LOAD_VFX(SPECIAL_BEAMPARTICLE,				"Bin/Resources/Effect/Special_Dynamic/Special_BeamParticle.vfx");
	LOAD_VFX(DUMP_PARTICLE,						"Bin/Resources/Effect/Special_Dynamic/Dump_Particle.vfx");
	LOAD_VFX(DUMP_PARTICLE1,					"Bin/Resources/Effect/Special_Dynamic/Dump_Particle1.vfx");
	LOAD_VFX(DUMP_HIT1,							"Bin/Resources/Effect/Special_Dynamic/Dump_Hit1.vfx");//
	LOAD_VFX(DUMP_HIT0,							"Bin/Resources/Effect/Special_Dynamic/Dump_Hit0.vfx");
	LOAD_VFX(BULLDOZER_SMOKE,					"Bin/Resources/Effect/Special_Dynamic/Bulldozer_Smoke.vfx");//
	LOAD_VFX(BULLDOZER_HIT0,					"Bin/Resources/Effect/Special_Dynamic/Bulldozer_Hit0.vfx");//
	LOAD_VFX(BULLDOZER_HIT1,					"Bin/Resources/Effect/Special_Dynamic/Bulldozer_Hit1.vfx");//
//==============================================================
//==LOAD_VFX(TEST,								"Bin/Resources/Effect/test.vfx");
//==LOAD_VFX(TEST1,								"Bin/Resources/Effect/test1.vfx");
	
	for (auto& pair : m_umapEffectPrototypes)
	{
		if (FAILED(Load_Pool(pair.first, pair.second)))
		{
			MSG_CONTINUE("CEffect_Manager::Initialize", "Failed to Load_Pool");
		}
	}

	m_umapEffectPrototypes.clear();

	shared_ptr<CEffectLayer> pVFXLayer;

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::EM800_BEAM);
	pVFXLayer->Append_Effect(0.f, VFX::EM800_BEAM_LIG_INITIAL);
//	pVFXLayer->Append_Effect(0.5f, VFX::EM800_BEAM_LIG_INITIAL_DISTORTION);
	pVFXLayer->Append_Effect(0.5f, VFX::EM800_BEAM_LIG);
//	pVFXLayer->Append_Effect(0.5f, VFX::EM800_BEAM_LIG_DISTORTION);
	pVFXLayer->Append_Effect(0.5f, VFX::EM800_BEAM_DIFFUSE1);
	pVFXLayer->Append_Effect(0.5f, VFX::EM800_BEAM_DIFFUSE2);
	pVFXLayer->Append_Effect(0.5f, VFX::EM800_BEAM_DISTORTION);
	pVFXLayer->Append_Effect(0.5f, VFX::EM800_BEAM_LIG_LOOP);
	pVFXLayer->Append_Effect(0.9f, VFX::EM800_BEAM_LIG_LOOP);
	pVFXLayer->Append_Effect(1.3f, VFX::EM800_BEAM_LIG_LOOP);
	pVFXLayer->Append_Effect(1.7f, VFX::EM800_BEAM_LIG_LOOP);
	pVFXLayer->Append_Effect(2.1f, VFX::EM800_BEAM_LIG_LOOP);
	pVFXLayer->Append_Effect(2.5f, VFX::EM800_BEAM_LIG_LOOP);
	pVFXLayer->Append_Effect(2.9f, VFX::EM800_BEAM_LIG_LOOP);
	if (FAILED(Load_Pool(MULTIVFX::EM800_BEAM, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::HIT_REGULAR0);
	pVFXLayer->Append_Effect(0.f, VFX::HIT_FLUIDMESH);
	pVFXLayer->Append_Effect(0.f, VFX::HIT_REGULAR_EXP);
	if (FAILED(Load_Pool(MULTIVFX::HIT_REGULAR0, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::HIT_REGULAR1);
	pVFXLayer->Append_Effect(0.f, VFX::HIT_FLUIDMESH);
	pVFXLayer->Append_Effect(0.f, VFX::HIT_REGULAR_LIG);
	pVFXLayer->Append_Effect(0.f, VFX::HIT_REGULAR_PARTICLE);
	pVFXLayer->Append_Effect(0.1f, VFX::HIT_REGULAR_EXP);
	pVFXLayer->Append_Effect(0.1f, VFX::HIT_REGULAR_IMP);
	if (FAILED(Load_Pool(MULTIVFX::HIT_REGULAR1, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::SPAWN);
	pVFXLayer->Append_Effect(0.f, VFX::SPAWN_DIFFUSE);
	pVFXLayer->Append_Effect(0.f, VFX::SPAWN_DISTORTION);
	if (FAILED(Load_Pool(MULTIVFX::SPAWN, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::DRIVE);
	pVFXLayer->Append_Effect(0.f, VFX::DRIVE_READY);
	pVFXLayer->Append_Effect(0.05f, VFX::DRIVE_SPAWN_RING0);
	pVFXLayer->Append_Effect(0.05f, VFX::DRIVE_SPAWN_RING0_0);
	pVFXLayer->Append_Effect(0.05f, VFX::DRIVE_SPAWN_RING0_1);
	pVFXLayer->Append_Effect(0.05f, VFX::DRIVE_INITIALIZE_COG);
	pVFXLayer->Append_Effect(0.05f, VFX::DRIVE_INITIALIZE_COG_0);
	pVFXLayer->Append_Effect(0.95f, VFX::DRIVE_DEPLOY_BLOOM);
	pVFXLayer->Append_Effect(0.95f, VFX::DRIVE_DEPLOY_0);
	pVFXLayer->Append_Effect(0.95f, VFX::DRIVE_DEPLOY_1);
	pVFXLayer->Append_Effect(0.95f, VFX::DRIVE_DEPLOY_2);
	pVFXLayer->Append_Effect(0.95f, VFX::DRIVE_DEPLOY_3);
	pVFXLayer->Append_Effect(0.95f, VFX::DRIVE_DEPLOY_4);
	if (FAILED(Load_Pool(MULTIVFX::DRIVE, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::EXECUTE_COMMON);
	pVFXLayer->Append_Effect(0.00f, VFX::EXECUTE_COMMON_PARTICLE_REVERSE_1);
	pVFXLayer->Append_Effect(0.00f, VFX::EXECUTE_COMMON_PARTICLE_HAND);
	pVFXLayer->Append_Effect(0.65f, VFX::EXECUTE_COMMON_HAND_1);
	pVFXLayer->Append_Effect(0.65f, VFX::EXECUTE_COMMON_HAND_2);
	pVFXLayer->Append_Effect(0.65f, VFX::EXECUTE_COMMON_PARTICLE_REVERSE_0);
	pVFXLayer->Append_Effect(0.65f, VFX::EXECUTE_COMMON_LIG);
	pVFXLayer->Append_Effect(0.65f, VFX::NEW_KINESEIS_DIFFUSE0);
	pVFXLayer->Append_Effect(0.65f, VFX::NEW_KINESEIS_DIFFUSE1);
	pVFXLayer->Append_Effect(0.65f, VFX::NEW_KINESEIS_DISTORTION);
	pVFXLayer->Append_Effect(0.85f, VFX::EXECUTE_COMMON_DISTORTION);
	pVFXLayer->Append_Effect(1.55f, VFX::EXECUTE_COMMON_PARTICLE_HAND);
	pVFXLayer->Append_Effect(1.65f, VFX::EXECUTE_COMMON_PARTICLE_1);
	pVFXLayer->Append_Effect(1.65f, VFX::EXECUTE_COMMON_PARTICLE_2);
	pVFXLayer->Append_Effect(1.65f, VFX::EXECUTE_COMMON_PARTICLE_3);
	pVFXLayer->Append_Effect(1.65f, VFX::EXECUTE_COMMON_PARTICLE_4);
	pVFXLayer->Append_Effect(1.65f, VFX::EXECUTE_COMMON_EXP);
	pVFXLayer->Append_Effect(1.65f, VFX::EXECUTE_COMMON_SMOKE);
	pVFXLayer->Append_Effect(1.65f, VFX::EXECUTE_COMMON_SMOKE1);
	pVFXLayer->Append_Effect(1.65f, VFX::EXECUTE_COMMON_SMOKE2);
	if (FAILED(Load_Pool(MULTIVFX::EXECUTE_COMMON, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::STEALTHCHARGE);
//	pVFXLayer->Append_Effect(0.65f, VFX::STEALTHCHARGE_PARTICLE_REVERSE);
//	pVFXLayer->Append_Effect(0.65f, VFX::STEALTHCHARGE_CHARGING_A);
//	pVFXLayer->Append_Effect(2.25f, VFX::STEALTHCHARGE_LIG);
//	pVFXLayer->Append_Effect(2.25f, VFX::STEALTHCHARGE_DISTORTION);
//	pVFXLayer->Append_Effect(2.25f, VFX::STEALTHCHARGE_CHARGING_B);
//	pVFXLayer->Append_Effect(5.00f, VFX::SMOKE_0);
//	pVFXLayer->Append_Effect(5.00f, VFX::STEALTHCHARGE_BOOM);
//	pVFXLayer->Append_Effect(5.35f, VFX::STEALTHCHARGE_IMP);
//	pVFXLayer->Append_Effect(5.35f, VFX::STEALTHCHARGE_CHARGE);
//	pVFXLayer->Append_Effect(5.35f, VFX::STEALTHCHARGE_PARTICLE);
//	pVFXLayer->Append_Effect(5.35f, VFX::STEALTHCHARGE_PARTICLE_CHARGE);
	pVFXLayer->Append_Effect(0.65f, VFX::STEALTHCHARGE_PARTICLE_REVERSE);
	pVFXLayer->Append_Effect(0.65f, VFX::STEALTHCHARGE_CHARGING_A);
	pVFXLayer->Append_Effect(2.25f, VFX::STEALTHCHARGE_LIG);
	pVFXLayer->Append_Effect(2.25f, VFX::STEALTHCHARGE_DISTORTION);
	pVFXLayer->Append_Effect(2.25f, VFX::STEALTHCHARGE_CHARGING_B);
	pVFXLayer->Append_Effect(4.80f, VFX::SMOKE_0);
	pVFXLayer->Append_Effect(4.80f, VFX::STEALTHCHARGE_BOOM);
	pVFXLayer->Append_Effect(4.80f, VFX::STEALTHCHARGE_PARTICLE_CHARGE);
	pVFXLayer->Append_Effect(4.80f, VFX::STEALTHCHARGE_IMP);
	pVFXLayer->Append_Effect(5.15f, VFX::STEALTHCHARGE_CHARGE);
	pVFXLayer->Append_Effect(5.15f, VFX::STEALTHCHARGE_PARTICLE);
	if (FAILED(Load_Pool(MULTIVFX::STEALTHCHARGE, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

#pragma region Elec
	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::HIT_ELEC);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_PSY_EXPLOSION1);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_ATTACK_DISTORTION);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_ATTACK_PARTICLE);

	if (FAILED(Load_Pool(MULTIVFX::HIT_ELEC, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::PSY_ELEC);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_PSY_EXPLOSION);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_PSY_EXPLOSION1);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_PSY_PARTICLE1);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_SECOND_4);
	if (FAILED(Load_Pool(MULTIVFX::PSY_ELEC, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}
  
	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::HIT_SECOND_ELEC);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_ATTACK_SECOND_DISTORTION);
	pVFXLayer->Append_Effect(0.1f, VFX::ELEC_SECOND_0);
	pVFXLayer->Append_Effect(0.1f, VFX::ELEC_SECOND_4);
	pVFXLayer->Append_Effect(0.2f, VFX::ELEC_ATTACK_MIDDLE);

	if (FAILED(Load_Pool(MULTIVFX::HIT_SECOND_ELEC, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::HIT_THIRD_ELEC_1);
	pVFXLayer->Append_Effect(0.3f, VFX::ELEC_ATTACK_FIRST_EXPLOSION);
	pVFXLayer->Append_Effect(0.7f, VFX::ELEC_ATTACK_CIRCLE);
	pVFXLayer->Append_Effect(0.4f, VFX::ELEC_ATTACK_MIDDLE);
	pVFXLayer->Append_Effect(0.5f, VFX::ELEC_ATTACK_EXPLOSION);
	pVFXLayer->Append_Effect(0.5f, VFX::ELEC_SECOND_0);
	pVFXLayer->Append_Effect(0.5f, VFX::ELEC_ATTACK_SECOND_DISTORTION);
	pVFXLayer->Append_Effect(0.6f, VFX::ELEC_SECOND_4);
	if (FAILED(Load_Pool(MULTIVFX::HIT_THIRD_ELEC_1, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::HIT_THIRD_ELEC_2);
	pVFXLayer->Append_Effect(0.6f, VFX::ELEC_ATTACK_FIRST_EXPLOSION);
	pVFXLayer->Append_Effect(0.7f, VFX::ELEC_ATTACK_CIRCLE);
	pVFXLayer->Append_Effect(0.7f, VFX::ELEC_ATTACK_MIDDLE);
	pVFXLayer->Append_Effect(0.8f, VFX::ELEC_ATTACK_EXPLOSION);
	pVFXLayer->Append_Effect(0.8f, VFX::ELEC_ATTACK_SECOND_DISTORTION);
	pVFXLayer->Append_Effect(0.6f, VFX::ELEC_SECOND_0);
	pVFXLayer->Append_Effect(0.8f, VFX::ELEC_SECOND_4);
	if (FAILED(Load_Pool(MULTIVFX::HIT_THIRD_ELEC_2, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}
	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::HIT_ELEC_BACKSTEP);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_ATTACK_FIRST_EXPLOSION);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_ATTACK_EXTEND);
	pVFXLayer->Append_Effect(0.1f, VFX::ELEC_ATTACK_CIRCLE);
	pVFXLayer->Append_Effect(0.1f, VFX::ELEC_ATTACK_MIDDLE);
	pVFXLayer->Append_Effect(0.2f, VFX::ELEC_ATTACK_EXPLOSION);

	pVFXLayer->Append_Effect(0.2f, VFX::ELEC_ATTACK_SECOND_DISTORTION);
	pVFXLayer->Append_Effect(0.4f, VFX::ELEC_ATTACK_FIRST_EXPLOSION);
	pVFXLayer->Append_Effect(0.4f, VFX::ELEC_ATTACK_EXTEND);
	pVFXLayer->Append_Effect(0.4f, VFX::ELEC_ATTACK_SECOND_DISTORTION);
	pVFXLayer->Append_Effect(0.5f, VFX::ELEC_ATTACK_MIDDLE);

	pVFXLayer->Append_Effect(0.5f, VFX::ELEC_ATTACK_EXPLOSION);
	pVFXLayer->Append_Effect(0.5f, VFX::ELEC_SECOND_2);
	pVFXLayer->Append_Effect(0.7f, VFX::ELEC_ATTACK_MIDDLE);
	pVFXLayer->Append_Effect(0.7f, VFX::ELEC_ATTACK_EXPLOSION);
	pVFXLayer->Append_Effect(0.7f, VFX::ELEC_ATTACK_SECOND_DISTORTION);

	pVFXLayer->Append_Effect(0.9f, VFX::ELEC_ATTACK_SECOND_DISTORTION);
	pVFXLayer->Append_Effect(0.9f, VFX::ELEC_ATTACK_MIDDLE);
	pVFXLayer->Append_Effect(0.9f, VFX::ELEC_ATTACK_EXPLOSION);
	pVFXLayer->Append_Effect(0.9f, VFX::ELEC_ATTACK_CIRCLE);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_ATTACK_PARTICLE);

	pVFXLayer->Append_Effect(0.7f, VFX::ELEC_ATTACK_PARTICLE);

	if (FAILED(Load_Pool(MULTIVFX::HIT_ELEC_BACKSTEP, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}


	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::HIT_ELEC_DASH);
	pVFXLayer->Append_Effect(0.f, VFX::ELEC_ATTACK_MIDDLE);
	pVFXLayer->Append_Effect(0.1f, VFX::ELEC_ATTACK_FIRST_EXPLOSION);
	pVFXLayer->Append_Effect(0.1f, VFX::ELEC_ATTACK_EXTEND);
	pVFXLayer->Append_Effect(0.2f, VFX::ELEC_ATTACK_EXPLOSION);
	pVFXLayer->Append_Effect(0.2f, VFX::ELEC_ATTACK_MIDDLE);

	pVFXLayer->Append_Effect(0.3f, VFX::ELEC_ATTACK_FIRST_EXPLOSION);
	pVFXLayer->Append_Effect(0.3f, VFX::ELEC_ATTACK_EXTEND);
	pVFXLayer->Append_Effect(0.3f, VFX::ELEC_ATTACK_MIDDLE);
	pVFXLayer->Append_Effect(0.3f, VFX::ELEC_ATTACK_CIRCLE);
	pVFXLayer->Append_Effect(0.3f, VFX::ELEC_ATTACK_DISTORTION);

	pVFXLayer->Append_Effect(0.0f, VFX::ELEC_ATTACK_PARTICLE);

	if (FAILED(Load_Pool(MULTIVFX::HIT_ELEC_DASH, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}
	
#pragma endregion
	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::ITEM_USE);
	pVFXLayer->Append_Effect(0.5f, VFX::ITEM_USE_0);
	pVFXLayer->Append_Effect(0.6f, VFX::ITEM_USE_1);
	pVFXLayer->Append_Effect(0.8f, VFX::ITEM_USE_2);
	pVFXLayer->Append_Effect(0.8f, VFX::ITEM_USE_3);
	pVFXLayer->Append_Effect(0.8f, VFX::ITEM_USE_4);


	if (FAILED(Load_Pool(MULTIVFX::ITEM_USE, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}


	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::KAREN_TELPORT_ATT);
	pVFXLayer->Append_Effect(0.f, VFX::KAREN_TELEPORT_ATT0);
	pVFXLayer->Append_Effect(0.2f, VFX::KAREN_TELEPORT_ATT1);


	if (FAILED(Load_Pool(MULTIVFX::KAREN_TELPORT_ATT, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

#pragma region Drive
	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::START_DRIVEMODE0);
	pVFXLayer->Append_Effect(0.f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(0.2f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(0.6f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(0.7f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(1.2f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(1.3f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(1.9f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(2.f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(2.4f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(2.5f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(2.75f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(3.f, VFX::START_DRIVEMODE_0);
	pVFXLayer->Append_Effect(3.25f, VFX::START_DRIVEMODE_0);

	pVFXLayer->Append_Effect(0.75f, VFX::START_DRIVEMODE_PARTICLE);
	pVFXLayer->Append_Effect(1.75f, VFX::START_DRIVEMODE_PARTICLE);
	pVFXLayer->Append_Effect(2.f, VFX::START_DRIVEMODE_PARTICLE);
	pVFXLayer->Append_Effect(2.75f, VFX::START_DRIVEMODE_PARTICLE);

	if (FAILED(Load_Pool(MULTIVFX::START_DRIVEMODE0, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::START_DRIVEMODE1);
	pVFXLayer->Append_Effect(0.2f, VFX::START_DRIVEMODE_1);
	pVFXLayer->Append_Effect(0.7f, VFX::START_DRIVEMODE_1);
	pVFXLayer->Append_Effect(1.3f, VFX::START_DRIVEMODE_1);
	pVFXLayer->Append_Effect(2.f, VFX::START_DRIVEMODE_1);
	pVFXLayer->Append_Effect(2.5f, VFX::START_DRIVEMODE_1);
	pVFXLayer->Append_Effect(3.f, VFX::START_DRIVEMODE_1);
	pVFXLayer->Append_Effect(3.25f, VFX::START_DRIVEMODE_1);

	if (FAILED(Load_Pool(MULTIVFX::START_DRIVEMODE1, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::START_DRIVEMODE2);
	pVFXLayer->Append_Effect(2.f, VFX::START_DRIVEMODE_2);
	pVFXLayer->Append_Effect(2.5f, VFX::START_DRIVEMODE_2);
	pVFXLayer->Append_Effect(3.25f, VFX::START_DRIVEMODE_2);

	if (FAILED(Load_Pool(MULTIVFX::START_DRIVEMODE2, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::START_DRIVEMODE_BRAINITER);
	pVFXLayer->Append_Effect(1.f, VFX::START_DRIVEMODE_BRAINITER0);
	pVFXLayer->Append_Effect(1.5f, VFX::START_DRIVEMODE_BRAINITER1);
	pVFXLayer->Append_Effect(2.5f, VFX::START_DRIVEMODE_BRAINITER0);
	pVFXLayer->Append_Effect(3.f, VFX::START_DRIVEMODE_BRAINITER1);

	if (FAILED(Load_Pool(MULTIVFX::START_DRIVEMODE_BRAINITER, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::START_DRIVEMODE_BRAINITER1);
	pVFXLayer->Append_Effect(1.3f, VFX::START_DRIVEMODE_BRAINITER2);
	pVFXLayer->Append_Effect(1.3f, VFX::START_DRIVEMODE_BRAINITER3);

	if (FAILED(Load_Pool(MULTIVFX::START_DRIVEMODE_BRAINITER1, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}
	
#pragma endregion

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::SPERICAL_ROCK0);
	pVFXLayer->Append_Effect(0.f, VFX::ROCK_EXPLOSION);
	pVFXLayer->Append_Effect(0.f, VFX::SPECIAL_ROCK_PARTICLE);
	pVFXLayer->Append_Effect(0.f, VFX::BUS_SMOKE);
	pVFXLayer->Append_Effect(0.2f, VFX::BUS_SMOKE);

	if (FAILED(Load_Pool(MULTIVFX::SPERICAL_ROCK0, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::SPERICAL_ROCK1);
	pVFXLayer->Append_Effect(0.0f, VFX::ROCK_EXPLOSION);
	pVFXLayer->Append_Effect(0.0f, VFX::SPECIAL_ROCK_PARTICLE);
	pVFXLayer->Append_Effect(0.025f, VFX::ROCK_EXPLOSION);
	pVFXLayer->Append_Effect(0.05f, VFX::ROCK_EXPLOSION);
	pVFXLayer->Append_Effect(0.075f, VFX::ROCK_EXPLOSION);
	pVFXLayer->Append_Effect(0.01f, VFX::ROCK_EXPLOSION);
	pVFXLayer->Append_Effect(0.f, VFX::BUS_SMOKE);
	pVFXLayer->Append_Effect(0.1f, VFX::BUS_SMOKE);
	pVFXLayer->Append_Effect(0.1f, VFX::SPECIAL_ROCK_PARTICLE);
	pVFXLayer->Append_Effect(0.2f, VFX::BUS_SMOKE);

	if (FAILED(Load_Pool(MULTIVFX::SPERICAL_ROCK1, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::BUS);
//	pVFXLayer->Append_Effect(0.f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.1f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.2f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.3f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.4f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.5f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.6f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.7f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.8f, VFX::BUS_EXPLOSION);
//	pVFXLayer->Append_Effect(0.9f, VFX::BUS_EXPLOSION);
	pVFXLayer->Append_Effect(0.f, VFX::BUS_EXP);
	pVFXLayer->Append_Effect(0.1f, VFX::BUS_EXP_LIG);
	pVFXLayer->Append_Effect(0.5f, VFX::BUS_EXP_DISTORTION);

	if (FAILED(Load_Pool(MULTIVFX::BUS, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::SPERICAL_BEAM);
	pVFXLayer->Append_Effect(0.f, VFX::SPECIAL_BEAM0);
	pVFXLayer->Append_Effect(0.f, VFX::SPECIAL_BEAM1);
	pVFXLayer->Append_Effect(0.f, VFX::SPECIAL_BEAMPARTICLE);

	pVFXLayer->Append_Effect(1.f, VFX::SPECIAL_BEAM0);
	pVFXLayer->Append_Effect(1.f, VFX::SPECIAL_BEAM1);
	pVFXLayer->Append_Effect(1.f, VFX::SPECIAL_BEAMPARTICLE);

	pVFXLayer->Append_Effect(2.f, VFX::SPECIAL_BEAM0);
	pVFXLayer->Append_Effect(2.f, VFX::SPECIAL_BEAM1);
	pVFXLayer->Append_Effect(2.f, VFX::SPECIAL_BEAMPARTICLE);

	if (FAILED(Load_Pool(MULTIVFX::SPERICAL_BEAM, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	pVFXLayer = CEffectLayer::Create(m_pDevice, m_pContext, MULTIVFX::SPERICAL_DUMP);
	pVFXLayer->Append_Effect(0.f, VFX::DUMP_PARTICLE);
	pVFXLayer->Append_Effect(0.f, VFX::DUMP_PARTICLE1);
	pVFXLayer->Append_Effect(0.f, VFX::DUMP_HIT0);
	pVFXLayer->Append_Effect(0.f, VFX::DUMP_HIT1);

	if (FAILED(Load_Pool(MULTIVFX::SPERICAL_DUMP, pVFXLayer)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Initialize", "Failed to Load_Pool");
	}

	return S_OK;
}

void CEffect_Manager::Tick(_float _fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	for (auto pair : m_umapEffectPools)
	{
		pair.second->Tick(_fTimeDelta * pGameInstance->Get_Effect_TimeScale());
	}
	for (auto pair : m_umapMultiEffectPools)
	{
		pair.second->Tick(_fTimeDelta * pGameInstance->Get_Effect_TimeScale());
	}

#if ACTIVATE_TOOL
	for (auto pair : m_umapToolEffectPools)
	{
		pair.second->Tick(_fTimeDelta);
	}
#endif

#ifdef _DEBUG
	shared_ptr<CGameObject>	pPlayer;
	shared_ptr<CTransform>	pTransform;

	pPlayer		= Function::Find_Player();
	if (pPlayer)
	pTransform	= pPlayer->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	if (SCENE::TOOL != CGameInstance::Get_Instance()->Current_Scene() && CGameInstance::Get_Instance()->Key_Down(VK_OEM_MINUS))
	{
		//	_float4x4 mOriginal = m_pTransform->Get_Matrix();
		//	m_pTransform->Translate(XMVectorSet(0.f, 1.f, 0.f, 0.f));
		//	CEffect_Manager::Get_Instance()->Fetch(VFX::AIR_KINESEIS_DIFFUSE0, m_pTransform->Get_Matrix());
		//	CEffect_Manager::Get_Instance()->Fetch(VFX::AIR_KINESEIS_DIFFUSE1, m_pTransform->Get_Matrix());
		//	m_pTransform->Set_Matrix(mOriginal);

		//	VFXMGR->Fetch(VFX::PLAYER_SYM, make_pair(shared_from_gameobject(), "TopMask1"));

		//	VFXMGR->Fetch(VFX::PLAYER_EXECUTE_SMK_0, make_pair(shared_from_gameobject(), "TopMask1"));
		//	VFXMGR->Fetch(VFX::PLAYER_EXECUTE_SMK_1, make_pair(shared_from_gameobject(), "TopMask1"));
		//	VFXMGR->Fetch(VFX::PLAYER_EXECUTE_SMK_2, make_pair(shared_from_gameobject(), "TopMask1"));

		//	VFXMGR->Fetch(VFX::ROCK_PARTICLE, m_pTransform->Get_State(TRANSFORM::POSITION));
		//	VFXMGR->Fetch(VFX::KAREN_SHOCKWAVE, m_pTransform->Get_State(TRANSFORM::POSITION));

		//	_matrix mOriginal = m_pTransform->Get_Matrix();
		//	m_pTransform->Translate(- 1.5f * m_pTransform->Get_State(TRANSFORM::LOOK));
		//	VFXMGR->Fetch(VFX::KAREN_KICK3_0, m_pTransform->Get_Matrix());
		//	VFXMGR->Fetch(VFX::KAREN_KICK3_1, m_pTransform->Get_Matrix());
		//	VFXMGR->Fetch(VFX::KAREN_KICK3_2, m_pTransform->Get_Matrix());
		//	m_pTransform->Set_Matrix(mOriginal);

		//	_float fHeight = _float4x4(m_pModel->Get_Bone("LeftWeapon")->Get_CombinedTransformation() * m_pModel->Get_Pivot() * m_pTransform->Get_Matrix()).row(3).y - MINDROOM_TERRAIN_HEIGHT;
		//	VFXMGR->Fetch(VFX::KAREN_AIR_0,				make_tuple(shared_from_gameobject(), "Reference", fHeight));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_1,				make_tuple(shared_from_gameobject(), "Reference", fHeight));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_2,				make_tuple(shared_from_gameobject(), "Reference", fHeight));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_HORIZONTAL,	make_tuple(shared_from_gameobject(), "Reference", fHeight));
		//
		//	_float4x4 mOriginal = m_pTransform->Get_Matrix();
		//	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * 6.f);
		//	_float3 vPosition = m_pTransform->Get_State(TRANSFORM::POSITION);
		//	m_pTransform->Set_State(TRANSFORM::POSITION, _float3(vPosition.x, MINDROOM_TERRAIN_HEIGHT, vPosition.z));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_0,	m_pTransform->Get_State(TRANSFORM::POSITION));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_1,	m_pTransform->Get_State(TRANSFORM::POSITION));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_2,	m_pTransform->Get_State(TRANSFORM::POSITION));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_3,	m_pTransform->Get_State(TRANSFORM::POSITION));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_4,	m_pTransform->Get_State(TRANSFORM::POSITION));
		//	VFXMGR->Fetch(VFX::KAREN_AIR_PARTICLE_5,	m_pTransform->Get_State(TRANSFORM::POSITION));
		//	m_pTransform->Set_Matrix(mOriginal);

	//	_float3 vLook	= pTransform->Get_State(TRANSFORM::LOOK);
	//	_float3 vPos	= pTransform->Get_State(TRANSFORM::POSITION) - _float3(0.f, 25.f, 0.f) + vLook * 25.f;
	//	Fetch(MULTIVFX::BUS, initializer_list<any>{vPos, vPos, vPos});
	//	Fetch(VFX::BUS_EXP,		vPos);
	//	Fetch(VFX::BUS_EXP_LIG,	vPos);

		shared_ptr<CGameObject> pPlayer(Function::Find_Player());
	//	VFXMGR->Fetch(VFX::STEALTHCHARGE_PARTICLE_REVERSE, make_pair(pPlayer, "Eff01"));
	//	VFXMGR->Fetch(VFX::STEALTHCHARGE_PARTICLE_REVERSE, pPlayer->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION));
	//	VFXMGR->Fetch(VFX::STEALTHCHARGE_PARTICLE, make_pair(pPlayer, "Eff01"));
	//	_float4x4 mOriginal = pTransform->Get_Matrix();
	//	pTransform->Translate(pTransform->Get_State(TRANSFORM::LOOK) * 5.f + _float3(0.f, 0.5f, 0.f));
	//	VFXMGR->Fetch(VFX::STEALTHCHARGE_CHARGE, pTransform->Get_Matrix());
	//	pTransform->Set_Matrix(mOriginal);
	//	Fetch(VFX::STEALTHCHARGE_LIG, make_pair(pPlayer, "Eff01"));
	// 
	//	Fetch(VFX::AFTERIMAGE, Function::Find_Player());
	//	auto a = Fetch(VFX::SPAWN_DIFFUSE, Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION));
//			->Get_Component<CTransform>(COMPONENT::TRANSFORM)->LookTo(Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::LOOK));
	//	auto b = Fetch(VFX::SPAWN_DISTORTION, Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION));
//			->Get_Component<CTransform>(COMPONENT::TRANSFORM)->LookTo(Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::LOOK));
		Fetch(VFX::EM910_WATERBEAM, make_pair(pPlayer, "Reference"));
	}
#endif
}

void CEffect_Manager::Late_Tick(_float _fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	for (auto pair : m_umapEffectPools)
	{
		pair.second->Late_Tick(_fTimeDelta * pGameInstance->Get_Effect_TimeScale());
	}
	for (auto pair : m_umapMultiEffectPools)
	{
		pair.second->Late_Tick(_fTimeDelta * pGameInstance->Get_Effect_TimeScale());
	}

#if ACTIVATE_TOOL
	for (auto pair : m_umapToolEffectPools)
	{
		pair.second->Late_Tick(_fTimeDelta);
	}
#endif
}

shared_ptr<CGameObject> CEffect_Manager::Fetch(VFX _eVFX, any _arg)
{
	if (m_umapEffectPools.end() == m_umapEffectPools.find(_eVFX))
	{
		MSG_RETURN(nullptr, "CEffect_Manager::Fetch", "Invalid Tag");
	}

	return m_umapEffectPools[_eVFX]->Pop(_arg);
}

shared_ptr<CGameObject> CEffect_Manager::Fetch(MULTIVFX _eVFXs, any _ilArgs)
{
	if (m_umapMultiEffectPools.end() == m_umapMultiEffectPools.find(_eVFXs))
	{
		MSG_RETURN(nullptr, "CEffect_Manager::Fetch", "Invalid Tag");
	}

	return m_umapMultiEffectPools[_eVFXs]->Pop(_ilArgs);
}

#if ACTIVATE_TOOL
void CEffect_Manager::Fetch(shared_ptr<CEffect> _pEffect, any _arg, _bool _bReset)
{
	if (_bReset)
	{
		auto iter = m_umapToolEffectPools.find(_pEffect);
		if (iter != m_umapToolEffectPools.end())
		{
			m_umapToolEffectPools.erase(iter);
		}
	}

	if (m_umapToolEffectPools.find(_pEffect) == m_umapToolEffectPools.end())
	{
		m_umapToolEffectPools.emplace(_pEffect, CObjectPool::Create(SCENE::TOOL, 100, _pEffect));
	}

	m_umapToolEffectPools[_pEffect]->Pop(_arg);
}

pair<CEffect_Manager::LOCATION_ITERATOR, CEffect_Manager::LOCATION_ITERATOR> CEffect_Manager::Load_Effects(EFFECT _eEffect)
{
	return m_mmapFileLocation.equal_range(_eEffect);
}
#endif

HRESULT CEffect_Manager::Load_Effect(VFX _eVFX, const wstring& _wstrPath)
{
	if (m_umapEffectPrototypes.end() != m_umapEffectPrototypes.find(_eVFX))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Load_Effect", "Invalid Tag");
	}

	std::ifstream inFile(_wstrPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Load_Effect", "Failed to Open File");
	}

	shared_ptr<CEffect> pInstance;
	EFFECT eEffect(EFFECT::MAX);
	inFile.read(reinterpret_cast<_byte*>(&eEffect),	sizeof(EFFECT));

	switch (eEffect)
	{
	case EFFECT::VFX_Visual:
		pInstance = CEffectRegular_Visual::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_Distortion:
		pInstance = CEffectRegular_Distortion::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticlePoint:
		pInstance = CEffectInstance_ParticlePoint::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticlePointTarget:
		pInstance = CEffectInstance_ParticlePointTarget::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_TrailLineDiffuse:
		pInstance = CEffectInstance_TrailLineDiffuse::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_TrailQuad:
		pInstance = CEffectInstance_TrailQuadDiffuse::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_MeshAfterImage:
		pInstance = CEffectInstance_MeshAfterImage::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_VisualMesh:
		pInstance = CEffectRegular_VisualMesh::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticleMesh:
		pInstance = CEffectInstance_ParticleMesh::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticleRect:
		pInstance = CEffectInstance_ParticleRect::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticlePointCurve:
		pInstance = CEffectInstance_ParticlePointCurve::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_VisualCurve:
		pInstance = CEffectRegular_VisualCurve::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticlePointDynamicCurve:
		pInstance = CEffectInstance_ParticlePointDynamicCurve::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_MeshAnim:
		pInstance = CEffectRegular_MeshAnim::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticlePointVectorFieldCurve:
		pInstance = CEffectInstance_ParticlePointVectorFieldCurve::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_VisualDetailCurve:
		pInstance = CEffectRegular_VisualDetailCurve::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticleLine:
		pInstance = CEffectInstance_ParticleLine::Read(m_pDevice, m_pContext, inFile);
		break;

	default:
		MSG_RETURN(E_FAIL, "CEffect_Manager::Load_Effect", "Invalid EFFECT");
	}

	inFile.close();

	m_umapEffectPrototypes.emplace(_eVFX, pInstance);

#if ACTIVATE_TOOL
	m_mmapFileLocation.emplace(eEffect, _wstrPath);
#endif

	return S_OK;
}

HRESULT CEffect_Manager::Load_Pool(VFX _eVFX, shared_ptr<CEffect> _pPrototype)
{
	if (m_umapEffectPools.end() != m_umapEffectPools.find(_eVFX))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Load_Pool", "Invalid Tag");
	}

	shared_ptr<CObjectPool> pPool = CObjectPool::Create(SCENE::STATIC, 10, dynamic_pointer_cast<CGameObject>(_pPrototype));
	if (nullptr == pPool)
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Load_Pool", "Failed to CGameInstance::Add_Pool");
	}

	m_umapEffectPools.emplace(_eVFX, pPool);

	return S_OK;
}

HRESULT CEffect_Manager::Load_Pool(MULTIVFX _eVFXs, shared_ptr<CEffectLayer> _pPrototype)
{
	if (m_umapMultiEffectPools.end() != m_umapMultiEffectPools.find(_eVFXs))
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Load_Pool", "Invalid Tag");
	}

	shared_ptr<CObjectPool> pPool = CObjectPool::Create(SCENE::STATIC, 10, dynamic_pointer_cast<CGameObject>(_pPrototype));
	if (nullptr == pPool)
	{
		MSG_RETURN(E_FAIL, "CEffect_Manager::Load_Pool", "Failed to CGameInstance::Add_Pool");
	}

	m_umapMultiEffectPools.emplace(_eVFXs, pPool);

	return S_OK;
}
