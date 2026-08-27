#pragma once
#include <DirectXMath.h>
#include <vector>

namespace Destiny
{
	struct AABB
	{
		DirectX::XMFLOAT3 min = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 max = { 0.0f, 0.0f, 0.0f };
	};

    struct BoundingBox
    {
        DirectX::XMFLOAT3 center;
        DirectX::XMFLOAT3 halfDim;
    };

    struct AABBNode
    {
        float    center[3];
        unsigned int    flags;
        float halfDim[3];
        unsigned int    rightNodeIndex;
    };

    std::vector<AABB> GenerateTrulyRandomAABBs(unsigned int boxCount);

    struct BVHOffsets
    {
         unsigned int    offsetToBoxes;
         unsigned int    offsetToVertices;
         unsigned int    offsetToPrimitiveMetaData;
         unsigned int    totalSize;
    };


    struct EMULATED_GPU_POINTER
    {
        unsigned int OffsetInBytes;
        unsigned int DescriptorHeapIndex;
    };

    struct WRAPPED_GPU_POINTER
    {
        union
        {
            EMULATED_GPU_POINTER EmulatedGpuPtr;
            unsigned __int64 GpuVA;
        };

        WRAPPED_GPU_POINTER operator+(unsigned __int64 offset)
        {
            WRAPPED_GPU_POINTER pointer = *this;
            pointer.GpuVA += offset;
            return pointer;
        }
    };

    typedef struct RAYTRACING_FALLBACK_INSTANCE_DESC
    {
        float Transform[3][4];
        unsigned int InstanceID : 24;
        unsigned int InstanceMask : 8;
        unsigned int InstanceContributionToHitGroupIndex : 24;
        unsigned int Flags : 8;
        WRAPPED_GPU_POINTER AccelerationStructure;
    }     RAYTRACING_FALLBACK_INSTANCE_DESC;

    struct RaytracingInstanceDesc
    {
        DirectX::XMFLOAT4 Transform[3];
        unsigned int InstanceIDAndMask;
        unsigned int InstanceContributionToHitGroupIndexAndFlags;
        unsigned __int64 AccelerationStructure;
    };

    struct BVHMetadata
    {
        RaytracingInstanceDesc instanceDesc;
        DirectX::XMFLOAT4 ObjectToWorld[3];
        unsigned int InstanceIndex;
    };

    struct ScratchMemoryPartitions
    {
        unsigned __int64 OffsetToSceneAABB;
        unsigned __int64 OffsetToElements;
        unsigned __int64 OffsetToMortonCodes;
        unsigned __int64 OffsetToIndexBuffer;
        unsigned __int64 OffsetToHierarchy;
        unsigned __int64 OffsetToBaseTreeletsCount;

        unsigned __int64 OffsetToSceneAABBScratchMemory;

        unsigned __int64 OffsetToCalculateAABBDispatchArgs;
        unsigned __int64 OffsetToPerNodeCounter;
        unsigned __int64 TotalSize;
    };

    struct Primitive
    {
        unsigned int PrimitiveType;
        DirectX::XMUINT4 data0;
        DirectX::XMUINT4 data1;
        unsigned int data2;
    };

    struct PrimitiveMetaData
    {
         unsigned int GeometryContributionToHitGroupIndex;
         unsigned int PrimitiveIndex;
         unsigned int GeometryFlags;
    };

    struct HierarchyNode
    {
        unsigned int ParentIndex;
        unsigned int LeftChildIndex;
        unsigned int RightChildIndex;

        static const int IsCollapseChildren = 0x80000000; // for extracting HierarchyNode::bCollapseChildren
    };

#define		ElementsSummedPerThread 8
#define     THREAD_GROUP_WIDTH 8
#define     THREAD_GROUP_HEIGHT 8
#define     WAVE_SIZE (THREAD_GROUP_WIDTH * THREAD_GROUP_HEIGHT)
#define     THREAD_GROUP_1D_WIDTH WAVE_SIZE
}
