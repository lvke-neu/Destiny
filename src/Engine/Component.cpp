#include "Component.h"

namespace Destiny
{
	Component::Component() :
		m_enable(true),
		m_node(nullptr),
		m_scene(nullptr),
		m_updateCategory(UpdateCategory::update)
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
			.property("enable", &Component::get_enable, &Component::set_enable)
			.property("updateCategory", &Component::get_updateCategory, &Component::set_updateCategory);

		rttr::registration::enumeration<Component::UpdateCategory>("Component::UpdateCategory")
		(
			rttr::value("update", Component::UpdateCategory::update),
			rttr::value("late_update", Component::UpdateCategory::late_update)
		);
	}
}