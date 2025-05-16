#include "ShaderCommon.hlsli"

Texture2D g_texScopeTarget;
Texture2D g_texUI_3DTarget;

float g_fCrosshairBoundsX;
float g_fCrosshairBoundsY;


// For. Sprite_UI
bool  g_bOnOff;
bool  g_bChecker1, g_bChecker2;
uint  g_iIndex, g_iIndexMaxX, g_iIndexMaxY;
float g_fTime, g_fTimeAlpha;
float g_fTime_Electric, g_fTime_Acceleration, g_fTime_Clone, g_fTime_Stealth;
float g_fRatio;
float g_fSizeX, g_fSizeY;
float g_fGauge_Electric, g_fGauge_Acceleration, g_fGauge_Clone, g_fGauge_Stealth;
float g_fOrigin;
float g_fVariable = 1.f;

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

struct PS_OUT_BLOOM
{
    float4 vTexture : SV_TARGET0;
    float4 vData : SV_TARGET1;
};

float4 SampleSpriteSheet(Texture2D texSpriteSheet, float2 vTexCoord, uint2 vSize, uint iIndex)
{
    uint2 vRowCol = uint2(iIndex % vSize.x, iIndex / vSize.x);

    float2 Temp = float2(1.f / vSize.x, 1.f / vSize.y) * vRowCol;
    vTexCoord *= float2(1.f / vSize.x, 1.f / vSize.y);
    return texSpriteSheet.Sample(PointSampler, vTexCoord + Temp);                  
}

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
	
//	Out.vColor.a = 0.2f;
        
    //if (0.6f >= Out.vColor.g)
    //    discard;        
	
    return Out;
}

PS_OUT PS_SPRITESHEET(PS_IN In)
{
    PS_OUT Out;
	
    Out.vColor = SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, g_iSpriteSheetIndex);
    Out.vColor *= g_vMtrlDiffuse;
	
    return Out;
}

PS_OUT PS_UI_HP(PS_IN_ORTHOGRAPHIC In)
{
    PS_OUT Out;

	//Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	//Out.vColor *= g_vMtrlDiffuse;
	//Out.vColor.a = 0.2f;
	//if (Out.vColor.a <= 0.3f)
	//	discard;
	//uint	iIndex = 1;
	//iIndex += g_fTime;
	
    Out.vColor = SampleSpriteSheet(g_texDiffuse[0], In.vTexCoord, uint2(g_iIndexMaxX, g_iIndexMaxY), g_iIndex);
    Out.vColor *= vector(1.f, 0.65f, 0.65f, 1.f);

    return Out;
}

PS_OUT PS_UI_HP_40(PS_IN_ORTHOGRAPHIC In)
{
    PS_OUT Out;

	//Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	//Out.vColor *= g_vMtrlDiffuse;
	//Out.vColor.a = 0.2f;
	//if (Out.vColor.a <= 0.3f)
	//	discard;
	//uint	iIndex = 1;
	//iIndex += g_fTime;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= vector(1.f, 0.65f, 0.65f, 1.f);

    return Out;
}

PS_OUT PS_UI_ITEMBOX(PS_IN_ORTHOGRAPHIC In)
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    if (0.3f >= Out.vColor.a)
        discard;

    return Out;
}

PS_OUT PS_UI_PSYCOWAVE(PS_IN_ORTHOGRAPHIC In)
{
    PS_OUT Out;

    float2 Diffuse = In.vTexCoord;
    
    if (false == g_bOnOff) // Wave
    {
        if (false == g_bChecker1) // Original
        {
            In.vTexCoord.x -= g_fTime;
	
            Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
            Out.vColor *= g_vMtrlDiffuse;
            
            if (g_fVariable < Diffuse.x)
                discard;
        }
        else // Background
        {
            In.vTexCoord.x -= g_fTime;
	
            Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
            Out.vColor *= vector(92.f / 255.f, 6.f / 255.f, 14.f / 255.f, 1.f);
            
            if (g_fVariable < Diffuse.x)
                discard;
        }
    }
    else // Disonance
    {
        if (false == g_bChecker2) // Original
        {
            Out.vColor = SampleSpriteSheet(g_texDiffuse[0], In.vTexCoord, uint2(g_iIndexMaxX, g_iIndexMaxY), g_iIndex);
            Out.vColor *= g_vMtrlDiffuse; /*vector(1.f, 0.65f, 0.65f, 1.f);*/
            
            if (g_fVariable < Diffuse.x)
                discard;
        }
        else // Background
        {
            Out.vColor = SampleSpriteSheet(g_texDiffuse[0], In.vTexCoord, uint2(g_iIndexMaxX, g_iIndexMaxY), g_iIndex);
            Out.vColor *= vector(92.f / 255.f, 6.f / 255.f, 14.f / 255.f, 1.f);
            
            if (g_fVariable < Diffuse.x)
                discard;
        }
    }

	//Out.vColor.a = 0.2f;
	
	//if (Out.vColor.a <= 0.3f)
	//	discard;
	
	//uint	iIndex = 1;
	//iIndex += g_fTime;

    //Out.vColor = SampleSpriteSheet(g_texDiffuse[0], In.vTexCoord, uint2(g_iIndexMaxX, g_iIndexMaxY), g_iIndex);
    //Out.vColor *= vector(1.f, 0.65f, 0.65f, 1.f);

    return Out;
}

PS_OUT PS_UI_MENU_BACKGROUND(PS_IN_ORTHOGRAPHIC In)
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgb = (Diffuse.g) * vector(70.f / 255.f, 65.f / 255.f, 45.f / 255.f, 0.f);
    Out.vColor.rgb -= Diffuse.r * 0.15f; // Black Frame Range
    Out.vColor.a = 0.9f; // Black Frame Alpha
	
	
	
	return Out;
}

PS_OUT PS_UI_MENU_CATEGORY(PS_IN_ORTHOGRAPHIC In) // 8
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	if (Diffuse.g <= 0.32f)
        discard;
    //clip(Diffuse.g - 0.2f);
	
    Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f * g_fTime);
    Out.vColor.rgb += (Diffuse.r * 0.15f);
    //Out.vColor.a = 1 - Diffuse.r;
    Out.vColor.a = Diffuse.g;
	
	
	
    return Out;
}

