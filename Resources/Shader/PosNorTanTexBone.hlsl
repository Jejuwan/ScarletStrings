#include "ShaderCommon.hlsli"

float g_fThreshold;

// Cutscene
float3 g_vAdditiveColor = float3(0.f, 0.f, 0.f);

Texture2D g_vLineNoiseTexture;
Texture2D g_vTriangleNoiseTexture;

int g_iDissolve;

// Rim
float3 g_vRimColor = float3(1.f, 1.f, 1.f);
float g_fRimStrength = 0.5f;
float g_fRimPower = 5.f;

//Disolve
static const float fTimeFactor = 4.f;
static const float fEdgeThickness = 0.25f;
static const float fEdgeFluctuation = 0.1f;
static const float3 vDissolveColor = float3(24.f, 2.f, 1.f);

// Item
float4 g_vBaseLineColor;
float2 g_vTilingFactor;
float g_fSeed;

// Grain Noise
float3 g_vGrainColor = float3(0.f, 0.f, 0.f);

// Gradient
float g_fGradientFactor;

float3 g_vGradientStartColor;
float3 g_vGradientEndColor;

// Stripe
float3 g_vStripeColor;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndices : BLENDINDEX;
    float4 vBlendWeights : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct VS_OUT_SHADOWDEPTH
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float4 vDirection : TEXCOORD2;
};

struct VS_OUT_OVERSIZING
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct VS_OUT_NEON
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct VS_OUT_WIRE
{
    float4 vPosition : SV_POSITION;
};

struct VS_OUT_GRADIENT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float2 vTexCoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float fZFactor : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
	
    float4x4 mWV = mul(g_mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);
	
    float4x4 mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
    float4 vPosition = mul(float4(In.vPosition, 1.f), mBone);
    float4 vNormal = mul(float4(In.vNormal, 0.f), mBone);
    float4 vTangent = mul(float4(In.vTangent, 0.f), mBone);

    Out.vPosition = mul(vPosition, mWVP);
    Out.vNormal = normalize(mul(vNormal, g_mWorld));
    Out.vTangent = normalize(mul(vTangent, g_mWorld));
    Out.vTexCoord = In.vTexCoord;
    Out.vWorldPos = mul(vPosition, g_mWorld);
    Out.vProjPos = Out.vPosition;

    return Out;
}

VS_OUT_OVERSIZING VS_MAIN_OVERSIZING(VS_IN In)
{
    VS_OUT_OVERSIZING Out;
	
    float4x4 mVP = mul(g_mView, g_mProj);
	
    float4x4 mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
    float4x4 mFinalTransform = mul(mBone, g_mWorld);
	
    float4 vBlendPosition = mul(float4(In.vPosition, 1.f), mFinalTransform);
    float4 vBlendNormal = normalize(mul(float4(normalize(In.vNormal), 0.f), mFinalTransform));
	
    float4 vScaledPosition = vBlendPosition + vBlendNormal * g_fOutLineSize;
	
    Out.vPosition = mul(vScaledPosition, mVP);
    Out.vProjPos = Out.vPosition;
	
    return Out;
}

VS_OUT_SHADOWDEPTH VS_MAIN_SHADOWDEPTH(VS_IN In)
{
    VS_OUT_SHADOWDEPTH Out;
	
    float4x4 mWV = mul(g_mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);
	
    float4x4 mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
    float4 vPosition = mul(float4(In.vPosition, 1.f), mBone);
    float4 vNormal = mul(float4(In.vNormal, 0.f), mBone);
	
    Out.vNormal = normalize(mul(vNormal, g_mWorld));
    Out.vPosition = mul(vPosition, mWVP);
    Out.vProjPos = Out.vPosition;
    Out.vDirection = g_mView[2];
	
    return Out;
}

VS_OUT_NEON VS_NEON(VS_IN In)
{
    VS_OUT_NEON Out;
	
    float4x4 mWV = mul(g_mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);
	
    float4x4 mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
    float4 vPosition = mul(float4(In.vPosition, 1.f), mBone);

    Out.vPosition = mul(vPosition, mWVP);
    Out.vTexCoord = In.vTexCoord;
    Out.vProjPos = Out.vPosition;

    return Out;
}

VS_OUT_WIRE VS_WIRE(VS_IN In)
{
    VS_OUT_WIRE Out;
	
    float4x4 mWV = mul(g_mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);
	
    float4x4 mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
    float4 vPosition = mul(float4(In.vPosition, 1.f), mBone);
    Out.vPosition = vPosition;

    return Out;
}

