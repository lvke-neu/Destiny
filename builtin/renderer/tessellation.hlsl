#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
    matrix u_worldMatrix;
    matrix u_worldInvTransposeMatrix;
    uint  u_triEdgeTess0;
    uint  u_triEdgeTess1;
    uint  u_triEdgeTess2;
    uint  u_triInsideTess;
}

struct VertexIn
{
    float3 positionL : POSITION;
};

struct VertexOut
{
    float3 positionL : POSITION;
};

typedef VertexOut HullOut;

struct TriPatchTess
{
    float edgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;

    // 可以在下面为每个面片附加所需的额外信息
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.positionL = vIn.positionL;

    return vOut;
}

TriPatchTess TriConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
    TriPatchTess pt;

    pt.edgeTess[0] = u_triEdgeTess0;
    pt.edgeTess[1] = u_triEdgeTess1;
    pt.edgeTess[2] = u_triEdgeTess2;
    pt.InsideTess = u_triInsideTess;

    return pt;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("TriConstantHS")]
float3 HS(InputPatch<VertexOut, 3> patch, uint i: SV_OutputControlPointID, uint patchId : SV_PrimitiveID) : POSITION
{
    return patch[i].positionL;
}

[domain("tri")]
float4 DS(TriPatchTess patchTess, float3 weights : SV_DomainLocation, const OutputPatch<HullOut, 3> tri) : SV_POSITION
{
    // 重心坐标系插值
    float3 pos = tri[0].positionL * weights[0] +
        tri[1].positionL * weights[1] +
        tri[2].positionL * weights[2];

    return mul(mul(mul(float4(pos, 1.0f), u_worldMatrix), g_view), g_proj);
}

float4 PS(VertexOut pIn) : SV_Target
{
   return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
