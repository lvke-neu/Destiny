#include "Material.h"

namespace Destiny
{
	Material::Material() :
		m_visual(nullptr)
	{

	}

	void Material::bind(std::shared_ptr<Visual> visual)
	{
		m_visual = visual;
		update();
	}


	RTTR_REGISTRATION
	{
		rttr::registration::class_<Material>("Material")
			.constructor<>();
	}
}