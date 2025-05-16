#include "ShaderCommon.hlsli"

bool	g_bDeclareColorAsArguement	= false;
float	g_fDistortionPower			= 0.1f;
float	g_fLineThickness			= 1.f;
float3	g_vLineNormal				= float3(0.f, -1.f, 0.f);

struct VS_IN
{
	float3 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vRight		: TEXCOORD1;
	float4 vUp			: TEXCOORD2;
	float4 vLook		: TEXCOORD3;
	float4 vTranslation	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
	float4 vArgument	: TEXCOORD6;
};

struct VS_OUT
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
	float4 vArgument	: TEXCOORD2;
};

struct VS_OUT_BILLBOARD
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
	float4 vArguement	: TEXCOORD2;
	float2 vStartSection : TEXCOORD3;
};

struct VS_OUT_WORLD
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vRight		: TEXCOORD1;
	float4 vUp			: TEXCOORD2;
	float4 vLook		: TEXCOORD3;
	float4 vTranslation	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
	float4 vArguement	: TEXCOORD6;
	float2 vStartSection : TEXCOORD7;
};

struct VS_OUT_QUAD
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vWorldPos0	: TEXCOORD1;
	float4 vWorldPos1	: TEXCOORD2;
	float4 vWorldPos2	: TEXCOORD3;
	float4 vWorldPos3	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
	float4 vArgument	: TEXCOORD6;
};

struct VS_OUT_LINE
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vPreviousPos	: TEXCOORD1;
	float4 vCurrentPos	: TEXCOORD2;
	float4 vColor		: TEXCOORD3;
	float4 vArgument	: TEXCOORD4;
	float2 vStartSection : TEXCOORD5;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out;
	
	float4x4			mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	float4				vPosition		= mul(float4(In.vPosition, 1.f), mTransformation);
	
	float4x4			mWV, mWVP;

	mWV					= mul(g_mWorld, g_mView);
	mWVP				= mul(mWV, g_mProj);

	Out.vPosition		= mul(vPosition, mWVP);
	Out.vSize			= In.vSize;
	Out.vColor			= In.vColor;
	Out.vArgument		= In.vArgument;

	return Out;
}

VS_OUT_BILLBOARD VS_MAIN_BILLBOARD(VS_IN In)
{
	VS_OUT_BILLBOARD Out;
	
	float4x4			mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	float4				vPosition		= mul(float4(In.vPosition, 1.f), mTransformation);
	
//	Out.vPosition		= mul(vPosition, g_mWorld);
	Out.vPosition		= In.vTranslation;
	Out.vSize			= In.vSize;
	Out.vColor			= In.vColor;
	Out.vArguement		= In.vArgument;
	
    uint iColIndex, iRowIndex;
    iColIndex = Out.vArguement.z % g_vSpriteSheetSize.x;
    iRowIndex = Out.vArguement.z / g_vSpriteSheetSize.x;
    Out.vStartSection = float2((float) iColIndex / g_vSpriteSheetSize.x, (float) iRowIndex / g_vSpriteSheetSize.y);
	
	return Out;
}

VS_OUT_QUAD VS_MAIN_QUAD(VS_IN In)
{
	VS_OUT_QUAD Out;
	
	float4x4			mWV, mWVP;

	mWV					= mul(g_mWorld, g_mView);
	mWVP				= mul(mWV, g_mProj);

	Out.vPosition		= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vSize			= In.vSize;
	
	Out.vWorldPos0		= In.vRight;
	Out.vWorldPos1		= In.vUp;
	Out.vWorldPos2		= In.vLook;
	Out.vWorldPos3		= In.vTranslation;
	Out.vColor			= In.vColor;
	Out.vArgument		= In.vArgument;

	return Out;
}

VS_OUT_LINE VS_MAIN_LINE(VS_IN In)
{
	VS_OUT_LINE Out;
	
	float4x4			mWV, mWVP;

	mWV					= mul(g_mWorld, g_mView);
	mWVP				= mul(mWV, g_mProj);

	Out.vPosition		= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vSize			= In.vSize;
	
	Out.vPreviousPos	= In.vLook;
	Out.vCurrentPos		= In.vTranslation;
	Out.vColor			= In.vColor;
	Out.vArgument		= In.vArgument;
	
//	Out.vPreviousPos	= float4(0,0,0,1);
//	Out.vCurrentPos		= float4(1,1,1,1);
//	Out.vColor			= float4(1,1,1,1);
//	Out.vArgument		= float4(1,1,0,1);
	
    uint iColIndex, iRowIndex;
    iColIndex = Out.vArgument.z % g_vSpriteSheetSize.x;
    iRowIndex = Out.vArgument.z / g_vSpriteSheetSize.x;
    Out.vStartSection = float2((float) iColIndex / g_vSpriteSheetSize.x, (float) iRowIndex / g_vSpriteSheetSize.y);
	
//	uint iColIndex, iRowIndex;
//	iColIndex = 0;
//	iRowIndex = 0;
//	Out.vStartSection = float2((float) iRowIndex / 1, (float) iColIndex / 1);

	return Out;
}

VS_OUT_WORLD VS_MAIN_WORLD(VS_IN In)
{
	VS_OUT_WORLD Out;
	
	Out.vPosition        = float4(In.vPosition, 1.f);
	Out.vSize            = In.vSize;
	
	Out.vRight = In.vRight;
	Out.vUp = In.vUp;
	Out.vLook = In.vLook;
	Out.vTranslation = In.vTranslation;
	
	Out.vColor = In.vColor;
	Out.vArguement = In.vArgument;
	
    uint iColIndex, iRowIndex;
    iColIndex = Out.vArguement.z % g_vSpriteSheetSize.x;
    iRowIndex = Out.vArguement.z / g_vSpriteSheetSize.x;
    Out.vStartSection = float2((float) iColIndex / g_vSpriteSheetSize.x, (float) iRowIndex / g_vSpriteSheetSize.y);
	
	return Out;
}

struct GS_IN
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
	float4 vArguement	: TEXCOORD2;
};

struct GS_IN_BILLBOARD
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
	float4 vArguement	: TEXCOORD2;
	float2 vStartSection : TEXCOORD3;
};

struct GS_IN_WORLD
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vRight		: TEXCOORD1;
	float4 vUp			: TEXCOORD2;
	float4 vLook		: TEXCOORD3;
	float4 vTranslation	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
	float4 vArguement	: TEXCOORD6;
	float2 vStartSection : TEXCOORD7;
};

struct GS_IN_QUAD
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vWorldPos0	: TEXCOORD1;
	float4 vWorldPos1	: TEXCOORD2;
	float4 vWorldPos2	: TEXCOORD3;
	float4 vWorldPos3	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
	float4 vArguement	: TEXCOORD6;
};

struct GS_IN_LINE
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vPreviousPos	: TEXCOORD1;
	float4 vCurrentPos	: TEXCOORD2;
	float4 vColor		: TEXCOORD3;
	float4 vArguement	: TEXCOORD4;
	float2 vStartSection : TEXCOORD5;
};

