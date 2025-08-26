#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class TestComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TestComponent();
		virtual ~TestComponent() = default;
	};
}