VS_OUT_GRADIENT VS_MAIN_GRADIENT(VS_IN In)
{
    VS_OUT_GRADIENT Out;
	
    float4x4 mWV = mul(g_mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);
	
    float4x4 mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
    float4 vPosition = mul(float4(In.vPosition, 1.f), mBone);
    float4 vNormal = mul(float4(In.vNormal, 0.f), mBone);
    float4 vTangent = mul(float4(In.vTangent, 0.f), mBone);

    Out.vPosition = mul(vPosition, mWVP);
    Out.vNormal = normalize(mul(vNormal, g_mWorld));
    Out.vTangent = normalize(mul(vTangent, g_mWorld));
    Out.vTexCoord = In.vTexCoord;
    Out.vProjPos = Out.vPosition;
    Out.fZFactor = In.vPosition.y;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_IN_SHADOWDEPTH
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float4 vDirection : TEXCOORD2;
};

struct PS_IN_OVERSIZING
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_IN_NEON
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_IN_WIRE
{
    float4 vPosition : SV_POSITION;
};

struct PS_IN_GRADIENT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float2 vTexCoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float fZFactor : TEXCOORD2;
};

struct PS_OUT
{
//	float4	vColor			: SV_TARGET0;
    float4 vDiffuse : SV_TARGET0;
    float4 vAmbient : SV_TARGET1;
    float4 vSpecular : SV_TARGET2;
    float4 vEmissive : SV_TARGET3;
    float4 vNormal : SV_TARGET4;
    float4 vDepth : SV_TARGET5;
    float4 vMask : SV_TARGET6;
};

struct PS_OUT_SHADOWDEPTH
{
    float4 vShadowDepth : SV_TARGET0;
};

struct PS_OUT_OVERSIZING
{
    float4 vOutLineColor : SV_TARGET0;
};

struct PS_OUT_BLOOM
{
    float4 vTexture : SV_TARGET0;
    float4 vData : SV_TARGET1;
};

struct PS_OUT_COLOR
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float3 vFinalColor;
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
    if (vTexColor.a <= 0.2)
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
	
    Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
    Out.vAmbient = vTexColor * g_vMtrlAmbient;
    Out.vSpecular = g_vMtrlSpecular;
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out = (PS_OUT_BLOOM) 0;
	
    float3 vFinalColor;
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
    Out.vTexture = vTexColor * g_vMtrlDiffuse;
    Out.vData = float4(g_fBloomStrength, 0.f, 0.f, 0.f);
	
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

PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float3 vDiffuseBRDF = vDiffuse.xyz * (1 / PI);
    float4 vDissolve = g_texDissolve[0].Sample(LinearSampler, In.vTexCoord);
    if (vDiffuse.a <= 0.3f)
        discard;
	// Normal
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
	
    float3 vLightDirection = normalize(g_vLightDirection);
    float3 vViewDirection = normalize(g_vCamPosition - In.vProjPos);
    float3 vSpecularBRDF = SpecularBRDF(g_vMtrlSpecular.rgb, vNormal, vViewDirection, vLightDirection, fRoughness, fMetallic, fAO);
	
    Out.vDiffuse = float4((vDiffuseBRDF) * fAO, 1.f) * g_vMtrlDiffuse;
    Out.vAmbient = float4(vDiffuseBRDF, 1.f) * g_vMtrlAmbient;
    Out.vSpecular = float4(vSpecularBRDF * g_fMtrlShininess, 1.f);
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
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

PS_OUT_SHADOWDEPTH PS_MAIN_SHADOWDEPTH(PS_IN_SHADOWDEPTH In)
{
    PS_OUT_SHADOWDEPTH Out;
	
//	if (0.f > dot(In.vNormal, In.vDirection))
//	{
//	    discard;
//	}
	
    Out.vShadowDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, 0.f);
	
    return Out;
}

PS_OUT_OVERSIZING PS_MAIN_OVERSIZING(PS_IN_OVERSIZING In)
{
    PS_OUT_OVERSIZING Out;
	
    Out.vOutLineColor = vector(In.vProjPos.z / In.vProjPos.w, g_vOutlineColor.rgb);

    return Out;
}

