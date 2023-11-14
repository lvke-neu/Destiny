#include "SceneManager.h"
#include "Graphics/Visual3D.h"
#include "Graphics/GraphicsSystem.h"
#include "Engine/Engine.h"
#include <memory>

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
		//m_visual3D = std::make_shared<Visual3D>();
		//Engine::GetInstance()->getGraphicsSystem()->addCommandList(m_visual3D->getCommmandList());
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::update()
	{
		//m_visual3D->draw();
	}
}