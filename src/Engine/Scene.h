#pragma once
#include "Node.h"

namespace Destiny
{
	class Scene : public Node
	{
		RTTR_ENABLE(Node);
	public:
		Scene(const std::string& name);
		virtual ~Scene() = default;
	public:
		GET_CLASS_NAME(Scene);
	public:
		virtual void initialize() = 0;
		virtual void uninitialize() = 0;
		virtual void update(float deltaTime) = 0;
		virtual void onCull() = 0;	
	};
}