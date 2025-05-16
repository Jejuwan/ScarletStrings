#include "ShaderCommon.hlsli"

float g_fTilingFactor;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
};

struct VS_OUT_WORLD
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct VS_OUT_ORTHOGRAPHIC
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float2 vLocalPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    float4x4 mWV = mul(g_mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), mWVP);
    Out.vTexCoord = In.vTexCoord;

    return Out;
}

VS_OUT_WORLD VS_WORLD(VS_IN In)
{
    VS_OUT_WORLD Out;

    float4x4 mWV = mul(g_mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), mWVP);
    Out.vTexCoord = In.vTexCoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_mWorld);

    return Out;
}

VS_OUT_ORTHOGRAPHIC VS_ORTHOGRAPHIC(VS_IN In)
{
    VS_OUT_ORTHOGRAPHIC Out;
	
    Out.vPosition = mul(float4(In.vPosition, 1.f), mul(g_mWorld, g_mOrthographic));
    Out.vTexCoord = In.vTexCoord;
    Out.vLocalPos = In.vPosition.xy;

    return Out;
}

VS_OUT VS_BILLBOARD(VS_IN In)
{
    VS_OUT Out;
	
    float3 vViewDir = normalize(g_mWorld[3].xyz - g_vCamPosition.xyz);
    float fRightScaleFactor = length(g_mWorld[0]);
    float fUpScaleFactor = length(g_mWorld[1]);
    float fLookScaleFactor = length(g_mWorld[2]);
	
    float4 vRight = float4(cross(float3(0.f, 1.f, 0.f), vViewDir), 0.f);
    float4 vUp = float4(cross(vViewDir, vRight.xyz), 0.f);
    float4 vLook = float4(vViewDir, 0.f);
	
    float4x4 mWorld = float4x4(vRight * fRightScaleFactor, vUp * fUpScaleFactor, vLook * fLookScaleFactor, g_mWorld[3]);
	
    float4x4 mWV = mul(mWorld, g_mView);
    float4x4 mWVP = mul(mWV, g_mProj);
	
    Out.vPosition = mul(float4(In.vPosition, 1.f), mWVP);
    Out.vTexCoord = In.vTexCoord;
	
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
};

struct PS_IN_OVERSIZING
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_IN_WORLD
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_IN_ORTHOGRAPHIC
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float2 vLocalPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
	
    return Out;
}

PS_OUT PS_LINE_STRIPE(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
    Out.vColor.a = 1.f;
    Out.vColor *= (0.5f + g_texMask.Sample(LinearSampler, In.vTexCoord * g_fTilingFactor).a);
	
    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass Orthographic
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass Billboard
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass Orthographic_Line_Stripe
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LINE_STRIPE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }
}
