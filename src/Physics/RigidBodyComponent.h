#pragma once
#include "Engine/Component.h"
#include "Engine/Utility.h"
#include <DirectXMath.h>>

class btRigidBody;
namespace Destiny
{
	class RigidBodyComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		RigidBodyComponent() = default;
		virtual ~RigidBodyComponent() = default;
	public:
		virtual btRigidBody* getRigidBody() = 0;
	};
}