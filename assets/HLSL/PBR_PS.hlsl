#include "Common.hlsli"

cbuffer cbPBRMaterial : register(b6)
{
	float4 g_albedo;
	float g_metallic;
	float g_roughness;
	float g_ao;
	float pad;
}
const float PI = 3.14159265359;

float DistributionGGX(float3 N, float3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = NdotH2 * (a2 - 1.0f) + 1.0f;
	denom = 3.14159265359 * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0f);
	float k = (r * r) / 8.0f;

	float nom = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return nom / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0f);
	float NdotL = max(dot(N, L), 0.0f);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
	return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

float4 PS(VertexOut pIn) : SV_Target
{
	float3 N = normalize(pIn.normalW);
	float3 V = normalize(g_eyePos.xyz - pIn.positionW.xyz);


	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	F0 = lerp(F0, g_albedo, g_metallic);

	float3 Lo = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 5; i++)
	{
		//radiance
		float3 L = normalize(g_pointLightPosition[i].xyz - pIn.positionW.xyz);
		float3 H = normalize(V + L);
		float distance = length(g_pointLightPosition[i].xyz - pIn.positionW.xyz);
		float attenuation = 1.0f / (distance * distance);
		float3 radiance = g_pointLightColor[i].xyz * attenuation;

		// cook-torrance brdf
		float NDF = DistributionGGX(N, H, g_roughness);
		float G = GeometrySmith(N, V, L, g_roughness);
		float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);

		float3 kS = F;
		float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
		kD *= 1.0f - g_metallic;

		float3 nominator = NDF * G * F;
		float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
		float3 specular = nominator / denominator;

		float NdotL = max(dot(N, L), 0.0f);
		Lo += (kD * g_albedo / 3.14159265359 + specular) * radiance * NdotL;
	}


	float3 ambient = float3(0.03f, 0.03f, 0.03f) * g_albedo * g_ao;
	float3 color = ambient + Lo;

	color = color / (color + float3(1.0f, 1.0f, 1.0f));
	color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

	return float4(color, 1.0f);
}
