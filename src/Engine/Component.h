#pragma once
#include "Engine/Object.h"

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
		GET(bool, enable);
		void set_enable(bool enable);
		GET(std::shared_ptr<Node>, node);
		GET(std::shared_ptr<Scene>, scene);
	public:
		virtual void			onUpdate(float deltaTime) {};
		virtual void			onAddToNode() {};
		virtual void			onNodeTransformChanged() {};
		virtual void			onEnterScene() {};
		virtual void			onPropertyChanged(const std::string& property) {};
	protected:
		std::shared_ptr<Node>	m_node;
		std::shared_ptr<Scene>	m_scene;
		bool					m_enable;
	};
}