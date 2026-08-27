#include "BitonicSortCommon.hlsli"

RWStructuredBuffer<uint> g_SortBuffer : register(u0);
RWStructuredBuffer<uint> g_IndexBuffer : register(u1);

cbuffer Constants : register(b0)
{
    uint k;    // k >= 4096
    uint j;    // j >= 2048 && j < k
};

[numthreads(1024, 1, 1)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    // Form unique index pair from dispatch thread ID
    uint Index2 = InsertOneBit(DTid.x, j);
    uint Index1 = Index2 ^ (k == 2 * j ? k - 1 : j);

    if (Index2 >= ListCount)
        return;

    uint A = g_SortBuffer[Index1];
    uint B = g_SortBuffer[Index2];
    uint indexA = g_IndexBuffer[Index1];
    uint indexB = g_IndexBuffer[Index2];
    if (ShouldSwap(A, B, indexA, indexB))
    {
        g_SortBuffer[Index1] = B;
        g_SortBuffer[Index2] = A;

        g_IndexBuffer[Index1] = indexB;
        g_IndexBuffer[Index2] = indexA;
    }
}
