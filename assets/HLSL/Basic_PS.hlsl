#include "Common.hlsli"


float4 PS(VertexOut pIn) : SV_Target
{
	float4 color = g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
	//color.a = 0.5f;
	return float4(0,0,0,0);
}
