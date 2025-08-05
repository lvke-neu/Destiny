#include "GpuBvh2BuilderComponent.h"
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
#include "MortonCodesCalculatorComponent.h"
#include "BitonicSortComponent.h"
#include "ConstructHierarchyComponent.h"
#include "ConstructAABBPassComponent.h"
#include <numeric> 

namespace Destiny
{
	GpuBvh2BuilderComponent::GpuBvh2BuilderComponent() :
		m_sceneAABBCalculatorComponent(std::make_shared<SceneAABBCalculatorComponent>()),
		m_mortonCodesCalculatorComponent(std::make_shared<MortonCodesCalculatorComponent>()),
		m_bitonicSortComponent(std::make_shared<BitonicSortComponent>()),
		m_constructHierarchyComponent(std::make_shared<ConstructHierarchyComponent>()),
		m_constructAABBPassComponent(std::make_shared<ConstructAABBPassComponent>())
	{
		auto aabbs = GenerateTrulyRandomAABBs(1000000);

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

		m_sceneAABBCalculatorComponent->init(aabbs);
		m_mortonCodesCalculatorComponent->init(aabbs, m_sceneAABBCalculatorComponent->m_outputBuffer);
		m_bitonicSortComponent->init((unsigned int)aabbs.size(), m_mortonCodesCalculatorComponent->m_outputMortonCodesBuffer, m_mortonCodesCalculatorComponent->m_outputIndicesBuffer);
		m_constructHierarchyComponent->init((unsigned int)aabbs.size(), m_mortonCodesCalculatorComponent->m_outputMortonCodesBuffer);
		m_constructAABBPassComponent->init(aabbs, m_constructHierarchyComponent->m_hierarchyBuffer, m_mortonCodesCalculatorComponent->m_outputIndicesBuffer);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<GpuBvh2BuilderComponent>("GpuBvh2BuilderComponent")
			.constructor<>();
	}
}