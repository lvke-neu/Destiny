
cbuffer ConstantBuffer : register(b0)
{
	matrix g_View;  
}

cbuffer ConstantBuffer : register(b1)
{
	matrix g_Proj;
}

cbuffer ConstantBuffer : register(b2)
{
	matrix g_World;
}
struct VertexIn
{
	float3 posL : POSITION;
	float4 color : COLOR;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};
