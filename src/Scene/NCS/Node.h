#pragma once
#include "Engine/Object.h"
#include "Transform.h"
#include <string>
#include <memory>
#include <vector>

namespace Destiny
{
	class Scene;
	class Component;
	class Node : public Object, public std::enable_shared_from_this<Node>
	{
		RTTR_ENABLE(Object);
	public:
		Node();
		Node(const std::string& name);
		virtual ~Node() = default;
	public:
		void addToParent(std::shared_ptr<Node> parent);
		void removeFromParent();
		void addComponent(std::shared_ptr<Component> component);
		void removeComponent(std::shared_ptr<Component> component);
		const std::vector<std::shared_ptr<Component>>& getComponents();
		const std::vector<std::shared_ptr<Node>>& getChilds();
		void moveZAxis(float distance);
		void moveXAxis(float distance);
		void rotateXAxis(float angle);
		void rotateYAxis(float angle);
	public:
		GET_CLASS_NAME(Node);
		GET_SET(std::string, name);
		GET(Transform, transform);
		void set_transform(const Transform& transform);
	private:
		void onEnterScene();
		void onNodeTransformChanged();
	private:
		std::string m_name;
		std::shared_ptr<Node> m_parent;
		std::vector<std::shared_ptr<Node>> m_childs;
		std::vector<std::shared_ptr<Component>> m_components;
		Transform m_transform;
	};

	inline const std::vector<std::shared_ptr<Component>>& Node::getComponents()
	{
		return m_components;
	}

	inline const std::vector<std::shared_ptr<Node>>& Node::getChilds()
	{
		return m_childs;
	}
}
