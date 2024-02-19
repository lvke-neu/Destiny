#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class PBRSphereComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		PBRSphereComponent();
	public:
		GET_CLASS_NAME(SpherePBRSphereComponentComponent);
	};
}