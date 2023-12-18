#include "Common.hlsli"

TextureCube g_cubeTexture : register(t3);
SamplerState g_cubeSampler : register(s3);

float4 PS(VertexOut pIn) : SV_Target
{
	return g_cubeTexture.Sample(g_cubeSampler, pIn.positionL);
	//return float4(1,1,0,1);
}
