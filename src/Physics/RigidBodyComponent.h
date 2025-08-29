#pragma once
#include "Engine/Component.h"
#include "Engine/Utility.h"
#include <DirectXMath.h>

class btRigidBody;
class btCollisionShape;
class btMotionState;
namespace Destiny
{
	class RigidBodyComponent : public Component , public std::enable_shared_from_this<RigidBodyComponent>
	{
		RTTR_ENABLE(Component);
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

	class RigidBodyBoxComponent : public RigidBodyComponent
	{
		RTTR_ENABLE(RigidBodyComponent);
	public:
		RigidBodyBoxComponent();
		virtual ~RigidBodyBoxComponent();
	public:
		GET(DirectX::XMFLOAT3, boxHalfExtents);
		void set_boxHalfExtents(DirectX::XMFLOAT3 boxHalfExtents);
	private:
		DirectX::XMFLOAT3 m_boxHalfExtents;
	};

	class RigidBodyStaticPlaneComponent : public RigidBodyComponent
	{
		RTTR_ENABLE(RigidBodyComponent);
	public:
		RigidBodyStaticPlaneComponent();
		virtual ~RigidBodyStaticPlaneComponent();
	public:
		GET(float, position);
		void set_position(float position);
	private:
		float m_position;
	};
}