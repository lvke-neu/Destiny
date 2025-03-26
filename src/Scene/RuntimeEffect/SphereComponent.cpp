#include "SphereComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"

namespace Destiny
{
	SphereComponent::SphereComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/forward_pbr.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Sphere_PositionNormalTexcoord();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
		set_material(PbrMaterial::Create_Rusted_Iron());
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<SphereComponent>("SphereComponent")
			.constructor<>();
	}
}