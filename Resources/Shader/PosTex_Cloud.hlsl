#include "ShaderCommon.hlsli"

float fTime = 0.f;

struct VS_IN
{
	float3	vPosition	: POSITION;
	float2	vTexCoord	: TEXCOORD0;
};

struct VS_OUT_PERLIN_CLOUD
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
};

VS_OUT_PERLIN_CLOUD VS_PERLIN_CLOUD(VS_IN In)
{
    VS_OUT_PERLIN_CLOUD Out;

    matrix mWV, mWVP;

    mWV = mul(g_mWorld, g_mView);
    mWVP = mul(mWV, g_mProj);

    Out.vPosition = mul(vector(In.vPosition, 1.f), mWVP);
    Out.vTexCoord = In.vTexCoord;
    
    return Out;
}

struct PS_IN_PERLIN_CLOUD
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor		: SV_TARGET0;
};

PS_OUT PS_PERLIN_CLOUD(PS_IN_PERLIN_CLOUD In)
{
    PS_OUT Out;
    
    float2 vScaledTexCoord = (In.vTexCoord * 2.f) - 0.5f - (fTime * 0.005f);
    float3 vColorCoefficients = cos(float3(fTime * 0.006f, fTime * 0.025f, fTime * 0.0095f) * 3.0f) * 2.0f + 2.0f;
    for (int i = 0; i < 27; ++i)
    {
        float3 vPosition = float3(vScaledTexCoord * float(i), float(i));
        vColorCoefficients += float3(sin(vColorCoefficients.y + sin(vPosition.x)),
                   cos(vColorCoefficients.z + sin(vPosition.z)),
                   -sin(vColorCoefficients.x + sin(vPosition.y)));
    }
    
    float fAlpha = (1.f - g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord).a) * 0.5f;
    Out.vColor = float4(vColorCoefficients * vColorCoefficients * 0.004f, pow(fAlpha, 3.f));
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Perlin_Cloud
    {
        VertexShader = compile vs_5_0 VS_PERLIN_CLOUD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_PERLIN_CLOUD();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
}
