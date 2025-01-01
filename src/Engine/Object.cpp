#include "Object.h"

namespace Destiny
{
	Object::Object() : 
		m_uuid(Utility::GenerateUUID())
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Object>("Object")
			.constructor<>();
	}
}