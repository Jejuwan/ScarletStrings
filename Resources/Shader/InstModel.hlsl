#include "ShaderCommon.hlsli"

#define MAX_INSTANCE 256

float g_fMaterialFadeRatio = 0.f;

float fWorldScale = 3.f;
float fTimeScale = 12.f;
float fWaveScale = 0.1f;
float fOffsetScale = 1.f;
float fFlowTimeScale = 1.f;
float fFlowScale = 1.f;
float fMinAlpha = 0.f;
float fMaxAlpha = 1.f;
float4 vOffset = float4(0.f, 1.f, 2.f, 3.f);

float g_fAccTime;

// Demo
float g_fMinDistance = 7.f;
float g_fMaxDistance = 30.f;

float3 g_vScaleFactor;

// Circular Sampling
float3 g_vTargetPosition;
float g_fSampleRadius;
float g_fSampleMinRadius;

// Single Color
float3 g_vSingleColor;
float g_fColorFactor;

struct VS_IN
{
    float3 vPosition	: POSITION;
    float3 vNormal		: NORMAL;
    float3 vTangent		: TANGENT;
    float2 vTexCoord	: TEXCOORD0;
	
	float4 vRight		: TEXCOORD1;
	float4 vUp			: TEXCOORD2;
	float4 vLook		: TEXCOORD3;
	float4 vTranslation	: TEXCOORD4;
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

struct VS_OUT_SAMPLING
{
    float4 vPosition : SV_POSITION;
};

struct VS_OUT_BATTLEFILED
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    float2 vAnimatedTexCoord : TEXCOORD3;
    
    float2 vScaleFactor : TEXCOORD4;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	float4x4	mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4		vPosition		= float4(In.vPosition, 1.f);

    float4x4	mIWV				= mul(mTransformation, g_mView);
	float4x4	mIWVP			= mul(mIWV, g_mProj);
	
	Out.vPosition	= mul(vPosition, mIWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), mTransformation));
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), mTransformation));
	Out.vTexCoord	= In.vTexCoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), mTransformation);
	Out.vProjPos    = Out.vPosition;

	return Out;
}

VS_OUT_SHADOWDEPTH VS_MAIN_SHADOWDEPTH(VS_IN In)
{
    VS_OUT_SHADOWDEPTH Out;
	
    float4x4 mTransformation = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4 vPosition = float4(In.vPosition, 1.f);

    float4x4 mIWV = mul(mTransformation, g_mView);
    float4x4 mIWVP = mul(mIWV, g_mProj);
	
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), mTransformation));
    Out.vPosition = mul(float4(In.vPosition, 1.f), mTransformation);
//	Out.vPosition       += Out.vNormal * fNormalDepthOffsetScale;
    Out.vPosition = mul(Out.vPosition, g_mView);
    Out.vPosition = mul(Out.vPosition, g_mProj);
    Out.vProjPos = Out.vPosition;
    Out.vDirection = g_mView[2];
	
    return Out;
}

VS_OUT_OVERSIZING VS_MAIN_OVERSIZING(VS_IN In)
{
    VS_OUT_OVERSIZING Out;

    float4x4 mTransformation = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4 vPosition = float4(In.vPosition, 1.f);

    float4x4 mIWV = mul(mTransformation, g_mView);
    float4x4 mIWVP = mul(mIWV, g_mProj);

    float3 vLocalPosition = In.vPosition + normalize(In.vNormal) * g_fOutLineSize;
	
    Out.vPosition = mul(float4(vLocalPosition, 1.f), mIWVP);
    Out.vProjPos = Out.vPosition;

    return Out;
}

VS_OUT_NEON VS_NEON(VS_IN In)
{
    VS_OUT_NEON Out;
    
    float4x4 mTransformation = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4 vPosition = float4(In.vPosition, 1.f);

    float4x4 mIWV = mul(mTransformation, g_mView);
    float4x4 mIWVP = mul(mIWV, g_mProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), mIWVP);
    Out.vProjPos = Out.vPosition;
    Out.vTexCoord = In.vTexCoord;
    Out.vTexCoord.x += g_fAccTime;
    
    return Out;
}

VS_OUT_NEON VS_NEON_WALL(VS_IN In)
{
    VS_OUT_NEON Out;
    
    float4x4 mTransformation = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4 vPosition = float4(In.vPosition, 1.f);

    float4x4 mIWV = mul(mTransformation, g_mView);
    float4x4 mIWVP = mul(mIWV, g_mProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), mIWVP);
    Out.vProjPos = Out.vPosition;
    Out.vTexCoord = In.vTexCoord;
    Out.vTexCoord.y -= g_fAccTime;
	
    return Out;
}

