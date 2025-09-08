#define HLSL
#include "../common/RaytracingHlslCompat.h"

AABB MergeAABB(AABB a, AABB b)
{
    AABB result;
    result.min = min(a.min, b.min); // 内建 min 支持 float3
    result.max = max(a.max, b.max); // 内建 max 支持 float3
    return result;
}

struct Cluster64
{
    uint clusterIndex;
    uint clusterStart;
    uint clusterCount;
    AABB aabb;
};

RWStructuredBuffer<AABB> AABBBuffer : register(u0);
RWStructuredBuffer<uint> ClusterCounts: register(u1);
RWStructuredBuffer<uint> ClusterOffsets: register(u2);
RWStructuredBuffer<uint> ClusterIndexes : register(u3);
RWStructuredBuffer<uint> ClusterElements : register(u4);
AppendStructuredBuffer<Cluster64> ClustersSmallers : register(u5);

[numthreads(1, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint clusterIndex = ClusterIndexes[dispatchThreadID.x];
    uint clusterCount = ClusterCounts[clusterIndex];
    uint clusterOffset = ClusterOffsets[clusterIndex];
    if (clusterCount == 0)
    {
        return;
    }

    const uint clusterSize = 64;
    uint clusterSmallerCount = (clusterCount + clusterSize - 1) / clusterSize;

    for (uint c = 0; c < clusterSmallerCount; c++)
    {
        Cluster64 cluster64;
        cluster64.clusterIndex = clusterIndex;


        uint start = c * clusterSize;
        uint count = min(clusterSize, clusterCount - start);

        cluster64.clusterStart = start;
        cluster64.clusterCount = count;

        AABB tmpAABB;
        tmpAABB.min = float3(3.402823466e+38f, 3.402823466e+38f, 3.402823466e+38f);
        tmpAABB.max = float3(-3.402823466e+38f, -3.402823466e+38f, -3.402823466e+38f);
        for (uint i = start; i < start + count; i++)
        {
            uint elementIndex = ClusterElements[clusterOffset + i];
            tmpAABB = MergeAABB(tmpAABB, AABBBuffer[elementIndex]);
        }

        cluster64.aabb = tmpAABB;

        ClustersSmallers.Append(cluster64);
    }
}