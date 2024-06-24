#pragma once
#include <unordered_set>
#include <memory>

namespace Destiny
{
	class EffectTechnique;
	class Effect
	{
	public:
		void addEffectTechnique(std::shared_ptr<EffectTechnique> effectTechnique);
		const std::unordered_set<std::shared_ptr<EffectTechnique>>& getEffectTechniques() const;
	private:
		std::unordered_set<std::shared_ptr<EffectTechnique>> m_effectTechniques;
	};

	inline const std::unordered_set<std::shared_ptr<EffectTechnique>>& Effect::getEffectTechniques() const
	{
		return m_effectTechniques;
	}
}