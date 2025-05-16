#include "ShaderCommon.hlsli"

#define	TEMP_AFTERIMAGE
//#define	LUT_SKINNING

int			g_iCurrentMeshIndex	= 0;

#ifndef LUT_SKINNING
#ifdef TEMP_AFTERIMAGE
float4x4	mBone0[MAX_BONE];
float4x4	mBone1[MAX_BONE];
float4x4	mBone2[MAX_BONE];
#else
StructuredBuffer<float4x4>	g_sbBoneInstances;
#endif
#else
Texture2D					g_texBoneInstances	: register(t5);
#endif

#ifdef LUT_SKINNING
float4x4 BoneSample(int iBoneIndex, int iInstanceIndex, int iMeshIndex, int iNumInstance, int iNumMesh)
{
	float4 vRight		=	g_texBoneInstances.SampleLevel(TargetSampler, float2(
							float(iBoneIndex * 4.f + 0.f) / float(MAX_BONE * 4.f),
							float(iInstanceIndex * iNumMesh + iMeshIndex) / float(iNumInstance * iNumMesh)), 0);
	float4 vUp			=	g_texBoneInstances.SampleLevel(TargetSampler, float2(
							float(iBoneIndex * 4.f + 1.f) / float(MAX_BONE * 4.f),
							float(iInstanceIndex * iNumMesh + iMeshIndex) / float(iNumInstance * iNumMesh)), 0);
	float4 vLook		=	g_texBoneInstances.SampleLevel(TargetSampler, float2(
							float(iBoneIndex * 4.f + 2.f) / float(MAX_BONE * 4.f),
							float(iInstanceIndex * iNumMesh + iMeshIndex) / float(iNumInstance * iNumMesh)), 0);
	float4 vTranslation	=	g_texBoneInstances.SampleLevel(TargetSampler, float2(
							float(iBoneIndex * 4.f + 3.f) / float(MAX_BONE * 4.f),
							float(iInstanceIndex * iNumMesh + iMeshIndex) / float(iNumInstance * iNumMesh)), 0);

	return float4x4(vRight, vUp, vLook, vTranslation);
}
#elif !defined(TEMP_AFTERIMAGE)
float4x4 BoneSample(int iBoneIndex, int iInstanceIndex, int iMeshIndex, int iNumInstance, int iNumMesh)
{
    return g_sbBoneInstances[iBoneIndex + (iMeshIndex + iNumMesh * iInstanceIndex) * MAX_BONE];
}
#endif

struct VS_IN
{
	float3	vPosition		: POSITION;
	float3	vNormal			: NORMAL;
	float3	vTangent		: TANGENT;
	float2	vTexCoord		: TEXCOORD0;
	uint4	vBlendIndices	: BLENDINDEX;
	float4	vBlendWeights	: BLENDWEIGHT;
	
	float4	vRight			: TEXCOORD1;
	float4	vUp				: TEXCOORD2;
	float4	vLook			: TEXCOORD3;
	float4	vTranslation	: TEXCOORD4;
	float4	vColor			: TEXCOORD5;
	float4	vArgument		: TEXCOORD6;
};

struct VS_OUT
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: NORMAL;
	float4	vTangent		: TANGENT;
	float2	vTexCoord		: TEXCOORD0;
	float4	vWorldPos		: TEXCOORD1;
	float4	vProjPos		: TEXCOORD2;
	
	float4	vColor			: TEXCOORD3;
	float4	vArgument		: TEXCOORD4;
};

struct VS_OUT_AFTERIMAGE
{
	float4	vPosition		: SV_POSITION;
	
	float4	vColor			: TEXCOORD0;
	float4	vArgument		: TEXCOORD1;
};

