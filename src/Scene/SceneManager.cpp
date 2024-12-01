#include "SceneManager.h"
#include "NCS/Node.h"
#include "NCS/VisualScene.h"
#include "NCS/CameraComponent.h"
#include "NCS/CameraController.h"

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
		auto cameraNode = std::make_shared<Node>("Camera");
		cameraNode->addComponent(std::make_shared<CameraComponent>());
		cameraNode->addComponent(std::make_shared<CameraController>());
		cameraNode->addToParent(m_scene);

		m_scene->initialize();
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