#include "ShaderCommon.hlsli"

float4x4			g_mQuadWorld;
float4x4			g_mQuadView;
float4x4			g_mQuadProj;

texture				g_texHeatMap;

Texture2D			g_texShadowDepthTarget[MAX_LIGHT];
Texture2D			g_texMtrlDiffuseTarget;
Texture2D			g_texMtrlAmbientTarget;
Texture2D			g_texMtrlSpecularTarget;
Texture2D			g_texMtrlEmissiveTarget;
Texture2D			g_texNormalTarget;
Texture2D			g_texDepthTarget;
Texture2D			g_texPreMaskTarget;
Texture2D			g_texOutLineTarget;
Texture2D			g_texLightDiffuseTarget;
Texture2D			g_texLightAmbientTarget;
Texture2D			g_texLightSpecularTarget;
Texture2D			g_texShadowProcessTarget;
Texture2D			g_texPreProcessTarget;
Texture2D			g_texProcessDataTarget;
Texture2D			g_texMaskTarget;
Texture2D			g_texBloomTarget;
Texture2D			g_texBloomStrengthTarget;
Texture2D			g_texSSAOTarget;
Texture2D			g_texLensflareTarget;

int					g_iShadowCount		= 0;
float4x4			g_mShadowViews[MAX_LIGHT];
float4x4			g_mShadowProjs[MAX_LIGHT];

bool				g_bFog				= false;
float4				g_vFogColor			= float4(1.f, 1.f, 1.f, 1.f);
float				g_fFogStart			= 0.f;
float				g_fFogEnd			= 1.f;
float				g_fFogAmbient		= 0.f;
float				g_fFogMax			= 1.f;
float				g_fFogPower			= 1.f;

//	Shadow
float				fBias				= 0.f;
float				fShadowShade		= 0.8f;
float				fMinShadowAvg		= 0.f;
float				fTexelScale			= 0.25f;
int2				vGridSize			= int2(1, 1);
static const float2	vTexel				= float2(1.f / 1920.f, 1.f / 1080.f);

//	Dissolve
static const float	fTimeFactor			= 4.f;
static const float	fEdgeThickness		= 0.25f;
static const float	fEdgeFluctuation	= 0.1f;
static const float3	vDissolveColor		= float3(24.f, 2.f, 1.f);

// SSAO
float g_fRadius							= 0.001f;
float g_fFalloff						= 0.000002f;
float g_fStrength						= 0.0007f;
float g_fTotStrength					= 1.38f;
float g_fInvSamples						= 1.f / 16.f;

// Radial Blur
float g_fRadialBlurSampleLevel			= 10;

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

float4 ShadowDepthTargetLiteral(int iIndex, float2 vUV);

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

struct PS_OUT_LIGHT
{
	float4	vDiffuse		: SV_TARGET0;
	float4	vAmbient		: SV_TARGET1;
	float4	vSpecular		: SV_TARGET2;
};

struct PS_OUT_POSTPROCESS
{
	float4	vPreProcess		: SV_TARGET0;
	float4	vProcessData	: SV_TARGET1;
	float4	vMask			: SV_TARGET2;
};

float4 NormalizeRange(float4 color)
{
	return color / max(max(max(color.r, color.g), color.b), color.a);
}

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
 
		if (fOccNorm <= In.fDepth + 0.0001f)
			++iColor;
	}
 
	vAmbient = (1.f - abs((iColor / 16.f) - 1));
	vAmbient.a = 1.f;
	
	return vAmbient;
}

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT	Out;
	
	float4			vNormalTarget	= g_texNormalTarget.Sample(TargetSampler, In.vTexCoord);
	float4			vNormal			= vNormalTarget * 2.f - 1.f;
	
	float4			vDepthTarget	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
	float			fViewZ			= vDepthTarget.y * g_fCamFar;

	float			fShade			= saturate(dot(normalize(g_vLightDirection), -vNormal));

	//fShade							= floor(fShade * CELL_SHADE_FREQUENCY) / CELL_SHADE_FREQUENCY;
	
	Out.vDiffuse = g_vLightDiffuse * fShade;
	Out.vAmbient = (fShade * 0.5f + 0.5f);
	
	float4			vPosition;
	
	vPosition.x		= In.vTexCoord.x * +2.f - 1.f;
	vPosition.y		= In.vTexCoord.y * -2.f + 1.f;
	vPosition.z		= vDepthTarget.x;
	vPosition.w		= 1.f;
	
	vPosition		*= fViewZ;
	vPosition		= mul(vPosition, g_mProjInv);
	vPosition		= mul(vPosition, g_mViewInv);
	
	float4			vLook			= vPosition - g_vCamPosition;
	float4			vHalfWay		= normalize(normalize(g_vLightDirection) + normalize(-vLook));

