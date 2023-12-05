#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class SphereComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		SphereComponent();
	public:
		GET_CLASS_NAME(SphereComponent);
	};
}