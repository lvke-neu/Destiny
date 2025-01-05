#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class PlaneComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		PlaneComponent();
		virtual ~PlaneComponent() = default;
	public:
		void set_color(const DirectX::XMFLOAT4& color);
	};
}