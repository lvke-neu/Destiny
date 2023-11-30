#pragma once
#include "Engine/Reflection.h"

namespace Destiny
{
	class Color : public Reflection
	{
		RTTR_ENABLE(Reflection);
	public:
		Color();
		Color(float r, float g, float b, float a);
	public:
		float* toFloat()
		{
			return &m_r;
		}
	public:
		GET_CLASS_NAME(Color);
		GET_SET(float, r);
		GET_SET(float, g);
		GET_SET(float, b);
		GET_SET(float, a);
	public:
		static Color White;
		static Color Black;
		static Color Red;
		static Color Green;
		static Color Blue;
	private:
		float m_r;
		float m_g;
		float m_b;
		float m_a;
	};
}