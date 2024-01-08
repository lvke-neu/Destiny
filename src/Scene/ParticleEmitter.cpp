#include "ParticleEmitter.h"
#include "Graphics/Visual3D.h"

namespace Destiny
{
	ParticleEmitter::ParticleEmitter() : 
		m_visual3D(nullptr)
	{

	}

	ParticleEmitter::~ParticleEmitter()
	{

	}

	void ParticleEmitter::initialize()
	{
		m_visual3D = std::make_shared<Visual3D>();
	}
}