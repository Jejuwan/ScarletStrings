#include "ShaderCommon.hlsli"

Texture2D g_texNoiseGrid;
Texture2D g_texWhiteLine;
float g_fTime = 0.f;

Texture2D g_NoiseMask;

float4 g_vGlitchBaseColor;
float g_fSeed;

float g_fFadeFactor = 1.f;

Texture2D   g_texLUT;
float       g_fLUTFactor;

// Ripple
float       g_fRippleSpeed;
float       g_fRippleStrength;

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

struct PS_OUT_BLOOM
{
    float4 vTexture : SV_TARGET0;
    float4 vData : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.a = 1.f - g_texNormal[0].Sample(LinearSampler, In.vTexCoord).a;
    if (Out.vColor.a <= 0.f)
        discard;
    
    if (Out.vColor.a <= 0.1f)
    {
        vector vGridColor = g_texNoiseGrid.Sample(LinearSampler, In.vTexCoord * 20.f);
        if (vGridColor.r < 0.5f)
        {
            Out.vColor.r = 1.f;
            Out.vColor.a = (1.f - Out.vColor.a);
        }
    }
	
    return Out;
}

float Rand(float fSeed)
{
    return frac(sin(dot(float2(fSeed, fSeed), float2(12.9898f, 78.233f))) * 43758.5453f);
}

float4 Interlace(float2 vTexelCoord, float4 vColor)
{
    if (int(vTexelCoord.y) % 3 == 0)
    {
        return vColor * ((sin(g_fTime * 4.f) * 0.1f) + 0.75f) + (Rand(g_fTime) * 0.05f);
    }
    return vColor;
}

PS_OUT PS_CCTV(PS_IN In)
{
    PS_OUT Out;
    
    float2 vRedDisplace = float2(0.f, 0.f);
    float2 vGreenDisplace = float2(0.f, 0.f);
    float2 vBlueDisplace = float2(0.f, 0.f);
    
    vRedDisplace.x += 0.0025 * (0.5f - Rand(g_fTime * 37.f * In.vTexCoord.y));
    vRedDisplace.x += 0.0035 * (0.5f - Rand(g_fTime * 41.f * In.vTexCoord.y));
    vRedDisplace.x += 0.00065 * (0.5f - Rand(g_fTime * 53.f * In.vTexCoord.y));
    
    vRedDisplace.y += 0.001 * (0.5f - Rand(g_fTime * 37.f * In.vTexCoord.x));
    vRedDisplace.y += 0.001 * (0.5f - Rand(g_fTime * 41.f * In.vTexCoord.x));
    vRedDisplace.y += 0.001 * (0.5f - Rand(g_fTime * 53.f * In.vTexCoord.x));
    
    float fRColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord + vRedDisplace).r;
    float fGColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord + vRedDisplace).g;
    float fBColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord + vRedDisplace).b;
    
    Out.vColor = Interlace(In.vTexCoord * (1920.f, 1080.f), float4(fRColor, fGColor, fBColor, 1.f));
    
    float4 vLineColor = g_texWhiteLine.Sample(LinearSampler, In.vTexCoord + float2(0.f, g_fTime * 30.f)) * 0.25f;
    Out.vColor += vLineColor;
    
    return Out;
}

PS_OUT PS_LINE_EFFECT(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    float fColorStrength = Out.vColor.r - g_fTime;
    if (fColorStrength <= 0.f)
        discard;
    
    Out.vColor = float4(1.f, pow(fColorStrength, 2.f), 0.13f, fColorStrength);

    return Out;
}

float SeedRand(float2 vSeed)
{
    float fRandTime = floor(g_fSeed * 20.f) / 10.f;
    return frac(sin(dot(vSeed, float2(fRandTime * 12.9898, fRandTime * 78.233))) * 43758.5453);
}

