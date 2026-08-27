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
SamplerState s_sampler : register(s0);
float4 PS(VertexOut pIn) : SV_Target
{
   return t_albedo.Sample(s_sampler, pIn.texcoord);
}
