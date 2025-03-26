#include "PbrMaterial.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Visual.h"
#include "Engine/BlobHolder.h"
#include <d3d11.h>

namespace Destiny
{
	PbrMaterial::PbrMaterial() :
		m_albedo(nullptr),
		m_normal(nullptr),
		m_metallic(nullptr),
		m_roughness(nullptr),
		m_ao(nullptr)
	{
		m_sampler = std::make_shared<SamplerState>();
		m_sampler->getSamplerDesc()->AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_sampler->getSamplerDesc()->AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_sampler->getSamplerDesc()->AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		//m_sampler->getSamplerDesc()->Filter = D3D11_FILTER_ANISOTROPIC;
		m_sampler->load(0);
	}

	std::shared_ptr<PbrMaterial> PbrMaterial::Create_Default()
	{
		auto pbrMaterial = std::make_shared<PbrMaterial>();
		pbrMaterial->set_albedo("builtin://texture/pbr/default/albedo.png");
		pbrMaterial->set_normal("builtin://texture/pbr/default/normal.png");
		pbrMaterial->set_metallic("builtin://texture/pbr/default/metallic.png");
		pbrMaterial->set_roughness("builtin://texture/pbr/default/roughness.png");
		pbrMaterial->set_ao("builtin://texture/pbr/default/ao.png");

		return pbrMaterial;
	}

	std::shared_ptr<PbrMaterial> PbrMaterial::Create_Gold()
	{
		auto pbrMaterial = std::make_shared<PbrMaterial>();
		pbrMaterial->set_albedo("builtin://texture/pbr/gold/albedo.png");
		pbrMaterial->set_normal("builtin://texture/pbr/gold/normal.png");
		pbrMaterial->set_metallic("builtin://texture/pbr/gold/metallic.png");
		pbrMaterial->set_roughness("builtin://texture/pbr/gold/roughness.png");
		pbrMaterial->set_ao("builtin://texture/pbr/gold/ao.png");

		return pbrMaterial;
	}

	std::shared_ptr<PbrMaterial> PbrMaterial::Create_Grass()
	{
		auto pbrMaterial = std::make_shared<PbrMaterial>();
		pbrMaterial->set_albedo("builtin://texture/pbr/grass/albedo.png");
		pbrMaterial->set_normal("builtin://texture/pbr/grass/normal.png");
		pbrMaterial->set_metallic("builtin://texture/pbr/grass/metallic.png");
		pbrMaterial->set_roughness("builtin://texture/pbr/grass/roughness.png");
		pbrMaterial->set_ao("builtin://texture/pbr/grass/ao.png");

		return pbrMaterial;
	}

	std::shared_ptr<PbrMaterial> PbrMaterial::Create_Peel()
	{
		auto pbrMaterial = std::make_shared<PbrMaterial>();
		pbrMaterial->set_albedo("builtin://texture/pbr/peel/albedo.png");
		pbrMaterial->set_normal("builtin://texture/pbr/peel/normal.png");
		pbrMaterial->set_metallic("builtin://texture/pbr/peel/metallic.png");
		pbrMaterial->set_roughness("builtin://texture/pbr/peel/roughness.png");
		pbrMaterial->set_ao("builtin://texture/pbr/peel/ao.png");

		return pbrMaterial;
	}

	std::shared_ptr<PbrMaterial> PbrMaterial::Create_Plastic()
	{
		auto pbrMaterial = std::make_shared<PbrMaterial>();
		pbrMaterial->set_albedo("builtin://texture/pbr/plastic/albedo.png");
		pbrMaterial->set_normal("builtin://texture/pbr/plastic/normal.png");
		pbrMaterial->set_metallic("builtin://texture/pbr/plastic/metallic.png");
		pbrMaterial->set_roughness("builtin://texture/pbr/plastic/roughness.png");
		pbrMaterial->set_ao("builtin://texture/pbr/plastic/ao.png");

		return pbrMaterial;
	}

	std::shared_ptr<PbrMaterial> PbrMaterial::Create_Rusted_Iron()
	{
		auto pbrMaterial = std::make_shared<PbrMaterial>();
		pbrMaterial->set_albedo("builtin://texture/pbr/rusted_iron/albedo.png");
		pbrMaterial->set_normal("builtin://texture/pbr/rusted_iron/normal.png");
		pbrMaterial->set_metallic("builtin://texture/pbr/rusted_iron/metallic.png");
		pbrMaterial->set_roughness("builtin://texture/pbr/rusted_iron/roughness.png");
		pbrMaterial->set_ao("builtin://texture/pbr/rusted_iron/ao.png");

		return pbrMaterial;
	}

