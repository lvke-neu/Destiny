#include "Component.h"

namespace Destiny
{
	Component::Component()
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Component>("Component")
			.constructor<>();
	}
}