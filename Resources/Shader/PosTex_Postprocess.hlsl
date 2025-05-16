#include "ShaderCommon.hlsli"

Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
Texture2D g_LUTTexture : register(t2);
Texture2D g_texDepthTarget : register(t3);

// Tonemapping
Texture2D g_texBloomStrength;

float g_Exposure;
float g_Gamma;

float2 g_SampleStep;

// LUT Filter
bool g_bMaskingLUT;

// FXAA
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        32
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))

// DOF
float g_fNearPlane = 0.1f;
float g_fFarPlane = 10000.f;

float g_fDepthStart = 10.f;
float g_fDepthRange = 30.f;

struct VS_IN
{
	float3	vPosition	: POSITION;
	float2	vTexCoord	: TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;

	Out.vPosition = float4(In.vPosition, 1.f);
	Out.vTexCoord = In.vTexCoord;

	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor		: SV_TARGET0;
};

PS_OUT PS_DOWNSAMPLING(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	float fX = In.vTexCoord.x;
	float fY = In.vTexCoord.y;
	
	clip(fX);
	clip(fY);
	
	float4 a = g_texture0.Sample(LinearSampler, saturate(float2(fX - 2 * g_SampleStep.x, fY + 2 * g_SampleStep.y)));
	float4 b = g_texture0.Sample(LinearSampler, saturate(float2(fX, fY + 2 * g_SampleStep.y)));
	float4 c = g_texture0.Sample(LinearSampler, saturate(float2(fX + 2 * g_SampleStep.x, fY + 2 * g_SampleStep.y)));

	float4 d = g_texture0.Sample(LinearSampler, saturate(float2(fX - 2 * g_SampleStep.x, fY)));
	float4 e = g_texture0.Sample(LinearSampler, saturate(float2(fX, fY)));
	float4 f = g_texture0.Sample(LinearSampler, saturate(float2(fX + 2 * g_SampleStep.x, fY)));

	float4 g = g_texture0.Sample(LinearSampler, saturate(float2(fX - 2 * g_SampleStep.x, fY - 2 * g_SampleStep.y)));
	float4 h = g_texture0.Sample(LinearSampler, saturate(float2(fX, fY - 2 * g_SampleStep.y)));
	float4 i = g_texture0.Sample(LinearSampler, saturate(float2(fX + 2 * g_SampleStep.x, fY - 2 * g_SampleStep.y)));

	float4 j = g_texture0.Sample(LinearSampler, saturate(float2(fX - g_SampleStep.x, fY + g_SampleStep.y)));
	float4 k = g_texture0.Sample(LinearSampler, saturate(float2(fX + g_SampleStep.x, fY + g_SampleStep.y)));
	float4 l = g_texture0.Sample(LinearSampler, saturate(float2(fX - g_SampleStep.x, fY - g_SampleStep.y)));
	float4 m = g_texture0.Sample(LinearSampler, saturate(float2(fX + g_SampleStep.x, fY - g_SampleStep.y)));
	
	float4 vDownSample = e * 0.125;
	vDownSample += (a + c + g + i) * 0.03125;
	vDownSample += (b + d + f + h) * 0.0625;
	vDownSample += (j + k + l + m) * 0.125;
	
	Out.vColor = vDownSample;
	
	return Out;
}

