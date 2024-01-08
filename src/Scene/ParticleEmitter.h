#pragma once
#include <memory>

namespace Destiny
{
	class Particle;
	class Visual3D;
	class ParticleEmitter
	{
	public:
		ParticleEmitter();
		~ParticleEmitter();
	public:
		void initialize();
		//void addParticle(std::shared_ptr<Particle> particle);
		//void update(void* data);
		//void render();
		//void resetParticle(std::shared_ptr<Particle> particle);
	private:
		std::shared_ptr<Visual3D> m_visual3D;
	};
}