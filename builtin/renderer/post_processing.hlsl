#include "common.hlsl"

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float2 texcoord	 : TEXCOORD;
};


VertexOut VS(uint vertexID : SV_VertexID)
{
	VertexOut vOut;
	float2 grid = float2((vertexID << 1) & 2, vertexID & 2);
	float2 xy = grid * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
	vOut.positionH = float4(xy, 1.0f, 1.0f);
	vOut.texcoord.x = (xy.x + 1.0f) * 0.5f;
	vOut.texcoord.y = (1.0f - xy.y) * 0.5f;
	return vOut;
}

Texture2D t_renderedTexture : register(t0);
SamplerState s_sampler : register(s0);
float4 PS(VertexOut pIn) : SV_TARGET
{
	return t_renderedTexture.Sample(s_sampler, pIn.texcoord);
	//float4 color = t_renderedTexture.Sample(s_sampler, pIn.texcoord);
	//float gray = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
	//float4 grayscaleColor = float4(gray, gray, gray, color.a);

	//return grayscaleColor;
}



Texture2D t_texture : register(t0);
RWTexture2D<unorm float4> u_texture : register(u0);

//SV_DispatchThreadID 
//SV_GroupID
//SV_GroupThreadID

[numthreads(16, 16, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	u_texture[dispatchThreadID.xy] = t_texture[dispatchThreadID.xy];
}
