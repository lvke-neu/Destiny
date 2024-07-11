#include "Component.h"

namespace Destiny
{
	Component::Component()
	{

	}

	void Component::onAddToNode(std::shared_ptr<Node> node)
	{

	}

	void Component::onNodeTransformChanged(const Transform& transform)
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Component>("Component")
			.constructor<>();
	}
}