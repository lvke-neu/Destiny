#pragma once
#include "Component.h"

namespace Destiny
{
	class DirectionLightComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		DirectionLightComponent();
		~DirectionLightComponent();
	public:
		GET_CLASS_NAME(DirectionLightComponent);
	private:
		DirectX::XMFLOAT3 m_direction;
		DirectX::XMFLOAT3 m_color;
	};
}