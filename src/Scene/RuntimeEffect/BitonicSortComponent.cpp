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
#include <numeric> 

namespace Destiny
{
	BitonicSortComponent::BitonicSortComponent() : 
		m_elementCount(16),
		m_cIndirectArgStride(12),
		m_indirectArgsTexture(nullptr),
		m_indirectArgsCS(nullptr),
		m_preSortCS(nullptr)
	{
		generateFakeData();

		indirectArgsCS();
		preSortCS();

		setCSConstants();
	}

	void BitonicSortComponent::set_test(Button test)
	{
		std::vector<unsigned int> fakeSortData;
		std::vector<unsigned int> fakeIndexData;
		fakeSortData.resize(m_elementCount);
		fakeIndexData.resize(m_elementCount);

		std::shared_ptr<Blob> data = std::make_shared<Blob>(m_elementCount * sizeof(uint32_t));
		if (m_sortTexture->getBuffer(data))
		{
			memcpy_s(fakeSortData.data(), data->getLength(), data->getData(), data->getLength());
		}
		
		data.reset(new Blob(m_elementCount * sizeof(uint32_t)));
		if (m_indexTexture->getBuffer(data))
		{
			memcpy_s(fakeIndexData.data(), data->getLength(), data->getData(), data->getLength());
		}
	}

	void BitonicSortComponent::generateFakeData()
	{
		//m_fakeSortData.resize(m_elementCount);
		m_fakeSortData = { 35, 72, 18, 91, 4, 56, 83, 29, 67, 11, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
		m_fakeIndexData.resize(m_elementCount);

		//Math::RandomUint(0, ElementCount, m_fakeSortData);
		//std::iota(m_fakeSortData.begin(), m_fakeSortData.end(), 0);

		std::iota(m_fakeIndexData.begin(), m_fakeIndexData.end(), 0);

		std::shared_ptr<Blob> data = std::make_shared<Blob>(m_elementCount * sizeof(uint32_t));
		data->copyfrom(m_fakeSortData.data(), data->getLength());
		m_sortTexture = Texture::CreateRaw((unsigned int)data->getLength(), data);
		m_sortTexture->load();

		data.reset(new Blob(m_elementCount * sizeof(uint32_t)));
		data->copyfrom(m_fakeIndexData.data(), data->getLength());
		m_indexTexture = Texture::CreateRaw((unsigned int)data->getLength(), data);
		m_indexTexture->load();
	}

	void BitonicSortComponent::indirectArgsCS()
	{
		m_indirectArgsTexture = Texture::CreateRaw(m_cIndirectArgStride * 22 * 23 / 2, nullptr);
		m_indirectArgsTexture->load();

		std::vector<std::shared_ptr<Texture>> uavs = { m_indirectArgsTexture };
		m_indirectArgsCS = std::make_shared<ComputerCommand>();
		m_indirectArgsCS->setDebugName(L"IndirectArgsCS");
		m_indirectArgsCS->setComputerEffectPath("builtin://renderer/bitonic_sort/IndirectArgsCS.hlsl");
		m_indirectArgsCS->setUnorderedAccessViews(uavs);
		m_indirectArgsCS->setThreadGroupCount(1, 1, 1);

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(m_indirectArgsCS);
	}

	void BitonicSortComponent::preSortCS()
	{
		std::vector<std::shared_ptr<Texture>> uavs = { m_sortTexture, m_indexTexture };
		
		m_preSortCS = std::make_shared<ComputerCommand>();
		m_preSortCS->setDebugName(L"PreSortCS");
		m_preSortCS->setComputerEffectPath("builtin://renderer/bitonic_sort/PreSortCS.hlsl");
		m_preSortCS->setUnorderedAccessViews(uavs);
		m_preSortCS->setIndirectMode(true, m_indirectArgsTexture, { 0 });

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(m_preSortCS);
	}

	void BitonicSortComponent::setCSConstants()
	{
		//constant
		const uint32_t ElementCount = (unsigned int)m_fakeSortData.size();
		const uint32_t AlignedNumElements = Math::AlignPowerOfTwo(ElementCount);
		const uint32_t MaxIterations = Math::Log2(std::max(2048u, AlignedNumElements)) - 10;

		bool SortAscending = true;
		const uint32_t NullItem = SortAscending ? 0xffffffff : 0;
		const uint32_t ListCount = ElementCount;

		m_indirectArgsCS->setConstant("MaxIterations", MaxIterations);
		m_indirectArgsCS->setConstant("NullItem", NullItem);
		m_indirectArgsCS->setConstant("ListCount", ListCount);

		m_preSortCS->setConstant("NullItem", NullItem);
		m_preSortCS->setConstant("ListCount", ListCount);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BitonicSortComponent>("BitonicSortComponent")
			.constructor<>()
			.property("test", &BitonicSortComponent::get_test, &BitonicSortComponent::set_test);
	}
}