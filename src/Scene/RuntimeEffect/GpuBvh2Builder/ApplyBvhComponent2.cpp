#include "ApplyBvhComponent2.h"
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
	ApplyBvhComponent2::ApplyBvhComponent2() :
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>()),
		m_applyBvhTopLevelCS(nullptr),
		m_applyBvhMiddleLevelCS(nullptr),
		m_applyBvhBottomLevelCS(nullptr),
		m_copyStructureCount(nullptr),
		m_copyStructureCount2(nullptr),
		m_outputBVH(nullptr),
		m_bitonicSortIndicesBuffer(nullptr),
		m_stack(nullptr),
		m_nextLayerBuffer(nullptr),
		m_nextLayerBuffer2(nullptr),
		m_cullElement(nullptr),
		m_indirectBuffer(nullptr),
		m_indirectBuffer2(nullptr)
	{
		set_serializable(false);
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"ApplyBvh", m_graphicsCommandList);
	}

	void ApplyBvhComponent2::init(unsigned int elementCount, std::shared_ptr<Texture> outputBVH, std::shared_ptr<Texture> bitonicSortIndicesBuffer)
	{
		m_outputBVH = outputBVH;
		m_bitonicSortIndicesBuffer = bitonicSortIndicesBuffer;

		struct StackNode
		{
			unsigned int nodeIndex;
			unsigned int depth;
		};

		{
			m_stack = Texture::CreateStructured(sizeof(StackNode), elementCount * sizeof(StackNode));
			m_stack->load();
		}

		{
			m_nextLayerBuffer = Texture::CreateStructuredAppend(sizeof(unsigned int), elementCount * sizeof(unsigned int));
			m_nextLayerBuffer->load();
		}

		{
			m_nextLayerBuffer2 = Texture::CreateStructuredAppend(sizeof(unsigned int), elementCount * sizeof(unsigned int));
			m_nextLayerBuffer2->load();
		}

		{
			m_cullElement = Texture::CreateStructuredAppend(sizeof(unsigned int), elementCount * sizeof(unsigned int));
			m_cullElement->load();
		}

		{
			std::vector<unsigned int> vec = { 1,1,1 };
			std::shared_ptr<Blob> data = std::make_shared<Blob>(3 * sizeof(unsigned int));
			data->copyfrom(vec.data(), data->getLength());
			m_indirectBuffer = Texture::CreateRaw((unsigned int)data->getLength(), data);
			m_indirectBuffer->load();
		}
		{
			std::vector<unsigned int> vec = { 1,1,1 };
			std::shared_ptr<Blob> data = std::make_shared<Blob>(3 * sizeof(unsigned int));
			data->copyfrom(vec.data(), data->getLength());
			m_indirectBuffer2 = Texture::CreateRaw((unsigned int)data->getLength(), data);
			m_indirectBuffer2->load();
		}
		applyBvhTopLevelCS();
		applyBvhMiddleLevelCS();
		applyBvhBottomLevelCS(elementCount);
		copyStructureCount();
	}

	void ApplyBvhComponent2::onUpdate(float deltaTime)
	{
		if (m_applyBvhBottomLevelCS)
		{
			auto scene = std::static_pointer_cast<VisualScene>(Engine::GetInstance()->getSceneManager()->getCurrentScene());
			if (scene)
			{
				const auto& cameraViewMatrix = scene->getCameraNode()->getInvTransposeWorldMatrix();
				const auto& cameraProjMaTrix = DirectX::XMMatrixTranspose(scene->getCamera()->getProjectionMatrix());
				
				m_applyBvhTopLevelCS->setConstant("g_view", cameraViewMatrix);
				m_applyBvhTopLevelCS->setConstant("g_proj", cameraProjMaTrix);
				
				m_applyBvhMiddleLevelCS->setConstant("g_view", cameraViewMatrix);
				m_applyBvhMiddleLevelCS->setConstant("g_proj", cameraProjMaTrix);

				m_applyBvhBottomLevelCS->setConstant("g_view", cameraViewMatrix);
				m_applyBvhBottomLevelCS->setConstant("g_proj", cameraProjMaTrix);
			}
		}
	}

	void ApplyBvhComponent2::applyBvhTopLevelCS()
	{
		std::vector<std::shared_ptr<Texture>> uavs =
		{
			m_outputBVH, m_stack, m_nextLayerBuffer
		};

		m_applyBvhTopLevelCS = std::make_shared<ComputerCommand>();
		m_applyBvhTopLevelCS->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/apply_bvh2/apply_bvh_top_level.hlsl");
		m_applyBvhTopLevelCS->setUnorderedAccessViews(uavs);
		m_applyBvhTopLevelCS->setThreadGroupCount(1, 1, 1);

		m_graphicsCommandList->addGraphicsCommand(m_applyBvhTopLevelCS);
	}

	void ApplyBvhComponent2::applyBvhMiddleLevelCS()
	{
		m_copyStructureCount = std::make_shared<CopyStructureCount>();
		m_copyStructureCount->setDstAndSrcTexture(m_indirectBuffer, m_nextLayerBuffer);

		std::vector<std::shared_ptr<Texture>> uavs =
		{
			m_outputBVH, m_stack, m_nextLayerBuffer, m_nextLayerBuffer2
		};

		
		m_applyBvhMiddleLevelCS = std::make_shared<ComputerCommand>();
		m_applyBvhMiddleLevelCS->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/apply_bvh2/apply_bvh_middle_level.hlsl");
		m_applyBvhMiddleLevelCS->setUnorderedAccessViews(uavs);
		m_applyBvhMiddleLevelCS->setIndirectMode(true, m_indirectBuffer, { 0 });

		m_graphicsCommandList->addGraphicsCommand(m_copyStructureCount);
		m_graphicsCommandList->addGraphicsCommand(m_applyBvhMiddleLevelCS);

	}

	void ApplyBvhComponent2::applyBvhBottomLevelCS(unsigned int elementCount)
	{
		m_copyStructureCount2 = std::make_shared<CopyStructureCount>();
		m_copyStructureCount2->setDstAndSrcTexture(m_indirectBuffer2, m_nextLayerBuffer2);

		std::vector<std::shared_ptr<Texture>> uavs =
		{
			m_outputBVH, m_bitonicSortIndicesBuffer, m_stack, m_nextLayerBuffer2, m_cullElement
		};

		m_applyBvhBottomLevelCS = std::make_shared<ComputerCommand>();
		m_applyBvhBottomLevelCS->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/apply_bvh2/apply_bvh_bottom_level.hlsl");
		m_applyBvhBottomLevelCS->setUnorderedAccessViews(uavs);
		m_applyBvhBottomLevelCS->setConstant("NumberOfElements", elementCount);
		m_applyBvhBottomLevelCS->setIndirectMode(true, m_indirectBuffer2, { 0 });

		m_graphicsCommandList->addGraphicsCommand(m_copyStructureCount2);
		m_graphicsCommandList->addGraphicsCommand(m_applyBvhBottomLevelCS);
	}

	void ApplyBvhComponent2::copyStructureCount()
	{
		{
			m_cullElementCount = Texture::CreateBuffer(sizeof(unsigned int));
			m_cullElementCount->load();
		}

		m_copyStructureCount = std::make_shared<CopyStructureCount>();
		m_copyStructureCount->setDstAndSrcTexture(m_cullElementCount, m_cullElement);

		m_graphicsCommandList->addGraphicsCommand(m_copyStructureCount);
	}
}