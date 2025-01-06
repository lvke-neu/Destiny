#pragma once
#include "Meta.h"

namespace Destiny
{
	class Node;
}

class ViewPortPanel : public Meta
{
public:
	ViewPortPanel();
public:
	void update();
public slots:
	void onChoosedNode(void* parameter);
private:
	void onViewPortResize();
	void processGzimo();
private:
	unsigned int m_width  = 0;
	unsigned int m_height = 0;

	int m_gzimoType;
	int m_gzimoMode;
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
