#include "ShaderCommon.hlsli"

float g_fThreshold;

float	g_fMaterialFadeRatio	= 0.f;

float fWorldScale = 3.f;
float fTimeScale = 12.f;
float fWaveScale = 0.1f;
float fOffsetScale = 1.f;
float fFlowTimeScale = 1.f;
float fFlowScale = 1.f;
float fMinAlpha = 0.f;
float fMaxAlpha = 1.f;
float4 vOffset = float4(0.f, 1.f, 2.f, 3.f);

float fNormalDepthOffsetScale = 0.f;

//Disolve
static const float fTimeFactor = 4.f;
static const float fEdgeThickness = 0.25f;
static const float fEdgeFluctuation = 0.1f;
static const float3 vDissolveColor = float3(24.f, 2.f, 1.f);

// Aura
Texture2D texAura;
float3 vAuraColor;
float fAuraBlendFactor;
float fAuraTimeFactor;

// Glitch
float g_fSeed;
Texture2D g_vLineNoiseTexture;

struct VS_IN
{
	float3	vPosition	: POSITION;
	float3	vNormal		: NORMAL;
	float3	vTangent	: TANGENT;
	float2	vTexCoord	: TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float4	vTangent	: TANGENT;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
	float4	vProjPos	: TEXCOORD2;
};

struct VS_OUT_SHADOWDEPTH
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: TEXCOORD0;
	float4	vProjPos		: TEXCOORD1;
	float4	vDirection		: TEXCOORD2;
};

struct VS_OUT_OVERSIZING
{
	float4 vPosition : SV_POSITION;
	float4 vProjPos : TEXCOORD0;
};

struct VS_OUT_SAMPLING
{
	float4 vPosition : SV_POSITION;
};

struct VS_OUT_NEON
{
	float4 vPosition : SV_POSITION;
	float2 vTexCoord : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
};

float g_fAccTime;
float g_fDiscardThreshold = 0.7f;

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	matrix	mWV, mWVP;

	mWV				= mul(g_mWorld, g_mView);
	mWVP			= mul(mWV, g_mProj);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vNormal		= normalize(mul(float4(In.vNormal, 0.f), g_mWorld));
	Out.vTangent	= normalize(mul(float4(In.vTangent, 0.f), g_mWorld));
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= mul(float4(In.vPosition, 1.f), g_mWorld);
	Out.vProjPos    = Out.vPosition;

	return Out;
}

VS_OUT_OVERSIZING VS_MAIN_OVERSIZING(VS_IN In)
{
	VS_OUT_OVERSIZING Out;

	matrix mWV, mWVP;

	mWV = mul(g_mWorld, g_mView);
	mWVP = mul(mWV, g_mProj);

	float3 vLocalPosition = In.vPosition + normalize(In.vNormal) * g_fOutLineSize;
	
	Out.vPosition = mul(float4(vLocalPosition, 1.f), mWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}

VS_OUT_SHADOWDEPTH VS_MAIN_SHADOWDEPTH(VS_IN In)
{
	VS_OUT_SHADOWDEPTH	Out;
	
	float4x4	mWV		= mul(g_mWorld, g_mView);
	float4x4	mWVP	= mul(mWV, g_mProj);

//	Out.vNormal			= normalize(mul(float4(In.vNormal, 0.f), g_mWorld));
//	Out.vPosition		= mul(float4(In.vPosition, 1.f) - Out.vNormal, mWVP);
	
	Out.vNormal			= normalize(mul(float4(In.vNormal, 0.f), g_mWorld));
	Out.vPosition		= mul(float4(In.vPosition, 1.f), g_mWorld);
//	Out.vPosition       += Out.vNormal * fNormalDepthOffsetScale;
	Out.vPosition		= mul(Out.vPosition, g_mView);
	Out.vPosition		= mul(Out.vPosition, g_mProj);
	Out.vProjPos		= Out.vPosition;
	Out.vDirection		= g_mView[2];
	
	return Out;
}

VS_OUT_SAMPLING VS_SAMPLING(VS_IN In)
{
	VS_OUT_SAMPLING Out;

	matrix mWV, mWVP;

	mWV = mul(g_mWorld, g_mView);
	mWVP = mul(mWV, g_mProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), mWVP);

	return Out;
}

