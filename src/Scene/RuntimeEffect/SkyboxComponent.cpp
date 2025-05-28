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

		auto irradiance = Texture::Create((texture.substr(0, texture.find(".hdr")) + "Irradiance.dds").c_str());
		irradiance->load(); 
		auto prefilter = Texture::Create((texture.substr(0, texture.find(".hdr")) + "Prefilter.dds").c_str());
		prefilter->load(0);
		auto brdfLUT = Texture::Create("builtin://texture/skybox/hdr/BrdfLUT.dds");
		brdfLUT->load(0);

		auto sampler1 = std::make_shared<SamplerState>();
		sampler1->getSamplerDesc()->Filter = D3D11_FILTER_ANISOTROPIC;
		sampler1->getSamplerDesc()->AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler1->getSamplerDesc()->AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler1->getSamplerDesc()->AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler1->getSamplerDesc()->MaxAnisotropy = 0;
		sampler1->getSamplerDesc()->ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler1->getSamplerDesc()->MinLOD = 0;
		sampler1->getSamplerDesc()->MaxLOD = D3D11_FLOAT32_MAX;
		sampler1->getSamplerDesc()->MipLODBias = 0.0f;
		sampler1->load();

		auto sampler2 = std::make_shared<SamplerState>();
		sampler2->load();

		notifyVisualRendererConstantChanged(m_scene);
		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setShaderResource("t_irradianceMap", irradiance);
			renderer.second->setShaderResource("t_prefilterMap", prefilter);
			renderer.second->setShaderResource("t_brdfLUT", brdfLUT);

			renderer.second->setSamplerSate("s_cubeSampler", sampler1);
			//renderer.second->setSamplerSate("s_brdfLUTSampler", sampler2);
		}
		//if (m_texture.find(".hdr") != std::string::npos)
		//{
		//	auto tex2 = Texture::Create((m_texture + "?type=prefilter").c_str());
		//	tex2->load();
		//	setShaderResource("t_cube", tex2);
		//}
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