PS_OUT PS_UPSAMPLING(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	float x = In.vTexCoord.x;
	float y = In.vTexCoord.y;
	
	clip(x);
	clip(y);

	float4 a = g_texture0.Sample(LinearSampler, saturate(float2(x - g_SampleStep.x, y + g_SampleStep.y)));
	float4 b = g_texture0.Sample(LinearSampler, saturate(float2(x, y + g_SampleStep.y)));
	float4 c = g_texture0.Sample(LinearSampler, saturate(float2(x + g_SampleStep.x, y + g_SampleStep.y)));

	float4 d = g_texture0.Sample(LinearSampler, saturate(float2(x - g_SampleStep.x, y)));
	float4 e = g_texture0.Sample(LinearSampler, saturate(float2(x, y)));
	float4 f = g_texture0.Sample(LinearSampler, saturate(float2(x + g_SampleStep.x, y)));

	float4 g = g_texture0.Sample(LinearSampler, saturate(float2(x - g_SampleStep.x, y - g_SampleStep.y)));
	float4 h = g_texture0.Sample(LinearSampler, saturate(float2(x, y - g_SampleStep.y)));
	float4 i = g_texture0.Sample(LinearSampler, saturate(float2(x + g_SampleStep.x, y - g_SampleStep.y)));

	float4 upsample = e * 4.0;
	upsample += (b + d + f + h) * 2.0;
	upsample += (a + c + g + i);
	upsample *= 1.0 / 16.0;
	
	Out.vColor = upsample;
	
	return Out;
}

float3 FilmicToneMapping(float3 color)
{
	color = max(float3(0, 0, 0), color);
	color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
	return color;
}

float3 LinearToneMapping(float3 color)
{
	float3 invGamma = float3(1, 1, 1) / g_Gamma;

	color = clamp(g_Exposure * color, 0., 1.);
	color = pow(color, invGamma);
	return color;
}

float4 LinearToneMapping(float4 color)
{
	float4 invGamma = float4(1, 1, 1, 1) / g_Gamma;

	color = clamp(g_Exposure * color, 0., 1.);
	color = pow(color, invGamma);
	return color;
}

float3 Uncharted2ToneMapping(float3 color)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	
	color *= g_Exposure;
	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
	float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
	color /= white;
	color = pow(color, float3(1.0, 1.0, 1.0) / g_Gamma);
	return color;
}

float3 lumaBasedReinhardToneMapping(float3 color)
{
	float3 invGamma = float3(1, 1, 1) / g_Gamma;
	float luma = dot(color, float3(0.2126, 0.7152, 0.0722));
	float toneMappedLuma = luma / (1. + luma);
	color *= toneMappedLuma / luma;
	color = pow(color, invGamma);
	return color;
}

PS_OUT PS_COMBINE_TONEMAPPING(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float3 color0 = g_texture0.Sample(LinearSampler, In.vTexCoord).rgb;
	float4 color1 = g_texture1.Sample(LinearSampler, In.vTexCoord);

	float colorExist = color1.r + color1.g + color1.b + color1.a;
	if (colorExist == 0.f)
		discard;
	
	float3 combined = (1.0 - g_fBloomStrength) * color0 + g_fBloomStrength * color1.rgb;
	
	combined = LinearToneMapping(combined);

	Out.vColor = float4(combined, color1.a);
	
	return Out;
}

PS_OUT PS_COMBINE_TONEMAPPING_BYTARGET(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 color1 = g_texture1.Sample(LinearSampler, In.vTexCoord);
	
//  float4 strength = g_texBloomStrength.Sample(LinearSampler, In.vTexCoord);
	float strength = g_texBloomStrength.Sample(LinearSampler, In.vTexCoord).r;
	
	float4 combined =strength * color1;
	
    Out.vColor = LinearToneMapping(combined);
	
	return Out;
}

PS_OUT PS_COMBINE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float3 color0 = g_texture0.Sample(LinearSampler, In.vTexCoord).rgb;
	float3 color1 = g_texture1.Sample(LinearSampler, In.vTexCoord).rgb;

	float3 combined = (1.0 - g_fBloomStrength) * color0 + g_fBloomStrength * color1;
//	float3 combined = g_fBloomStrength * color1;

	Out.vColor = float4(combined, 1.0f);
	
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

