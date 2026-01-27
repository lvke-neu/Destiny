#pragma once
#include "Engine/Utility.h"
#include <memory>
#include <unordered_set>

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
		void initialize();
		void uninitialize();
		void update(float deltaTime);
	public:
		void addRigidBody(std::shared_ptr<RigidBodyComponent> rigidBodyComponent);
		void removeRigidBody(std::shared_ptr<RigidBodyComponent> rigidBodyComponent);
	public:
		bool getSimulation();
		void setSimulation(bool bSimulation);
	private:
		std::shared_ptr<btDefaultCollisionConfiguration> m_btDefaultCollisionConfiguration;
		std::shared_ptr<btCollisionDispatcher> m_btCollisionDispatcher;
		std::shared_ptr<btBroadphaseInterface> m_btBroadphaseInterface;
		std::shared_ptr<btSequentialImpulseConstraintSolver> m_btSequentialImpulseConstraintSolver;
		std::shared_ptr<btDiscreteDynamicsWorld> m_btDiscreteDynamicsWorld;
		std::unordered_set<std::shared_ptr<RigidBodyComponent>> m_rigidBodyComponents;

		bool m_bSimulation;
	};

	inline bool PhysicsSystem::getSimulation()
	{
		return m_bSimulation;
	}

	inline void PhysicsSystem::setSimulation(bool bSimulation)
	{
		m_bSimulation = bSimulation;
	}
}