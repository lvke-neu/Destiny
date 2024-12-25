#include "Material.h"

Destiny::Material::Material() :
	c_has_c_ambient(false),
	c_has_c_diffuse(false),
	c_has_c_specular(false),
	c_has_t_ambient(false),
	c_has_t_diffuse(false),
	c_has_t_specular(false),
	c_ambient({ 0.0f, 0.0f, 0.0f, 0.0f }),
	c_diffuse({ 0.0f, 0.0f, 0.0f, 0.0f }),
	c_specular({ 0.0f, 0.0f, 0.0f, 0.0f }),
	t_ambient(nullptr),
	t_diffuse(nullptr),
	t_specular(nullptr)
{

}
