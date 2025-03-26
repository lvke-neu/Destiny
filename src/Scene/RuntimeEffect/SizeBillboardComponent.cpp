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
		auto renderer = Renderer::Create("builtin://renderer/size_billboard.hlsl");
		renderer->load();

		auto texture = Texture::Create("builtin://texture/wind.png");
		texture->load();
		
		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load();
		
		auto renderStates = RenderStates::CreateBlendState();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Transparent);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Position3();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);

		setConstant("c_size", DirectX::XMFLOAT2{ 1.0f, 1.0f });
		setShaderResource("t_texture", texture);
		setSamplerSate("s_sampler", samplerState);
	}

	void SizeBillboardComponent::set_size(const DirectX::XMFLOAT2& size)
	{
		setConstant("c_size", size);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<SizeBillboardComponent>("SizeBillboardComponent")
			.constructor<>();
	}
}