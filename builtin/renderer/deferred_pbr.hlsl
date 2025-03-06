#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
    matrix u_worldInvTransposeMatrix;
}

struct VertexIn
{
	float3 positionL : POSITION;
	float3 normalL	 : NORMAL;
	float2 texcoord  : TEXCOORD;
};

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float4 positionW : POSITION;
    float3 normalW   : NORMAL;
	float2 texcoord  : TEXCOORD;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_view), g_proj);
    vOut.positionW = mul(float4(vIn.positionL, 1.0f), u_worldMatrix);
    vOut.normalW = mul(vIn.normalL, (float3x3) u_worldInvTransposeMatrix);
	vOut.texcoord = vIn.texcoord;
   
	return vOut;
}

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ao : register(t4);
SamplerState s_sampler : register(s0);

GBuffer PS(VertexOut pIn)
{
	GBuffer gBuffer;
	gBuffer.albedo = t_albedo.Sample(s_sampler, pIn.texcoord);
	gBuffer.normal = t_normal.Sample(s_sampler, pIn.texcoord);
	float metallic = t_metallic.Sample(s_sampler, pIn.texcoord).x;
	float roughness = t_roughness.Sample(s_sampler, pIn.texcoord).x;
	float ao = t_ao.Sample(s_sampler, pIn.texcoord).x;
	gBuffer.mra = float4(metallic, roughness, ao, 1.0f);
	gBuffer.positionW = pIn.positionW;
	gBuffer.normalW = float4(pIn.normalW, 1.0f);
	gBuffer.texcoord = float4(pIn.texcoord, pIn.positionH.z, 1.0f);

	return gBuffer;
}
