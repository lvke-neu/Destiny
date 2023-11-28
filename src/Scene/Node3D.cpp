#include "Node3D.h"
#include "Component.h"
#include "Engine/Utility.h"

namespace Destiny
{
	Node3D::Node3D() : 
		m_uuid(Utility::GenerateUUID()),
		m_name("Default Name"), 
		m_parent(nullptr),
		m_transform3D(Transform3D())
	{

	}

	void Node3D::addToParent(std::shared_ptr<Node3D> parent)
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

	void Node3D::removeFromParent()
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

	void Node3D::addComponent(std::shared_ptr<Component> component)
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
		component->m_node = shared_from_this();
		component->onAttachNode();
		m_components.emplace_back(component);
	}

	void Node3D::removeComponent(std::shared_ptr<Component> component)
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

		component->m_node.reset();
		m_components.erase(iter);
	}

	void Node3D::moveZAxis(float distance)
	{
		m_transform3D.moveZAxis(distance);
		for (const auto& component : m_components)
		{
			component->onNodeTransformChanged();
		}
	}

	void Node3D::moveXAxis(float distance)
	{
		m_transform3D.moveXAxis(distance);
		for (const auto& component : m_components)
		{
			component->onNodeTransformChanged();
		}
	}

	void Node3D::rotateXAxis(float angle)
	{
		m_transform3D.rotateXAxis(angle);
		for (const auto& component : m_components)
		{
			component->onNodeTransformChanged();
		}
	}

	void Node3D::rotateYAxis(float angle)
	{
		m_transform3D.rotateYAxis(angle);
		for (const auto& component : m_components)
		{
			component->onNodeTransformChanged();
		}
	}

	void Node3D::set_transform3D(Transform3D transform3D)
	{
		m_transform3D = transform3D;
		for (const auto& component : m_components)
		{
			component->onNodeTransformChanged();
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Node3D>("Node3D")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
		    .property("uuid", &Node3D::get_uuid, &Node3D::set_uuid)
		    .property("name", &Node3D::get_name, &Node3D::set_name)
		    .property("transform3D", &Node3D::get_transform3D, &Node3D::set_transform3D);
	}
}