PS_OUT PS_MAIN_NONALPHA(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float3 vFinalColor;
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
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
	
    Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
    Out.vAmbient = vTexColor * g_vMtrlAmbient;
    Out.vSpecular = g_vMtrlSpecular;
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_CUTSCENE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
    
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = normalize(mul(vRecoveredNormal, WorldMatrix));
    }
	
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float3 vDiffuseBRDF = vDiffuse.xyz * (1 / PI);
	
    float4 vAmbient = float4(vDiffuseBRDF, 1.f);
	
    Out.vDiffuse = float4(vDiffuseBRDF, 1.f) * g_vMtrlDiffuse;
    Out.vAmbient = vAmbient * g_vMtrlAmbient;
    Out.vSpecular = g_vMtrlSpecular;
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

	// Additive Color
    Out.vDiffuse.xyz += g_vAdditiveColor;
	
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	// Line Noise
    vector vLineNoise = g_vLineNoiseTexture.Sample(LinearSampler, vTexUV * 50.f);
    Out.vDiffuse.xyz *= vLineNoise.a;
	
	// Triangle Noise
    if (g_iDissolve)
    {
        vector vTriangleNoise = g_vTriangleNoiseTexture.Sample(LinearSampler, vTexUV * 5.f);
        if (vTriangleNoise.x <= g_fThreshold)
            discard;
		
        vector vDissolveNoise = g_texDissolve[0].Sample(PointSampler, vTexUV * 25.f);
        Out.vDiffuse.xyz -= (vDissolveNoise.xyz * g_fThreshold);
		
        float fColorExist = Out.vDiffuse.r + Out.vDiffuse.g + Out.vDiffuse.b;
        if (fColorExist <= 0.4f)
            discard;
    }
	
    return Out;
}

PS_OUT_OVERSIZING PS_MAIN_OVERSIZING_CUTSCENE(PS_IN_OVERSIZING In)
{
    PS_OUT_OVERSIZING Out;
	
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
	
    float3 vOutLineColor = g_vOutlineColor;
	
	// Triangle Noise
    if (g_iDissolve)
    {
        vector vTriangleNoise = g_vTriangleNoiseTexture.Sample(LinearSampler, vTexUV * 5.f);
        if (vTriangleNoise.x == 0.f)
            discard;
		
        vector vDissolveNoise = g_texDissolve[0].Sample(LinearSampler, vTexUV * 25.f);
        vOutLineColor -= (vDissolveNoise.rgb * g_fThreshold);
    }
	
    Out.vOutLineColor = vector(In.vProjPos.z / In.vProjPos.w, vOutLineColor.rgb);

    return Out;
}

PS_OUT_COLOR PS_NEON(PS_IN_NEON In)
{
    PS_OUT_COLOR Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
	
    return Out;
}

PS_OUT_COLOR PS_TRANSPARENT(PS_IN In)
{
    PS_OUT_COLOR Out = (PS_OUT_COLOR) 0;
   
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
    }

    float4 vColor = float4(g_vRimColor, 0.f);
	
    float4 vToCamera = normalize((g_vCamPosition - In.vWorldPos));
    float vRim = pow(max(0.f, (1 - abs(dot(In.vNormal.xyz, vToCamera.xyz)))), g_fRimPower) * g_fRimStrength;
    vColor.a = vRim;
	
    Out.vColor = vColor;

    return Out;
}

PS_OUT PS_DRIVE_MASK(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float4 vFinalColor = float4(0.f, 0.f, 0.f, 1.f);
    if (g_fThreshold < 1.f)
    {
        float4 vDissolve = g_texDissolve[0].Sample(LinearSampler, In.vTexCoord);
		
        float fDissolveFactor = vDissolve.r - (1.f - g_fThreshold);
        if (fDissolveFactor <= 0.f)
        {
            discard;
        }
        else
        {
            if (abs(fDissolveFactor) <= 0.05f)
                vFinalColor = float4(0.5f, 1.f, 1.f, 1.f);
            else
                vFinalColor = float4(0.f, 0.f, 0.f, 1.f);
        }
    }
    else
    {
        vFinalColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
        vFinalColor.r *= 10.f;
    }
	
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
    }
	
    Out.vDiffuse = vFinalColor * g_vMtrlDiffuse;
    Out.vAmbient = vFinalColor * g_vMtrlAmbient;
    Out.vSpecular = g_vMtrlSpecular;
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
}

