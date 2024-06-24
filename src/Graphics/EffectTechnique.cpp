#include "EffectTechnique.h"

namespace Destiny
{
	void EffectTechnique::addEffectPass(std::shared_ptr<EffectPass> effectPass)
	{
		m_effectPasses.insert(effectPass);
	}
}