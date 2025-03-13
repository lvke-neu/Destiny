#include "Application.h"
#include "ViewPortPanel.h"
#include "ScenePanel.h"
#include "PropertyPanel.h"
#include "AssetPanel.h"
#include "DeferredRenderDebugPanel.h"
#include "GraphicsStatPanel.h"
#include "FileDialog.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/EventSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/VisualScene.h"
#include "Scene/SceneManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "Engine/Serializer.h"
#include "Engine/UnSerializer.h"
#include "Engine/FileSystem.h"
#include "Engine/Blob.h"

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
		Destiny::Engine::GetInstance()->getGraphicsSystem()->onResize_(LOWORD(lParam), HIWORD(lParam));
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
	m_hInstance(nullptr),
	m_viewPortPanel(std::make_shared<ViewPortPanel>()),
	m_scenePanel(std::make_shared<ScenePanel>()),
	m_propertyPanel(std::make_shared<PropertyPanel>()),
	m_assetPanel(std::make_shared<AssetPanel>()),
	m_deferredRenderDebugPanel(std::make_shared<DeferredRenderDebugPanel>()),
	m_graphicsStatPanel(std::make_shared<GraphicsStatPanel>())
{
	AllocConsole();
	MetaConnect(m_scenePanel, ScenePanel::ChoosedNode, m_propertyPanel, &PropertyPanel::onChoosedNode);
	MetaConnect(m_scenePanel, ScenePanel::ChoosedNode, m_viewPortPanel, &ViewPortPanel::onChoosedNode);
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
			//engine render
			Destiny::Engine::GetInstance()->update();

			//imgui render
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();	

			drawDock();
			
			//static bool show = true;
			//ImGui::ShowDemoWindow(&show);
			//ImGui::ShowMetricsWindow();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			//present
			Destiny::Engine::GetInstance()->getGraphicsSystem()->present();
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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	
	auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader("builtin://");
	if (blobLoader)
	{
		auto blobHolder1 = blobLoader->createBlobHolder("builtin://fonts/opensans/OpenSans-Bold.ttf");
		auto blobHolder2 = blobLoader->createBlobHolder("builtin://fonts/opensans/OpenSans-Regular.ttf");
		
		if (blobHolder1)
		{
			io.Fonts->AddFontFromFileTTF(blobLoader->normalizedPath(blobHolder1).c_str(), 20);
		}

		if (blobHolder2)
		{
			io.FontDefault = io.Fonts->AddFontFromFileTTF(blobLoader->normalizedPath(blobHolder2).c_str(), 20);
		}
	}

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

void Application::drawDock()
{
	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
// In this specific demo, we are not using DockSpaceOverViewport() because:
// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
// - we allow the host window to have padding (when opt_padding == true)
// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
// TL;DR; this demo is more complicated than what you would normally use.
// If we removed all the options we are showcasing, this demo would become:
//     void ShowExampleAppDockSpace()
//     {
//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
//     }

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	static bool p_open = true;
	ImGui::Begin("DockSpace", &p_open, window_flags);
	if (!opt_padding)
	{
		ImGui::PopStyleVar();
	}
		

	if (opt_fullscreen)
	{
		ImGui::PopStyleVar(2);
	}
		

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	//menu bar
	if (ImGui::BeginMenuBar())
	{
		if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_N)) ||
			(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_N))
			)
		{
			newScene();
		}
		if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_S)) ||
			(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_S))
			)
		{
			saveScene();
		}
		if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_O)) ||
			(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_O))
			)
		{
			openScene();
		}
		if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_C)) ||
			(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_C))
			)
		{
			Destiny::Engine::GetInstance()->getSceneManager()->setCurrentScene(nullptr);
		}
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				openScene();
			}
			if (ImGui::MenuItem("New", "CTRL+N"))
			{
				newScene();
			}
			if (ImGui::MenuItem("Save", "CTRL+S"))
			{
				saveScene();
			}
			if (ImGui::MenuItem("Clear", "CTRL+C"))
			{
				Destiny::Engine::GetInstance()->getSceneManager()->setCurrentScene(nullptr);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}



	m_viewPortPanel->update();
	m_scenePanel->update();
	m_propertyPanel->update();
	m_assetPanel->update();
	m_deferredRenderDebugPanel->update();
	m_graphicsStatPanel->update();

	ImGui::End();
}

void Application::newScene()
{
	auto scene = std::make_shared<Destiny::VisualScene>("New Scene");
	scene->initialize();
	Destiny::Engine::GetInstance()->getSceneManager()->setCurrentScene(scene);
	m_propertyPanel->onChoosedNode(nullptr);
}

void Application::saveScene()
{
	auto scene = Destiny::Engine::GetInstance()->getSceneManager()->getCurrentScene();
	if (scene)
	{
		std::string scenePath = "builtin://scene";
		auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(scenePath.c_str());
		scenePath = blobLoader->normalizedPath(blobLoader->createBlobHolder(scenePath));

		scenePath = FileDialog::SaveFile(scenePath.c_str(), "Scene Files (*.scene)\0*.scene\0");
		if (scenePath.empty())
		{
			return;
		}

		std::string sceneContent;
		Destiny::Serializer::Serialize(sceneContent, scene);
		std::shared_ptr<Destiny::Blob> blob = std::make_shared<Destiny::Blob>(sceneContent.size());
		blob->copyfrom((void*)sceneContent.data(), sceneContent.length());

		Destiny::FileSystem::WriteBlob(scenePath.c_str(), blob);
	}
}

void Application::openScene()
{
	std::string scenePath = "builtin://scene";
	auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(scenePath.c_str());
	scenePath = blobLoader->normalizedPath(blobLoader->createBlobHolder(scenePath));

	scenePath = FileDialog::OpenFile(scenePath.c_str(), "Scene Files (*.scene)\0*.scene\0");
	if (scenePath.empty())
	{
		return;
	}

	
	auto blob = Destiny::FileSystem::ReadBlob(scenePath.c_str());
	if (!blob)
	{
		return;
	}
	std::string sceneContent((char*)blob->getData(), blob->getLength());
	std::shared_ptr<Destiny::Object> object = nullptr;
	Destiny::UnSerializer::UnSerialize(object, sceneContent);

	auto scene = std::dynamic_pointer_cast<Destiny::Scene>(object);
	if (scene)
	{
		scene->initialize();
		Destiny::Engine::GetInstance()->getSceneManager()->setCurrentScene(scene);
	}
}
