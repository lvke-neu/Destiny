#include "PhysicsSystem.h"
#include "Engine/Engine.h"
#include "Engine/Scene.h"
#include "Scene/SceneManager.h"
#include "RigidBodyComponent.h"
#include <btBulletDynamicsCommon.h>
#include <queue>

namespace Destiny
{
	PhysicsSystem::PhysicsSystem() :
        m_btDefaultCollisionConfiguration(nullptr),
        m_btCollisionDispatcher(nullptr),
        m_btBroadphaseInterface(nullptr),
        m_btSequentialImpulseConstraintSolver(nullptr),
        m_btDiscreteDynamicsWorld(nullptr),
        m_bSimulation(false)
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{

	}

    void PhysicsSystem::initialize()
	{
        m_btDefaultCollisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
        m_btCollisionDispatcher = std::make_shared<btCollisionDispatcher>(m_btDefaultCollisionConfiguration.get());
        m_btBroadphaseInterface = std::make_shared<btDbvtBroadphase>();
        m_btSequentialImpulseConstraintSolver = std::make_shared<btSequentialImpulseConstraintSolver>();
        m_btDiscreteDynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(m_btCollisionDispatcher.get(), m_btBroadphaseInterface.get(), m_btSequentialImpulseConstraintSolver.get(), m_btDefaultCollisionConfiguration.get());
        m_btDiscreteDynamicsWorld->setGravity({ 0.0f, -9.8f, 0.0f });
        
        
        //btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

        //btDefaultMotionState* groundMotionState =
        //    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

        //btRigidBody::btRigidBodyConstructionInfo
        //    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        //btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        //m_btDiscreteDynamicsWorld->addRigidBody(groundRigidBody);


        //btCollisionShape* fallShape = new btBoxShape(btVector3(1, 1, 1));

        //btDefaultMotionState* fallMotionState =
        //    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

        //btScalar mass = 1;
        //btVector3 fallInertia(0, 0, 0);
        //fallShape->calculateLocalInertia(mass, fallInertia);

        //btRigidBody::btRigidBodyConstructionInfo
        //    fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
        //btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);

        //m_btDiscreteDynamicsWorld->addRigidBody(fallRigidBody);

        //for (int i = 0; i < 300; i++) {
        //    //m_btDiscreteDynamicsWorld->stepSimulation(1.f / 60.f, 10);

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

	void PhysicsSystem::update(float deltaTime)
	{
        if (!m_bSimulation)
        {
            return;
        }
        m_btDiscreteDynamicsWorld->stepSimulation(deltaTime, 10);
        for(const auto& rigidBodyComponent : m_rigidBodyComponents)
        {
            if (!rigidBodyComponent || 
                !rigidBodyComponent->getRigidBody() || !rigidBodyComponent->getRigidBody()->getCollisionShape() ||
                rigidBodyComponent->getRigidBody()->getCollisionShape()->getShapeType() == 28 ||
                !rigidBodyComponent->get_node().lock())
            {
                continue;
            }
            
            auto btTranslation = rigidBodyComponent->getRigidBody()->getWorldTransform().getOrigin();
            rigidBodyComponent->get_node().lock()->set_translation({ btTranslation.getX(), btTranslation.getY(), btTranslation.getZ() });

            DirectX::XMFLOAT3 rotation;
            auto btQuaternion = rigidBodyComponent->getRigidBody()->getWorldTransform().getRotation();
            btQuaternion.getEulerZYX(rotation.z, rotation.y, rotation.x);

            rotation.x = DirectX::XMConvertToDegrees(rotation.x);
            rotation.y = DirectX::XMConvertToDegrees(rotation.y);
            rotation.z = DirectX::XMConvertToDegrees(rotation.z);

            rigidBodyComponent->get_node().lock()->set_rotation(rotation);
        }
	}

    void PhysicsSystem::addRigidBody(std::shared_ptr<RigidBodyComponent> rigidBodyComponent)
    {
        if (!rigidBodyComponent)
        {
            return;
        }
        
        m_btDiscreteDynamicsWorld->addRigidBody(rigidBodyComponent->getRigidBody());
        m_rigidBodyComponents.insert(rigidBodyComponent);
    }

    void PhysicsSystem::removeRigidBody(std::shared_ptr<RigidBodyComponent> rigidBodyComponent)
    {
        if (!rigidBodyComponent)
        {
            return;
        }

        m_btDiscreteDynamicsWorld->removeRigidBody(rigidBodyComponent->getRigidBody());
        m_rigidBodyComponents.erase(rigidBodyComponent);
    }
}