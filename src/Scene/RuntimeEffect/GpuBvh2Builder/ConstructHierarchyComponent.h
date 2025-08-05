#pragma once
#include "Engine/Component.h"
#include "StructDefine.h"

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class GraphicsCommandList;
	class ConstructHierarchyComponent : public Component
	{
		friend class GpuBvh2BuilderComponent;
		RTTR_ENABLE(Component);
	public:
		ConstructHierarchyComponent();
		virtual ~ConstructHierarchyComponent() = default;
	public:
		void init(unsigned int numElements, std::shared_ptr<Texture> outputMortonCodesBuffer);
	public:
		void buildSplitsCS();
	private:
		unsigned int m_numElements;
		std::shared_ptr<Texture> m_outputMortonCodesBuffer;

		std::shared_ptr<Texture> m_hierarchyBuffer;
		std::shared_ptr<ComputerCommand> m_buildSplits;
		std::shared_ptr<GraphicsCommandList> m_graphicsCommandList;
	};
}