VS_OUT_NEON VS_NEON(VS_IN In)
{
	VS_OUT_NEON Out;
	
	matrix mWV, mWVP;

	mWV = mul(g_mWorld, g_mView);
	mWVP = mul(mWV, g_mProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), mWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexCoord = In.vTexCoord;
	Out.vTexCoord.x += g_fAccTime;
	
	return Out;
}

VS_OUT VS_MAIN_WATER(VS_IN In)
{
	VS_OUT Out;
	
//	float4x4 mWV	= mul(g_mWorld, g_mView);
//	float4x4 mWVP	= mul(mWV, g_mProj);
//	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	
	Out.vPosition	= mul(float4(In.vPosition, 1.f), g_mWorld);
	Out.vPosition.x += sin(Out.vPosition.x * 10.f * g_fGameTime * 0.5f + 0.25f * 0.f) * 0.02f;
	Out.vPosition.y += sin(Out.vPosition.y * 10.f * g_fGameTime * 0.5f + 0.25f * 1.f) * 0.02f;
	Out.vPosition.z += sin(Out.vPosition.z * 10.f * g_fGameTime * 0.5f + 0.25f * 2.f) * 0.02f;
	Out.vPosition	= mul(Out.vPosition, g_mView);
	Out.vPosition	= mul(Out.vPosition, g_mProj);
	
	Out.vNormal		= normalize(mul(float4(In.vNormal, 0.f), g_mWorld));
	Out.vTangent	= normalize(mul(float4(In.vTangent, 0.f), g_mWorld));
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= mul(float4(In.vPosition, 1.f), g_mWorld);
	Out.vProjPos    = Out.vPosition;
	
	return Out;
}

VS_OUT VS_MAIN_AURA(VS_IN In)
{
    VS_OUT Out;

    matrix mWV, mWVP;

    mWV = mul(g_mWorld, g_mView);
    mWVP = mul(mWV, g_mProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), mWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_mWorld));
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_mWorld));
    Out.vTexCoord = In.vTexCoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_mWorld);
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: NORMAL;
	float4	vTangent		: TANGENT;
	float2	vTexCoord		: TEXCOORD0;
	float4	vWorldPos		: TEXCOORD1;
	float4	vProjPos		: TEXCOORD2;
};

struct PS_IN_SHADOWDEPTH	
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: TEXCOORD0;
	float4	vProjPos		: TEXCOORD1;
	float4	vDirection		: TEXCOORD2;
};

struct PS_IN_OVERSIZING
{
	float4 vPosition : SV_POSITION;
	float4 vProjPos : TEXCOORD0;
};

struct PS_IN_SAMPLING
{
	float4 vPosition : SV_POSITION;
};

struct PS_IN_NEON
{
	float4 vPosition : SV_POSITION;
	float2 vTexCoord : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{
//	float4	vColor			: SV_TARGET0;
	float4	vDiffuse		: SV_TARGET0;
	float4	vAmbient		: SV_TARGET1;
	float4	vSpecular		: SV_TARGET2;
	float4	vEmissive		: SV_TARGET3;
	float4	vNormal			: SV_TARGET4;
	float4	vDepth			: SV_TARGET5;
	float4	vMask			: SV_TARGET6;
};

struct PS_OUT_SHADOWDEPTH
{
	float4	vShadowDepth	: SV_TARGET0;
};

struct PS_OUT_OVERSIZING
{
	float4 vOutLineColor : SV_TARGET0;
};

struct PS_OUT_SAMPLING
{
	float4 vColor : SV_TARGET0;
};

struct PS_OUT_BLOOM
{
	float4  vTexture    : SV_TARGET0;
	float4  vData   : SV_TARGET1;
};

struct PS_OUT_NEON
{
	float4 vColor : SV_TARGET0;
};

struct PS_OUT_POSTPROCESS
{
	float4 vPreProcess : SV_TARGET0;
	float4 vProcessData : SV_TARGET1;
	float4 vMask : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out;
	
	float3		vFinalColor;
	float4		vDiffuse		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
	float4      vAmbient        =   vDiffuse;
	float4      vEmissive       =   float4(1.f, 1.f, 1.f, 1.f);

	float3 vNormal = In.vNormal.xyz;
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
		vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
		float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
		float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
		vNormal = mul(vRecoveredNormal, WorldMatrix);
	}
	
