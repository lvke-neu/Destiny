#pragma once
#include "Transform.h"
#include "Engine/Object.h"
#include <memory>

namespace Destiny
{
	class Node;
	class Component : public Object
	{
		RTTR_ENABLE(Object);
	public:
		Component();
		virtual ~Component() = default;
	public:
		GET_CLASS_NAME(Component);
	public:
		virtual void onAddToNode(std::shared_ptr<Node> node);
		virtual void onNodeTransformChanged(const Transform& transform);
	};
}