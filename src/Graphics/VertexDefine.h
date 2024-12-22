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

	struct Position
	{
		DirectX::XMFLOAT3 position;
	};
}