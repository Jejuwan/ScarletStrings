#pragma once

enum class SCENE
{
	STATIC,
	LOADING,
	TEST,
	REALTEST,
	MIZUHA,
	SUOH,
	SUOH_BATTLE,
	MINDROOM,
	HIDEOUT,
#ifdef ACTIVATE_TOOL
	TOOL,
	MAPTOOL,
#endif
	GAMEPLAY,
	CUTSCENE_MIZUHA,
	TITLE,
	MAX
};

enum DIRECTION {
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	DIR_END
};

enum class COLLISION_GROUP
{
	MAX
};

enum class SOUND_CHANNEL
{
	MAX
};

enum class EFFECT	// TYPE
{
	VFX_Visual,
	VFX_Distortion,
	VFX_ParticlePoint,
	VFX_TrailLineDiffuse,
	VFX_TrailQuad,
	VFX_MeshAfterImage,
	VFX_ParticlePointTarget,
	VFX_VisualMesh,
	VFX_ParticleMesh,
	VFX_ParticleRect,
	VFX_ParticlePointCurve,
	VFX_VisualCurve,
//	VFX_ParticleLineCurve,
//	VFX_MeshSplash,
	VFX_ParticlePointDynamicCurve,
	VFX_MeshAnim,
	VFX_ParticlePointVectorFieldCurve,
	VFX_VisualDetailCurve,
	VFX_ParticleLine,
	MAX
};

enum class PARTICLE
{

};

enum class PLAYER_STATE
{
	IDLE,
	RUN,
	JUMP,
	ATTACK,
	ATTACK1,
	ATTACK2,
	ATTACK3,
	ATTACK_DASH,
	ATTACK_BACKSTEP,
	ATTACK_UPPER,
	JUMP_ATTACK1,
	JUMP_ATTACK2,
	JUMP_ATTACK3,
	ATTACK4,
	ATTACK_AIRDOWN,
	ATTACK_CHARGE,
	ATTACK_FOLLOW1,
	ATTACK_FOLLOW2,
	ATTACK_JUSTDODGE,
	SWING_A1,
	SWING_A2,
	SWING_A3,
	HURT,
	DASH,
	THROW,
	SPECIALTHROW,
	EXECUTE,
	DODGE,
	BRAIN_DRIVE,
	BRAIN_FIELD,
	HEAL
};

enum class PLAYER_DIR
{
	F,
	FR,
	R,
	BR,
	B,
	BL,
	L,
	FL
};

enum class PLAYER_SAS
{
	STEALTH,
	ACCELERATION,
	ELECTRIC,
	REPLICATION,
	NONE
};

enum class PLAYER_ADDITIVE_SKILL
{
	FORTH,
	ONECLICKCHARGE,
	JUMPDASHATK,
	JUMPDOWNATK,
	JUSTDODGEATK,
	SECONDTHROW
};

enum class SPECIAL_THROW
{
	HEAVY_EQUIPMENT,
	BEAM,
	BEAM_PIECE,
	ROCK,
	BULLDOZER,
	BUS,
	FINAL_ROCK,
	LONG,
	MINDROOM,
	MAX
};
#pragma region Monster
#pragma region DispenPerry
enum class DP_PHASE
{
	PHASE0,
	PHASE1,
	PHASE2,
	CLEAR,
	MAX
};
enum class DP_STATE
{
	NOEVENT,
	DAMAGE_I,
	DAMAGE_M,
	WEAK,
	MAX
};
enum class DP_PARTS
{
	BODY,
	WATERTANK,
	MAX
};
#pragma endregion
#pragma region FlowerChest
enum class FC_PHASE
{
	PHASE0,
	PHASE1,
	PHASE2,
	PHASE3,
	PHASE4,
	MAX
};
#pragma endregion
#pragma endregion
enum class MAPTYPE
{
	COLLISION,
	NONCOLLISION,
	DYNAMIC,
	TYPE_END
};

enum class CHARACTER
{
	ARASHI,
	GEMMA,
	HARUKA,
	KASANE,
	KAGERO,
	KYOKA,
	NAOMI,
	SHIDEN,
};

enum class EVENT_TYPE
{
	BATTLE_FIELD,
	TUTORIAL,
	DIALOG,
	CUTSCENE,
	BOSS,

	NONE,
};