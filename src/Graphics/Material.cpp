#include "Material.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include "Texture.h"

namespace Destiny
{
	Material::Material() :
		m_ambientColor(Color::White),
		m_diffuseColor(Color::White),
		m_specularColor(Color::White),
		m_ambientTexturePath(""),
		m_diffuseTexturePath(""),
		m_specularTexturePath(""),
		m_ambientTexture(nullptr),
		m_diffuseTexture(nullptr),
		m_specularTexture(nullptr),
		m_ambientSamplerState(nullptr),
		m_diffuseSamplerState(nullptr),
		m_specularSamplerState(nullptr)
	{

	}

	Material::~Material()
	{

	}

	void Material::set_ambientTexturePath(std::string ambientTexturePath)
	{
		m_ambientTexturePath = ambientTexturePath;
		m_ambientTexture.reset();
		m_ambientTexture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_ambientTexturePath.c_str());
		m_ambientTexture->load();
	}

	void Material::set_diffuseTexturePath(std::string diffuseTexturePath)
	{
		m_diffuseTexturePath = diffuseTexturePath;
		m_diffuseTexture.reset();
		m_diffuseTexture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_diffuseTexturePath.c_str());
		m_diffuseTexture->load();
	}

	void Material::set_specularTexturePath(std::string specularTexturePath)
	{
		m_specularTexturePath = specularTexturePath;
		m_specularTexture.reset();
		m_specularTexture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_specularTexturePath.c_str());
		m_specularTexture->load();
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Material>("Material")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
				.property("ambientColor", &Material::get_ambientColor, &Material::set_ambientColor)
				.property("diffuseColor", &Material::get_diffuseColor, &Material::set_diffuseColor)
				.property("specularColor", &Material::get_specularColor, &Material::set_specularColor)
				.property("ambientTexturePath", &Material::get_ambientTexturePath, &Material::set_ambientTexturePath)
				.property("diffuseTexturePath", &Material::get_diffuseTexturePath, &Material::set_diffuseTexturePath)
				.property("specularTexturePath", &Material::get_specularTexturePath, &Material::set_specularTexturePath);

	}
}