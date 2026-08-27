#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class TestComputerComponent2 : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TestComputerComponent2();
		virtual ~TestComputerComponent2() = default;
	};
}