#pragma once
#include <memory>

namespace Destiny
{
	class Node;
}
class StatPanel
{
public:
	StatPanel();
public:
	void update();
	void setChoosedNode(std::shared_ptr<Destiny::Node> choosedNode);
private:
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
