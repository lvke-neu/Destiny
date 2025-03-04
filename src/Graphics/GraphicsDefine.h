#pragma once
#include <DirectXMath.h>
#include "Math/Color.h"

namespace Destiny
{
	enum class RendererCategory
	{
		None,
		DeferredOpaque,
		ForwardOpaque,
		Transparent,
		Gui
	};

	class Button
	{

	};

	//Pay attention to memory alignment
	struct DirectionLight
	{
		Color lightColor;
		DirectX::XMFLOAT3 lightDirection;
		float lightIntensity;
	};

	struct PointLight
	{
		Color lightColor;
		DirectX::XMFLOAT3 lightPosition;
		float lightIntensity;
	};
}