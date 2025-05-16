#include "ShaderCommon.hlsli"

float4x4			g_mQuadWorld;
float4x4			g_mQuadView;
float4x4			g_mQuadProj;

Texture2D g_texNormalTarget;
Texture2D g_texDepthTarget;
Texture2D g_texBlurTarget;

// SSAO
float g_fRadius							= 0.001f;
float g_fStrength						= 1.0f;

float3 g_vRandom[16] =
{
    float3(0.2024537f, 0.841204f, -0.9060141f),
    float3(-0.2200423f, 0.6282339f, -0.8275437f),
    float3(0.3677659f, 0.1086345f, -0.4466777f),
    float3(0.8775856f, 0.4617546f, -0.6427765f),
    float3(0.7867433f, -0.141479f, -0.1567597f),
    float3(0.4839356f, -0.8253108f, -0.1563844f),
    float3(0.4401554f, -0.4228428f, -0.3300118f),
    float3(0.0019193f, -0.8048455f, 0.0726584f),
    float3(-0.7578573f, -0.5583301f, 0.2347527f),
    float3(-0.4540417f, -0.252365f, 0.0694318f),
    float3(-0.0483353f, -0.2527294f, 0.5924745f),
    float3(-0.4192392f, 0.2084218f, -0.3672943f),
    float3(-0.8433938f, 0.1451271f, 0.2202872f),
    float3(-0.4037157f, -0.8263387f, 0.4698132f),
    float3(-0.6657394f, 0.6298575f, 0.6342437f),
    float3(-0.0001783f, 0.2834622f, 0.8343929f),
};

// Blur

float BlurWeights[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
	1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};
float fTotal = 6.2108;

struct VS_IN
{
	float3	vPosition		: POSITION;
	float2	vTexCoord		: TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition		: SV_POSITION;
	float2	vTexCoord		: TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	matrix	mWV, mWVP;

	mWV				= mul(g_mQuadWorld, g_mQuadView);
	mWVP			= mul(mWV, g_mQuadProj);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vTexCoord	= In.vTexCoord;

	return Out;
}

struct PS_IN
{
	float4	vPosition		: SV_POSITION;
	float2	vTexCoord		: TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor			: SV_TARGET0;
};

struct tagSSAO_In
{
    float2 vUV;
    float fDepth;
    float fViewZ;
    float3 vNormal;
};

float3 RandomNormal(float2 vTexCoord)
{
    float noiseX = (frac(sin(dot(vTexCoord, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
    float noiseY = (frac(sin(dot(vTexCoord, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
    float noiseZ = (frac(sin(dot(vTexCoord, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));
    return normalize(float3(noiseX, noiseY, noiseZ));
}

float4 Get_SSAO(tagSSAO_In In)
{
    float4 vAmbient;

    float3 vRay;
    float3 vReflect;
    float2 vRandomUV;
    float fOccNorm;
 
    uint iColor = 0;
 
    for (uint i = 0; i < 16; ++i)
    {
        vRay = reflect(RandomNormal(In.vUV), g_vRandom[i]);
        vReflect = normalize(reflect(vRay, In.vNormal)) * g_fRadius;
        vReflect.x *= -1.f;
        vRandomUV = In.vUV + vReflect.xy;
        fOccNorm = g_texDepthTarget.Sample(TargetSampler, vRandomUV).y;
 
        if (fOccNorm <= In.fDepth + 0.0003f)
            ++iColor;
    }
 
    vAmbient = (1.f - abs((iColor / 16.f) - 1));
    vAmbient.a = 1.f;
	
    return vAmbient;
}

PS_OUT PS_MAIN_BlurX(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTextureSize;
    g_texBlurTarget.GetDimensions(vTextureSize.x, vTextureSize.y);

    float fStepX = 1.f / (vTextureSize.x / 2.f);
    float2 vBlurTexCoord = float2(0.f, 0.f);
    for (int i = -6; i < 6; ++i)
    {
        vBlurTexCoord = In.vTexCoord + float2(fStepX * i, 0);
        Out.vColor += BlurWeights[6 + i] * g_texBlurTarget.Sample(LinearSampler, vBlurTexCoord);
    }

    Out.vColor /= fTotal;

    return Out;
}

PS_OUT PS_MAIN_BlurY(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 vTextureSize;
    g_texBlurTarget.GetDimensions(vTextureSize.x, vTextureSize.y);
    
    float fStepY = 1.f / (vTextureSize.y / 2.f);
    float2 vBlurTexCoord = float2(0.f, 0.f);
    for (int i = -6; i < 6; ++i)
    {
        vBlurTexCoord = In.vTexCoord + float2(0, fStepY * i);
        Out.vColor += BlurWeights[6 + i] * g_texBlurTarget.Sample(LinearSampler, vBlurTexCoord);
    }
	
    Out.vColor /= fTotal;

    return Out;
}

PS_OUT PS_MAIN_SSAO(PS_IN In)
{
    PS_OUT Out;
	
	float4			vNormalTarget	= g_texNormalTarget.Sample(TargetSampler, In.vTexCoord);
	float4			vNormal			= vNormalTarget * 2.f - 1.f;
	
	float4			vDepthTarget	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
	float			fViewZ			= vDepthTarget.y;
	
	// SSAO
    tagSSAO_In SSAO_In = (tagSSAO_In) 0;
    SSAO_In.vUV = In.vTexCoord;
    SSAO_In.vNormal = vNormal.rgb;
    SSAO_In.fViewZ = fViewZ;
    SSAO_In.fDepth = vDepthTarget.y;
	
    Out.vColor = saturate(g_fStrength * Get_SSAO(SSAO_In));

	return Out;
}

technique11 DefaultTechnique
{
    pass BlurX
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BlurX();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass BlurY
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BlurY();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

	pass SSAO
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_SSAO();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}
}