PS_OUT PS_UI_MENU_LINE(PS_IN_ORTHOGRAPHIC In)
{
    PS_OUT Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
	
	Out.vColor.a = 0.3f;
	
	
    return Out;
}

PS_OUT PS_UI_MENU_SEPERATE_LINE(PS_IN_ORTHOGRAPHIC In)
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    //if (Diffuse.r <= 0.1f)
    //    discard;
	
    Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    Out.vColor.rgb += (Diffuse.r * 0.15f);
	
    Out.vColor.a = Diffuse.g;
	
	if (0.3f >= Out.vColor.a)
        discard;
	
	
    return Out;
}

PS_OUT PS_UI_MENU_PROFILEOFF(PS_IN_ORTHOGRAPHIC In) // 12
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    if (Diffuse.g <= 0.2f)
        discard;
    //clip(Diffuse.g - 0.2f);
	
    Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    Out.vColor.rgb += (Diffuse.r * 0.15f);
    //Out.vColor.a = 1 - Diffuse.r;
    Out.vColor.a = Diffuse.g;
	
	
	
    return Out;
}

PS_OUT PS_UI_INDICATOR_GAUGE(PS_IN_ORTHOGRAPHIC In) // 13
{
    PS_OUT Out;

        
    float2 normalizedCoords = In.vTexCoord - float2(0.5f, 0.5f);
    float angle = atan2(normalizedCoords.y, normalizedCoords.x) + PI;
    
    float startAngle = -PI * 2;
    float endAngle = 2 * PI * g_fRatio;
    
    if (angle < startAngle || angle > endAngle)
        discard;
    
    //float4 vMaskColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    //if (vMaskColor.a == 0.f)
    //{
    //    discard;
    //}
    //Out.vColor = vMaskColor;
    
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    Out.vColor = (Out.vColor.r) * vector(221.f / 255.f, 169.f / 255.f, 239.f / 255.f, 1.f);
    
    if (0.05 >= Out.vColor.r)
        discard;

    
    return Out;
}

PS_OUT PS_UI_ITEMSHADER(PS_IN_ORTHOGRAPHIC In) // 14
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);

    clip(Diffuse.g - 0.2f);
    
    Out.vColor.rgb = (Diffuse.g) * vector(0.f / 255.f, 255.f / 255.f, 0.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    Out.vColor.rgb += (Diffuse.r * 0.15f);
	
    Out.vColor.a = 0.15f;
	
    if (0.1f >= Out.vColor.a)
        discard;
	
	
    return Out;
}

