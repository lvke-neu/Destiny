#pragma once
#include "Meta.h"

namespace Destiny
{
	class Node;
}

class ViewPortPanel : public Meta
{
public:
	enum Signal
	{
		ChoosedNode
	};
public:
	ViewPortPanel();
public:
	void update();
	void setGzimoType(int gzimoType);
	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
public slots:
	void onChoosedNode(void* parameter);
private:
	void processEvent();
	void processGzimo();
private:
	unsigned int m_width  = 0;
	unsigned int m_height = 0;

	int m_gzimoType;
	int m_gzimoMode;
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
