#define HLSL
#include "../common/RaytracingHlslCompat.h"

uint GetFloatExponent(float f)
{
    uint u = asuint(f); // 把 float 当作 uint 位表示
    uint exp = (u >> 23) & 0xFF; // 取第 23~30 位（8-bit exponent）
    return exp;
}

float Mid(float t1, float t2, float t3)
{
    if (t1 < t2)
    {
        if (t3 < t1)
        {
            return t1;
        }
        else
        {
            return t3 < t2 ? t3 : t2;
        }
    }
    else
    {
        if (t3 < t2)
        {
            return t2;
        }
        else
        {
            return t3 < t1 ? t3 : t1;
        }
    }
}

float Max(float t1, float t2, float t3)
{
    if (t1 < t2)
    {
        return t2 < t3 ? t3 : t2;
    }
    else
    {
        return t1 < t3 ? t3 : t1;
    }
}

RWStructuredBuffer<AABB> AABBBuffer : register(u0);
RWStructuredBuffer<uint> ClusterCounts: register(u1);
RWStructuredBuffer<uint> ClusterOffsets: register(u2);
RWStructuredBuffer<uint> ClusterElements : register(u3);

[numthreads(1024, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint idx = dispatchThreadID.x;
    if (idx >= AABBBuffer.Length)
    {
        return;
    }

    AABB box = AABBBuffer[idx];

    float3 minPos = box.min;
    float3 maxPos = box.max;
    float3 visualSize = maxPos - minPos;
    float midVisualLength = Mid(visualSize.x, visualSize.y, visualSize.z);
    //float maxVisualLength = Max(visualSize.x, visualSize.y, visualSize.z);
    //float maxVisualArea = midVisualLength * maxVisualLength;

    uint groupID = GetFloatExponent(midVisualLength);

    // 在当前组内的索引
    uint localIdx;
    InterlockedAdd(ClusterCounts[groupID], 1, localIdx);

    // 全局索引 = 偏移 + 组内索引
    uint globalIdx = ClusterOffsets[groupID] + localIdx;

    // 写入元素 index
    ClusterElements[globalIdx] = idx;
}