#include "SceneManager.h"
#include "NCS/Scene.h"
#include "NCS/Node.h"
#include "NCS/VisualComponent.h"
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
		bfsRender(m_scene);
	}

	void SceneManager::bfsRender(std::shared_ptr<Node> node)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
			if (visualComponent)
			{
				Engine::GetInstance()->getGraphicsSystem()->commitRenderParameters(visualComponent->getRenderParameters());
			}	
		}

		for (const auto& childNode : node->getChilds())
		{
			bfsRender(childNode);
		}
	}
}