	float4 vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	float vSpecularExist = g_vMtrlSpecular.x + g_vMtrlSpecular.y + g_vMtrlSpecular.z;
	if (vSpecularExist > 0.f)
	{
		float3 vToCam = normalize(g_vCamPosition - In.vProjPos);
		vSpecular = g_texReflection.Sample(LinearSampler, reflect(-vToCam, vNormal));
		vSpecular *= pow(abs(vSpecular.r + vSpecular.g + vSpecular.b) / 3.0, g_fMtrlShininess);
		vSpecular *= g_vMtrlSpecular;
	}
	
	Out.vDiffuse = vDiffuse * g_vMtrlDiffuse;
	Out.vAmbient	= vAmbient * g_vMtrlAmbient;
	Out.vSpecular = vSpecular;
	Out.vEmissive	= vEmissive * g_vMtrlEmissive;
	Out.vNormal     = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
	Out.vMask		= float4(0.f, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In)
{
	PS_OUT_BLOOM Out;
	
	Out.vTexture    = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0] * g_vMtrlDiffuse;
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingEmissive[0] * g_vMtrlEmissive;
        Out.vTexture.rgb += vEmissive.rgb * vEmissive.a;
    }
//	Out.vData       = float4(0.f, 0.f, 0.f, Out.vTexture.a);
//	Out.vTexture.a	= g_fBloomStrength;
	Out.vData       = float4(g_fBloomStrength, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_FADEMATERIAL(PS_IN In)
{
	PS_OUT		Out;
	
	float4	vTexColor				= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
	
	float3 vNormal = In.vNormal.xyz;
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
		vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
		float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
		float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
		vNormal = mul(vRecoveredNormal, WorldMatrix);
	}
	
	float4 vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	float vSpecularExist = g_vMtrlSpecular.x + g_vMtrlSpecular.y + g_vMtrlSpecular.z;
	if (vSpecularExist > 0.f)
	{
		float3 toEye = normalize(g_vCamPosition - In.vProjPos);
		vSpecular = g_texReflection.Sample(LinearSampler, reflect(-toEye, vNormal));
		vSpecular *= pow(abs(vSpecular.r + vSpecular.g + vSpecular.b) / 3.0, g_fMtrlShininess);
		vSpecular *= g_vMtrlSpecular;
	}
	
	Out.vDiffuse	= (float4(1.f, 1.f, 1.f, 1.f) * g_fMaterialFadeRatio + vTexColor * (1.f -  g_fMaterialFadeRatio)) * g_vMtrlDiffuse;
	Out.vAmbient	= (float4(1.f, 1.f, 1.f, 1.f) * g_fMaterialFadeRatio + vTexColor * (1.f -  g_fMaterialFadeRatio)) * g_vMtrlAmbient;
	Out.vSpecular = vSpecular;
	Out.vEmissive	= g_vMtrlEmissive;
	Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
	Out.vMask		= float4(0.f, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT_OVERSIZING PS_MAIN_OVERSIZING(PS_IN_OVERSIZING In)
{
	PS_OUT_OVERSIZING Out;
	
	Out.vOutLineColor = vector(In.vProjPos.z / In.vProjPos.w, g_vOutlineColor.xyz);

	return Out;
}

PS_OUT_SHADOWDEPTH PS_MAIN_SHADOWDEPTH(PS_IN_SHADOWDEPTH In)
{
	PS_OUT_SHADOWDEPTH	Out;
	
//	if (0.f > dot(In.vNormal, In.vDirection))
//	{
//	    discard;
//	}
	
	Out.vShadowDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, 0.f);
	
	return Out;
}

PS_OUT PS_MAIN_LEAVES(PS_IN In)
{
	PS_OUT Out;
	
	float3 vFinalColor;
	float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
	
	if (vTexColor.a <= 0.f)
		discard;
	
	float3 vNormal = In.vNormal.xyz;
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
		vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
		float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
		float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
		vNormal = mul(vRecoveredNormal, WorldMatrix);
	}
	
	float4 vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	float vSpecularExist = g_vMtrlSpecular.x + g_vMtrlSpecular.y + g_vMtrlSpecular.z;
	if (vSpecularExist > 0.f)
	{
		float3 toEye = normalize(g_vCamPosition - In.vProjPos);
		vSpecular = g_texReflection.Sample(LinearSampler, reflect(-toEye, vNormal));
		vSpecular *= pow(abs(vSpecular.r + vSpecular.g + vSpecular.b) / 3.0, g_fMtrlShininess);
		vSpecular *= g_vMtrlSpecular;
	}
	
	Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
	Out.vAmbient = vTexColor * g_vMtrlAmbient;
	Out.vSpecular = vSpecular;
	Out.vEmissive = g_vMtrlEmissive;
	Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);

	return Out;
}

