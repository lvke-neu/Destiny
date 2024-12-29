#include "SceneManager.h"
#include "Graphics/VisualScene.h"
#include "Graphics/CameraController.h"
#include "RuntimeEffect/BoxComponent.h"
#include "RuntimeEffect/PlaneComponent.h"
#include "RuntimeEffect/TestGeometryShaderComponent.h"
#include "RuntimeEffect/PixelBillboardComponent.h"
#include "RuntimeEffect/SizeBillboardComponent.h"
#include "RuntimeEffect/InstancedComponet.h"
#include "Model/ModelComponent.h"

namespace Destiny
{
	SceneManager::SceneManager() : 
		m_scene(std::make_shared<VisualScene>("VisualScene"))
	{
		
	}

	SceneManager::~SceneManager()
	{

	}	

	void SceneManager::initialize()
	{
		m_scene->initialize();

		//camera
		auto cameraTransfrom = m_scene->getCameraNode()->get_transform();
		cameraTransfrom.set_translation({ -1.17f, 1.4f, -2.6f });
		cameraTransfrom.set_rotation({ -1.12f, 36.6f, 0.0f });
		m_scene->getCameraNode()->set_transform(cameraTransfrom);

		//light gzimo
		{
			auto lightGzimoComponent = std::make_shared<PixelBillboardComponent>("builtin://texture/directional_light_icon.png");
			lightGzimoComponent->set_enable(true);
			lightGzimoComponent->set_size({ 50.0f, 50.0f });
			Transform transform;
			transform.set_translation({ 0.0f, 2.0f, -2.0f });
			transform.set_rotation({ 0.0f, -1.0f, 1.0f });
			m_scene->getDirectionLightNode()->addComponent(lightGzimoComponent);
			m_scene->getDirectionLightNode()->set_transform(transform);
		}

		//plane
		{
			auto planeComponent = std::make_shared<PlaneComponent>();
			planeComponent->set_enable(true);
			planeComponent->set_color({ 65.0f / 255, 90.0f / 255, 20.0f / 255, 1.0f });

			auto node = std::make_shared<Node>();
			node->set_name("PlaneNode");
			node->addComponent(planeComponent);
			node->addToParent(m_scene);

			Transform transform;
			transform.set_scale({ 100.0f, 100.0f, 1.0f });
			transform.set_rotation({ 90.0f, 0.0f, 0.0f });
			transform.set_translation({ 0.0f, 0.0f, 0.0f });
			node->set_transform(transform);
		}

		//model
		{
			auto modelComponent1 = std::make_shared<ModelComponent>();
			modelComponent1->set_path("builtin://model/nanosuit/nanosuit.obj");
			auto node1 = std::make_shared<Node>();
			Transform transform1;
			transform1.set_scale({ 0.11f, 0.11f, 0.11f });
			transform1.set_rotation({ 0.0f, 0.0f, 0.0f });
			transform1.set_translation({ 2.0f, 0.0f, 0.0f });
			node1->set_name("nanosuit");
			node1->addComponent(modelComponent1);
			node1->addToParent(m_scene);
			node1->set_transform(transform1);

			auto modelComponent2 = std::make_shared<ModelComponent>();
			modelComponent2->set_path("builtin://model/walk/Standard Walk.dae");
			auto node2 = std::make_shared<Node>();
			node2->set_name("Walk");
			node2->addComponent(modelComponent2);
			node2->addToParent(m_scene);

			auto modelComponent3 = std::make_shared<ModelComponent>();
			modelComponent3->set_path("builtin://model/Jumping/Jumping.dae");
			auto node3 = std::make_shared<Node>();
			Transform transform3;
			transform3.set_translation({ -2.0f, 0.0f, 0.0f });
			node3->set_name("Jump");
			node3->addComponent(modelComponent3);
			node3->addToParent(m_scene);
			node3->set_transform(transform3);

			auto modelComponent4 = std::make_shared<ModelComponent>();
			modelComponent4->set_path("builtin://model/Ymca Dance/Ymca Dance.dae");
			auto node4 = std::make_shared<Node>();
			Transform transform4;
			transform4.set_translation({ 0.0f, 0.0f, 2.0f });
			node4->set_name("Ymca Dance");
			node4->addComponent(modelComponent4);
			node4->addToParent(m_scene);
			node4->set_transform(transform4);

			auto modelComponent5 = std::make_shared<ModelComponent>();
			modelComponent5->set_path("builtin://model/planet/planet.obj");
			auto node5 = std::make_shared<Node>();
			Transform transform5;
			transform5.set_translation({ -1.0f, 1.0f, 2.0f });
			transform5.set_scale({ 0.1f, 0.1f, 0.1f });
			node5->set_name("planet");
			node5->addComponent(modelComponent5);
			node5->addToParent(m_scene);
			node5->set_transform(transform5);
		}
	}

	void SceneManager::uninitialize()
	{
		m_scene->uninitialize();
	}

	void SceneManager::update(float deltaTime)
	{
		m_scene->update(deltaTime);
		m_scene->onCull();
	}
}