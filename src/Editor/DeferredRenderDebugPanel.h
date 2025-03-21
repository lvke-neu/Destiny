#pragma once
#include "Meta.h"

class DeferredRenderDebugPanel : public Meta
{
public:
	DeferredRenderDebugPanel();
public:
	void update();
	void setOpen(bool open);
private:
	bool m_open;
};
