#include "SceneManager.h"
#include "Graphics/VisualScene.h"
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

		////pixelbillboard
		//{
		//	auto pixelBillboardComponent = std::make_shared<PixelBillboardComponent>("builtin://texture/tree0.dds");
		//	pixelBillboardComponent->set_enable(true);

		//	auto node = std::make_shared<Node>();
		//	node->set_name("PixelBillboardNode");
		//	node->addComponent(pixelBillboardComponent);
		//	node->addToParent(m_scene);
		//}

		////pixelbillboard2
		//{
		//	auto pixelBillboardComponent = std::make_shared<PixelBillboardComponent>("builtin://texture/yuanhuan.png");
		//	pixelBillboardComponent->set_enable(true);

		//	auto node = std::make_shared<Node>();
		//	node->set_name("PixelBillboardNode2");
		//	node->addComponent(pixelBillboardComponent);
		//	node->addToParent(m_scene);

		//	Transform transform;
		//	transform.set_translation({ 5.0f, 0.0f, 0.0f });
		//	node->set_transform(transform);
		//}

		////sizebillboard
		//{
		//	auto sizeBillboardComponent = std::make_shared<SizeBillboardComponent>();
		//	sizeBillboardComponent->set_enable(true);

		//	auto node = std::make_shared<Node>();
		//	node->set_name("SizeBillboardNode");
		//	node->addComponent(sizeBillboardComponent);
		//	node->addToParent(m_scene);

		//	Transform transform;
		//	transform.set_translation({ 0.0f, 0.0f, 5.0f });
		//	node->set_transform(transform);
		//}

		////box
		//{
		//	auto boxComponent = std::make_shared<BoxComponent>();
		//	boxComponent->set_enable(true);
		//	
		//	auto node = std::make_shared<Node>();
		//	node->set_name("BoxNode");
		//	node->addComponent(boxComponent);
		//	node->addToParent(m_scene);

		//	Transform transform;
		//	transform.set_translation({ -5.0f, 0.0f, 0.0f });
		//	node->set_transform(transform);
		//}

		//plane
		{
			auto planeComponent = std::make_shared<PlaneComponent>();
			planeComponent->set_enable(true);
			
			auto node = std::make_shared<Node>();
			node->set_name("PlaneNode");
			node->addComponent(planeComponent);
			node->addToParent(m_scene);

			Transform transform;
			transform.set_scale({ 10.0f, 10.0f, 1.0f });
			transform.set_rotation({ 90.0f, 0.0f, 0.0f });
			transform.set_translation({ 0.0f, 0.0f, 0.0f });
			node->set_transform(transform);
		}

		////instance
		//{
		//	auto instancedComponet = std::make_shared<InstancedComponet>();
		//	instancedComponet->set_enable(true);

		//	auto node = std::make_shared<Node>();
		//	node->set_name("InstancedNode");
		//	node->addComponent(instancedComponet);
		//	node->addToParent(m_scene);
		//}

		//model
		{
			auto modelComponent1 = std::make_shared<ModelComponent>();
			modelComponent1->set_path("builtin://model/2nrtbod1out/2nrtbod1out.obj");
			auto node1 = std::make_shared<Node>();
			Transform transform1;
			transform1.set_scale({ 0.02f, 0.02f, 0.02f });
			transform1.set_rotation({ 90.0f, 0.0f, 0.0f });
			transform1.set_translation({ 2.0f, 0.0f, 0.0f });
			node1->set_name("ModelComponentNode1");
			node1->addComponent(modelComponent1);
			node1->addToParent(m_scene);
			node1->set_transform(transform1);

			auto modelComponent2 = std::make_shared<ModelComponent>();
			modelComponent2->set_path("builtin://model/nanosuit/nanosuit.obj");
			auto node2 = std::make_shared<Node>();
			Transform transform2;
			transform2.set_scale({ 0.2f, 0.2f, 0.2f });
			transform2.set_rotation({ 0.0f, 0.0f, 0.0f });
			transform2.set_translation({ -2.0f, 0.0f, 0.0f });
			node2->set_name("ModelComponentNode2");
			node2->addComponent(modelComponent2);
			node2->addToParent(m_scene);
			node2->set_transform(transform2);
		}

		auto cameraTransfrom = m_scene->getCameraNode()->get_transform();
		cameraTransfrom.set_translation({ 0.0f, 0.5f, -5.0f });
		m_scene->getCameraNode()->set_transform(cameraTransfrom);
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