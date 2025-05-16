#include "ShaderCommon.hlsli"

struct TRANSCOLORARG
{
    float4 vRight;
    float4 vUp;
    float4 vLook;
    float4 vTranslation;
    float4 vColor;
    float4 vArgument;
};

StructuredBuffer<int>				g_inputInt;
StructuredBuffer<float3>			g_inputFloat3;
StructuredBuffer<float4>			g_inputFloat4;
StructuredBuffer<TRANSCOLORARG>		g_inputTransColorArg;

RWStructuredBuffer<int>				g_outputInt;
RWStructuredBuffer<float3>			g_outputFloat3;
RWStructuredBuffer<float4>			g_outputFloat4;
RWStructuredBuffer<TRANSCOLORARG>	g_outputTransColorArg;

float3								g_vSphericalCenter	= float3(0.f, 0.f, 0.f);
float2								g_vTiling			= float2(1.f, 1.f);

[numthreads(32, 1, 1)]
void VECTORFIELD_NOISE_00( uint3 DTid : SV_DispatchThreadID )
{
	float3	vSphericalRelative	= g_inputTransColorArg[DTid.x].vTranslation.xyz - g_vSphericalCenter;
	float	fLongitude			= atan2(vSphericalRelative.y, vSphericalRelative.x);
	float	fLatitude			= asin(vSphericalRelative.z / length(vSphericalRelative));
	float2	vTexCoord			= float2((fLongitude / PI) * 0.5f + 0.5f, (fLatitude / PI) + 0.5f);
	
	g_outputFloat3[DTid.x]		= g_texMask.SampleLevel(LinearSampler, vTexCoord * g_vTiling, 0).xyz * 2.f - 1.f;
}

[numthreads(32, 1, 1)]
void VECTORFIELD_NOISE_01( uint3 DTid : SV_DispatchThreadID )
{
	float3	vSphericalRelative	= g_inputTransColorArg[DTid.x].vUp.xyz - g_vSphericalCenter;
	float	fLongitude			= atan2(vSphericalRelative.z, vSphericalRelative.x);
	float	fLatitude			= asin(vSphericalRelative.y / length(vSphericalRelative));
	float2	vTexCoord			= float2(0.5f, 0.5f) + 0.5f * float2(cos(fLongitude), -sin(fLongitude)) * (cos(fLatitude + PI * 0.5f) * 0.5f + 0.5f);
	
	g_outputFloat3[DTid.x]		= g_texMask.SampleLevel(LinearSampler, vTexCoord * g_vTiling, 0).xyz * 2.f - 1.f;
}

technique11 DefaultTechnique
{
	// 0
	pass VECTORFIELD_NOISE_00
	{
		VertexShader	= NULL;
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= NULL;

        ComputeShader	= compile cs_5_0 VECTORFIELD_NOISE_00();
    }

	// 1
	pass VECTORFIELD_NOISE_01
	{
		VertexShader	= NULL;
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= NULL;

        ComputeShader	= compile cs_5_0 VECTORFIELD_NOISE_01();
    }
}