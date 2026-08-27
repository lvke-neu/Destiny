#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class SphereComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		SphereComponent();
		virtual ~SphereComponent() = default;
	};
}