#include "ParticleEmitter.h"
#include "Math/Math.h"

namespace Destiny
{
	Particle ParticleEmitter::emit()
	{
		Particle particle;

		particle.position = { 0.0f, 0.0f, 0.0f };
		particle.velocity = { 0.0f, 0.0f, 0.0f };
		particle.age = 0.0f;

		return particle;
	}
}