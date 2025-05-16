#include "ShaderCommon.hlsli"

Texture2D g_texScopeTarget;
Texture2D g_texUI_3DTarget;

float g_fCrosshairBoundsX;
float g_fCrosshairBoundsY;


// For. Sprite_UI
bool  g_bOnOff;
uint  g_iIndex, g_iIndexMaxX, g_iIndexMaxY;
float g_fTime, g_fTimeAlpha;
float g_fTime_Electric, g_fTime_Acceleration, g_fTime_Clone, g_fTime_Stealth;
float g_fRatio;
float g_fSizeX, g_fSizeY;
float g_fGauge_Electric, g_fGauge_Acceleration, g_fGauge_Clone, g_fGauge_Stealth;


// For. BossName 
float g_fB1_Alpha,  g_fB1_Discard;
float g_fB2_R,      g_fB2_G,        g_fB2_B,        g_fB2_Alpha,    g_fB2_Discard;
float g_fB3_Alpha,  g_fB3_Discard;
float g_fNoise_R,   g_fNoise_G,     g_fNoise_B,     g_fNoise_A;



const float blurSize = 1.0f / 512.0f;
const float intensity = 0.01f;


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
	
    float4 vRight = float4(cross(float3(0.f, 1.f, 0.f), vViewDir), 0.f);
    float4 vUp = float4(cross(vViewDir, vRight.xyz), 0.f);
    float4 vLook = float4(vViewDir, 0.f);
	
    float4x4 mWorld = float4x4(vRight, vUp, vLook, g_mWorld[3]);
	
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

float4 SampleSpriteSheet(Texture2D texSpriteSheet, float2 vTexCoord, uint2 vSize, uint iIndex)
{
    uint2 vRowCol = uint2(iIndex % vSize.x, iIndex / vSize.x);

    float2 Temp = float2(1.f / vSize.x, 1.f / vSize.y) * vRowCol;
    vTexCoord *= float2(1.f / vSize.x, 1.f / vSize.y);
    return texSpriteSheet.Sample(PointSampler, vTexCoord + Temp);                  
}

PS_OUT PS_MAIN(PS_IN In) // 0
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
	
    return Out;
}

PS_OUT PS_UI_SHOP_SUB(PS_IN_ORTHOGRAPHIC In) // 1
{
    PS_OUT Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba *= g_fRatio;
    
    return Out;
}

PS_OUT PS_UI_SHOP_OUTLINE(PS_IN_ORTHOGRAPHIC In) // 2
{
    PS_OUT Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgb = (Out.vColor.g) * vector(225.f / 255.f, 215.f / 255.f, 10.f / 255.f, 1.f);
    
    Out.vColor.a = 0.75f * g_fRatio;
    
    return Out;
}

PS_OUT PS_UI_SHOP_GREENTEX(PS_IN_ORTHOGRAPHIC In) // 3
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    if (Diffuse.g <= 0.2f)
        discard;
	
    Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    Out.vColor.a = Diffuse.g * g_fRatio;
    Out.vColor.rgb = float3(0.8f, 0.8f, 0.f);
    
    return Out;
}

PS_OUT PS_UI_MENU(PS_IN_ORTHOGRAPHIC In) // 4
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    if (Diffuse.g <= 0.32f)
        discard;
	
    Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    Out.vColor.rgb += (Diffuse.r * 0.15f);
    Out.vColor.a = Diffuse.g * g_fRatio;
	
    return Out;
}

PS_OUT PS_UI_SHOP_CATALOG(PS_IN_ORTHOGRAPHIC In) // 5
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    //if (Diffuse.g <= 0.32f)
    //    discard;
	
    Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    Out.vColor.rgb += (Diffuse.r * 0.15f);
    Out.vColor.rgb *= g_fRatio;
    //Out.vColor.a = Diffuse.g;
    Out.vColor.a = 0.5f * g_fRatio;
	
    return Out;
}

PS_OUT PS_UI_BACKGROUND(PS_IN In) // 6
{
    PS_OUT Out;
        
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
    
    Out.vColor.a = 0.9f;
    
    return Out;
}

PS_OUT PS_UI_K_INFO(PS_IN_ORTHOGRAPHIC In) // 7
{
    PS_OUT Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
    Out.vColor.rgb = (Out.vColor.g) * vector(225.f / 255.f, 215.f / 255.f, 10.f / 255.f, 1.f);
    
    //Out.vColor.rg += (Out.vColor.r * 0.15f);
    
    Out.vColor.a = Out.vColor.g * g_fRatio;
    
    //Out.vColor.rg = float2(0.05f, 0.2f);
	
    return Out;
}

PS_OUT PS_UI_ARROW(PS_IN_ORTHOGRAPHIC In) // 8
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    if (0.3f >= Out.vColor.a)
        discard;
    
    if (0.1f >= Out.vColor.r)
        discard;
    
    Out.vColor.a *= g_fRatio;

    return Out;
}

PS_OUT PS_UI_CHECKBOX(PS_IN_ORTHOGRAPHIC In) // 9
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    Out.vColor.rgb += (Diffuse.r * 0.5f);
    Out.vColor.a = 1.f;
    
    if(Out.vColor.g <0.1f)
        Out.vColor.a = 0.5f;
        
	
    return Out;
}

technique11 DefaultTechnique
{
    pass Default    // 0
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

    pass SHOP_SUB // 1
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SHOP_SUB();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass SHOP_OUTLINE // 2 Yellow
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SHOP_OUTLINE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }    

    pass SHOP_GREENTEX // 3
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SHOP_GREENTEX();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_Menu // 4
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_MENU();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SHOP_CATALOG // 5
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SHOP_CATALOG();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_BACKGROUND // 6
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_BACKGROUND();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
    pass UI_K_INFO // 7
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_K_INFO();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
    pass PS_UI_ARROW // 8
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_ARROW();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
    pass PS_UI_CHECKBOX // 8
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_CHECKBOX();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

}