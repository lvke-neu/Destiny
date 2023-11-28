#include "Scene3D.h"
#include "Node3D.h"
#include "Component.h"
#include "Visual3DComponent.h"
#include "BoxComponent.h"
#include "SphereComponent.h"
#include "Camera3DComponent.h"
#include "CameraController.h"

namespace Destiny
{
	Scene3D::Scene3D() 
	{
		m_rootNode = std::make_shared<Node3D>();
		m_rootNode->set_name("Root");


		m_cameraNode = std::make_shared<Node3D>();
		m_cameraNode->set_name("Camera");
		m_cameraNode->addToParent(m_rootNode);
		Transform3D transform3D;
		transform3D.set_translation({ 0.0f, 0.0f, -10.0f });
		m_cameraNode->set_transform3D(transform3D);
		auto camera3DComponent = std::make_shared<Camera3DComponent>();
		m_cameraNode->addComponent(camera3DComponent);
		auto cameraController = std::make_shared<CameraController>();
		m_cameraNode->addComponent(cameraController);

		auto boxNode1 = std::make_shared<Node3D>();
		boxNode1->set_name("Box1");
		boxNode1->addToParent(m_rootNode);
		auto boxComponent1 = std::make_shared<BoxComponent>();
		boxNode1->addComponent(boxComponent1);
		transform3D.set_translation({ -2.0f, 0.0f, 0.0f });
		boxNode1->set_transform3D(transform3D);

		auto boxNode2 = std::make_shared<Node3D>();
		boxNode2->set_name("Box2");
		boxNode2->addToParent(m_rootNode);
		auto boxComponent2 = std::make_shared<BoxComponent>();
		boxNode2->addComponent(boxComponent2);
		transform3D.set_translation({ 2.0f, 0.0f, 0.0f });
		boxNode2->set_transform3D(transform3D);

		auto sphereNode = std::make_shared<Node3D>();
		sphereNode->set_name("Sphere");
		sphereNode->addToParent(m_rootNode);
		auto sphereComponent = std::make_shared<SphereComponent>();
		sphereNode->addComponent(sphereComponent);
	}
}