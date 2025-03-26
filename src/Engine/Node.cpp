#include "Node.h"
#include "Scene.h"
#include "Component.h"

namespace Destiny
{
	Node::Node() :
		m_name("Default Name"),
		m_parent(nullptr)
	{

	}

	Node::Node(const std::string& name) :
		m_name(name),
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

		onEnterScene();
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
		component->m_node = shared_from_this();
		component->onAddToNode();
		
		m_components.emplace_back(component);

		onEnterScene();
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

		onLeaveScene();
		m_components.erase(iter);
	}

	void Node::set_transform(Transform transform)
	{
		m_transform = transform;
		onNodeTransformChanged();
	}

	void Node::moveZAxis(float distance)
	{
		m_transform.moveZAxis(distance);
		onNodeTransformChanged();
	}

	void Node::moveXAxis(float distance)
	{
		m_transform.moveXAxis(distance);
		onNodeTransformChanged();
	}

	void Node::rotateXAxis(float angle)
	{
		m_transform.rotateXAxis(angle);
		onNodeTransformChanged();
	}

	void Node::rotateYAxis(float angle)
	{
		m_transform.rotateYAxis(angle);
		onNodeTransformChanged();
	}

	DirectX::XMMATRIX Node::getRootToThisWorldMatrix()
	{
		auto worldMatrix = m_transform.getWorldMatrix();
		auto tmpNode = m_parent;
		while (tmpNode)
		{
			worldMatrix *= tmpNode->m_transform.getWorldMatrix();
			tmpNode = tmpNode->getParent();
		}

		return worldMatrix;
	}

	void Node::onEnterScene()
	{
		auto tmpParent = shared_from_this();
		while (tmpParent)
		{
			auto scene = std::dynamic_pointer_cast<Scene>(tmpParent);
			if (scene)
			{
				for (const auto& component : m_components)
				{
					if (component)
					{
						component->m_scene = scene;
						component->onEnterScene();
					}
				}
				for (const auto& childNode : m_childs)
				{
					if (childNode)
					{
						childNode->onEnterScene();
					}
				}
				break;
			}
			tmpParent = tmpParent->m_parent;
		}
	}

	void Node::onLeaveScene()
	{
		auto tmpParent = shared_from_this();
		while (tmpParent)
		{
			auto scene = std::dynamic_pointer_cast<Scene>(tmpParent);
			if (scene)
			{
				for (const auto& component : m_components)
				{
					if (component)
					{
						component->onLeaveScene();
					}
				}
				for (const auto& childNode : m_childs)
				{
					if (childNode)
					{
						childNode->onEnterScene();
					}
				}
				break;
			}
			tmpParent = tmpParent->m_parent;
		}
	}

	void Node::onNodeTransformChanged()
	{
		for (const auto& component : m_components)
		{
			if (component)
			{
				component->onNodeTransformChanged();
			}
		}

		for (const auto& node : m_childs)
		{
			if (node)
			{
				node->onNodeTransformChanged();
			}
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Node>("Node")
			.constructor<>()
			.property("name", &Node::get_name, &Node::set_name)
			.property("transform", &Node::get_transform, &Node::set_transform);
	}
}
