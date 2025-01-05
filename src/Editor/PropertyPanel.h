#pragma once
#include "Meta.h"

namespace Destiny
{
	class Node;
	class Object;
}

class PropertyPanel : public Meta
{
public:
	PropertyPanel();
public:
	void update();
public slots:
	void onChoosedNode(void* parameter);
private:
	void reflect(std::shared_ptr<Destiny::Object> object);
private:
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
