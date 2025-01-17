#include "SkyboxComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Texture.h"

namespace Destiny
{
	SkyboxComponent::SkyboxComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/skybox.hlsl");
		renderer->load(0);

		auto sampler = std::make_shared<SamplerState>();
		sampler->load();
		auto texture = Texture::Create("builtin://texture/skybox/daylight.dds");
		texture->load();

		renderer->setShaderResource("t_cube", texture);
		renderer->setSamplerSate("s_sampler", sampler);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE;
		renderStates->getDepthStencilStateDesc()->DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Gui);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Box_Position();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<SkyboxComponent>("SkyboxComponent")
			.constructor<>();
	}
}