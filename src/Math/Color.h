#pragma once
#include "Engine/Utility.h"
#include <DirectXMath.h>

namespace Destiny
{
	// 0 - 255
	class Color
	{
	public:
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	public:
		GET_SET(unsigned char, r);
		GET_SET(unsigned char, g);
		GET_SET(unsigned char, b);
		GET_SET(unsigned char, a);
	public:
		DirectX::XMFLOAT4 toColor32();
	private:
		unsigned char m_r;
		unsigned char m_g;
		unsigned char m_b;
		unsigned char m_a;
	};
}