struct GS_OUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
	float4 vArguement : TEXCOORD2;
	float2 vStartSection : TEXCOORD3;
};

[maxvertexcount(6)]
void GS_MAIN_RECT(point GS_IN In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];

	Out[0].vPosition	= float4(In[0].vPosition.xyz + In[0].vSize.x + In[0].vSize.y, 1.f);
	Out[0].vTexCoord	= float2(0.f, 0.f);
	Out[0].vColor		= In[0].vColor;
	Out[0].vArguement	= In[0].vArguement;
	Out[0].vStartSection = float2(0.f, 0.f);

	Out[1].vPosition	= float4(In[0].vPosition.xyz - In[0].vSize.x + In[0].vSize.y, 1.f);
	Out[1].vTexCoord	= float2(1.f, 0.f);
	Out[1].vColor		= In[0].vColor;
	Out[1].vArguement	= In[0].vArguement;
	Out[1].vStartSection = float2(0.f, 0.f);

	Out[2].vPosition	= float4(In[0].vPosition.xyz - In[0].vSize.x - In[0].vSize.y, 1.f);
	Out[2].vTexCoord	= float2(1.f, 1.f);
	Out[2].vColor		= In[0].vColor;
	Out[2].vArguement	= In[0].vArguement;
	Out[2].vStartSection = float2(0.f, 0.f);

	Out[3].vPosition	= float4(In[0].vPosition.xyz + In[0].vSize.x - In[0].vSize.y, 1.f);
	Out[3].vTexCoord	= float2(0.f, 1.f);
	Out[3].vColor		= In[0].vColor;
	Out[3].vArguement	= In[0].vArguement;
	Out[3].vStartSection = float2(0.f, 0.f);

	Triangles.Append(Out[0]);
	Triangles.Append(Out[1]);
	Triangles.Append(Out[2]);
	Triangles.RestartStrip();

	Triangles.Append(Out[0]);
	Triangles.Append(Out[2]);
	Triangles.Append(Out[3]);
	Triangles.RestartStrip();
}

[maxvertexcount(6)]
void GS_MAIN_RECT_BILLBOARD(point GS_IN_BILLBOARD In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];
	
	float4 vLook		= g_vCamPosition - In[0].vPosition;
//	float3 vRight		= normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vSize.x * 0.5f;
//	float3 vRight		= normalize(cross(float3(sin(In[0].vArguement.w), cos(In[0].vArguement.w), 0.f), vLook.xyz)) * In[0].vSize.x * 0.5f * In[0].vArguement.x;
	float3 vRight		= normalize(cross(float3(sin(In[0].vArguement.w), cos(In[0].vArguement.w), 0.f), vLook.xyz)) * In[0].vSize.x * 0.5f * In[0].vArguement.x;
	float3 vUp			= normalize(cross(vLook.xyz, vRight)) * In[0].vSize.y * 0.5f * In[0].vArguement.y;

	matrix matVP		= mul(g_mView, g_mProj);

	Out[0].vPosition	= float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vPosition	= mul(Out[0].vPosition, matVP);
	Out[0].vTexCoord	= float2(0.f, 0.f);
	Out[0].vColor		= In[0].vColor;
	Out[0].vArguement	= In[0].vArguement;
	Out[0].vStartSection = In[0].vStartSection;

	Out[1].vPosition	= float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition	= mul(Out[1].vPosition, matVP);
	Out[1].vTexCoord	= float2(1.f, 0.f);
	Out[1].vColor		= In[0].vColor;
	Out[1].vArguement	= In[0].vArguement;
	Out[1].vStartSection = In[0].vStartSection;

	Out[2].vPosition	= float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition	= mul(Out[2].vPosition, matVP);
	Out[2].vTexCoord	= float2(1.f, 1.f);
	Out[2].vColor		= In[0].vColor;
	Out[2].vArguement	= In[0].vArguement;
	Out[2].vStartSection = In[0].vStartSection;

	Out[3].vPosition	= float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition	= mul(Out[3].vPosition, matVP);
	Out[3].vTexCoord	= float2(0.f, 1.f);
	Out[3].vColor		= In[0].vColor;
	Out[3].vArguement	= In[0].vArguement;
	Out[3].vStartSection = In[0].vStartSection;

	Triangles.Append(Out[0]);
	Triangles.Append(Out[1]);
	Triangles.Append(Out[2]);
	Triangles.RestartStrip();

	Triangles.Append(Out[0]);
	Triangles.Append(Out[2]);
	Triangles.Append(Out[3]);
	Triangles.RestartStrip();
}

[maxvertexcount(6)]
void GS_MAIN_QUAD(point GS_IN_QUAD In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];

	matrix matVP		= mul(g_mView, g_mProj);
	//LD
	Out[0].vPosition	= float4(In[0].vWorldPos0);
	Out[0].vPosition	= mul(Out[0].vPosition, matVP);
//	Out[0].vTexCoord	= float2(0.f, 1.f);
	Out[0].vTexCoord	= float2(In[0].vArguement.x, 1.f);
	Out[0].vColor		= In[0].vColor;
	Out[0].vArguement	= In[0].vArguement;
	Out[0].vStartSection = float2(0.f, 0.f);
	//LU
	Out[1].vPosition	= float4(In[0].vWorldPos1);
	Out[1].vPosition	= mul(Out[1].vPosition, matVP);
//	Out[1].vTexCoord	= float2(0.f, 0.f);
	Out[1].vTexCoord	= float2(In[0].vArguement.y, 0.f);
	Out[1].vColor		= In[0].vColor;
	Out[1].vArguement	= In[0].vArguement;
	Out[1].vStartSection = float2(0.f, 0.f);
	//RD
	Out[2].vPosition	= float4(In[0].vWorldPos2);
	Out[2].vPosition	= mul(Out[2].vPosition, matVP);
//	Out[2].vTexCoord	= float2(1.f, 1.f);
	Out[2].vTexCoord	= float2(In[0].vArguement.z, 1.f);
	Out[2].vColor		= In[0].vColor;
	Out[2].vArguement	= In[0].vArguement;
	Out[2].vStartSection = float2(0.f, 0.f);
	//RU
	Out[3].vPosition	= float4(In[0].vWorldPos3);
	Out[3].vPosition	= mul(Out[3].vPosition, matVP);
//	Out[3].vTexCoord	= float2(1.f, 0.f);
	Out[3].vTexCoord	= float2(In[0].vArguement.w, 0.f);
	Out[3].vColor		= In[0].vColor;
	Out[3].vArguement	= In[0].vArguement;
	Out[3].vStartSection = float2(0.f, 0.f);

	if (In[0].vWorldPos0.w && In[0].vWorldPos1.w && In[0].vWorldPos2.w && In[0].vWorldPos3.w)
	{
		Triangles.Append(Out[1]);
		Triangles.Append(Out[3]);
		Triangles.Append(Out[2]);
		Triangles.RestartStrip();
	
		Triangles.Append(Out[1]);
		Triangles.Append(Out[2]);
		Triangles.Append(Out[0]);
		Triangles.RestartStrip();
	}
}

