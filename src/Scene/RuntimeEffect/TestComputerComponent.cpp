#include "TestComputerComponent.h"
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
	TestComputerComponent::TestComputerComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/test_computershader.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Box_PositionNormalTexcoord();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);


		auto sampler = std::make_shared<SamplerState>();
		sampler->load();
		setSamplerSate("s_sampler", sampler);

		auto texture0 = Texture::Create("builtin://texture/flare.dds");
		auto texture1 = Texture::Create("builtin://texture/flarealpha.dds");
		texture0->load();
		texture1->load();

		auto uavTexture = Texture::Create2DUAV(DXGI_FORMAT_R8G8B8A8_UNORM, 512, 512);
		uavTexture->load();
		std::vector<std::shared_ptr<Texture>> uavs = { uavTexture };
		auto computerCommand = std::make_shared<ComputerCommand>();
		computerCommand->setDebugName(L"test computer shader");
		computerCommand->setComputerEffectPath("builtin://renderer/computer_shader.hlsl");
		computerCommand->setShaderResourceView("t_texture0", texture0);
		computerCommand->setShaderResourceView("t_texture1", texture1);
		computerCommand->setUnorderedAccessViews(uavs, {0});
		computerCommand->setThreadGroupCount(32, 32, 1);

		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(computerCommand);

		setShaderResource("t_texture", uavTexture);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TestComputerComponent>("TestComputerComponent")
			.constructor<>();
	}
}