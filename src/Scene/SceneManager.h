#pragma once
#include <memory>
#include <vector>

namespace Destiny
{
	class Scene;
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
		std::shared_ptr<Scene>			getCurrentScene();
		void							setCurrentScene(std::shared_ptr<Scene> currentScene);
	private:
		std::shared_ptr<Scene>			m_currentScene;
	};

	inline std::shared_ptr<Scene> SceneManager::getCurrentScene()
	{
		return m_currentScene;
	}
}