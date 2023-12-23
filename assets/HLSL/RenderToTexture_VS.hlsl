#include "Common.hlsli"

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = float4(vIn.positionL, 1.0f);
	vOut.texcoord = vIn.texcoord;
    return vOut;
}
