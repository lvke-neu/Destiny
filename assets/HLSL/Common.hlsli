
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

cbuffer cbMaterialColor : register(b3)
{
	float4 g_ambientColor;
	float4 g_diffuseColor;
	float4 g_specularColor;
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


Texture2D g_ambientTexture : register(t0);
SamplerState g_ambientSampler : register(s0);
Texture2D g_diffuseTexture : register(t1);
SamplerState g_diffuseSampler : register(s1);
Texture2D g_specularTexture : register(t2);
SamplerState g_specularSampler : register(s2);