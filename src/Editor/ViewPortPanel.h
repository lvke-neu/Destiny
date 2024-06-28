#pragma once

class ViewPortPanel
{
public:
	void update();
private:
	void onViewPortResize();
private:
	unsigned int m_width  = 0;
	unsigned int m_height = 0;
};
