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
	BitonicSortComponent::BitonicSortComponent()
	{
		std::vector<unsigned int> numbers = { 3,2,5,1,7,2,1,9,22,32 };

		std::shared_ptr<Blob> data = std::make_shared<Blob>(sizeof(unsigned int) * numbers.size());
		data->copyfrom(numbers.data(), data->getLength());

		auto sortedBufferIn = Texture::CreateTyped((int)DXGI_FORMAT_R32_UINT, sizeof(unsigned int), (unsigned int)data->getLength(), data);
		sortedBufferIn->load();

		auto sortedBufferOut = Texture::CreateTyped((int)DXGI_FORMAT_R32_UINT, sizeof(unsigned int), (unsigned int)data->getLength());
		sortedBufferOut->load();

		std::vector<std::shared_ptr<Texture>> uavs = { sortedBufferIn, sortedBufferOut };
		auto computerCommand = std::make_shared<ComputerCommand>();
		computerCommand->setDebugName(L"BitonicSort");
		computerCommand->setComputerEffectPath("builtin://renderer/bitonic_sort.hlsl");

		computerCommand->setUnorderedAccessViews(uavs);
		computerCommand->setThreadGroupCount(1, 1, 1);

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(computerCommand);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BitonicSortComponent>("BitonicSortComponent")
			.constructor<>();
	}
}