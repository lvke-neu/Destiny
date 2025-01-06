#include "Color32.h"
#include <rttr/registration.h>

namespace Destiny
{
	Color32::Color32() : m_r(0.0f), m_g(0.0f), m_b(0.0f), m_a(255.0f)
	{

	}

	Color32::Color32(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a)
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Color32>("Color32")
			.constructor<>()
			.property("r", &Color32::get_r, &Color32::set_r)
			.property("g", &Color32::get_g, &Color32::set_g)
			.property("b", &Color32::get_b, &Color32::set_b)
			.property("a", &Color32::get_a, &Color32::set_a);
	}
}