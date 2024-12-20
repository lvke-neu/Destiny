#include "Component.h"

namespace Destiny
{
	Component::Component() :
		m_enable(true)
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Component>("Component")
			.constructor<>();
	}
}