//	
//	Out.vSpecular	= pow(abs(dot(vNormal, vHalfWay)), g_fMtrlShininess);
	Out.vSpecular	= g_vLightSpecular * pow(saturate(dot(vNormal, vHalfWay)), g_fMtrlShininess);
    Out.vSpecular.a = 1.f;

	return Out;
}

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL_SSAO(PS_IN In)
{
	PS_OUT_LIGHT Out;
	
	float4 vNormalTarget = g_texNormalTarget.Sample(TargetSampler, In.vTexCoord);
	float4 vNormal = vNormalTarget * 2.f - 1.f;
	
	float4 vDepthTarget = g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
	float fViewZ = vDepthTarget.y * g_fCamFar;

	float fShade = saturate(dot(normalize(g_vLightDirection), -vNormal));

	//fShade = floor(fShade * CELL_SHADE_FREQUENCY) / CELL_SHADE_FREQUENCY;
	
    Out.vDiffuse = max(0.5f, g_vLightDiffuse * fShade);
	
	float4 vAmbient = g_texSSAOTarget.Sample(TargetSampler, In.vTexCoord);
	Out.vAmbient = (fShade * 0.5f + 0.5f) * vAmbient;
	
	float4 vPosition;
	
	vPosition.x = In.vTexCoord.x * +2.f - 1.f;
	vPosition.y = In.vTexCoord.y * -2.f + 1.f;
	vPosition.z = vDepthTarget.x;
	vPosition.w = 1.f;
	
	vPosition *= fViewZ;
	vPosition = mul(vPosition, g_mProjInv);
	vPosition = mul(vPosition, g_mViewInv);
	
	float4 vLook = vPosition - g_vCamPosition;
	float4 vHalfWay = normalize(normalize(g_vLightDirection) + normalize(-vLook));
	
	Out.vSpecular = g_vLightSpecular * pow(saturate(dot(vNormal, vHalfWay)), 3.f);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT	Out;
	
	float4			vNormalTarget	= g_texNormalTarget.Sample(TargetSampler, In.vTexCoord);
	float4			vNormal			= vNormalTarget * 2.f - 1.f;
	
	float4			vDepthTarget	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
	float			fViewZ			= vDepthTarget.y * g_fCamFar;
	
	float4			vPosition;
	
	vPosition.x		= In.vTexCoord.x * +2.f - 1.f;
	vPosition.y		= In.vTexCoord.y * -2.f + 1.f;
	vPosition.z		= vDepthTarget.x;
	vPosition.w		= 1.f;
	
	vPosition		*= fViewZ;
	vPosition		= mul(vPosition, g_mProjInv);
	vPosition		= mul(vPosition, g_mViewInv);
	
	float fShade = saturate(dot(normalize(g_vLightDirection), -vNormal));
	//fShade = floor(fShade * CELL_SHADE_FREQUENCY) / CELL_SHADE_FREQUENCY;

	float4			vLightDirection	= vPosition - g_vLightPosition;
	float			fLightDistance	= length(vLightDirection);
	vLightDirection	= normalize(vLightDirection);
	
	float			fSmoothAtt		= smoothstep(0.f, 1.f, (1.f - saturate(fLightDistance / g_fLightRange)));
	if (fSmoothAtt <= 0.f)
		discard;
	
	Out.vDiffuse = g_vLightDiffuse * fSmoothAtt;
    Out.vAmbient = g_vLightDiffuse * fSmoothAtt;
	
	float4			vLook			= vPosition - g_vCamPosition;
	float4			vHalfWay		= normalize(vLightDirection + normalize(-vLook));
	
	Out.vSpecular	= g_vLightSpecular * pow(saturate(dot(vNormal, vHalfWay)), 3.f);
    Out.vSpecular.a = 1.f;
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_PREPROCESS(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vProcessData		= float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask				= float4(0.f, 0.f, 0.f, 0.f);
	
	float4	vMask			= g_texPreMaskTarget.Sample(TargetSampler, In.vTexCoord);
	float4	vShadow			= g_texShadowProcessTarget.Sample(TargetSampler, In.vTexCoord);
	
//	Pre Process	
	float4	vMtrlDiffuse	= g_texMtrlDiffuseTarget.Sample(LinearSampler, In.vTexCoord);
	if (vMtrlDiffuse.a == 0.f)
	{
		discard;
	}
	float4	vMtrlAmbient	= g_texMtrlAmbientTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vMtrlSpecular	= g_texMtrlSpecularTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vMtrlEmissive	= g_texMtrlEmissiveTarget.Sample(LinearSampler, In.vTexCoord);
	
	float4	vLightDiffuse	= g_texLightDiffuseTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vLightAmbient	= g_texLightAmbientTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vLightSpecular	= g_texLightSpecularTarget.Sample(LinearSampler, In.vTexCoord);
	
    Out.vPreProcess = vMtrlDiffuse * vLightDiffuse
							+ vMtrlAmbient * vLightAmbient
							+ vMtrlSpecular/* * vLightSpecular*/
							+ vMtrlEmissive;
	Out.vPreProcess *=		vShadow;
	
//	Dissolve: 0.1
	if (0.05f < vMask.a && vMask.a < 0.15f)
	{
		float fFluctuationThreshold = vMask.y + sin(g_fGameTime * fTimeFactor) * fEdgeFluctuation;
		
		if (vMask.x < fFluctuationThreshold)
		{
			float fDissolveAmount = smoothstep(fFluctuationThreshold, fFluctuationThreshold - fEdgeFluctuation, vMask.x);
		
			if (vMask.x > fFluctuationThreshold - fEdgeThickness)
			{
			//	float3 vDissolve = vMask.x * vDissolveColor;
			//	Out.vPreProcess.xyz = lerp(Out.vPreProcess.xyz, vDissolve, saturate(fDissolveAmount));
				
				Out.vPreProcess.xyz = lerp(float3(1.f, 0.004f, 0.004f), float3(1.f, 0.009f, 0.006f), saturate(fDissolveAmount)) * 100.f;
				
				return Out;
			}
			discard;
		}
	}
	
	return Out;
}

PS_OUT PS_MAIN_POSTPROCESS(PS_IN In)
{
	PS_OUT Out;
	
	float4	vData	= g_texProcessDataTarget.Sample(TargetSampler, In.vTexCoord);
	float4	vMask	= g_texMaskTarget.Sample(TargetSampler, In.vTexCoord);
	float4	vDepth	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);

//	Distortion: 0.1
	if (0.05f < vMask.a && vMask.a < 0.15f) 
	{
	//	if (vMask.x)
	//	{
	//		Out.vColor = g_texPreProcessTarget.Sample(TargetSampler, In.vTexCoord);
	//	}
	//	else
		{
			float2 vOffset = (vMask.xy * 2.f - 1.f) * AvgGrayScale(vData.rgb) * vMask.z;
			Out.vColor = g_texPreProcessTarget.Sample(TargetSampler, In.vTexCoord + vOffset);
		}

	}
//	Regular 
	else
	{
		Out.vColor = g_texPreProcessTarget.Sample(TargetSampler, In.vTexCoord);
	}
	
//	Fog
	if (g_bFog)
	{
		float	fFogFactor	= saturate(vDepth.y);
		fFogFactor			= smoothstep(g_fFogStart, g_fFogEnd, clamp(fFogFactor, 0.1f, g_fFogEnd));
		fFogFactor			= pow(fFogFactor, g_fFogPower);
		Out.vColor			= lerp(Out.vColor, g_vFogColor, saturate(min(fFogFactor, g_fFogMax) + g_fFogAmbient));
	}

	return Out;
}

