#include "TestComputerComponent2.h"
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
	TestComputerComponent2::TestComputerComponent2()
	{
		struct ClusterBound
		{
			unsigned int x;
			unsigned int y;
			unsigned int z;
			unsigned int w;
		};

		std::vector<ClusterBound> clusterBounds;
		for (unsigned int i = 0; i < 256; i++)
		{
			clusterBounds.push_back({ i, i, i, i });
		}
		
		std::shared_ptr<Blob> data = std::make_shared<Blob>(sizeof(ClusterBound) * clusterBounds.size());
		data->copyfrom(clusterBounds.data(), data->getLength());

		auto clusterBoundsIn = Texture::CreateStructured(sizeof(ClusterBound), sizeof(ClusterBound) * (unsigned int)clusterBounds.size(), data);
		clusterBoundsIn->load();

		auto clusterBoundsOut = Texture::CreateStructured(sizeof(ClusterBound), sizeof(ClusterBound) * (unsigned int)clusterBounds.size());
		clusterBoundsOut->load();

		std::vector<std::shared_ptr<Texture>> uavs = { clusterBoundsIn,  clusterBoundsOut };
		auto computerCommand = std::make_shared<ComputerCommand>();
		computerCommand->setDebugName(L"test computer shader2");
		computerCommand->setComputerEffectPath("builtin://renderer/computer_shader2.hlsl");

		computerCommand->setUnorderedAccessViews(uavs);
		computerCommand->setThreadGroupCount(1, 1, 1);

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(computerCommand);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TestComputerComponent2>("TestComputerComponent2")
			.constructor<>();
	}
}