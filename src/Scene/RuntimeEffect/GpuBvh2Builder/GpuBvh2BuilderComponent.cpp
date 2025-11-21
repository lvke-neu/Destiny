#include "GpuBvh2BuilderComponent.h"
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
#include "../BoxVisualizationComponent.h"
#include <numeric> 

namespace Destiny
{
	GpuBvh2BuilderComponent::GpuBvh2BuilderComponent() :
		m_sceneAABBCalculatorComponent(std::make_shared<SceneAABBCalculatorComponent>()),
		m_mortonCodesCalculatorComponent(std::make_shared<MortonCodesCalculatorComponent>()),
		m_bitonicSortComponent(std::make_shared<BitonicSortComponent>()),
		m_constructHierarchyComponent(std::make_shared<ConstructHierarchyComponent>()),
		m_constructAABBPassComponent(std::make_shared<ConstructAABBPassComponent>()),
		m_applyBvhComponent(std::make_shared<ApplyBvhComponent>()),
		m_applyBvhComponent2(std::make_shared<ApplyBvhComponent2>())
	{
		m_aabbs = GenerateTrulyRandomAABBs(1000000);
		//m_aabbs =
		//{
		//	{
		//		{-5.0f, -5.0f, 95.0f},
		//		{5.0f, 5.0f,105.0f}
		//	},
		//	{
		//		{-5.0f, -5.0f, -105.0f},
		//		{5.0f, 5.0f,-95.0f}
		//	},
		//	{
		//		{-105.0f, -5.0f, -5.0f},
		//		{-95.0f, 5.0f,5.0f}
		//	},
		//	{
		//		{95.0f, -5.0f, -5.0f},
		//		{105.0f, 5.0f,5.0f}
		//	}
		//};

		//for (int i = 0; i < m_aabbs.size(); i++)
		//{
		//	m_boxVisualizationComponents.push_back(std::make_shared<BoxVisualizationComponent>());

		//	DirectX::XMFLOAT3 center;
		//	center.x = (m_aabbs[i].min.x + m_aabbs[i].max.x) / 2;
		//	center.y = (m_aabbs[i].min.y + m_aabbs[i].max.y) / 2;
		//	center.z = (m_aabbs[i].min.z + m_aabbs[i].max.z) / 2;

		//	DirectX::XMFLOAT3 extent;
		//	extent.x = (m_aabbs[i].min.x - m_aabbs[i].max.x) / 2;
		//	extent.y = (m_aabbs[i].min.y - m_aabbs[i].max.y) / 2;
		//	extent.z = (m_aabbs[i].min.z - m_aabbs[i].max.z) / 2;

		//	m_boxVisualizationComponents[i]->modifyMesh({ center,extent });
		//	m_boxVisualizationComponents[i]->set_serializable(false);
		//}

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

		//m_boxVisualizationComponents.push_back(std::make_shared<BoxVisualizationComponent>());

		//DirectX::XMFLOAT3 center;
		//center.x = (min.x + max.x) / 2;
		//center.y = (min.y + max.y) / 2;
		//center.z = (min.z + max.z) / 2;

		//DirectX::XMFLOAT3 extent;
		//extent.x = (min.x - max.x) / 2;
		//extent.y = (min.y - max.y) / 2;
		//extent.z = (min.z - max.z) / 2;

		//m_boxVisualizationComponents.back()->modifyMesh({ center,extent });
		//m_boxVisualizationComponents.back()->set_serializable(false);


		//m_constructClusterComponent->init(m_aabbs);
		m_sceneAABBCalculatorComponent->init(m_aabbs);
		m_mortonCodesCalculatorComponent->init(m_aabbs, m_sceneAABBCalculatorComponent->m_outputBuffer);
		m_bitonicSortComponent->init((unsigned int)m_aabbs.size(), m_mortonCodesCalculatorComponent->m_outputMortonCodesBuffer, m_mortonCodesCalculatorComponent->m_outputIndicesBuffer);
		m_constructHierarchyComponent->init((unsigned int)m_aabbs.size(), m_mortonCodesCalculatorComponent->m_outputMortonCodesBuffer);
		m_constructAABBPassComponent->init(m_aabbs, m_constructHierarchyComponent->m_hierarchyBuffer, m_mortonCodesCalculatorComponent->m_outputIndicesBuffer);
		//m_applyBvhComponent->init((unsigned int)m_aabbs.size(), m_constructAABBPassComponent->m_outputBVH, m_constructAABBPassComponent->m_hierarchyBuffer, m_mortonCodesCalculatorComponent->m_outputIndicesBuffer);
		m_applyBvhComponent2->init((unsigned int)m_aabbs.size(), m_constructAABBPassComponent->m_outputBVH, m_mortonCodesCalculatorComponent->m_outputIndicesBuffer);
	}

	GpuBvh2BuilderComponent::~GpuBvh2BuilderComponent()
	{

	}

