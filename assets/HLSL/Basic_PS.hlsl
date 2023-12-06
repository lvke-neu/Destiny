#include "Common.hlsli"


float4 PS(VertexOut pIn) : SV_Target
{
	//float4 color = g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
	//color.a = 0.5f;

	float ambient = 0.2f;
	float4 ambientColor = ambient * g_ambientColor;

	float diffuse = 0.8 * dot(normalize(-g_directLightDirection.xyz), normalize(pIn.normalW));
	float4 diffuseColor = diffuse * g_diffuseColor;

	float3 v = reflect(g_directLightDirection.xyz, pIn.normalW);
	float specular = 0.5 * pow(max(dot(normalize(v), normalize(g_eyePos.xyz - pIn.positionW.xyz)), 0.0f), 32.0f);
	float4 specularColor = specular * g_specularColor;

	return ambientColor + diffuseColor + specularColor;
}
