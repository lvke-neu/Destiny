#include "ApplyBvhComponent3.h"
#include "Engine/Engine.h"
#include "Engine/Node.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/GraphicsPipeline/GraphicsCommandList.h"
#include "Graphics/VisualScene.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/CopyStructureCount.h"
#include "Scene/SceneManager.h"

namespace Destiny
{
	ApplyBvhComponent3::ApplyBvhComponent3() :
		m_clustersSmallers(nullptr),
		m_commitClusters(nullptr),
		m_aabbuffer(nullptr),
		m_clusterOffsets(nullptr),
		m_clusterElements(nullptr), 
		m_commitElements(nullptr),		
		m_copyStructureCount(nullptr),	
		m_copyStructureCount2(nullptr),
		m_cullClusters(nullptr),
		m_cullElements(nullptr),
		m_indirectBuffer(nullptr),
		m_indirectBuffer2(nullptr),
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>())
	{
		set_serializable(false);
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"ApplyBvh", m_graphicsCommandList);
	}

	void ApplyBvhComponent3::init(unsigned int elementCount, std::shared_ptr<Texture> clustersSmallers,
		std::shared_ptr<Texture> aabbuffer,
		std::shared_ptr<Texture> clusterOffsets,
		std::shared_ptr<Texture> clusterElements)
	{
		m_clustersSmallers = clustersSmallers;
		m_aabbuffer = aabbuffer;
		m_clusterOffsets = clusterOffsets;
		m_clusterElements = clusterElements;

		{
			m_commitClusters = Texture::CreateStructuredAppend(sizeof(unsigned int), elementCount * sizeof(unsigned int));
			m_commitClusters->load();
		}

		{
			m_commitElements = Texture::CreateStructuredAppend(sizeof(unsigned int), elementCount * sizeof(unsigned int));
			m_commitElements->load();
		}


		{
			m_copyStructureCount = std::make_shared<CopyStructureCount>();

			std::vector<unsigned int> vec = { 1,1,1 };
			std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)vec.size() * sizeof(unsigned int));
			data->copyfrom(vec.data(), (unsigned int)data->getLength());
			m_indirectBuffer = Texture::CreateRaw(3 * sizeof(unsigned int), data);
			m_indirectBuffer->load();

			m_copyStructureCount->setDstAndSrcTexture(m_indirectBuffer, m_clustersSmallers);
		}

		{
			m_copyStructureCount2 = std::make_shared<CopyStructureCount>();

			std::vector<unsigned int> vec = { 1,1,1 };
			std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)vec.size() * sizeof(unsigned int));
			data->copyfrom(vec.data(), (unsigned int)data->getLength());
			m_indirectBuffer2 = Texture::CreateRaw(3 * sizeof(unsigned int), data);
			m_indirectBuffer2->load();

			m_copyStructureCount2->setDstAndSrcTexture(m_indirectBuffer2, m_commitClusters);
		}


		std::vector<std::shared_ptr<Texture>> uavs = { m_clustersSmallers, m_commitClusters };

		m_cullClusters = std::make_shared<ComputerCommand>();
		m_cullClusters->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/apply_bvh3/cull_clusters.hlsl");
		m_cullClusters->setUnorderedAccessViews(uavs);
		m_cullClusters->setIndirectMode(true, m_indirectBuffer, { 0 });

		uavs = { m_clustersSmallers, m_commitClusters, m_aabbuffer, m_clusterOffsets, m_clusterElements, m_commitElements };
		m_cullElements = std::make_shared<ComputerCommand>();
		m_cullElements->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/apply_bvh3/cull_elements.hlsl");
		m_cullElements->setUnorderedAccessViews(uavs);
		m_cullElements->setIndirectMode(true, m_indirectBuffer2, { 0 });


		m_graphicsCommandList->addGraphicsCommand(m_copyStructureCount);
		m_graphicsCommandList->addGraphicsCommand(m_cullClusters);
		m_graphicsCommandList->addGraphicsCommand(m_copyStructureCount2);
		m_graphicsCommandList->addGraphicsCommand(m_cullElements);
	}

	void ApplyBvhComponent3::onUpdate(float deltaTime)
	{
		if (m_cullClusters)
		{
			auto scene = std::static_pointer_cast<VisualScene>(Engine::GetInstance()->getSceneManager()->getCurrentScene());
			if (scene)
			{
				const auto& cameraViewMatrix = scene->getCameraNode()->getInvTransposeWorldMatrix();
				const auto& cameraProjMaTrix = DirectX::XMMatrixTranspose(scene->getCamera()->getProjectionMatrix());

				m_cullClusters->setConstant("g_view", cameraViewMatrix);
				m_cullClusters->setConstant("g_proj", cameraProjMaTrix);

				m_cullElements->setConstant("g_view", cameraViewMatrix);
				m_cullElements->setConstant("g_proj", cameraProjMaTrix);
			}
		}
	}
}