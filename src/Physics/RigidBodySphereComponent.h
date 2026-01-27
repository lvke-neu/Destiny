#pragma once
#include "RigidBodyComponent.h"

class btRigidBody;
class btMotionState;
class btCollisionShape;
namespace Destiny
{
	class RigidBodySphereVisualComponent;
	class RigidBodySphereComponent : public RigidBodyComponent, public std::enable_shared_from_this<RigidBodySphereComponent>
	{
		RTTR_ENABLE(RigidBodyComponent);
	public:
		RigidBodySphereComponent();
		virtual ~RigidBodySphereComponent();
	public:
		virtual btRigidBody* getRigidBody() override;

		virtual void onAddToNode() override;
		virtual void onNodeTransformChanged() override;
		virtual void onEnterScene() override;
		virtual void onLeaveScene() override;
	public:
		GET(float, restitution);
		void set_restitution(float restitution);
		GET(float, mass);
		void set_mass(float mass);
		GET(float, friction);
		void set_friction(float friction);
		GET(float, radius);
		void set_radius(float radius);
	private:
		void updateRigidBodyTransform();
	private:
		btRigidBody* m_btRigidBody;
		btMotionState* m_btMotionState;
		btCollisionShape* m_btCollisionShape;

		float m_restitution;
		float m_friction;
		float m_mass;
		float m_radius;

		std::shared_ptr<RigidBodySphereVisualComponent> m_rigidBodySphereVisualComponent;
	};

	inline btRigidBody* RigidBodySphereComponent::getRigidBody()
	{
		return m_btRigidBody;
	}
}
