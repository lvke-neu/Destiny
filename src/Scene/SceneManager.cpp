#include "SceneManager.h"
#include "Graphics/Visual3D.h"

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
		m_visual3D->draw();
	}
}