PS_OUT PS_ITEM(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float4 vFinalColor = float4(0.f, 0.15f, 0.f, 1.f);
	
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
	
    float4 vGlitch0;
    float4 vGlitch1;
	
    float fRandFactor0 = frac(sin(dot(float2(g_fSeed, g_fSeed), float2(12.9898f, 78.233f))) * 43758.5453f);
    float fRandFactor1 = frac(sin(dot(float2(g_fSeed, g_fSeed), float2(78.233f, 12.9898f))) * 43758.5453f);
	
    vGlitch0 = g_texDiffuse[0].Sample(LinearSampler, vTexUV * fRandFactor0 * g_vTilingFactor);
    vGlitch1 = g_texDiffuse[1].Sample(LinearSampler, In.vTexCoord * fRandFactor1 * g_vTilingFactor);
	
    Out.vDiffuse = (vFinalColor + vGlitch0 + vGlitch1) * g_vMtrlDiffuse;
    Out.vAmbient = (vFinalColor + vGlitch0 + vGlitch1) * g_vMtrlAmbient;
    Out.vSpecular = g_vMtrlSpecular;
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(0.f, 0.f, 0.f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
}

PS_OUT_BLOOM PS_ITEM_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out = (PS_OUT_BLOOM) 0;
	
    float4 vFinalColor = float4(0.f, 0.15f, 0.f, 1.f);
	
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
	
    float4 vGlitch0;
    float4 vGlitch1;
	
    float fRandFactor0 = frac(sin(dot(float2(g_fSeed, g_fSeed), float2(12.9898f, 78.233f))) * 43758.5453f);
    float fRandFactor1 = frac(sin(dot(float2(g_fSeed, g_fSeed), float2(78.233f, 12.9898f))) * 43758.5453f);
	
    vGlitch0 = g_texDiffuse[0].Sample(LinearSampler, vTexUV * fRandFactor0 * g_vTilingFactor);
    vGlitch1 = g_texDiffuse[1].Sample(LinearSampler, In.vTexCoord * fRandFactor1 * g_vTilingFactor);
	
    Out.vTexture = vFinalColor + vGlitch0 + vGlitch1;
    Out.vData = float4(g_fBloomStrength, 0.f, 0.f, 0.f);
	
    return Out;
}

[maxvertexcount(4)]
void GS_RenderLines(triangle VS_OUT_WIRE In[3], inout LineStream<PS_IN_WIRE> stream)
{
    float4x4 mWV = mul(g_mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);
	
    PS_IN_WIRE Out;
	
    float dist01 = distance(In[0].vPosition, In[1].vPosition);
    float dist02 = distance(In[0].vPosition, In[2].vPosition);
    float dist12 = distance(In[1].vPosition, In[2].vPosition);
	
    float maxLength = max(dist01, max(dist02, dist12));
	
    if (dist12 == maxLength)
    {
        Out.vPosition = mul(In[0].vPosition, mWVP);
        stream.Append(Out);
        Out.vPosition = mul(In[1].vPosition, mWVP);
        stream.Append(Out);
		
        stream.RestartStrip();

        Out.vPosition = mul(In[0].vPosition, mWVP);
        stream.Append(Out);
        Out.vPosition = mul(In[2].vPosition, mWVP);
        stream.Append(Out);
    }
    else if (dist02 == maxLength)
    {
        Out.vPosition = mul(In[0].vPosition, mWVP);
        stream.Append(Out);
        Out.vPosition = mul(In[1].vPosition, mWVP);
        stream.Append(Out);
		
        stream.RestartStrip();

        Out.vPosition = mul(In[1].vPosition, mWVP);
        stream.Append(Out);
        Out.vPosition = mul(In[2].vPosition, mWVP);
        stream.Append(Out);
    }
    else if (dist01 == maxLength)
    {
        Out.vPosition = mul(In[1].vPosition, mWVP);
        stream.Append(Out);
        Out.vPosition = mul(In[2].vPosition, mWVP);
        stream.Append(Out);
		
        stream.RestartStrip();

        Out.vPosition = mul(In[0].vPosition, mWVP);
        stream.Append(Out);
        Out.vPosition = mul(In[2].vPosition, mWVP);
        stream.Append(Out);
    }
}

PS_OUT_COLOR PS_WIREFRAME_BLOOM(PS_IN_WIRE In)
{
    PS_OUT_COLOR Out = (PS_OUT_COLOR) 0;
	
    Out.vColor = g_vBaseLineColor;
	
    return Out;
}

PS_OUT PS_DECAL_TARGET(PS_IN In)
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
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
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
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
}

