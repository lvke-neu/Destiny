#include "Material.h"

namespace Destiny
{
	Material::Material() :
		m_renderer(nullptr)
	{

	}

	void Material::bind(std::shared_ptr<Renderer> renderer)
	{
		m_renderer = renderer;
		update();
	}
}