	static UINT GetNumberOfInternalNodes(UINT numLeaves)
	{
		// A binary tree with N leaves will always have N - 1 internal nodes
		return std::max(0, (INT)(numLeaves - 1));

	}
	void GpuBvh2BuilderComponent::set_test(Button test)
	{
		auto numElements = (unsigned int)m_aabbs.size();
		unsigned int totalNumNodes = numElements + GetNumberOfInternalNodes(numElements);
		auto resultDataMaxSizeInBytes = sizeof(BVHOffsets) + sizeof(AABBNode) * totalNumNodes + sizeof(BVHMetadata) * numElements;

		auto rawBufferWidth = std::static_pointer_cast<TextureCreationParam>(m_constructAABBPassComponent->m_outputBVH->getCreationParam())->rawBufferWidth;
		std::shared_ptr<Blob> data = std::make_shared<Blob>(rawBufferWidth);

		std::vector<AABBNode> aabbnodes;
		aabbnodes.resize(totalNumNodes);
		if (m_constructAABBPassComponent->m_outputBVH->getBuffer(data))
		{
			memcpy_s(aabbnodes.data(), sizeof(AABBNode) * totalNumNodes, (char*)data->getData() + sizeof(BVHOffsets), sizeof(AABBNode) * totalNumNodes);
		}
		
		//LOG_TRACE("====================origin box=========================================");

		//for (const auto& aabb : m_aabbs)
		//{
		//	LOG_TRACE("min:{0},{1},{2}     max:{3},{4},{5}",
		//		std::to_string(aabb.min.x), std::to_string(aabb.min.y), std::to_string(aabb.min.z),
		//		std::to_string(aabb.max.x), std::to_string(aabb.max.y), std::to_string(aabb.max.z));
		//}


		//LOG_TRACE("=====================bvh box========================================");

		//for (const auto& aabbnode : aabbnodes)
		//{
		//	LOG_TRACE("min:{0},{1},{2}     max:{3},{4},{5}",
		//		std::to_string(aabbnode.center[0] - aabbnode.halfDim[0]), std::to_string(aabbnode.center[1] - aabbnode.halfDim[1]), std::to_string(aabbnode.center[2] - aabbnode.halfDim[2]),
		//		std::to_string(aabbnode.center[0] + aabbnode.halfDim[0]), std::to_string(aabbnode.center[1] + aabbnode.halfDim[1]), std::to_string(aabbnode.center[2] + aabbnode.halfDim[2]));
		//}

		LOG_TRACE("=====================copy structure count========================================");
		unsigned int copytStructureCount = 0;
		data.reset(new Blob(sizeof(unsigned int)));
		if (m_applyBvhComponent2->m_cullElementCount->getBuffer(data))
		{
			memcpy_s(&copytStructureCount, sizeof(unsigned int), data->getData(), sizeof(unsigned int));
		}
		LOG_TRACE("CopytStructureCount:{0}", std::to_string(copytStructureCount));

		//LOG_TRACE("=====================cull elment========================================");
		//std::vector<unsigned int> cullElements;
		//cullElements.resize(numElements);
		//data.reset(new Blob(numElements * sizeof(unsigned int)));
		//if (m_applyBvhComponent2->m_cullElement->getBuffer(data))
		//{
		//	memcpy_s(cullElements.data(), numElements * sizeof(unsigned int), data->getData(), numElements * sizeof(unsigned int));
		//}

		//std::string resStr;
		//for (unsigned int i = 0; i < copytStructureCount; i++)
		//{
		//	resStr += std::to_string(cullElements[i]);
		//	if (i != copytStructureCount - 1)
		//	{
		//		resStr += ",";
		//	}
		//}

		//LOG_TRACE(resStr);
	}

	void GpuBvh2BuilderComponent::onAddToNode()
	{
		auto node = std::make_shared<Node>();
		node->set_name("Debug_Apply");
		node->addToParent(m_node.lock());
		//for (const auto& boxVisualizationComponent : m_boxVisualizationComponents)
		//{
		//	node->addComponent(boxVisualizationComponent);
		//}
		//node->addComponent(m_applyBvhComponent);
		node->addComponent(m_applyBvhComponent2);
	}

	void GpuBvh2BuilderComponent::onLeaveScene()
	{
		m_sceneAABBCalculatorComponent->onLeaveScene();
		m_mortonCodesCalculatorComponent->onLeaveScene();
		m_bitonicSortComponent->onLeaveScene();
		m_constructHierarchyComponent->onLeaveScene();
		m_constructAABBPassComponent->onLeaveScene();
		m_applyBvhComponent->onLeaveScene();
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<GpuBvh2BuilderComponent>("GpuBvh2BuilderComponent")
			.constructor<>()
			.property("test", &GpuBvh2BuilderComponent::get_test, &GpuBvh2BuilderComponent::set_test);
	}
}