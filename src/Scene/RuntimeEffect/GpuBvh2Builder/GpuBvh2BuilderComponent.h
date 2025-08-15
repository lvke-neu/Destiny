#pragma once
#include "Engine/Component.h"
#include "Graphics/GraphicsDefine.h"
#include "StructDefine.h"

namespace Destiny
{
	class SceneAABBCalculatorComponent;
	class MortonCodesCalculatorComponent;
	class BitonicSortComponent;
	class ConstructHierarchyComponent;
	class ConstructAABBPassComponent;
	class GpuBvh2BuilderComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		GpuBvh2BuilderComponent();
		virtual ~GpuBvh2BuilderComponent();
	public:
		GET(Button, test);
		void set_test(Button test);
	private:
		std::shared_ptr<SceneAABBCalculatorComponent> m_sceneAABBCalculatorComponent;
		std::shared_ptr<MortonCodesCalculatorComponent> m_mortonCodesCalculatorComponent;
		std::shared_ptr<BitonicSortComponent> m_bitonicSortComponent;
		std::shared_ptr<ConstructHierarchyComponent> m_constructHierarchyComponent;
		std::shared_ptr<ConstructAABBPassComponent> m_constructAABBPassComponent;

		Button m_test;

		std::vector<AABB> m_aabbs;
	};
}