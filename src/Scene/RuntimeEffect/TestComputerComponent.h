#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class TestComputerComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TestComputerComponent();
		virtual ~TestComputerComponent() = default;
	};
}