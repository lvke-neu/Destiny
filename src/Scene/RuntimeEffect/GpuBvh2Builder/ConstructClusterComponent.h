#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class ConstructClusterComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		ConstructClusterComponent();
		virtual ~ConstructClusterComponent() = default;
	};
}