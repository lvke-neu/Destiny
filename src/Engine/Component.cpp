#include "Component.h"

namespace Destiny
{
	Component::Component() :
		m_enable(true),
		m_node(nullptr),
		m_scene(nullptr)
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Component>("Component")
			.constructor<>();
	}
}