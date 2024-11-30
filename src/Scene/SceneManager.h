#pragma once
#include <memory>

namespace Destiny
{
	class Scene;
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
	public:
		void initialize();
		void uninitialize();
		void update(float deltaTime);
	public:
		std::shared_ptr<Scene> getScene();
	private:
		std::shared_ptr<Scene> m_scene;
	};

	inline std::shared_ptr<Scene> SceneManager::getScene()
	{
		return m_scene;
	}
}