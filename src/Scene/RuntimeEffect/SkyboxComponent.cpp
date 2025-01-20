#include "SkyboxComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Texture.h"

namespace Destiny
{
	SkyboxComponent::SkyboxComponent() :
		m_texture("builtin://texture/skybox/daylight.dds")
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/skybox.hlsl");
		renderer->load(0);

		auto sampler = std::make_shared<SamplerState>();
		sampler->load();
		auto texture = Texture::Create(m_texture.c_str());
		texture->load();

		renderer->setShaderResource("t_cube", texture);
		renderer->setSamplerSate("s_sampler", sampler);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE;
		renderStates->getDepthStencilStateDesc()->DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Gui);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Box_Position();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	void SkyboxComponent::set_texture(std::string texture)
	{
		auto visual = getVisual();
		if (visual)
		{
			m_texture = texture;
			auto texture = Texture::Create(m_texture.c_str());
			texture->load();
			visual->setShaderResource("t_cube", texture);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<SkyboxComponent>("SkyboxComponent")
			.constructor<>()
			.property("texture", &SkyboxComponent::get_texture, &SkyboxComponent::set_texture);
	}
}