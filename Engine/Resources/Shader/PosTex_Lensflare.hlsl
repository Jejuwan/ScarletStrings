#include "ShaderCommon.hlsli"

float4x4			g_mQuadWorld;
float4x4			g_mQuadView;
float4x4			g_mQuadProj;

float3              g_vFlarePosition;
float               fTime = 0.f;
float               fStrength = 1.f;
float3              g_vSunBaseColor = float3(1.4, 1.2, 1.0);

Texture2D           g_NoiseTexture;
Texture2D           g_texDepthTarget;

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

float Noise(float t)
{
    int2 iResolution;
    g_NoiseTexture.GetDimensions(iResolution.x, iResolution.y);
    
    return g_NoiseTexture.Sample(LinearSampler, float2(t, 0.0f) / iResolution.xy).x;
}

float Noise(float2 t)
{
    int2 iResolution;
    g_NoiseTexture.GetDimensions(iResolution.x, iResolution.y);
    
    return g_NoiseTexture.Sample(LinearSampler, (t + float2(fTime, fTime)) / iResolution.xy).x;
}

float3 Lensflare(float2 uv, float2 pos)
{
    float2 main = uv - pos;
    float2 uvd = uv * (length(uv));
	
    float ang = abs(atan2(main.y, main.x));
    float dist = length(main);
    dist = pow(dist, .1);
    float n = Noise(float2((ang - fTime / 9.0) * 16.0, dist * 32.0));
	
    float f0 = 1.0 / (length(uv - pos) * 16.0 + 1.0);
	
    f0 = f0 + f0 * (sin((ang + fTime / 18.0 + Noise(abs(ang) + n / 2.0) * 2.0) * 12.0) * .1 + dist * .1 + .8);

    float f2 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.8 * pos), 2.0)), .0) * 0.25;
    float f22 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.85 * pos), 2.0)), .0) * 0.23;
    float f23 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.9 * pos), 2.0)), .0) * 0.21;
	
    float2 uvx = lerp(uv, uvd, -0.5);
	
    float f4 = max(0.01 - pow(length(uvx + 0.4 * pos), 2.4), .0) * 6.0;
    float f42 = max(0.01 - pow(length(uvx + 0.45 * pos), 2.4), .0) * 5.0;
    float f43 = max(0.01 - pow(length(uvx + 0.5 * pos), 2.4), .0) * 3.0;
	
    uvx = lerp(uv, uvd, -.4);
	
    float f5 = max(0.01 - pow(length(uvx + 0.2 * pos), 5.5), .0) * 2.0;
    float f52 = max(0.01 - pow(length(uvx + 0.4 * pos), 5.5), .0) * 2.0;
    float f53 = max(0.01 - pow(length(uvx + 0.6 * pos), 5.5), .0) * 2.0;
	
    uvx = lerp(uv, uvd, -0.5);
	
    float f6 = max(0.01 - pow(length(uvx - 0.3 * pos), 1.6), .0) * 6.0;
    float f62 = max(0.01 - pow(length(uvx - 0.325 * pos), 1.6), .0) * 3.0;
    float f63 = max(0.01 - pow(length(uvx - 0.35 * pos), 1.6), .0) * 5.0;
	
    float3 c = float3(0.0f, 0.0f, 0.0f);
	
    c.r += f2 + f4 + f5 + f6;
    c.g += f22 + f42 + f52 + f62;
    c.b += f23 + f43 + f53 + f63;
    c += float3(f0, f0, f0);
	
    return c;
}

float3 ModifyColor(float3 color, float fFactor, float fFactor2)
{
    float w = color.x + color.y + color.z;
    return lerp(color, float3(w, w, w) * fFactor, w * fFactor2);
}

PS_OUT PS_MAIN_Lensflare(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 vResolution;
    g_texDepthTarget.GetDimensions(vResolution.x, vResolution.y);
    float fAspectRatio = vResolution.x / vResolution.y;
    
    float2 vTexCoord = In.vTexCoord;
    vTexCoord.x *= fAspectRatio;
    
    float4 vFlarePos = mul(float4(g_vFlarePosition, 1.f), g_mWorld);
    vFlarePos = mul(mul(vFlarePos, g_mView), g_mProj);

    float fFlareDir = vFlarePos.w;
    vFlarePos /= fFlareDir;
    
    if (fFlareDir < 0.f)
    {
        Out.vColor = float4(0.f, 0.f, 0.f, 1.0f);
    }
    else
    {
        float2 vFlarePixelPos = float2(fAspectRatio * vFlarePos.x, vFlarePos.y * -1.f);
        float3 vColor = g_vSunBaseColor * Lensflare(vTexCoord, vFlarePixelPos);
        vColor = ModifyColor(vColor, 0.5f, 0.1f);
        Out.vColor = float4(pow(vColor, fStrength), 1.0f);
    }
    
    vector vDepth = g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
    if (vDepth.x == 0.f)
    {
        Out.vColor.a = 0.3f;
    }
    else
    {
        discard;
    }
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Lensflare
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Lensflare();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }
}