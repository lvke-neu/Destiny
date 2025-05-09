#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
    matrix u_worldInvTransposeMatrix;
}

struct VertexIn
{
	float3 positionL : POSITION;
	float3 normalL	 : NORMAL;
	float2 texcoord  : TEXCOORD;
};

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float4 positionW : POSITION;
    float3 normalW   : NORMAL;
	float2 texcoord  : TEXCOORD;
    float4 shadowPosH : TEXCOORD1;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_view), g_proj);
    vOut.positionW = mul(float4(vIn.positionL, 1.0f), u_worldMatrix);
    vOut.normalW = mul(vIn.normalL, (float3x3) u_worldInvTransposeMatrix);
	vOut.texcoord = vIn.texcoord;
    vOut.shadowPosH = mul(mul(mul(vOut.positionW, g_shadowView), g_shadowProj), T);

	return vOut;
}

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ao : register(t4);
SamplerState s_sampler : register(s0);

float4 PS(VertexOut pIn) : SV_Target
{
	float3 albedo = pow(t_albedo.Sample(s_sampler, pIn.texcoord).xyz, float3(2.2f, 2.2f, 2.2f));
	float metallic = t_metallic.Sample(s_sampler, pIn.texcoord).x;
	float roughness = t_roughness.Sample(s_sampler, pIn.texcoord).x;
	float ao = t_ao.Sample(s_sampler, pIn.texcoord).x;

	//getNormalFromMap---------
	float3 tangentNormal = t_normal.Sample(s_sampler, pIn.texcoord).xyz * 2.0f - 1.0f;
	float3 Q1 = ddx(pIn.positionW.xyz);
	float3 Q2 = ddy(pIn.positionW.xyz);
	float2 st1 = ddx(pIn.texcoord);
	float2 st2 = ddy(pIn.texcoord);

	float3 N_ = normalize(pIn.normalW);
	float3 T = normalize(Q1 * st2.y - Q2 * st1.y);
	float3 B = -normalize(cross(N_, T));
	
	float3x3 TBN = float3x3(T, B, N_);
	
	//-------------------------

	float3 N = normalize(mul(tangentNormal, TBN));
	float3 V = normalize(g_eyePosition.xyz - pIn.positionW.xyz);

	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	F0 = lerp(F0, albedo, metallic);

	float3 Lo = float3(0.0f, 0.0f, 0.0f);

    //shadow
    float shadow = calculateShadow(pIn.shadowPosH);

	//radiance
    for (int i = 0; i < g_directionLightCount; i++)
    {
        float3 L = normalize(-g_directionLights[i].lightDirection);
        float3 H = normalize(V + L);

        float3 radiance = g_directionLights[i].lightColor.xyz * g_directionLights[i].lightIntensity;

	// cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);

        float3 nominator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
        float3 specular = nominator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
        kD *= 1.0f - metallic;

        float NdotL = max(dot(N, L), 0.0f);
        
        Lo += (kD * albedo / PI + specular /*+ t_environment.Sample(s_sampler, reflect(-V, N_)).xyz * kS*/) * radiance * NdotL * shadow;

    }
    for (int j = 0; j < g_pointLightCount; j++)
    {
        float3 L = normalize(g_pointLights[j].lightPosition - pIn.positionW.xyz);
        float3 H = normalize(V + L);

        float distance = length(g_pointLights[j].lightPosition - pIn.positionW.xyz);
        float attenuation = 1.0 / (distance * distance);
        float3 radiance = g_pointLights[j].lightColor.xyz * g_pointLights[j].lightIntensity * attenuation;

	// cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);

        float3 nominator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
        float3 specular = nominator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
        kD *= 1.0f - metallic;

        float NdotL = max(dot(N, L), 0.0f);
        Lo += (kD * albedo / PI + specular /*+ t_environment.Sample(s_sampler, reflect(-V, N_)).xyz * kS*/) * radiance * NdotL;

    }

    float3 ambient = float3(0.03f, 0.03f, 0.03f) * albedo * ao;
	float3 color = ambient + Lo;

	color = color / (color + float3(1.0f, 1.0f, 1.0f));
	color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

	return float4(color, 1.0f);
}
