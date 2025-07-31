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
#include "StructDefine.h"
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
		generateFakeData();
		calculateSceneAABBFromBVHsCS();
		calculateSceneAABBFromAABBsCS();

		m_sceneAABBCalculatorCommand->setParam(m_numElements, m_aabbBuffer, m_outputBuffer, m_calculateSceneAABBFromBVHs, m_calculateSceneAABBFromAABBs);
		m_graphicsCommandList->addGraphicsCommand(m_sceneAABBCalculatorCommand);
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"SceneAABBCalculate", m_graphicsCommandList);
	}

	std::vector<AABB> generateTrulyRandomAABBs() {
		std::vector<AABB> aabbs;
		aabbs.reserve(1000);

		auto now = std::chrono::high_resolution_clock::now();
		auto seed = now.time_since_epoch().count();
		std::mt19937_64 gen(seed);

		
		std::uniform_real_distribution<float> baseOffsetDist(-500.0f, 500.0f);
		float baseX = baseOffsetDist(gen);
		float baseY = baseOffsetDist(gen);
		float baseZ = baseOffsetDist(gen);

		std::uniform_real_distribution<float> scaleDist(100.0f, 1000.0f);
		float scale = scaleDist(gen);

		for (int i = 0; i < 1000; ++i) {

			std::uniform_real_distribution<float> centerDist(-scale / 2, scale / 2);
			float centerX = baseX + centerDist(gen);
			float centerY = baseY + centerDist(gen);
			float centerZ = baseZ + centerDist(gen);

			std::uniform_real_distribution<float> sizeDist(1.0f, scale * 0.1f);
			float sizeX = sizeDist(gen);
			float sizeY = sizeDist(gen);
			float sizeZ = sizeDist(gen);


			AABB aabb;
			aabb.min = { centerX - sizeX / 2, centerY - sizeY / 2, centerZ - sizeZ / 2 };
			aabb.max = { centerX + sizeX / 2, centerY + sizeY / 2, centerZ + sizeZ / 2 };

			aabbs.push_back(aabb);
		}

		return aabbs;
	}


	void SceneAABBCalculatorComponent::generateFakeData()
	{
		//for (int i = 0; i < 10; i++)
		//{
		//	std::vector<AABB> aabbs = generateTrulyRandomAABBs();
		//	DirectX::XMFLOAT3 min = { FLT_MAX,FLT_MAX,FLT_MAX };
		//	DirectX::XMFLOAT3 max = { FLT_MIN,FLT_MIN ,FLT_MIN };

		//	for (const auto& aabb : aabbs)
		//	{
		//		if (aabb.min.x < min.x)
		//		{
		//			min.x = aabb.min.x;
		//		}
		//		if (aabb.min.y < min.y)
		//		{
		//			min.y = aabb.min.y;
		//		}
		//		if (aabb.min.z < min.z)
		//		{
		//			min.z = aabb.min.z;
		//		}

		//		if (aabb.max.x > max.x)
		//		{
		//			max.x = aabb.max.x;
		//		}
		//		if (aabb.max.y > max.y)
		//		{
		//			max.y = aabb.max.y;
		//		}
		//		if (aabb.max.z > max.z)
		//		{
		//			max.z = aabb.max.z;
		//		}
		//	}

		//	LOG_TRACE("Merge Box min:{0},{1},{2}     max:{3},{4},{5}",
		//		std::to_string(min.x), std::to_string(min.y), std::to_string(min.z),
		//		std::to_string(max.x), std::to_string(max.y), std::to_string(max.z));
		//}

		std::vector<AABB> aabbs = generateTrulyRandomAABBs();

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
	
		//std::vector<std::shared_ptr<Texture>> uavs = { m_aabbBuffer, m_outputBuffer };
		//m_calculateSceneAABBFromBVHs->setUnorderedAccessViews(uavs);
	}

	void SceneAABBCalculatorComponent::calculateSceneAABBFromAABBsCS()
	{
		m_calculateSceneAABBFromAABBs = std::make_shared<ComputerCommand>();
		m_calculateSceneAABBFromAABBs->setDebugName(L"CalculateSceneAABBFromAABBs");
		m_calculateSceneAABBFromAABBs->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/scene_aabb_calculator/CalculateSceneAABBFromAABBs.hlsl");
		
		//std::vector<std::shared_ptr<Texture>> uavs = { m_aabbBuffer, m_outputBuffer };
		//m_calculateSceneAABBFromAABBs->setUnorderedAccessViews(uavs);
	}

	
	RTTR_REGISTRATION
	{
		rttr::registration::class_<SceneAABBCalculatorComponent>("SceneAABBCalculatorComponent")
			.constructor<>();
	}
}