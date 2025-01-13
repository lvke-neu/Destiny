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
    DirectionLight g_directionLights[250];
    int g_pointLightCount;
    PointLight g_pointLights[250];
}