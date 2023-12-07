#include "Common.hlsli"


float4 PS(VertexOut pIn) : SV_Target
{
	//float4 color = g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
	//color.a = 0.5f;
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;

	float ambient = 1.0f;
	float diffuse = 1.0f * dot(normalize(-g_directLightDirection.xyz), normalize(pIn.normalW));
	float3 v = reflect(g_directLightDirection.xyz, pIn.normalW);
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

	//ambientColor = ambient * g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord) * g_ambientColor;;
	//diffuseColor = diffuse * g_diffuseTexture.Sample(g_diffuseSampler, pIn.texcoord) * g_diffuseColor;
	//specularColor = specular * g_specularTexture.Sample(g_specularSampler, pIn.texcoord) * g_specularColor;
	return ambientColor + diffuseColor + specularColor;
}