[maxvertexcount(6)]
void GS_MAIN_LINE(point GS_IN_LINE In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];

	matrix	matVP			= mul(g_mView, g_mProj);
	
	float3	vDir			= In[0].vCurrentPos.xyz - In[0].vPreviousPos.xyz;
	float3	vLook			= In[0].vCurrentPos.xyz - g_vCamPosition.xyz;
	float3	vRight			= normalize(cross(vDir.xyz, vLook.xyz));
	
//	float	fLength			= length(vDir);
	vDir					= normalize(vDir);
	float	fTail			= In[0].vArguement.w ? In[0].vArguement.w * 2.f : 2.f;
	
	
	Out[0].vPosition		= float4(In[0].vCurrentPos.xyz + vRight.xyz * g_vSize.x, 1.f);
//	Out[0].vPosition		= float4(In[0].vCurrentPos.xyz + vRight.xyz * g_vSize.x, 1.f);
	Out[0].vTexCoord		= float2(1.f, 0.f);
	Out[0].vColor			= In[0].vColor;
	Out[0].vArguement		= In[0].vArguement;
	Out[0].vStartSection	= In[0].vStartSection;
	
	Out[1].vPosition		= float4(In[0].vCurrentPos.xyz - vRight.xyz * g_vSize.x, 1.f);
//	Out[1].vPosition		= float4(In[0].vCurrentPos.xyz - vRight.xyz * g_vSize.x, 1.f);
	Out[1].vTexCoord		= float2(0.f, 0.f);
	Out[1].vColor			= In[0].vColor;
	Out[1].vArguement		= In[0].vArguement;
	Out[1].vStartSection	= In[0].vStartSection;
	
	Out[2].vPosition		= float4(In[0].vCurrentPos.xyz - (vDir * g_vSize.y * fTail) - vRight.xyz * g_vSize.x, 1.f);
//	Out[2].vPosition		= float4(In[0].vPreviousPos.xyz - vRight.xyz * g_vSize.x, 1.f);
	Out[2].vTexCoord		= float2(0.f, 1.f);
	Out[2].vColor			= In[0].vColor;
	Out[2].vArguement		= In[0].vArguement;
	Out[2].vStartSection	= In[0].vStartSection;
	
	Out[3].vPosition		= float4(In[0].vCurrentPos.xyz - (vDir * g_vSize.y * fTail) + vRight.xyz * g_vSize.x, 1.f);
//	Out[3].vPosition		= float4(In[0].vPreviousPos.xyz + vRight.xyz * g_vSize.x, 1.f);
	Out[3].vTexCoord		= float2(1.f, 1.f);
	Out[3].vColor			= In[0].vColor;
	Out[3].vArguement		= In[0].vArguement;
	Out[3].vStartSection	= In[0].vStartSection;
	
	Out[0].vPosition		= mul(Out[0].vPosition, matVP);
	Out[1].vPosition		= mul(Out[1].vPosition, matVP);
	Out[2].vPosition		= mul(Out[2].vPosition, matVP);
	Out[3].vPosition		= mul(Out[3].vPosition, matVP);
	
	Triangles.Append(Out[0]);
	Triangles.Append(Out[1]);
	Triangles.Append(Out[2]);
	Triangles.RestartStrip();
	
	Triangles.Append(Out[0]);
	Triangles.Append(Out[2]);
	Triangles.Append(Out[3]);
	Triangles.RestartStrip();
}

[maxvertexcount(6)]
void GS_MAIN_WORLD(point GS_IN_WORLD In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];
	
	float4x4	mTransformation;
	float4		vRight			= normalize(In[0].vRight) * In[0].vColor.x;
	float4		vUp				= normalize(In[0].vUp) * In[0].vColor.y;
	float4		vLook			= float4(normalize(cross(vRight.xyz, vUp.xyz)) * In[0].vColor.z, 0.f);
	float4		vTranslation	= In[0].vTranslation;
	
	mTransformation				= float4x4(vRight, vUp, vLook, vTranslation - vRight * g_vSize.x * In[0].vColor.x + vUp * g_vSize.y * In[0].vColor.y);
	Out[0].vPosition			= mul(mul(mul(In[0].vPosition, mTransformation), g_mView), g_mProj);
	Out[0].vColor				= In[0].vColor;
	Out[0].vArguement			= In[0].vArguement;
	Out[0].vStartSection		= In[0].vStartSection;
	Out[0].vTexCoord			= float2(0.f, 0.f);
	
	mTransformation				= float4x4(vRight, vUp, vLook, vTranslation + vRight * g_vSize.x * In[0].vColor.x + vUp * g_vSize.y * In[0].vColor.y);
	Out[1].vPosition			= mul(mul(mul(In[0].vPosition, mTransformation), g_mView), g_mProj);
	Out[1].vColor				= In[0].vColor;
	Out[1].vArguement			= In[0].vArguement;
	Out[1].vStartSection		= In[0].vStartSection;
	Out[1].vTexCoord			= float2(1.f, 0.f);
	
	mTransformation				= float4x4(vRight, vUp, vLook, vTranslation + vRight * g_vSize.x * In[0].vColor.x - vUp * g_vSize.y * In[0].vColor.y);
	Out[2].vPosition			= mul(mul(mul(In[0].vPosition, mTransformation), g_mView), g_mProj);
	Out[2].vColor				= In[0].vColor;
	Out[2].vArguement			= In[0].vArguement;
	Out[2].vStartSection		= In[0].vStartSection;
	Out[2].vTexCoord			= float2(1.f, 1.f);
	
	mTransformation				= float4x4(vRight, vUp, vLook, vTranslation - vRight * g_vSize.x * In[0].vColor.x - vUp * g_vSize.y * In[0].vColor.y);
	Out[3].vPosition			= mul(mul(mul(In[0].vPosition, mTransformation), g_mView), g_mProj);
	Out[3].vColor				= In[0].vColor;
	Out[3].vArguement			= In[0].vArguement;
	Out[3].vStartSection		= In[0].vStartSection;
	Out[3].vTexCoord			= float2(0.f, 1.f);
	
	if (In[0].vColor.a)
	{
		Triangles.Append(Out[0]);
		Triangles.Append(Out[1]);
		Triangles.Append(Out[2]);
		Triangles.RestartStrip();
		
		Triangles.Append(Out[0]);
		Triangles.Append(Out[2]);
		Triangles.Append(Out[3]);
		Triangles.RestartStrip();
	}
}

