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
#include "Engine/Node.h"

namespace Destiny
{
	SkyboxComponent::SkyboxComponent() :
		m_texture("builtin://texture/skybox/daylight.dds"),
		m_exposure(1.0f)
	{
		auto renderer = Renderer::Create("builtin://renderer/skybox.hlsl");
		renderer->load(0);

		auto sampler = std::make_shared<SamplerState>();
		sampler->load();
		auto texture = Texture::Create(m_texture.c_str());
		texture->load();

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

		setShaderResource("t_cube", texture);
		setSamplerSate("s_sampler", sampler);
		setConstant("exposure", m_exposure);
	}

	void SkyboxComponent::onNodeTransformChanged()
	{
		setConstant("u_worldMatrix", DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity()));
	}

	void SkyboxComponent::set_texture(std::string texture)
	{
		m_texture = texture;
		auto tex = Texture::Create(m_texture.c_str());
		tex->load();
		setShaderResource("t_cube", tex);
	}

	void SkyboxComponent::set_exposure(float exposure)
	{
		m_exposure = exposure;
		setConstant("exposure", m_exposure);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<SkyboxComponent>("SkyboxComponent")
			.constructor<>()
			.property("texture", &SkyboxComponent::get_texture, &SkyboxComponent::set_texture)
			.property("exposure", &SkyboxComponent::get_exposure, &SkyboxComponent::set_exposure);
	}
}