PS_OUT PS_COMBINE_TONEMAPPING_LUT(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	// Tonemapping
	float3 color0 = g_texture0.Sample(LinearSampler, In.vTexCoord).rgb;
	float3 color1 = g_texture1.Sample(LinearSampler, In.vTexCoord).rgb;
	
	float3 combined = (1.0 - g_fBloomStrength) * color0 + g_fBloomStrength * color1;
	
	combined = LinearToneMapping(combined);
	
	// LUT Filter
	Out.vColor = float4(ExecuteLUTFilter(g_LUTTexture, In.vTexCoord, combined), 1.f);
	if (g_bMaskingLUT)
	{
		float fMask = g_texMask.Sample(LinearSampler, In.vTexCoord).a;
		Out.vColor = float4(lerp(combined, Out.vColor.rgb, fMask), 1.f);
	}
	
	return Out;
}

float4 FxaaTexOff(Texture2D Texture, float2 vPos, int2 vOff)
{
	return Texture.SampleLevel(LinearSampler, vPos.xy, 0.0, vOff.xy);
}

float FxaaLuma(float3 vRGB)
{
	return vRGB.y * (0.587 / 0.299) + vRGB.x;
}

float3 FxaaFilterReturn(float3 vRGB)
{
	return vRGB;
}

float4 FxaaTexGrad(Texture2D Texture, float2 vPos, float2 vGrad)
{
	return Texture.SampleGrad(LinearSampler, vPos.xy, vGrad, vGrad);
}

float3 FxaaLerp3(float3 fA, float3 fB, float fAmountOfA)
{
	return (float3(-fAmountOfA, 0, 0) * fB) +
		((fA * float3(fAmountOfA, 0, 0)) + fB);
}

float4 FxaaTexLod0(Texture2D Texture, float2 vPos)
{
	return Texture.SampleLevel(LinearSampler, vPos.xy, 0.0);
}

