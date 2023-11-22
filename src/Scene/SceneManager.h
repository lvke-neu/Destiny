#pragma once
#include <memory>

namespace Destiny
{
	class Scene3D;
	class Visual3D;
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
	public:
		void initialize();
		void uninitialize();
		void update();
		std::shared_ptr<Scene3D> getScene3D();

		//TODO:remove
		void additionalCommands();

	private:
		std::shared_ptr<Visual3D> m_visual3D;
		std::shared_ptr<Scene3D> m_scene3D;
	};

	inline std::shared_ptr<Scene3D> SceneManager::getScene3D()
	{
		return m_scene3D;
	}
}