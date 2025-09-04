#include "ConstructClusterComponent.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/GraphicsPipeline/GraphicsCommandList.h"
#include "Engine/Engine.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/ClearUnorderedAccessView.h"
#include <d3d11.h>

namespace Destiny
{
	ConstructClusterComponent::ConstructClusterComponent() :
		m_aabbBuffer(nullptr),
		m_clusterCounts(nullptr),
		m_clusterOffsets(nullptr),
		m_clusterElements(nullptr),
		m_clearUnorderedAccessView(std::make_shared<ClearUnorderedAccessView>()),
		m_clearUnorderedAccessView2(std::make_shared<ClearUnorderedAccessView>()),
		m_calculateClusterCounts(nullptr),
		m_calculateClusterOffsets(nullptr),
		m_writeClusterElements(nullptr),
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

		{
			std::vector<unsigned int> vec;
			vec.resize(aabbs.size(), 0);

			std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)aabbs.size() * sizeof(unsigned int));
			data->copyfrom((void*)vec.data(), data->getLength());

			m_clusterCounts = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)data->getLength(), data);
			m_clusterCounts->load();
		}

		{
			m_clusterOffsets = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)aabbs.size() * sizeof(unsigned int));
			m_clusterOffsets->load();
		}

		{
			m_clusterElements = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)aabbs.size() * sizeof(unsigned int));
			m_clusterElements->load();
		}

		m_clearUnorderedAccessView->setParam(ClearUnorderedAccessView::ClearUint, m_clusterCounts);
		m_clearUnorderedAccessView2->setParam(ClearUnorderedAccessView::ClearUint, m_clusterCounts);

		std::vector<std::shared_ptr<Texture>> uavs = { m_aabbBuffer,  m_clusterCounts, m_clusterOffsets, m_clusterElements };
		unsigned int numGroupsX = ((unsigned int)aabbs.size() + 1024 - 1) / 1024;

		m_calculateClusterCounts = std::make_shared<ComputerCommand>();
		m_calculateClusterCounts->setDebugName(L"CalculateClusterCounts");
		m_calculateClusterCounts->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_cluster/calculate_cluster_counts.hlsl");
		m_calculateClusterCounts->setUnorderedAccessViews(uavs);
		m_calculateClusterCounts->setThreadGroupCount(numGroupsX, 1, 1);

		m_calculateClusterOffsets = std::make_shared<ComputerCommand>();
		m_calculateClusterOffsets->setDebugName(L"CalculateClusterOffsets");
		m_calculateClusterOffsets->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_cluster/calculate_cluster_offsets.hlsl");
		m_calculateClusterOffsets->setUnorderedAccessViews(uavs);
		m_calculateClusterOffsets->setThreadGroupCount(numGroupsX, 1, 1);

		m_writeClusterElements = std::make_shared<ComputerCommand>();
		m_writeClusterElements->setDebugName(L"WriteClusterElements");
		m_writeClusterElements->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_cluster/write_cluster_elements.hlsl");
		m_writeClusterElements->setUnorderedAccessViews(uavs);
		m_writeClusterElements->setThreadGroupCount(numGroupsX, 1, 1);

		m_graphicsCommandList->addGraphicsCommand(m_clearUnorderedAccessView);
		m_graphicsCommandList->addGraphicsCommand(m_calculateClusterCounts);
		m_graphicsCommandList->addGraphicsCommand(m_calculateClusterOffsets);
		m_graphicsCommandList->addGraphicsCommand(m_clearUnorderedAccessView2);
		m_graphicsCommandList->addGraphicsCommand(m_writeClusterElements);
	}
}
