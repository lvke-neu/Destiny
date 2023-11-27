#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Engine//Reflection.h"

namespace Destiny
{
	class Node3D : public Reflection, public std::enable_shared_from_this<Node3D>
	{
		RTTR_ENABLE();
	public:
		Node3D();
	public:
		void addToParent(std::shared_ptr<Node3D> parent);
		void removeFromParent();
	public:
		GET_CLASS_NAME(Node3D);
		GET_SET(std::string, uuid);
		GET_SET(std::string, name);
		GET(std::vector<std::shared_ptr<Node3D>>, childs);
	private:
		std::string m_uuid;
		std::string m_name;
		std::shared_ptr<Node3D> m_parent;
		std::vector<std::shared_ptr<Node3D>> m_childs;
	};
}
