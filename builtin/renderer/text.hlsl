#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	float2 c_size;
    float2 c_screenPosition;
    float4 c_color;
}

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

VertexOut VS()
{
	VertexOut vOut;
    vOut.positionH = float4(c_screenPosition, 0.0f, 1.0f);
	return vOut;
}

[maxvertexcount(4)]
void GS(point VertexOut input[1], inout TriangleStream<VertexOut> output)
{
	float4 position = input[0].positionH;

	VertexOut vertex;

	vertex.positionH = position;
    vertex.positionH.x -= c_size.x * g_rcpViewportWidth;
    vertex.positionH.y -= c_size.y * g_rcpViewportHeight;
	vertex.texcoord = float2(0.0f, 1.0f);
	output.Append(vertex);

	vertex.positionH = position;
    vertex.positionH.x -= c_size.x * g_rcpViewportWidth;
    vertex.positionH.y += c_size.y * g_rcpViewportHeight;
	vertex.texcoord = float2(0.0f, 0.0f);
	output.Append(vertex);

	vertex.positionH = position;
    vertex.positionH.x += c_size.x * g_rcpViewportWidth;
    vertex.positionH.y -= c_size.y * g_rcpViewportHeight;
	vertex.texcoord = float2(1.0f, 1.0f);
	output.Append(vertex);

	vertex.positionH = position;
    vertex.positionH.x += c_size.x * g_rcpViewportWidth;
    vertex.positionH.y += c_size.y * g_rcpViewportHeight;
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

