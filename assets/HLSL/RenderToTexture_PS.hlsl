#include "Common.hlsli"


float4 PS(VertexOut pIn) : SV_Target
{
	float4 tmp = g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
	return tmp;
	//return float4(1,1,1,1);
}