PS_OUT PS_AMB(PS_IN In)
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
	
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float3 vDiffuseBRDF = vDiffuse.xyz * (1 / PI);
    
    vector vAmbient = g_texAmbient[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vDiffuse = float4((vDiffuseBRDF), 1.f) * g_vMtrlDiffuse;
    Out.vAmbient = vAmbient * g_vMtrlAmbient;
    Out.vSpecular = float4(0.f, 0.f, 0.f, 0.f);
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
}

PS_OUT PS_PBR_ALPHADISCARD(PS_IN In)
{
    PS_OUT Out;
	
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    if (vDiffuse.a <= 0.2)
        discard;
	
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
    
    float3 vDiffuseBRDF = vDiffuse.xyz * (1 / PI);
    
    float3 vLightDirection = normalize(g_vLightDirection);
    float3 vViewDirection = normalize(g_vCamPosition - In.vProjPos);
    float3 vSpecularBRDF = SpecularBRDF(g_vMtrlSpecular.rgb, vNormal, vViewDirection, vLightDirection, fRoughness, fMetallic, fAO);
    
    Out.vDiffuse = float4((vDiffuseBRDF) * fAO, 1.f) * g_vMtrlDiffuse;
    Out.vAmbient = float4(vDiffuseBRDF, 1.f) * g_vMtrlAmbient;
    Out.vSpecular = float4(vSpecularBRDF * g_fMtrlShininess, 1.f);
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
}

PS_OUT_COLOR PS_GLITCH_NOISE(PS_IN_NEON In)
{
    PS_OUT_COLOR Out;
    
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

float FilmGrainNoise(float fTime, float2 vTexcoord)
{
    return frac(sin(dot(vTexcoord, float2(12.9898f, 78.233f) * fTime)) * 43758.5453);
}

PS_OUT PS_PBR_GRAIN_NOISE(PS_IN In)
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
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
    
    float fGrainFactor = FilmGrainNoise(g_vGrainColor.b * 2.f, In.vTexCoord);
    Out.vDiffuse.rgb = lerp(Out.vDiffuse.rgb, g_vGrainColor, fGrainFactor * g_vGrainColor.b);
	
    return Out;
}

PS_OUT PS_LINEAR_GRADIENT(PS_IN_GRADIENT In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
    }
    
    float fGradientFactor = (In.fZFactor + g_fGradientFactor) / 1.6f;
    float4 vTimeGradient = lerp(float4(0.f, 0.f, 0.f, 1.f), float4(1.f, 0.f, 0.f, 1.f), 1.f - fGradientFactor);
    vTexColor += vTimeGradient;
    
    Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
    Out.vAmbient = vTexColor * g_vMtrlAmbient;
    Out.vSpecular = g_vMtrlSpecular;
    Out.vEmissive = vTimeGradient;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT_COLOR PS_STRIPE(PS_IN_NEON In)
{
    PS_OUT_COLOR Out;
    
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
    
    float4 vStripe = g_texAmbient[0].Sample(LinearSampler, vTexUV * 45.f);
    if (vStripe.r == 0.f)
        discard;

    Out.vColor = float4(g_vStripeColor, 1.f);

    return Out;
}

PS_OUT PS_RIMLIGHT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
   
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
    }

    float4 vRimColor = float4(g_vRimColor, 0.f);
	
    float4 vToCamera = normalize((g_vCamPosition - In.vWorldPos));
    float vRim = pow(max(0.f, (1 - abs(dot(In.vNormal.xyz, vToCamera.xyz)))), g_fRimPower) * g_fRimStrength;
    vRimColor *= vRim;
	
    Out.vDiffuse = (vTexColor + vRimColor) * g_vMtrlDiffuse;
    Out.vAmbient = vTexColor * g_vMtrlAmbient;
    Out.vSpecular = float4(0.f, 0.f, 0.f, 1.f);
    Out.vEmissive = float4(0.f, 0.f, 0.f, 1.f);
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT_COLOR PS_STRIPE_DISSOLVE(PS_IN_NEON In)
{
    PS_OUT_COLOR Out;
    
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
    
    float fNoise = g_vLineNoiseTexture.Sample(LinearSampler, vTexUV * 4.f).r;
    if (fNoise > g_fThreshold)
    {
        float4 vStripe = g_texAmbient[0].Sample(LinearSampler, vTexUV * 45.f);
        if (vStripe.r == 0.f)
            discard;
        
        Out.vColor = float4(g_vStripeColor, 1.f);
    }
    else
    {
        Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    }

    return Out;
}

