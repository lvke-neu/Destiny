#include "SizeBillboardComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include <d3d11.h>

namespace Destiny
{
	SizeBillboardComponent::SizeBillboardComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/size_billboard.hlsl");
		renderer->load(0);
		renderer->setConstant("c_size", DirectX::XMFLOAT2{ 5.0f, 5.0f });

		auto texture = Texture::Create("builtin://texture/wind.png");
		texture->load();
		renderer->setShaderResource("t_texture", texture);

		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load();
		renderer->setSamplerSate("s_sampler", samplerState);

		auto renderStates = RenderStates::CreateBlendState();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RenderPass::ForwardTransparent);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Position();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<SizeBillboardComponent>("SizeBillboardComponent")
			.constructor<>();
	}
}