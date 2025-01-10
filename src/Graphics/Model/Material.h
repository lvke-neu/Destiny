#pragma once
#include <DirectXMath.h>
#include <memory>

namespace Destiny
{
	class Texture;
	class SamplerState;
	class Material
	{
	public:
		Material();
	public:
		std::shared_ptr<Texture>		t_albedo;
		std::shared_ptr<Texture>		t_normal;
		std::shared_ptr<Texture>		t_metallic;
		std::shared_ptr<Texture>		t_roughness;
		std::shared_ptr<Texture>		t_ao;

		std::shared_ptr<SamplerState>	s_sampler;
	};
}