#pragma once
#include "Engine/Component.h"
#include "Graphics/GraphicsDefine.h"
#include "StructDefine.h"

namespace Destiny
{
	class ConstructClusterComponent;
	class SceneAABBCalculatorComponent;
	class MortonCodesCalculatorComponent;
	class BitonicSortComponent;
	class ConstructHierarchyComponent;
	class ConstructAABBPassComponent;
	class ApplyBvhComponent;
	class ApplyBvhComponent2;
	class BoxVisualizationComponent;
	class GpuBvh2BuilderComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		GpuBvh2BuilderComponent();
		virtual ~GpuBvh2BuilderComponent();
	public:
		GET(Button, test);
		void set_test(Button test);
	public:
		virtual void onAddToNode() override;
		virtual void onLeaveScene() override;
	private:
		std::shared_ptr<ConstructClusterComponent> m_constructClusterComponent;
		std::shared_ptr<SceneAABBCalculatorComponent> m_sceneAABBCalculatorComponent;
		std::shared_ptr<MortonCodesCalculatorComponent> m_mortonCodesCalculatorComponent;
		std::shared_ptr<BitonicSortComponent> m_bitonicSortComponent;
		std::shared_ptr<ConstructHierarchyComponent> m_constructHierarchyComponent;
		std::shared_ptr<ConstructAABBPassComponent> m_constructAABBPassComponent;
		std::shared_ptr<ApplyBvhComponent> m_applyBvhComponent;
		std::shared_ptr<ApplyBvhComponent2> m_applyBvhComponent2;
		std::vector<std::shared_ptr<BoxVisualizationComponent>> m_boxVisualizationComponents;

		Button m_test;

		std::vector<AABB> m_aabbs;
	};
}