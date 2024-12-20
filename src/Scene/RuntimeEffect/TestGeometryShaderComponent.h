#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class TestGeometryShaderComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TestGeometryShaderComponent();
		virtual ~TestGeometryShaderComponent() = default;
	public:
		GET_CLASS_NAME(TestGeometryShaderComponent);
	};
}