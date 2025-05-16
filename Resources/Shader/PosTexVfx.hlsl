#include "ShaderCommon.hlsli"

float	g_fDistortionPower	= 0.1f;

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

struct VS_OUT_WORLD
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
};

struct VS_OUT_ORTHOGRAPHIC
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float2	vLocalPos	: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	float4x4 mWV	= mul(g_mWorld, g_mView);
	float4x4 mWVP	= mul(mWV, g_mProj);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vTexCoord	= In.vTexCoord;

	return Out;
}

VS_OUT_WORLD VS_WORLD(VS_IN In)
{
	VS_OUT_WORLD Out;

	float4x4 mWV	= mul(g_mWorld, g_mView);
	float4x4 mWVP	= mul(mWV, g_mProj);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= mul(float4(In.vPosition, 1.f), g_mWorld);

	return Out;
}

VS_OUT_ORTHOGRAPHIC VS_ORTHOGRAPHIC(VS_IN In)
{
	VS_OUT_ORTHOGRAPHIC	Out;
	
	Out.vPosition	= mul(float4(In.vPosition, 1.f), mul(g_mWorld, g_mOrthographic));
	Out.vTexCoord	= In.vTexCoord;
	Out.vLocalPos	= In.vPosition.xy;

	return Out;
}

VS_OUT VS_BILLBOARD(VS_IN In)
{
	VS_OUT	Out;
	
    float3		vScale		= float3(length(g_mWorld[0].xyz), length(g_mWorld[1].xyz), length(g_mWorld[2].xyz));
	float3		vViewDir	= normalize(g_mWorld[3].xyz - g_vCamPosition.xyz);
	
//	float4		vRight		= normalize(float4(cross(float3(0.f, 1.f, 0.f), vViewDir), 0.f)) * vScale.x;
//	float4		vRight		= normalize(float4(cross(normalize(g_mWorld[1].xyz), -vViewDir), 0.f)) * vScale.x;
//	float4		vUp			= normalize(float4(cross(vViewDir, vRight.xyz), 0.f)) * vScale.y;
//	float4		vLook		= normalize(float4(vViewDir, 0.f)) * vScale.z;
	
	float4		vLook		= normalize(float4(vViewDir, 0.f));
	float4		vRight		= normalize(float4(cross(normalize(g_mWorld[1].xyz), vLook.xyz), 0.f));
	float4		vUp			= normalize(float4(cross(vLook.xyz, vRight.xyz), 0.f));
	
	float4x4	mWorld		= float4x4(vRight * vScale.x, vUp * vScale.y, vLook * vScale.z, g_mWorld[3]);
	
	float4x4	mWV			= mul(mWorld, g_mView);
	float4x4	mWVP		= mul(mWV, g_mProj);
	
	Out.vPosition			= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vTexCoord			= In.vTexCoord;
	
	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
};

struct PS_IN_WORLD
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
};

struct PS_IN_ORTHOGRAPHIC
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float2	vLocalPos	: TEXCOORD1;
};

struct PS_OUT
{
	float4	vColor		: SV_TARGET0;
};

struct PS_OUT_POSTPROCESS
{
	float4 vPreProcess	: SV_TARGET0;
	float4 vProcessData	: SV_TARGET1;
	float4 vMask		: SV_TARGET2;
};

struct PS_OUT_BLOOM
{
    float4 vTexture : SV_TARGET0;
    float4 vData : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out;

	Out.vColor	=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
        Out.vColor.rgb += vEmissive.rgb * vEmissive.a;
    }
	
    Out.vColor.a *= g_fFadeRatio;
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out;

	Out.vTexture	= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;

    Out.vTexture.a	= g_fFadeRatio;
//	Out.vData		= float4(1.f, 1.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a	= g_fBloomStrength * g_fFadeRatio;
	Out.vData        = float4(g_fBloomStrength * g_fFadeRatio, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT PS_SPRITESHEET(PS_IN In)
{
	PS_OUT Out;
 	
	Out.vColor		=	SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, g_iSpriteSheetIndex) * g_vMtrlDiffuse;
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        float4 vEmissive	= SpriteSheet(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, g_iSpriteSheetIndex) * g_vMtrlEmissive;
        Out.vColor.rgb	+= vEmissive.rgb * vEmissive.a;
    }
    
	Out.vColor.a	*= g_fFadeRatio;
	
	return Out;
}

PS_OUT_BLOOM PS_SPRITESHEET_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out;
	
	Out.vTexture		= SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, g_iSpriteSheetIndex) * g_vMtrlDiffuse;
	Out.vTexture.a		*= g_fFadeRatio;
//	Out.vData			= float4(0.f, 0.f, 0.f, Out.vTexture.a);
//	Out.vTexture.a		= g_fBloomStrength * g_fFadeRatio;
	Out.vData			= float4(g_fBloomStrength * g_fFadeRatio, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT PS_SPRITESHEET_GRAYSCALE(PS_IN In)
{
	PS_OUT Out;
	
	Out.vColor		=	SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, g_iSpriteSheetIndex);
	Out.vColor.a	=	AvgGrayScale(Out.vColor.rgb);
	Out.vColor		*=  g_vMtrlDiffuse;
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        float4 vEmissive = SpriteSheet(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, g_iSpriteSheetIndex) * g_vMtrlEmissive;
        Out.vColor.rgb += vEmissive.rgb * vEmissive.a;
    }
	
    Out.vColor.a	*=	g_fFadeRatio;

	return Out;
}

PS_OUT_POSTPROCESS PS_DISTORTION(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
//	Out.vPreProcess	=	SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, g_iSpriteSheetIndex);
//	Out.vPreProcess	*=	g_vMtrlDiffuse;
	Out.vPreProcess		=	float4(0.f, 0.f, 0.f, 0.f);
    Out.vProcessData	=	SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, g_iSpriteSheetIndex);
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        Out.vProcessData *= g_texMask.Sample(LinearSampler, In.vTexCoord * g_fTilingEmissive[0]);
    }
	Out.vMask			=	float4(In.vTexCoord.xy, g_fDistortionPower, 0.1f);

	return Out;
}

technique11 DefaultTechnique
{
	// 0
	pass Default
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 1
    pass Orthographic
    {
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 2
    pass Billboard
    {
		VertexShader	= compile vs_5_0 VS_BILLBOARD();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 3
    pass Billboard_SpriteSheet_Additive
    {
		VertexShader	= compile vs_5_0 VS_BILLBOARD();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 4
    pass Billboard_SpriteSheet_GrayScale
    {
		VertexShader	= compile vs_5_0 VS_BILLBOARD();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 5
    pass Distortion
    {
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_DISTORTION();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 6
    pass SpriteSheet
    {
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 7
    pass SpriteSheet_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 8
    pass BLOOM
    {
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 9
    pass BLOOM_SpriteSheet
    {
        VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_SPRITESHEET_BLOOM();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 10
    pass BLOOM_Billboard
    {
		VertexShader	= compile vs_5_0 VS_BILLBOARD();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 11
    pass BLOOM_Billboard_SpriteSheet
    {
		VertexShader	= compile vs_5_0 VS_BILLBOARD();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_SPRITESHEET_BLOOM();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 12
    pass Billboard_Distortion
    {
		VertexShader	= compile vs_5_0 VS_BILLBOARD();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_DISTORTION();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 13
    pass ADDITIVEBLEND_Billboard
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 14
    pass Billboard_SpriteSheet
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 15
    pass Billboard_SpriteSheet_GrayScale_Additive
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }
}
