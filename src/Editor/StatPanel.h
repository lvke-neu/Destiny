#pragma once
#include "Meta.h"

namespace Destiny
{
	class Node;
}
class StatPanel : public Meta
{
public:
	StatPanel();
public:
	void update();
	void setChoosedNode(std::shared_ptr<Destiny::Node> choosedNode);
public:
	void onChoosedNode();
private:
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