PS_OUT PS_FXAA(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float2 vResolution;
	g_texture0.GetDimensions(vResolution.x, vResolution.y);
	
	vResolution = 1.f / vResolution;
	
	float3 fRGB_N = FxaaTexOff(g_texture0, In.vTexCoord, int2(0, -1)).xyz;
	float3 fRGB_W = FxaaTexOff(g_texture0, In.vTexCoord, int2(-1, 0)).xyz;
	float3 fRGB_M = FxaaTexOff(g_texture0, In.vTexCoord, int2(0, 0)).xyz;
	float3 fRGB_E = FxaaTexOff(g_texture0, In.vTexCoord, int2(1, 0)).xyz;
	float3 fRGB_S = FxaaTexOff(g_texture0, In.vTexCoord, int2(0, 1)).xyz;
	float fLuma_N = FxaaLuma(fRGB_N);
	float fLuma_W = FxaaLuma(fRGB_W);
	float fLuma_M = FxaaLuma(fRGB_M);
	float fLuma_E = FxaaLuma(fRGB_E);
	float fLuma_S = FxaaLuma(fRGB_S);
	float fRangeMin = min(fLuma_M, min(min(fLuma_N, fLuma_W), min(fLuma_S, fLuma_E)));
	float fRangeMax = max(fLuma_M, max(max(fLuma_N, fLuma_W), max(fLuma_S, fLuma_E)));
	float fRange = fRangeMax - fRangeMin;
	if (fRange < max(FXAA_EDGE_THRESHOLD_MIN, fRangeMax * FXAA_EDGE_THRESHOLD))
	{
		Out.vColor = float4(FxaaFilterReturn(fRGB_M), 1.0f);
		return Out;
	}
	float3 fRGB_L = fRGB_N + fRGB_W + fRGB_M + fRGB_E + fRGB_S;
	
#if FXAA_SUBPIX != 0
	float fLuma_L = (fLuma_N + fLuma_W + fLuma_E + fLuma_S) * 0.25;
	float fRange_L = abs(fLuma_L - fLuma_M);
#endif
#if FXAA_SUBPIX == 1
	float fBlend_L = max(0.0,
			(fRange_L / fRange) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
	fBlend_L = min(FXAA_SUBPIX_CAP, fBlend_L);
#endif
	
	
	float3 vRGB_NW = FxaaTexOff(g_texture0, In.vTexCoord, int2(-1, -1)).xyz;
	float3 vRGB_NE = FxaaTexOff(g_texture0, In.vTexCoord, int2(1, -1)).xyz;
	float3 vRGB_SW = FxaaTexOff(g_texture0, In.vTexCoord, int2(-1, 1)).xyz;
	float3 vRGB_SE = FxaaTexOff(g_texture0, In.vTexCoord, int2(1, 1)).xyz;
#if (FXAA_SUBPIX_FASTER == 0) && (FXAA_SUBPIX > 0)
	fRGB_L += (vRGB_NW + vRGB_NE + vRGB_SW + vRGB_SE);
	fRGB_L *= float3(1.0 / 9.0, 0, 0);
#endif
	float fLuma_NW = FxaaLuma(vRGB_NW);
	float fLuma_NE = FxaaLuma(vRGB_NE);
	float fLuma_SW = FxaaLuma(vRGB_SW);
	float fLuma_SE = FxaaLuma(vRGB_SE);
	float fEdgeVert =
		abs((0.25 * fLuma_NW) + (-0.5 * fLuma_N) + (0.25 * fLuma_NE)) +
		abs((0.50 * fLuma_W) + (-1.0 * fLuma_M) + (0.50 * fLuma_E)) +
		abs((0.25 * fLuma_SW) + (-0.5 * fLuma_S) + (0.25 * fLuma_SE));
	float fEdgeHorz =
		abs((0.25 * fLuma_NW) + (-0.5 * fLuma_W) + (0.25 * fLuma_SW)) +
		abs((0.50 * fLuma_N) + (-1.0 * fLuma_M) + (0.50 * fLuma_S)) +
		abs((0.25 * fLuma_NE) + (-0.5 * fLuma_E) + (0.25 * fLuma_SE));
	bool fHorzSpan = fEdgeHorz >= fEdgeVert;
	float fLengthSign = fHorzSpan ? -vResolution.y : -vResolution.x;
	if (!fHorzSpan)
		fLuma_N = fLuma_W;
	if (!fHorzSpan)
		fLuma_S = fLuma_E;
	float fGradient_N = abs(fLuma_N - fLuma_M);
	float fGradient_S = abs(fLuma_S - fLuma_M);
	fLuma_N = (fLuma_N + fLuma_M) * 0.5;
	fLuma_S = (fLuma_S + fLuma_M) * 0.5;
	
	
	bool bPair_N = fGradient_N >= fGradient_S;
	if (!bPair_N)
		fLuma_N = fLuma_S;
	if (!bPair_N)
		fGradient_N = fGradient_S;
	if (!bPair_N)
		fLengthSign *= -1.0;
	float2 vPos_N;
	vPos_N.x = In.vTexCoord.x + (fHorzSpan ? 0.0 : fLengthSign * 0.5);
	vPos_N.y = In.vTexCoord.y + (fHorzSpan ? fLengthSign * 0.5 : 0.0);
	
	fGradient_N *= FXAA_SEARCH_THRESHOLD;

	float2 vPos_P = vPos_N;
	float2 vOff_NP = fHorzSpan ?
		float2(vResolution.x, 0.0) :
		float2(0.0f, vResolution.y);
	float fLumaEnd_N = fLuma_N;
	float fLumaEnd_P = fLuma_N;
	bool bDone_N = false;
	bool bDone_P = false;
#if FXAA_SEARCH_ACCELERATION == 1
	vPos_N += vOff_NP * float2(-1.0, -1.0);
	vPos_P += vOff_NP * float2(1.0, 1.0);
#endif
	for (int i = 0; i < FXAA_SEARCH_STEPS; i++)
	{
#if FXAA_SEARCH_ACCELERATION == 1
		if (!bDone_N)
			fLumaEnd_N =
				FxaaLuma(FxaaTexLod0(g_texture0, vPos_N.xy).xyz);
		if (!bDone_P)
			fLumaEnd_P =
				FxaaLuma(FxaaTexLod0(g_texture0, vPos_P.xy).xyz);
#endif
		bDone_N = bDone_N || (abs(fLumaEnd_N - fLuma_N) >= fGradient_N);
		bDone_P = bDone_P || (abs(fLumaEnd_P - fLuma_N) >= fGradient_N);
		if (bDone_N && bDone_P)
			break;
		if (!bDone_N)
			vPos_N -= vOff_NP;
		if (!bDone_P)
			vPos_P += vOff_NP;
	}
	
	
	float fDst_N = fHorzSpan ? In.vTexCoord.x - vPos_N.x : In.vTexCoord.y - vPos_N.y;
	float fDst_P = fHorzSpan ? vPos_P.x - In.vTexCoord.x : vPos_P.y - In.vTexCoord.y;
	bool bDirection_N = fDst_N < fDst_P;
	fLumaEnd_N = bDirection_N ? fLumaEnd_N : fLumaEnd_P;
	
	if (((fLuma_M - fLuma_N) < 0.0) == ((fLumaEnd_N - fLuma_N) < 0.0)) 
		fLengthSign = 0.0;
	
	float fSpanLength = (fDst_P + fDst_N);
	fDst_N = bDirection_N ? fDst_N : fDst_P;
	float fSubPixelOffset = (0.5 + (fDst_N * (-1.0 / fSpanLength))) * fLengthSign;
	float3 vRGB_F = FxaaTexLod0(g_texture0, float2(
	In.vTexCoord.x + (fHorzSpan ? 0.0 : fSubPixelOffset),
	In.vTexCoord.y + (fHorzSpan ? fSubPixelOffset : 0.0))).xyz;
	
	Out.vColor = float4(FxaaFilterReturn(FxaaLerp3(fRGB_L, vRGB_F, fBlend_L)), 1.0f);
	
	return Out;
}

float DepthBufferConvertToLinear(float fDepth)
{
	float fAlpha = 1.0f / (g_fNearPlane - g_fFarPlane);
	return (g_fNearPlane * g_fFarPlane * fAlpha) / (fDepth + g_fFarPlane * fAlpha);
}

PS_OUT PS_DOF(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 vColor = g_texture0.Sample(ClampLinearSampler, In.vTexCoord);
	float4 vBlur = g_texture1.Sample(ClampLinearSampler, In.vTexCoord);
	
	float fDepth = g_texDepthTarget.Sample(ClampLinearSampler, In.vTexCoord).r;
	float fViewZ = DepthBufferConvertToLinear(fDepth);
	
	if (fDepth < 1.f)
	{
		float fPercent = saturate((fViewZ - g_fDepthStart) / g_fDepthRange);
		vColor = lerp(vColor, vBlur, fPercent);
	}
	vColor.a = 1.0f;
	
	Out.vColor = vColor;
	
	return Out;
}

PS_OUT COMBINE_BLOOM_STRENGTH(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float3 color1 = g_texture1.Sample(LinearSampler, In.vTexCoord).rgb;
    Out.vColor = float4(color1, 1.0f);
	
    return Out;
}

technique11 DefaultTechnique
{
	pass DownSampling
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DOWNSAMPLING();

		ComputeShader = NULL;
	}

	pass UpSampling
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_UPSAMPLING();

		ComputeShader = NULL;
	}

	pass Combine_Tonemapping
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COMBINE_TONEMAPPING();

		ComputeShader = NULL;
	}

	pass Combine
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COMBINE();

		ComputeShader = NULL;
	}

	pass Combine_Tonemapping_ByTarget
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COMBINE_TONEMAPPING_BYTARGET();

		ComputeShader = NULL;
	}

	pass Combine_Tonemapping_LUT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COMBINE_TONEMAPPING_LUT();

		ComputeShader = NULL;
	}

	pass FXAA_Filter
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_FXAA();

		ComputeShader = NULL;
	}

	pass DOF_Filter
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DOF();

		ComputeShader = NULL;
	}

    pass COMBINE_BLOOM_STRENGTH
    {
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 COMBINE_BLOOM_STRENGTH();

        ComputeShader = NULL;
    }
}