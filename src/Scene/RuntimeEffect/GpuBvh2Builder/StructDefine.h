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

    struct AABBNode
    {
        float    center[3];
        unsigned int    flags;
        float halfDim[3];
        unsigned int    rightNodeIndex;
    };

    std::vector<AABB> GenerateTrulyRandomAABBs(unsigned int boxCount);

#define		ElementsSummedPerThread 8
#define     THREAD_GROUP_WIDTH 8
#define     THREAD_GROUP_HEIGHT 8
#define     WAVE_SIZE (THREAD_GROUP_WIDTH * THREAD_GROUP_HEIGHT)
#define     THREAD_GROUP_1D_WIDTH WAVE_SIZE
}
