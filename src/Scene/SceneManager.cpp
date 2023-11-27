#include "SceneManager.h"
#include "Scene3D.h"
#include "Node3D.h"
#include "Visual3DComponent.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	SceneManager::SceneManager() :
		m_scene3D(nullptr)
	{
		
	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::initialize()
	{
		m_scene3D = std::make_shared<Scene3D>();
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::update()
	{
		trace(Engine::GetInstance()->getSceneManager()->getScene3D()->getRootNode());
	}

	void SceneManager::trace(std::shared_ptr<Node3D> node)
	{
		if (!node)
		{
			return;
		}
		for (const auto& componet : node->get_components())
		{
			Visual3DComponent* visual3DComponent = dynamic_cast<Visual3DComponent*>(componet.get());

			if (!visual3DComponent)
			{
				continue;
			}
			Engine::GetInstance()->getGraphicsSystem()->commitVisual3D(visual3DComponent->get_visual3D());
		}
		for (const auto& node : node->get_childs())
		{
			trace(node);
		}	
	}
}