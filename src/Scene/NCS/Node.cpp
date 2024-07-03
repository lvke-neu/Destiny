#include "Node.h"
#include "Component.h"

namespace Destiny
{
	Node::Node() :
		m_name("Default Name"), 
		m_parent(nullptr)
	{

	}

	void Node::addToParent(std::shared_ptr<Node> parent)
	{
		if (!parent || shared_from_this() == parent)
		{
			return;
		}

		auto iter = std::find(parent->m_childs.begin(), parent->m_childs.end(), shared_from_this());
		if (iter != parent->m_childs.end())
		{
			return;
		}

		removeFromParent();
		parent->m_childs.emplace_back(shared_from_this());
		m_parent = parent;
	}

	void Node::removeFromParent()
	{
		if (!m_parent)
		{
			return;
		}

		auto iter = std::find(m_parent->m_childs.begin(), m_parent->m_childs.end(), shared_from_this());
		if (iter != m_parent->m_childs.end())
		{
			m_parent->m_childs.erase(iter);
		}

		m_parent.reset();
	}

	void Node::addComponent(std::shared_ptr<Component> component)
	{
		if (!component)
		{
			return;
		}

		auto iter = std::find(m_components.begin(), m_components.end(), component);
		if (iter != m_components.end())
		{
			return;
		}

		m_components.emplace_back(component);
	}

	void Node::removeComponent(std::shared_ptr<Component> component)
	{
		if (!component)
		{
			return;
		}
		
		auto iter = std::find(m_components.begin(), m_components.end(), component);
		if (iter == m_components.end())
		{
			return;
		}

		m_components.erase(iter);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Node>("Node")
			.constructor<>()
		    .property("name", &Node::get_name, &Node::set_name);
	}
}
