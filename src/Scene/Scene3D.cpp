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
#include "BillboardComponent.h"
#include "SkyboxComponent.h"
#include "RenderToTextureComponent.h"
#include "ParticleComponent.h"

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
		transform3D.set_translation({ 0.0f, 2.0f, -10.0f });
		m_cameraNode->set_transform3D(transform3D);
		auto camera3DComponent = std::make_shared<Camera3DComponent>();
		m_cameraNode->addComponent(camera3DComponent);
		auto cameraController = std::make_shared<CameraController>();
		m_cameraNode->addComponent(cameraController);

		auto directLightNode = std::make_shared<Node3D>();
		directLightNode->set_name("DirectLight");
		directLightNode->addToParent(m_rootNode);
		//transform3D.set_translation({ -0.5f, -0.5f, 0.5f });
		directLightNode->set_transform3D(transform3D);
		auto directLightComponent = std::make_shared<DirectLightComponent>();
		directLightNode->addComponent(directLightComponent);

		auto skyboxNode = std::make_shared<Node3D>();
		skyboxNode->set_name("Skybox");
		skyboxNode->addToParent(m_rootNode);
		auto skyboxComponent = std::make_shared<SkyboxComponent>();
		skyboxNode->addComponent(skyboxComponent);

		auto planeNode = std::make_shared<Node3D>();
		planeNode->set_name("Plane");
		planeNode->addToParent(m_rootNode);
		transform3D = Transform3D();
		transform3D.set_translation({ 0.0f, -1.0f, 0.0f });
		transform3D.set_scale({ 50.0f, 50.0f, 1.0f });
		transform3D.set_rotation({90.0f, 0.0f, 0.0f});
		planeNode->set_transform3D(transform3D);
		auto planeComponent = std::make_shared<PlaneComponent>();
		planeNode->addComponent(planeComponent);

		auto boxNode = std::make_shared<Node3D>();
		transform3D = Transform3D();
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

		Transform3D transformModel;
		auto powerplantNode = std::make_shared<Node3D>();
		powerplantNode->set_name("powerplant");
		powerplantNode->addToParent(m_rootNode);
		transformModel.set_translation({ 0.0f, 0.0f, 5.0f });
		transformModel.set_scale({ 0.05f, 0.05f, 0.05f });
		powerplantNode->set_transform3D(transformModel);
		auto powerplantComponent = std::make_shared<Model3DComponent>();
		powerplantNode->addComponent(powerplantComponent);
		powerplantComponent->set_path("assets://Model/powerplant/powerplant.gltf");

		auto walkNode = std::make_shared<Node3D>();
		walkNode->set_name("walk");
		walkNode->addToParent(m_rootNode);
		transformModel.set_scale({ 0.03f, 0.03f, 0.03f });
		walkNode->set_transform3D(transformModel);
		auto walkComponent = std::make_shared<Model3DComponent>();
		walkNode->addComponent(walkComponent);
		walkComponent->set_path("assets://Model/walk/Standard Walk.dae");

		Transform3D transform3DGS;
		transform3DGS.set_translation({ 3.0f, 0.0f, 0.0f });
		auto gsNode = std::make_shared<Node3D>();
		gsNode->set_name("Billboard");
		gsNode->addToParent(m_rootNode);
		auto testGsComponent = std::make_shared<BillboardComponent>();
		gsNode->addComponent(testGsComponent);
		gsNode->set_transform3D(transform3DGS);



		auto renderToTextureNode = std::make_shared<Node3D>();
		renderToTextureNode->set_name("RenderToTexture");
		renderToTextureNode->addToParent(m_rootNode);
		auto renderToTextureComponent = std::make_shared<RenderToTextureComponent>();
		renderToTextureNode->addComponent(renderToTextureComponent);

		auto particleNode = std::make_shared<Node3D>();
		particleNode->set_name("Particle");
		particleNode->addToParent(m_rootNode);
		auto particleComponent = std::make_shared<ParticleComponent>();
		particleNode->addComponent(renderToTextureComponent);
	}
}