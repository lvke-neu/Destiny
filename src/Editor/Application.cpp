#include "Application.h"
#include "MainWindow.h"
#include "Engine/Engine.h"
#include "Engine/RenderView.h"

namespace Destiny
{
	Application::Application(int argc, char* argv[]) : QApplication(argc, argv), m_mainWindow(std::make_shared<MainWindow>())
	{
		Engine::GetInstance()->initialize();
		m_mainWindow->show();
	}

	Application::~Application()
	{
		Engine::GetInstance()->uninitialize();
	}
}