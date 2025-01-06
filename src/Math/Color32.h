#pragma once
#include "Engine/Utility.h"
#include <DirectXMath.h>

namespace Destiny
{
	struct Pixel
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	// 0 - 255
	class Color32
	{
	public:
		Color32();
		Color32(float r, float g, float b, float a);
	public:
		GET_SET(float, r);
		GET_SET(float, g);
		GET_SET(float, b);
		GET_SET(float, a);
	private:
		float m_r;
		float m_g;
		float m_b;
		float m_a;
	};
}