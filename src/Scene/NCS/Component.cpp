#include "Component.h"

namespace Destiny
{
	Component::Component()
	{

	}

	void Component::onAddToNode()
	{

	}

	void Component::onNodeTransformChanged()
	{

	}

	void Component::onEnterScene()
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Component>("Component")
			.constructor<>();
	}
}