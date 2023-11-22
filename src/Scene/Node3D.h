#pragma once
#include <string>
#include <memory>
#include <vector>

namespace Destiny
{
	class Node3D : std::enable_shared_from_this<Node3D>
	{
	public:
		Node3D();
		~Node3D();
	public:
		void addToParent(std::shared_ptr<Node3D> parent);
		void removeFromParent();
	private:
		std::string m_name;
		std::shared_ptr<Node3D> m_parent;
		std::vector<std::shared_ptr<Node3D>> m_childs;
	};
}
