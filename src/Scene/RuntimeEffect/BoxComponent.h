#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class BoxComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		BoxComponent();
		virtual ~BoxComponent() = default;
	public:
		GET_CLASS_NAME(BoxComponent);
	};
}