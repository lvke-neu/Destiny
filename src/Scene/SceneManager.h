#pragma once
#include <memory>

namespace Destiny
{
	class Scene;
	class Node;
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
	public:
		void initialize();
		void uninitialize();
		void update(float deltaTime);
	private:
		void bfs(std::shared_ptr<Node> node);
	private:
		std::shared_ptr<Scene> m_scene;
	};

}