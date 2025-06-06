#include "ShaderCommon.hlsli"

struct VS_IN
{
	float3 vPosition	: POSITION;
	float2 vTexCoord	: TEXCOORD0;
	
	float4 vRight		: TEXCOORD1;
	float4 vUp			: TEXCOORD2;
	float4 vLook		: TEXCOORD3;
	float4 vTranslation	: TEXCOORD4;
	float4 vArgument0	: TEXCOORD5;
	float4 vArgument1	: TEXCOORD6;
};

struct VS_OUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
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

	return Out;
}

struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
};

struct PS_OUT_POSTPROCESS
{
	float4 vPreProcess	: SV_TARGET0;
	float4 vProcessData	: SV_TARGET1;
	float4 vMask		: SV_TARGET2;
};

PS_OUT_POSTPROCESS PS_MAIN(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;

    Out.vPreProcess		= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vProcessData	= float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			= float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;

    Out.vPreProcess		= g_texDiffuse[0].Sample(PointSampler, In.vTexCoord) * g_vMtrlDiffuse;
    Out.vProcessData	= float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			= float4(0.f, 0.f, 0.f, 0.f);
	
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
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Gizmo
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_POINT();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
}
