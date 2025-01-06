#include "Color.h"
#include <rttr/registration.h>

namespace Destiny
{
	Color::Color() : m_r(0), m_g(0), m_b(0), m_a(255)
	{

	}

	Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : m_r(r), m_g(g), m_b(b), m_a(a)
	{

	}

	DirectX::XMFLOAT4 Color::toColor32()
	{
		return { m_r / 255.0f, m_g / 255.0f, m_b / 255.0f, m_a / 255.0f };
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Color>("Color")
			.constructor<>()
			.property("r", &Color::get_r, &Color::set_r)
			.property("g", &Color::get_g, &Color::set_g)
			.property("b", &Color::get_b, &Color::set_b)
			.property("a", &Color::get_a, &Color::set_a);
	}
}