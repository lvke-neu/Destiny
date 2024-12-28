#include "PlaneComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"

namespace Destiny
{
	PlaneComponent::PlaneComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/basic.hlsl");
		renderer->load(0);
		renderer->setConstant("u_color", DirectX::XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RenderPass::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Plane_PositionNormalTexcoord();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	void PlaneComponent::set_color(const DirectX::XMFLOAT4& color)
	{
		auto visual = getVisual();
		if (visual)
		{
			visual->setConstant("u_color", color);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<PlaneComponent>("PlaneComponent")
			.constructor<>();
	}
}