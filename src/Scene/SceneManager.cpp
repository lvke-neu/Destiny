#include "SceneManager.h"
#include "NCS/Scene.h"

namespace Destiny
{
	SceneManager::SceneManager() : 
		m_scene(std::make_shared<Scene>())
	{
		
	}

	SceneManager::~SceneManager()
	{

	}	

	void SceneManager::initialize()
	{
		m_scene->initialize();
	}

	void SceneManager::uninitialize()
	{
		m_scene->uninitialize();
	}

	void SceneManager::update(float deltaTime)
	{
		m_scene->update(deltaTime);
	}
}