float RandNoise(float2 vTexcoord, float fBlockiness)
{
    float2 vLV = frac(vTexcoord);
    float2 vID = floor(vTexcoord);
    
    float n1 = SeedRand(vID);
    float n2 = SeedRand(vID + float2(1, 0));
    float n3 = SeedRand(vID + float2(0, 1));
    float n4 = SeedRand(vID + float2(1, 1));
    
    float2 u = smoothstep(0.0, 1.0 + fBlockiness, vLV);

    return lerp(lerp(n1, n2, u.x), lerp(n3, n4, u.x), u.y);
}

float Glitch(float2 vTexcoord, int iLevel, float fBlockiness, float fComplexity)
{
    float fValue = 0.0;
    float fAmp = 0.5;
    
    while (iLevel != 0)
    {
        fValue += fAmp * RandNoise(vTexcoord + (SeedRand(ceil(vTexcoord * 3.0) / 3.0) * 2.0 + (floor(g_fSeed * 20.0) / 10.0 / float(iLevel)) - 1.0), fBlockiness);
        fAmp *= 0.5;
        vTexcoord *= fComplexity;
        iLevel--;
    }
    
    return fValue;
}

PS_OUT PS_GLITCH_EFFECT(PS_IN In)
{
    PS_OUT Out;
    
    float2 vTexcoord0 = In.vTexCoord;
    float2 vTexcoord1 = In.vTexCoord;
    
    vTexcoord0 *= 15.f;
    vTexcoord0.x *= Glitch(vTexcoord0, 2, 3.0, 1.0);
    
    float noiseVal = (smoothstep(0.5, 1.0, Glitch(vTexcoord0, 2, 3.0, 1.5)));
    
    float fR = g_texDiffuse[0].Sample(LinearSampler, float2(vTexcoord1.x + noiseVal * 0.05, vTexcoord1.y)).r;
    float fG = g_texDiffuse[0].Sample(LinearSampler, float2(vTexcoord1.x - noiseVal * 0.05, vTexcoord1.y)).g;
    float fB = g_texDiffuse[0].Sample(LinearSampler, float2(vTexcoord1.x, vTexcoord1.y)).b;
    float3 vColor = float3(fR, fG, fB);
    vColor = g_vGlitchBaseColor.rgb * smoothstep(0.2, 0.3, noiseVal);
    
    float fMask = 1.f - g_NoiseMask.Sample(LinearSampler, In.vTexCoord).a;
    Out.vColor = float4(vColor, fMask);
    
    return Out;
}

PS_OUT PS_NEON(PS_IN In)
{
    PS_OUT Out;
    
    float4 vDiffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor = vDiffuse * g_vMtrlDiffuse;
    
    return Out;
}

PS_OUT PS_SAMPLE_DEPTH(PS_IN In)
{
    PS_OUT Out;
    
    float fDepthExist = g_texMask.Sample(TargetSampler, In.vTexCoord).r;
    if (fDepthExist <= 0.00001f)
        discard;
    
    Out.vColor = g_texDiffuse[0].Sample(TargetSampler, In.vTexCoord) * g_fFadeFactor;
    
    return Out;
}

PS_OUT PS_DAMAGE_OVERLAY(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_texDiffuse[0].Sample(TargetSampler, In.vTexCoord) * g_vMtrlDiffuse;
    
    return Out;
}

PS_OUT PS_RIPPLE(PS_IN In)
{
    PS_OUT Out;
    
    float fRadius = length(In.vTexCoord - float2(0.5f, 0.5f));
    float fRippleScale = smoothstep(0.1f, 0.5f, fRadius);
    float fLength = fRadius * 25.f;
    float2 vSinFactor = normalize(In.vTexCoord) * (sin(fLength - g_fTime * g_fRippleSpeed)) * fRippleScale * g_fRippleStrength;
    
    Out.vColor = g_texDiffuse[0].Sample(TargetSampler, In.vTexCoord + vSinFactor);
    
    return Out;
}

