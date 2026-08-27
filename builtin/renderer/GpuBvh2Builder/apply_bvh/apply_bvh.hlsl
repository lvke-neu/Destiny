#define HLSL
#include "../common/RayTracingHlslCompat.h"
#include "../common/RayTracingHelper.hlsli"

static const uint rootNodeIndex = 0;
static const int offsetToBoxes = SizeOfBVHOffsets;

cbuffer cbPerObject : register(b0)
{
    uint NumberOfElements;
};

cbuffer cbView : register(b1)
{
    matrix g_view;
}

cbuffer cbProj : register(b2)
{
    matrix g_proj;
}

RWByteAddressBuffer outputBVH : register(u0);
RWStructuredBuffer<HierarchyNode> hierarchyBuffer : register(u1);
RWStructuredBuffer<uint> bitonicSortIndicesBuffer : register(u2);
RWStructuredBuffer<uint> stack : register(u3);
AppendStructuredBuffer<uint> cullElement : register(u4);



// 提取视锥平面（基于 view * proj 矩阵，适用于以行向量 mul(pos, mat) 的写法）
// 返回 6 个 plane，格式为 float4 (nx, ny, nz, d)，平面方程：nx*x + ny*y + nz*z + d = 0
void ExtractFrustumPlanes(float4x4 view, float4x4 proj, out float4 planes[6])
{
    // 先构造 viewProj：先到 view 再到 proj（更直观）
    float4x4 VP = mul(view, proj); // 注意：这里假设你在 shader 里用 mul(pos, mat) 风格

    // 行访问：行0 = VP._m00.., 行1 = row1, 行2 = row2, row3 = row3
    // 对应标准提取方法（row-major，行向量）
    // left  = row3 + row0
    planes[0] = float4(VP._14 + VP._11, VP._24 + VP._21, VP._34 + VP._31, VP._44 + VP._41);
    // right = row3 - row0
    planes[1] = float4(VP._14 - VP._11, VP._24 - VP._21, VP._34 - VP._31, VP._44 - VP._41);
    // bottom = row3 + row1
    planes[2] = float4(VP._14 + VP._12, VP._24 + VP._22, VP._34 + VP._32, VP._44 + VP._42);
    // top = row3 - row1
    planes[3] = float4(VP._14 - VP._12, VP._24 - VP._22, VP._34 - VP._32, VP._44 - VP._42);
    // near = row3 + row2
    planes[4] = float4(VP._14 + VP._13, VP._24 + VP._23, VP._34 + VP._33, VP._44 + VP._43);
    // far = row3 - row2
    planes[5] = float4(VP._14 - VP._13, VP._24 - VP._23, VP._34 - VP._33, VP._44 - VP._43);

    // 归一化每个平面（把法线长度变为 1）
    [unroll]
    for (int i = 0; i < 6; ++i)
    {
        float3 n = planes[i].xyz;
        float len = length(n);
        // 如果 len 极小就跳过以避免除以 0（但正常情况下不会）
        if (len > 1e-6f)
        {
            planes[i] = planes[i] / len;
        }
    }
}

// 使用 plane-vs-AABB 测试（更鲁棒）
bool AABBFrustumIntersect_Planes(BoundingBox box, float4x4 view, float4x4 proj)
{
    float4 planes[6];
    ExtractFrustumPlanes(view, proj, planes);

    float3 c = box.center;
    float3 e = box.halfDim; // extents

    [unroll]
    for (int i = 0; i < 6; ++i)
    {
        float3 n = planes[i].xyz;
        float d = planes[i].w;

        // radius = sum(ext * abs(normal component))
        float r = e.x * abs(n.x) + e.y * abs(n.y) + e.z * abs(n.z);

        // distance from center to plane
        float dist = dot(n, c) + d;

        // 如果中心到平面的距离再减去盒子半径仍小于 0 => 全面在平面外
        if (dist + r < 0.0f)
            return false; // 被剔除
    }

    // 没有任何一个平面把盒子全剔除 => 相交或包含 => 可见
    return true;
}


void BVHNodeIntersect(uint rootNodeIndex)
{
    //uint2 flag;
    //BoundingBox box = GetBoxFromBuffer(outputBVH, offsetToBoxes, nodeIndex, flag);
    //if (AABBFrustumIntersect(box, g_view, g_proj))
    //{
    //    if (IsLeaf(flag))
    //    {
    //        cullElement.Append(nodeIndex);
    //    }
    //    else
    //    {
    //        uint leftNodeIndex = GetLeftNodeIndex(flag);
    //        uint rightNodeIndex = GetRightNodeIndex(flag);
    //        BVHNodeIntersect(leftNodeIndex);
    //        BVHNodeIntersect(rightNodeIndex);
    //    }
    //}

    int stackPtr = 0;

    stack[stackPtr++] = rootNodeIndex;

    while (stackPtr > 0)
    {
        uint nodeIndex = stack[--stackPtr];

        uint2 flag;
        BoundingBox box = GetBoxFromBuffer(outputBVH, offsetToBoxes, nodeIndex, flag);

        if (!AABBFrustumIntersect_Planes(box, g_view, g_proj))
            continue;

        if (IsLeaf(flag))
        {
            // 把叶子（物体 index）写入结果
            uint NumberOfInternalNodes = GetNumInternalNodes(NumberOfElements);
            uint bitonicSortIndex = nodeIndex - NumberOfInternalNodes;
            cullElement.Append(bitonicSortIndicesBuffer[bitonicSortIndex]);
        }
        else
        {
            uint left = GetLeftNodeIndex(flag);
            uint right = GetRightNodeIndex(flag);

            // 入栈
            stack[stackPtr++] = left;
            stack[stackPtr++] = right;
        }
    }
}

[numthreads(1, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    BVHNodeIntersect(dispatchThreadID.x);
}                                                                  