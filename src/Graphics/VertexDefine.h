#pragma once
#include <DirectXMath.h>

namespace Destiny
{
	struct PositionNormalTexcoord
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};

	struct PositionColor
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	struct PositionTexcoord
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoord;
	};

	struct Position3
	{
		DirectX::XMFLOAT3 position;
	};

	struct Position2
	{
		DirectX::XMFLOAT2 position;
	};
}