#pragma once
#include <memory>

namespace Destiny
{
	class VisualScene;
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
	public:
		void							initialize();
		void							uninitialize();
		void							update(float deltaTime);
	public:
		std::shared_ptr<VisualScene>	getScene();
		std::shared_ptr<VisualScene>	getScene2();
	private:
		std::shared_ptr<VisualScene>	m_scene;
		std::shared_ptr<VisualScene>	m_scene2;
	};

	inline std::shared_ptr<VisualScene> SceneManager::getScene()
	{
		return m_scene;
	}

	inline std::shared_ptr<VisualScene> SceneManager::getScene2()
	{
		return m_scene2;
	}
}