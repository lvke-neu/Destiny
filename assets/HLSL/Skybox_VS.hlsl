#include "Common.hlsli"

VertexOut VS(VertexIn vIn)
{
	float4 positionW = mul(float4(vIn.positionL, 1.0f), g_world);
	matrix view = g_view;
	view[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 positionV = mul(positionW, view);
	float4 positionH = mul(positionV, g_proj).xyww;
	VertexOut vOut;
	vOut.positionL = vIn.positionL;
	vOut.positionH = positionH;
	
    return vOut;
}
