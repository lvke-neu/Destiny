#include "utility.hlsl"

cbuffer cbView : register(b1)
{
	matrix g_view; 
    float3 g_eyePosition;
}

cbuffer cbProj : register(b2)
{
	matrix g_proj;
	float g_viewportWidth;
	float g_rcpViewportWidth;
	float g_viewportHeight;
	float g_rcpViewportHeight;
    float g_nearPlane;
    float g_farPlane;
}


struct DirectionLight
{
    float4 lightColor;
    float3 lightDirection;
    float lightIntensity;
};

struct PointLight
{
    float4 lightColor;
    float3 lightPosition;
    float lightIntensity;
};

cbuffer cbLight : register(b3)
{
    int g_directionLightCount;
    DirectionLight g_directionLights[1000];
    int g_pointLightCount;
    PointLight g_pointLights[1000];
}

cbuffer cbShadow : register(b4)
{
    matrix g_shadowView;
    matrix g_shadowProj;
    matrix T;
    float  g_shadowBias;
    float  g_dx;
    float  g_dy;
}

struct GBuffer
{
    float4 albedo    : SV_Target0;
    float4 normal    : SV_Target1;
    float4 mra	     : SV_Target2;
    float4 positionW : SV_Target3;
    float4 normalW   : SV_Target4;
    float4 texcoord  : SV_Target5;
    float4 shadowPosH : SV_Target6;
};

Texture2D t_shadowMap : register(t10);
SamplerComparisonState s_shadowMapSampler : register(s10);

float calculateShadow(float4 shadowPosH)
{
	// 透视除法
	shadowPosH.xyz /= shadowPosH.w;

	// NDC空间的深度值
	float depth = shadowPosH.z - g_shadowBias;

	float percentLit = 0.0f;
	const float2 offsets[9] =
	{
		float2(-g_dx, -g_dy), float2(0.0f, -g_dy), float2(g_dx, -g_dy),
		float2(-g_dx, 0.0f), float2(0.0f, 0.0f), float2(g_dx, 0.0f),
		float2(-g_dx, +g_dy), float2(0.0f, +g_dy), float2(g_dx, +g_dy)
	};

	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		percentLit += t_shadowMap.SampleCmpLevelZero(s_shadowMapSampler,
			shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.0f;
}