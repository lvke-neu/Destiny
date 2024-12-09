#include "common.hlsl"

struct VertexIn
{
	float3 positionL : POSITION;
	float4 color	 : COLOR;
};


struct VertexOut
{
	float4 positionH : SV_POSITION;
	float4 color	 : COLOR;
};



VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = float4(vIn.positionL, 1.0f);
	vOut.color = vIn.color;
	return vOut;
}


float4 PS(VertexOut pIn) : SV_Target
{
	return pIn.color;
}