PS_OUT PS_UI_ITEMCOOLTIME(PS_IN_ORTHOGRAPHIC In) // 15
{
    PS_OUT Out;
    
    float2 normalizedCoords = In.vTexCoord- float2(0.5f, 0.5f);
    float angle = atan2(normalizedCoords.y, normalizedCoords.x) +PI; // 그림을 그려주기 시작하는 각을 설정하는 부분
    //angle %= PI;
    // Angle 범위 : -PI ~ PI
        
    //float startAngle = -1 / 3.f * PI - PI * 0.5;
    //float endAngle = 2 * PI * g_fRatio - 1 / 3.f * PI - PI * 0.5;
    float startAngle = 0;
    float endAngle = 2 * PI * g_fRatio;
    
    
    if (angle < startAngle || angle > endAngle)
        discard;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgba = (Diffuse.r) * vector(128.f / 255.f, 250.f / 255.f, 144.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_UI_SKILLLIGHT_Electric(PS_IN_ORTHOGRAPHIC In) // 16
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(PointSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    Out.vColor.a *= g_fTime_Electric;
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    //clip(Out.vColor.a - 0.1f);
    
    return Out;
}

PS_OUT PS_UI_SKILLLIGHT_Acceleration(PS_IN_ORTHOGRAPHIC In) // 17
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(PointSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    Out.vColor.a *= g_fTime_Acceleration;
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    //clip(Out.vColor.a - 0.1f);
    
    return Out;
}

PS_OUT PS_UI_SKILLLIGHT_CLONE(PS_IN_ORTHOGRAPHIC In) // 18
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(PointSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    Out.vColor.a *= g_fTime_Clone;
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    //clip(Out.vColor.a - 0.1f);
    
    return Out;
}

PS_OUT PS_UI_SKILLLIGHT_Stealth(PS_IN_ORTHOGRAPHIC In) // 19
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(PointSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    Out.vColor.a *= g_fTime_Stealth;
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    //clip(Out.vColor.a - 0.1f);
    
    return Out;
}

PS_OUT PS_UI_SKILLCOOL_Electric(PS_IN_ORTHOGRAPHIC In) // 20
{
    PS_OUT Out;
    
    float2 normalizedCoords = In.vTexCoord - float2(0.5f, 0.5f);
    float angle = atan2(normalizedCoords.y, normalizedCoords.x) + PI; // 그림을 그려주기 시작하는 각을 설정하는 부분
    //angle %= PI;
    // Angle 범위 : -PI ~ PI
        
    //float startAngle = -1 / 3.f * PI - PI * 0.5;
    //float endAngle = 2 * PI * g_fRatio - 1 / 3.f * PI - PI * 0.5;
    float startAngle = 0;
    float endAngle = 2 * PI * g_fGauge_Electric;
    
    if (angle < startAngle || angle > endAngle)
        discard;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgba = (Diffuse.r) * vector(905.f / 255.f, 872.f / 255.f, 178.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    
    clip(Diffuse.r);
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_UI_SKILLCOOL_Acceleration(PS_IN_ORTHOGRAPHIC In) // 21
{
    PS_OUT Out;
    
    float2 normalizedCoords = In.vTexCoord - float2(0.5f, 0.5f);
    float angle = atan2(normalizedCoords.y, normalizedCoords.x) + PI; // 그림을 그려주기 시작하는 각을 설정하는 부분
    //angle %= PI;
    // Angle 범위 : -PI ~ PI
        
    //float startAngle = -1 / 3.f * PI - PI * 0.5;
    //float endAngle = 2 * PI * g_fRatio - 1 / 3.f * PI - PI * 0.5;
    float startAngle = 0;
    float endAngle = 2 * PI * g_fGauge_Acceleration;
    
    
    if (angle < startAngle || angle > endAngle)
        discard;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgba = (Diffuse.r) * vector(1155.f / 255.f, 464.f / 255.f, 530.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    
    clip(Diffuse.r);
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_UI_SKILLCOOL_CLONE(PS_IN_ORTHOGRAPHIC In) // 22
{
    PS_OUT Out;
    
    float2 normalizedCoords = In.vTexCoord - float2(0.5f, 0.5f);
    float angle = atan2(normalizedCoords.y, normalizedCoords.x) + PI; // 그림을 그려주기 시작하는 각을 설정하는 부분
    //angle %= PI;
    // Angle 범위 : -PI ~ PI
        
    //float startAngle = -1 / 3.f * PI - PI * 0.5;
    //float endAngle = 2 * PI * g_fRatio - 1 / 3.f * PI - PI * 0.5;
    float startAngle = 0;
    float endAngle = 2 * PI * g_fGauge_Clone;
    
    
    if (angle < startAngle || angle > endAngle)
        discard;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgba = (Diffuse.r) * vector(389.f / 255.f, 411.f / 255.f, 855.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    
    clip(Diffuse.r);
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_UI_SKILLCOOL_Stealth(PS_IN_ORTHOGRAPHIC In) // 23
{
    PS_OUT Out;
    
    float2 normalizedCoords = In.vTexCoord - float2(0.5f, 0.5f);
    float angle = atan2(normalizedCoords.y, normalizedCoords.x) + PI; // 그림을 그려주기 시작하는 각을 설정하는 부분
    //angle %= PI;
    // Angle 범위 : -PI ~ PI
        
    //float startAngle = -1 / 3.f * PI - PI * 0.5;
    //float endAngle = 2 * PI * g_fRatio - 1 / 3.f * PI - PI * 0.5;
    float startAngle = 0;
    float endAngle = 2 * PI * g_fGauge_Stealth;
    

    
    if (angle < startAngle || angle > endAngle)
        discard;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgba = (Diffuse.r) * vector(140.f / 255.f, 838.f / 255.f, 824.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    
    clip(Diffuse.r);
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_UI_HPBLANK(PS_IN_ORTHOGRAPHIC In) // 24
{
    PS_OUT Out;

	//Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	//Out.vColor *= g_vMtrlDiffuse;
	//Out.vColor.a = 0.2f;
	//if (Out.vColor.a <= 0.3f)
	//	discard;
	//uint	iIndex = 1;
	//iIndex += g_fTime;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgb = vector(138.f / 255.f, 25.f / 255.f, 34.f / 255.f, 1.f);
   
    if (true == g_bOnOff)
        discard;

    
    
    if (0.3f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_FADE(PS_IN In) // 25
{
    PS_OUT Out;

    
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
	
	Out.vColor.a = g_fTime;
	
    return Out;
}

PS_OUT PS_UI_3D(PS_IN In) // 26
{
    PS_OUT Out;
        
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
    
    vector vColor = g_texUI_3DTarget.Sample(LinearSampler, In.vTexCoord);
    if (vColor.a == 1.f)
    {
        Out.vColor = vColor;
    }
    
    return Out;
}

PS_OUT PS_BLOOM_Electric(PS_IN In) // 27
{    
    PS_OUT Out;
    
    float4 sum = float4(0.f, 0.f, 0.f, 0.f);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
    //sum += texture(iChannel0, float2(In.vTexCoord.x - 4.0 * blurSize, In.vTexCoord.y)) * 0.05;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - 3.0 * blurSize, In.vTexCoord.y)) * 0.09f;  
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - 2.0 * blurSize, In.vTexCoord.y)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - blurSize, In.vTexCoord.y)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y)) * 0.16;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + blurSize, In.vTexCoord.y)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 2.0 * blurSize, In.vTexCoord.y)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 3.0 * blurSize, In.vTexCoord.y)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 4.0 * blurSize, In.vTexCoord.y)) * 0.05;
	
	// blur in y (vertical)
   // take nine samples, with the distance blurSize between them
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 4.0 * blurSize)) * 0.05;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 3.0 * blurSize)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 2.0 * blurSize)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - blurSize)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y)) * 0.16;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + blurSize)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 2.0 * blurSize)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 3.0 * blurSize)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 4.0 * blurSize)) * 0.05;

    sum + g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * intensity;
    
    Out.vColor = sum;
    
    if (0.6f >= Out.vColor.g || 0.6f >= Out.vColor.b)
        discard;
        
    return Out;
}

PS_OUT PS_BLOOM_Acceleration(PS_IN In) // 28
{
    PS_OUT Out;
    
    float4 sum = float4(0.f, 0.f, 0.f, 0.f);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
    //sum += texture(iChannel0, float2(In.vTexCoord.x - 4.0 * blurSize, In.vTexCoord.y)) * 0.05;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - 3.0 * blurSize, In.vTexCoord.y)) * 0.09f;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - 2.0 * blurSize, In.vTexCoord.y)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - blurSize, In.vTexCoord.y)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y)) * 0.16;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + blurSize, In.vTexCoord.y)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 2.0 * blurSize, In.vTexCoord.y)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 3.0 * blurSize, In.vTexCoord.y)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 4.0 * blurSize, In.vTexCoord.y)) * 0.05;
	
	// blur in y (vertical)
   // take nine samples, with the distance blurSize between them
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 4.0 * blurSize)) * 0.05;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 3.0 * blurSize)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 2.0 * blurSize)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - blurSize)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y)) * 0.16;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + blurSize)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 2.0 * blurSize)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 3.0 * blurSize)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 4.0 * blurSize)) * 0.05;

    sum + g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * intensity;
    
    Out.vColor = sum;
    
    return Out;
}