[maxvertexcount(6)]
void GS_MAIN_LOCAL(point GS_IN_WORLD In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];
	
    float4x4	mWVP			= mul(mul(g_mWorld, g_mView), g_mProj);
	
	float4x4	mTransformation;
	float4		vRight			= normalize(In[0].vRight) * g_vSize.x * In[0].vColor.x;
	float4		vUp				= normalize(In[0].vUp);
	float4		vLook			= normalize(In[0].vLook) * g_vSize.y * In[0].vColor.y;
	float4		vTranslation	= In[0].vTranslation;
	
	mTransformation				= float4x4(vRight, vUp, vLook, vTranslation - vRight + vLook);
	Out[0].vPosition			= mul(mul(In[0].vPosition, mTransformation), mWVP);
	Out[0].vColor				= In[0].vColor;
	Out[0].vArguement			= In[0].vArguement;
	Out[0].vStartSection		= In[0].vStartSection;
	Out[0].vTexCoord			= float2(0.f, 0.f);
	
	mTransformation				= float4x4(vRight, vUp, vLook, vTranslation + vRight + vLook);
	Out[1].vPosition			= mul(mul(In[0].vPosition, mTransformation), mWVP);
	Out[1].vColor				= In[0].vColor;
	Out[1].vArguement			= In[0].vArguement;
	Out[1].vStartSection		= In[0].vStartSection;
	Out[1].vTexCoord			= float2(1.f, 0.f);
	
	mTransformation				= float4x4(vRight, vUp, vLook, vTranslation + vRight - vLook);
	Out[2].vPosition			= mul(mul(In[0].vPosition, mTransformation), mWVP);
	Out[2].vColor				= In[0].vColor;
	Out[2].vArguement			= In[0].vArguement;
	Out[2].vStartSection		= In[0].vStartSection;
	Out[2].vTexCoord			= float2(1.f, 1.f);
	
	mTransformation				= float4x4(vRight, vUp, vLook, vTranslation - vRight - vLook);
	Out[3].vPosition			= mul(mul(In[0].vPosition, mTransformation), mWVP);
	Out[3].vColor				= In[0].vColor;
	Out[3].vArguement			= In[0].vArguement;
	Out[3].vStartSection		= In[0].vStartSection;
	Out[3].vTexCoord			= float2(0.f, 1.f);
	
	if (In[0].vColor.a)
	{
		Triangles.Append(Out[0]);
		Triangles.Append(Out[1]);
		Triangles.Append(Out[2]);
		Triangles.RestartStrip();
		
		Triangles.Append(Out[0]);
		Triangles.Append(Out[2]);
		Triangles.Append(Out[3]);
		Triangles.RestartStrip();
	}
}

[maxvertexcount(6)]
void GS_MAIN_WORLD_BILLBOARD(point GS_IN_WORLD In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];
	
	float4		vPosition		= In[0].vTranslation;
	float4		vView			= normalize(g_vCamPosition - vPosition);
	
	float4		vRight			= float4(normalize(cross(float3(0.f, 1.f, 0.f), vView.xyz)), 0.f) * In[0].vColor.x * g_vSize.x;
	float4		vUp				= float4(normalize(cross(vView.xyz, vRight.xyz)), 0.f) * In[0].vColor.y * g_vSize.y;
	
	float4x4	mWorld			= mul(float4x4(vRight, vUp, vView, vPosition), AxisRotation(vView.xyz, In[0].vColor.z));
	
	Out[0].vPosition			= mul(mul(vPosition + mWorld[0] + mWorld[1], g_mView), g_mProj);
	Out[0].vColor				= In[0].vColor;
	Out[0].vArguement			= In[0].vArguement;
	Out[0].vStartSection		= In[0].vStartSection;
	Out[0].vTexCoord			= float2(0.f, 0.f);
	
	Out[1].vPosition			= mul(mul(vPosition - mWorld[0] + mWorld[1], g_mView), g_mProj);
	Out[1].vColor				= In[0].vColor;
	Out[1].vArguement			= In[0].vArguement;
	Out[1].vStartSection		= In[0].vStartSection;
	Out[1].vTexCoord			= float2(1.f, 0.f);
	
	Out[2].vPosition			= mul(mul(vPosition - mWorld[0] - mWorld[1], g_mView), g_mProj);
	Out[2].vColor				= In[0].vColor;
	Out[2].vArguement			= In[0].vArguement;
	Out[2].vStartSection		= In[0].vStartSection;
	Out[2].vTexCoord			= float2(1.f, 1.f);
	
	Out[3].vPosition			= mul(mul(vPosition + mWorld[0] - mWorld[1], g_mView), g_mProj);
	Out[3].vColor				= In[0].vColor;
	Out[3].vArguement			= In[0].vArguement;
	Out[3].vStartSection		= In[0].vStartSection;
	Out[3].vTexCoord			= float2(0.f, 1.f);
	
	if (In[0].vColor.a)
	{
		Triangles.Append(Out[0]);
		Triangles.Append(Out[1]);
		Triangles.Append(Out[2]);
		Triangles.RestartStrip();
		
		Triangles.Append(Out[0]);
		Triangles.Append(Out[2]);
		Triangles.Append(Out[3]);
		Triangles.RestartStrip();
	}
}

[maxvertexcount(6)]
void GS_MAIN_LOCAL_BILLBOARD(point GS_IN_WORLD In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];
	
//	float4x4	mWVP			= mul(mul(g_mWorld, g_mView), g_mProj);
	
	float4		vPosition		= mul(In[0].vTranslation, g_mWorld);
//	float4		vVelocity		= mul(In[0].vLook, g_mWorld);
	float4		vView			= normalize(g_vCamPosition - vPosition);
	
	float4		vRight			= float4(normalize(cross(float3(0.f, 1.f, 0.f), vView.xyz)), 0.f) * In[0].vColor.x * g_vSize.x;
	float4		vUp				= float4(normalize(cross(vView.xyz, vRight.xyz)), 0.f) * In[0].vColor.y * g_vSize.y;
	
	Out[0].vPosition			= mul(mul(vPosition + vRight + vUp, g_mView), g_mProj);
	Out[0].vColor				= In[0].vColor;
	Out[0].vArguement			= In[0].vArguement;
	Out[0].vStartSection		= In[0].vStartSection;
	Out[0].vTexCoord			= float2(0.f, 0.f);
	
	Out[1].vPosition			= mul(mul(vPosition - vRight + vUp, g_mView), g_mProj);
	Out[1].vColor				= In[0].vColor;
	Out[1].vArguement			= In[0].vArguement;
	Out[1].vStartSection		= In[0].vStartSection;
	Out[1].vTexCoord			= float2(1.f, 0.f);
	
	Out[2].vPosition			= mul(mul(vPosition - vRight - vUp, g_mView), g_mProj);
	Out[2].vColor				= In[0].vColor;
	Out[2].vArguement			= In[0].vArguement;
	Out[2].vStartSection		= In[0].vStartSection;
	Out[2].vTexCoord			= float2(1.f, 1.f);
	
	Out[3].vPosition			= mul(mul(vPosition + vRight - vUp, g_mView), g_mProj);
	Out[3].vColor				= In[0].vColor;
	Out[3].vArguement			= In[0].vArguement;
	Out[3].vStartSection		= In[0].vStartSection;
	Out[3].vTexCoord			= float2(0.f, 1.f);
	
	if (In[0].vColor.a)
	{
		Triangles.Append(Out[0]);
		Triangles.Append(Out[1]);
		Triangles.Append(Out[2]);
		Triangles.RestartStrip();
		
		Triangles.Append(Out[0]);
		Triangles.Append(Out[2]);
		Triangles.Append(Out[3]);
		Triangles.RestartStrip();
	}
}

