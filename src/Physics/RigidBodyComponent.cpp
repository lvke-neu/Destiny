#include "RigidBodyComponent.h"
#include "Engine/Node.h"
#include "Engine/Engine.h"
#include "Physics/PhysicsSystem.h"
#include <btBulletDynamicsCommon.h>

namespace Destiny
{
	RigidBodyComponent::RigidBodyComponent() :
		m_btRigidBody(nullptr),
		m_btMotionState(nullptr),
		m_btCollisionShape(nullptr),
		m_mass(0.0f),
		m_restitution(0.0f)
	{

	}

	RigidBodyComponent::~RigidBodyComponent()
	{

	}

	void RigidBodyComponent::set_mass(float mass)
	{
		if (Engine::GetInstance()->getPhysicsSystem()->get_bSimulation())
		{
			return;
		}

		if (m_mass != mass)
		{
			m_mass = mass;
			reConstructRigidBody();
		}
	}

	void RigidBodyComponent::set_restitution(float restitution)
	{
		if (Engine::GetInstance()->getPhysicsSystem()->get_bSimulation())
		{
			return;
		}

		if (m_restitution != restitution)
		{
			m_restitution = restitution;
			if (m_btRigidBody)
			{
				m_btRigidBody->setRestitution(m_restitution);
			}
		}
	}

	void RigidBodyComponent::onAddToNode()
	{
		VisualComponent::onAddToNode();

		if (!m_node)
		{
			return;
		}

		DirectX::XMFLOAT3 translation = m_node->get_translation();
		btVector3 btTranslation;
		btTranslation.setValue(translation.x, translation.y, translation.z);

		DirectX::XMFLOAT3 rotation = m_node->get_rotation();
		DirectX::XMFLOAT3 radianRotation{ DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z) };
		btQuaternion btRotation;
		btRotation.setEulerZYX(radianRotation.y, radianRotation.x, radianRotation.z);

		btTransform transform;
		transform.setOrigin(btTranslation);
		transform.setRotation(btRotation);

		m_btMotionState = std::make_shared<btDefaultMotionState>(transform);

		reConstructRigidBody();
	}

	void RigidBodyComponent::onNodeTransformChanged()
	{
		VisualComponent::onNodeTransformChanged();

		if (Engine::GetInstance()->getPhysicsSystem()->get_bSimulation())
		{
			return;
		}

		if (!m_node)
		{
			return;
		}

		DirectX::XMFLOAT3 translation = m_node->get_translation();
		btVector3 btTranslation;
		btTranslation.setValue(translation.x, translation.y, translation.z);

		DirectX::XMFLOAT3 rotation = m_node->get_rotation();
		DirectX::XMFLOAT3 radianRotation{ DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z) };
		btQuaternion btRotation;
		btRotation.setEulerZYX(radianRotation.y, radianRotation.x, radianRotation.z);

		btTransform transform;
		transform.setOrigin(btTranslation);
		transform.setRotation(btRotation);

		m_btMotionState = std::make_shared<btDefaultMotionState>(transform);

		reConstructRigidBody();
	}

	void RigidBodyComponent::onUpdate(float deltaTime)
	{
		if (!Engine::GetInstance()->getPhysicsSystem()->get_bSimulation() ||!m_btRigidBody ||!m_node)
		{
			return;
		}

		auto btTranslation = m_btRigidBody->getWorldTransform().getOrigin();
		m_node->set_translation({ btTranslation.getX(), btTranslation.getY(), btTranslation.getZ() });

		DirectX::XMFLOAT3 rotation;
		auto btQuaternion = m_btRigidBody->getWorldTransform().getRotation();
		btQuaternion.getEulerZYX(rotation.y, rotation.x, rotation.z);

		rotation.x = DirectX::XMConvertToDegrees(rotation.x);
		rotation.y = DirectX::XMConvertToDegrees(rotation.y);
		rotation.z = DirectX::XMConvertToDegrees(rotation.z);

		m_node->set_rotation(rotation);
	}

	void RigidBodyComponent::reConstructRigidBody()
	{
		if (!m_btMotionState || !m_btCollisionShape)
		{
			return;
		}
		
		btVector3 inertia(0, 0, 0);
		m_btCollisionShape->calculateLocalInertia(m_mass, inertia);

		m_btRigidBody = std::make_shared<btRigidBody>(m_mass, m_btMotionState.get(), m_btCollisionShape.get(), inertia);
		m_btRigidBody->setRestitution(m_restitution);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodyComponent>("RigidBodyComponent")
			.constructor<>()
			.property("mass", &RigidBodyComponent::get_mass, &RigidBodyComponent::set_mass)
			.property("restitution", &RigidBodyComponent::get_restitution, &RigidBodyComponent::set_restitution);
	}
}