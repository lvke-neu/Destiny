#define HLSL
#include "../common/RaytracingHlslCompat.h"

struct Cluster64
{
    uint clusterIndex;
    uint clusterStart;
    uint clusterCount;
    AABB aabb;
};

RWStructuredBuffer<AABB> AABBBuffer : register(u0);
RWStructuredBuffer<uint> ClusterCounts : register(u1);
RWStructuredBuffer<uint> ClusterOffsets : register(u2);
AppendStructuredBuffer<uint> ClusterIndexes : register(u3);
AppendStructuredBuffer<Cluster64> ClustersSmallers : register(u4);

[numthreads(1024, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint idx = dispatchThreadID.x;

    if (idx >= ClusterCounts.Length)
    {
        return;
    }

    if (ClusterCounts[idx] == 0)
    {
        return;
    }

    if (idx == 0)
    {
        ClusterOffsets[0] = 0;
        return;
    }
    
    uint res = 0;
    for (uint i = 0; i < idx; i++)
    {
        res += ClusterCounts[i];
    }

    ClusterOffsets[idx] = res;
    ClusterIndexes.Append(idx);








    uint clusterIndex = idx;
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

        cluster64.aabb = tmpAABB;

        ClustersSmallers.Append(cluster64);
    }
}