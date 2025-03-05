#include "common.hlsl"

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float2 texcoord	 : TEXCOORD;
};

struct PixelOut
{
	float4 color : SV_TARGET;
	float depth : SV_Depth;
};

VertexOut VS(uint vertexID : SV_VertexID)
{
	VertexOut vOut;
	float2 grid = float2((vertexID << 1) & 2, vertexID & 2);
	float2 xy = grid * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
	vOut.positionH = float4(xy, 1.0f, 1.0f);
	vOut.texcoord.x = (xy.x + 1.0f) * 0.5f;
	vOut.texcoord.y = (1.0f - xy.y) * 0.5f;
	return vOut;
}

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_mra : register(t2);
Texture2D t_positionW :  register(t3);
Texture2D t_normalW :  register(t4);
Texture2D t_texcoord :  register(t5);
SamplerState s_sampler : register(s0);

PixelOut PS(VertexOut pIn)
{
	float3 albedo = pow(t_albedo.Sample(s_sampler, pIn.texcoord).xyz, float3(2.2f, 2.2f, 2.2f));
	float4 mra = t_mra.Sample(s_sampler, pIn.texcoord);
	float metallic = mra.x;
	float roughness = mra.y;
	float ao = mra.z;

	//getNormalFromMap---------
	float4 positionW = t_positionW.Sample(s_sampler, pIn.texcoord);
	float3 normalW = t_normalW.Sample(s_sampler, pIn.texcoord).xyz;
	float4 texcoordSample = t_texcoord.Sample(s_sampler, pIn.texcoord);
	float2 texcoord = texcoordSample.xy;
	float depth = texcoordSample.z;

	float3 tangentNormal = t_normal.Sample(s_sampler, pIn.texcoord).xyz * 2.0f - 1.0f;
	float3 Q1 = ddx(positionW.xyz);
	float3 Q2 = ddy(positionW.xyz);
	float2 st1 = ddx(texcoord);
	float2 st2 = ddy(texcoord);

	float3 N_ = normalize(normalW);
	float3 T = normalize(Q1 * st2.y - Q2 * st1.y);
	float3 B = -normalize(cross(N_, T));

	float3x3 TBN = float3x3(T, B, N_);
	//-------------------------

	float3 N = normalize(mul(tangentNormal, TBN));
	float3 V = normalize(g_eyePosition.xyz - positionW.xyz);

	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	F0 = lerp(F0, albedo, metallic);

	float3 Lo = float3(0.0f, 0.0f, 0.0f);

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
		Lo += (kD * albedo / PI + specular /*+ t_environment.Sample(s_sampler, reflect(-V, N_)).xyz * kS*/) * radiance * NdotL;

	}
	for (int j = 0; j < g_pointLightCount; j++)
	{
		float3 L = normalize(g_pointLights[j].lightPosition - positionW.xyz);
		float3 H = normalize(V + L);

		float distance = length(g_pointLights[j].lightPosition - positionW.xyz);
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

	PixelOut pOut;
	pOut.color = float4(color, 1.0f);
	pOut.depth = depth;

	return pOut;
}

