#pragma once
#include <DirectXMath.h>

namespace Destiny
{
	struct Particle
	{
		DirectX::XMFLOAT3	position{ 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3	velocity{ 0.0f, 0.0f, 0.0f };
		float				age{ 0.0f };
	};
}