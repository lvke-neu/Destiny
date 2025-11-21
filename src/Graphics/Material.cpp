#include "Material.h"

namespace Destiny
{
	Material::Material()
	{

	}

	Material::~Material()
	{
		int i = 0;
		i++;
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