VS_OUT_NEON VS_NEON_WARNING(VS_IN In)
{
    VS_OUT_NEON Out;
    
    float4x4 mTransformation = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4 vPosition = float4(In.vPosition, 1.f);

    float4x4 mIWV = mul(mTransformation, g_mView);
    float4x4 mIWVP = mul(mIWV, g_mProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), mIWVP);
    Out.vProjPos = Out.vPosition;
    Out.vTexCoord = In.vTexCoord;
	
    return Out;
}

VS_OUT VS_MINDROOM_WATER(VS_IN In)
{
    VS_OUT Out;

    float4x4 mTransformation = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4 vPosition = float4(In.vPosition, 1.f);

    float4x4 mIWV = mul(mTransformation, g_mView);
    float4x4 mIWVP = mul(mIWV, g_mProj);
	
    Out.vPosition = mul(vPosition, mIWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), mTransformation));
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), mTransformation));
    Out.vTexCoord = In.vTexCoord;
    Out.vTexCoord.x += g_fAccTime;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), mTransformation);
    Out.vProjPos = Out.vPosition;

    return Out;
}

VS_OUT_SAMPLING VS_SAMPLING(VS_IN In)
{
    VS_OUT_SAMPLING Out;

    float4x4 mTransformation = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4 vPosition = float4(In.vPosition, 1.f);

    float4x4 mIWV = mul(mTransformation, g_mView);
    float4x4 mIWVP = mul(mIWV, g_mProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), mIWVP);

    return Out;
}

VS_OUT_BATTLEFILED VS_BATTLEFILED(VS_IN In)
{
    VS_OUT_BATTLEFILED Out;
    
    float4x4 mTransformation = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4 vPosition = float4(In.vPosition, 1.f);

    float4x4 mIWV = mul(mTransformation, g_mView);
    float4x4 mIWVP = mul(mIWV, g_mProj);
	
    Out.vPosition = mul(vPosition, mIWVP);
    Out.vTexCoord = In.vTexCoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), mTransformation);
    Out.vProjPos = Out.vPosition;
    Out.vAnimatedTexCoord = In.vTexCoord + float2(0.f, g_fAccTime);
    Out.vScaleFactor = float2(length(In.vRight), length(In.vUp));
    
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

struct PS_IN_SAMPLING
{
    float4 vPosition : SV_POSITION;
};

struct PS_IN_BATTLEFILED
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    float2 vAnimatedTexCoord : TEXCOORD3;
    
    float2 vScaleFactor : TEXCOORD4;
};

struct PS_OUT
{
	float4	vDiffuse	: SV_TARGET0;
	float4	vAmbient	: SV_TARGET1;
	float4	vSpecular	: SV_TARGET2;
	float4	vEmissive	: SV_TARGET3;
	float4	vNormal		: SV_TARGET4;
	float4	vDepth		: SV_TARGET5;
    float4  vMask       : SV_TARGET6;
};

struct PS_OUT_SHADOWDEPTH
{
    float4 vShadowDepth : SV_TARGET0;
};

struct PS_OUT_OVERSIZING
{
    float4 vOutLineColor : SV_TARGET0;
};

struct PS_OUT_NEON
{
    float4 vColor : SV_TARGET0;
};

struct PS_OUT_BLOOM
{
    float4 vTexture : SV_TARGET0;
    float4 vData : SV_TARGET1;
};

struct PS_OUT_SAMPLING
{
    float4 vColor : SV_TARGET0;
};

struct PS_OUT_BATTLEFIELD
{
    float4 vColor : SV_Target0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    float3 vFinalColor;
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
    float4 vAmbient = vDiffuse;
    float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);

    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
    
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
    }
    
    //float4 vSpecular = float4(0.f, 0.f, 0.f, 1.f);
    //float vSpecularExist = g_vMtrlSpecular.x + g_vMtrlSpecular.y + g_vMtrlSpecular.z;
    //if (vSpecularExist > 0.f)
    //{
    //    float3 vToCam = normalize(g_vCamPosition - In.vProjPos);
    //    vSpecular = g_texReflection.Sample(LinearSampler, reflect(-vToCam, vNormal));
    //    vSpecular *= pow(abs(vSpecular.r + vSpecular.g + vSpecular.b) / 3.0, g_fMtrlShininess);
    //    vSpecular *= g_vMtrlSpecular;
    //}
	
    Out.vDiffuse = vDiffuse * g_vMtrlDiffuse;
    Out.vAmbient = vAmbient * g_vMtrlAmbient;
    Out.vSpecular = float4(0.f, 0.f, 0.f, 1.f);
    Out.vEmissive = vEmissive * g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT_SHADOWDEPTH PS_MAIN_SHADOWDEPTH(PS_IN_SHADOWDEPTH In)
{
    PS_OUT_SHADOWDEPTH Out;
	
    Out.vShadowDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, 0.f);
	
    return Out;
}

