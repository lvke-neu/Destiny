#pragma once
#include <memory>

namespace Destiny
{
	class Node;
}
class Application;
class ScenePanel
{
public:
	ScenePanel(Application* application);
public:
	void update();
private:
	void bfs(std::shared_ptr<Destiny::Node> node);
private:
	Application*	m_application;
};
