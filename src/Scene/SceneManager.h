#pragma once
#include <memory>

namespace Destiny
{
	class Node3D;
	class Scene3D;
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
	private:
		void trace(std::shared_ptr<Node3D> node);
	private:
		std::shared_ptr<Scene3D> m_scene3D;
	};

	inline std::shared_ptr<Scene3D> SceneManager::getScene3D()
	{
		return m_scene3D;
	}
}