PS_OUT_COLOR PS_SAMPLE_STRIPE(PS_IN_NEON In)
{
    PS_OUT_COLOR Out;
    
    Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
    
    float2 vTexUV;
    vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

    vector vLineNoise = g_vLineNoiseTexture.Sample(LinearSampler, vTexUV * 50.f);
    Out.vColor.xyz *= (0.5f + vLineNoise.a);

    return Out;
}

PS_OUT PS_SMOOTH_GRADIENT(PS_IN_GRADIENT In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
    }
    
    float fGradientFactor = saturate(In.fZFactor / g_fGradientFactor);
    float3 vGradientColor = lerp(g_vGradientStartColor, g_vGradientEndColor, fGradientFactor);
    vTexColor.rgb += vGradientColor;
    
    Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
    Out.vAmbient = Out.vDiffuse;
    Out.vSpecular = g_vMtrlSpecular;
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT PS_AMB_ALPHA(PS_IN In)
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
	
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float3 vDiffuseBRDF = vDiffuse.xyz * (1 / PI);
    
    vector vAmbient = g_texAmbient[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vDiffuse = float4((vDiffuseBRDF), 1.f) * g_vMtrlDiffuse;
    Out.vDiffuse.a = vDiffuse.a;
    Out.vAmbient = vAmbient * g_vMtrlAmbient;
    Out.vAmbient.a = vDiffuse.a;
    Out.vSpecular = float4(0.f, 0.f, 0.f, 0.f);
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
}

PS_OUT PS_DECAL_SHADOW_TARGET(PS_IN In)
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
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 5.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
}

technique11 DefaultTechnique
{
    pass MESH //0
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass WIREFRAME //1
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_WireFrame);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass SHADOWDEPTH //2
    {
        VertexShader = compile vs_5_0 VS_MAIN_SHADOWDEPTH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOWDEPTH();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass OUTLINE //3
    {
        VertexShader = compile vs_5_0 VS_MAIN_OVERSIZING();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_OVERSIZING();

        SetRasterizerState(RS_InverseCull);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass NONALPHA //4
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONALPHA();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass DISSOLVE //5
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

    pass BLOOM //6
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass CUTSCENE //7
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CUTSCENE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass CUTSCENE_OUTLINE //8
    {
        VertexShader = compile vs_5_0 VS_MAIN_OVERSIZING();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_OVERSIZING_CUTSCENE();

        SetRasterizerState(RS_InverseCull);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass NEON // 9
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

    pass TRANSPARENT //10
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_TRANSPARENT();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass DRIVE_MASK // 11
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DRIVE_MASK();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass ITEM // 12
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_ITEM();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass ITEM_BLOOM // 13
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_ITEM_BLOOM();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass WIREFRAME_BLOOM // 14
    {
        VertexShader = compile vs_5_0 VS_WIRE();
        GeometryShader = compile gs_5_0 GS_RenderLines();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_WIREFRAME_BLOOM();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass DECAL_TARGET // 15
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DECAL_TARGET();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass PBR_NONALPHA // 16
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

    pass MESH_AMB // 17
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_AMB();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass PBR_ALPHADISCARD // 18
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR_ALPHADISCARD();

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

    pass GRAIN_NOISE // 20
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR_GRAIN_NOISE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass LinearGradient // 21
    {
        VertexShader = compile vs_5_0 VS_MAIN_GRADIENT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LINEAR_GRADIENT();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass STRIPE // 22
    {
        VertexShader = compile vs_5_0 VS_NEON();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_STRIPE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass RIMLIGHT // 23
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RIMLIGHT();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass RIM_TRANSPARENT //24
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_TRANSPARENT();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass STRIPE_DISSOLVE // 25
    {
        VertexShader = compile vs_5_0 VS_NEON();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_STRIPE_DISSOLVE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass SAMPLE_STRIPE // 26
    {
        VertexShader = compile vs_5_0 VS_NEON();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SAMPLE_STRIPE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass SMOOTH_Gradient // 27
    {
        VertexShader = compile vs_5_0 VS_MAIN_GRADIENT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SMOOTH_GRADIENT();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass MESH_AMB_ALPHA // 28
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_AMB_ALPHA();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass DECAL_SHADOW_TARGET // 29
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DECAL_SHADOW_TARGET();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
}
