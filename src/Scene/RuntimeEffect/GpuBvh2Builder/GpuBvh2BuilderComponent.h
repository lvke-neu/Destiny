#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class SceneAABBCalculatorComponent;
	class MortonCodesCalculatorComponent;
	class BitonicSortComponent;
	class GpuBvh2BuilderComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		GpuBvh2BuilderComponent();
		virtual ~GpuBvh2BuilderComponent() = default;
	private:
		std::shared_ptr<SceneAABBCalculatorComponent> m_sceneAABBCalculatorComponent;
		std::shared_ptr<MortonCodesCalculatorComponent> m_mortonCodesCalculatorComponent;
		std::shared_ptr<BitonicSortComponent> m_bitonicSortComponent;
	};
}