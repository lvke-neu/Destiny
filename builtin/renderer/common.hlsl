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


struct GBuffer
{
    float4 albedo    : SV_Target0;
    float4 normal    : SV_Target1;
    float4 mra	     : SV_Target2;
    float4 positionW : SV_Target3;
    float4 normalW   : SV_Target4;
    float4 texcoord  : SV_Target5;
};