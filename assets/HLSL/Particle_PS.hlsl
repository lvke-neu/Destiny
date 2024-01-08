#include "Common.hlsli"

float4 PS(ParticleVertexOut pIn) : SV_Target
{
	return pIn.color;
}
