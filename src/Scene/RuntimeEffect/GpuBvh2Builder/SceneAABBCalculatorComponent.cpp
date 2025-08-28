#include "SceneAABBCalculatorComponent.h"
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
#include "SceneAABBCalculatorCommand.h"
#include <numeric> 
#include <random>

namespace Destiny
{
	SceneAABBCalculatorComponent::SceneAABBCalculatorComponent() :
		m_calculateSceneAABBFromBVHs(nullptr),
		m_calculateSceneAABBFromAABBs(nullptr),
		m_sceneAABBCalculatorCommand(std::make_shared<SceneAABBCalculatorCommand>()),
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>()),
		m_aabbBuffer(nullptr),
		m_outputBuffer(nullptr),
		m_numElements(0)
	{
		m_graphicsCommandList->addGraphicsCommand(m_sceneAABBCalculatorCommand);
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"SceneAABBCalculate", m_graphicsCommandList);
	}

	unsigned int GetNumAABBsOutputFromPass(unsigned int numElements)
	{
		return  Math::DivideAndRoundUp<unsigned int>(numElements, ElementsSummedPerThread);
	}

	unsigned int SceneAABBCalculatorComponent::ScratchBufferSizeNeeded(unsigned int numElements)
	{
		if (numElements == 0) return 0;

		// Data is ping-ponged between the upper and lower part of the scratch buffer so
		// only need to sum the first 2 biggest passes
		auto numberOfAABBsOutput = GetNumAABBsOutputFromPass(numElements);
		if (numberOfAABBsOutput > 1)
		{
			numberOfAABBsOutput += GetNumAABBsOutputFromPass(numberOfAABBsOutput);
		}
		return numberOfAABBsOutput * sizeof(AABB);
	}

	void SceneAABBCalculatorComponent::init(const std::vector<AABB>& aabbs)
	{
		m_numElements = (unsigned int)aabbs.size();

		std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)aabbs.size() * sizeof(AABB));
		data->copyfrom((void*)aabbs.data(), data->getLength());

		m_aabbBuffer = Texture::CreateStructured(sizeof(AABB), (unsigned int)data->getLength(), data);
		m_aabbBuffer->load();

		m_outputBuffer = Texture::CreateStructured(sizeof(AABB), sizeof(AABB));
		m_outputBuffer->load();

		calculateSceneAABBFromBVHsCS();
		calculateSceneAABBFromAABBsCS();

		m_sceneAABBCalculatorCommand->setParam(m_numElements, m_aabbBuffer, m_outputBuffer, m_calculateSceneAABBFromBVHs, m_calculateSceneAABBFromAABBs);
	}

	void SceneAABBCalculatorComponent::onLeaveScene()
	{
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->removeBeforePipelineCommandList(L"SceneAABBCalculate", m_graphicsCommandList);
	}

	void SceneAABBCalculatorComponent::generateFakeData()
	{
		std::vector<AABB> aabbs = GenerateTrulyRandomAABBs(10000);

		DirectX::XMFLOAT3 min = { FLT_MAX,FLT_MAX,FLT_MAX };
		DirectX::XMFLOAT3 max = { -FLT_MAX,-FLT_MAX ,-FLT_MAX };
		
		for (const auto& aabb : aabbs)
		{
			if (aabb.min.x < min.x)
			{
				min.x = aabb.min.x;
			}
			if (aabb.min.y < min.y)
			{
				min.y = aabb.min.y;
			}
			if (aabb.min.z < min.z)
			{
				min.z = aabb.min.z;
			}

			if (aabb.max.x > max.x)
			{
				max.x = aabb.max.x;
			}
			if (aabb.max.y > max.y)
			{
				max.y = aabb.max.y;
			}
			if (aabb.max.z > max.z)
			{
				max.z = aabb.max.z;
			}
		}

		LOG_TRACE("Merge Box min:{0},{1},{2}     max:{3},{4},{5}",
			std::to_string(min.x), std::to_string(min.y), std::to_string(min.z),
			std::to_string(max.x), std::to_string(max.y), std::to_string(max.z));

		m_numElements = (unsigned int)aabbs.size();

		std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)aabbs.size() * sizeof(AABB));
		data->copyfrom(aabbs.data(), data->getLength());

		m_aabbBuffer = Texture::CreateStructured(sizeof(AABB), (unsigned int)data->getLength(), data);
		m_aabbBuffer->load();

		m_outputBuffer = Texture::CreateStructured(sizeof(AABB), sizeof(AABB));
		m_outputBuffer->load();
	}

	void SceneAABBCalculatorComponent::calculateSceneAABBFromBVHsCS()
	{
		m_calculateSceneAABBFromBVHs = std::make_shared<ComputerCommand>();
		m_calculateSceneAABBFromBVHs->setDebugName(L"CalculateSceneAABBFromBVHs");
		m_calculateSceneAABBFromBVHs->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/scene_aabb_calculator/CalculateSceneAABBFromBVHs.hlsl");
	}

	void SceneAABBCalculatorComponent::calculateSceneAABBFromAABBsCS()
	{
		m_calculateSceneAABBFromAABBs = std::make_shared<ComputerCommand>();
		m_calculateSceneAABBFromAABBs->setDebugName(L"CalculateSceneAABBFromAABBs");
		m_calculateSceneAABBFromAABBs->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/scene_aabb_calculator/CalculateSceneAABBFromAABBs.hlsl");
	}

	
	RTTR_REGISTRATION
	{
		rttr::registration::class_<SceneAABBCalculatorComponent>("SceneAABBCalculatorComponent")
			.constructor<>();
	}
}