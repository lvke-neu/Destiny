#pragma once
#include "Meta.h"

class DebugShadowMapPanel : public Meta
{
public:
	DebugShadowMapPanel();
public:
	void update();
	void setOpen(bool open);
private:
	bool m_open;
};
