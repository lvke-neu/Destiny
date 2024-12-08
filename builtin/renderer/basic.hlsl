#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
	float4 u_color;
}

struct VertexIn
{
	float3 positionL : POSITION;
	float3 normal	 : NORMAL;
	float2 texcoord  : TEXCOORD;
};


struct VertexOut
{
	float4 positionH : SV_POSITION;
	float3 normal	 : NORMAL;
	float2 texcoord  : TEXCOORD;
};



VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_view), g_proj);
	vOut.normal = vIn.normal;
	vOut.texcoord = vIn.texcoord;
	return vOut;
}


float4 PS(VertexOut pIn) : SV_Target
{
	return u_color;
}

