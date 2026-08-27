#include "TestGeometryShaderComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"

namespace Destiny
{
	TestGeometryShaderComponent::TestGeometryShaderComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/test_geometryshader.hlsl");
		renderer->load(0);
		

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Triangle_PositionColor();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TestGeometryShaderComponent>("TestGeometryShaderComponent")
			.constructor<>();
	}
}