PS_OUT PS_BLOOM_CLONE(PS_IN In) // 29
{
    PS_OUT Out;
    
    float4 sum = float4(0.f, 0.f, 0.f, 0.f);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
    //sum += texture(iChannel0, float2(In.vTexCoord.x - 4.0 * blurSize, In.vTexCoord.y)) * 0.05;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - 3.0 * blurSize, In.vTexCoord.y)) * 0.09f;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - 2.0 * blurSize, In.vTexCoord.y)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - blurSize, In.vTexCoord.y)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y)) * 0.16;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + blurSize, In.vTexCoord.y)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 2.0 * blurSize, In.vTexCoord.y)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 3.0 * blurSize, In.vTexCoord.y)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 4.0 * blurSize, In.vTexCoord.y)) * 0.05;
	
	// blur in y (vertical)
   // take nine samples, with the distance blurSize between them
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 4.0 * blurSize)) * 0.05;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 3.0 * blurSize)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 2.0 * blurSize)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - blurSize)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y)) * 0.16;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + blurSize)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 2.0 * blurSize)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 3.0 * blurSize)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 4.0 * blurSize)) * 0.05;

    sum + g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * intensity;
    
    Out.vColor = sum;
    
    return Out;
}

PS_OUT PS_BLOOM_Stealth(PS_IN In) // 30
{
    PS_OUT Out;
    
    float4 sum = float4(0.f, 0.f, 0.f, 0.f);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
    //sum += texture(iChannel0, float2(In.vTexCoord.x - 4.0 * blurSize, In.vTexCoord.y)) * 0.05;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - 3.0 * blurSize, In.vTexCoord.y)) * 0.09f;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - 2.0 * blurSize, In.vTexCoord.y)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x - blurSize, In.vTexCoord.y)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y)) * 0.16;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + blurSize, In.vTexCoord.y)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 2.0 * blurSize, In.vTexCoord.y)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 3.0 * blurSize, In.vTexCoord.y)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x + 4.0 * blurSize, In.vTexCoord.y)) * 0.05;
	
	// blur in y (vertical)
   // take nine samples, with the distance blurSize between them
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 4.0 * blurSize)) * 0.05;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 3.0 * blurSize)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - 2.0 * blurSize)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y - blurSize)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y)) * 0.16;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + blurSize)) * 0.15;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 2.0 * blurSize)) * 0.12;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 3.0 * blurSize)) * 0.09;
    sum += g_texDiffuse[0].Sample(LinearSampler, float2(In.vTexCoord.x, In.vTexCoord.y + 4.0 * blurSize)) * 0.05;

    sum + g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * intensity;
    
    Out.vColor = sum;
    
    return Out;
}

PS_OUT PS_INDICATOR_NOTICE(PS_IN_ORTHOGRAPHIC In) // 31
{
    PS_OUT Out;
    
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
    
    Out.vColor.a *= g_fTime;
    
    return Out;
}

PS_OUT PS_INDICATOR_NOTICE_SIDE(PS_IN_ORTHOGRAPHIC In) // 32
{
    PS_OUT Out;
    
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgb = vector(223.f / 255.f, 96.f / 255.f, 106.f / 255.f, 1.f);

    Out.vColor.a *= g_fTime;
    
    return Out;
}

PS_OUT PS_NAME_TOWN_BACKGROUND_ONE(PS_IN_ORTHOGRAPHIC In) // 33
{
    PS_OUT Out;
    
    //vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    //Out.vColor.rgb = vector(239.f / 255.f, 231.f / 255.f, 204.f / 255.f, 1.f) * Diffuse.r * g_fTime;
    //Out.vColor *= g_vMtrlDiffuse * g_fTime;
    
    //Out.vColor.a = pow(Diffuse.r, 2.f);
    
    //float fDiscard = 1.f - g_fTime;
    
    //if (fDiscard >= Out.vColor.r)
    //    discard;
    
    ////if (0.3f >= Out.vColor.r)
    ////    discard;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgb = Diffuse;

    Out.vColor *= g_vMtrlDiffuse;
    
    Out.vColor.a = Diffuse.r;
    
    float fDiscard = 1.f - g_fTime;
    
    if (fDiscard >= Out.vColor.r)
        discard;
    
    return Out;
}

PS_OUT PS_NAME_TOWN_BACKGROUND_TWO(PS_IN_ORTHOGRAPHIC In) // 34
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);

    Out.vColor.rgb = float3(223.f / 255.f, 214.f / 255.f, 149.f / 255.f) * Diffuse.r * g_fTime /** g_fTime*/;
    Out.vColor.a = Diffuse.r * g_fTime;

    if (0.3f >= Out.vColor.r)
        discard;
    
    return Out;
}

PS_OUT PS_NAME_TOWN_LINE_M(PS_IN_ORTHOGRAPHIC In) // 35
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);  
    
    Out.vColor.rgba = (Diffuse.g) * vector(255.f / 255.f, 233.f / 255.f, 170.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    
    clip(Diffuse.g);
    
    
    return Out;
}

PS_OUT PS_NAME_TOWN_LINE_SIDE(PS_IN_ORTHOGRAPHIC In) // 36
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse.g) * vector(255.f / 255.f, 233.f / 255.f, 170.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    
    if (0.1f >= Out.vColor.g)
        discard;    
    
    return Out;
}

PS_OUT PS_INFO_BACKGROUND(PS_IN_ORTHOGRAPHIC In) // 37
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse.g) + vector(5.f / 255.f, 5.f / 255.f, 5.f / 255.f, g_fTime);
    Out.vColor *= g_vMtrlDiffuse;
    
    
    return Out;
}

PS_OUT PS_INFO_MOVIE(PS_IN_ORTHOGRAPHIC In) // 38
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.a = 0.f;
    Out.vColor.rgb = Diffuse * g_fTime;
    Out.vColor.a += g_fTime;
    
    
    return Out;
}

