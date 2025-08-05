#pragma once
#include "Engine/Component.h"
#include "StructDefine.h"

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class GraphicsCommandList;
	class ConstructAABBPassComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		ConstructAABBPassComponent();
		virtual ~ConstructAABBPassComponent() = default;
	public:
		void init(const std::vector<AABB>& aabbs, std::shared_ptr<Texture> hierarchyBuffer, std::shared_ptr<Texture> bitonicSortIndicesBuffer);
	public:
		void prepareForComputeAABBsCS();
		void computeAABBsCS();
		void bindResource(const std::vector<AABB>& aabbs);
	private:
		std::shared_ptr<ComputerCommand> m_prepareForComputeAABBs;
		std::shared_ptr<ComputerCommand> m_computeAABBs;

		unsigned int m_numElements;
		std::shared_ptr<Texture> m_outputBVH;
		std::shared_ptr<Texture> m_scratchMemory;
		std::shared_ptr<Texture> m_childNodesProcessedCounter;
		std::shared_ptr<Texture> m_hierarchyBuffer;
		std::shared_ptr<Texture> m_aabbParentBuffer;

		std::shared_ptr<Texture> m_boundingBoxBuffer;
		std::shared_ptr<Texture> m_bitonicSortIndicesBuffer;

		std::shared_ptr<GraphicsCommandList> m_graphicsCommandList;
	};
}