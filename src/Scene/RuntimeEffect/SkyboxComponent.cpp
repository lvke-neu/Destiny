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
#include "Graphics/RenderSystem.h"
#include "Graphics/DeferredOpaquePipeline.h"
#include "Engine/Node.h"
#include "Engine/Scene.h"

namespace Destiny
{
	SkyboxComponent::SkyboxComponent() :
		m_texture(""),
		m_exposure(1.0f)
	{
		auto renderer = Renderer::Create("builtin://renderer/skybox.hlsl");
		renderer->load(0);

		auto sampler = std::make_shared<SamplerState>();
		sampler->load();

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

		setSamplerSate("s_sampler", sampler);
		setConstant("exposure", m_exposure);

		m_cubeSampler = std::make_shared<SamplerState>();
		m_cubeSampler->getSamplerDesc()->Filter = D3D11_FILTER_ANISOTROPIC;
		m_cubeSampler->getSamplerDesc()->AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_cubeSampler->getSamplerDesc()->AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_cubeSampler->getSamplerDesc()->AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_cubeSampler->getSamplerDesc()->MaxAnisotropy = 0;
		m_cubeSampler->getSamplerDesc()->ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_cubeSampler->getSamplerDesc()->MinLOD = 0;
		m_cubeSampler->getSamplerDesc()->MaxLOD = D3D11_FLOAT32_MAX;
		m_cubeSampler->getSamplerDesc()->MipLODBias = 0.0f;
		m_cubeSampler->load();
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

		auto pos = texture.find(".hdr");
		if (pos == std::string::npos)
		{
			pos = texture.find("Cube.dds");
		}

		std::shared_ptr<Texture> irradiance = nullptr;
		std::shared_ptr<Texture> prefilter = nullptr;

		if (pos != std::string::npos)
		{
			irradiance = Texture::Create((texture.substr(0, pos) + "Irradiance.dds").c_str());
			prefilter = Texture::Create((texture.substr(0, pos) + "Prefilter.dds").c_str());

			irradiance->load();
			prefilter->load();
		}

		std::shared_ptr<Texture> brdfLUT = Texture::Create("builtin://texture/skybox/hdr/BrdfLUT.dds");
		brdfLUT->load();

		auto pipeLine = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->getDeferredOpaquePipeline();
		std::static_pointer_cast<DeferredOpaquePipeline>(pipeLine)->onRendererTexturesChanged();
		std::static_pointer_cast<DeferredOpaquePipeline>(pipeLine)->onRendererSamplerStatesChanged();
		notifyVisualRendererConstantChanged(m_scene);
		
		Renderer::SetShaderResource("t_irradianceMap", irradiance);
		Renderer::SetShaderResource("t_prefilterMap", prefilter);
		Renderer::SetShaderResource("t_brdfLUT", brdfLUT);
		Renderer::SetSamplerSate("s_cubeSampler", m_cubeSampler);
	}

	void SkyboxComponent::set_exposure(float exposure)
	{
		m_exposure = exposure;
		setConstant("exposure", m_exposure);
	}

	void SkyboxComponent::notifyVisualRendererConstantChanged(std::shared_ptr<Node> node)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
			if (visualComponent)
			{
				visualComponent->onRendererTextureChanged();
				visualComponent->onRendererSamplerStateChanged();
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			notifyVisualRendererConstantChanged(childNode);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<SkyboxComponent>("SkyboxComponent")
			.constructor<>()
			.property("texture", &SkyboxComponent::get_texture, &SkyboxComponent::set_texture)
			.property("exposure", &SkyboxComponent::get_exposure, &SkyboxComponent::set_exposure);
	}
}