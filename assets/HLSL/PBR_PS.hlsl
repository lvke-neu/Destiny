#include "Common.hlsli"

float4 PS(VertexOut pIn) : SV_Target
{
	//float4 color = g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
	//color.a = 0.5f;
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;

	float3 lightDir = normalize(g_pointLightPosition.xyz - pIn.positionW.xyz);
	float ambient = 1.0f;
	float diffuse = 1.0f * dot(lightDir, normalize(pIn.normalW));
	float3 v = reflect(-lightDir, pIn.normalW);
	float specular = 1.0f * pow(max(dot(normalize(v), normalize(g_eyePos.xyz - pIn.positionW.xyz)), 0.0f), 32.0f);
	
	if (g_useColor.x == 1.0f)
	{
		ambientColor = ambient * g_ambientColor;
		diffuseColor = diffuse * g_diffuseColor;
		specularColor = specular * g_specularColor;
	}
	else
	{
		ambientColor = ambient * g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
		diffuseColor = diffuse * g_diffuseTexture.Sample(g_diffuseSampler, pIn.texcoord);
		specularColor = specular * g_specularTexture.Sample(g_specularSampler, pIn.texcoord);
	}

	float shadow = 0.5f;
	float4 shadowMapPositionH = pIn.shadowMapPositionH;
	shadowMapPositionH.xyz /= shadowMapPositionH.w;
	shadowMapPositionH.xy = shadowMapPositionH.xy * float2(0.5, -0.5) + float2(0.5, 0.5);
	float depth = shadowMapPositionH.z;
	float bias = 0.005f;
	if (depth <= g_shadowMapTexture.Sample(g_shadowMapSampler, shadowMapPositionH.xy).r + bias)
	{
		shadow = 1.0f;
	}

	return shadow * (ambientColor + diffuseColor + specularColor);
}
