#include "Scene.h"
#include "CameraComponent.h"
#include "CameraController.h"

namespace Destiny
{
	Scene::Scene(const std::string& name) : Node(name)
	{

	}

	void Scene::initialize()
	{
		m_camera = std::make_shared<CameraComponent>();
		m_cameraController = std::make_shared<CameraController>();
		m_cameraNode = std::make_shared<Node>("Camera");

		m_cameraNode->addComponent(m_camera);
		m_cameraNode->addComponent(m_cameraController);
		m_cameraNode->addToParent(shared_from_this());
	}
}