
cbuffer cbView : register(b0)
{
	matrix g_view;  
}

cbuffer cbProj : register(b1)
{
	matrix g_proj;
}

cbuffer cbWorld : register(b2)
{
	matrix g_world;
}

struct VertexIn
{
	float3 positionL : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};
struct VertexOut
{
	float4 positionH : SV_POSITION;
	float2 texcoord : TEXCOORD;
};
