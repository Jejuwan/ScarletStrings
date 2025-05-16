#include "ShaderCommon.hlsli"

float	fWorldScale		= 3.f;
float	fTimeScale		= 12.f;
float	fWaveScale		= 0.1f;
float	fOffsetScale	= 1.f;
float	fFlowTimeScale	= 1.f;
float	fFlowScale		= 1.f;
float	fMinAlpha		= 0.f;
float	fMaxAlpha		= 1.f;
float4	vOffset			= float4(0.f, 1.f, 2.f, 3.f);

float3 vGizmoColor = { 1.f, 0.68f, 0.785f };

struct VS_IN
{
	float3	vPosition	: POSITION;
	float3	vNormal		: NORMAL;
	float2	vTexCoord	: TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
	float4	vProjPos	: TEXCOORD2;
};

struct VS_OUT_OVERSIZING
{
	float4 vPosition : SV_POSITION;
	float4 vProjPos	 : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out;

	matrix			mWV, mWVP;

	mWV				= mul(g_mWorld, g_mView);
	mWVP			= mul(mWV, g_mProj);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vNormal		= normalize(mul(float4(In.vNormal, 0.f), g_mWorld));
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= mul(float4(In.vPosition, 1.f), g_mWorld);
	Out.vProjPos    = Out.vPosition;

	return Out;
}

