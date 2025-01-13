#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
	float2 c_size;
    float4 c_color;
}

struct VertexIn
{
	float3 positionL : POSITION;
};

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float2 texcoord : TEXCOORD;
};



VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_view), g_proj);
	return vOut;
}

[maxvertexcount(4)]
void GS(point VertexOut input[1], inout TriangleStream<VertexOut> output)
{
	float4 position = input[0].positionH;

	VertexOut vertex;

	vertex.positionH = position;
	vertex.positionH.x -= c_size.x * g_rcpViewportWidth * position.w;
	vertex.positionH.y -= c_size.y * g_rcpViewportHeight * position.w;
	vertex.texcoord = float2(0.0f, 1.0f);
	output.Append(vertex);

	vertex.positionH = position;
	vertex.positionH.x -= c_size.x * g_rcpViewportWidth * position.w;
	vertex.positionH.y += c_size.y * g_rcpViewportHeight * position.w;
	vertex.texcoord = float2(0.0f, 0.0f);
	output.Append(vertex);

	vertex.positionH = position;
	vertex.positionH.x += c_size.x * g_rcpViewportWidth * position.w;
	vertex.positionH.y -= c_size.y * g_rcpViewportHeight * position.w;
	vertex.texcoord = float2(1.0f, 1.0f);
	output.Append(vertex);

	vertex.positionH = position;
	vertex.positionH.x += c_size.x * g_rcpViewportWidth * position.w;
	vertex.positionH.y += c_size.y * g_rcpViewportHeight * position.w;
	vertex.texcoord = float2(1.0f, 0.0f);
	output.Append(vertex);

	output.RestartStrip();
}

Texture2D	 t_texture : register(t0);
SamplerState s_sampler : register(s0);
float4 PS(VertexOut pIn) : SV_Target
{
    return t_texture.Sample(s_sampler, pIn.texcoord) * c_color;
}

