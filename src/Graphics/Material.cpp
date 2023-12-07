#include "Material.h"

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
		m_specularSamplerState(nullptr),
		m_useColor(true)
	{

	}

	Material::~Material()
	{

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
				.property("specularTexturePath", &Material::get_specularTexturePath, &Material::set_specularTexturePath)
				.property("useColor", &Material::get_useColor, &Material::set_useColor);

	}
}