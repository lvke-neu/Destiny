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
}