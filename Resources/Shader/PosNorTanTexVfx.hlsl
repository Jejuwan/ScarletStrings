#include "ShaderCommon.hlsli"

float		g_fMaterialFadeRatio	= 0.f;
float		g_fDistortionPower		= 0.1f;

bool		g_bMaskAlpha			= false;
bool		g_bExtraMaskAlpha		= false;
bool		g_bReverseGrayScale0	= false;
bool		g_bReverseGrayScale1	= false;

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

struct PS_IN
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: NORMAL;
	float4	vTangent		: TANGENT;
	float2	vTexCoord		: TEXCOORD0;
	float4	vWorldPos		: TEXCOORD1;
	float4	vProjPos		: TEXCOORD2;
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

struct PS_OUT_POSTPROCESS
{
	float4 vPreProcess		: SV_TARGET0;
	float4 vProcessData		: SV_TARGET1;
	float4 vMask			: SV_TARGET2;
};

struct PS_OUT_BLOOM
{
	float4  vTexture		: SV_TARGET0;
	float4  vData			: SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out;
	
	float3		vFinalColor;
	float4		vDiffuse		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	float4      vAmbient        =   vDiffuse;
	float4      vSpecular       =   float4(1.f, 1.f, 1.f, 1.f);
	float4      vEmissive       =   float4(1.f, 1.f, 1.f, 1.f);
	
	// Normal
	float3 vNormal = In.vNormal.xyz;
	
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		float2 vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord).xy;
		float vNormalZ = sqrt(1 - vNormalTex.x * vNormalTex.x - vNormalTex.y * vNormalTex.y);
		float3 vRecoveredNormal = normalize(float3(vNormalTex.xy, vNormalZ) * 2.f - 1.f);
		
		float3x3 mTBN = float3x3(In.vTangent.xyz, normalize(cross(vRecoveredNormal, In.vTangent.xyz)), vRecoveredNormal);
		vNormal = normalize(mul(vRecoveredNormal, mTBN));
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
        vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord);
    }
	
	Out.vDiffuse	= vDiffuse * g_vMtrlDiffuse;
	Out.vAmbient	= vAmbient * g_vMtrlAmbient;

	//Out.vAmbient = vTexColor;
	Out.vSpecular	= vSpecular * g_vMtrlSpecular;
	Out.vEmissive	= vEmissive * g_vMtrlEmissive;
	Out.vNormal     = float4(vNormal, 0.f) * 0.5f + 0.5f;
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 1.f, In.vProjPos.z);
	Out.vMask		= float4(0.f, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In)
{
	PS_OUT_BLOOM Out;
	
	Out.vTexture =	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset) * g_vMtrlDiffuse;
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive * g_vUVOffset1) * g_vMtrlEmissive;
		Out.vTexture.rgb += vEmissive.rgb * vEmissive.a;
	}
	Out.vTexture.a *= g_fFadeRatio;
