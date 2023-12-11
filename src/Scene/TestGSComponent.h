#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class TestGSComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		TestGSComponent();
	public:
		GET_CLASS_NAME(TestGSComponent);
	};
}