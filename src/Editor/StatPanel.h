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
public slots:
	void onChoosedNode(void* parameter);
private:
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