[maxvertexcount(6)]
void GS_MAIN_LINE_BILLBOARD(point GS_IN_WORLD In[1], inout TriangleStream<GS_OUT> Triangles)
{
    GS_OUT Out[4];
/*
	float4 vStart			= In[0].vTranslation;
	float4 vDirection		= normalize(In[0].vLook) * In[0].vArguement.w * g_vSize.x;
	
	float4 vStartLook		= normalize(vStart - g_vCamPosition);
	float4 vStartRight		= normalize(float4(cross(vStartLook.xyz, float3(0.f, 1.f, 0.f)), 0.f));
	float4 vStartUp			= normalize(float4(cross(vStartRight.xyz, vStartLook.xyz), 0.f));
	
//	float4 vEndLook			= normalize(g_vCamPosition - (vStart + vDirection));
//	float4 vEndRight		= normalize(float4(cross(vEndLook.xyz, float3(0.f, 1.f, 0.f)), 0.f));
//	float4 vEndUp			= normalize(float4(cross(vEndRight.xyz, vEndLook.xyz), 0.f));
	
	float4 vStartThickness	= 0.5f * g_vSize.y * vStartUp;
//	float4 vEndThickness	= 0.5f * g_vSize.y * vEndUp;
//	float4 vThickness		= 0.5f * (vStartThickness + vEndThickness);
*/
	float4	vTranslation	= In[0].vTranslation;
	float4	vRight			= normalize(In[0].vLook) * In[0].vUp.y * g_vSize.x;
	float4	vStartLook		= normalize(vTranslation - g_vCamPosition);
	float	fStartUpScale	= 0.5f + 0.5f * (1.f - abs(dot(vStartLook, normalize(vRight))));
	float4	vStartUp		= normalize(float4(cross(vRight.xyz, vStartLook.xyz), 0.f)) * fStartUpScale * g_vSize.y;
	float4	vEndLook		= normalize(vTranslation + vRight - g_vCamPosition);
	float	fEndUpScale		= 0.5f + 0.5f * (1.f - abs(dot(vEndLook, normalize(vRight))));
    float4	vEndUp			= normalize(float4(cross(vRight.xyz, vEndLook.xyz), 0.f)) * fEndUpScale* g_vSize.y;
	float4	vUp				= 0.5f * (vStartUp + vEndUp) * In[0].vUp.x;
	
	float	fRatio			= In[0].vArguement.w;
	float4	vStartRight		= vRight * fRatio * (1.f - g_fLineThickness);
	float4	vEndRight		= vRight * (g_fLineThickness + fRatio * (1.f - g_fLineThickness));
	
    Out[0].vPosition		= mul(mul(vTranslation + vStartRight + vUp, g_mView), g_mProj);
    Out[0].vColor			= In[0].vColor;
    Out[0].vArguement		= In[0].vArguement;
    Out[0].vStartSection	= In[0].vStartSection;
    Out[0].vTexCoord		= float2(0.f, 0.f);
	
    Out[1].vPosition		= mul(mul(vTranslation + vEndRight + vUp, g_mView), g_mProj);
    Out[1].vColor			= In[0].vColor;
    Out[1].vArguement		= In[0].vArguement;
    Out[1].vStartSection	= In[0].vStartSection;
    Out[1].vTexCoord		= float2(1.f, 0.f);
	
    Out[2].vPosition		= mul(mul(vTranslation + vEndRight - vUp, g_mView), g_mProj);
    Out[2].vColor			= In[0].vColor;
    Out[2].vArguement		= In[0].vArguement;
    Out[2].vStartSection	= In[0].vStartSection;
    Out[2].vTexCoord		= float2(1.f, 1.f);
	
    Out[3].vPosition		= mul(mul(vTranslation + vStartRight - vUp, g_mView), g_mProj);
    Out[3].vColor			= In[0].vColor;
    Out[3].vArguement		= In[0].vArguement;
    Out[3].vStartSection	= In[0].vStartSection;
    Out[3].vTexCoord		= float2(0.f, 1.f);
	
    if (In[0].vColor.a)
    {
        Triangles.Append(Out[1]);
        Triangles.Append(Out[0]);
        Triangles.Append(Out[3]);
        Triangles.RestartStrip();
		
        Triangles.Append(Out[1]);
        Triangles.Append(Out[3]);
        Triangles.Append(Out[2]);
        Triangles.RestartStrip();
    }
}

[maxvertexcount(6)]
void GS_MAIN_LINE_BILLBOARD_NORMAL(point GS_IN_WORLD In[1], inout TriangleStream<GS_OUT> Triangles)
{
    GS_OUT Out[4];
	
	float	fCos			= dot(normalize(g_vLineNormal.xyz), normalize(In[0].vLook.xyz));
	float	fLength			= length(g_vLineNormal.xyz) / fCos;

	float4	vTranslation	= In[0].vTranslation;
	float4	vRight			= normalize(In[0].vLook) * In[0].vUp.y * fLength;
	float4	vStartLook		= normalize(vTranslation - g_vCamPosition);
	float	fStartUpScale	= 0.5f + 0.5f * (1.f - abs(dot(vStartLook, normalize(vRight))));
	float4	vStartUp		= normalize(float4(cross(vRight.xyz, vStartLook.xyz), 0.f)) * fStartUpScale * g_vSize.y;
	float4	vEndLook		= normalize(vTranslation + vRight - g_vCamPosition);
	float	fEndUpScale		= 0.5f + 0.5f * (1.f - abs(dot(vEndLook, normalize(vRight))));
    float4	vEndUp			= normalize(float4(cross(vRight.xyz, vEndLook.xyz), 0.f)) * fEndUpScale * g_vSize.y;
	float4	vUp				= 0.5f * (vStartUp + vEndUp) * In[0].vUp.x;
	
	float	fRatio			= In[0].vArguement.w;
	float4	vStartRight		= vRight * fRatio * (1.f - g_fLineThickness);
	float4	vEndRight		= vRight * (g_fLineThickness + fRatio * (1.f - g_fLineThickness));
	
    Out[0].vPosition		= mul(mul(vTranslation + vStartRight + vUp, g_mView), g_mProj);
    Out[0].vColor			= In[0].vColor;
    Out[0].vArguement		= In[0].vArguement;
    Out[0].vStartSection	= In[0].vStartSection;
    Out[0].vTexCoord		= float2(0.f, 0.f);
	
    Out[1].vPosition		= mul(mul(vTranslation + vEndRight + vUp, g_mView), g_mProj);
    Out[1].vColor			= In[0].vColor;
    Out[1].vArguement		= In[0].vArguement;
    Out[1].vStartSection	= In[0].vStartSection;
    Out[1].vTexCoord		= float2(1.f, 0.f);
	
    Out[2].vPosition		= mul(mul(vTranslation + vEndRight - vUp, g_mView), g_mProj);
    Out[2].vColor			= In[0].vColor;
    Out[2].vArguement		= In[0].vArguement;
    Out[2].vStartSection	= In[0].vStartSection;
    Out[2].vTexCoord		= float2(1.f, 1.f);
	
    Out[3].vPosition		= mul(mul(vTranslation + vStartRight - vUp, g_mView), g_mProj);
    Out[3].vColor			= In[0].vColor;
    Out[3].vArguement		= In[0].vArguement;
    Out[3].vStartSection	= In[0].vStartSection;
    Out[3].vTexCoord		= float2(0.f, 1.f);
	
    if (In[0].vColor.a)
    {
        Triangles.Append(Out[1]);
        Triangles.Append(Out[0]);
        Triangles.Append(Out[3]);
        Triangles.RestartStrip();
		
        Triangles.Append(Out[1]);
        Triangles.Append(Out[3]);
        Triangles.Append(Out[2]);
        Triangles.RestartStrip();
    }
}

