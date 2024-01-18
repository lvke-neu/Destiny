#include "Common.hlsli"

ParticleVertexOut VS(ParticleVertexIn vIn)
{
	float4 positionW = mul(float4(vIn.positionL, 1.0f), g_world);
	float4 positionV = mul(positionW, g_view);
	float4 positionH = mul(positionV, g_proj);
	ParticleVertexOut vOut;
	vOut.positionH = positionH;
	vOut.texcoord = vIn.texcoord;

    return vOut;
}
