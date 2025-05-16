#include "ShaderCommon.hlsli"

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
	float4 vColor		: TEXCOORD5;
	float4 vArgument	: TEXCOORD6;
};

struct VS_OUT
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

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

//	float4x4	mTransformation;
//	mTransformation[0]			= In.vRight;
//	mTransformation[1]			= In.vUp;
//	mTransformation[2]			= In.vLook;
//	mTransformation[3]			= In.vTranslation;
	float4x4	mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    float4		vPosition		= float4(In.vPosition, 1.f);

	float4x4	mIW				= mul(mTransformation, g_mWorld);
	float4x4	mIWV			= mul(mIW, g_mView);
	float4x4	mIWVP			= mul(mIWV, g_mProj);
	
	Out.vPosition	= mul(vPosition, mIWVP);
	Out.vNormal		= normalize(mul(float4(In.vNormal, 0.f), mIW));
	Out.vTangent	= normalize(mul(float4(In.vTangent, 0.f), mIW));
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= mul(float4(In.vPosition, 1.f), mIW);
	Out.vProjPos    = Out.vPosition;
	Out.vColor		= In.vColor;
	Out.vArgument	= In.vArgument;

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

struct PS_OUT
{
	float4	vDiffuse	: SV_TARGET0;
	float4	vAmbient	: SV_TARGET1;
	float4	vSpecular	: SV_TARGET2;
	float4	vEmissive	: SV_TARGET3;
	float4	vNormal		: SV_TARGET4;
	float4	vDepth		: SV_TARGET5;
	float4	vMask		: SV_TARGET6;
};

struct PS_OUT_BLOOM
{
    float4 vTexture : SV_TARGET0;
    float4 vData : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out;
	
	float3		vFinalColor;
	float4		vTexColor		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
    float3		vNormal = In.vNormal.xyz;
    if (g_iShaderFlag & STATUS_TEXNORMAL)
    {
        vector vNormalTex = g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
        vNormalTex.z = sqrt(1 - saturate(dot(vNormalTex.xy, vNormalTex.xy)));
        float3 vRecoveredNormal = vNormalTex.xyz * 2.f - 1.f;
	
        float3x3 WorldMatrix = float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
        vNormal = mul(vRecoveredNormal, WorldMatrix);
    }
	
    Out.vDiffuse	= vTexColor * g_vMtrlDiffuse;
	Out.vAmbient	= vTexColor * g_vMtrlAmbient;
	Out.vSpecular	= g_vMtrlSpecular;
	Out.vEmissive	= g_vMtrlEmissive;
	Out.vNormal		= float4(vNormal, 0.f) * 0.5f + 0.5f;
    Out.vDepth		= float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
	Out.vMask		= float4(0.f, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out;
	
    Out.vTexture	= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
        Out.vTexture.rgb += vEmissive.rgb * vEmissive.a;
    }
//	Out.vData = float4(1.f, 1.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a = g_fBloomStrength;
    Out.vData = float4(g_fBloomStrength, 0.f, 0.f, 0.f);
	
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

	pass Bloom
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	pass Bloom_DepthReadOnly
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }
}
