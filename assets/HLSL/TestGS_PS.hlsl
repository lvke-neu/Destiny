#include "Common.hlsli"


float4 PS(VertexOut pIn) : SV_Target
{
	return g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
}
