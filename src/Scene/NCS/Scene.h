#pragma once
#include <memory>

namespace Destiny
{
	class Node;
	class Scene
	{
	public:
		Scene();
		~Scene();
	public:
		void initialize();
		void uninitialize();
		void update(float deltaTime);
	public:
		std::shared_ptr<Node> getRootNode();
	private:
		std::shared_ptr<Node> m_rootNode;
	};

	inline std::shared_ptr<Node> Scene::getRootNode()
	{
		return m_rootNode;
	}
}