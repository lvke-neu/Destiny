#include "PixelBillboardComponent.h"
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
	PixelBillboardComponent::PixelBillboardComponent(const char* texturePath)
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/pixel_billboard.hlsl");
		renderer->load(0);
		renderer->setConstant("c_size", DirectX::XMFLOAT2{ 150.0f, 150.0f });

		auto texture = Texture::Create(texturePath);
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
		rttr::registration::class_<PixelBillboardComponent>("PixelBillboardComponent")
			.constructor<>();
	}
}