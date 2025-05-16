#include "ShaderCommon.hlsli"

Texture2D g_texDepthTarget;
Texture2D g_texDecalMask;
float4x4 g_mWorldInv;

float g_fNormalizedLifeTime;

float3 g_vDecalColor;

float g_fTilingFactor;

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
	float4	vClipPos	: TEXCOORD1;
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
    Out.vClipPos	= Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float2	vTexCoord	: TEXCOORD0;
    float4	vClipPos	: TEXCOORD1;
};

struct PS_OUT
{
	float4	vDiffuse	: SV_TARGET0;
};

struct PS_OUT_BLOOM
{
    float4 vTexture : SV_TARGET0;
    float4 vBloomStrength : SV_TARGET1;
};

struct PS_OUT_DEFERRED
{
    float4 vDiffuse : SV_TARGET0;
    float4 vAmbient : SV_TARGET1;
    float4 vSpecular : SV_TARGET2;
    float4 vEmissive : SV_TARGET3;
    float4 vNormal : SV_TARGET4;
    float4 vDepth : SV_TARGET5;
    float4 vMask : SV_TARGET6;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out;
	
    float2 vTexUV;
    vTexUV.x = (In.vClipPos.x / In.vClipPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vClipPos.y / In.vClipPos.w) * -0.5f + 0.5f;
	
    float4 vPixelDepth = g_texDepthTarget.Sample(PointSampler, vTexUV);
	
    vector vPosition;
    vPosition.x = vTexUV.x * 2.f - 1.f;
    vPosition.y = vTexUV.y * -2.f + 1.f;
    vPosition.z = vPixelDepth.r;
    vPosition.w = 1.f;
	
    vPosition = vPosition * vPixelDepth.a;
    vPosition = mul(vPosition, g_mProjInv);
    vPosition = mul(vPosition, g_mViewInv);
	
    vector vLocalPos = mul(vPosition, g_mWorldInv);
	
    clip(0.5 - abs(vLocalPos.xyz));
	
    float2 vDecalUV = (vLocalPos.xy + 0.5f);
	
    Out.vDiffuse = g_texDiffuse[0].Sample(LinearSampler, vDecalUV) * g_vMtrlDiffuse;
	
	return Out;
}

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vDiffuse = float4(0.f, 1.f, 0.f, 1.f);

    return Out;
}

PS_OUT PS_DECAL_MASKING(PS_IN In)
{
    PS_OUT Out;
	
    float2 vTexUV;
    vTexUV.x = (In.vClipPos.x / In.vClipPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vClipPos.y / In.vClipPos.w) * -0.5f + 0.5f;
	
    float4 vPixelDepth = g_texDepthTarget.Sample(PointSampler, vTexUV);
    if (vPixelDepth.b < 0.9f)
    {
        discard;
    }
	
    vector vPosition;
    vPosition.x = vTexUV.x * 2.f - 1.f;
    vPosition.y = vTexUV.y * -2.f + 1.f;
    vPosition.z = vPixelDepth.r;
    vPosition.w = 1.f;
	
    vPosition = vPosition * vPixelDepth.a;
    vPosition = mul(vPosition, g_mProjInv);
    vPosition = mul(vPosition, g_mViewInv);
	
    vector vLocalPos = mul(vPosition, g_mWorldInv);
	
    clip(0.5 - abs(vLocalPos.xyz));
	
    float2 vDecalUV = (vLocalPos.xy + 0.5f);
    
    float fDecalMask = g_texDecalMask.Sample(LinearSampler, vDecalUV).r;
    if (fDecalMask == 0.f)
        discard;
	
    Out.vDiffuse = g_texDiffuse[0].Sample(LinearSampler, vDecalUV) * g_vMtrlDiffuse;
    Out.vDiffuse.a = 1.f - g_fNormalizedLifeTime;
    
    return Out;
}

