#include "BitonicSortComponent.h"
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
#include "BitonicOuterAndInnerCommand.h"
#include <numeric> 

namespace Destiny
{
	BitonicSortComponent::BitonicSortComponent() : 
		m_elementCount(8192),
		m_cIndirectArgStride(12),
		m_alignedNumElements(0),
		m_indirectArgsTexture(nullptr),
		m_sortTexture(nullptr),
		m_indexTexture(nullptr),
		m_indirectArgsCS(nullptr),
		m_preSortCS(nullptr),
		m_outerSortCS(nullptr),
		m_innerSortCS(nullptr),
		m_bitonicOuterAndInnerCommand(nullptr),
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>())
	{
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"BitonicSort", m_graphicsCommandList);
	}

	void BitonicSortComponent::set_test(Button test)
	{
		//std::vector<unsigned int> fakeSortData;
		//std::vector<unsigned int> fakeIndexData;
		//fakeSortData.resize(m_elementCount);
		//fakeIndexData.resize(m_elementCount);

		//std::shared_ptr<Blob> data = std::make_shared<Blob>(m_elementCount * sizeof(uint32_t));
		//if (m_sortTexture->getBuffer(data))
		//{
		//	memcpy_s(fakeSortData.data(), data->getLength(), data->getData(), data->getLength());
		//}
		//
		//std::vector<unsigned int> tmpFakeSortData = m_fakeSortData;
		//std::sort(tmpFakeSortData.begin(), tmpFakeSortData.end());

		//bool isSame = !memcmp(fakeSortData.data(), tmpFakeSortData.data(),
		//	sizeof(uint32_t) * fakeSortData.size());

		//LOG_WARN("sort count:{0}, gpu sort is the same as std::sort: {1}", m_elementCount, std::string(isSame ? "true" : "false"));

		//data.reset(new Blob(m_elementCount * sizeof(uint32_t)));
		//if (m_indexTexture->getBuffer(data))
		//{
		//	memcpy_s(fakeIndexData.data(), data->getLength(), data->getData(), data->getLength());
		//}
	}

	void BitonicSortComponent::init(unsigned int elementCount, std::shared_ptr<Texture> sortTexture, std::shared_ptr<Texture> indexTexture)
	{
		m_elementCount = elementCount;

		m_sortTexture = sortTexture;
		m_indexTexture = indexTexture;

		indirectArgsCS();
		preSortCS();
		outerSortCS();
		innerSortCS();
		setCSConstants();
		bitonicOuterAndInnerCommand();
	}

	void BitonicSortComponent::onLeaveScene()
	{
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->removeBeforePipelineCommandList(L"BitonicSort", m_graphicsCommandList);
	}

	void BitonicSortComponent::indirectArgsCS()
	{
		m_indirectArgsTexture = Texture::CreateRaw(m_cIndirectArgStride * 22 * 23 / 2, nullptr);
		m_indirectArgsTexture->load();

		std::vector<std::shared_ptr<Texture>> uavs = { m_indirectArgsTexture };
		m_indirectArgsCS = std::make_shared<ComputerCommand>();
		m_indirectArgsCS->setDebugName(L"IndirectArgsCS");
		m_indirectArgsCS->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/bitonic_sort/IndirectArgsCS.hlsl");
		m_indirectArgsCS->setUnorderedAccessViews(uavs, {0});
		m_indirectArgsCS->setThreadGroupCount(1, 1, 1);

		m_graphicsCommandList->addGraphicsCommand(m_indirectArgsCS);
	}

	void BitonicSortComponent::preSortCS()
	{
		std::vector<std::shared_ptr<Texture>> uavs = { m_sortTexture, m_indexTexture };
		
		m_preSortCS = std::make_shared<ComputerCommand>();
		m_preSortCS->setDebugName(L"PreSortCS");
		m_preSortCS->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/bitonic_sort/PreSortCS.hlsl");
		m_preSortCS->setUnorderedAccessViews(uavs, {0,0});
		m_preSortCS->setIndirectMode(true, m_indirectArgsTexture, { 0 });

		m_graphicsCommandList->addGraphicsCommand(m_preSortCS);
	}

	void BitonicSortComponent::outerSortCS()
	{
		std::vector<std::shared_ptr<Texture>> uavs = { m_sortTexture, m_indexTexture };

		m_outerSortCS = std::make_shared<ComputerCommand>();
		m_outerSortCS->setDebugName(L"OuterSortCS");
		m_outerSortCS->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/bitonic_sort/OuterSortCS.hlsl");
		m_outerSortCS->setUnorderedAccessViews(uavs, {0,0});
	}

	void BitonicSortComponent::innerSortCS()
	{
		std::vector<std::shared_ptr<Texture>> uavs = { m_sortTexture, m_indexTexture };

		m_innerSortCS = std::make_shared<ComputerCommand>();
		m_innerSortCS->setDebugName(L"InnerSort");
		m_innerSortCS->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/bitonic_sort/InnerSortCS.hlsl");
		m_innerSortCS->setUnorderedAccessViews(uavs, {0,0});
	}

	void BitonicSortComponent::bitonicOuterAndInnerCommand()
	{
		m_bitonicOuterAndInnerCommand = std::make_shared<BitonicOuterAndInnerCommand>();
		m_bitonicOuterAndInnerCommand->setParam(m_cIndirectArgStride, m_alignedNumElements, m_outerSortCS, m_innerSortCS, m_indirectArgsTexture);
		m_graphicsCommandList->addGraphicsCommand(m_bitonicOuterAndInnerCommand);
	}

	void BitonicSortComponent::setCSConstants()
	{
		//constant
		const uint32_t ElementCount = m_elementCount;
		const uint32_t AlignedNumElements = Math::AlignPowerOfTwo(ElementCount);
		const uint32_t MaxIterations = Math::Log2(std::max(2048u, AlignedNumElements)) - 10;

		m_alignedNumElements = AlignedNumElements;

		bool SortAscending = true;
		const uint32_t NullItem = SortAscending ? 0xffffffff : 0;
		const uint32_t ListCount = ElementCount;

		m_indirectArgsCS->setConstant("MaxIterations", MaxIterations);
		m_indirectArgsCS->setConstant("NullItem", NullItem);
		m_indirectArgsCS->setConstant("ListCount", ListCount);

		m_preSortCS->setConstant("NullItem", NullItem);
		m_preSortCS->setConstant("ListCount", ListCount);

		m_outerSortCS->setConstant("NullItem", NullItem);
		m_outerSortCS->setConstant("ListCount", ListCount);

		m_innerSortCS->setConstant("NullItem", NullItem);
		m_innerSortCS->setConstant("ListCount", ListCount);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BitonicSortComponent>("BitonicSortComponent")
			.constructor<>()
			.property("test", &BitonicSortComponent::get_test, &BitonicSortComponent::set_test);
	}
}