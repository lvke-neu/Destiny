#pragma once
#include "Engine/Component.h"
#include "Graphics/GraphicsDefine.h"
#include "StructDefine.h"

namespace Destiny
{
	class ConstructClusterComponent;
	class BoxVisualizationComponent;
	class ApplyBvhComponent3;
	class GpuBvh2BuilderComponent2 : public Component
	{
		RTTR_ENABLE(Component);
	public:
		GpuBvh2BuilderComponent2();
		virtual ~GpuBvh2BuilderComponent2();
	public:
		virtual void onAddToNode() override;
	private:
		std::vector<AABB> m_aabbs;

		std::shared_ptr<ConstructClusterComponent> m_constructClusterComponent;
		std::shared_ptr<ApplyBvhComponent3> m_applyBvhComponent3;
	
		std::vector<std::shared_ptr<BoxVisualizationComponent>> m_boxVisualizationComponents;
	};
}