#pragma once
#include <Windows.h>

class Application
{
public:
	Application();
public:
	void initialize(HINSTANCE hInstance);
	void run();
	void uninitialize();
private:
	void initImGui();
private:
	HWND		m_hwnd;
	HINSTANCE	m_hInstance;
};

	