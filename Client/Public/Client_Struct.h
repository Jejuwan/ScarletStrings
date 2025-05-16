#pragma once

BEGIN(Engine)
class CTransform;
class CTexture;
class CModel;
class CMesh;
class CVIBufferInstance_Mesh;
END

#pragma region Effect

typedef struct tagEffect_Visual
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fLifeTime				= 1.f;

	_bool								bFade					= false;
	_float2								vFadeScope				= _float2(0.5f, 0.5f);

	_int2								vSpriteSheetSize		= _int2(1, 1);
	_float								fBloom					= 0.f;
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_Visual;

typedef struct tagEffect_VisualMesh
{
	shared_ptr<CMesh>					pMesh;
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;
	shared_ptr<CTexture>				pMask;
	shared_ptr<CTexture>				pExtraMask;

	_float								fLifeTime				= 1.f;
	_float3								vRollPitchYaw			= _float3(0.f, 0.f, 0.f);

	_float2								vUVOffsetSpeed			= _float2(0.f, 0.f);
	_float2								vUVOffsetSpeed1			= _float2(0.f, 0.f);
	_float2								vUVOffsetSpeed2			= _float2(0.f, 0.f);
	_float2								vUVOffsetSpeed3			= _float2(0.f, 0.f);

	_float2								vUVOffset				= _float2(0.f, 0.f);
	_float2								vUVOffset1				= _float2(0.f, 0.f);
	_float2								vUVOffset2				= _float2(0.f, 0.f);
	_float2								vUVOffset3				= _float2(0.f, 0.f);

	_float2								vDiffuseTiling			= _float2(1.f, 1.f);
	_float2								vEmissiveTiling			= _float2(1.f, 1.f);
	_float2								vSpecularTiling			= _float2(1.f, 1.f);
	_float2								vMaskTiling				= _float2(1.f, 1.f);

	_bool								bMaskAlpha				= false;
	_bool								bExtraMaskAlpha			= false;
	_bool								bReverseGrayScale0		= false;
	_bool								bReverseGrayScale1		= false;
	_bool								bSRT					= true;
	_bool								bRotateWorldAxis		= false;
	_bool								bBillboard				= false;

	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_VisualMesh;

typedef struct tagEffect_Distortion
{
	shared_ptr<CTexture>				pTexture;

	_float								fLifeTime				= 1.f;

	_float								fDistortionPower		= 0.1f;

	_int2								vSpriteSheetSize		= _int2(1, 1);
	_float								fBloom					= 0.f;
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_Distortion;

typedef struct tagEffect_MeshAnim
{
	vector<shared_ptr<CMesh>>			vecMesh;
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fLifeTime				= 1.f;
	_float2								vUVOffsetSpeed			= _float2(0.f, 0.f);
	
	_float								fDiffuseTiling			= 1.f;
	_float								fEmissiveTiling			= 1.f;

	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_MeshAnim;

typedef struct tagEffect_ParticlePoint
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fLifeTime				= 1.f;
	_float								fInstanceLifeTime		= 1.f;
	_float								fStartGap				= 0.f;
	_float								fSpeed					= 1.f;
	_float								fVelocityScale			= 1.f;

	_float								fTheta					= 0.f;
	_float								fPhi					= 0.f;
	_float3								vForceReference			= _float3(0.f, 1.f, 0.f);

	_float2								vHorizontalScope		= _float2(0.f, 0.f);
	_float2								vVerticalScope			= _float2(0.f, 0.f);
	_float2								vRotationScope			= _float2(0.f, 0.f);

	_bool								bGravity				= false;
	_bool								bMaterialFade			= false;
	_float2								vMaterialFadeScope		= _float2(0.f, 1.f);
	_float								fMaterialFadeWeight		= 1.f;
	_bool								bApplyAlpha				= false;

	_int								iMaxInstance			= 1;
	_float2								vSize					= _float2(1.f, 1.f);
	_int2								vSpriteSheetSize		= _int2(1, 1);
	_float								fBloom					= 0.f;
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_ParticlePoint;

typedef struct tagEffect_ParticlePointCurve
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fTheta					= 0.f;
	_float								fPhi					= 0.f;
	_float3								vForceReference			= _float3(0.f, 1.f, 0.f);

	_float								fInstLifeTimeScale		= 1.f;
	_float								fInstStartTimeScale		= 1.f;
	_float								fVelocityTail			= 0.f;

	_int								iMaxInstance			= 1;
	_int2								vSpriteSheetSize		= _int2(1, 1);
	_float2								vSize					= _float2(1.f, 1.f);
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_ParticlePointCurve;

typedef struct tagEffect_ParticlePointDynamicCurve
{
	shared_ptr<CMesh>					pMesh;
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fTheta					= 0.f;
	_float								fPhi					= 0.f;
	_float3								vForceReference			= _float3(0.f, 1.f, 0.f);

	_float								fInstLifeTimeScale		= 1.f;
	_float								fInstStartTimeScale		= 1.f;
	_float								fVelocityTail			= 0.f;

	_bool								bLocal					= false;
	_bool								bReverse				= false;
	_float3								vLocalSpaceMax			= _float3(0.f, 0.f, 0.f);
	_float3								vLocalSpaceMin			= _float3(0.f, 0.f, 0.f);
	_float3								vSpawnRange				= _float3(1.f, 1.f, 1.f);

	_int								iMaxInstance			= 1;
	_int2								vSpriteSheetSize		= _int2(1, 1);
	_float2								vSize					= _float2(1.f, 1.f);
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_ParticlePointDynamicCurve;

typedef struct tagEffect_ParticlePointVectorFieldCurve
{
	shared_ptr<CMesh>					pMesh;
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;
	shared_ptr<CTexture>				pNoise;

	_float								fLifeTime				= 1.f;
	_float3								vVelocityScale			= _float3(1.f, 1.f, 1.f);
	_float3								vRotationScale			= _float3(1.f, 1.f, 1.f);
	_float3								vSpawnScale				= _float3(1.f, 1.f, 1.f);
	_float3								vNoiseScale				= _float3(1.f, 1.f, 1.f);
	_float3								vNoiseOffset			= _float3(0.f, 0.f, 0.f);
	_float2								vNoiseTiling			= _float2(1.f, 1.f);

	_bool								bLocal					= true;

	_int								iSpawnMethod			= 0;	// 0: None, 1: Spiral
	_float								fSpawnTerm				= 1.f;
	_float								fSpawnOffset			= 0.1f;

	_int								iTerminateMethod		= 0;
	_float								fTerminateTerm			= 1.f;

	_float2								vSize					= _float2(1.f, 1.f);
	_int								iMaxInstance			= 1;
	_int								iNoisePass				= 0;
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_ParticlePointVectorFieldCurve;

typedef struct tagEffect_ParticleLine
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fLifeTime			= 1.f;
	_float								fThickness			= 1.f;
	_float3								vSpawnRange			= _float3(0.f, 0.f, 0.f);

	_float								fReversePower		= 1.f;
	_bool								bReverse			= false;
	_bool								bPreserveDirection	= false;

	_float								fTheta				= 0.f;
	_float								fPhi				= 0.f;
	_float3								vForceReference		= _float3(0.f, 1.f, 0.f);
	_float3								vNormal				= _float3(0.f, 0.f, 0.f);

	_int								iMaxInstance		= 1;
	_float2								vSize				= _float2(1.f, 1.f);
	_int2								vSpriteSheetSize	= _int2(1, 1);
	_int								iBloomPass			= 0;
	_int								iShaderPass			= 0;
} VFX_ParticleLine;

typedef struct tagEffect_VisualCurve
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fLifeTime				= 1.f;
	_float								fDistortionPower		= 0.f;
	
	_float								fDiffuseTiling			= 1.f;
	_float								fEmissiveTiling			= 1.f;

	_int2								vSpriteSheetSize		= _int2(1, 1);
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_VisualCurve;

typedef struct tagEffect_VisualDetailCurve
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;
	shared_ptr<CTexture>				pMask;

	_float								fLifeTime				= 1.f;
	_float								fDistortionPower		= 0.f;
	
	_float								fDiffuseTiling			= 1.f;
	_float								fEmissiveTiling			= 1.f;

	_bool								bLocal					= true;

	_int2								vSpriteSheetSize		= _int2(1, 1);
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_VisualDetailCurve;

typedef struct tagEffect_ParticleLineCurve
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;
	
	_float								fTheta					= 0.f;
	_float								fPhi					= 0.f;
	_float3								vForceReference			= _float3(0.f, 1.f, 0.f);

	_bool								fVelocityTail			= 0.f;

	_int								iMaxInstance			= 1;
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_ParticleLineCurve;

typedef struct tagEffect_ParticleRect
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_bool								bLoop					= true;
	_bool								bFade					= true;
	_bool								bPhysX					= false;
	_bool								bGravity				= false;

	_int								iMaxInstance			= 1;

	_float								fLifeTime				= 1.f;
	_float								fInstanceLifeTime		= 1.f;
	
	_float								fSpeed					= 1.f;
	_float2								vSpeedScope				= _float2(1.f, 1.f);
	_float								fScale					= 1.f;
	_float2								vScaleScope				= _float2(1.f, 1.f);

	_float2								vHorizontalScope		= _float2(0.f, 0.f);
	_float2								vVerticalScope			= _float2(0.f, 0.f);
	_float2								vRotationScope			= _float2(0.f, 0.f);

	_float								fTheta					= 0.f;
	_float								fPhi					= 0.f;
	_float3								vForceReference			= _float3(0.f, 1.f, 0.f);

	_float								fBloom					= 0.f;
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_ParticleRect;

typedef struct tagEffect_ParticlePointTarget
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fInstanceLifeTime		= 1.f;
	_float								fStartGap				= 0.f;
	_float								fSpeed					= 1.f;
	_float								fVelocityScale			= 1.f;

	_float								fTheta					= 0.f;
	_float								fPhi					= 0.f;
	_float3								vForceReference			= _float3(0.f, 1.f, 0.f);
	
	_float2								vHorizontalScope		= _float2(0.f, 0.f);
	_float2								vVerticalScope			= _float2(0.f, 0.f);
	_float2								vRotationScope			= _float2(0.f, 0.f);

	_bool								bGravity				= false;
	_bool								bMaterialFade			= false;
	_float2								vMaterialFadeScope		= _float2(0.f, 1.f);
	_float								fMaterialFadeWeight		= 1.f;
	_bool								bApplyAlpha				= false;

	_int								iMaxInstance			= 1;
	_int2								vSpriteSheetSize		= _int2(1, 1);
	_float2								vSize					= _float2(1.f, 1.f);
	_float								fBloom					= 0.f;
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_ParticlePointTarget;

typedef struct tagEffect_ParticleMesh
{
	shared_ptr<CMesh>					pMesh;
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fLifeTime				= 1.f;
//	_float								fInstanceLifeTime		= 1.f;
//	_float								fStartGap				= 0.f;
//	_float								fSpeed					= 1.f;
//	_float								fVelocityScale			= 1.f;

	_float								fTheta					= 0.f;
	_float								fPhi					= 0.f;
	_float3								vForceReference			= _float3(0.f, 1.f, 0.f);
//	
//	_float2								vHorizontalScope		= _float2(0.f, 0.f);
//	_float2								vVerticalScope			= _float2(0.f, 0.f);
//	_float2								vRotationScope			= _float2(0.f, 0.f);
//
//	_bool								bGravity				= false;
//	_bool								bPhysx					= false;
//	_bool								bMaterialFade			= false;
//	_bool								bApplyAlpha				= false;
	_bool								bBlend					= false;

	_int								iMaxInstance			= 1;
	_float								fBloom					= 0.f;
	_int								iBloomPass				= 0;
	_int								iShaderPass				= 0;
} VFX_ParticleMesh;

typedef struct tagEffect_TrailLineDiffuse
{
	_float								fLifeTime			= 1.f;
	_float								fInterval			= 0.f;
	_float								fThickness			= 1.f;

	_int								iMaxInstance		= 1;
	_float								fBloom				= 0.f;
	_int								iBloomPass			= 0;
	_int								iShaderPass			= 1;
} VFX_TrailLineDiffuse;

typedef struct tagEffect_TrailQuad
{
	shared_ptr<CTexture>				pDiffuse;
	shared_ptr<CTexture>				pEmission;

	_float								fLifeTime			= 1.f;
	_float								fInterval			= 0.f;

	_float								fDistortionPower	= 0.1f;

	_bool								bInstanceTexture	= false;

	_int								iMaxInstance		= 1;
	_float								fBloom				= 0.f;
	_int								iBloomPass			= 0;
	_int								iShaderPass			= 1;
} VFX_TrailQuad;

typedef struct tagEffect_MeshAfterImage
{
	_float								fLifeTime			= 1.f;
	_float								fInterval			= 0.f;
	_float								fAfterImageInterval	= 0.f;
	_float								fFadeOut			= 1.f;
	
	_int								iMaxInstance		= 1;
	_float								fBloom				= 0.f;
	_int								iBloomPass			= 0;
	_int								iShaderPass			= 1;
} VFX_MeshAfterImage;

typedef struct tagInstanceDesc
{
	_float								fTimeAcc			= 0.f;
	_float4x4							mTransformation		= g_mUnit;
	_float4								vVelocity			= _float4(0.f, 0.f, 0.f, 0.f);
	_float								fRotation			= 0.f;
	_float4								vArgument			= _float4(0.f, 0.f, 0.f, 0.f);
} INSTDESC;

#pragma endregion

typedef struct tagWeaponDesc
{
	string szBoneName;
}WEAPONDESC;

typedef struct tagTentacleDesc
{
	string szBoneName;
	_float3 vTentacleMovePos;
	_int index;
}TENTACLEDESC;


typedef struct tagPlayerData
{
	_int iHp;
	_int iKinesis;

	_float fAnimSpeed;
	_float fSpeed;
	_float fDashSpeed;

	_float fJumpHeight;
	_float fJumpForwardSpeed;
	_float fJumpRunForwardSpeed;
	_float fJumpDashForwardSpeed;

	_float fJumpAtkHeight;
	_float fUpperAtkHeight;

	_float fGravity;

	PLAYER_SAS eSAS;
	_float SAS_Gauge[4];
	_float SAS_GaugeIncreaseTime;
	_float SAS_GaugeDecreaseTime;
	_bool bSAS_Enable;
	_float fSasTimeAcc[4];
	_bool bCallSym[4];

	_bool bInvincibility;

	_float fCritRate;

	_bool bDriveMode;
	_float fDriveAnimSpeed;
	_float fDriveGauge;
	_float fDriveGaugeIncrease;
	_float fDriveGaugeDecreaseTime;

	_float fHealValue;

	_bool AcquiredSkill[10];

	_bool bBigHit;
	_bool bJustDodge;
	_float fJustDodgeTime;
	_float fJustDodgeMaxTime;

	_float fKinesisGauge;
	_float fMaxKinesisGauge;
	_float fKinesisConsume;
	_float fKinesisNormalIncrease;
	_float fKinesisBigIncrease;

	_float fBrainFieldGauge;
	_float fMaxBrainFieldGauge;
	_float fBrainFieldDecrease;

	_bool bBrainField = false;
	_bool bRideStair = false;
	_bool bTick = false;
}PLAYERDESC;

typedef struct tagDecalDesc
{
	shared_ptr<CTransform>	pTargetTransform;
	_float4x4				matHitTransform;

	_float					fLifeTime;
} DECALDESC;

typedef struct tagRimEffectDesc
{
	_bool bTransparency;
	_float3 vScale;
	_float3 vRimColor;
	_float fRimStrength;
	_float fRimPower;

	_float fDuration;
	_float fAccTime;
} RIMEFFECTDESC;

typedef struct tagIceDecalDesc
{
	_float4			vDecalColor;

	_float3			vPosition;
	_float3			vScale;

	_float			fTilingFactor;
	_float			fDuration;
	_float			fTimeScale;
} ICEDECALDESC;