[maxvertexcount(6)]
void GS_MAIN_LINE_REVERSE_BILLBOARD_NORMAL(point GS_IN_WORLD In[1], inout TriangleStream<GS_OUT> Triangles)
{
    GS_OUT Out[4];
	
	float	fCosA			= dot(normalize(g_vLineNormal.xyz), normalize(In[0].vTranslation.xyz));
	float	fLengthA		= length(g_vLineNormal.xyz) / fCosA;
	float	fCosB			= dot(normalize(g_vLineNormal.xyz), normalize(In[0].vLook.xyz));
	float	fLengthB		= length(g_vLineNormal.xyz) / fCosB;
	
	float4	vStartPoint		= float4(In[0].vUp.xyz, 1.f);
	float4	vDirectionA		= fLengthA * normalize(In[0].vTranslation);
	float4	vDirectionB		= fLengthB * normalize(In[0].vLook);
	
	float	fFullRatio		= In[0].vRight.w;
	float	fRatio			= In[0].vArguement.w;
	
	float4	vPointA			= vStartPoint + vDirectionA * fFullRatio;
	float4	vPointB			= vStartPoint + vDirectionB * fFullRatio;
	float4	vRight			= vPointB - vPointA;

	float4	vLookA			= normalize(vPointA - g_vCamPosition);
	float	fUpScaleA		= 0.5f + 0.5f * (1.f - abs(dot(vLookA, normalize(vRight))));
	float4	vUpA			= normalize(float4(cross(vRight.xyz, vLookA.xyz), 0.f)) * fUpScaleA * g_vSize.y;
	
	float4	vLookB			= normalize(vPointB - g_vCamPosition);
	float	fUpScaleB		= 0.5f + 0.5f * (1.f - abs(dot(vLookB, normalize(vRight))));
	float4	vUpB			= normalize(float4(cross(vRight.xyz, vLookA.xyz), 0.f)) * fUpScaleB * g_vSize.y;
	
	float4	vUp				= 0.5f * (vUpA + vUpB) * In[0].vUp.w;
	
    Out[0].vPosition		= mul(mul(vPointA + vUp, g_mView), g_mProj);
    Out[0].vColor			= In[0].vColor;
    Out[0].vArguement		= In[0].vArguement;
    Out[0].vStartSection	= In[0].vStartSection;
    Out[0].vTexCoord		= float2(0.f, 0.f);
	
    Out[1].vPosition		= mul(mul(vPointB + vUp, g_mView), g_mProj);
    Out[1].vColor			= In[0].vColor;
    Out[1].vArguement		= In[0].vArguement;
    Out[1].vStartSection	= In[0].vStartSection;
    Out[1].vTexCoord		= float2(1.f, 0.f);
	
    Out[2].vPosition		= mul(mul(vPointB - vUp, g_mView), g_mProj);
    Out[2].vColor			= In[0].vColor;
    Out[2].vArguement		= In[0].vArguement;
    Out[2].vStartSection	= In[0].vStartSection;
    Out[2].vTexCoord		= float2(1.f, 1.f);
	
    Out[3].vPosition		= mul(mul(vPointA - vUp, g_mView), g_mProj);
    Out[3].vColor			= In[0].vColor;
    Out[3].vArguement		= In[0].vArguement;
    Out[3].vStartSection	= In[0].vStartSection;
    Out[3].vTexCoord		= float2(0.f, 1.f);
	
    if (In[0].vColor.a)
    {
        Triangles.Append(Out[1]);
        Triangles.Append(Out[0]);
        Triangles.Append(Out[3]);
        Triangles.RestartStrip();
		
        Triangles.Append(Out[1]);
        Triangles.Append(Out[3]);
        Triangles.Append(Out[2]);
        Triangles.RestartStrip();
    }
}

struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
	float4 vArgument	: TEXCOORD2; 
	float2 vStartSection : TEXCOORD3;
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