PS_OUT_OVERSIZING PS_MAIN_OVERSIZING(PS_IN_OVERSIZING In)
{
    PS_OUT_OVERSIZING Out;
	
    Out.vOutLineColor = vector(In.vProjPos.z / In.vProjPos.w, g_vOutlineColor.xyz);

    return Out;
}

PS_OUT_NEON PS_NEON(PS_IN_NEON In)
{
    PS_OUT_NEON Out;
    
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
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
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);

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
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);

    return Out;
}

PS_OUT PS_MAIN_CARGLASS(PS_IN In)
{
    PS_OUT Out;
	
    float3 vFinalColor;
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
    
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
    
    vTexColor.a = 0.1f;
    vSpecular.a = 0.1f;
	
    Out.vDiffuse = vTexColor * g_vMtrlDiffuse;
    Out.vAmbient = vTexColor * g_vMtrlAmbient;
    Out.vSpecular = vSpecular;
    Out.vEmissive = float4(0.f, 0.f, 0.f, 0.f);
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);

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
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    
    return Out;
}

PS_OUT PS_MAIN_FADEMATERIAL(PS_IN In)
{
    PS_OUT Out;
	
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
	
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
	
    Out.vDiffuse = (float4(1.f, 1.f, 1.f, 1.f) * g_fMaterialFadeRatio + vTexColor * (1.f - g_fMaterialFadeRatio)) * g_vMtrlDiffuse;
    Out.vAmbient = (float4(1.f, 1.f, 1.f, 1.f) * g_fMaterialFadeRatio + vTexColor * (1.f - g_fMaterialFadeRatio)) * g_vMtrlAmbient;
    Out.vSpecular = vSpecular;
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out;
	
    float3 vFinalColor;
    float4 vTexColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_fTilingDiffuse[0];
    
    Out.vTexture = vTexColor * g_vMtrlDiffuse;
    Out.vData = float4(0.f, 0.f, 0.f, g_fBloomStrength);

    return Out;
}

PS_OUT_SAMPLING PS_SAMPLING(PS_IN_SAMPLING In)
{
    PS_OUT_SAMPLING Out;
    
    Out.vColor = float4(1.f, 1.f, 1.f, 1.f);

    return Out;
}

PS_OUT_NEON PS_NEON_CRASH(PS_IN_NEON In)
{
    PS_OUT_NEON Out;
    
    if (In.vTexCoord.y < 0.9f)
    {
        Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    }
    else
    {
        Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    }

    return Out;
}

//float3 SchlickFresnel(float3 F0, float NdotH)
//{
//    return F0 + (1.0 - F0) * pow(2.0, (-5.55473 * NdotH - 6.98316) * NdotH);
//}

//float NdfGGX(float NdotH, float roughness)
//{
//    float alpha = roughness * roughness;
//    float alphaSq = alpha * alpha;
//    float denom = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;

//    return alphaSq / (PI * denom * denom);
//}

//// Single term for separable Schlick-GGX below.
//float SchlickG1(float NdotV, float k)
//{
//    return NdotV / (NdotV * (1.0 - k) + k);
//}

//float SchlickGGX(float NdotI, float NdotO, float roughness)
//{
//    float r = roughness + 1.0;
//    float k = (r * r) / 8.0;
//    return SchlickG1(NdotI, k) * SchlickG1(NdotO, k);
//}

//float3 SchlickFresnel(float3 F0, float cosTheta)
//{
//    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
//}

//float GGX(float3 N, float3 H, float alpha)
//{
//    float a2 = alpha * alpha;
//    float NdotH = max(0.001, dot(N, H));
//    float NdotH2 = NdotH * NdotH;

//    float nom = a2;
//    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
//    denom = PI * denom * denom;

//    return nom / denom;
//}

//float SmithGGXCorrelated(float3 N, float3 V, float Roughness)
//{
//    float NdotV = max(0.001, dot(N, V));
//    float a = Roughness * Roughness;
//    float k = (a + 1.0) * (a + 1.0) / 8.0;

