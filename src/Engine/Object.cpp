#include "Object.h"

namespace Destiny
{
	Object::Object() :
		m_uuid(Utility::GenerateUUID()),
		m_serializable(true)
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Object>("Object")
			.constructor<>()
			.property("serializable", &Object::get_serializable, &Object::set_serializable);
	}
}