PS_OUT_POSTPROCESS PS_MAIN(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;

	Out.vPreProcess		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
//	Out.vPreProcess.w	*=	lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
		Out.vPreProcess.rgb	+= vEmissive.rgb * vEmissive.a;
	}
    if (g_bDeclareColorAsArguement)
    {
		Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
		Out.vPreProcess *= In.vColor;
    }
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_SPRITESHEET(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;

//	Out.vPreProcess		=	SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlDiffuse * In.vColor;
	Out.vPreProcess		=	SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
//	Out.vPreProcess.w	*=	lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
	//	Out.vPreProcess += SpriteSheet(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlEmissive * In.vColor;
	//	Out.vPreProcess += SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
		float4 vEmissive = SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
		Out.vPreProcess.rgb	+= vEmissive.rgb * vEmissive.a;
	}
    if (g_bDeclareColorAsArguement)
    {
		Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
		Out.vPreProcess *= In.vColor;
    }
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_SPRITESHEET_LINE(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;

//	Out.vPreProcess		=	SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlDiffuse * In.vColor;
	Out.vPreProcess		=	SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
//	Out.vPreProcess.w	*=	lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
	//	Out.vPreProcess += SpriteSheet(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlEmissive * In.vColor;
	//	Out.vPreProcess += SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
		float4 vEmissive = SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
	Out.vPreProcess.a	*=	In.vArgument.x;
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_SPRITESHEET_LINE_GRAYSCALE(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;

//	Out.vPreProcess		=	SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlDiffuse * In.vColor;
	Out.vPreProcess		=	SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
	Out.vPreProcess.a	=	AvgGrayScale(Out.vPreProcess.rgb);
//	Out.vPreProcess.w	*=	lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
	//	Out.vPreProcess += SpriteSheet(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlEmissive * In.vColor;
	//	Out.vPreProcess += SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
		float4 vEmissive = SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
	Out.vPreProcess.a	*=	In.vArgument.x;
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_GRAYSCALE(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;

	Out.vPreProcess		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
	Out.vPreProcess.a	=	AvgGrayScale(Out.vPreProcess.rgb);
//	Out.vPreProcess.w	*=	lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		float4 vEmissive =	g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_GRAYSCALE_SPRITESHEET(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vPreProcess = SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
	Out.vPreProcess.a	=	AvgGrayScale(Out.vPreProcess.rgb);
//	Out.vPreProcess.w	*=	lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
	//	Out.vPreProcess += SpriteSheet(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlEmissive;
		float4 vEmissive = SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_TRAIL_QUAD(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vPreProcess		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
//	Out.vPreProcess.w	*=	lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
//	Out.vPreProcess.w	=	max(lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x), 0.f);
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
//+	Out.vPreProcess.a	*=	In.vColor.a;
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_TRAIL_QUAD_DISTORTION(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vPreProcess		=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vProcessData	=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	Out.vMask			=	float4(In.vTexCoord.xy, g_fDistortionPower * In.vColor.a, 0.1f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_WORLD(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vPreProcess		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		float4 vEmissive =	g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_WORLD_SPRITESHEET(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vPreProcess		=	SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		float4 vEmissive =	SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
    Out.vPreProcess.a	*= In.vArgument.x;
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_WORLD_SPRITESHEET_GRAYSCALE(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vPreProcess		=	SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection);
	Out.vPreProcess.a	=	AvgGrayScale(Out.vPreProcess.rgb);
	Out.vPreProcess		*=	g_vMtrlDiffuse;
	if (g_iShaderFlag & STATUS_TEXEMISSIVE)
	{
		float4 vEmissive =	SampleSection(g_texEmission[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
    Out.vPreProcess.a	*= In.vArgument.x;
	Out.vProcessData	=	float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask			=	float4(0.f, 0.f, 1.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;

    Out.vPreProcess = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vPreProcess *= g_vMtrlDiffuse;
    Out.vPreProcess.rgb += g_vMtrlEmissive.rgb * g_vMtrlEmissive.a;
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
    Out.vPreProcess.a *= In.vArgument.x;
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_POINT_GRAYSCALE(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;

    Out.vPreProcess = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vPreProcess.a = AvgGrayScale(Out.vPreProcess.rgb);
    Out.vPreProcess *= g_vMtrlDiffuse;
    if (g_iShaderFlag & STATUS_TEXEMISSIVE)
    {
        float4 vEmissive = g_texEmission[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlEmissive;
        Out.vPreProcess.rgb += vEmissive.rgb * vEmissive.a;
    }
    if (g_bDeclareColorAsArguement)
    {
        Out.vPreProcess.a *= In.vColor.a;
    }
    else
    {
        Out.vPreProcess *= In.vColor;
    }
    Out.vPreProcess.a *= In.vArgument.x;
	Out.vProcessData = float4(0.f, 0.f, 0.f, 0.f);
	Out.vMask = float4(0.f, 0.f, 1.f, 0.f);
	
    return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_POINT(PS_IN In)
{
    PS_OUT_BLOOM Out;
	
    Out.vTexture = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vTexture *= g_vMtrlDiffuse;
    if (g_bDeclareColorAsArguement)
    {
        Out.vTexture.a *= In.vColor.a;
    }
    else
    {
        Out.vTexture *= In.vColor;
    }
    Out.vTexture *= In.vArgument.x;
    Out.vTexture.rgb *= Out.vTexture.a;
	//	Out.vData = float4(1.f, 1.f, 1.f, Out.vTexture.a);
	//	Out.vTexture.a = g_fBloomStrength * In.vColor.a;
    Out.vData = float4(In.vArgument.y, 0.f, 0.f, 0.f);
	
    return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_POINT_GRAYSCALE(PS_IN In)
{
    PS_OUT_BLOOM Out;
	
    Out.vTexture = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vTexture = AvgGrayScale(Out.vTexture.rgb);
    Out.vTexture *= g_vMtrlDiffuse;
    if (g_bDeclareColorAsArguement)
    {
        Out.vTexture.a *= In.vColor.a;
    }
    else
    {
        Out.vTexture *= In.vColor;
    }
    Out.vTexture *= In.vArgument.x;
//	Out.vTexture *= fGrayScale;
	//	Out.vData = float4(1.f, 1.f, 1.f, Out.vTexture.a);
	//	Out.vTexture.a = g_fBloomStrength * In.vColor.a;
    Out.vData = float4(In.vArgument.y, 0.f, 0.f, 0.f);
	
    return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM(PS_IN In)
{
	PS_OUT_BLOOM Out;

	Out.vTexture = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord) * g_vMtrlDiffuse;
    if (g_bDeclareColorAsArguement)
    {
        Out.vTexture.a *= In.vColor.a;
    }
    else
    {
        Out.vTexture *= In.vColor;
    }
//	Out.vData = float4(1.f, 1.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a = g_fBloomStrength * In.vColor.a;
	Out.vData = float4(g_fBloomStrength * In.vColor.a, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_SPRITESHEET(PS_IN In)
{
	PS_OUT_BLOOM Out;

//	Out.vTexture = SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlDiffuse * In.vColor;
	Out.vTexture = SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
    if (g_bDeclareColorAsArguement)
    {
        Out.vTexture.a *= In.vColor.a;
    }
    else
    {
        Out.vTexture *= In.vColor;
    }
//	Out.vData = float4(1.f, 1.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a = g_fBloomStrength * In.vColor.a;
	Out.vData = float4(g_fBloomStrength * In.vColor.a, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_SPRITESHEET_LINE(PS_IN In)
{
	PS_OUT_BLOOM Out;

//	Out.vTexture = SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlDiffuse * In.vColor;
	Out.vTexture = SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
    if (g_bDeclareColorAsArguement)
    {
        Out.vTexture.a *= In.vColor.a;
    }
    else
    {
        Out.vTexture *= In.vColor;
    }
    Out.vTexture.a *= In.vArgument.x;
//	Out.vData = float4(10.f, 0.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a = In.vArgument.x * In.vArgument.y;
	Out.vData = float4(In.vArgument.x * In.vArgument.y, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_SPRITESHEET_LINE_GRAYSCALE(PS_IN In)
{
	PS_OUT_BLOOM Out;

//	Out.vTexture = SpriteSheet(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vArgument.z) * g_vMtrlDiffuse * In.vColor;
	Out.vTexture = SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
    Out.vTexture.a = AvgGrayScale(Out.vTexture.rgb);
    if (g_bDeclareColorAsArguement)
    {
        Out.vTexture.a *= In.vColor.a;
    }
    else
    {
        Out.vTexture *= In.vColor;
    }
    Out.vTexture.a *= In.vArgument.x;
//	Out.vData = float4(10.f, 0.f, 1.f, Out.vTexture.a);
//	Out.vTexture.a = In.vArgument.x * In.vArgument.y;
	Out.vData = float4(In.vArgument.x * In.vArgument.y, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_WORLD_SPRITESHEET(PS_IN In)
{
	PS_OUT_BLOOM Out;

    Out.vTexture = SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection) * g_vMtrlDiffuse;
    if (g_bDeclareColorAsArguement)
    {
        Out.vTexture.a *= In.vColor.a;
    }
    else
    {
        Out.vTexture *= In.vColor;
    }
    Out.vTexture *= In.vArgument.x;
	Out.vData = float4(In.vArgument.x * In.vArgument.y, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_BLOOM PS_MAIN_BLOOM_WORLD_SPRITESHEET_GRAYSCALE(PS_IN In)
{
	PS_OUT_BLOOM Out;

    Out.vTexture = SampleSection(g_texDiffuse[0], In.vTexCoord, g_vSpriteSheetSize, In.vStartSection);
    Out.vTexture.a = AvgGrayScale(Out.vTexture.rgb);
	Out.vTexture *= g_vMtrlDiffuse;
    if (g_bDeclareColorAsArguement)
    {
        Out.vTexture.a *= In.vColor.a;
    }
    else
    {
        Out.vTexture *= In.vColor;
    }
    Out.vTexture *= In.vArgument.x;
	Out.vData = float4(In.vArgument.x * In.vArgument.y, 0.f, 0.f, 0.f);
	
	return Out;
}

technique11 DefaultTechnique
{
	// 0
	pass Default
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 1
	pass Billboard
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 2
	pass BillBoard_GrayScale
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_GRAYSCALE();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 3
	pass Trail_Quad
	{
		VertexShader	= compile vs_5_0 VS_MAIN_QUAD();
		GeometryShader	= compile gs_5_0 GS_MAIN_QUAD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_TRAIL_QUAD();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 4
	pass Trail_Quad_Distortion
	{
		VertexShader	= compile vs_5_0 VS_MAIN_QUAD();
		GeometryShader	= compile gs_5_0 GS_MAIN_QUAD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_TRAIL_QUAD_DISTORTION();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 5
	pass BillBoard_SpriteSheet
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_SPRITESHEET();

		SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 6
	pass BillBoard_GrayScale_SpriteSheet
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_GRAYSCALE_SPRITESHEET();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 7
	pass World_BillBoard
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 8
	pass World_BillBoard_GrayScale
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_GRAYSCALE();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 9
	pass World_BillBoard_SpriteSheet
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_SPRITESHEET();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 10
	pass World_BillBoard_GrayScale_SpriteSheet
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_GRAYSCALE_SPRITESHEET();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 11
	pass Bloom_Billboard
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 12
	pass Bloom_TrailQuad
	{
		VertexShader	= compile vs_5_0 VS_MAIN_QUAD();
		GeometryShader	= compile gs_5_0 GS_MAIN_QUAD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 13
	pass Bloom_Billboard_SpriteSheet
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM_SPRITESHEET();

		SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 14
	pass Bloom_World_Billboard
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 15
	pass Bloom_World_Billboard_Sprite
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_BLOOM_SPRITESHEET();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 16
	pass BillBoard_SpriteSheet_Line
	{
		VertexShader = compile vs_5_0 VS_MAIN_LINE();
		GeometryShader = compile gs_5_0 GS_MAIN_LINE();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SPRITESHEET_LINE();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 17
	pass Bloom_BillBoard_SpriteSheet_Line
	{
		VertexShader = compile vs_5_0 VS_MAIN_LINE();
		GeometryShader = compile gs_5_0 GS_MAIN_LINE();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLOOM_SPRITESHEET_LINE();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 18
	pass World
	{
		VertexShader = compile vs_5_0 VS_MAIN_WORLD();
		GeometryShader = compile gs_5_0 GS_MAIN_WORLD();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WORLD();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 19
	pass World_SpriteSheet
	{
		VertexShader = compile vs_5_0 VS_MAIN_WORLD();
		GeometryShader = compile gs_5_0 GS_MAIN_WORLD();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET();

		SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 20
	pass Bloom_World_SpriteSheet
	{
		VertexShader = compile vs_5_0 VS_MAIN_WORLD();
		GeometryShader = compile gs_5_0 GS_MAIN_WORLD();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
	}

	// 21
    pass Local_Point
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LOCAL_BILLBOARD();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 22
    pass Bloom_Local_Point
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LOCAL_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_POINT();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 23
    pass World_Point
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_WORLD_BILLBOARD();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 24
    pass Bloom_World_Point
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_WORLD_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_POINT();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 25
    pass Local_Point_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LOCAL_BILLBOARD();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT_GRAYSCALE();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 26
    pass Bloom_Local_Point_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LOCAL_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_POINT_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 27
    pass World_Point_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_WORLD_BILLBOARD();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT_GRAYSCALE();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 28
    pass Bloom_World_Point_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_WORLD_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_POINT_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 29
    pass World_Point_Billboard_SpriteSheet
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_WORLD_BILLBOARD();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 30
    pass Bloom_World_Point_Billboard_SpriteSheet
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_WORLD_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 31
    pass World_Point_Billboard_SpriteSheet_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_WORLD_BILLBOARD();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET_GRAYSCALE();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 32
    pass Bloom_World_Point_Billboard_SpriteSheet_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_WORLD_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 33
    pass World_Line_Billboard_Sprite
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 34
    pass World_Line_Billboard_Sprite_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 35
    pass Bloom_World_Line_Billboard_Sprite
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 36
    pass Bloom_World_Line_Billboard_Sprite_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_BILLBOARD();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 37
    pass World_Line_Normal_Billboard_Sprite
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_BILLBOARD_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 38
    pass World_Line_Normal_Billboard_Sprite_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_BILLBOARD_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 39
    pass Bloom_World_Line_Normal_Billboard_Sprite
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_BILLBOARD_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 40
    pass Bloom_World_Line_Normal_Billboard_Sprite_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_BILLBOARD_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 41
    pass World_Line_Reverse_Normal_Billboard_Sprite
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_REVERSE_BILLBOARD_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 42
    pass World_Line_Reverse_Normal_Billboard_Sprite_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_REVERSE_BILLBOARD_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WORLD_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 43
    pass Bloom_World_Reverse_Line_Normal_Billboard_Sprite
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_REVERSE_BILLBOARD_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 44
    pass Bloom_World_Line_Reverse_Normal_Billboard_Sprite_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_WORLD();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE_REVERSE_BILLBOARD_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_WORLD_SPRITESHEET_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AdditiveBlend_Particle, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 45
    pass BillBoard_SpriteSheet_Line_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_LINE();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SPRITESHEET_LINE_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }

	// 46
    pass Bloom_BillBoard_SpriteSheet_Line_GrayScale
    {
        VertexShader = compile vs_5_0 VS_MAIN_LINE();
        GeometryShader = compile gs_5_0 GS_MAIN_LINE();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM_SPRITESHEET_LINE_GRAYSCALE();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthRead_Particle, 0);
    }
}
