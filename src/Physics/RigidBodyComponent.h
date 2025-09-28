#pragma once
#include "Graphics/VisualComponent.h"
#include "Engine/Utility.h"
#include <DirectXMath.h>

class btRigidBody;
class btCollisionShape;
class btMotionState;
namespace Destiny
{
	class RigidBodyComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		RigidBodyComponent();
		virtual ~RigidBodyComponent();
	public:
		GET(float, mass);
		void set_mass(float mass);
		GET(float, restitution);
		void set_restitution(float restitution);
	public:
		virtual void onAddToNode() override;
		virtual void onNodeTransformChanged() override;
		virtual void onUpdate(float deltaTime) override;
	public:
		std::shared_ptr<btRigidBody> getRigidBody() const;
	protected:
		void reConstructRigidBody();
	protected:
		std::shared_ptr<btRigidBody> m_btRigidBody;
		std::shared_ptr<btMotionState> m_btMotionState;
		std::shared_ptr<btCollisionShape> m_btCollisionShape;

		float m_mass;
		float m_restitution;
	};

	inline std::shared_ptr<btRigidBody> RigidBodyComponent::getRigidBody() const
	{
		return m_btRigidBody;
	}
}