//	Out.vData = float4(1.f, 1.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a = g_fBloomStrength;
	Out.vData = float4(g_fBloomStrength, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
		vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        if (g_bMaskAlpha)
        {
            Out.vPreProcess.a *= g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2);
        }
		else
        {
            Out.vPreProcess *= g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2);
        }
    }
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_ALPHAMASK(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse		= float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive	= float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
		vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
		if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_EXTRAMASK_REGULAR(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
		vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        if (g_bMaskAlpha)
        {
            Out.vPreProcess.a *= g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2);
        }
		else
        {
            Out.vPreProcess *= g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2);
        }
    }
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        if (g_bExtraMaskAlpha)
        {
            Out.vPreProcess.a *= g_texExtraMask.Sample(LinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3);
        }
		else
        {
            Out.vPreProcess *= g_texExtraMask.Sample(LinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3);
        }
    }
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_EXTRAMASK_REGULAR_ALPHA(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
		vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        if (g_bMaskAlpha)
        {
            Out.vPreProcess.a *= g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2);
        }
		else
        {
            Out.vPreProcess *= g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2);
        }
    }
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        float fGrayScale = AvgGrayScale(g_texExtraMask.Sample(LinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);;
		if (g_bReverseGrayScale1)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_EXTRAMASK_ALPHA(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
		vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
		if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        float fGrayScale = AvgGrayScale(g_texExtraMask.Sample(LinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);;
		if (g_bReverseGrayScale1)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_EXTRAMASK_ALPHA_CLAMP(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
		vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(ClampLinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
		if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        float fGrayScale = AvgGrayScale(g_texExtraMask.Sample(ClampLinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);;
		if (g_bReverseGrayScale1)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_EXTRAMASK_ALPHA_CLAMP(PS_IN In)
{
    PS_OUT_BLOOM Out;

	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
		vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
    Out.vTexture = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
    Out.vTexture.a *= g_fFadeRatio;
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(ClampLinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
		if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vTexture.a *= fGrayScale;
    }
	
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        float fGrayScale = AvgGrayScale(g_texExtraMask.Sample(ClampLinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);;
		if (g_bReverseGrayScale1)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vTexture.a *= fGrayScale;
    }
	
    Out.vData = float4(g_fBloomStrength, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_EXTRAMASK_ALPHA_WUCVDIFFUSE(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
        vDiffuse = g_texDiffuse[0].Sample(LinearUClampVLinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
    }
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
		if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        float fGrayScale = AvgGrayScale(g_texExtraMask.Sample(LinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);;
		if (g_bReverseGrayScale1)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_EXTRAMASK_ALPHA_CLAMP_WUCVDIFFUSE(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
        vDiffuse = g_texDiffuse[0].Sample(LinearUClampVLinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
    }
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(ClampLinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
		if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        float fGrayScale = AvgGrayScale(g_texExtraMask.Sample(ClampLinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);;
		if (g_bReverseGrayScale1)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_EXTRAMASK_ALPHA_COMBINE(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
		vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	}
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    float fGrayA = AvgGrayScale(g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
    float fGrayB = AvgGrayScale(g_texExtraMask.Sample(LinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);
	
	if (g_bReverseGrayScale0)
    {
        fGrayA = 1.f - fGrayA;
    }
	if (g_bReverseGrayScale1)
    {
        fGrayB = 1.f - fGrayB;
    }
	
    float fMaskA = Out.vPreProcess.a * fGrayA;
    float fMaskB = Out.vPreProcess.a * fGrayB;
	
    Out.vPreProcess.a *= max(fMaskA, fMaskB);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_EXTRAMASK_ALPHA_COMBINE_CLAMP(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
	float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
	if (g_iShaderFlag & STATUS_TEXDIFFUSE)
	{
        vDiffuse = g_texDiffuse[0].Sample(ClampLinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
    }
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
        vEmissive = g_texEmission[0].Sample(ClampLinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
	Out.vPreProcess.a *= g_fFadeRatio;
	
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    float fGrayA = AvgGrayScale(g_texMask.Sample(ClampLinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
    float fGrayB = AvgGrayScale(g_texExtraMask.Sample(ClampLinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);
	
	if (g_bReverseGrayScale0)
    {
        fGrayA = 1.f - fGrayA;
    }
	if (g_bReverseGrayScale1)
    {
        fGrayB = 1.f - fGrayB;
    }
	
    float fMaskA = Out.vPreProcess.a * fGrayA;
    float fMaskB = Out.vPreProcess.a * fGrayB;
	
    Out.vPreProcess.a *= max(fMaskA, fMaskB);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BLEND_DISTORTION(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
    Out.vPreProcess = float4(0.f, 0.f, 0.f, 0.f);
	Out.vProcessData	=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
	Out.vMask			=	float4(In.vTexCoord.xy, g_fDistortionPower * g_fFadeRatio, 0.1f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_GRAYSCALE(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;
	
    float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
    float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
    if (g_iShaderFlag & STATUS_TEXDIFFUSE)
    {
        vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
    }
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
    Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
    Out.vPreProcess.a = AvgGrayScale(Out.vPreProcess.rgb);
    Out.vPreProcess.a *= g_fFadeRatio;
	
    Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
    Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);;
        if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess *= fGrayScale;
    }
	
    return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_EXTRAMASK_GRAYSCALE(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;
	
    float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
    float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
    if (g_iShaderFlag & STATUS_TEXDIFFUSE)
    {
        vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
    }
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
    Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
    Out.vPreProcess.a = AvgGrayScale(Out.vPreProcess.rgb);
    Out.vPreProcess.a *= g_fFadeRatio;
	
    Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
    Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb);
        if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess *= fGrayScale;
    }
	
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        float fGrayScale = AvgGrayScale(g_texExtraMask.Sample(LinearUClampVLinearSampler , In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);
        if (g_bReverseGrayScale1)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess *= fGrayScale;
    }
	
    return Out;
}

PS_OUT_BLOOM PS_MAIN_ADAPTIVE_BLOOM(PS_IN In)
{
	PS_OUT_BLOOM Out;
	
	Out.vTexture =	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset) * g_vMtrlDiffuse;
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1) * g_vMtrlEmissive;
		Out.vTexture.rgb += vEmissive.rgb * vEmissive.a;
	}
	Out.vTexture.a *= g_fFadeRatio;
    float fGrayScale = AvgGrayScale(Out.vTexture.rgb);
	Out.vData = float4(fGrayScale * g_fBloomStrength, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_ADAPTIVE_DISTORTION(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
    Out.vPreProcess		= float4(0.f, 0.f, 0.f, 0.f);
	Out.vProcessData	=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
    float fGrayScale	=	AvgGrayScale(Out.vProcessData.rgb);
	Out.vMask			=	float4(In.vTexCoord.xy, g_fDistortionPower * g_fFadeRatio * fGrayScale, 0.1f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BEAM(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;
	
    float4 vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
    float4 vEmissive = float4(1.f, 1.f, 1.f, 1.f);
	
    if (g_iShaderFlag & STATUS_TEXDIFFUSE)
    {
        vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
    }
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord * g_vTilingEmissive + g_vUVOffset1);
    }
	
    Out.vPreProcess = vDiffuse * g_vMtrlDiffuse + float4(vEmissive.rgb * g_vMtrlEmissive.rgb * g_vMtrlEmissive.a, 0.f);
    Out.vPreProcess.a *= g_fFadeRatio;
	
    float fGrayScale = AvgGrayScale(Out.vPreProcess.rgb);
    Out.vProcessData = float4(fGrayScale * g_fBloomStrength, 0.f, 0.f, 0.f);
    Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb) * g_vMtrlSpecular;
        if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess.a *= fGrayScale;
    }
	
    if (g_iShaderFlag & STATUS_TEXSPECULAR)
    {
        float fGrayScale = AvgGrayScale(g_texExtraMask.Sample(LinearSampler, In.vTexCoord * g_vTilingSpecular + g_vUVOffset3).rgb);
        if (g_bReverseGrayScale1)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vPreProcess *= fGrayScale;
    }
	
    return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_BEAM_DISTORTION(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;
	
    Out.vPreProcess = float4(0.f, 0.f, 0.f, 0.f);
    Out.vProcessData = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_vTilingDiffuse + g_vUVOffset);
    Out.vMask = float4(In.vTexCoord.xy, g_fDistortionPower * g_fFadeRatio, 0.1f);
	
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        float fGrayScale = AvgGrayScale(g_texMask.Sample(LinearSampler, In.vTexCoord * g_vTilingNormal + g_vUVOffset2).rgb) * g_vMtrlSpecular;
        if (g_bReverseGrayScale0)
        {
            fGrayScale = 1.f - fGrayScale;
        }
        Out.vMask.y *= fGrayScale;
    }
	
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

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass BLOOM //1
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	pass BLEND //2
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLEND();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass BLEND_DISTORTION //3
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLEND_DISTORTION();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

    pass BLEND_GRAYSCALE //4
    {
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
        PixelShader		= compile ps_5_0 PS_MAIN_GRAYSCALE();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass ADAPTIVE_BLOOM //5
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ADAPTIVE_BLOOM();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass ADAPTIVE_DISTORTION //6
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ADAPTIVE_DISTORTION();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass BLEND_ALPHAMASK //7
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_ALPHAMASK();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_REGULAR //8
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_EXTRAMASK_REGULAR();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_REGULAR_ALPHA //9
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_EXTRAMASK_REGULAR_ALPHA();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_ALPHA //10
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_EXTRAMASK_ALPHA();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_ALPHA_CLAMP //11
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_EXTRAMASK_ALPHA_CLAMP();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_ALPHA_COMBINE //12
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_EXTRAMASK_ALPHA_COMBINE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_ALPHA_COMBINE_CLAMP //13
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_EXTRAMASK_ALPHA_COMBINE_CLAMP();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_ALPHA_WUCVDIFFUSE //14
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_EXTRAMASK_ALPHA_WUCVDIFFUSE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_ALPHA_CLAMP_WUCVDIFFUSE //15
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_EXTRAMASK_ALPHA_CLAMP_WUCVDIFFUSE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLOOM_EXTRAMASK_ALPHA_CLAMP //16
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_EXTRAMASK_ALPHA_CLAMP();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLEND_EXTRAMASK_GRAYSCALE //17
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EXTRAMASK_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass BEAM //18
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BEAM();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass BEAM_DISTORTION //19
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BEAM_DISTORTION();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
}
