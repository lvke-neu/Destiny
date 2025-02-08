#pragma once
#include "Graphics/VisualComponent.h"
#include "Particle.h"

namespace Destiny
{
	class ParticleComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		ParticleComponent();
		virtual ~ParticleComponent() = default;
	public:
		virtual void onUpdate(float deltaTime) override;
	private:
		void initParticles();
		void updateParticles(float deltaTime);
		void updateMesh();
	private:
		std::vector<Particle> m_particles;
	};
}