#include "Scene3D.h"
#include "Node3D.h"
#include "Component.h"
#include "Visual3DComponent.h"
#include "BoxComponent.h"
#include "SphereComponent.h"
#include "Camera3DComponent.h"
#include "CameraController.h"
#include "DirectLightComponent.h"
#include "PlaneComponent.h"
#include "Model3DComponent.h"

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

		auto directLightNode = std::make_shared<Node3D>();
		directLightNode->set_name("DirectLight");
		directLightNode->addToParent(m_rootNode);
		transform3D.set_translation({ -0.5f, -0.5f, 0.5f });
		directLightNode->set_transform3D(transform3D);
		auto directLightComponent = std::make_shared<DirectLightComponent>();
		directLightNode->addComponent(directLightComponent);

		auto planeNode = std::make_shared<Node3D>();
		planeNode->set_name("Plane");
		planeNode->addToParent(m_rootNode);
		transform3D.set_translation({ -50.0f, 0.0f, 50.0f });
		planeNode->set_transform3D(transform3D);
		auto planeComponent = std::make_shared<PlaneComponent>();
		planeNode->addComponent(planeComponent);

		auto boxNode = std::make_shared<Node3D>();
		boxNode->set_name("Box");
		boxNode->addToParent(m_rootNode);
		auto boxComponent = std::make_shared<BoxComponent>();
		boxNode->addComponent(boxComponent);
		transform3D.set_translation({ -2.0f, 0.0f, 0.0f });
		boxNode->set_transform3D(transform3D);


		auto sphereNode = std::make_shared<Node3D>();
		sphereNode->set_name("Sphere");
		sphereNode->addToParent(m_rootNode);
		auto sphereComponent = std::make_shared<SphereComponent>();
		sphereNode->addComponent(sphereComponent);

		auto modelNode = std::make_shared<Node3D>();
		modelNode->set_name("Model");
		modelNode->addToParent(m_rootNode);
		auto model3DComponent = std::make_shared<Model3DComponent>();
		modelNode->addComponent(model3DComponent);
	}
}