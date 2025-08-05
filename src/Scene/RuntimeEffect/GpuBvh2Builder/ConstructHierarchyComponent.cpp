#include "ConstructHierarchyComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/RenderSystem.h"
#include "Math/Math.h"
#include "SceneAABBCalculatorComponent.h"
#include <numeric> 
#include <random>

namespace Destiny
{
	ConstructHierarchyComponent::ConstructHierarchyComponent() :
		m_numElements(0),
		m_outputMortonCodesBuffer(nullptr),
		m_hierarchyBuffer(nullptr),
		m_buildSplits(nullptr),
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>())
	{
		//init({AABB()}, nullptr);

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"BuildSplits", m_graphicsCommandList);
	}

	void ConstructHierarchyComponent::init(unsigned int numElements, std::shared_ptr<Texture> outputMortonCodesBuffer)
	{
		m_numElements = numElements;
		m_outputMortonCodesBuffer = outputMortonCodesBuffer;

		buildSplitsCS();
	}

	static UINT GetNumberOfInternalNodes(UINT numLeaves)
	{
		// A binary tree with N leaves will always have N - 1 internal nodes
		return std::max(0, (INT)(numLeaves - 1));
	}

	void ConstructHierarchyComponent::buildSplitsCS()
	{
		{
			unsigned int totalNumNodes = m_numElements + GetNumberOfInternalNodes(m_numElements);
			m_hierarchyBuffer = Texture::CreateStructured(sizeof(HierarchyNode), totalNumNodes * sizeof(HierarchyNode));
			m_hierarchyBuffer->load();
		}


		m_buildSplits = std::make_shared<ComputerCommand>();
		m_buildSplits->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_hierarchy/BuildBVHSplits.hlsl");


		std::vector<std::shared_ptr<Texture>> uavs =
		{
			m_outputMortonCodesBuffer,  m_hierarchyBuffer
		};
		const UINT dispatchWidth = Math::DivideAndRoundUp<UINT>(m_numElements, THREAD_GROUP_1D_WIDTH);
		m_buildSplits->setUnorderedAccessViews(uavs); 
		m_buildSplits->setConstant("Constants", m_numElements);
		m_buildSplits->setThreadGroupCount(dispatchWidth, 1, 1);
		
		m_graphicsCommandList->addGraphicsCommand(m_buildSplits);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ConstructHierarchyComponent>("ConstructHierarchyComponent")
			.constructor<>();
	}
}