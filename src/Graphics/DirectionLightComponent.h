#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class DirectionLightComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		virtual ~DirectionLightComponent() = default;
	public:
		virtual void		onEnterScene() override;
		virtual void		onNodeTransformChanged() override;
	private:
		void				traversalLightDirectionChanged(std::shared_ptr<Node> node);
	};
}