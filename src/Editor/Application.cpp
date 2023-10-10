#include "Application.h"
#include "RenderWindow.h"

Application::Application(int argc, char* argv[]) : QApplication(argc, argv), m_renderWindow(std::make_shared<RenderWindow>())
{
	m_renderWindow->show();
}
