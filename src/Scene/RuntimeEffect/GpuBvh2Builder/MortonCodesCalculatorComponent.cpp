#include "MortonCodesCalculatorComponent.h"
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
	MortonCodesCalculatorComponent::MortonCodesCalculatorComponent() :
		m_calcuateMortonCodesForAABBs(nullptr),
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>())
	{
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"MortonCodesCalculate", m_graphicsCommandList);
	}


	void MortonCodesCalculatorComponent::init(const std::vector<AABB>& aabbs, std::shared_ptr<Texture> sceneAABB)
	{
		m_calcuateMortonCodesForAABBs = std::make_shared<ComputerCommand>();
		m_calcuateMortonCodesForAABBs->setDebugName(L"CalcuateMortonCodesForAABBs");
		m_calcuateMortonCodesForAABBs->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/morton_codes_calculator/CalculateMortonCodesForAABBs.hlsl");

		{
			m_outputIndicesBuffer = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)aabbs.size() * sizeof(unsigned int));
			m_outputIndicesBuffer->load();
		}

		{
			m_outputMortonCodesBuffer = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)aabbs.size() * sizeof(unsigned int));
			m_outputMortonCodesBuffer->load();
		}

		{
			m_sceneAABB = sceneAABB;
		}

		{
			std::vector<AABBNode> aabbNodes;

			for (const auto& aabb : aabbs)
			{
				AABBNode aabbNode;

				aabbNode.center[0] = (aabb.min.x + aabb.max.x) * 0.5f;
				aabbNode.center[1] = (aabb.min.y + aabb.max.y) * 0.5f;
				aabbNode.center[2] = (aabb.min.z + aabb.max.z) * 0.5f;

				aabbNode.halfDim[0] = (aabb.max.x - aabb.min.x) * 0.5f;
				aabbNode.halfDim[1] = (aabb.max.y - aabb.min.y) * 0.5f;
				aabbNode.halfDim[2] = (aabb.max.z - aabb.min.z) * 0.5f;

				aabbNode.flags = 0;
				aabbNode.rightNodeIndex = 0;
				aabbNodes.push_back(aabbNode);
			}

			std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)aabbNodes.size() * sizeof(AABBNode));
			data->copyfrom(aabbNodes.data(), data->getLength());

			m_inputBuffer = Texture::CreateRaw((unsigned int)data->getLength(), data);
			m_inputBuffer->load();
		}

		std::vector<std::shared_ptr<Texture>> uavs = { m_outputIndicesBuffer,  m_outputMortonCodesBuffer, m_sceneAABB, m_inputBuffer };
		m_calcuateMortonCodesForAABBs->setUnorderedAccessViews(uavs);
		m_calcuateMortonCodesForAABBs->setConstant("Constants", (unsigned int)aabbs.size());

		const unsigned int dispatchWidth = Math::DivideAndRoundUp<unsigned int>((unsigned int)aabbs.size(), THREAD_GROUP_1D_WIDTH);
		m_calcuateMortonCodesForAABBs->setThreadGroupCount(dispatchWidth, 1, 1);
		m_graphicsCommandList->addGraphicsCommand(m_calcuateMortonCodesForAABBs);
	}

	void MortonCodesCalculatorComponent::calcuateMortonCodesForAABBsCS()
	{
		m_calcuateMortonCodesForAABBs = std::make_shared<ComputerCommand>();
		m_calcuateMortonCodesForAABBs->setDebugName(L"CalcuateMortonCodesForAABBs");
		m_calcuateMortonCodesForAABBs->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/morton_codes_calculator/CalculateMortonCodesForAABBs.hlsl");

		m_calcuateMortonCodesForAABBs->setThreadGroupCount(1, 2, 3);

		std::vector<AABB> aabbs = { {{0.0f, 0.0f, 0.0f},{10.0f, 10.0f, 10.0f}} };

		{
			m_outputIndicesBuffer = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)aabbs.size() * sizeof(unsigned int));
			m_outputIndicesBuffer->load();
		}

		{
			m_outputMortonCodesBuffer = Texture::CreateStructured(sizeof(unsigned int), (unsigned int)aabbs.size() * sizeof(unsigned int));
			m_outputMortonCodesBuffer->load();
		}

		{
			AABB sceneAABB;
			sceneAABB.min = { 0.0f, 0.0f, 0.0f };
			sceneAABB.max = { 10.0f, 10.0f, 10.0f };


			LOG_TRACE("Merge Box min:{0},{1},{2}     max:{3},{4},{5}",
				std::to_string(sceneAABB.min.x), std::to_string(sceneAABB.min.y), std::to_string(sceneAABB.min.z),
				std::to_string(sceneAABB.max.x), std::to_string(sceneAABB.max.y), std::to_string(sceneAABB.max.z));

			std::shared_ptr<Blob> data = std::make_shared<Blob>(sizeof(AABB));
			memcpy_s(data->getData(), data->getLength(), &sceneAABB, data->getLength());

			m_sceneAABB = Texture::CreateStructured(sizeof(AABB), (unsigned int)data->getLength(), data);
			m_sceneAABB->load();
		}

		{
			std::vector<AABBNode> aabbNodes;

			for (const auto& aabb : aabbs)
			{
				AABBNode aabbNode;

				aabbNode.center[0] = (aabb.min.x + aabb.max.x) * 0.5f;
				aabbNode.center[1] = (aabb.min.y + aabb.max.y) * 0.5f;
				aabbNode.center[2] = (aabb.min.z + aabb.max.z) * 0.5f;

				aabbNode.halfDim[0] = (aabb.max.x - aabb.min.x) * 0.5f;
				aabbNode.halfDim[1] = (aabb.max.y - aabb.min.y) * 0.5f;
				aabbNode.halfDim[2] = (aabb.max.z - aabb.min.z) * 0.5f;

				aabbNode.flags = 0;
				aabbNode.rightNodeIndex = 0;
				aabbNodes.push_back(aabbNode);
			}

			std::shared_ptr<Blob> data = std::make_shared<Blob>((unsigned int)aabbNodes.size() * sizeof(AABBNode));
			data->copyfrom(aabbNodes.data(), data->getLength());

			m_inputBuffer = Texture::CreateRaw((unsigned int)data->getLength(), data);
			m_inputBuffer->load();
		}

		std::vector<std::shared_ptr<Texture>> uavs = { m_outputIndicesBuffer,  m_outputMortonCodesBuffer, m_sceneAABB, m_inputBuffer };
		m_calcuateMortonCodesForAABBs->setUnorderedAccessViews(uavs);
		m_calcuateMortonCodesForAABBs->setConstant("Constants", (unsigned int)aabbs.size());

		const unsigned int dispatchWidth = Math::DivideAndRoundUp<unsigned int>((unsigned int)aabbs.size(), THREAD_GROUP_1D_WIDTH);
		m_calcuateMortonCodesForAABBs->setThreadGroupCount(dispatchWidth, 1, 1);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<MortonCodesCalculatorComponent>("MortonCodesCalculatorComponent")
			.constructor<>();
	}
}