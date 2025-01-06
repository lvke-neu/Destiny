#include "BoxComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"

namespace Destiny
{
	BoxComponent::BoxComponent() :
		m_color({ 1.0f, 1.0f, 1.0f, 1.0f })
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/basic.hlsl");
		renderer->load(0);
		renderer->setConstant("u_color", m_color);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RenderPass::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Box_PositionNormalTexcoord();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	void BoxComponent::set_color(Color32 color)
	{
		auto visual = getVisual();
		if (visual)
		{
			m_color = color;
			visual->setConstant("u_color", m_color);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BoxComponent>("BoxComponent")
			.constructor<>()
			.property("color", &BoxComponent::get_color, &BoxComponent::set_color);
	}
}