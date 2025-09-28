#include "RigidBodyStaticPlaneComponent.h"
#include "Engine/Node.h"
#include "Engine/Engine.h"
#include "Physics/PhysicsSystem.h"
#include <btBulletDynamicsCommon.h>

namespace Destiny
{
	RigidBodyStaticPlaneComponent::RigidBodyStaticPlaneComponent() :
		m_position(0.0f)
	{
		m_btCollisionShape = std::make_shared<btStaticPlaneShape>(btVector3(0.0f, 1.0f, 0.0f), m_position);
	}

	RigidBodyStaticPlaneComponent::~RigidBodyStaticPlaneComponent()
	{

	}

	void RigidBodyStaticPlaneComponent::set_position(float position)
	{
		if (Engine::GetInstance()->getPhysicsSystem()->get_bSimulation())
		{
			return;
		}

		m_position = position;
		m_btCollisionShape = std::make_shared<btStaticPlaneShape>(btVector3(0.0f, 1.0f, 0.0f), m_position);
		
		reConstructRigidBody();
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodyStaticPlaneComponent>("RigidBodyStaticPlaneComponent")
			.constructor<>()
			.property("position", &RigidBodyStaticPlaneComponent::get_position, &RigidBodyStaticPlaneComponent::set_position);
	}
}