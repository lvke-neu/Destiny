#include "RigidBodyStaticPlaneComponent.h"
#include "Engine/Node.h"
#include "Engine/Engine.h"
#include "Physics/PhysicsSystem.h"
#include <btBulletDynamicsCommon.h>

namespace Destiny
{
	RigidBodyStaticPlaneComponent::RigidBodyStaticPlaneComponent() 
	{
		m_btCollisionShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);

		m_btRigidBody = new btRigidBody(0.0f, nullptr, m_btCollisionShape);

		m_btRigidBody->setMassProps(0.0f, btVector3(0, 0, 0));
		//m_btRigidBody->setRestitution(0.0f);
		//m_btRigidBody->setFriction(0.8f);
		//m_btRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
	
		m_restitution = 0.0f;
		m_friction = 0.0f;
		m_normal = { 0.0f, 1.0f, 0.0f };
	}

	RigidBodyStaticPlaneComponent::~RigidBodyStaticPlaneComponent()
	{
		SAFE_DELETE(m_btRigidBody);
		SAFE_DELETE(m_btCollisionShape);
	}

	void RigidBodyStaticPlaneComponent::onAddToNode()
	{
		if (!m_node.lock())
		{
			return;
		}

		SAFE_DELETE(m_btCollisionShape);
		m_btCollisionShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), m_node.lock()->get_translation().y);
		m_btRigidBody->setCollisionShape(m_btCollisionShape);
	}

	void RigidBodyStaticPlaneComponent::onNodeTransformChanged()
	{
		if (!m_node.lock())
		{
			return;
		}

		SAFE_DELETE(m_btCollisionShape);
		m_btCollisionShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), m_node.lock()->get_translation().y);
		m_btRigidBody->setCollisionShape(m_btCollisionShape);
	}

	void RigidBodyStaticPlaneComponent::onEnterScene()
	{
		Engine::GetInstance()->getPhysicsSystem()->addRigidBody(shared_from_this());
	}

	void RigidBodyStaticPlaneComponent::onLeaveScene()
	{
		Engine::GetInstance()->getPhysicsSystem()->removeRigidBody(shared_from_this());
	}

	void RigidBodyStaticPlaneComponent::set_restitution(float restitution)
	{
		m_restitution = restitution;
		if (m_btRigidBody)
		{
			m_btRigidBody->setRestitution(m_restitution);
		}
	}

	void RigidBodyStaticPlaneComponent::set_normal(DirectX::XMFLOAT3 normal)
	{
		m_normal = normal;
		if (!m_node.lock())
		{
			return;
		}
		SAFE_DELETE(m_btCollisionShape);
		m_btCollisionShape = new btStaticPlaneShape(btVector3(m_normal.x, m_normal.y, m_normal.z), m_node.lock()->get_translation().y);
		m_btRigidBody->setCollisionShape(m_btCollisionShape);
	}

	void RigidBodyStaticPlaneComponent::set_friction(float friction)
	{
		m_friction = friction;
		if (m_btRigidBody)
		{
			m_btRigidBody->setFriction(m_friction);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodyStaticPlaneComponent>("RigidBodyStaticPlaneComponent")
			.constructor<>()
			.property("restitution", &RigidBodyStaticPlaneComponent::get_restitution, &RigidBodyStaticPlaneComponent::set_restitution)
			.property("friction", &RigidBodyStaticPlaneComponent::get_friction, &RigidBodyStaticPlaneComponent::set_friction)
			.property("normal", &RigidBodyStaticPlaneComponent::get_normal, &RigidBodyStaticPlaneComponent::set_normal);
	}
}