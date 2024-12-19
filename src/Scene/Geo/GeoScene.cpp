#include "GeoScene.h"
#include "Cull/GeoSceneCuller.h"
#include "RuntimeEffect/BoxComponent.h"
#include "RuntimeEffect/PlaneComponent.h"
#include "RuntimeEffect/TestGeometryShaderComponent.h"
#include "RuntimeEffect/PixelBillboardComponent.h"
#include "RuntimeEffect/SizeBillboardComponent.h"

namespace Destiny
{
	GeoScene::GeoScene(const std::string& name) : 
		Scene(name),
		m_geoSceneCuller(nullptr)
	{

	}

	GeoScene::~GeoScene()
	{

	}

	void GeoScene::initialize()
	{
		Scene::initialize();
		m_geoSceneCuller = std::make_shared<GeoSceneCuller>(std::dynamic_pointer_cast<GeoScene>(shared_from_this()));


		//pixelbillboard
		{
			auto pixelBillboardComponent = std::make_shared<PixelBillboardComponent>();

			auto node = std::make_shared<Node>();
			node->set_name("PixelBillboardNode");
			node->addComponent(pixelBillboardComponent);
			node->addToParent(shared_from_this());
		}

		//sizebillboard
		{
			auto sizeBillboardComponent = std::make_shared<SizeBillboardComponent>();

			auto node = std::make_shared<Node>();
			node->set_name("SizeBillboardNode");
			node->addComponent(sizeBillboardComponent);
			node->addToParent(shared_from_this());

			Transform transform;
			transform.set_translation({ 5.0f, 0.0f, 0.0f });
			node->set_transform(transform);
		}

		//box
		{
			auto boxComponent = std::make_shared<BoxComponent>();
			//boxComponent->set_enable(false);
			auto node = std::make_shared<Node>();
			node->set_name("BoxNode");
			node->addComponent(boxComponent);
			node->addToParent(shared_from_this());

			Transform transform;
			transform.set_translation({ -5.0f, 0.0f, 0.0f });
			node->set_transform(transform);
		}

		//plane
		{
			auto planeComponent = std::make_shared<PlaneComponent>();
			//planeComponent->set_enable(false);
			auto node = std::make_shared<Node>();
			node->set_name("PlaneNode");
			node->addComponent(planeComponent);
			node->addToParent(shared_from_this());

			Transform transform;
			transform.set_scale({ 10.0f, 10.0f, 1.0f });
			transform.set_rotation({ 90.0f, 0.0f, 0.0f });
			transform.set_translation({ 0.0f, -3.0f, 0.0f });
			node->set_transform(transform);
		}

		//test geometryshader
		{
			auto testGeometryShaderComponent = std::make_shared<TestGeometryShaderComponent>();
			testGeometryShaderComponent->set_enable(false);

			auto node = std::make_shared<Node>();
			node->set_name("TestGeometryShaderNode");
			node->addComponent(testGeometryShaderComponent);
			node->addToParent(shared_from_this());
		}


		auto cameraTransfrom = m_cameraNode->get_transform();
		cameraTransfrom.set_translation({ 0.0f, 0.0f, -5.0f });
		m_cameraNode->set_transform(cameraTransfrom);
	}

	void GeoScene::uninitialize()
	{

	}

	void GeoScene::update(float deltaTime)
	{

	}

	void GeoScene::onCull()
	{
		if (m_geoSceneCuller)
		{
			m_geoSceneCuller->onCull();
		}	
	}
}