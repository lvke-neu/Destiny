#include "RigidBodySphereComponent.h"
#include "Engine/Node.h"
#include "Engine/Engine.h"
#include "Physics/PhysicsSystem.h"
#include "RigidBodyVisual/RigidBodySphereVisualComponent.h"
#include <btBulletDynamicsCommon.h>

namespace Destiny
{
	RigidBodySphereComponent::RigidBodySphereComponent()
	{
		m_radius = 1.0f;
		m_btMotionState = new btDefaultMotionState();
		m_btCollisionShape = new btSphereShape(m_radius);
		
		m_mass = 1.0f;
		btVector3 localInertia(0, 0, 0);
		m_btCollisionShape->calculateLocalInertia(m_mass, localInertia);
		
		m_btRigidBody = new btRigidBody(m_mass, m_btMotionState, m_btCollisionShape, localInertia);

		m_restitution = 0.0f;
		m_friction = 0.0f;

		m_rigidBodySphereVisualComponent = std::make_shared<RigidBodySphereVisualComponent>();
	}

	RigidBodySphereComponent::~RigidBodySphereComponent()
	{
		SAFE_DELETE(m_btRigidBody);
		SAFE_DELETE(m_btMotionState);
		SAFE_DELETE(m_btCollisionShape);
	}

	void RigidBodySphereComponent::onAddToNode()
	{
		updateRigidBodyTransform();

		if(m_node.lock())
		{
			m_node.lock()->addComponent(m_rigidBodySphereVisualComponent);
			m_rigidBodySphereVisualComponent->modifyMesh(m_radius);
		}
	}

	void RigidBodySphereComponent::onNodeTransformChanged()
	{
		updateRigidBodyTransform();
	}

	void RigidBodySphereComponent::onEnterScene()
	{
		Engine::GetInstance()->getPhysicsSystem()->addRigidBody(shared_from_this());
	}

	void RigidBodySphereComponent::onLeaveScene()
	{
		Engine::GetInstance()->getPhysicsSystem()->removeRigidBody(shared_from_this());
	}

	void RigidBodySphereComponent::set_restitution(float restitution)
	{
		m_restitution = restitution;
		if (m_btRigidBody)
		{
			m_btRigidBody->setRestitution(m_restitution);
		}
	}

	void RigidBodySphereComponent::set_mass(float mass)
	{
		if (mass < 0)
		{
			return;
		}

		m_mass = mass;
		btVector3 localInertia(0, 0, 0);
		m_btCollisionShape->calculateLocalInertia(m_mass, localInertia);

		m_btRigidBody->setMassProps(m_mass, localInertia);
	}

	void RigidBodySphereComponent::set_friction(float friction)
	{
		m_friction = friction;
		if (m_btRigidBody)
		{
			m_btRigidBody->setFriction(m_friction);
		}
	}

	void RigidBodySphereComponent::set_radius(float radius)
	{
		if (radius <= 0)
			return;

		m_radius = radius;
		// Recreate shape if radius changes, as btSphereShape radius is not dynamic
		SAFE_DELETE(m_btCollisionShape);
		m_btCollisionShape = new btSphereShape(m_radius);
		
		// Recalculate inertia
		btVector3 localInertia(0, 0, 0);
		m_btCollisionShape->calculateLocalInertia(m_mass, localInertia);
		
		m_btRigidBody->setCollisionShape(m_btCollisionShape);
		m_btRigidBody->setMassProps(m_mass, localInertia);

		m_rigidBodySphereVisualComponent->modifyMesh(m_radius);
	}

	void RigidBodySphereComponent::updateRigidBodyTransform()
	{
		if (Engine::GetInstance()->getPhysicsSystem()->getSimulation() || !m_btRigidBody || !m_node.lock())
		{
			return;
		}

		DirectX::XMFLOAT3 translation = m_node.lock()->get_translation();
		btVector3 btTranslation;
		btTranslation.setValue(translation.x, translation.y, translation.z);

		DirectX::XMFLOAT3 rotation = m_node.lock()->get_rotation();
		DirectX::XMFLOAT3 radianRotation{ DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z) };
		btQuaternion btRotation;
		btRotation.setEulerZYX(radianRotation.z, radianRotation.y, radianRotation.x);

		btTransform transform;
		transform.setOrigin(btTranslation);
		transform.setRotation(btRotation);

		//reset 
		m_btRigidBody->setLinearVelocity(btVector3(0, 0, 0));
		m_btRigidBody->setAngularVelocity(btVector3(0, 0, 0));
		m_btRigidBody->clearForces();
		m_btRigidBody->setWorldTransform(transform);
		m_btRigidBody->activate(true);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodySphereComponent>("RigidBodySphereComponent")
			.constructor<>()
			.property("restitution", &RigidBodySphereComponent::get_restitution, &RigidBodySphereComponent::set_restitution)
			.property("friction", &RigidBodySphereComponent::get_friction, &RigidBodySphereComponent::set_friction)
			.property("mass", &RigidBodySphereComponent::get_mass, &RigidBodySphereComponent::set_mass)
			.property("radius", &RigidBodySphereComponent::get_radius, &RigidBodySphereComponent::set_radius);
	}
}