//    return NdotV / (NdotV * (1.0 - k) + k);
//}

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

PS_OUT_BATTLEFIELD PS_BATTLEFIELD(PS_IN_BATTLEFILED In)
{
    PS_OUT_BATTLEFIELD Out;
    
    float2 vResolution;
    g_texDiffuse[0].GetDimensions(vResolution.x, vResolution.y);
    
    float2 vTexelSize = float2(1.f / vResolution.x, 1.f / vResolution.y);
    
    float fDistance = length(In.vWorldPos - g_vCamPosition);
    float fNormalizedDistance = saturate((fDistance - g_fMinDistance) / (g_fMaxDistance - g_fMinDistance));
    int iLevel = int(fNormalizedDistance * 7.0);
    
    float2 vTexCoord = float2(In.vTexCoord.x * 30.f * In.vScaleFactor.x, In.vTexCoord.y * 60.f * In.vScaleFactor.y);
    
    float vUINoise0 = g_texNormal[0].Sample(LinearSampler, In.vAnimatedTexCoord * In.vScaleFactor.xy).r;
    float vUINoise1 = g_texNormal[0].Sample(LinearSampler, In.vAnimatedTexCoord * 3.f * In.vScaleFactor.xy).r;
    
    float normalizedValue = (1.f - In.vTexCoord.y);
    iLevel = max(0, iLevel + int(normalizedValue * 7.0));
    if (vUINoise1 >= 0.5f)
    {
        iLevel += 1;
    }
    
    if (iLevel == 0)
        Out.vColor = g_texDiffuse[0].Sample(LinearSampler, vTexCoord);
    else if (iLevel == 1)
        Out.vColor = g_texDiffuse[1].Sample(LinearSampler, vTexCoord);
    else if (iLevel == 2)
        Out.vColor = g_texDiffuse[2].Sample(LinearSampler, vTexCoord);
    else if (iLevel == 3)
        Out.vColor = g_texDiffuse[3].Sample(LinearSampler, vTexCoord);
    else if (iLevel == 4)
        Out.vColor = g_texDiffuse[4].Sample(LinearSampler, vTexCoord);
    else if (iLevel == 5)
        Out.vColor = g_texDiffuse[5].Sample(LinearSampler, vTexCoord);
    else if (iLevel == 6)
        Out.vColor = g_texDiffuse[6].Sample(LinearSampler, vTexCoord);
    else
        Out.vColor = g_texDiffuse[7].Sample(LinearSampler, vTexCoord);

    float vUINoise2 = g_texNormal[0].Sample(LinearSampler, In.vAnimatedTexCoord * 15.f * In.vScaleFactor.xy).r;
    
    float fColorStrength = max(pow(In.vTexCoord.y - 0.01f, 40.f), 0.f);
    Out.vColor = float4(2.5f, fColorStrength, 0.f, Out.vColor.r * vUINoise0 * vUINoise1);

    float2 vNoiseTexCoord = In.vAnimatedTexCoord * In.vScaleFactor.xy;
    vNoiseTexCoord.x *= 2.f;
    vNoiseTexCoord.y *= 8.f;
    float4 vNoise = g_texDissolve[0].Sample(PointSampler, vNoiseTexCoord);
    float fNoiseStrength = max(pow(In.vTexCoord.y, 60.f), 0.f) * (1.f - fNormalizedDistance);
    if (vNoise.r >= 0.1f && fNoiseStrength >= 0.01f)
    {
        vNoise = float4(1.f * fNoiseStrength, 0.8f * fNoiseStrength, 0.1f * fNoiseStrength, fNoiseStrength);
        Out.vColor = Out.vColor.a * Out.vColor + (1.f - Out.vColor.a) * vNoise;
    }
    else if (vUINoise2 <= 0.3f)
        discard;
    
    return Out;
}

PS_OUT_NEON PS_CIRCULAR_SAMPLING(PS_IN In)
{
    PS_OUT_NEON Out;
	
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    if (vDiffuse.a <= 0.f)
        discard;

    float fTargetDistance = distance(In.vWorldPos.xyz, g_vTargetPosition);
    if (fTargetDistance > g_fSampleRadius)
        discard;
    
    float fNormalizedDistance = fTargetDistance / g_fSampleRadius;
    vDiffuse.a = (1.f - fNormalizedDistance);
    
    Out.vColor = vDiffuse * g_vMtrlDiffuse;

    return Out;
}

