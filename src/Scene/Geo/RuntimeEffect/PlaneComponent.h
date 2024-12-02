#pragma once
#include "Scene/NCS/VisualComponent.h"

namespace Destiny
{
	class PlaneComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		PlaneComponent();
		virtual ~PlaneComponent() = default;
	public:
		GET_CLASS_NAME(PlaneComponent);
	};
}