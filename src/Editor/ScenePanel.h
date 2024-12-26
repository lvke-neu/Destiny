#pragma once
#include "Meta.h"

namespace Destiny
{
	class Node;
}
class Application;
class ScenePanel : public Meta
{
public:
	enum Signal
	{
		ChoosedNode
	};
public:
	ScenePanel(Application* application);
public:
	void update();
private:
	void traversal(std::shared_ptr<Destiny::Node> node);
private:
	Application*	m_application;
};
