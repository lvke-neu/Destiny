#include "Common.hlsli"

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = mul (mul(mul(float4(vIn.positionL, 1.0f), g_world), g_view), g_proj);
    return vOut;
}
