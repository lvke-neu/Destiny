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
		GET_SET(bool, enable);
	public:
		virtual void onAddToNode();
		virtual void onNodeTransformChanged();
		virtual void onEnterScene();
	protected:
		std::shared_ptr<Node>	m_node;
		std::shared_ptr<Scene>	m_scene;
		bool					m_enable;
	};
}