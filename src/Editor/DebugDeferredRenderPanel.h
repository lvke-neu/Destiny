#pragma once
#include "Meta.h"

class DebugDeferredRenderPanel : public Meta
{
public:
	DebugDeferredRenderPanel();
public:
	void update();
	void setOpen(bool open);
private:
	bool m_open;
};
