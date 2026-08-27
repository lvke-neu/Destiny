#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class PlaneComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		PlaneComponent();
		virtual ~PlaneComponent() = default;
	};
}