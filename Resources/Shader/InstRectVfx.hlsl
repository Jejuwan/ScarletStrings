#include "ShaderCommon.hlsli"

struct VS_IN
{
	float3 vPosition	: POSITION;
	float2 vTexCoord	: TEXCOORD0;
	
	float4 vRight		: TEXCOORD1;
	float4 vUp			: TEXCOORD2;
	float4 vLook		: TEXCOORD3;
	float4 vTranslation	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
	float4 vArgument	: TEXCOORD6;
};

struct VS_OUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
	float4 vArgument	: TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out;
	
	float4x4			mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4				vPosition		= mul(float4(In.vPosition, 1.f), mTransformation);
	matrix				mWV, mWVP;

	mWV					= mul(g_mWorld, g_mView);
	mWVP				= mul(mWV, g_mProj);

    Out.vPosition		= mul(vPosition, mWVP);
	Out.vTexCoord		= In.vTexCoord;
	Out.vColor	        = In.vColor;
	Out.vArgument		= In.vArgument;

	return Out;
}

struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
	float4 vArgument	: TEXCOORD2;
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
    float4 vData : SV_TARGET1;
};

PS_OUT_POSTPROCESS PS_MAIN(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;

	Out.vPreProcess		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        float4 vEmissive = Out.vPreProcess = g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
		Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
	Out.vPreProcess *= In.vColor;
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out;

    Out.vTexture		= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
        Out.vTexture.rgb += vEmissive.rgb * vEmissive.a;
    }
    Out.vTexture *= In.vColor;
//	Out.vData			= float4(1.f, 1.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a		= g_fBloomStrength * In.vColor.a;
	Out.vData			= float4(g_fBloomStrength * In.vColor.a, 0.f, 0.f, 0.f);
	
    return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

    pass Trail
    {
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }
}