PS_OUT PS_MAIN_CAR(PS_IN In)
{
	PS_OUT Out;
	
	float3 vFinalColor;
	float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
	
	vTexColor.a = 1.f;
	float3 vNormal = In.vNormal.xyz;
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
		vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
		float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
		float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
		vNormal = mul(vRecoveredNormal, WorldMatrix);
	}
	
	float4 vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	float vSpecularExist = g_vMtrlSpecular.x + g_vMtrlSpecular.y + g_vMtrlSpecular.z;
	if (vSpecularExist > 0.f)
	{
		float3 toEye = normalize(g_vCamPosition - In.vProjPos);
		vSpecular = g_texReflection.Sample(LinearSampler, reflect(-toEye, vNormal));
		vSpecular *= pow(abs(vSpecular.r + vSpecular.g + vSpecular.b) / 3.0, g_fMtrlShininess);
		vSpecular *= g_vMtrlSpecular;
	}
	
	Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
	Out.vAmbient = vTexColor * g_vMtrlAmbient;
	Out.vSpecular = vSpecular;
	Out.vEmissive = g_vMtrlEmissive;
	Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);

	return Out;
}

PS_OUT PS_MAIN_CARGLASS(PS_IN In)
{
	PS_OUT Out;
	
	float3 vFinalColor;
	float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
	
	vTexColor.a *= vTexColor.a;
	
	float3 vNormal = In.vNormal.xyz;
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
		vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
		float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
		float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
		vNormal = mul(vRecoveredNormal, WorldMatrix);
	}
	
	float4 vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	float vSpecularExist = g_vMtrlSpecular.x + g_vMtrlSpecular.y + g_vMtrlSpecular.z;
	if (vSpecularExist > 0.f)
	{
		float3 toEye = normalize(g_vCamPosition - In.vProjPos);
		vSpecular = g_texReflection.Sample(LinearSampler, reflect(-toEye, vNormal));
		vSpecular *= pow(abs(vSpecular.r + vSpecular.g + vSpecular.b) / 3.0, g_fMtrlShininess);
		vSpecular *= g_vMtrlSpecular;
	}
	
	Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
	Out.vAmbient = vTexColor * g_vMtrlAmbient;
	Out.vSpecular = vSpecular;
	Out.vEmissive = g_vMtrlEmissive;
	Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);

	return Out;
}

PS_OUT PS_MAIN_GRASS(PS_IN In)
{
	PS_OUT Out;
	
	float3 vFinalColor;
	float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
	if (vTexColor.a <= 0.1)
		discard;
	
	// Normal
	float3 vNormal = In.vNormal.xyz;
	
	float4 vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	float vSpecularExist = g_vMtrlSpecular.x + g_vMtrlSpecular.y + g_vMtrlSpecular.z;
	if (vSpecularExist > 0.f)
	{
		float3 toEye = normalize(g_vCamPosition - In.vProjPos);
		vSpecular = g_texReflection.Sample(LinearSampler, reflect(-toEye, vNormal));
		vSpecular *= pow(abs(vSpecular.r + vSpecular.g + vSpecular.b) / 3.0, g_fMtrlShininess);
		vSpecular *= g_vMtrlSpecular;
	}
	
	Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
	Out.vAmbient = vTexColor * g_vMtrlAmbient;
	Out.vSpecular = vSpecular;
	Out.vEmissive = g_vMtrlEmissive;
	Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
	
	return Out;
}

PS_OUT_SAMPLING PS_SAMPLING(PS_IN_SAMPLING In)
{
	PS_OUT_SAMPLING Out;
	
	Out.vColor = float4(1.f, 1.f, 1.f, 1.f);

	return Out;
}

PS_OUT_NEON PS_NEON(PS_IN_NEON In)
{
	PS_OUT_NEON Out;
	
	Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
	return Out;
}

