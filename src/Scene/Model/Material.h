#pragma once
#include <DirectXMath.h>
#include <memory>

namespace Destiny
{
	class Texture;
	class Material
	{
	public:
		Material();
	public:
		unsigned int c_has_c_ambient;
		unsigned int c_has_c_diffuse;
		unsigned int c_has_t_ambient;
		unsigned int c_has_t_diffuse;

		DirectX::XMFLOAT4			c_ambient;
		DirectX::XMFLOAT4			c_diffuse;

		std::shared_ptr<Texture>	t_ambient;
		std::shared_ptr<Texture>	t_diffuse;
	};
}