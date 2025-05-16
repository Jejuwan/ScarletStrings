#include "ShaderCommon.hlsli"

TextureCube g_texCubeDiffuse;

bool		g_bCloud	= false;

float3 g_vColorA;
float3 g_vColorB;
float g_fLerpFactor;

struct VS_IN
{
	float3	vPosition	: POSITION;
	float3	vTexCoord	: TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
    float3 vWorldPosition : TEXCOORD0;
};

struct VS_OUT_GRADATION
{
    float4 vPosition : SV_POSITION;
    float4 vProjPosition : TEXCOORD0;
};

struct VS_OUT_GRADATION_CLEAR
{
    float4 vPosition : SV_POSITION;
    float3 vWorldPosition : TEXCOORD0;
    float4 vProjPosition : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	matrix	mWV, mWVP;

	mWV				= mul(g_mWorld, g_mView);
	mWVP			= mul(mWV, g_mProj);

    Out.vPosition = mul(vector(In.vPosition, 0.f), g_mView);
    Out.vPosition = mul(vector(Out.vPosition.xyz, 1.f), g_mProj);
    Out.vWorldPosition = In.vPosition;

	return Out;
}

VS_OUT_GRADATION VS_GRADATION(VS_IN In)
{
    VS_OUT_GRADATION Out;

    matrix mWV, mWVP;

    mWV = mul(g_mWorld, g_mView);
    mWVP = mul(mWV, g_mProj);

    Out.vPosition = mul(vector(In.vPosition, 1.f), mWVP);
    Out.vProjPosition = Out.vPosition;

    return Out;
}

VS_OUT_GRADATION_CLEAR VS_GRADATION_CLEAR(VS_IN In)
{
    VS_OUT_GRADATION_CLEAR Out;

    matrix mWV, mWVP;

    mWV = mul(g_mWorld, g_mView);
    mWVP = mul(mWV, g_mProj);

    Out.vPosition = mul(vector(In.vPosition, 1.f), mWVP);
    
    vector vSamplePosition = mul(vector(In.vPosition, 0.f), g_mView);
    vSamplePosition = mul(vector(vSamplePosition.xyz, 1.f), g_mProj);
    Out.vWorldPosition = vSamplePosition;
    
    Out.vProjPosition = Out.vPosition;

    return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
    float3 vWorldPosition : TEXCOORD0;
};

struct PS_IN_GRADATION
{
    float4 vPosition : SV_POSITION;
    float4 vProjPosition : TEXCOORD0;
};

struct PS_IN_GRADATION_CLEAR
{
    float4 vPosition : SV_POSITION;
    float3 vWorldPosition : TEXCOORD0;
    float4 vProjPosition : TEXCOORD1;
};

struct PS_OUT
{
	float4	vColor		: SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out;
	
    Out.vColor = g_texCubeDiffuse.Sample(LinearSampler, In.vWorldPosition.xyz);
	
	return Out;
}

PS_OUT PS_LERP_COLOR(PS_IN In)
{
    PS_OUT Out;
	
    Out.vColor = float4(lerp(g_vColorA, g_vColorB, g_fLerpFactor), 1.f);
	
    return Out;
}

PS_OUT PS_GRADATION(PS_IN_GRADATION In)
{
    PS_OUT Out;
    
    float2 vTexUV;
    vTexUV.x = (In.vProjPosition.x / In.vProjPosition.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPosition.y / In.vProjPosition.w) * -0.5f + 0.5f;
	
    float r = g_vColorA.x + vTexUV.y * (g_vColorB.x - g_vColorA.x);
    float g = g_vColorA.y + vTexUV.y * (g_vColorB.y - g_vColorA.y);
    float b = g_vColorA.z + vTexUV.y * (g_vColorB.z - g_vColorA.z);
    
    Out.vColor = float4(r, g, b, 1.f);
    
    return Out;
}

PS_OUT PS_GRADATION_CLEAR(PS_IN_GRADATION_CLEAR In)
{
    PS_OUT Out;
    
    float2 vTexUV;
    vTexUV.x = (In.vProjPosition.x / In.vProjPosition.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vProjPosition.y / In.vProjPosition.w) * -0.5f + 0.5f;
    
    float4 vOriginColor = g_texCubeDiffuse.Sample(LinearSampler, In.vWorldPosition.xyz);
	
    float r = g_vColorA.x + (vTexUV.y + g_fLerpFactor) * (vOriginColor.x - g_vColorA.x);
    float g = g_vColorA.y + (vTexUV.y + g_fLerpFactor) * (vOriginColor.y - g_vColorA.y);
    float b = g_vColorA.z + (vTexUV.y + g_fLerpFactor) * (vOriginColor.z - g_vColorA.z);
    
    Out.vColor = float4(r, g, b, 1.f);
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Sky
    {
        VertexShader	= compile vs_5_0 VS_MAIN();
        GeometryShader	= NULL;
        HullShader		= NULL;
        DomainShader	= NULL;
        PixelShader		= compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_InverseCull);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass LerpColor
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LERP_COLOR();

        SetRasterizerState(RS_InverseCull);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass GRADATION
    {
        VertexShader = compile vs_5_0 VS_GRADATION();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_GRADATION();

        SetRasterizerState(RS_InverseCull);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass GRADATION_CLEAR
    {
        VertexShader = compile vs_5_0 VS_GRADATION_CLEAR();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_GRADATION_CLEAR();

        SetRasterizerState(RS_InverseCull);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }
}
