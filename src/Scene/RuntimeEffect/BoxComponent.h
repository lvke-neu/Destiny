#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color32.h"

namespace Destiny
{
	class BoxComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		BoxComponent();
		virtual ~BoxComponent() = default;
	};
}