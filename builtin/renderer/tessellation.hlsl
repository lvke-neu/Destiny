#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
    matrix u_worldMatrix;
    matrix u_worldInvTransposeMatrix;
    float  u_maxTessellation;     // 新增：最大细分级别
    float  u_minTessellation;     // 新增：最小细分级别
    float  u_tessellationRange;   // 新增：细分影响范围
    float  u_heightScale;         // 新增：高度缩放
}

struct VertexIn
{
    float3 positionL : POSITION;
    float3 normalL   : NORMAL;
    float2 texcoord  : TEXCOORD;
};

struct VertexOut
{
    float3 positionL : POSITION;
    float3 normalL   : NORMAL;
    float2 texcoord  : TEXCOORD;
};

typedef VertexOut HullOut;

struct DomainOut
{
    float3 positionL : POSITIONL;
    float4 positionH : SV_POSITION;
    float3 normalW   : NORMAL;
    float2 texcoord  : TEXCOORD;
};

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
    vOut.normalL = vIn.normalL;
    vOut.texcoord = vIn.texcoord;

    return vOut;
}

// 计算三角形面片到相机的距离
float CalculateDistanceToCamera(InputPatch<VertexOut, 3> patch)
{
    // 计算三角形中心的世界坐标
    float3 centerL = (patch[0].positionL + patch[1].positionL + patch[2].positionL) / 3.0f;
    float3 centerW = mul(float4(centerL, 1.0f), u_worldMatrix).xyz;

    // 计算到相机的距离
    return distance(centerW, g_eyePosition);
}

// 根据距离计算细分级别
float CalculateTessellationFactor(float distance)
{
    // 距离越近，细分级别越高
    float factor = u_maxTessellation - (distance / u_tessellationRange) * (u_maxTessellation - u_minTessellation);

    // 确保细分级别在有效范围内
    return clamp(factor, u_minTessellation, u_maxTessellation);
}

TriPatchTess TriConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
    TriPatchTess pt;

    // 计算面片到相机的距离
    float dist = CalculateDistanceToCamera(patch);

    // 计算动态细分因子
    float tessFactor = CalculateTessellationFactor(dist);

    // 设置所有边和内部的细分因子
    pt.edgeTess[0] = tessFactor;
    pt.edgeTess[1] = tessFactor;
    pt.edgeTess[2] = tessFactor;
    pt.InsideTess = tessFactor;

    return pt;
}

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("TriConstantHS")]
HullOut HS(InputPatch<VertexOut, 3> patch, uint i: SV_OutputControlPointID, uint patchId : SV_PrimitiveID) : POSITION
{
    HullOut hOut;
    hOut.positionL = patch[i].positionL;
    hOut.normalL = patch[i].normalL;
    hOut.texcoord = patch[i].texcoord;

    return hOut;
}

float GetHeight(float x, float z)
{
    return 0.3f * (z * sin(0.1f * x) + x * cos(0.1f * z));
}

[domain("tri")]
DomainOut DS(TriPatchTess patchTess, float3 weights : SV_DomainLocation, const OutputPatch<HullOut, 3> tri)
{
    // 重心坐标系插值
    float3 positionL = tri[0].positionL * weights[0] + tri[1].positionL * weights[1] + tri[2].positionL * weights[2];
    float3 normalL = tri[0].normalL * weights[0] + tri[1].normalL * weights[1] + tri[2].normalL * weights[2];
    float2 texcoord = tri[0].texcoord * weights[0] + tri[1].texcoord * weights[1] + tri[2].texcoord * weights[2];

    positionL.y = GetHeight(positionL.x, positionL.z) * u_heightScale;
    
    DomainOut dmOut;
    dmOut.positionL = positionL;
    dmOut.positionH = mul(mul(mul(float4(positionL, 1.0f), u_worldMatrix), g_view), g_proj);
    dmOut.normalW = mul(normalL, (float3x3) u_worldInvTransposeMatrix);
    dmOut.texcoord = texcoord;

    return dmOut;
}

Texture2D t_albedo : register(t0);
SamplerState s_sampler : register(s0);
float4 PS(DomainOut pIn) : SV_Target
{
    return t_albedo.Sample(s_sampler, pIn.texcoord);
}
