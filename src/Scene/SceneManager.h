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
		void							setCurrentScene(std::shared_ptr<Scene> currentScene);
	public:
		std::shared_ptr<Scene>			getCurrentScene();
	private:
		std::shared_ptr<Scene>			m_currentScene;
	};

	inline std::shared_ptr<Scene> SceneManager::getCurrentScene()
	{
		return m_currentScene;
	}
}