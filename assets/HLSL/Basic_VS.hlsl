#include "Common.hlsli"

VertexOut VS(VertexIn vIn)
{
	float4 positionW = mul(float4(vIn.positionL, 1.0f), g_world);
	float4 positionV = mul(positionW, g_view);
	float4 positionH = mul(positionV, g_proj);
	VertexOut vOut;
	vOut.positionH = positionH;
	vOut.positionW = positionW;
	vOut.normalW = mul(vIn.normalL, (float3x3)g_worldInvTranspose);
	vOut.texcoord = vIn.texcoord;
	vOut.shadowMapPositionH = mul(mul(positionW, g_directLightView), g_directLightProj);
    return vOut;
}
