#pragma once
#include "Graphics/Color.h"
#include <DirectXMath.h>

namespace Destiny
{
	class Particle
	{
	public:
		Particle();
		~Particle();
	public:
		bool m_isLive;
		float m_lifeTime;
		float m_age;
		Color m_color;
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_velocity;
		DirectX::XMFLOAT3 m_initVelocity;
	};
}