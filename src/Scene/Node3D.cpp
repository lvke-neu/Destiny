#include "Node3D.h"

namespace Destiny
{
	Node3D::Node3D() : 
		m_name(""), 
		m_parent(nullptr)
	{

	}

	Node3D::~Node3D()
	{
		removeFromParent();
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
		parent->m_childs.emplace_back(this);
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
}