PS_OUT PS_INFO_SEPERATE(PS_IN_ORTHOGRAPHIC In) // 39
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.a = 0.f;
    Out.vColor.rgba = vector(120.f / 255.f, 120.f / 255.f, 120.f / 255.f, g_fTime);
    
    
    return Out;
}

PS_OUT PS_INDICATOR_MONSTER_ROCKON(PS_IN_ORTHOGRAPHIC In) // 40
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(PointSampler, In.vTexCoord);
    
    //Out.vColor.a = 0.f;
    Out.vColor.ra = Diffuse.ra * g_fTime;
    Out.vColor.gb = Diffuse.gb * g_fTime * 0.5f;
    
    //Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    //Out.vColor *= g_vMtrlDiffuse;
    
    return Out;
}

PS_OUT PS_INDICATOR_MONSTER_ROCKON_BACKGROUND(PS_IN_ORTHOGRAPHIC In) // 41
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    if (0.3f >= Diffuse.r)
        discard;
    
    Out.vColor.rgba = (Diffuse.r) * vector(0.5f, 0.f / 255.f , 0.f / 255.f, 0.25f * g_fTime);
    Out.vColor *= g_vMtrlDiffuse;
    
    return Out;
}

PS_OUT PS_INDICATOR_MONSTER_HP(PS_IN_ORTHOGRAPHIC In) // 42
{
    PS_OUT Out;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    
    Out.vColor.rgba = (Diffuse) * vector(250.f / 255.f, 172.f / 255.f, 168.f / 255.f, 1.f);

    if (0.1f >= Out.vColor.a)
        discard;
    
    if (In.vTexCoord.x > g_fRatio)
        discard;
    
    return Out;
}

PS_OUT PS_UI_INDICATOR_MONSTER_HPBLANK(PS_IN_ORTHOGRAPHIC In) // 43
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse) * vector(113.f / 255.f, 29.f / 255.f, 22.f / 255.f, 1.f);
   
    if (true == g_bOnOff)
        discard;
    
    if (0.3f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_UI_NAME_BOSS_BACKGROUND1(PS_IN_ORTHOGRAPHIC In) // 44
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
   
    Out.vColor.rgba = (Diffuse) * vector(g_fB1_Alpha, 0.f / 255.f, 0.f / 255.f, 0.3f * g_fB1_Alpha);

    if (g_fB1_Discard >= Out.vColor.r)
        discard;
    
    return Out;
}

PS_OUT PS_UI_NAME_BOSS_BACKGROUND2(PS_IN_ORTHOGRAPHIC In) // 45
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
   
    
    Out.vColor.rgba = (Diffuse) * vector(g_fB2_R, g_fB2_G, g_fB2_B, 1.f - g_fB2_Alpha);

    //if (0.3f >= Out.vColor.a)
    //    discard;
    
    return Out;
}

PS_OUT PS_UI_NAME_BOSS_BACKGROUND3(PS_IN_ORTHOGRAPHIC In) // 46
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
   
    
    Out.vColor.rgba = (Diffuse) * vector(1.f, 1.f, 1.f, g_fB3_Alpha);

    //if (0.3f >= Out.vColor.a)
    //    discard;
    
    return Out;
}

PS_OUT PS_UI_NAME_BOSS_NOISE(PS_IN_ORTHOGRAPHIC In) // 47
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
   
    
    Out.vColor.rgba = (Diffuse.g) * vector(g_fNoise_R / 255.f, g_fNoise_G / 255.f, g_fNoise_B / 255.f, 1.f - g_fNoise_A);

    //if (0.3f >= Out.vColor.a)
    //    discard;
    
    return Out;
}

PS_OUT PS_UI_INDICATOR_TUTORIAL_TIPS(PS_IN_ORTHOGRAPHIC In) // 48
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgb = float3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f);
    Out.vColor.a = Diffuse * g_fTime;

    if (0.3f >= Out.vColor.a)
        discard;

    
    return Out;
}

PS_OUT PS_UI_INDICATOR_BOSS_HP(PS_IN_ORTHOGRAPHIC In) // 49
{
    PS_OUT Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.a *= g_fTime;
    
    if (0.55f > Out.vColor.a)
        discard;

    return Out;
}

PS_OUT PS_UI_INDICATOR_BOSS_HP_BACK(PS_IN_ORTHOGRAPHIC In) // 50
{
    PS_OUT Out;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    
    Out.vColor.rgba = (Diffuse) * vector(113.f / 255.f, 29.f / 255.f, 22.f / 255.f, 1.f);

    if (0.1f >= Out.vColor.a)
        discard;
    
    if (In.vTexCoord.x > g_fRatio)
        discard;
    
    return Out;
}

PS_OUT PS_INFO_ICONS(PS_IN_ORTHOGRAPHIC In) // 51
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = Diffuse * g_fTime;
    //Out.vColor.a += g_fTime;
    
    
    return Out;
}

PS_OUT PS_INFO_ICONS_LR(PS_IN_ORTHOGRAPHIC In) // 52
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.a = Diffuse.a * g_fTime;
    Out.vColor.rgb = Diffuse.rgb * 0.5f + g_fTimeAlpha;
    //Out.vColor.a += g_fTime;
    
    
    return Out;
}

PS_OUT PS_INFO_ANSWER(PS_IN_ORTHOGRAPHIC In) // 53
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.a = Diffuse.a * g_fTime;
    Out.vColor.rgb = Diffuse.rgb * float3(255.f / 255.f, 60.f / 255.f, 60.f / 255.f);
    //Out.vColor.a += g_fTime;
    
    return Out;
}

PS_OUT PS_INFO_SELECTOR(PS_IN_ORTHOGRAPHIC In) // 54
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.a = Diffuse.a * g_fTime;
    Out.vColor.rgb = Diffuse.rgb * float3(255.f / 255.f, 70.f / 255.f, 70.f / 255.f);
    
    Out.vColor.r += g_fRatio;
    //Out.vColor.a += g_fTime;
    
    return Out;
}

