//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#define HLSL
#include "ConstructAABBBindings.h"
#include "../common/RayTracingHelper.hlsli"
#include "../common/EmulatedPointerIntrinsics.hlsli"

[numthreads(THREAD_GROUP_1D_WIDTH, 1, 1)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    uint instanceIndex = DTid.x;
    bool IsEmptyAccelerationStructure = NumberOfElements == 0;
    if (instanceIndex >= NumberOfElements ||
        (IsEmptyAccelerationStructure && DTid.x != 0)) // Special case for constructing empty AS
    {
        return;
    }

    const uint NumberOfInternalNodes = GetNumInternalNodes(NumberOfElements);
    const uint NumberOfAABBs = NumberOfInternalNodes + NumberOfElements;
    const uint offsetToBoxes = SizeOfBVHOffsets;
    const uint offsetToLeafNodes = GetOffsetToLeafNodeAABBs(NumberOfElements);
    const uint offsetToLeafNodeMetadata = offsetToLeafNodes + GetOffsetFromLeafNodesToBottomLevelMetadata(NumberOfElements);
    const uint totalSize = offsetToLeafNodeMetadata + NumberOfElements * SizeOfBVHMetadata;

    if (DTid.x == 0)
    {
        outputBVH.Store(OffsetToBoxesOffset, offsetToBoxes);
        outputBVH.Store(OffsetToLeafNodeMetaDataOffset, offsetToLeafNodeMetadata);
        outputBVH.Store(OffsetToTotalSize, totalSize);

        if (IsEmptyAccelerationStructure)
        {
            BoundingBox boxData;
            boxData.center = 0;
            boxData.halfDim = 0;
            uint2 flags = 0;
            WriteBoxToBuffer(outputBVH, offsetToBoxes, 0, boxData, flags);
            return;
        }
    }

    // Initialize argument for the AABB building phase, easier than uploading them
    // but probably not the best place for this logic
    {
        uint nodeIndex = NumberOfAABBs - DTid.x - 1;
        uint threadScratchAddress = DTid.x * SizeOfUINT32;
        scratchMemory.Store(threadScratchAddress, nodeIndex);

        // Initialize count buffer to 0
        if (DTid.x < NumberOfInternalNodes)
        {
            childNodesProcessedCounter.Store(threadScratchAddress, 0);
        }
    }
}
