#pragma once
#include "Graphics/VisualComponent.h"
#include "Particle.h"

namespace Destiny
{
	class ParticleRainComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		ParticleRainComponent();
		virtual ~ParticleRainComponent() = default;
	public:
		GET_SET(float, particleMaxAge);
		GET_SET(float, gravity);
	public:
		virtual void onUpdate(float deltaTime) override;
	private:
		void initParticles();
		void updateParticles(float deltaTime);
		void updateMesh();

		float randomFloat(float min, float max);
	private:
		std::vector<Particle> m_particles;
		float m_particleMaxAge;
		float m_gravity;
	};
}