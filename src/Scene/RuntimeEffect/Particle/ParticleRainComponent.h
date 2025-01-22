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
		virtual void onUpdate(float deltaTime) override;
	private:
		void initParticles();
		void updateMesh();
	private:
		std::vector<Particle> m_particles;
	};
}