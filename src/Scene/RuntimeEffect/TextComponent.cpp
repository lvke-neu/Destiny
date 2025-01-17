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
	TextComponent::TextComponent() :
		m_text(""),
		m_size({ 10.0f, 20.0f }),
		m_screenPosition({ 0.0f, 0.0f }),
		m_color({1.0f, 1.0f, 1.0f, 1.0f})
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/text.hlsl");
		renderer->load(0);
		renderer->setConstant("c_size", m_size);
		renderer->setConstant("c_screenPosition", m_screenPosition);
		renderer->setConstant("c_color", m_color);

		//renderer->setShaderResource("t_texture", FontManager::GetInstance()->getFontTexture('A', 5000, 5000));

		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load();
		renderer->setSamplerSate("s_sampler", samplerState);

		auto renderStates = RenderStates::CreateBlendState();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Gui);
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

	void TextComponent::set_text(std::string text)
	{
		if (m_text != text && getVisual())
		{
			m_text = text;
			getVisual()->setShaderResource("t_texture", FontManager::GetInstance()->getFontTexture(text));
		}
	}

	void TextComponent::set_size(DirectX::XMFLOAT2 size)
	{
		if (getVisual())
		{
			m_size = size;
			getVisual()->setConstant("c_size", m_size);
		}
	}

	void TextComponent::set_screenPosition(DirectX::XMFLOAT2 screenPosition)
	{
		if (getVisual())
		{
			m_screenPosition = screenPosition;
			getVisual()->setConstant("c_screenPosition", m_screenPosition);
		}
	}

	void TextComponent::set_color(Color color)
	{
		if (getVisual())
		{
			m_color = color;
			getVisual()->setConstant("c_color", m_color);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TextComponent>("TextComponent")
			.constructor<>()
			.property("text", &TextComponent::get_text, &TextComponent::set_text)
			.property("size", &TextComponent::get_size, &TextComponent::set_size)
			.property("screenPosition", &TextComponent::get_screenPosition, &TextComponent::set_screenPosition)
			.property("color", &TextComponent::get_color, &TextComponent::set_color);
	}
}