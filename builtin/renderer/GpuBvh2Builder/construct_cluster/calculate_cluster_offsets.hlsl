#define HLSL
#include "../common/RaytracingHlslCompat.h"

RWStructuredBuffer<AABB> AABBBuffer : register(u0);
RWStructuredBuffer<uint> ClusterCounts : register(u1);
RWStructuredBuffer<uint> ClusterOffsets : register(u2);
AppendStructuredBuffer<uint> ClusterIndexes : register(u3);

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
}