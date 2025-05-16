#include "ShaderDefine.hlsli"

float		g_fGameTime				= 0.f;
int			g_iShaderFlag;

float4x4	g_mWorld, g_mView, g_mProj;
float4x4	g_mOrthographic;
float4x4	g_mViewInv, g_mProjInv;
float4		g_vCamPosition;
float		g_fCamFar				= 10000.f;

Texture2D	g_texMask;
Texture2D	g_texExtraMask;
Texture2D	g_texDiffuse			[MAX_TEXTURE];
Texture2D	g_texNormal				[MAX_TEXTURE];
Texture2D	g_texAmbient			[MAX_TEXTURE];
Texture2D	g_texSpecular			[MAX_TEXTURE];
Texture2D	g_texEmission			[MAX_TEXTURE];
Texture2D	g_texDissolve			[MAX_TEXTURE];

TextureCube	g_texReflection;

float		g_fTilingDiffuse		[MAX_TEXTURE];
float		g_fTilingNormal			[MAX_TEXTURE];
float		g_fTilingEmissive		[MAX_TEXTURE];
float2		g_vTilingDiffuse		= float2(1.f, 1.f);
float2		g_vTilingNormal			= float2(1.f, 1.f);
float2		g_vTilingSpecular		= float2(1.f, 1.f);
float2		g_vTilingEmissive		= float2(1.f, 1.f);
int			g_iLightCount			= 1;
int			g_iLightType;			//	[MAX_LIGHT];
float4		g_vLightPosition;		//	[MAX_LIGHT];
float4		g_vLightDirection;		//	[MAX_LIGHT];
float4		g_vLightDiffuse;		//	[MAX_LIGHT];
float4		g_vLightAmbient;		//	[MAX_LIGHT];
float4		g_vLightSpecular;		//	[MAX_LIGHT];
float		g_fLightRange;			//	[MAX_LIGHT];
float		g_fLightAttenuation0;	//	[MAX_LIGHT];
float		g_fLightAttenuation1;	//	[MAX_LIGHT];
float		g_fLightAttenuation2;	//	[MAX_LIGHT];

float4x4	g_mBones				[MAX_BONE];

float4		g_vMtrlDiffuse;
float4		g_vMtrlAmbient;
float4		g_vMtrlSpecular;
float4		g_vMtrlEmissive;
float		g_fMtrlShininess		= 16.f;

float		g_fTPSharpness			= 2.f;

float3		g_vSize					= float3(1.f, 1.f, 1.f);
float		g_fThickness			= 1.f;
int			g_iMaxInstance			= 100;

int			g_iSpriteSheetIndex		= 0;
int2		g_vSpriteSheetSize		= int2(1, 1);
float		g_fFadeRatio			= 0.f;
float2		g_vFadeScope			= float2(0.5f, 0.5f);

float2		g_vUVOffset				= float2(0.f, 0.f);
float2		g_vUVOffset1			= float2(0.f, 0.f);
float2		g_vUVOffset2			= float2(0.f, 0.f);
float2		g_vUVOffset3			= float2(0.f, 0.f);

float		g_fOutLineSize			= 1.0f;
float3		g_vOutlineColor			= float3(1.f, 1.f, 1.f);

int			g_iInstanceIndex		[MAX_INSTANCE];

float		g_fBloomStrength		= 0.f;

float AvgGrayScale(float3 vRGB)
{
	return (vRGB.r + vRGB.g + vRGB.b) / 3.f;
}

float2 RotateUV(float2 vUV, float fRadian)
{
	float fSin = sin(fRadian);
	float fCos = cos(fRadian);

	float2 vRotateUV;
	vRotateUV.x = vUV.x * fCos - vUV.y * fSin;
	vRotateUV.y = vUV.x * fSin + vUV.y * fCos;
	
	return vRotateUV;
}

float3 BlendFactor(float3 vNormal)
{
	float3	vWeights		=	abs(vNormal);
	vWeights				/=	dot(vWeights, 1.f);

	float3	vBlendFactor	=	pow(vWeights, g_fTPSharpness);
	vBlendFactor			/=	dot(vBlendFactor, 1.f);
	
	return vBlendFactor;
}

