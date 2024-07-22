#pragma once
#include "Transform.h"
#include "Engine/Object.h"
#include <memory>

namespace Destiny
{
	class Scene;
	class Node;
	class Component : public Object
	{
		friend class Node;
		RTTR_ENABLE(Object);
	public:
		Component();
		virtual ~Component() = default;
	public:
		GET_CLASS_NAME(Component);
	public:
		virtual void onAddToNode(std::shared_ptr<Node> node);
		virtual void onNodeTransformChanged(const Transform& transform);
		virtual void onEnterScene(std::shared_ptr<Scene> scene);
	protected:
		std::shared_ptr<Node> m_node;
		std::shared_ptr<Scene> m_scene;
	};
}