PS_OUT PS_INFO_SUCCESS_BACKGROUND(PS_IN_ORTHOGRAPHIC In) // 55
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    if (Diffuse.g <= 0.32f)
        discard;
    //clip(Diffuse.g - 0.2f);
	
    Out.vColor.rgb = (Diffuse.g) * vector(g_fTime, g_fTime, g_fTime, 1.f);
    //Out.vColor.rgb += (Diffuse.r * 0.15f);
    //Out.vColor.a = 1 - Diffuse.r;
    Out.vColor.a = g_fRatio;
	
	
	
    return Out;
}

PS_OUT PS_INFO_SUCCESS(PS_IN_ORTHOGRAPHIC In) // 56
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);

    Out.vColor.r = g_fTime;
    Out.vColor.g = g_fTime;
    Out.vColor.b = g_fTime;
    Out.vColor.a = Diffuse.a;
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    return Out;
}


PS_OUT PS_INFO_BRAINTALK_FRAME(PS_IN_ORTHOGRAPHIC In) // 57
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);

    Out.vColor.rgba = Diffuse.rgba;
    
    if (0.2f <= Out.vColor.a && 0.6f > Out.vColor.a)
        Out.vColor.a = 0.8f;
    
    return Out;
}

PS_OUT PS_INFO_BRAINTALK_LIGHTFRAME(PS_IN_ORTHOGRAPHIC In) // 58
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);

    Out.vColor.rgba = Diffuse.rgba;
    Out.vColor.a *= g_fTime;
    
    return Out;
}

PS_OUT PS_INFO_BRAINTALK_LIGHTFRAME_NOISE(PS_IN_ORTHOGRAPHIC In) // 59
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgba = (Diffuse.g) * vector(1.f * g_fTime, 0.f, 0.f, 1.f * g_fTime);
    
    
    return Out;
}

PS_OUT PS_INFO_BRAINTALK_FACE_CONTEXT_NOISE(PS_IN_ORTHOGRAPHIC In) // 60
{
    PS_OUT Out;
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = Diffuse.rgba;
    
    Out.vColor.rgba *= g_fTime;
    
    return Out;
}

PS_OUT PS_INFO_BRAINTALK_FACE_FRAME(PS_IN_ORTHOGRAPHIC In) // 61
{
    PS_OUT Out;
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = Diffuse.rgba;
    
    Out.vColor.a *= g_fTime;
    
    return Out;
}

PS_OUT PS_ITEM_ARROW_B(PS_IN_ORTHOGRAPHIC In) // 62
{
    PS_OUT Out;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse.r) * vector(160.f / 255.f, 255.f / 255.f, 160.f / 255.f, 1.0f);
    Out.vColor *= g_vMtrlDiffuse;
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_DAMAGEFONT_NORMAL(PS_IN_ORTHOGRAPHIC In) // 63
{
    PS_OUT Out;
    
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba *= g_fTime;
    Out.vColor *= g_vMtrlDiffuse;
    
    return Out;
}

PS_OUT PS_UI_SHOP_MENU_LINE(PS_IN_ORTHOGRAPHIC In) // 64
{
    PS_OUT Out;
   
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    //Out.vColor *= g_vMtrlDiffuse;
    
    //Out.vColor.g = 0.5f;
    Out.vColor.b = 0.5f;
   
    Out.vColor.a = 0.3f;
   
    return Out;
}

PS_OUT PS_UI_ICONS(PS_IN_ORTHOGRAPHIC In) // 65
{
    PS_OUT Out;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = Diffuse * g_fTime;    
    
    if (0.1f >= Out.vColor.a)
        discard;
   
    return Out;
}

PS_OUT PS_UI_SHOP_SUB(PS_IN_ORTHOGRAPHIC In) // 66
{
    PS_OUT Out;
	
    //vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    //if (Diffuse.g <= 0.32f)
    //    discard;
	
    //Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    //Out.vColor.rgb += (Diffuse.r * 0.15f);
    //Out.vColor.a = Diffuse.g;
    Out.vColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return Out;
}

PS_OUT PS_INDICATOR_MISSPLAYER(PS_IN_ORTHOGRAPHIC In) // 67
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba *= g_fTime;

    return Out;
}

PS_OUT PS_SPCOBJECT_FILLTER(PS_IN_ORTHOGRAPHIC In) // 68
{
    PS_OUT Out;

    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgb = (Diffuse.r) * vector(2400.f / 255.f, 36.f / 255.f,  33.f / 255.f, 1.f);
    Out.vColor.a = pow(Diffuse.g + 0.5f, 5.f) ;
    
    if (1.f <= Out.vColor.a)
        Out.vColor.a = 1.f;
    
    if (0.1f >= Out.vColor.r)
        discard;

    return Out;
}

PS_OUT PS_SPCOBJECT_ROTATIONARROW(PS_IN_ORTHOGRAPHIC In) // 69
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    if (0.77f > Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_SPCOBJECT_BACKGROUND(PS_IN_ORTHOGRAPHIC In) // 70
{
    PS_OUT Out;

    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;

    if (0.3f > Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_SPCOBJECT_IMPULSE(PS_IN_ORTHOGRAPHIC In) // 71
{
    PS_OUT Out;

    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse.g) * vector(200.f * g_fTimeAlpha, 200.f * g_fTimeAlpha, 200.f * g_fTimeAlpha, 1.f);

    
    return Out;
}

PS_OUT PS_SPCOBJECT_HIGHLIGHTFILLTER(PS_IN_ORTHOGRAPHIC In) // 72
{
    PS_OUT Out;

    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgb = (Diffuse.r) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    Out.vColor.a = pow(Diffuse.g + 0.5f * g_fTimeAlpha, 5.f);
    
    if (1.f <= Out.vColor.a)
        Out.vColor.a = 1.f;
    
    if (0.1f >= Out.vColor.r)
        discard;

    return Out;
}

PS_OUT PS_SPCOBJECT_HIGHLIGHTIMPULSE(PS_IN_ORTHOGRAPHIC In) // 73
{
    PS_OUT Out;

    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse.r) * vector(255.f / 255.f, 255.f / 255.f , 255.f / 255.f, 1.f * g_fTimeAlpha);

    
    return Out;
}

PS_OUT PS_SPCOBJECT_FILLTER_ROCK(PS_IN_ORTHOGRAPHIC In) // 74
{
    PS_OUT Out;

    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgb = (Diffuse.r) * vector(480.f / 255.f, 480.f / 255.f, 480.f / 255.f, 1.f);
    Out.vColor.a = pow(Diffuse.g + 0.5f, 5.f);
    
    if (1.f <= Out.vColor.a)
        Out.vColor.a = 1.f;
    
    if (0.1f >= Out.vColor.r)
        discard;

    return Out;
}

PS_OUT PS_BRAIN(PS_IN_ORTHOGRAPHIC In) // 75
{
    PS_OUT Out;

    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse.g) * vector(126.f / 255.f, 126.f / 255.f, 114.f / 255.f, 1.f * g_fTimeAlpha);
    
    if (0.1f >= Out.vColor.r)
        discard;

    return Out;
}

