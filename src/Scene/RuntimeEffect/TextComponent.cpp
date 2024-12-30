#include "TextComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Gui/FontManager.h"
#include <d3d11.h>

namespace Destiny
{
	TextComponent::TextComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/text.hlsl");
		renderer->load(0);
		renderer->setConstant("c_size", DirectX::XMFLOAT2{ 15.0f, 15.0f });

		//renderer->setShaderResource("t_texture", FontManager::GetInstance()->getFontTexture('A', 5000, 5000));

		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load();
		renderer->setSamplerSate("s_sampler", samplerState);

		auto renderStates = RenderStates::CreateBlendState();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RenderPass::ForwardTransparent);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Position2(-0.9f, 0.9f);
		mesh->load(0);

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	void TextComponent::updateText(unsigned char text)
	{
		if (getVisual())
		{
			getVisual()->setShaderResource("t_texture", FontManager::GetInstance()->getFontTexture(text, 500, 500));
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TextComponent>("TextComponent")
			.constructor<>();
	}
}