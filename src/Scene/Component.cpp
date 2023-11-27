#include "Component.h"

namespace Destiny
{
	Component::Component() :
		m_enable(true),
		m_node(nullptr)
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Component>("Component")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
			.property("enable", &Component::get_enable, &Component::set_enable);
	}
}