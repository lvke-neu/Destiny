#include "Common.hlsli"

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.posL, 1.0f), g_World);  
    vOut.posW = vOut.posH.xyz;
    vOut.posH = mul(vOut.posH, g_View);               
    vOut.posH = mul(vOut.posH, g_Proj);               
    vOut.normalW = mul(vIn.normalL, (float3x3) g_InvTranspose);
    vOut.tex = vIn.tex;
    return vOut;
}
