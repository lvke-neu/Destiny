#include "RigidBodyBoxComponent.h"
#include "Engine/Node.h"
#include "Engine/Engine.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/VertexDefine.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Physics/PhysicsSystem.h"
#include "RigidBodyVisual/RigidBodyBoxVisualComponent.h"
#include <btBulletDynamicsCommon.h>

namespace Destiny
{
	RigidBodyBoxComponent::RigidBodyBoxComponent() 
	{
		m_btMotionState = new btDefaultMotionState();
		m_btCollisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
		btVector3 localInertia(0, 0, 0);
		m_btCollisionShape->calculateLocalInertia(1.0f, localInertia);
		m_btRigidBody = new btRigidBody(1.0f, m_btMotionState, m_btCollisionShape, localInertia);
		//m_btRigidBody->setRestitution(1.0f);
		//m_btRigidBody->setFriction(0.5f);
		//m_btRigidBody->activate(true);

		m_restitution = 0.0f;
		m_friction = 0.0f;
		m_mass = 1.0f;
		m_boxHalfExtents = { 0.5f, 0.5f, 0.5f };

		m_rigidBodyBoxVisualComponent = std::make_shared<RigidBodyBoxVisualComponent>();
		m_rigidBodyBoxVisualComponent->modifyMesh(m_boxHalfExtents);
	}

	RigidBodyBoxComponent::~RigidBodyBoxComponent()
	{
		SAFE_DELETE(m_btRigidBody);
		SAFE_DELETE(m_btMotionState);
		SAFE_DELETE(m_btCollisionShape);
	}

	void RigidBodyBoxComponent::onAddToNode()
	{
		updateRigidBodyTransform();
		if (m_node.lock())
		{
			m_node.lock()->addComponent(m_rigidBodyBoxVisualComponent);
		}
	}

	void RigidBodyBoxComponent::onNodeTransformChanged()
	{
		updateRigidBodyTransform();
	}

	void RigidBodyBoxComponent::onEnterScene()
	{
		Engine::GetInstance()->getPhysicsSystem()->addRigidBody(shared_from_this());
	}

	void RigidBodyBoxComponent::onLeaveScene()
	{
		Engine::GetInstance()->getPhysicsSystem()->removeRigidBody(shared_from_this());
	}

	void RigidBodyBoxComponent::set_restitution(float restitution)
	{
		m_restitution = restitution;
		if (m_btRigidBody)
		{
			m_btRigidBody->setRestitution(m_restitution);
		}
	}

	void RigidBodyBoxComponent::set_mass(float mass)
	{
		if (mass < 0)
		{
			return;
		}

		m_mass = mass;
		btVector3 localInertia(0, 0, 0);
		m_btCollisionShape->calculateLocalInertia(1.0f, localInertia);

		m_btRigidBody->setMassProps(m_mass, localInertia);
	}

	void RigidBodyBoxComponent::set_friction(float friction)
	{
		m_friction = friction;
		if (m_btRigidBody)
		{
			m_btRigidBody->setFriction(m_friction);
		}
	}

	void RigidBodyBoxComponent::set_boxHalfExtents(DirectX::XMFLOAT3 boxHalfExtents)
	{
		m_boxHalfExtents = boxHalfExtents;
		SAFE_DELETE(m_btCollisionShape);
		m_btCollisionShape = new btBoxShape(btVector3(m_boxHalfExtents.x, m_boxHalfExtents.y, m_boxHalfExtents.z));
		m_btRigidBody->setCollisionShape(m_btCollisionShape);
		set_mass(m_mass);

		m_rigidBodyBoxVisualComponent->modifyMesh(boxHalfExtents);
	}

	void RigidBodyBoxComponent::updateRigidBodyTransform()
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
		rttr::registration::class_<RigidBodyBoxComponent>("RigidBodyBoxComponent")
			.constructor<>()
			.property("restitution", &RigidBodyBoxComponent::get_restitution, &RigidBodyBoxComponent::set_restitution)
			.property("friction", &RigidBodyBoxComponent::get_friction, &RigidBodyBoxComponent::set_friction)
			.property("mass", &RigidBodyBoxComponent::get_mass, &RigidBodyBoxComponent::set_mass)
			.property("boxHalfExtents", &RigidBodyBoxComponent::get_boxHalfExtents, &RigidBodyBoxComponent::set_boxHalfExtents);
	}
}