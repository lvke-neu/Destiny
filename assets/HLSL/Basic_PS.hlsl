#include "Common.hlsli"

Texture2D g_texture2D : register(t0);
SamplerState g_sampler : register(s0);

float4 PS(VertexOut pIn) : SV_Target
{
	return g_texture2D.Sample(g_sampler, pIn.texcoord);;
}
