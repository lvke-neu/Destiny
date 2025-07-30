#include "BitonicSortCommon.hlsli"

RWByteAddressBuffer g_SortBuffer : register(u0);
RWByteAddressBuffer g_IndexBuffer : register(u1);

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

    uint A = g_SortBuffer.Load(Index1 * 4);
    uint B = g_SortBuffer.Load(Index2 * 4);
    uint indexA = g_IndexBuffer.Load(Index1 * 4);
    uint indexB = g_IndexBuffer.Load(Index2 * 4);
    if (ShouldSwap(A, B, indexA, indexB))
    {
        g_SortBuffer.Store(Index1 * 4, B);
        g_SortBuffer.Store(Index2 * 4, A);


        g_IndexBuffer.Store(Index1 * 4, indexB);
        g_IndexBuffer.Store(Index2 * 4, indexA);
    }
}
