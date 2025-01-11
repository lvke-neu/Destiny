#pragma once
#include <DirectXMath.h>
#include "Math/Color32.h"

namespace Destiny
{
	enum class RendererCategory
	{
		None,
		ForwardOpaque,
		ForwardTransparent,
		Gui
	};

	class Button
	{

	};

	//Pay attention to memory alignment
	struct DirectionLight
	{
		Color32 lightColor;
		DirectX::XMFLOAT3 lightDirection;
		float lightIntensity;
	};
}