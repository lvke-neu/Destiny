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

namespace Destiny
{
	BitonicSortComponent::BitonicSortComponent() : 
		m_cIndirectArgStride(12),
		m_indirectArgsTexture(nullptr)
	{
		indirectArgsCS();
	}

	void BitonicSortComponent::indirectArgsCS()
	{
		m_indirectArgsTexture = Texture::CreateRaw(m_cIndirectArgStride * 22 * 23 / 2, nullptr);
		m_indirectArgsTexture->load();

		std::vector<std::shared_ptr<Texture>> uavs = { m_indirectArgsTexture };
		auto IndirectArgsCS = std::make_shared<ComputerCommand>();
		IndirectArgsCS->setDebugName(L"IndirectArgsCS");
		IndirectArgsCS->setComputerEffectPath("builtin://renderer/bitonic_sort/IndirectArgsCS.hlsl");

		IndirectArgsCS->setUnorderedAccessViews(uavs);
		IndirectArgsCS->setThreadGroupCount(1, 1, 1);

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(IndirectArgsCS);

		auto IndirectArgsCS2 = std::make_shared<ComputerCommand>();
		IndirectArgsCS2->setDebugName(L"IndirectArgsCS2");
		IndirectArgsCS2->setComputerEffectPath("builtin://renderer/bitonic_sort/IndirectArgsCS2.hlsl");
		IndirectArgsCS2->setIndirectMode(true, m_indirectArgsTexture, { 0 });

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(IndirectArgsCS2);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BitonicSortComponent>("BitonicSortComponent")
			.constructor<>();
	}
}