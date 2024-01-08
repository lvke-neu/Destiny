#include "Particle.h"

namespace Destiny
{

	Particle::Particle() :
		m_isLive(false),
		m_lifeTime(0.0f),
		m_age(0.0f),
		m_color(Color::White),
		m_position({0.0f, 0.0f, 0.0f}),
		m_velocity({0.0f, 0.0f, 0.0f}),
		m_initVelocity({0.0f, 0.0f, 0.0f})
	{

	}

	Particle::~Particle()
	{

	}

}