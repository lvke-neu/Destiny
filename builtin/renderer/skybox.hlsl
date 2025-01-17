#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
}

struct VertexIn
{
	float3 positionL : POSITION;
};

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float3 positionL : POSITIONL;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	float4x4 view = g_view;
	view[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), view), g_proj).xyww;
	vOut.positionL = vIn.positionL;

	return vOut;
}

TextureCube t_cube : register(t0);
SamplerState s_sampler : register(s0);
float4 PS(VertexOut pIn) : SV_Target
{
	return t_cube.Sample(s_sampler, pIn.positionL);
}