VS_OUT_OVERSIZING VS_MAIN_OVERSIZING(VS_IN In)
{
	VS_OUT_OVERSIZING Out;

	matrix mWV, mWVP;

	mWV = mul(g_mWorld, g_mView);
	mWVP = mul(mWV, g_mProj);

	float3 vLocalPosition = In.vPosition + normalize(In.vNormal) * g_fOutLineSize;
	
	Out.vPosition = mul(float4(vLocalPosition, 1.f), mWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}

VS_OUT VS_MAIN_WATER(VS_IN In)
{
	VS_OUT Out;
	
//	float4x4 mWV	= mul(g_mWorld, g_mView);
//	float4x4 mWVP	= mul(mWV, g_mProj);
//	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	
	Out.vPosition	= mul(float4(In.vPosition, 1.f), g_mWorld);
	Out.vPosition.x += sin(Out.vPosition.x * fWorldScale + g_fGameTime * fTimeScale + vOffset.x * fOffsetScale) * fWaveScale;
	Out.vPosition.y += sin(Out.vPosition.y * fWorldScale + g_fGameTime * fTimeScale + vOffset.y * fOffsetScale) * fWaveScale;
	Out.vPosition.z += sin(Out.vPosition.z * fWorldScale + g_fGameTime * fTimeScale + vOffset.z * fOffsetScale) * fWaveScale;
	Out.vPosition	= mul(Out.vPosition, g_mView);
	Out.vPosition	= mul(Out.vPosition, g_mProj);
	
	Out.vNormal		= normalize(mul(float4(In.vNormal, 0.f), g_mWorld));
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= mul(float4(In.vPosition, 1.f), g_mWorld);
	Out.vProjPos    = Out.vPosition;
	
	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
	float4	vProjPos	: TEXCOORD2;
};

struct PS_IN_OVERSIZING
{
	float4 vPosition : SV_POSITION;
	float4 vProjPos : TEXCOORD0;
};

struct PS_OUT
{
//	float4	vColor		: SV_TARGET0;
	float4	vDiffuse	: SV_TARGET0;
	float4	vAmbient	: SV_TARGET1;
	float4	vSpecular	: SV_TARGET2;
	float4	vEmissive	: SV_TARGET3;
	float4	vNormal		: SV_TARGET4;
	float4	vDepth		: SV_TARGET5;
	float4	vMask		: SV_TARGET6;
};

struct PS_OUT_OVERSIZING
{
	float4 vOutLineColor : SV_TARGET0;
};

struct PS_OUT_POSTPROCESS
{
	float4 vPreProcess : SV_TARGET0;
	float4 vProcessData : SV_TARGET1;
	float4 vMask : SV_TARGET2;
};

struct PS_OUT_BLOOM
{
    float4 vTexture : SV_TARGET0;
    float4 vStrength : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out;
	
	float3	vFinalColor;
	
	float4	vTexColor		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	// Normal
	float3	vNormal			=	In.vNormal.xyz;
	
	Out.vDiffuse	= vTexColor * g_vMtrlDiffuse;
	Out.vAmbient	= vTexColor * g_vMtrlAmbient;
	Out.vSpecular	= g_vMtrlSpecular;
	Out.vEmissive	= g_vMtrlEmissive;
	Out.vNormal		= float4(vNormal, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, In.vProjPos.z);
	Out.vMask		= float4(0.f, 0.f, 0.f, 0.f);
	
//	// Specular
//	float3	vViewDir		=	normalize(g_vCamPosition - In.vWorldPos).xyz;
//	
//	for (int i = 0; i < g_iLightCount; ++i)
//	{
//		// Lighting
//		float3	vLightDir	=	float3(0.f, 0.f, 0.f);
//		switch (g_iLightType[i])
//		{
//		case POINT:
//			vLightDir		=	normalize(In.vWorldPos - g_vLightPosition[i]).xyz;
//			break;
//		case DIRECTIONAL:
//			vLightDir		=	normalize(g_vLightDirection[i]).xyz;
//			break;
//		}
//		
//		float	fDiffuse	=	saturate(dot(-vLightDir, vNormal) + DIFFUSE_OFFSET);
//		
//		// Specular
//		float3	vHalfDir	=	normalize(vViewDir - vLightDir);
//		float	fSpc		=	pow(max(dot(vViewDir, vHalfDir), 0.f), g_fMtrlShininess);
//		
//		// Attenuation
//		float	fSmoothAtt	= 1.f;
//		if (POINT == g_iLightType[i])
//		{
//			float	fDist		=	distance(g_vLightPosition[i], In.vWorldPos);
//			float	fAtt		=	1.f / (g_fLightAttenuation0[i] + g_fLightAttenuation1[i] * fDist + g_fLightAttenuation2[i] * fDist * fDist);
//			float	fLightDist	=	1.f - saturate(fDist / g_fLightRange[i]);
//			fSmoothAtt			=	smoothstep(0.f, 1.f, fAtt * fLightDist);
//		}
//		
//		// Final
//		vFinalColor			+=	vTexColor.rgb * (g_vLightDiffuse[i].rgb * fDiffuse + g_vLightAmbient[i].rgb)
//							+	fSpc * g_vLightSpecular[i].rgb * g_vMtrlSpecular.rgb;
//		
//		vFinalColor			*=	fSmoothAtt;
//	}
//	
//	// Final
//	vFinalColor			+=	g_vMtrlEmissive.rgb;
//	
//	// Out
//	Out.vColor.rgb		= vFinalColor;
//	Out.vColor.a		= vTexColor.a;
	return Out;
}

PS_OUT PS_TERRAIN(PS_IN In)
{
	PS_OUT	Out;
	
	float3	vFinalColor;
	
	float4	vTexColor		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * 32.f);
	// Normal
	float3	vNormal			=	In.vNormal.xyz;
	
	Out.vDiffuse	= vTexColor * g_vMtrlDiffuse;
	Out.vAmbient	= vTexColor * g_vMtrlAmbient;
	Out.vSpecular	= g_vMtrlSpecular;
	Out.vEmissive	= g_vMtrlEmissive;
	Out.vNormal		= float4(vNormal, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, In.vProjPos.z);
	Out.vMask		= float4(0.f, 0.f, 0.f, 0.f);
	
//	// Specular
//	float3	vViewDir		=	normalize(g_vCamPosition - In.vWorldPos).xyz;
//	
//	for (int i = 0; i < g_iLightCount; ++i)
//	{
//		// Lighting
//		float3	vLightDir	=	float3(0.f, 0.f, 0.f);
//		switch (g_iLightType[i])
//		{
//		case POINT:
//			vLightDir		=	normalize(In.vWorldPos - g_vLightDirection[i]).xyz;
//			break;
//		case DIRECTIONAL:
//			vLightDir		=	normalize(g_vLightDirection[i]).xyz;
//			break;
//		}
//		
//		float	fDiffuse	=	saturate(dot(-vLightDir, vNormal) + DIFFUSE_OFFSET);
//		
//		// Specular
//		float3	vHalfDir	=	normalize(vViewDir - vLightDir);
//		float	fSpc		=	pow(max(dot(vViewDir, vHalfDir), 0.f), g_fMtrlShininess);
//		
//		// Attenuation
//		float	fSmoothAtt	= 1.f;
//		if (POINT == g_iLightType[i])
//		{
//			float	fDist		=	distance(g_vLightPosition[i], In.vWorldPos);
//			float	fAtt		=	1.f / (g_fLightAttenuation0[i] + g_fLightAttenuation1[i] * fDist + g_fLightAttenuation2[i] * fDist * fDist);
//			float	fLightDist	=	1.f - saturate(fDist / g_fLightRange[i]);
//			fSmoothAtt			=	smoothstep(0.f, 1.f, fAtt * fLightDist);
//		}
//		
//		// Final
//		vFinalColor			+=	vTexColor.rgb * (g_vLightDiffuse[i].rgb * g_vMtrlDiffuse.rgb * fDiffuse + g_vLightAmbient[i].rgb * g_vMtrlAmbient.rgb)
//							+	g_vLightSpecular[i].rgb * g_vMtrlSpecular.rgb * fSpc;
//		
//		vFinalColor			*=	fSmoothAtt;
//	}
//	
//	// Final
//	vFinalColor			+=	g_vMtrlEmissive.rgb;
//	
//	// Out
//	Out.vColor.rgb		=	vFinalColor;
//	Out.vColor.a		=	vTexColor.a;

	return Out;
}

