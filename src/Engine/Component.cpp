#include "Component.h"

namespace Destiny
{
	Component::Component() :
		m_enable(true),
		m_node(nullptr),
		m_scene(nullptr)
	{

	}

	void Component::set_enable(bool enable)
	{
		m_enable = enable;
		onPropertyChanged("enable");
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Component>("Component")
			.constructor<>()
			.property("enable", &Component::get_enable, &Component::set_enable);
	}
}