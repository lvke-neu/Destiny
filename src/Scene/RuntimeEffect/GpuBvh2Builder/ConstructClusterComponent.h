#pragma once
#include "Engine/Component.h"
#include "StructDefine.h"

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class GraphicsCommandList;
	class ClearUnorderedAccessView;
	class CopyStructureCount;
	class ConstructClusterComponent : public Component
	{
		friend class GpuBvh2BuilderComponent2;
		RTTR_ENABLE(Component);
	public:
		ConstructClusterComponent();
		virtual ~ConstructClusterComponent() = default;
	public:
		void init(const std::vector<AABB>& aabbs);
	private:
		std::shared_ptr<Texture>	m_aabbBuffer;

		std::shared_ptr<Texture>    m_indirectBuffer;

		std::shared_ptr<Texture>	m_clusterCounts;
		std::shared_ptr<Texture>	m_clusterOffsets;
		std::shared_ptr<Texture>	m_clusterIndexes;
		std::shared_ptr<Texture>	m_clusterElements;
		std::shared_ptr<Texture>	m_clustersSmallers;

		std::shared_ptr<ClearUnorderedAccessView> m_clearUnorderedAccessView;
		std::shared_ptr<ClearUnorderedAccessView> m_clearUnorderedAccessView2;
		std::shared_ptr<ComputerCommand> m_calculateClusterCounts;
		std::shared_ptr<ComputerCommand> m_calculateClusterOffsets;
		std::shared_ptr<ComputerCommand> m_writeClusterElements;
		std::shared_ptr<CopyStructureCount> m_copyStructureCount;
		std::shared_ptr<ComputerCommand> m_writeCluster64;
		std::shared_ptr<GraphicsCommandList> m_graphicsCommandList;
	};
}