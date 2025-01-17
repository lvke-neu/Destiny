#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class SkyboxComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		SkyboxComponent();
		virtual ~SkyboxComponent() = default;
	};
}