float3 UnpackNormalDXT5nm(float4 vPackedNormal, float3x3 mTBN)
{
	float3 vNormalMap;
	
	vNormalMap.rg	= vPackedNormal.ag * 2.f - 1.f;
	vNormalMap.b	= sqrt(1.f - saturate(dot(vNormalMap.rg, vNormalMap.rg)));
	
	return normalize(mul(vNormalMap, mTBN));
}

float4 SpriteSheet(Texture2D texSpriteSheet, float2 vTexCoord, int2 vSize, uint iIndex)
{
	uint2	vRowCol	=	uint2(iIndex % vSize.x, iIndex / vSize.x);
	float2	vOffset	=	float2(1.f / vSize.x, 1.f / vSize.y) * vRowCol;
	
	vTexCoord		*=	float2(1.f / vSize.x, 1.f / vSize.y);
	
	return texSpriteSheet.Sample(PointSampler, vTexCoord + vOffset);
}

float4 SampleSection(Texture2D textureSheet, float2 vTexCoord, int2 vSize, float2 vStartSection)
{
	float2 vOffset = float2(1.f / vSize.x, 1.f / vSize.y);
	return textureSheet.Sample(LinearSampler, vTexCoord * vOffset + vStartSection);
}

float2 ScalarSinCos(float fValue)
{
	float2	vOut;
	float	fQuotient	= DIV2PI * fValue;
	if (fValue >= 0.f)
	{
		fQuotient		= float(int(fQuotient + 0.5f));
	}
	else
	{
		fQuotient		= float(int(fQuotient - 0.5f));
	}
	float	fY			= fValue - PI2 * fQuotient;
	float	fSign;
	if (fY > PIDIV2)
	{
		fY				= PI - fY;
		fSign			= -1.f;
	}
	else if (fY < -PIDIV2)
	{
		fY				= -PI - fY;
		fSign			= -1.f;
	}
	else
	{
		fSign			= +1.f;
	}
	float	fY2			= fY * fY;
	vOut.x				= (((((-2.3889859e-08f * fY2 + 2.7525562e-06f) * fY2 - 0.00019840874f) * fY2 + 0.0083333310f) * fY2 - 0.16666667f) * fY2 + 1.f) * fY;
	vOut.y				= fSign * (((((-2.6051615e-07f * fY2 + 2.4760495e-05f) * fY2 - 0.0013888378f) * fY2 + 0.041666638f) * fY2 - 0.5f) * fY2 + 1.f);
	
	return vOut;
}

float4x4 AxisRotation(float3 vAxis, float fRadian)
{
	vector	vNormal		= normalize(vector(vAxis, 0.f));
	
	float2	vSinCos		= ScalarSinCos(fRadian);
	float	fSinAngle	= vSinCos.x;
	float	fCosAngle	= vSinCos.y;
	
	vector	A           = vector(fSinAngle, fCosAngle, 1.f - fCosAngle, 0.f);

	vector	C2          = vector(A.z, A.z, A.z, A.z);
	vector	C1          = vector(A.y, A.y, A.y, A.y);
	vector	C0          = vector(A.x, A.x, A.x, A.x);

	vector	N0          = vector(vNormal.y, vNormal.z, vNormal.x, vNormal.w);
	vector	N1          = vector(vNormal.z, vNormal.x, vNormal.y, vNormal.w);

	vector	V0          = C2 * N0;
	V0					= V0 * N1;

	vector	R0			= C2 * vNormal;
	R0					= R0 * vNormal + C1;

	vector	R1			= C0 * vNormal + V0;
	vector	R2			= V0 - C0 * vNormal;

	V0					= vector(R0.x, R0.y, R0.z, A.w);
	vector	V1			= vector(R1.z, R2.y, R2.z, R1.x);
	vector	V2			= vector(R1.y, R2.x, R1.y, R2.x);

	matrix	M;
	M[0]				= vector(V0.x, V1.x, V1.y, V0.w);
	M[1]				= vector(V1.z, V0.y, V1.w, V0.w);
	M[2]				= vector(V2.x, V2.y, V0.z, V0.w);
	M[3]				= vector(0.f, 0.f, 0.f, 1.f);
	
	return M;
}
