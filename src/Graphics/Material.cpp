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
		m_specularSamplerState(nullptr)
	{

	}

	Material::~Material()
	{

	}
}