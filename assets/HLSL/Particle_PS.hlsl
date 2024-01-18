#include "Common.hlsli"

float4 PS(ParticleVertexOut pIn) : SV_Target
{
	//return pIn.color;
	if (g_useColor.x == 1.0f)
	{
		return g_ambientColor;
	}

	return g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
}