PS_OUT_OVERSIZING PS_MAIN_OVERSIZING(PS_IN_OVERSIZING In)
{
	PS_OUT_OVERSIZING Out;
	
	Out.vOutLineColor = vector(In.vProjPos.z / In.vProjPos.w, g_vOutlineColor);

	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_WATER(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float2	vNoise		= g_texMask.Sample(LinearSampler, In.vTexCoord).xy;
//	float3	vNormal		= g_texNormal[0].Sample(LinearSampler, In.vTexCoord+ vNoise * (sin(g_fGameTime * fFlowTimeScale) + 1.f) * fFlowScale).xyz;
	float4	vDiffuse	= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord + vNoise * (sin(g_fGameTime * fFlowTimeScale) + 1.f) * fFlowScale);
//	vNormal.z = sqrt(1.f - saturate(dot(vNormal.xy, vNormal.xy)));
//	vNormal = vNormal.xyz * 2.f - 1.f;
	
	float3	vViewDir	= normalize(g_vCamPosition - In.vWorldPos).xyz;
	float	fRim		= dot(In.vNormal.xyz, vViewDir);
	float	fAlpha		= saturate(pow(1.f - fRim, 1.f) + 0.1f);
	
	Out.vPreProcess = vDiffuse * g_vMtrlDiffuse;
    Out.vPreProcess.a *= fMinAlpha + smoothstep(0.f, fMaxAlpha, fAlpha);
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);

	return Out;
}

PS_OUT_BLOOM PS_MAIN_WATER_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out;
	
	float2	vNoise		= g_texMask.Sample(LinearSampler, In.vTexCoord).xy;
//	float3	vNormal		= g_texNormal[0].Sample(LinearSampler, In.vTexCoord+ vNoise * (sin(g_fGameTime * fFlowTimeScale) + 1.f) * fFlowScale).xyz;
	float4	vDiffuse	= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord + vNoise * (sin(g_fGameTime * fFlowTimeScale) + 1.f) * fFlowScale);
//	vNormal.z = sqrt(1.f - saturate(dot(vNormal.xy, vNormal.xy)));
//	vNormal = vNormal.xyz * 2.f - 1.f;
	
	float3	vViewDir	= normalize(g_vCamPosition - In.vWorldPos).xyz;
	float	fRim		= dot(In.vNormal.xyz, vViewDir);
	float	fAlpha		= saturate(pow(1.f - fRim, 1.f) + 0.1f);
	
	Out.vTexture		= vDiffuse * g_vMtrlDiffuse * fMinAlpha + smoothstep(0.f, fMaxAlpha, fAlpha);
//	Out.vStrength		= float4(1.f, 1.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a		= g_fBloomStrength;
	Out.vStrength		= float4(g_fBloomStrength, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_GIZMO(PS_IN In)
{
    PS_OUT Out;
	
    float4 vColor = float4(vGizmoColor, 1.f);
    float3 vNormal = In.vNormal.xyz;
	
    Out.vDiffuse = vColor;
    Out.vAmbient = vColor * 0.5f;
    Out.vSpecular = float4(0.f, 0.f, 0.f, 0.f);
    Out.vEmissive = float4(0.f, 0.f, 0.f, 0.f);
    Out.vNormal = float4(vNormal, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, In.vProjPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);

    return Out;
}
	
technique11 DefaultTechnique
{
	pass DEFAULT
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

	pass TERRAIN
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_TERRAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass OUTLINE
	{
		VertexShader = compile vs_5_0 VS_MAIN_OVERSIZING();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_OVERSIZING();

		SetRasterizerState(RS_InverseCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass WATER
	{
		VertexShader	= compile vs_5_0 VS_MAIN_WATER();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_WATER();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

    pass WATER_BLOOM
    {
		VertexShader	= compile vs_5_0 VS_MAIN_WATER();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_WATER_BLOOM();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

    pass GIZMO
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GIZMO();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
}
