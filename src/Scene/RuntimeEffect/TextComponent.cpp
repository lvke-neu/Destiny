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
		renderer->setConstant("c_size", DirectX::XMFLOAT2{ 10.0f, 20.0f });
		renderer->setConstant("c_screenPosition", DirectX::XMFLOAT2{ 0.0f, 0.0f });
		renderer->setConstant("c_color", DirectX::XMFLOAT3{ 1.0f, 1.0f, 1.0f });

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

		DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::Draw;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::PointList;
		drawCall.indexCount = 0;
		drawCall.vertexCount = 1;

		auto mesh = std::make_shared<Mesh>(aabb, drawCall, nullptr, nullptr);

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	void TextComponent::updateText(const std::string& text)
	{
		if (getVisual())
		{
			getVisual()->setShaderResource("t_texture", FontManager::GetInstance()->getFontTexture(text));
		}
	}

	void TextComponent::updateSize(float size_x, float size_y)
	{
		if (getVisual())
		{
			getVisual()->setConstant("c_size", DirectX::XMFLOAT2{ size_x, size_y });
		}
	}

	void TextComponent::updateScreenPosition(float screen_x, float screen_y)
	{
		if (getVisual())
		{
			getVisual()->setConstant("c_screenPosition", DirectX::XMFLOAT2{ screen_x, screen_y });
		}
	}

	void TextComponent::updateTextColor(float r, float g, float b)
	{
		if (getVisual())
		{
			getVisual()->setConstant("c_color", DirectX::XMFLOAT3{ r, g, b });
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TextComponent>("TextComponent")
			.constructor<>();
	}
}