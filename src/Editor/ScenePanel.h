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
	void update();
private:
	void traversal(std::shared_ptr<Destiny::Node> node);
private:

};
