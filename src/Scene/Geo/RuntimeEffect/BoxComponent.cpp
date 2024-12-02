#include "BoxComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"

namespace Destiny
{
	BoxComponent::BoxComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/basic.rdr");
		renderer->load(0);
		renderer->setConstant("u_color", DirectX::XMFLOAT4{ 1.0f, 1.0f, 0.0f, 1.0f });

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

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BoxComponent>("BoxComponent")
			.constructor<>();
	}
}