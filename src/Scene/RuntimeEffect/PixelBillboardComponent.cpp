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
	PixelBillboardComponent::PixelBillboardComponent() :
		m_size({ 150.0f, 150.0f }),
		m_texturePath("")
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/pixel_billboard.hlsl");
		renderer->load(0);
		renderer->setConstant("c_size", m_size);

		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load();
		renderer->setSamplerSate("s_sampler", samplerState);

		auto renderStates = RenderStates::CreateBlendState();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RenderPass::ForwardTransparent);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Position3();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	void PixelBillboardComponent::set_size(DirectX::XMFLOAT2 size)
	{
		auto visual = getVisual();
		if (visual)
		{
			m_size = size;
			visual->setConstant("c_size", size);
		}
	}

	void PixelBillboardComponent::set_texturePath(std::string texturePath)
	{
		auto visual = getVisual();
		if (visual)
		{
			m_texturePath = texturePath;
			auto texture = Texture::Create(m_texturePath.c_str());
			texture->load();
			visual->setShaderResource("t_texture", texture);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<PixelBillboardComponent>("PixelBillboardComponent")
			.constructor<>()
			.property("size", &PixelBillboardComponent::get_size, &PixelBillboardComponent::set_size)
			.property("texturePath", &PixelBillboardComponent::get_texturePath, &PixelBillboardComponent::set_texturePath);
	}
}