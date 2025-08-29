#include "PhysicsSystem.h"
#include "Engine/Utility.h"
#include <btBulletDynamicsCommon.h>

namespace Destiny
{
	PhysicsSystem::PhysicsSystem() :
        m_btDefaultCollisionConfiguration(nullptr),
        m_btCollisionDispatcher(nullptr),
        m_btBroadphaseInterface(nullptr),
        m_btSequentialImpulseConstraintSolver(nullptr),
        m_btDiscreteDynamicsWorld(nullptr)
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{
        SAFE_DELETE(m_btDefaultCollisionConfiguration);
        SAFE_DELETE(m_btCollisionDispatcher);
        SAFE_DELETE(m_btBroadphaseInterface);
        SAFE_DELETE(m_btSequentialImpulseConstraintSolver);
        SAFE_DELETE(m_btDiscreteDynamicsWorld);
	}

	void PhysicsSystem::initialize()
	{
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -10, 0));


        //btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

        //btDefaultMotionState* groundMotionState =
        //    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

        //btRigidBody::btRigidBodyConstructionInfo
        //    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        //btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        //dynamicsWorld->addRigidBody(groundRigidBody);


        //btCollisionShape* fallShape = new btBoxShape(btVector3(1, 1, 1));

        //btDefaultMotionState* fallMotionState =
        //    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

        //btScalar mass = 1;
        //btVector3 fallInertia(0, 0, 0);
        //fallShape->calculateLocalInertia(mass, fallInertia);

        //btRigidBody::btRigidBodyConstructionInfo
        //    fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
        //btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);

        //dynamicsWorld->addRigidBody(fallRigidBody);

        //for (int i = 0; i < 300; i++) {
        //    dynamicsWorld->stepSimulation(1.f / 60.f, 10);

        //    btTransform trans;
        //    fallRigidBody->getMotionState()->getWorldTransform(trans);

        //    LOG_INFO("step {0} cube height: {1}", std::to_string(i), std::to_string(trans.getOrigin().getY()));
        //}

        //dynamicsWorld->removeRigidBody(fallRigidBody);
        //delete fallRigidBody->getMotionState();
        //delete fallRigidBody;
        //delete fallShape;

        //dynamicsWorld->removeRigidBody(groundRigidBody);
        //delete groundRigidBody->getMotionState();
        //delete groundRigidBody;
        //delete groundShape;

        //delete dynamicsWorld;
        //delete solver;
        //delete overlappingPairCache;
        //delete dispatcher;
        //delete collisionConfiguration;
	}

	void PhysicsSystem::uninitialize()
	{

	}

	void PhysicsSystem::update()
	{

	}
}