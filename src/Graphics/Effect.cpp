#include "Effect.h"

namespace Destiny
{
	void Effect::addEffectTechnique(std::shared_ptr<EffectTechnique> effectTechnique)
	{
		m_effectTechniques.insert(effectTechnique);
	}
}