	std::shared_ptr<PbrMaterial> PbrMaterial::Create_Wall()
	{
		auto pbrMaterial = std::make_shared<PbrMaterial>();
		pbrMaterial->set_albedo("builtin://texture/pbr/wall/albedo.png");
		pbrMaterial->set_normal("builtin://texture/pbr/wall/normal.png");
		pbrMaterial->set_metallic("builtin://texture/pbr/wall/metallic.png");
		pbrMaterial->set_roughness("builtin://texture/pbr/wall/roughness.png");
		pbrMaterial->set_ao("builtin://texture/pbr/wall/ao.png");

		return pbrMaterial;
	}

	std::string PbrMaterial::get_albedo()
	{
		if (m_albedo && std::dynamic_pointer_cast<BlobHolder>(m_albedo->getCreationParam()))
		{
			return std::dynamic_pointer_cast<BlobHolder>(m_albedo->getCreationParam())->getPath();
		}
		return "";
	}

	void PbrMaterial::set_albedo(std::string albedo)
	{
		m_albedo.reset();
		m_albedo = Texture::Create(albedo.c_str());
		m_albedo->load();
		update();
	}

	std::string PbrMaterial::get_normal()
	{
		if (m_normal && std::dynamic_pointer_cast<BlobHolder>(m_normal->getCreationParam()))
		{
			return std::dynamic_pointer_cast<BlobHolder>(m_normal->getCreationParam())->getPath();
		}
		return "";
	}

	void PbrMaterial::set_normal(std::string normal)
	{
		m_normal.reset();
		m_normal = Texture::Create(normal.c_str());
		m_normal->load();
		update();
	}

	std::string PbrMaterial::get_metallic()
	{
		if (m_metallic && std::dynamic_pointer_cast<BlobHolder>(m_metallic->getCreationParam()))
		{
			return std::dynamic_pointer_cast<BlobHolder>(m_metallic->getCreationParam())->getPath();
		}
		return "";
	}

	void PbrMaterial::set_metallic(std::string metallic)
	{
		m_metallic.reset();
		m_metallic = Texture::Create(metallic.c_str());
		m_metallic->load();
		update();
	}

	std::string PbrMaterial::get_roughness()
	{
		if (m_roughness && std::dynamic_pointer_cast<BlobHolder>(m_roughness->getCreationParam()))
		{
			return std::dynamic_pointer_cast<BlobHolder>(m_roughness->getCreationParam())->getPath();
		}
		return "";
	}

	void PbrMaterial::set_roughness(std::string roughness)
	{
		m_roughness.reset();
		m_roughness = Texture::Create(roughness.c_str());
		m_roughness->load();
		update();
	}

	std::string PbrMaterial::get_ao()
	{
		if (m_ao && std::dynamic_pointer_cast<BlobHolder>(m_ao->getCreationParam()))
		{
			return std::dynamic_pointer_cast<BlobHolder>(m_ao->getCreationParam())->getPath();
		}
		return "";
	}

	void PbrMaterial::set_ao(std::string ao)
	{
		m_ao.reset();
		m_ao = Texture::Create(ao.c_str());
		m_ao->load();
		update();
	}

	void PbrMaterial::update()
	{
		if (!m_visual)
		{
			return;
		}
		m_visual->setShaderResource("t_albedo", m_albedo);
		m_visual->setShaderResource("t_normal", m_normal);
		m_visual->setShaderResource("t_metallic", m_metallic);
		m_visual->setShaderResource("t_roughness", m_roughness);
		m_visual->setShaderResource("t_ao", m_ao);
		m_visual->setSamplerSate("s_sampler", m_sampler);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<PbrMaterial>("PbrMaterial")
			.constructor<>()
			.property("albedo", &PbrMaterial::get_albedo, &PbrMaterial::set_albedo)
			.property("normal", &PbrMaterial::get_normal, &PbrMaterial::set_normal)
			.property("metallic", &PbrMaterial::get_metallic, &PbrMaterial::set_metallic)
			.property("roughness", &PbrMaterial::get_roughness, &PbrMaterial::set_roughness)
			.property("ao", &PbrMaterial::get_ao, &PbrMaterial::set_ao);
	}
}