
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
	matrix g_InvTranspose;
}
struct VertexIn
{
	float3 posL : POSITION;
	float3 normalL : NORMAL;
	float2 tex : TEXCOORD;
};
struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;     
	float3 normalW : NORMAL;
	float2 tex : TEXCOORD;
};
