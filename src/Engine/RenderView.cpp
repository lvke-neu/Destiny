#include "RenderView.h"
#include "Editor/MainWindow.h"

namespace Destiny
{
	RenderView::RenderView(int argc, char* argv[]) : m_application(argc, argv), m_mainWindow(std::make_shared<MainWindow>())
	{
		
	}

	RenderView::~RenderView()
	{

	}

	void RenderView::initialize()
	{
		m_mainWindow->show();
		m_application.exec();
	}

	void RenderView::uninitialize()
	{

	}
}