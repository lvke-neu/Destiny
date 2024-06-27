#include "Application.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Engine/Detail/BuiltinResourceBlobLoader.h"
#include "Graphics/GraphicsSystem.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return true;
	}
		
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

	initImGui();
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

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			static bool show = true;
			ImGui::ShowDemoWindow(&show);
			ImGui::ShowMetricsWindow();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	}
}

void Application::uninitialize()
{
	Destiny::Engine::GetInstance()->uninitialize();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(m_hwnd);
	UnregisterClassW(L"Destiny", m_hInstance);
}

void Application::initImGui()
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontFromFileTTF(Destiny::BuiltinResourceBlobLoader::getFullPath("builtin://fonts/opensans/OpenSans-Bold.ttf").c_str(), 20);
	io.FontDefault = io.Fonts->AddFontFromFileTTF(Destiny::BuiltinResourceBlobLoader::getFullPath("builtin://fonts/opensans/OpenSans-Regular.ttf").c_str(), 20);

	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	ImGui_ImplWin32_Init(m_hwnd);
	auto graphicsSystem = Destiny::Engine::GetInstance()->getGraphicsSystem();
	ImGui_ImplDX11_Init(graphicsSystem->getDevice(), graphicsSystem->getImmediateContext());
}
