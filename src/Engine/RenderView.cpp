#include "RenderView.h"
#include "Editor/MainWindow.h"

namespace Destiny
{
	RenderView::RenderView(int argc, char* argv[]) : m_mainWindow(nullptr), m_application(argc, nullptr)
	{
		
	}

	RenderView::~RenderView()
	{

	}

	void RenderView::initialize()
	{
		m_mainWindow = std::make_shared<MainWindow>();
		m_mainWindow->show();
	}

	void RenderView::uninitialize()
	{

	}

	void RenderView::run()
	{
		m_application.exec();
	}
}