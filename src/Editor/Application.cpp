#include "Application.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		return 0;
		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
	{
		// Save the new client area dimensions.
		Destiny::WindowResizeData windowResizeData;
		windowResizeData.width = LOWORD(lParam);
		windowResizeData.height = HIWORD(lParam);
		Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::WindowResize, &windowResizeData);
		return 0;
	}

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		return 0;
	case WM_EXITSIZEMOVE:
		return 0;
		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		return 0;



	//Mouse
	Destiny::Mouse mouse;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	{
		mouse.code = (Destiny::MouseCode)wParam;
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN)
		{
			Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MousePressed, &mouse);
		}
		if (msg == WM_LBUTTONUP || msg == WM_RBUTTONUP)
		{
			Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MouseReleased, &mouse);
		}
		if (msg == WM_MOUSEMOVE)
		{
			Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MouseMoved, &mouse);
		}
		return 0;
	}

	//Keyboard
	Destiny::KeyCode keyCode;
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		keyCode = (Destiny::KeyCode)wParam;
		if (msg == WM_KEYDOWN)
		{
			Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::KeyPressed, &keyCode);
		}
		if (msg == WM_KEYUP)
		{
			Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::KeyReleased, &keyCode);
		}
		return 0;
	}

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Application::Application() : 
	m_hwnd(nullptr),
	m_hInstance(nullptr)
{
	AllocConsole();
}

void Application::initialize(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Destiny";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return ;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	const unsigned int width = 1000;
	const unsigned int height = 500;

	RECT R = { 0, 0, width, height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	m_hwnd = CreateWindow(L"Destiny", L"Destiny", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!m_hwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return ;
	}

	Destiny::Engine::GetInstance()->initialize({ (long long)m_hwnd, 4 });

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
}

void Application::run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Destiny::Engine::GetInstance()->update();
		}
	}
}

void Application::uninitialize()
{
	Destiny::Engine::GetInstance()->uninitialize();
	DestroyWindow(m_hwnd);
	UnregisterClassW(L"Destiny", m_hInstance);
}
