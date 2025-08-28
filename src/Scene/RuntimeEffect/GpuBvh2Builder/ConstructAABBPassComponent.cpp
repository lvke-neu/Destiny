#include "ConstructAABBPassComponent.h"
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
	ConstructAABBPassComponent::ConstructAABBPassComponent() :
		m_prepareForComputeAABBs(nullptr),
		m_computeAABBs(nullptr),
		m_numElements(0),
		m_outputBVH(nullptr),
		m_scratchMemory(nullptr),
		m_childNodesProcessedCounter(nullptr),
		m_hierarchyBuffer(nullptr),
		m_aabbParentBuffer(nullptr),
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>())
	{
		//init({AABB()}, nullptr);

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"ConstructAABBPass", m_graphicsCommandList);
	}

	void ConstructAABBPassComponent::init(const std::vector<AABB>& aabbs, std::shared_ptr<Texture> hierarchyBuffer, std::shared_ptr<Texture> bitonicSortIndicesBuffer)
	{
		m_numElements = (unsigned int)aabbs.size();
		m_hierarchyBuffer = hierarchyBuffer;
		m_bitonicSortIndicesBuffer = bitonicSortIndicesBuffer;

		prepareForComputeAABBsCS();
		computeAABBsCS();
		bindResource(aabbs);
	}

	void ConstructAABBPassComponent::onLeaveScene()
	{
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->removeBeforePipelineCommandList(L"ConstructAABBPass", m_graphicsCommandList);
	}

	void ConstructAABBPassComponent::prepareForComputeAABBsCS()
	{
		m_prepareForComputeAABBs = std::make_shared<ComputerCommand>();
		m_prepareForComputeAABBs->setDebugName(L"PrepareForComputeAABBs");
		m_prepareForComputeAABBs->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_aabb_pass/PrepareForComputeAABBs.hlsl");
		m_graphicsCommandList->addGraphicsCommand(m_prepareForComputeAABBs);
	}

	void ConstructAABBPassComponent::computeAABBsCS()
	{
		m_computeAABBs = std::make_shared<ComputerCommand>();
		m_computeAABBs->setDebugName(L"ComputeAABBsCS");
		m_computeAABBs->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/construct_aabb_pass/ComputeAABBs.hlsl");
		m_graphicsCommandList->addGraphicsCommand(m_computeAABBs);
	}

	static UINT GetNumberOfInternalNodes(UINT numLeaves)
	{
		// A binary tree with N leaves will always have N - 1 internal nodes
		return std::max(0, (INT)(numLeaves - 1));
	}

	void ConstructAABBPassComponent::bindResource(const std::vector<AABB>& aabbs)
	{
		{
			unsigned int totalNumNodes = m_numElements + GetNumberOfInternalNodes(m_numElements);
			auto resultDataMaxSizeInBytes = sizeof(BVHOffsets) + sizeof(AABBNode) * totalNumNodes + sizeof(BVHMetadata) * m_numElements;
			m_outputBVH = Texture::CreateRaw((unsigned int)resultDataMaxSizeInBytes);
			m_outputBVH->load();
		}

#define ALIGN(alignment, num) (((num + alignment - 1) / alignment) * alignment)
#define ALIGN_GPU_VA_OFFSET(num) ALIGN(4, num)
		
		UINT numInternalNodes = GetNumberOfInternalNodes(m_numElements);
		UINT totalNumNodes = m_numElements + numInternalNodes;

		{
			UINT64 sizeNeededForAABBCalculation = 0;
			sizeNeededForAABBCalculation += ALIGN_GPU_VA_OFFSET(sizeof(UINT) * m_numElements);
			m_scratchMemory = Texture::CreateRaw((unsigned int)sizeNeededForAABBCalculation);
			m_scratchMemory->load();
		}
		
		{
			auto sizeNeeded = ALIGN_GPU_VA_OFFSET(sizeof(UINT) * (numInternalNodes));
			m_childNodesProcessedCounter = Texture::CreateRaw((unsigned int)sizeNeeded);
			m_childNodesProcessedCounter->load();
		}

		//{
		//	const UINT64 hierarchySize = ALIGN_GPU_VA_OFFSET(sizeof(HierarchyNode) * totalNumNodes);
		//	m_hierarchyBuffer = Texture::CreateStructured(sizeof(HierarchyNode), (unsigned int)hierarchySize);
		//	m_hierarchyBuffer->load();
		//}

		{
			UINT numberOfAABBs = 2 * m_numElements - 1;
			UINT bufferSizeInBytes = numberOfAABBs * sizeof(unsigned int);
			m_aabbParentBuffer = Texture::CreateStructured(sizeof(unsigned int), bufferSizeInBytes);
			m_aabbParentBuffer->load();
		}


		{
			BoundingBox box;
			std::vector<BoundingBox> boundingBoxs;
			for (const auto& aabb : aabbs)
			{
				box.center.x = (aabb.min.x + aabb.max.x) * 0.5f;
				box.center.y = (aabb.min.y + aabb.max.y) * 0.5f;
				box.center.z = (aabb.min.z + aabb.max.z) * 0.5f;

				box.halfDim.x = aabb.max.x - box.center.x;
				box.halfDim.y = aabb.max.y - box.center.y;
				box.halfDim.z = aabb.max.z - box.center.z;

				boundingBoxs.push_back({ box });
			}

			std::shared_ptr<Blob> data = std::make_shared<Blob>(sizeof(BoundingBox) * (unsigned int)boundingBoxs.size());
			data->copyfrom(boundingBoxs.data(), data->getLength());
			m_boundingBoxBuffer = Texture::CreateStructured(sizeof(BoundingBox), (unsigned int)data->getLength(), data);
			m_boundingBoxBuffer->load();
		}

		std::vector<std::shared_ptr<Texture>> uavs = 
		{ 
			m_outputBVH,  m_scratchMemory, m_childNodesProcessedCounter,
			m_hierarchyBuffer,m_aabbParentBuffer,m_boundingBoxBuffer,m_bitonicSortIndicesBuffer 
		};

		{
			m_prepareForComputeAABBs->setUnorderedAccessViews(uavs);
			m_computeAABBs->setUnorderedAccessViews(uavs);

			bool isEmptyAccelerationStructure = m_numElements == 0;
			unsigned int NumberOfElements = m_numElements;
			bool prepareUpdate = true;
			bool performUpdate = false;
			unsigned int UpdateFlags = ((UINT)prepareUpdate) | (performUpdate << 1);

			m_prepareForComputeAABBs->setConstant("NumberOfElements", NumberOfElements);
			m_prepareForComputeAABBs->setConstant("UpdateFlags", UpdateFlags);

			m_computeAABBs->setConstant("NumberOfElements", NumberOfElements);
			m_computeAABBs->setConstant("UpdateFlags", UpdateFlags);

			// Only given the GPU VA not the resource itself so need to resort to doing an overarching UAV barrier
			const UINT dispatchWidth = isEmptyAccelerationStructure ? 1 : Math::DivideAndRoundUp<UINT>(m_numElements, THREAD_GROUP_1D_WIDTH);
			m_prepareForComputeAABBs->setThreadGroupCount(dispatchWidth, 1, 1);

			if (isEmptyAccelerationStructure) 
				return;

			m_computeAABBs->setThreadGroupCount(dispatchWidth, 1, 1);
		}

		//{


		//	ScratchMemoryPartitions scratchMemoryPartitions = {};
		//	UINT64& totalSize = scratchMemoryPartitions.TotalSize;
		//	UINT numInternalNodes = GetNumberOfInternalNodes(m_numElements);
		//	UINT totalNumNodes = m_numElements + numInternalNodes;

		//	scratchMemoryPartitions.OffsetToSceneAABB = totalSize;
		//	totalSize += ALIGN_GPU_VA_OFFSET(sizeof(AABB));

		//	const UINT sizePerElement = sizeof(Primitive) + sizeof(PrimitiveMetaData);
		//	scratchMemoryPartitions.OffsetToElements = totalSize;
		//	totalSize += ALIGN_GPU_VA_OFFSET(sizePerElement * m_numElements);
		//	
		//	const UINT mortonCodeBufferSize = ALIGN_GPU_VA_OFFSET(sizeof(UINT) * m_numElements);
		//	scratchMemoryPartitions.OffsetToMortonCodes = totalSize;

		//	const UINT indexBufferSize = ALIGN_GPU_VA_OFFSET(sizeof(UINT) * m_numElements);
		//	scratchMemoryPartitions.OffsetToIndexBuffer = scratchMemoryPartitions.OffsetToMortonCodes + indexBufferSize;

		//	{
		//		// The scratch buffer used for calculating AABBs can alias over the MortonCode/IndexBuffer
		//		// because it's calculated before the MortonCode/IndexBuffer are needed. Additionally,
		//		// the AABB buffer used for treelet reordering is done after both stages so it can also alias
		//		scratchMemoryPartitions.OffsetToSceneAABBScratchMemory = scratchMemoryPartitions.OffsetToMortonCodes;
		//		INT64 sizeNeededToCalculateAABB = SceneAABBCalculatorComponent::ScratchBufferSizeNeeded(m_numElements);
		//		//INT64 sizeNeededForTreeletAABBs = TreeletReorder::RequiredSizeForAABBBuffer(numPrimitives);
		//		INT64 sizeNeededByMortonCodeAndIndexBuffer = mortonCodeBufferSize + indexBufferSize;
		//		//UINT64 extraBufferSize = std::max(sizeNeededToCalculateAABB, std::max(sizeNeededForTreeletAABBs, sizeNeededByMortonCodeAndIndexBuffer));
		//		UINT64 extraBufferSize = std::max(sizeNeededToCalculateAABB, sizeNeededByMortonCodeAndIndexBuffer);

		//		totalSize += extraBufferSize;
		//	}

		//	{
		//		UINT64 sizeNeededForAABBCalculation = 0;
		//		scratchMemoryPartitions.OffsetToCalculateAABBDispatchArgs = sizeNeededForAABBCalculation;
		//		sizeNeededForAABBCalculation += ALIGN_GPU_VA_OFFSET(sizeof(UINT) * m_numElements);

		//		scratchMemoryPartitions.OffsetToPerNodeCounter = sizeNeededForAABBCalculation;
		//		sizeNeededForAABBCalculation += ALIGN_GPU_VA_OFFSET(sizeof(UINT) * (numInternalNodes));

		//		totalSize = std::max(sizeNeededForAABBCalculation, totalSize);
		//	}

		//	const UINT64 hierarchySize = ALIGN_GPU_VA_OFFSET(sizeof(HierarchyNode) * totalNumNodes);
		//	scratchMemoryPartitions.OffsetToHierarchy = totalSize;
		//	totalSize += hierarchySize;

		//	//if (SupportsTreeletReordering(level))
		//	//{
		//	//	const UINT baseTreeletsScratchSize = TreeletReorder::RequiredSizeForBaseTreeletBuffers(numPrimitives);
		//	//	scratchMemoryPartitions.OffsetToBaseTreeletsCount = totalSize;
		//	//	totalSize += baseTreeletsScratchSize;
		//	//}

		//	auto ScratchDataSizeInBytes = scratchMemoryPartitions.TotalSize;
		//	m_scratchMemory = Texture::CreateRaw(ScratchDataSizeInBytes);
		//}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ConstructAABBPassComponent>("ConstructAABBPassComponent")
			.constructor<>();
	}
}