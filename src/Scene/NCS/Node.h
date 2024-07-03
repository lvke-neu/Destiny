#pragma once
#include "Engine/Object.h"
#include <string>
#include <memory>
#include <vector>

namespace Destiny
{
	class Component;
	class Node : public Object, public std::enable_shared_from_this<Node>
	{
		RTTR_ENABLE(Object);
	public:
		Node();
	public:
		void addToParent(std::shared_ptr<Node> parent);
		void removeFromParent();
		void addComponent(std::shared_ptr<Component> component);
		void removeComponent(std::shared_ptr<Component> component);
	public:
		GET_CLASS_NAME(Node);
		GET_SET(std::string, name);
	private:
		std::string m_name;
		std::shared_ptr<Node> m_parent;
		std::vector<std::shared_ptr<Node>> m_childs;
		std::vector<std::shared_ptr<Component>> m_components;
	};
}