PS_OUT PS_RGB_SHIFT(PS_IN In)
{
    PS_OUT Out;
    
    float fESP0 = 1.0f / 256.0f * sin(g_fTime * 2.0f);
    float fESP1 = 1.0f / 256.0f * cos(g_fTime * 2.0f);
    
    float fRandTime = floor(g_fTime * 20.f) / 10.f;
    float fRandFactor =  frac(sin(dot(g_fTime, float2(fRandTime * 12.9898, fRandTime * 78.233))) * 43758.5453);
    
    float3 vColor;
    vColor.r = g_texDiffuse[0].Sample(TargetSampler, float2(In.vTexCoord.x + fESP0 * fRandFactor, In.vTexCoord.y + fESP1 * fRandFactor)).r;
    vColor.g = g_texDiffuse[0].Sample(TargetSampler, float2(In.vTexCoord.x - fESP1 * fRandFactor, In.vTexCoord.y + fESP0 * fRandFactor)).g;
    vColor.b = g_texDiffuse[0].Sample(TargetSampler, float2(In.vTexCoord.x - fESP0 * fRandFactor, In.vTexCoord.y + fESP0 * fRandFactor)).b;
    
    Out.vColor = float4(vColor, 1.0f);
    
    return Out;
}

PS_OUT PS_COLOR_REVERSE(PS_IN In)
{
    PS_OUT Out;
    
    float fRadius = length(In.vTexCoord - float2(0.5f, 0.5f));
    float fReverseStep = smoothstep(0.f, 0.75f, fRadius);

    Out.vColor = lerp(g_texDiffuse[0].Sample(TargetSampler, In.vTexCoord), float4(0.2f, 0.4f, 0.5f, 1.f) - g_texDiffuse[0].Sample(TargetSampler, In.vTexCoord), fReverseStep);
    
    return Out;
}

float3 ExecuteLUTFilter(Texture2D LUTTexture, float2 vTexCoord, float3 vColor)
{
    float2 vLUTSize;
    LUTTexture.GetDimensions(vLUTSize.x, vLUTSize.y);
    vLUTSize = 1.f / vLUTSize;

    float4 LUTUV;
    float3 vScaledColor = saturate(vColor) * 15.0f;

    LUTUV.w = floor(vScaledColor.b);
    LUTUV.xy = (vScaledColor.rg + 0.5f) * vLUTSize;
    LUTUV.x += LUTUV.w * vLUTSize.y;
    LUTUV.z = LUTUV.x + vLUTSize.y;

    float3 color1 = LUTTexture.Sample(LinearSampler, LUTUV.xy).rgb;
    float3 color2 = LUTTexture.Sample(LinearSampler, LUTUV.zy).rgb;
    float lerpValue = vScaledColor.b - LUTUV.w;
    float3 result = lerp(color1, color2, lerpValue);
	
    return result;
}

PS_OUT PS_LUT_FILTER(PS_IN In)
{
    PS_OUT Out;
    
    float3 vOriginColor = g_texDiffuse[0].Sample(TargetSampler, In.vTexCoord).rgb;
    float3 vFilterColor = ExecuteLUTFilter(g_texLUT, In.vTexCoord, vOriginColor);
    
    float3 vColor = lerp(vOriginColor, vFilterColor, g_fLUTFactor);
    
    Out.vColor = float4(vColor, 1.f);
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Default // 0
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Orthographic // 1
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Billboard // 2
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass CCTV // 3
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_CCTV();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Line_Effect // 4
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LINE_EFFECT();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass GLITCH_EFFECT // 5
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_GLITCH_EFFECT();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass BILLBOARD_NEON // 6
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NEON();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass BILLBOARD_NEON_ADDITIVE // 7
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NEON();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass SAMPLE_DEPTH // 8
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SAMPLE_DEPTH();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass DAMAGE_OVERLAY // 9
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DAMAGE_OVERLAY();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass RIPPLE // 10
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RIPPLE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass RGB_SHIFT // 11
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RGB_SHIFT();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass COLOR_REVERSE // 12
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR_REVERSE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass LUT_FILTER // 13
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LUT_FILTER();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }
}