VS_OUT_AFTERIMAGE VS_MAIN_AFTERIMAGE(VS_IN In)
{
	VS_OUT_AFTERIMAGE Out;
	
	float4x4 mBone;
#ifndef LUT_SKINNING
#ifdef TEMP_AFTERIMAGE
	float4x4 m;
	switch (In.vArgument.x)
	{
	case 0:
	m =	mul(mBone0[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(mBone0[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(mBone0[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(mBone0[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
		break;
	case 1:
	m =	mul(mBone1[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(mBone1[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(mBone1[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(mBone1[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
		break;
	}
#else
    mBone =
		mul(BoneSample(In.vBlendIndices.x, In.vArgument.x, g_iCurrentMeshIndex, In.vArgument.z, In.vArgument.w), In.vBlendWeights.x) +
		mul(BoneSample(In.vBlendIndices.y, In.vArgument.x, g_iCurrentMeshIndex, In.vArgument.z, In.vArgument.w), In.vBlendWeights.y) +
		mul(BoneSample(In.vBlendIndices.z, In.vArgument.x, g_iCurrentMeshIndex, In.vArgument.z, In.vArgument.w), In.vBlendWeights.z) +
		mul(BoneSample(In.vBlendIndices.w, In.vArgument.x, g_iCurrentMeshIndex, In.vArgument.z, In.vArgument.w), (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));

#endif
#else
	mBone =
		mul(BoneSample(In.vBlendIndices.x, In.vArgument.x, g_iCurrentMeshIndex, In.vArgument.z, In.vArgument.w), In.vBlendWeights.x) +
		mul(BoneSample(In.vBlendIndices.y, In.vArgument.x, g_iCurrentMeshIndex, In.vArgument.z, In.vArgument.w), In.vBlendWeights.y) +
		mul(BoneSample(In.vBlendIndices.z, In.vArgument.x, g_iCurrentMeshIndex, In.vArgument.z, In.vArgument.w), In.vBlendWeights.z) +
		mul(BoneSample(In.vBlendIndices.w, In.vArgument.x, g_iCurrentMeshIndex, In.vArgument.z, In.vArgument.w), (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
#endif
	float4x4	mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
#ifdef LUT_SKINNING
	float4		vPosition		= mul(float4(In.vPosition, 1.f), mBone);
//	float4		vPos			= mul(float4(In.vPosition, 1.f), m);
#elif defined(TEMP_AFTERIMAGE)
	float4		vPosition		= mul(float4(In.vPosition, 1.f), m);
	float4		vPos			= mul(float4(In.vPosition, 1.f), mBone);
#else
	float4		vPosition		= mul(float4(In.vPosition, 1.f), mBone);
#endif
	float4x4	mIWV			= mul(mTransformation, g_mView);
	float4x4	mIWVP			= mul(mIWV, g_mProj);
	
	Out.vPosition	= mul(vPosition, mIWVP);
	
#ifdef LUT_SKINNING
//	Out.vColor		= vPos;
//	Out.vArgument	= vPosition;
	Out.vColor		= In.vColor;
	Out.vArgument	= In.vArgument;
#elif defined(TEMP_AFTERIMAGE)
	Out.vColor		= In.vColor;
	Out.vArgument	= In.vArgument;
#else
    Out.vColor		= In.vColor;
	Out.vArgument	= In.vArgument;
#endif

	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float4	vTangent	: TANGENT;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
	float4	vProjPos	: TEXCOORD2;
	
	float4	vColor		: TEXCOORD3;
	float4	vArgument	: TEXCOORD4;
};

struct PS_IN_AFTERIMAGE
{
	float4	vPosition	: SV_POSITION;
	
	float4	vColor		: TEXCOORD0;
	float4	vArgument	: TEXCOORD1;
};

struct PS_OUT
{
	float4	vDiffuse	: SV_TARGET0;
	float4	vAmbient	: SV_TARGET1;
	float4	vSpecular	: SV_TARGET2;
	float4	vEmissive	: SV_TARGET3;
	float4	vNormal		: SV_TARGET4;
	float4	vDepth		: SV_TARGET5;
};

struct PS_OUT_POSTPROCESS
{
	float4 vPreProcess	: SV_TARGET0;
	float4 vProcessData	: SV_TARGET1;
	float4 vMask		: SV_TARGET2;
};

struct PS_OUT_BLOOM
{
	float4 vTexture : SV_TARGET0;
	float4 vStrength : SV_TARGET1;
};

PS_OUT_POSTPROCESS PS_MAIN_AFTERIMAGE(PS_IN_AFTERIMAGE In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vPreProcess		= g_vMtrlDiffuse * In.vColor;
	Out.vProcessData = In.vArgument;
	Out.vMask			= float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_AFTERIMAGE_BLOOM(PS_IN_AFTERIMAGE In)
{
	PS_OUT_BLOOM Out;
	
	Out.vTexture = g_vMtrlDiffuse;
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		Out.vTexture.rgb += g_vMtrlEmissive.rgb * g_vMtrlEmissive.a;
	}
	Out.vTexture *= In.vColor;
//	Out.vStrength = float4(1.f, 1.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a = g_fBloomStrength * In.vColor.a;
	Out.vStrength = float4(g_fBloomStrength * In.vColor.a, 0.f, 0.f, 0.f);
	
	return Out;
}

technique11 DefaultTechnique
{
	pass Default_Anim_AfterImage
	{
		VertexShader	= compile vs_5_0 VS_MAIN_AFTERIMAGE();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_AFTERIMAGE();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);

	//	SetRasterizerState(RS_NoneCull);
	//	SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	//	SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass Bloom_Anim_AfterImage
	{
		VertexShader	= compile vs_5_0 VS_MAIN_AFTERIMAGE();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_AFTERIMAGE_BLOOM();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);

	//	SetRasterizerState(RS_NoneCull);
	//	SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	//	SetDepthStencilState(DSS_IgnoreDepth, 0);
	}
}
