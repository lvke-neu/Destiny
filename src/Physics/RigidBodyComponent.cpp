#include "RigidBodyComponent.h"
#include "Engine/Node.h"
#include "Engine/Engine.h"
#include "Physics/PhysicsSystem.h"
#include <btBulletDynamicsCommon.h>

namespace Destiny
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodyComponent>("RigidBodyComponent");
	}
}