PS_OUT_BLOOM PS_DECAL_BLOOM(PS_IN In)
{
    PS_OUT_BLOOM Out;
	
    float2 vTexUV;
    vTexUV.x = (In.vClipPos.x / In.vClipPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vClipPos.y / In.vClipPos.w) * -0.5f + 0.5f;
	
    float4 vPixelDepth = g_texDepthTarget.Sample(PointSampler, vTexUV);
    if (vPixelDepth.b < 0.9f)
    {
        discard;
    }
	
    vector vPosition;
    vPosition.x = vTexUV.x * 2.f - 1.f;
    vPosition.y = vTexUV.y * -2.f + 1.f;
    vPosition.z = vPixelDepth.r;
    vPosition.w = 1.f;
	
    vPosition = vPosition * vPixelDepth.a;
    vPosition = mul(vPosition, g_mProjInv);
    vPosition = mul(vPosition, g_mViewInv);
	
    vector vLocalPos = mul(vPosition, g_mWorldInv);
	
    clip(0.5 - abs(vLocalPos.xyz));
	
    float2 vDecalUV = (vLocalPos.xy + 0.5f);
    
    float fDecalMask = g_texDecalMask.Sample(LinearSampler, vDecalUV).a;
    if (fDecalMask == 0.f)
        discard;
	
    Out.vTexture.rgb = g_vDecalColor;
    Out.vTexture.a = 1.f - g_fNormalizedLifeTime;
    Out.vBloomStrength = float4(g_fBloomStrength, 0.f, 0.f, 0.f);
    
    return Out;
}

PS_OUT PS_DECAL_TILING(PS_IN In)
{
    PS_OUT Out;
	
    float2 vTexUV;
    vTexUV.x = (In.vClipPos.x / In.vClipPos.w) * 0.5f + 0.5f;
    vTexUV.y = (In.vClipPos.y / In.vClipPos.w) * -0.5f + 0.5f;
	
    float4 vPixelDepth = g_texDepthTarget.Sample(PointSampler, vTexUV);
    if (vPixelDepth.b < 0.9f)
    {
        discard;
    }
    
    vector vPosition;
    vPosition.x = vTexUV.x * 2.f - 1.f;
    vPosition.y = vTexUV.y * -2.f + 1.f;
    vPosition.z = vPixelDepth.r;
    vPosition.w = 1.f;
	
    vPosition = vPosition * vPixelDepth.a;
    vPosition = mul(vPosition, g_mProjInv);
    vPosition = mul(vPosition, g_mViewInv);
	
    vector vLocalPos = mul(vPosition, g_mWorldInv);
	
    clip(0.5 - abs(vLocalPos.xyz));
	
    float2 vDecalUV = (vLocalPos.xy + 0.5f);
    
    float fDecalMask = (1.f - g_texDecalMask.Sample(LinearSampler, vDecalUV).a);
    if (fDecalMask == 0.f)
        discard;
	
    Out.vDiffuse = g_texDiffuse[0].Sample(LinearSampler, vDecalUV * g_fTilingFactor) * g_vMtrlDiffuse;
    Out.vDiffuse.a = fDecalMask * g_vMtrlDiffuse.a;
    
    return Out;
}

PS_OUT_DEFERRED PS_DECAL_TARGET(PS_IN In)
{
    PS_OUT_DEFERRED Out;
	
    Out.vDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    Out.vAmbient = float4(0.f, 0.f, 0.f, 0.f);
    Out.vSpecular = float4(0.f, 0.f, 0.f, 0.f);
    Out.vEmissive = float4(0.f, 0.f, 0.f, 0.f);
    Out.vNormal = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDepth = float4(In.vClipPos.z / In.vClipPos.w, In.vClipPos.w / g_fCamFar, 1.f, In.vClipPos.z);
    Out.vMask = float4(0.f, 0.f, 0.f, 0.f);
	
    return Out;
}
	
technique11 DefaultTechnique
{
	pass DEFAULT // 0
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass DEBUG // 1
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEBUG();

        SetRasterizerState(RS_WireFrame);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass MASKING_DECAL // 2
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DECAL_MASKING();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass BLOOM_DECAL // 3
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DECAL_BLOOM();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass ADDITIVE_BLEND_MASKING_TILING // 4
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DECAL_TILING();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

    pass DECAL_TARGET // 5
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DECAL_TARGET();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }
}
