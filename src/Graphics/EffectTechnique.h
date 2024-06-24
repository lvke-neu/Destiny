#pragma once
#include <unordered_set>
#include <memory>

namespace Destiny
{
	class EffectPass;
	class EffectTechnique
	{
	public:
		void addEffectPass(std::shared_ptr<EffectPass> effectPass);
		const std::unordered_set<std::shared_ptr<EffectPass>>& getEffectPasses() const;
	private:
		std::unordered_set<std::shared_ptr<EffectPass>> m_effectPasses;
	};

	inline const std::unordered_set<std::shared_ptr<EffectPass>>& EffectTechnique::getEffectPasses() const
	{
		return m_effectPasses;
	}
}