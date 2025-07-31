#pragma once
#include <DirectXMath.h>

namespace Destiny
{
	struct AABB
	{
		DirectX::XMFLOAT3 min = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 max = { 0.0f, 0.0f, 0.0f };
	};

#define		ElementsSummedPerThread 8
#define     THREAD_GROUP_WIDTH 8
#define     THREAD_GROUP_HEIGHT 8
#define     WAVE_SIZE (THREAD_GROUP_WIDTH * THREAD_GROUP_HEIGHT)
#define     THREAD_GROUP_1D_WIDTH WAVE_SIZE
}
