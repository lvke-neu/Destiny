#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
}

struct VertexIn
{
	float3 positionL : POSITION;
	float3 normal	 : NORMAL;
	float2 texcoord  : TEXCOORD;
	matrix worldMatrix : WORLDMATRIX;
	float4 color	 : COLOR;
};


struct VertexOut
{
	float4 positionH : SV_POSITION;
	float3 normal	 : NORMAL;
	float2 texcoord  : TEXCOORD;
	float4 color	 : COLOR;
};



VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), vIn.worldMatrix), g_view), g_proj);
	vOut.normal = vIn.normal;
	vOut.texcoord = vIn.texcoord;
	vOut.color = vIn.color;
	//vOut.color = float4(vIn.worldMatrix[3][0],vIn.worldMatrix[3][0],vIn.worldMatrix[3][0],1.0);
	return vOut;
}


float4 PS(VertexOut pIn) : SV_Target
{
	return pIn.color;
}