PS_OUT_NEON PS_NEON_CRASH(PS_IN_NEON In)
{
	PS_OUT_NEON Out;
	
	if (In.vTexCoord.y < 0.9f)
	{
		Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
		
		//vector vCrash = g_texDissolve[0].Sample(LinearSampler, In.vTexCoord * 100.f);
		//if (vCrash.r < fTestThresold)
		//    discard;
		//Out.vColor *= vCrash;
	}
	else
	{
		Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	}

	return Out;
}

float3 SchlickFresnel(float3 vSpecularColor, float fVoH)
{
    float fFc = pow(1.f - fVoH, 5);
    return saturate(50.0 * vSpecularColor.g) * fFc + (1.f - fFc) * vSpecularColor;
}

float GGX(float a2, float fNoH)
{
    float d = (fNoH * a2 - fNoH) * fNoH + 1;
    return a2 / (PI * d * d);
}

float SmithJointApprox(float a2, float fNoV, float fNoL)
{
    float a = sqrt(a2);
    float SmithV = fNoL * (fNoV * (1.f - a) + a);
    float SmithL = fNoV * (fNoL * (1.f - a) + a);

    return 0.5f * rcp(SmithV + SmithL);
}

float3 SpecularBRDF(float3 vSpecularColor, float3 N, float3 V, float3 L, float Roughness, float Metallic, float AO)
{
    float3 H = normalize(V + L);
    float fNoL = saturate(dot(N, L));
    float fNoV = max(dot(N, V), 1e-5);
    float fNoH = saturate(dot(N, H));
    float fVoH = saturate(dot(V, H));
    
    float D = GGX(Roughness, fNoH);
    float G = saturate(SmithJointApprox(Roughness, fNoV, fNoL));
    float3 F = SchlickFresnel(vSpecularColor, fVoH);

    return D * G * F;
}

PS_OUT PS_PBR_NONALPHA(PS_IN In)
{
	PS_OUT Out;
	
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
    
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = normalize(mul(vRecoveredNormal, WorldMatrix));
    }
    
    vector vRMA = g_texAmbient[0].Sample(LinearSampler, In.vTexCoord);
    float fRoughness = vRMA.r;
    float fMetallic = vRMA.g;
    float fAO = vRMA.b;
    
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float3 vDiffuseBRDF = vDiffuse.xyz * (1 / PI);
    
    float3 vLightDirection = normalize(g_vLightDirection);
    float3 vViewDirection = normalize(g_vCamPosition - In.vProjPos);
    float3 vSpecularBRDF = SpecularBRDF(g_vMtrlSpecular.rgb, vNormal, vViewDirection, vLightDirection, fRoughness, fMetallic, fAO);
    
    Out.vDiffuse = float4((vDiffuseBRDF) * fAO, 1.f) * g_vMtrlDiffuse;
    Out.vAmbient = float4(vDiffuseBRDF, 1.f) * g_vMtrlAmbient;
    Out.vSpecular = float4(vSpecularBRDF * g_fMtrlShininess, 1.f);
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_PBR_DISSOLVE(PS_IN In)
{
	PS_OUT Out;
	
	float3 vFinalColor;
	float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	vDiffuse.a = 1.f;
	float4 vAmbient = vDiffuse;

	float3 vNormal = In.vNormal.xyz;
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
		vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
		float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
		float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
		vNormal = mul(vRecoveredNormal, WorldMatrix);
	}
	
	float2 vTexUV;
	vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
	vector vDissolve = g_texDissolve[0].Sample(LinearSampler, vTexUV * 25.f);
	
	Out.vDiffuse = vDiffuse - (vDissolve + g_fAccTime);
	float fColorExist = Out.vDiffuse.x + Out.vDiffuse.y + Out.vDiffuse.z;
	if (fColorExist < 0.1f)
	{
		discard;
	}
	Out.vAmbient = Out.vDiffuse;
	Out.vSpecular = float4(0.f, 0.f, 0.f, 0.f);
	Out.vEmissive = float4(0.f, 0.f, 0.f, 0.f);
	Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
	Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float3 vFinalColor;
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float4 vDissolve = g_texDissolve[0].Sample(LinearSampler, In.vTexCoord);
	
	// Normal
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
    }
	
    Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
    Out.vAmbient = vTexColor * g_vMtrlAmbient;
    Out.vSpecular = g_vMtrlSpecular;
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, 0.f);
    Out.vMask = float4(vDissolve.x, g_fThreshold, 0.f, 0.1f);
	
    float fFluctuationThreshold = g_fThreshold + sin(g_fGameTime * fTimeFactor) * fEdgeFluctuation;
    if (vDissolve.x < fFluctuationThreshold)
    {
        float fDissolveAmount = smoothstep(fFluctuationThreshold, fFluctuationThreshold - fEdgeFluctuation, vDissolve.x);
	
        if (vDissolve.x > fFluctuationThreshold - fEdgeThickness)
        {
            return Out;
        }
        discard;
    }

    return Out;
}

