#pragma once
#include "Engine/Reflection.h"
#include <memory>

namespace Destiny
{
	class Node3D;
	class Component : public Reflection
	{
		RTTR_ENABLE(Reflection);
		friend class Node3D;
	public:
		Component();
	public:
		GET_CLASS_NAME(Component);
		GET_SET(bool, enable);
		GET(std::shared_ptr<Node3D>, node);
	protected:
		bool m_enable;
		std::shared_ptr<Node3D> m_node;
	};
}