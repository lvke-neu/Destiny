#include "SceneManager.h"
#include "NCS/Scene.h"
#include "NCS/Node.h"
#include "VisualComponent.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"


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
		bfs(m_scene->getRootNode());
	}

	void SceneManager::bfs(std::shared_ptr<Node> node)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto visualComponent = dynamic_cast<VisualComponent*>(component.get());
			if (visualComponent)
			{
				Engine::GetInstance()->getGraphicsSystem()->commitRenderParameters(visualComponent->getRenderParameters());
			}	
		}

		for (const auto& childNode : node->getChilds())
		{
			bfs(childNode);
		}
	}
}