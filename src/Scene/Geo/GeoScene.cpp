#include "GeoScene.h"
#include "Cull/GeoSceneCuller.h"
#include "RuntimeEffect/BoxComponent.h"
#include "RuntimeEffect/PlaneComponent.h"

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

		//box
		{
			auto boxComponent = std::make_shared<BoxComponent>();

			auto node = std::make_shared<Node>();
			node->set_name("BoxNode");
			node->addComponent(boxComponent);
			node->addToParent(shared_from_this());
		}

		//plane
		{
			auto planeComponent = std::make_shared<PlaneComponent>();
			
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