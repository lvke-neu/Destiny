#include "ApplyBvhComponent.h"
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
	ApplyBvhComponent::ApplyBvhComponent() :
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>()),
		m_applyBvhCS(nullptr),
		m_copyStructureCount(nullptr),
		m_outputBVH(nullptr),
		m_bitonicSortIndicesBuffer(nullptr),
		m_hierarchyBuffer(nullptr),
		m_stack(nullptr),
		m_cullElement(nullptr),
		m_cullElementCount(nullptr)
	{
		set_serializable(false);
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"ApplyBvh", m_graphicsCommandList);
	}

	void ApplyBvhComponent::init(unsigned int elementCount, std::shared_ptr<Texture> outputBVH, std::shared_ptr<Texture> hierarchyBuffer, std::shared_ptr<Texture> bitonicSortIndicesBuffer)
	{
		m_outputBVH = outputBVH;
		m_hierarchyBuffer = hierarchyBuffer;
		m_bitonicSortIndicesBuffer = bitonicSortIndicesBuffer;

		applyBvhCS(elementCount);
		copyStructureCount();
	}

	void ApplyBvhComponent::onUpdate(float deltaTime)
	{
		if (m_applyBvhCS)
		{
			auto scene = std::static_pointer_cast<VisualScene>(Engine::GetInstance()->getSceneManager()->getCurrentScene());
			if (scene)
			{
				const auto& cameraViewMatrix = scene->getCameraNode()->getInvTransposeWorldMatrix();
				const auto& cameraProjMaTrix = DirectX::XMMatrixTranspose(scene->getCamera()->getProjectionMatrix());
				m_applyBvhCS->setConstant("g_view", cameraViewMatrix);
				m_applyBvhCS->setConstant("g_proj", cameraProjMaTrix);
			}
		}
	}

	void ApplyBvhComponent::onLeaveScene()
	{
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->removeBeforePipelineCommandList(L"ApplyBvh", m_graphicsCommandList);
	}

	void ApplyBvhComponent::applyBvhCS(unsigned int elementCount)
	{
		{
			m_stack = Texture::CreateStructured(sizeof(unsigned int), elementCount * sizeof(unsigned int));
			m_stack->load();
		}
		
		{
			m_cullElement = Texture::CreateStructuredAppend(sizeof(unsigned int), elementCount * sizeof(unsigned int));
			m_cullElement->load();
		}

		std::vector<std::shared_ptr<Texture>> uavs =
		{
			m_outputBVH, m_hierarchyBuffer, m_bitonicSortIndicesBuffer, m_stack, m_cullElement
		};
		
		m_applyBvhCS = std::make_shared<ComputerCommand>();
		m_applyBvhCS->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/apply_bvh/apply_bvh.hlsl");
		m_applyBvhCS->setUnorderedAccessViews(uavs, {0,0,0,0,0});
		m_applyBvhCS->setThreadGroupCount(1, 1, 1);
		m_applyBvhCS->setConstant("NumberOfElements", elementCount);

		m_graphicsCommandList->addGraphicsCommand(m_applyBvhCS);
	}

	void ApplyBvhComponent::copyStructureCount()
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