#pragma once
#include "Particle.h"
#include "Engine/Utility.h"

namespace Destiny
{
	class ParticleEmitter
	{
		SINGLETON(ParticleEmitter);
	private:
		ParticleEmitter() = default;
		~ParticleEmitter() = default;
		ParticleEmitter(const ParticleEmitter&) = default;
		ParticleEmitter& operator=(const ParticleEmitter&) = default;
	public:
		Particle emit();
	};
}