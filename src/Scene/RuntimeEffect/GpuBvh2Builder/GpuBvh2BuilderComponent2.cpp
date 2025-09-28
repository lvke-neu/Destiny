#include "GpuBvh2BuilderComponent2.h"
#include "Engine/Node.h"
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
#include "ApplyBvhComponent.h"
#include "ApplyBvhComponent2.h"
#include "ApplyBvhComponent3.h"
#include "ConstructClusterComponent.h"
#include "../BoxVisualizationComponent.h"
#include <numeric> 

namespace Destiny
{
	GpuBvh2BuilderComponent2::GpuBvh2BuilderComponent2() :
		m_constructClusterComponent(std::make_shared<ConstructClusterComponent>()),
		m_applyBvhComponent3(std::make_shared<ApplyBvhComponent3>())
	{
		//m_aabbs = GenerateTrulyRandomAABBs(100000);

		AABB tmp0 =
		{
				{-5.0f, -5.0f, 95.0f},
				{5.0f, 5.0f,105.0f}
		};

		AABB tmp1 =
		{
				{-5.0f, -5.0f, -105.0f},
				{5.0f, 5.0f,-95.0f}
		};
		AABB tmp2 =
		{
				{-105.0f, -5.0f, -5.0f},
				{-95.0f, 5.0f,5.0f}
		};
		AABB tmp3 =
		{
				{95.0f, -5.0f, -5.0f},
				{105.0f, 5.0f,5.0f}
		};


		for (int i = 0; i < 1; i++)
		{
			m_aabbs.push_back(tmp0);
		}

		for (int i = 0; i < 1; i++)
		{
			m_aabbs.push_back(tmp1);
		}

		for (int i = 0; i < 1; i++)
		{
			m_aabbs.push_back(tmp2);
		}

		for (int i = 0; i < 1; i++)
		{
			m_aabbs.push_back(tmp3);
		}

		for (int i = 0; i < m_aabbs.size(); i++)
		{
			m_boxVisualizationComponents.push_back(std::make_shared<BoxVisualizationComponent>());

			DirectX::XMFLOAT3 center;
			center.x = (m_aabbs[i].min.x + m_aabbs[i].max.x) / 2;
			center.y = (m_aabbs[i].min.y + m_aabbs[i].max.y) / 2;
			center.z = (m_aabbs[i].min.z + m_aabbs[i].max.z) / 2;

			DirectX::XMFLOAT3 extent;
			extent.x = (m_aabbs[i].min.x - m_aabbs[i].max.x) / 2;
			extent.y = (m_aabbs[i].min.y - m_aabbs[i].max.y) / 2;
			extent.z = (m_aabbs[i].min.z - m_aabbs[i].max.z) / 2;

			m_boxVisualizationComponents[i]->modifyMesh({ center,extent });
			m_boxVisualizationComponents[i]->set_serializable(false);
		}

		DirectX::XMFLOAT3 min = { FLT_MAX,FLT_MAX,FLT_MAX };
		DirectX::XMFLOAT3 max = { -FLT_MAX,-FLT_MAX ,-FLT_MAX };

		for (const auto& aabb : m_aabbs)
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

		m_constructClusterComponent->init(m_aabbs);
		m_applyBvhComponent3->init((unsigned int)m_aabbs.size(), m_constructClusterComponent->m_clustersSmallers, 
			m_constructClusterComponent->m_aabbBuffer, 
			m_constructClusterComponent->m_clusterOffsets,
			m_constructClusterComponent->m_clusterElements);
	}

	GpuBvh2BuilderComponent2::~GpuBvh2BuilderComponent2()
	{

	}

	void GpuBvh2BuilderComponent2::onAddToNode()
	{
		auto node = std::make_shared<Node>();
		node->set_name("Debug");
		node->addToParent(m_node);
		for (const auto& boxVisualizationComponent : m_boxVisualizationComponents)
		{
			node->addComponent(boxVisualizationComponent);
		}

		node->addComponent(m_applyBvhComponent3);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<GpuBvh2BuilderComponent2>("GpuBvh2BuilderComponent2")
			.constructor<>();
	}
}