#pragma once

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
namespace Destiny
{
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();
	public:
		void initialize();
		void uninitialize();
		void update();
	private:
		btDefaultCollisionConfiguration* m_btDefaultCollisionConfiguration;
		btCollisionDispatcher* m_btCollisionDispatcher;
		btBroadphaseInterface* m_btBroadphaseInterface;
		btSequentialImpulseConstraintSolver* m_btSequentialImpulseConstraintSolver;
		btDiscreteDynamicsWorld* m_btDiscreteDynamicsWorld;
	};
}