#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class InstancedComponet : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		InstancedComponet();
		virtual ~InstancedComponet() = default;
	};
}