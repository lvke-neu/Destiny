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
};

struct VertexOut
{
    float4 positionH : SV_POSITION;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_view), g_proj);
    return vOut;
}


float4 PS(VertexOut pIn) : SV_Target
{
    return float4(1.0f, 0.0f, 0.0f, 1.0);
}
