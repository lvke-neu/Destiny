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
		auto renderer = Renderer::Create("builtin://renderer/text.hlsl");
		renderer->load(0);


		//renderer->setShaderResource("t_texture", FontManager::GetInstance()->getFontTexture('A', 5000, 5000));

		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load();
		

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

		setConstant("c_size", m_size);
		setConstant("c_screenPosition", m_screenPosition);
		setConstant("c_color", m_color);
		setSamplerSate("s_sampler", samplerState);
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
		m_size = size;
		setConstant("c_size", m_size);
	}

	void TextComponent::set_screenPosition(DirectX::XMFLOAT2 screenPosition)
	{
		m_screenPosition = screenPosition;
		setConstant("c_screenPosition", m_screenPosition);
	}

	void TextComponent::set_color(Color color)
	{
		m_color = color;
		setConstant("c_color", m_color);
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