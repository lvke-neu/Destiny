#include "CylinderComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"


namespace Destiny
{
	CylinderComponent::CylinderComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/forward_pbr.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Cylinder_PositionNormalTexcoord();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
		set_material(PbrMaterial::Create_Rusted_Iron());


		auto shadowRenderer = Renderer::Create("builtin://renderer/forward_pbr_shadow.hlsl");
		shadowRenderer->load(0);
		auto shadowRenderStates = std::make_shared<RenderStates>();
		shadowRenderStates->load();
		auto shadowRenderPass = std::make_shared<RenderPass>();
		shadowRenderPass->setRenderer(shadowRenderer);
		shadowRenderPass->setRenderStates(shadowRenderStates);

		setShadowRenderPass(shadowRenderPass);
		setShadowMesh(mesh);
	}

	CylinderComponent::~CylinderComponent()
	{

	}

	void CylinderComponent::onUpdate(float deltaTime)
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<CylinderComponent>("CylinderComponent")
			.constructor<>();
	}
}