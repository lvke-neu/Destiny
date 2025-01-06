#pragma once
#include "Meta.h"

namespace Destiny
{
	class Node;
}

class ScenePanel : public Meta
{
public:
	enum Signal
	{
		ChoosedNode
	};
public:
	ScenePanel();
public:
	void update();
private:
	void traversal(std::shared_ptr<Destiny::Node> node);
private:
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
