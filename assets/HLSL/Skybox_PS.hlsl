#include "Common.hlsli"

TextureCube g_cubeTexture : register(t0);
SamplerState g_cubeSampler : register(s0);

float4 PS(VertexOut pIn) : SV_Target
{
	return g_cubeTexture.Sample(g_cubeSampler, pIn.positionL);
	//return float4(1,1,0,1);
}