PS_OUT_NEON PS_SINGLE_COLOR(PS_IN In)
{
    PS_OUT_NEON Out;
	
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float3 vSingleColor = lerp(vDiffuse.rgb, g_vSingleColor, g_fColorFactor <= 1.f ? g_fColorFactor : (1.f - g_fColorFactor));
    
    Out.vColor = float4(vSingleColor, 1.f) * g_vMtrlDiffuse;

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
        vNormal = mul(vRecoveredNormal, WorldMatrix);
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

PS_OUT PS_DECAL_TARGET_DEPTH(PS_IN In)
{
    PS_OUT Out;
    
    Out.vDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    Out.vAmbient = float4(0.f, 0.f, 0.f, 0.f);
    Out.vSpecular = float4(0.f, 0.f, 0.f, 0.f);
    Out.vEmissive = float4(0.f, 0.f, 0.f, 0.f);
    Out.vNormal = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT PS_RADIAL_SPREAD(PS_IN In)
{
    PS_OUT Out;
    
    float fTargetDistance = distance(In.vWorldPos.xyz, g_vTargetPosition);
    if (fTargetDistance > g_fSampleRadius)
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
    
    if (fTargetDistance < g_fSampleMinRadius)
    {
        vector vRMA = g_texAmbient[0].Sample(LinearSampler, In.vTexCoord);
        float fRoughness = vRMA.r;
        float fMetallic = vRMA.g;
        float fAO = vRMA.b;
    
        float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
        float3 vDiffuseBRDF = vDiffuse.xyz * (1 / PI);
    
        float3 vLightDirection = normalize(g_vLightDirection);
        float3 vViewDirection = normalize(g_vCamPosition - In.vProjPos);
        float3 vSpecularBRDF = SpecularBRDF(g_vMtrlSpecular.rgb, vNormal, vViewDirection, vLightDirection, fRoughness, fMetallic, fAO);
    
        float fRatio = pow(fTargetDistance / g_fSampleMinRadius, 3.f);
        
        Out.vDiffuse = float4((vDiffuseBRDF) * fAO, 1.f) * g_vMtrlDiffuse;
        Out.vAmbient = float4(vDiffuseBRDF, 1.f) * g_vMtrlAmbient;
        Out.vSpecular = float4(vSpecularBRDF * g_fMtrlShininess, 1.f);
        
        Out.vDiffuse = lerp(Out.vDiffuse, float4(0.6f, 0.027f, 0.211f, 1.f), fRatio);
        Out.vAmbient = lerp(Out.vAmbient, float4(0.6f, 0.027f, 0.211f, 1.f), fRatio);
    }
    else
    {
        Out.vDiffuse = float4(0.6f, 0.027f, 0.211f, 1.f) * g_vMtrlDiffuse;
        Out.vAmbient = float4(0.6f, 0.027f, 0.211f, 1.f) * g_vMtrlAmbient;
        Out.vSpecular = float4(0.f, 0.f, 0.f, 1.f);
    }
    
    Out.vEmissive = g_vMtrlEmissive;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT PS_SHADOW_DECAL_TARGET(PS_IN In)
{
    PS_OUT Out;

    float3 vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
    
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
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
	pass Default // 0
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

        SetRasterizerState(RS_Default);
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
        SetDepthStencilState(DSS_Default, 0);
    }

    pass NEON // 4 
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

    pass MESH_FADEMATERIAL // 11
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FADEMATERIAL();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLOOM // 12
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

    pass SAMPLING // 13
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

    pass BATTLEFIELD // 17
    {
        VertexShader = compile vs_5_0 VS_BATTLEFILED();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BATTLEFIELD();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }
    pass NEON_WALL // 18
    {
        VertexShader = compile vs_5_0 VS_NEON_WALL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NEON();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Neon, 0);
    }
    pass NEON_WARNING // 19
    {
        VertexShader = compile vs_5_0 VS_NEON_WARNING();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NEON();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Neon, 0);
    }
    pass MINDROOM_WATER // 20
    {
        VertexShader = compile vs_5_0 VS_MINDROOM_WATER();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CAR();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass CIRCULAR_SAMPLING // 21
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_CIRCULAR_SAMPLING();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass SINGLE_COLOR // 22
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SINGLE_COLOR();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass DECAL_TARGET // 23
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

    pass DECAL_TARGET_DEPTH // 24
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DECAL_TARGET_DEPTH();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass READIAL_SPREAD // 25
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RADIAL_SPREAD();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass SHADOW_DECAL_TARGET // 26
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SHADOW_DECAL_TARGET();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
}
