#pragma once
#include "RigidBodyComponent.h"

class btRigidBody;
class btCollisionShape;
namespace Destiny
{
	class RigidBodyStaticPlaneComponent : public RigidBodyComponent, public std::enable_shared_from_this<RigidBodyStaticPlaneComponent>
	{
		RTTR_ENABLE(RigidBodyComponent);
	public:
		RigidBodyStaticPlaneComponent();
		virtual ~RigidBodyStaticPlaneComponent();
	public:
		virtual btRigidBody* getRigidBody() override;

		virtual void onAddToNode() override;
		virtual void onNodeTransformChanged() override;
		virtual void onEnterScene() override;
		virtual void onLeaveScene() override;
	public:
		GET(float, restitution);
		void set_restitution(float restitution);
		GET(DirectX::XMFLOAT3, normal);
		void set_normal(DirectX::XMFLOAT3 normal);
		GET(float, friction);
		void set_friction(float friction);
	private:
		btRigidBody* m_btRigidBody;
		btCollisionShape* m_btCollisionShape;

		float m_restitution;
		float m_friction;
		DirectX::XMFLOAT3 m_normal;
	};

	btRigidBody* RigidBodyStaticPlaneComponent::getRigidBody()
	{
		return m_btRigidBody;
	}
}