PS_OUT PS_BRAIN_BACKGROUND(PS_IN_ORTHOGRAPHIC In) // 76
{
    PS_OUT Out;

    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse.r) * vector(32.f / 255.f, 32.f / 255.f, 21.f / 255.f, 0.3f);
    
    if (0.1f >= Out.vColor.r)
        discard;

    return Out;
}

PS_OUT PS_UI_SKILLOPEN_GAUGE(PS_IN_ORTHOGRAPHIC In) // 77
{
    PS_OUT Out;
    
    float2 normalizedCoords = In.vTexCoord - float2(0.5f, 0.5f);
    float angle = atan2(normalizedCoords.y, normalizedCoords.x) + PI; // 그림을 그려주기 시작하는 각을 설정하는 부분
    //angle %= PI;
    // Angle 범위 : -PI ~ PI
        
    //float startAngle = -1 / 3.f * PI - PI * 0.5;
    //float endAngle = 2 * PI * g_fRatio - 1 / 3.f * PI - PI * 0.5;
    float startAngle = 0;
    float endAngle = 2 * PI * g_fTime;
    
    
    if (angle < startAngle || angle > endAngle)
        discard;
    
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor.rgba = (Diffuse.r) * vector(255.f / 255.f, 66.f / 255.f, 0.f / 255.f, 1.f);
    Out.vColor *= g_vMtrlDiffuse;
    
    clip(Diffuse.r);
    
    if (0.1f >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_BM_BLACKFILLTER(PS_IN_ORTHOGRAPHIC In) // 78
{
    PS_OUT Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
	
    Out.vColor.a = 0.85f;
	
	
    return Out;
}

PS_OUT PS_INDICATOR_INSTKILL_LINE(PS_IN_ORTHOGRAPHIC In) // 79
{
    PS_OUT Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
	
    Out.vColor.a *= g_fTime;
	
    return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In) // 80
{
    PS_OUT_BLOOM Out;
	
    //Out.vTexture = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord).g;
    Out.vTexture = vector(97.f / 255.f, 63.f / 255.f, 204.f / 255.f, 155.f / 255.f) * g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord).g;
//	Out.vData       = float4(0.f, 0.f, 0.f, Out.vTexture.a);
//	Out.vTexture.a	= g_fBloomStrength;
    Out.vData = float4(g_fBloomStrength, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_SQUARE(PS_IN In) // 81
{
    PS_OUT_BLOOM Out;
	
    //Out.vTexture = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord).g;
    Out.vTexture = vector(97.f / 255.f, 63.f / 255.f, 204.f / 255.f, 255.f * g_fTime / 255.f) * g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
//	Out.vData       = float4(0.f, 0.f, 0.f, Out.vTexture.a);
//	Out.vTexture.a	= g_fBloomStrength;
    Out.vData = float4(g_fBloomStrength, 0.f, 0.f, 0.f);

    return Out;
}

PS_OUT PS_INDICATOR_INSTKILL_INDICATOR(PS_IN_ORTHOGRAPHIC In) // 82
{
    PS_OUT Out;
	
    Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vColor *= g_vMtrlDiffuse;
    Out.vColor.a *= g_fTime;
	
    return Out;
}

PS_OUT PS_INDICATOR_COMMUNICATION_FRAME(PS_IN_ORTHOGRAPHIC In) // 83
{
    PS_OUT Out;

    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor.rgba = (Diffuse.g) * vector(255.f * g_fTimeAlpha / 255.f, 255.f * g_fTimeAlpha / 255.f, 255.f * g_fTimeAlpha / 255.f, 1.f * g_fTimeAlpha);
    
    
    return Out;
}

PS_OUT PS_UI_INDICATOR_GIFTLISTFRAME(PS_IN_ORTHOGRAPHIC In) // 84
{
    PS_OUT Out;
	
    vector Diffuse = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    //if (Diffuse.g <= 0.32f)
    //    discard;
    //clip(Diffuse.g - 0.2f);
	
    Out.vColor.rgb = (Diffuse.g) * vector(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f * g_fTime);
    Out.vColor.rgb += (Diffuse.r * 0.15f);
    //Out.vColor.a = 1 - Diffuse.r;
    Out.vColor.a = Diffuse.g;
	
	
	
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

    pass Orthographic   // 1
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

    pass Billboard  // 2
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

    pass Billboard_SpriteSheet  // 3
    {
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITESHEET();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_HP  // 4
    {
        VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader	= NULL;
        HullShader		= NULL;
        DomainShader	= NULL;
        PixelShader		= compile ps_5_0 PS_UI_HP();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_ItemBox // 5
    {
        VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader	= NULL;
        HullShader		= NULL;
        DomainShader	= NULL;
        PixelShader		= compile ps_5_0 PS_UI_ITEMBOX();
    }
    
    pass UI_PsycoWave   // 6
    {
        VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader	= NULL;
        HullShader		= NULL;
        DomainShader	= NULL;
        PixelShader		= compile ps_5_0 PS_UI_PSYCOWAVE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_Menu_Background // 7
    {
        VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader	= NULL;
        HullShader		= NULL;
        DomainShader	= NULL;
        PixelShader		= compile ps_5_0 PS_UI_MENU_BACKGROUND();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_Menu_Category   // 8
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_MENU_CATEGORY();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_Menu_Line   // 9
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_MENU_LINE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_Menu_Seperate_Line  // 10
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_MENU_SEPERATE_LINE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_Menu_ProfileOff // 11
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_MENU_PROFILEOFF();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_Menu_Indicator // 12
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_INDICATOR_GAUGE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_ItemShader // 13
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_ITEMSHADER();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_ItemCoolTime // 14
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_ITEMCOOLTIME();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SkillLight_Electric // 15
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLLIGHT_Electric();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SkillCool_Electric // 16
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLCOOL_Electric();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SkillCool_Acceleration // 17
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLCOOL_Acceleration();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SkillCool_Clone // 18
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLCOOL_CLONE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SkillCool_Stealth // 19
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLCOOL_Stealth();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_HPBlank // 20
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_HPBLANK();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SkillLight_Acceleration // 21
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLLIGHT_Acceleration();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SkillLight_Clone // 22
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLLIGHT_CLONE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SkillLight_Stealth // 23
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLLIGHT_Stealth();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_HP_40 // 24
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_HP_40();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_FADE // 25
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_FADE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_3D // 26
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_3D();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Bloom_Electric // 27
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BLOOM_Electric();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Bloom_Acceleration // 28
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BLOOM_Acceleration();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Bloom_Clone // 29
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BLOOM_CLONE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Bloom_Stealth // 30
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BLOOM_Stealth();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Notice // 31
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_NOTICE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Notice_Side // 32
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_NOTICE_SIDE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Name_Town_Background_One // 33
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NAME_TOWN_BACKGROUND_ONE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Name_Town_Background_Two // 34
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NAME_TOWN_BACKGROUND_TWO();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Name_Town_Line_M // 35
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NAME_TOWN_LINE_M();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Name_Town_Line_Side // 36
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NAME_TOWN_LINE_SIDE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_ToolTip_PMCC // 37
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_BACKGROUND();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_ToolTip_PMCC_Movie // 38
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_MOVIE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_ToolTip_PMCC_SeperateLine // 39
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_SEPERATE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Monster_LockOn // 40
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_MONSTER_ROCKON();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Monster_LockOn_Background // 41
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_MONSTER_ROCKON_BACKGROUND();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Monster_Hp // 42
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_MONSTER_HP();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Monster_Hp_Blank // 43
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_INDICATOR_MONSTER_HPBLANK();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Name_Boss_Background1 // 44
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_NAME_BOSS_BACKGROUND1();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Name_Boss_Background2 // 45
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_NAME_BOSS_BACKGROUND2();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Name_Boss_Background3 // 46
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_NAME_BOSS_BACKGROUND3();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Name_Boss_Noise // 47
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_NAME_BOSS_NOISE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Tutorial_Tips // 48
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_INDICATOR_TUTORIAL_TIPS();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Boss_Hp // 49
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_INDICATOR_BOSS_HP();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Indicator_Boss_Hp_Back // 50
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_INDICATOR_BOSS_HP_BACK();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_Icons // 51
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_ICONS();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_Icons_LR // 52
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_ICONS_LR();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_Answer // 53
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_ANSWER();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_Selector // 54
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_SELECTOR();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_Success_Background // 55
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_SUCCESS_BACKGROUND();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_Success // 56
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_SUCCESS();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_BrainTalk_Frame  // 57
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_BRAINTALK_FRAME();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_BrainTalk_Light // 58
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_BRAINTALK_LIGHTFRAME();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_BrainTalk_Light_Noise // 59
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_BRAINTALK_LIGHTFRAME_NOISE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_BrainTalk_Face_Context_Noise // 60
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_BRAINTALK_FACE_CONTEXT_NOISE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass Info_BrainTalk_Face_Frame // 61
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INFO_BRAINTALK_FACE_FRAME();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_ItemArrow_B // 62
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_ITEM_ARROW_B();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_DamageFont_Normal // 63
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DAMAGEFONT_NORMAL();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SHOP_MENU_Line // 64
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SHOP_MENU_LINE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_Icons // 65
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_ICONS();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass SHOP_SUB // 66
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

    pass UI_MISSPLAYER // 67
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_MISSPLAYER();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SPCOBJECT_FILLTER // 68
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPCOBJECT_FILLTER();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass PS_SPCOBJECT_ROTATIONARROW // 69
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPCOBJECT_ROTATIONARROW();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass PS_SPCOBJECT_BACKGROUND // 70
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPCOBJECT_BACKGROUND();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass PS_SPCOBJECT_IMPULSE // 71
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPCOBJECT_IMPULSE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SPCOBJECT_HIGHLIGHTFILLTER // 72
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPCOBJECT_HIGHLIGHTFILLTER();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SPCOBJECT_HIGHLIGHTIMPULSE // 73
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPCOBJECT_HIGHLIGHTIMPULSE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SPCOBJECT_FILLTER_ROCK // 74
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SPCOBJECT_FILLTER_ROCK();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_BRAIN // 75
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BRAIN();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_BRAIN_BACKGROUND // 76
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BRAIN_BACKGROUND();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_SKILLOPEN_GAUGE // 77
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_SKILLOPEN_GAUGE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BM_BLACKFILLTER // 78
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BM_BLACKFILLTER();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_INDICATOR_INSTKILL_LINE // 79
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_INSTKILL_LINE();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass BLOOM //80
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass BLOOM_SQUARE //81
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_SQUARE();

        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

    pass UI_INDICATOR_INSTKILL_INDICATOR // 82
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_INSTKILL_INDICATOR();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_INDICATOR_COMMUNICATION_FRAME // 83
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INDICATOR_COMMUNICATION_FRAME();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }

    pass UI_INDICATOR_GIFTLIST_FRAME // 84
    {
        VertexShader = compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UI_INDICATOR_GIFTLISTFRAME();
        SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
}