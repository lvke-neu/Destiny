#include "SceneManager.h"
#include "Graphics/Visual3D.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	SceneManager::SceneManager() :
		m_visual3D(nullptr)
	{

	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::initialize()
	{
		m_visual3D = std::make_shared<Visual3D>();
		
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::update()
	{
		Engine::GetInstance()->getGraphicsSystem()->commitVisual3D(m_visual3D);
	}
}