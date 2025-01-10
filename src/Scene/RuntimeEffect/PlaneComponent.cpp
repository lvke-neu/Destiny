#include "PlaneComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include <d3d11.h>

namespace Destiny
{
	PlaneComponent::PlaneComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/pbr.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE;
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setMaterial(PbrMaterial::Create_Rusted_Iron());
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);
		

		auto mesh = MeshProvider::Create_Plane_PositionNormalTexcoord();
		mesh->load();


		setRenderPass(renderPass);
		setMesh(mesh);
	}
}