PS_OUT PS_PBR_AURA(PS_IN In)
{
    PS_OUT Out;
	
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
    
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = normalize(mul(vRecoveredNormal, WorldMatrix));
    }
    
    vector vRMA = g_texAmbient[0].Sample(LinearSampler, In.vTexCoord);
    float fRoughness = vRMA.r;
    float fMetallic = vRMA.g;
    float fAO = vRMA.b;
    
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float3 vDiffuseBRDF = vDiffuse.xyz * (1 / PI);
    
    float3 vLightDirection = normalize(g_vLightDirection);
    float3 vViewDirection = normalize(g_vCamPosition - In.vProjPos);
    float3 vSpecularBRDF = SpecularBRDF(g_vMtrlSpecular.rgb, vNormal, vViewDirection, vLightDirection, fRoughness, fMetallic, fAO);
	
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
    vTexUV.y += fAuraTimeFactor;
    vector vAura = texAura.Sample(LinearMirrorSampler, vTexUV);
	
    Out.vDiffuse = (float4(lerp((vDiffuseBRDF * fAO), (vAuraColor * vAura.rgb), fAuraBlendFactor), 1.f)) * g_vMtrlDiffuse;
    Out.vAmbient = Out.vDiffuse * g_vMtrlAmbient;
    Out.vSpecular = float4(vSpecularBRDF * g_fMtrlShininess, 1.f);
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT_NEON PS_GLITCH_NOISE(PS_IN_NEON In)
{
    PS_OUT_NEON Out;
    
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
    
    float fRandFactor0 = frac(sin(dot(float2(g_fSeed, g_fSeed), float2(12.9898f, 78.233f))) * 43758.5453f) + 0.5f;
  
    vTexUV *= 3.f;
    vTexUV.y * 2.f;
    float fGlitch0 = g_vLineNoiseTexture.Sample(LinearMirrorSampler, vTexUV * fRandFactor0).g;

    float fLineNoise = fGlitch0;
    Out.vColor = float4(fLineNoise, fLineNoise, fLineNoise, fLineNoise * 0.5f);

    return Out;
}

technique11 DefaultTechnique
{
	pass MESH //0
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass MESH_WIREFRAME //1
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_WireFrame);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass SHADOWDEPTH //2
	{
		VertexShader	= compile vs_5_0 VS_MAIN_SHADOWDEPTH();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_SHADOWDEPTH();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass OUTLINE		//3
	{
		VertexShader = compile vs_5_0 VS_MAIN_OVERSIZING();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_OVERSIZING();

		SetRasterizerState(RS_InverseCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
	pass NEON			// 4 
	{
		VertexShader = compile vs_5_0 VS_NEON();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_NEON();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Neon, 0);
	}
	pass LEAVES // 5
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LEAVES();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
	pass CAR // 6
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CAR();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass MESH_Alpha //7
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass CARGLASS // 8
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CARGLASS();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
	pass ROAD // 9
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
	pass GRASS // 10
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GRASS();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
	pass MESH_FADEMATERIAL //11
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_FADEMATERIAL();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass BLOOM //12
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass SAMPLING //13
	{
		VertexShader = compile vs_5_0 VS_SAMPLING();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SAMPLING();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass NEON_CRASH // 14 
	{
		VertexShader = compile vs_5_0 VS_NEON();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_NEON_CRASH();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Neon, 0);
	}

	pass PBR_NONALPHA // 15
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_PBR_NONALPHA();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass PBR_DISSOLVE // 16
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_PBR_DISSOLVE();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}


    pass DISSOLVE // 17
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass AURA // 18
    {
        VertexShader = compile vs_5_0 VS_MAIN_AURA();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR_AURA();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass GLITCH_NOISE // 19
    {
        VertexShader = compile vs_5_0 VS_NEON();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_GLITCH_NOISE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }
}