PS_OUT PS_MAIN_OUTLINE_COMBINE(PS_IN In)
{
	PS_OUT Out;
	
	float4 vDepth = g_texDepthTarget.Sample(LinearSampler, In.vTexCoord);
	float4 vOutLine = g_texOutLineTarget.Sample(LinearSampler, In.vTexCoord);

	if (vOutLine.r == 0.f)
		discard;
	
	if (vDepth.r != 0.f && vDepth.r < vOutLine.r)
		discard;
	
	Out.vColor.rgb = vOutLine.gba;
	Out.vColor.a = 1.f;
	
	return Out;
}

PS_OUT PS_MAIN_BLOOM_COMBINE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float3 color0 = g_texPreProcessTarget.Sample(LinearSampler, In.vTexCoord).rgb;
	float3 color1 = g_texBloomTarget.Sample(LinearSampler, In.vTexCoord).rgb;
	
	//float3 combined = (1.f - 0.5f) * color0 + 0.5f * color1;
	float3 combined = color0 + color1.rgb;

	Out.vColor = float4(combined, g_texBloomStrengthTarget.Sample(TargetSampler, In.vTexCoord).a);
	
	return Out;
}

PS_OUT PS_MAIN_BLOOM_COMBINE_ALPHA(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float3 color0 = g_texPreProcessTarget.Sample(LinearSampler, In.vTexCoord).rgb;
	float4 color1 = g_texBloomTarget.Sample(LinearSampler, In.vTexCoord);
	
	float colorExist = color1.r + color1.g + color1.b + color1.a;
	//float3 combined = (1.f - color1.a) * color0 + color1.a * color1.rgb;
	float3 combined = color0 + color1.rgb;

	Out.vColor = float4(combined, 1.0f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_COPY(PS_IN In)
{
	PS_OUT_POSTPROCESS Out = (PS_OUT_POSTPROCESS) 0;
	
	float3 copy = g_texBloomTarget.Sample(LinearSampler, In.vTexCoord).rgb;
	Out.vPreProcess = float4(copy, 1.0f);
	
	return Out;
}

PS_OUT PS_MAIN_COMBINE_SHADOW(PS_IN In)
{
	PS_OUT Out;
	
	Out.vColor = float4(1.f, 1.f, 1.f, 1.f);

	float4	vDepthTarget	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
	if (vDepthTarget.w == 1.f)
	{
		discard;
	}
	float	fViewZ			= vDepthTarget.y * g_fCamFar;
	
	float4	vPosition;
	vPosition.x		= In.vTexCoord.x * +2.f - 1.f;
	vPosition.y		= In.vTexCoord.y * -2.f + 1.f;
	vPosition.z		= vDepthTarget.x;
	vPosition.w		= 1.f;
	
	vPosition		*= fViewZ;
	vPosition		= mul(vPosition, g_mProjInv);
	vPosition		= mul(vPosition, g_mViewInv);
	
	float fShadowWeight = 0.f;
	for (int i = 0; i < g_iShadowCount; ++i)
	{
		float4	vShadowView			= mul(vPosition, g_mShadowViews[i]);
		float4	vShadowProj			= mul(vShadowView, g_mShadowProjs[i]);
		
		float2	vShadowUV			= vShadowProj.xy / vShadowProj.w;
		vShadowUV.x					= vShadowUV.x * +0.5f + 0.5f;
		vShadowUV.y					= vShadowUV.y * -0.5f + 0.5f;
		
		if (saturate(vShadowUV.x) != vShadowUV.x
		||	saturate(vShadowUV.y) != vShadowUV.y)
		{
			continue;
		}
		
		for (int y = -vGridSize.y * 0.5f; y <= vGridSize.y * 0.5f; ++y)
		{
			for (int x = -vGridSize.x * 0.5f; x <= vGridSize.x * 0.5f; ++x)
			{
				float4 vShadowDepthTarget = ShadowDepthTargetLiteral(i, vShadowUV + float2(x, y) * vTexel * fTexelScale);
                if (vDepthTarget.z >= 0.9f && vDepthTarget.z <= 1.9f)
                    continue;
				
				if (!vShadowDepthTarget.z && !vDepthTarget.z)
				{
					continue;
				}
				if (vShadowView.z + fBias > vShadowDepthTarget.x * g_fCamFar)
				{
					fShadowWeight += 1.f;
				}
			}
		}
	}
	
	float fShadowAvg = fShadowWeight / (vGridSize.x * vGridSize.y);
	if (fShadowAvg > fMinShadowAvg)
	{
		Out.vColor.rgb *= (1.f - fShadowAvg * fShadowShade);
	}
	
	return Out;
}

PS_OUT PS_RADIAL_BLUR(PS_IN In)
{
    PS_OUT Out;
	
    In.vTexCoord -= float2(0.5f, 0.5f);
    float fPreCompute = 0.1f * (1.0 / float(g_fRadialBlurSampleLevel - 1));
    
    float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < g_fRadialBlurSampleLevel; i++)
    {
        float fScale = 1.f + (float(i) * fPreCompute);
        float2 vTexCoord = In.vTexCoord * fScale + float2(0.5f, 0.5f);
        clip(vTexCoord);
        vColor += g_texPreProcessTarget.Sample(TargetSampler, vTexCoord);
    }

    vColor /= float(g_fRadialBlurSampleLevel);
    
    Out.vColor = vColor;
	
    return Out;
}

technique11 DefaultTechnique
{
	pass Directional
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_DIRECTIONAL();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Accumulate, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass Point
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_POINT();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Accumulate, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass PreProcess
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_PREPROCESS();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass PostProcess
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_POSTPROCESS();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass OutLine_Combine
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_OUTLINE_COMBINE();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Bloom_Combine
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLOOM_COMBINE();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass Copy
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COPY();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Shadow_Combine
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_COMBINE_SHADOW();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Accumulate, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass Directional_SSAO
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL_SSAO();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Accumulate, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass Bloom_Combine_Alpha
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLOOM_COMBINE_ALPHA();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

    pass RADIAL_BLUR // 10
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RADIAL_BLUR();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
}

float4 ShadowDepthTargetLiteral(int iIndex, float2 vUV)
{
	switch (iIndex)
	{
		case 0:
			return g_texShadowDepthTarget[0].SampleLevel(TargetSampler, vUV, 0);
		case 1:
			return g_texShadowDepthTarget[1].SampleLevel(TargetSampler, vUV, 0);
		case 2:
			return g_texShadowDepthTarget[2].SampleLevel(TargetSampler, vUV, 0);
		case 3:
			return g_texShadowDepthTarget[3].SampleLevel(TargetSampler, vUV, 0);
		case 4:
			return g_texShadowDepthTarget[4].SampleLevel(TargetSampler, vUV, 0);
		case 5:
			return g_texShadowDepthTarget[5].SampleLevel(TargetSampler, vUV, 0);
		case 6:
			return g_texShadowDepthTarget[6].SampleLevel(TargetSampler, vUV, 0);
		case 7:
			return g_texShadowDepthTarget[7].SampleLevel(TargetSampler, vUV, 0);
		case 8:
			return g_texShadowDepthTarget[8].SampleLevel(TargetSampler, vUV, 0);
		case 9:
			return g_texShadowDepthTarget[9].SampleLevel(TargetSampler, vUV, 0);
		case 10:
			return g_texShadowDepthTarget[10].SampleLevel(TargetSampler, vUV, 0);
		case 11:
			return g_texShadowDepthTarget[11].SampleLevel(TargetSampler, vUV, 0);
		case 12:
			return g_texShadowDepthTarget[12].SampleLevel(TargetSampler, vUV, 0);
		case 13:
			return g_texShadowDepthTarget[13].SampleLevel(TargetSampler, vUV, 0);
		case 14:
			return g_texShadowDepthTarget[14].SampleLevel(TargetSampler, vUV, 0);
		case 15:
			return g_texShadowDepthTarget[15].SampleLevel(TargetSampler, vUV, 0);
		case 16:
			return g_texShadowDepthTarget[16].SampleLevel(TargetSampler, vUV, 0);
		case 17:
			return g_texShadowDepthTarget[17].SampleLevel(TargetSampler, vUV, 0);
		case 18:
			return g_texShadowDepthTarget[18].SampleLevel(TargetSampler, vUV, 0);
		case 19:
			return g_texShadowDepthTarget[19].SampleLevel(TargetSampler, vUV, 0);
		case 20:
			return g_texShadowDepthTarget[20].SampleLevel(TargetSampler, vUV, 0);
		case 21:
			return g_texShadowDepthTarget[21].SampleLevel(TargetSampler, vUV, 0);
		case 22:
			return g_texShadowDepthTarget[22].SampleLevel(TargetSampler, vUV, 0);
		case 23:
			return g_texShadowDepthTarget[23].SampleLevel(TargetSampler, vUV, 0);
		case 24:
			return g_texShadowDepthTarget[24].SampleLevel(TargetSampler, vUV, 0);
		case 25:
			return g_texShadowDepthTarget[25].SampleLevel(TargetSampler, vUV, 0);
		case 26:
			return g_texShadowDepthTarget[26].SampleLevel(TargetSampler, vUV, 0);
		case 27:
			return g_texShadowDepthTarget[27].SampleLevel(TargetSampler, vUV, 0);
		case 28:
			return g_texShadowDepthTarget[28].SampleLevel(TargetSampler, vUV, 0);
		case 29:
			return g_texShadowDepthTarget[29].SampleLevel(TargetSampler, vUV, 0);
		case 30:
			return g_texShadowDepthTarget[30].SampleLevel(TargetSampler, vUV, 0);
		case 31:
			return g_texShadowDepthTarget[31].SampleLevel(TargetSampler, vUV, 0);
	}
	return float4(0.f, 0.f, 0.f, 0.f);
}
