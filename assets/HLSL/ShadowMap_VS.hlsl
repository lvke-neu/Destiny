#include "Common.hlsli"

VertexOut VS(VertexIn vIn)
{
	float4 positionW = mul(float4(vIn.positionL, 1.0f), g_world);
	float4 positionV = mul(positionW, g_directLightView);
	float4 positionH = mul(positionV, g_directLightProj);
	VertexOut vOut;
	vOut.positionH = positionH;
    return vOut;
}
