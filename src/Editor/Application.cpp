#include "Application.h"
#include "MainWindow.h"

Application::Application(int argc, char* argv[]) : QApplication(argc, argv), m_mainWindow(std::make_shared<MainWindow>())
{
	m_mainWindow->show();
}
