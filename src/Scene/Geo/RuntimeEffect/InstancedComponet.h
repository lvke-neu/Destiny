#pragma once
#include "Scene/NCS/VisualComponent.h"

namespace Destiny
{
	class InstancedComponet : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		InstancedComponet();
		virtual ~InstancedComponet() = default;
	public:
		GET_CLASS_NAME(InstancedComponet);
	};
}