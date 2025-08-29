#pragma once
#include "Engine/Utility.h"
#include <memory>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
namespace Destiny
{
	class RigidBodyComponent;
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();
	public:
		GET(bool, bSimulation);
		void set_bSimulation(bool bSimulation);
	public:
		void initialize();
		void uninitialize();
		void update(float deltaTime);
	public:
		void addRigidBody(std::shared_ptr<RigidBodyComponent> rigidBodyComponent);
		void removeRigidBody(std::shared_ptr<RigidBodyComponent> rigidBodyComponent);
	private:
		std::shared_ptr<btDefaultCollisionConfiguration> m_btDefaultCollisionConfiguration;
		std::shared_ptr<btCollisionDispatcher> m_btCollisionDispatcher;
		std::shared_ptr<btBroadphaseInterface> m_btBroadphaseInterface;
		std::shared_ptr<btSequentialImpulseConstraintSolver> m_btSequentialImpulseConstraintSolver;
		std::shared_ptr<btDiscreteDynamicsWorld> m_btDiscreteDynamicsWorld;

		bool m_bSimulation;
	};
}