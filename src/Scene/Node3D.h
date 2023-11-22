#pragma once
#include <string>
#include <memory>
#include <vector>
#include <rttr/registration>

namespace Destiny
{
	class Node3D : public std::enable_shared_from_this<Node3D>
	{
		RTTR_ENABLE();
	public:
		Node3D();
		~Node3D();
	public:
		void addToParent(std::shared_ptr<Node3D> parent);
		void removeFromParent();
	public:
		std::string getUuid();
		std::string getName();
		void setName(const std::string& name);
		std::vector<std::shared_ptr<Node3D>> getChilds();
	private:
		std::string m_uuid;
		std::string m_name;
		std::shared_ptr<Node3D> m_parent;
		std::vector<std::shared_ptr<Node3D>> m_childs;
	};

	inline std::string Node3D::getUuid()
	{
		return m_uuid;
	}

	inline std::string Node3D::getName()
	{
		return m_name;
	}

	inline void Node3D::setName(const std::string& name)
	{
		m_name = name;
	}

	inline std::vector<std::shared_ptr<Node3D>> Node3D::getChilds()
	{
		return m_childs;
	}
}
