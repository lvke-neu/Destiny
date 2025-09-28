#include "ConstructClusterComponent.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/GraphicsPipeline/GraphicsCommandList.h"
#include "Graphics/CopyStructureCount.h"
#include "Engine/Engine.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/ClearUnorderedAccessView.h"
#include <d3d11.h>

namespace Destiny
{
	ConstructClusterComponent::ConstructClusterComponent() :
		m_aabbBuffer(nullptr),
		m_indirectBuffer(nullptr),
		m_clusterCounts(nullptr),
		m_clusterOffsets(nullptr),
		m_clusterIndexes(nullptr),
		m_clusterElements(nullptr),
		m_clustersSmallers(nullptr),
		m_clearUnorderedAccessView(std::make_shared<ClearUnorderedAccessView>()),
		m_clearUnorderedAccessView2(std::make_shared<ClearUnorderedAccessView>()),
		m_calculateClusterCounts(nullptr),
		m_calculateClusterOffsets(nullptr),
		m_writeClusterElements(nullptr),
		m_copyStructureCount(nullptr),
		m_writeCluster64(nullptr),
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>())
	{
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"ConstructCluster", m_graphicsCommandList);
	}

	void ConstructClusterComponent::init(const std::vector<AABB>& aabbs)
	{
		{
			std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)aabbs.size() * sizeof(AABB));
			data->copyfrom((void*)aabbs.data(), data->getLength());

			m_aabbBuffer = Texture::CreateStructured(sizeof(AABB), (unsigned int)data->getLength(), data);
			m_aabbBuffer->load();
		}

		unsigned int ExponentMax = 300;

		{
			std::vector<unsigned int> vec;
			vec.resize(aabbs.size(), 0);

			std::shared_ptr<Blob> data = std::make_shared<Blob>(ExponentMax * sizeof(unsigned int));
			data->copyfrom((void*)vec.data(), data->getLength());

			m_clusterCounts = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)data->getLength(), data);
			m_clusterCounts->load();
		}

		{
			m_clusterOffsets = Texture::CreateStructured(sizeof(unsigned int), ExponentMax * sizeof(unsigned int));
			m_clusterOffsets->load();
		}

		{
			m_clusterIndexes = Texture::CreateStructuredAppend(sizeof(unsigned int), ExponentMax * sizeof(unsigned int));
			m_clusterIndexes->load();
		}

		{
			m_clusterElements = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)aabbs.size() * sizeof(unsigned int));
			m_clusterElements->load();
		}

		{
			struct Cluster64
			{
				unsigned int clusterIndex;
				unsigned int clusterStart;
				unsigned int clusterCount;
				AABB aabb;
			};

			m_clustersSmallers = Texture::CreateStructuredAppend(sizeof(Cluster64), (unsigned int)aabbs.size() * sizeof(Cluster64));
			m_clustersSmallers->load();
		}

		{
			m_copyStructureCount = std::make_shared<CopyStructureCount>();

			std::vector<unsigned int> vec = { 1,1,1 };
			std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)vec.size() * sizeof(unsigned int));
			data->copyfrom(vec.data(), (unsigned int)data->getLength());
			m_indirectBuffer = Texture::CreateRaw(3 * sizeof(unsigned int), data);
			m_indirectBuffer->load();
		}

		m_clearUnorderedAccessView->setParam(ClearUnorderedAccessView::ClearUint, m_clusterCounts);
		m_clearUnorderedAccessView2->setParam(ClearUnorderedAccessView::ClearUint, m_clusterCounts);

		std::vector<std::shared_ptr<Texture>> uavs;
		unsigned int numGroupsX = ((unsigned int)aabbs.size() + 1024 - 1) / 1024;

		uavs = { m_aabbBuffer,  m_clusterCounts };
		m_calculateClusterCounts = std::make_shared<ComputerCommand>();
		m_calculateClusterCounts->setDebugName(L"CalculateClusterCounts");
		m_calculateClusterCounts->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_cluster/calculate_cluster_counts.hlsl");
		m_calculateClusterCounts->setUnorderedAccessViews(uavs, {1,0});
		m_calculateClusterCounts->setThreadGroupCount(numGroupsX, 1, 1);

		uavs = { m_aabbBuffer,  m_clusterCounts, m_clusterOffsets, m_clusterIndexes,m_clustersSmallers };
		m_calculateClusterOffsets = std::make_shared<ComputerCommand>();
		m_calculateClusterOffsets->setDebugName(L"CalculateClusterOffsets");
		m_calculateClusterOffsets->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_cluster/calculate_cluster_offsets.hlsl");
		m_calculateClusterOffsets->setUnorderedAccessViews(uavs, {1,0,0,0,1});
		m_calculateClusterOffsets->setThreadGroupCount(1, 1, 1);

		uavs = { m_aabbBuffer,  m_clusterCounts, m_clusterOffsets, m_clusterElements };
		m_writeClusterElements = std::make_shared<ComputerCommand>();
		m_writeClusterElements->setDebugName(L"WriteClusterElements");
		m_writeClusterElements->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_cluster/write_cluster_elements.hlsl");
		m_writeClusterElements->setUnorderedAccessViews(uavs, {0,0,0,0});
		m_writeClusterElements->setThreadGroupCount(numGroupsX, 1, 1);

		
		m_copyStructureCount->setDstAndSrcTexture(m_indirectBuffer, m_clustersSmallers);
		uavs = { m_aabbBuffer, m_clusterCounts, m_clusterOffsets,m_clusterIndexes,m_clusterElements,m_clustersSmallers };
		m_writeCluster64 = std::make_shared<ComputerCommand>();
		m_writeCluster64->setDebugName(L"WriteClusterSmallers");
		m_writeCluster64->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_cluster/write_cluster_smaller_64.hlsl");
		m_writeCluster64->setUnorderedAccessViews(uavs, {0,0,0,0,0,1});
		m_writeCluster64->setIndirectMode(true, m_indirectBuffer, {0});

		m_graphicsCommandList->addGraphicsCommand(m_clearUnorderedAccessView);
		m_graphicsCommandList->addGraphicsCommand(m_calculateClusterCounts);
		m_graphicsCommandList->addGraphicsCommand(m_calculateClusterOffsets);
		m_graphicsCommandList->addGraphicsCommand(m_clearUnorderedAccessView2);
		m_graphicsCommandList->addGraphicsCommand(m_writeClusterElements);
		m_graphicsCommandList->addGraphicsCommand(m_copyStructureCount);
		m_graphicsCommandList->addGraphicsCommand(m_writeCluster64);
	}
}
