#include "PlaneComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "../Model/PbrMaterial.h"
#include <d3d11.h>

namespace Destiny
{
	PlaneComponent::PlaneComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/model.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE;
		renderStates->load();

		auto pbrMaterial = std::make_shared<PbrMaterial>();
		pbrMaterial->set_albedo("builtin://texture/pbr/rusted_iron/albedo.png");
		pbrMaterial->set_normal("builtin://texture/pbr/rusted_iron/normal.png");
		pbrMaterial->set_metallic("builtin://texture/pbr/rusted_iron/metallic.png");
		pbrMaterial->set_roughness("builtin://texture/pbr/rusted_iron/roughness.png");
		pbrMaterial->set_ao("builtin://texture/pbr/rusted_iron/ao.png");

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setMaterial(pbrMaterial);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);
		

		auto mesh = MeshProvider::Create_Plane_PositionNormalTexcoord();
		mesh->load();


		setRenderPass(renderPass);
		setMesh(mesh);
	}
}